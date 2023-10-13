// -----------------------------------------------------------------------------
// $Id$
//
// Copyright(c) 2020 Renesas Electronics Corporation
// Copyright(c) 2020 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// -----------------------------------------------------------------------------

#include "adc_u2b.h"
//Constructor of ADC class
Cadc_u2b::Cadc_u2b(sc_module_name name, unsigned int adc_instance):
         Cadc_u2b_regif((std::string)name, 32)

        //trigger input port
        ,ADmSG0TRG("ADmSG0TRG")
        ,ADmSG1TRG("ADmSG1TRG")
        ,ADmSG2TRG("ADmSG2TRG")
        ,ADmSG3TRG("ADmSG3TRG")
        ,ADmSG4TRG("ADmSG4TRG")
        ,ADmSGDTRG("ADmSGDTRG")
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
        ,ANm82("ANm82")
        ,ANm83("ANm83")
        ,ANm90("ANm90")
        ,ANm91("ANm91")
        ,ANm92("ANm92")
        ,ANm93("ANm93")
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
        ,INT_SGDm("INT_SGDm")
        ,ADPEm("ADPEm")
        //data output port
        ,ADENDm0("ADENDm0")
        ,ADENDm1("ADENDm1")
        ,ADENDm2("ADENDm2")
        ,ADENDm3("ADENDm3")
        ,ADENDm4("ADENDm4")
        ,INT_ERRm("INT_ERRm")
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
    Cadc_u2b_regif::set_instance_name(this->name());

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
    mIsPReset = false;
    mIsADReset = false;
    mAvrefh = 3.3;
    mAVcc = 3.3;
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
    mTargetSGD_0 = 0;
    mTargetSGD_1 = 0;
    mPCCR0_finish = false;
    mSgDiagStop = 0;
    mStrongPullDown[0] = 0;
    mStrongPullDown[1] = 0;
    mStrongPullDown[2] = 0;
    mLastTargetChannel = false;
    THzAssignChannel[0] = 1;   // Assign T&H0 to ANm01
    THzAssignChannel[1] = 2;   // Assign T&H1 to ANm02
    THzAssignChannel[2] = 3;   // Assign T&H2 to ANm03
    THzAssignChannel[3] = 29;  // Assign T&H3 to ANm71
    THzAssignChannel[4] = 30;  // Assign T&H4 to ANm72
    THzAssignChannel[5] = 31;  // Assign T&H5 to ANm73
    mAVccMax = 3.6;
    mAVccMin = 3;
    mAvrefhMin = 3;
    mADCNum = 3;
    mPhyChannelNum = 40;
    mVirChannelNum = 96;
    mMPXInput = 0;
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
    INT_ERRm.initialize(0);
    INT_SGDm.initialize(0);
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
            case 34:
                otp.set_sensitivity(&ANm82);
                break;
            case 35:
                otp.set_sensitivity(&ANm83);
                break;
            case 36:
                otp.set_sensitivity(&ANm90);
                break;
            case 37:
                otp.set_sensitivity(&ANm91);
                break;
            case 38:
                otp.set_sensitivity(&ANm92);
                break;
            case 39:
                otp.set_sensitivity(&ANm93);
                break;
            default:
                break;
        }
        sc_core::sc_spawn(sc_bind(&Cadc_u2b::THzMethod, this, i), sc_core::sc_gen_unique_name("THz_Method"), &otp);
    }
    
    // Method for HW trigger select of ADCA
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
        sc_core::sc_spawn(sc_bind(&Cadc_u2b::TriggerADCAMethod, this, i), sc_core::sc_gen_unique_name("TriggerADCA_Method"), &otp);
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

    SC_METHOD(ScanGroupDiagSGSTTriggerMethod);
    dont_initialize();
    sensitive << mWritingSGSTCREvent[emGroupSGDiag];

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

    SC_METHOD(ScanGroupDiagSoftwareTriggerMethod);
    dont_initialize();
    sensitive << mSGSTTriggerEvent[emGroupSGDiag];

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

    SC_METHOD(ADmSGDTRGMethod);
    dont_initialize();
    sensitive << ADmSGDTRG;

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

//destructor of Cadc_u2b class
Cadc_u2b::~Cadc_u2b()
{//{{{
    delete mTIMER[0];
    delete mTIMER[1];
}//}}}

//virtual channel end notice thread
void Cadc_u2b::VCENDNoticeThread(void)
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
void Cadc_u2b::BoundaryExcessThread(void)
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
void Cadc_u2b::ScanningThread(void)
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
                        if(mIsSuspendSGDiag) {
                            group_number = i;
                            mIsSuspendSGDiag = false;
                        } else {
                            group_number = emGroupNum - i;
                        }
                        if(mIsScanningProcess[group_number]) {
                            if (group_number == emGroupSGDiag) {
                                re_printf("info","The SG-Diag starts the scanning process.\n");
                            } else {
                                re_printf("info","The group %d starts the scanning process.\n", group_number);
                            }
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
                            }else if (group_number == emGroupSGDiag) {
                                is_multiple_mode = true;
                                group_loop = 1;
                                if ((unsigned int)(*SGDIAGCR)["SDPS"] == 1) {
                                    mStartVCPointer[group_number] = 0;
                                    mEndVCPointer[group_number] = 0;
                                } else {
                                    mStartVCPointer[group_number] = 0;
                                    mEndVCPointer[group_number] = 2;
                                }
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

                                        vc_add_number = VCAddition(vc_number, group_number);

                                        unsigned int data[4] = {0,0,0,0};
                                        // RH850_U2B added wait insertion for virtual channel (not start virtual channel of scanning group)
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
                                            if(vc_number < mVirChannelNum) {
                                                if (group_number != emGroupSGDiag) {
                                                    re_printf("info","The VC %d of group %d starts the conversion.\n", vc_number, group_number);
                                                } else {
                                                    re_printf("info","The VC %d of SG-Diag starts the conversion.\n", vc_number);
                                                }
                                            }
                                            //3.2.0.Assert INT_MPXm, ADENDmx, MPXCURR
                                            ExportData(emStartConvert, (unsigned int)(group_number), vc_number, data, group_loop);

                                            for(unsigned int vc_add = 0; vc_add < vc_add_number; vc_add++){
                                                double delay_time = ADCTimeCalculation(is_start_SG, vc_number, vc_add, group_number);
                                                if (!mTHAHoldingProcess && !mTHBHoldingProcess) {
                                                    // wait tSPL + tWAITTIME
                                                    wait((unsigned int)delay_time, SC_NS, mSuspendEvent | mDisableEvent | mADResetEvent | mZeroClockEvent);
                                                } else {
                                                    mSuspendFlag = true;
                                                }
                                                //3.2.1.Check the reset function
                                                if(mResetFlag){
                                                    throw vpcl::emRstException;
                                                }
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
                                                        if (group_number != emGroupSGDiag) { 
                                                            re_printf("info","The current scanning process of group %d is suspended in asynchronous mode.\n", group_number);
                                                            mSuspendVC[group_number]  = vc_number;
                                                            mLastVC = vc_number;
                                                        } else {
                                                            mIsSuspendSGDiag = true;
                                                            re_printf("info","The current scanning process of SG-Diag0 is suspended in asynchronous mode.\n");
                                                            mSuspendVC[group_number]  = 0;
                                                            mLastVC = 0;
                                                        }
                                                        mLastGroup = (unsigned int)(group_number);
                                                        is_suspend_immediate = true;
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
                                                        if (group_number != emGroupSGDiag) {
                                                            re_printf("info","The current scanning process of group %d is suspended in asynchronous mode.\n", group_number);
                                                            mSuspendVC[group_number]  = vc_number;
                                                            mLastVC = vc_number;
                                                        } else {
                                                            re_printf("info","The current scanning process of SG-Diag is suspended in asynchronous mode.\n");
                                                            mIsSuspendSGDiag = true;
                                                            mSuspendVC[group_number]  = 0;
                                                            mLastVC = 0;
                                                        }
                                                        is_suspend_immediate = true;
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
                                                        if (group_number != emGroupSGDiag) {
                                                            re_printf("info","The VC %d of group %d finishes the conversion.\n", vc_number, group_number);
                                                        } else {
                                                            re_printf("info","The VC %d of SG-Diag finishes the conversion.\n", vc_number);
                                                        }
                                                    }
                                                    if(end_vc == vc_number){
                                                        is_final_vc = true;
                                                        if(mSuspendFlag){
                                                            mSuspendVC[group_number] = start_vc;
                                                        }
                                                        if (group_number != emGroupSGDiag) {
                                                            re_printf("info","The group %d finishes the conversion for all VCs.\n", group_number);
                                                        } else {
                                                            re_printf("info","The SG-Diag finishes the conversion for all VCs.\n");
                                                        }
                                                    }
                                                    if (group_number != emGroupSGDiag) {
                                                        if(mSuspendFlag && ((!is_multiple_mode) || (end_vc != vc_number) || (group_loop > 1))){
                                                            re_printf("info","The current scanning process of group %d is suspended in synchronous mode.\n", group_number);
                                                        }
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
                                    if(mDisableFlag || mDisableOneSGFlag || mSuspendFlag || (mPCLKPeriod == 0) || (mADCLKPeriod == 0)) {
                                        break; //break out of for(vc_number ...)
                                    }
                                }//end for(unsigned int vc_num = start_vc)

                                //3.3.Check to continue the current group or not
                                if(mDisableFlag || mDisableOneSGFlag || is_suspend_immediate || (mSuspendFlag && (!is_final_vc)) || (mPCLKPeriod == 0) || (mADCLKPeriod == 0)) {
                                    if(mDisableOneSGFlag) {
                                        mDisableOneSGFlag = false;
                                        mGroupSuspendState[group_number] = false;
                                        (*SGSR[group_number])["SGACT"] = 0;
                                        (*SGSTR)["SGACT"] =(unsigned int)((*SGSTR)["SGACT"]) & (~(0x1 << group_number));
                                        re_printf("info","The group %d finishes the scanning process.\n", group_number);
                                        mIsScanningProcess[group_number] = false;
                                        mSGActive[group_number] = false;
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
                                            if (group_number != emGroupSGDiag) {
                                                (*SGSR[group_number])["SGACT"] = 0;
                                                (*SGSTR)["SGACT"] =(unsigned int)( (*SGSTR)["SGACT"]) & (~(0x1 << group_number));
                                                re_printf("info","The group %d finishes the scanning process.\n", group_number);
                                            } else {
                                                (*SGDIAGSR)["SDACT"] = 0;
                                                (*SGSTR)["SDACT"] = 0;
                                                re_printf("info","The SD-Diag finishes the scanning process.\n");
                                            }
                                            mStrongPullDown[0] = 0;
                                            mStrongPullDown[1] = 0;
                                            mStrongPullDown[2] = 0;
                                            mIsScanningProcess[group_number] = false;
                                            mSGActive[group_number] = false;
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
                        if(mSuspendFlag || mDisableFlag || (mPCLKPeriod == 0) || (mADCLKPeriod == 0)) {
                            mSuspendFlag = false;
                            break; //break out of for(unsigned int group_number.. )
                        }
                    }//end for(unsigned int group_number.. )
                    if ((mPCLKPeriod == 0) || (mADCLKPeriod == 0)) {
                        break;  //break out of scanning process --> while (is_active)
                    }
                    is_active = false;
                    if(mDisableFlag){
                        for(unsigned int group = 0; (group < emGroupNum); group++){
                            if (mDisableSGxFlag[group]){
                                mSGActive[group] = false;
                                if(group != emGroupSGDiag) {
                                    (*SGSR[group])["SGACT"] = 0;
                                    (*SGSTR)["SGACT"] = (unsigned int)((*SGSTR)["SGACT"]) & (~(0x1 << group));
                                } else {
                                    (*SGDIAGSR)["SDACT"] = 0;
                                    (*SGSTR)["SDACT"] = 0;
                                }
                                mIsScanningProcess[group] = false;
                                mGroupSuspendState[group] = false;
                                mDisableSGxFlag[group] = false;
                            }
                            is_active |= mSGActive[group];
                        }
                        mStrongPullDown[0] = 0;
                        mStrongPullDown[1] = 0;
                        mStrongPullDown[2] = 0;
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
void Cadc_u2b::ADCStateMethod(void)
{//{{{
    if((!mIsADReset) && (mADCInstance < mADCNum) && (mPCLKPeriod > 0) && (mADCLKPeriod > 0)) {
        bool scanning_circuit_state = false;
        bool th_circuit_state = false;
        mADCChangeState = false;
        for(unsigned int group_number = 0; group_number < emGroupNum; group_number++){
            if (group_number != emGroupSGDiag) {
                if(((*SGSR[group_number])["ADTACT"] == 1) || (mIsScanningProcess[group_number])){
                    scanning_circuit_state = true;
                }
            } else if (((*SGDIAGSR)["SDACT"] == 1) || (mIsScanningProcess[group_number])) {
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
                    if (mADCInstance != 4) {
                        re_printf("info","The ADC%d moves from IDLE state to SCANNING state.\n", mADCInstance);
                    } else {
                        re_printf("info","The ADCA moves from IDLE state to SCANNING state.\n");
                    }
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
                    if (mADCInstance != 4) {
                        re_printf("info","The ADC%d moves from SCANNING state to IDLE state.\n", mADCInstance);
                    } else {
                        re_printf("info","The ADCA moves from SCANNING state to IDLE state.\n");
                    }
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
void Cadc_u2b::THCircuitMethod(void)
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
#if 1
                    if((unsigned int)((*TDCR)["THSDE"]) == 1) {
                        mIntTHSample[TH_channel] = SelfDiagData(THzAssignChannel[TH_channel],tdlv);
                    }
#else
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
#if 1
                    if((unsigned int)((*TDCR)["THSDE"]) == 1) {
                        mIntTHSample[TH_channel] = SelfDiagData(THzAssignChannel[TH_channel],tdlv);
                    }
#else
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
#if 1
                        if((unsigned int)((*TDCR)["THSDE"]) == 1) {
                            mIntTHSample[TH_channel] = SelfDiagData(THzAssignChannel[TH_channel],tdlv);
                        }
#else
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
        mADCStateEvent.notify();
    }
}//}}}

void Cadc_u2b::THAHoldingEndMethod(void)
{//{{{
    if(!mIsADReset && (mADCInstance < mADCNum)){
        re_printf("info","The holding process of group A is finished.\n");
        if (!mTHBHoldingProcess) {
            mScanningEvent.notify();
        }
        mTHAHoldingProcess = false;
        HardwareTrigger(mGroupANumber, 1);
    }
}//}}}

void Cadc_u2b::THBHoldingEndMethod(void)
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

void Cadc_u2b::THzMethod(const unsigned int index)
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

void Cadc_u2b::cb_ADENDP_ENDP (RegCBstr str) 
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

void Cadc_u2b::cb_THSMPSTCR_SMPST (RegCBstr str) 
{//{{{
    if (mADCInstance == 4) {
        re_printf("warning","T&H is not supported in ADCA. Writing into THSMPSTCR register is not affected\n");
        (*THSMPSTCR) = str.pre_data;
        return;
    } else {
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
            if ((*THBCR)["HLDTE"]  == 1
                    &&(*THBCR)["HLDCTE"] == 1
                    &&(*SGCR[(*THBCR)["SGS"] + 1])["TRGMD"] != 1) {
                re_printf("warning","Sampling process in T&H circuit is not triggered because SGCR%d.TRGMD != 1.\n", (unsigned int)(*THBCR)["SGS"] + 1);
                return;
            }
            if ((*THACR)["HLDTE"] == 1
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
        }
    }
}//}}}

void Cadc_u2b::cb_THSTPCR_THSTP (RegCBstr str) //stop T&H circuit
{//{{{
    if (mADCInstance == 4) {
        re_printf("warning","T&H is not supported in ADCA. Writing into THSTPCR register is not affected.\n");
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

void Cadc_u2b::cb_THCR_ASMPMSK (RegCBstr str) //automatic sampling mask
{//{{{
    if (mADCInstance == 4) {
        re_printf("warning","T&H is not supported in ADCA. Writing into THCR register is not affected.\n");
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
}//}}}

void Cadc_u2b::cb_THAHLDSTCR_HLDST (RegCBstr str) //trigger holding group A
{//{{{
    if (mADCInstance == 4) {
        re_printf("warning","T&H is not supported in ADCA. Writing into THAHLDSTCR register is not affected.\n");
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

void Cadc_u2b::cb_THBHLDSTCR_HLDST (RegCBstr str) //trigger holding group B
{//{{{
    if (mADCInstance == 4) {
        re_printf("warning","T&H is not supported in ADCA. Writing into THBHLDSTCR register is not affected.\n");
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
}//}}}

void Cadc_u2b::cb_THACR_SGS (RegCBstr str)
{//{{{
    if (mADCInstance == 4) {
        re_printf("warning","T&H is not supported in ADCA. Writing into THACR register is not affected.\n");
        (*THACR) = str.pre_data;
        return;
    } else {
//        unsigned int ther = (unsigned int)(*THER);
//        unsigned int thgsr = (unsigned int)(*THGSR);
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

void Cadc_u2b::cb_THBCR_SGS (RegCBstr str)
{//{{{
    if (mADCInstance == 4) {
        re_printf("warning","T&H is not supported in ADCA. Writing into THBCR register is not affected.\n");
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
}//}}}

void Cadc_u2b::cb_THER_TH0E (RegCBstr str)
{//{{{
    if (mADCInstance == 4) {
        re_printf("warning","T&H is not supported in ADCA. Writing into THER register is not affected.\n");
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

void Cadc_u2b::cb_THGSR_TH0GS (RegCBstr str)
{//{{{
    if (mADCInstance == 4) {
        re_printf("warning","T&H is not supported in ADCA. Writing into THGSR register is not affected.\n");
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
}//}}}

void Cadc_u2b::cb_MPXCMDR_MPXCMD(RegCBstr str)
{//{{{
    // do nothing
}//}}}

void Cadc_u2b::cb_VCLMINTER1_ADUL00IE(RegCBstr str)
{//{{{
    if (!AllSGStopCheck()) {
        re_printf("warning", "Writing into VCLMINTER1 register is not affected.\n");
        (*VCLMINTER1) = str.pre_data;
    }
}//}}}

void Cadc_u2b::cb_VCLMINTER2_ADUL32IE(RegCBstr str)
{//{{{
    if (!AllSGStopCheck()) {
        re_printf("warning", "Writing into VCLMINTER2 register is not affected.\n");
        (*VCLMINTER2) = str.pre_data;
    }
}//}}}

#if 0
void Cadc_u2b::cb_VCLMINTER3_ADUL64IE(RegCBstr str)
{//{{{
    if (!AllSGStopCheck()) {
        re_printf("warning", "Writing into VCLMINTER3 register is not affected.\n");
        (*VCLMINTER3) = str.pre_data;
    }
}//}}}
#endif

void Cadc_u2b::cb_SGTSEL_TxSEL0(RegCBstr str)
{//{{{
    if ((mADCInstance == 0) || (mADCInstance == 1) || (mADCInstance == 2) || (mADCInstance == 3)) {
        re_printf("warning","SGTSEL%d is supported in ADCA only. Writing into SGTSEL%d register is not affected.\n",str.channel,str.channel);
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
                for(unsigned int group_number = 0; group_number < emGroupNum-1; group_number++){
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

void Cadc_u2b::ScanGroup0SGSTTriggerMethod(void)
{//{{{
    double start_delay = 0;
    if (mEnableTimeCalculation) {
        start_delay = emtDCoeffADCLK * mADCLKPeriod;
    } else {
        start_delay = mtD;
    }
    if(!mIsADReset && (mADCInstance < mADCNum)){
        if(!CheckScanMode(emGroup0,false,true)){
            mSGSTTriggerEvent [emGroup0].notify(start_delay,SC_NS);
        }
    }
}//}}}

void Cadc_u2b::ScanGroup1SGSTTriggerMethod(void)
{//{{{
    double start_delay = 0;
    if (mEnableTimeCalculation) {
        start_delay = emtDCoeffADCLK * mADCLKPeriod;
    } else {
        start_delay = mtD;
    }
    if(!mIsADReset && (mADCInstance < mADCNum)){
        if(!CheckScanMode(emGroup1,false,true)){
            mSGSTTriggerEvent [emGroup1].notify(start_delay,SC_NS);
        }
    }
}//}}}

void Cadc_u2b::ScanGroup2SGSTTriggerMethod(void)
{//{{{
    double start_delay = 0;
    if (mEnableTimeCalculation) {
        start_delay = emtDCoeffADCLK * mADCLKPeriod;
    } else {
        start_delay = mtD;
    }
    if(!mIsADReset && (mADCInstance < mADCNum)){
        if(!CheckScanMode(emGroup2,false,true)){
            mSGSTTriggerEvent [emGroup2].notify(start_delay,SC_NS);
        }
    }
}//}}}

void Cadc_u2b::ScanGroup3SGSTTriggerMethod(void)
{//{{{
    double start_delay = 0;
    if (mEnableTimeCalculation) {
        start_delay = emtDCoeffADCLK * mADCLKPeriod;
    } else {
        start_delay = mtD;
    }
    if(!mIsADReset && (mADCInstance < mADCNum)){
        if(!CheckScanMode(emGroup3,false,true)){
            mSGSTTriggerEvent [emGroup3].notify(start_delay,SC_NS);
        }
    }
}//}}}

void Cadc_u2b::ScanGroup4SGSTTriggerMethod(void)
{//{{{
    double start_delay = 0;
    if (mEnableTimeCalculation) {
        start_delay = emtDCoeffADCLK * mADCLKPeriod;
    } else {
        start_delay = mtD;
    }
    if(!mIsADReset && (mADCInstance < mADCNum)){
        if(!CheckScanMode(emGroup4,false,true)){
            mSGSTTriggerEvent [emGroup4].notify(start_delay,SC_NS);
        }
    }
}//}}}

void Cadc_u2b::ScanGroupDiagSGSTTriggerMethod(void)
{//{{{
    double start_delay = 0;
    if (mEnableTimeCalculation) {
        start_delay = emtDCoeffADCLK * mADCLKPeriod;
    } else {
        start_delay = mtD;
    }
    if(!mIsADReset && (mADCInstance < mADCNum)){
        if(!CheckScanMode(emGroupSGDiag,false,true)){
            mSGSTTriggerEvent[emGroupSGDiag].notify(start_delay,SC_NS);
        }
    }
}//}}}

void Cadc_u2b::ScanGroup0ADSYNTriggerMethod(void)
{//{{{
    double start_delay = 0;
    if (mEnableTimeCalculation) {
        start_delay = emtDCoeffADCLK * mADCLKPeriod;
    } else {
        start_delay = mtD;
    }
    if(!mIsADReset && (mADCInstance < mADCNum)){
        if(!CheckScanMode(emGroup0,false,true,true)){
            mADSYNTriggerEvent [emGroup0].notify(start_delay,SC_NS);
        }
    }
}//}}}

void Cadc_u2b::ScanGroup1ADSYNTriggerMethod(void)
{//{{{
    double start_delay = 0;
    if (mEnableTimeCalculation) {
        start_delay = emtDCoeffADCLK * mADCLKPeriod;
    } else {
        start_delay = mtD;
    }
    if(!mIsADReset && (mADCInstance < mADCNum)){
        if(!CheckScanMode(emGroup1,false,true,true)){
            mADSYNTriggerEvent [emGroup1].notify(start_delay,SC_NS);
        }
    }
}//}}}

void Cadc_u2b::ScanGroup2ADSYNTriggerMethod(void)
{//{{{
    double start_delay = 0;
    if (mEnableTimeCalculation) {
        start_delay = emtDCoeffADCLK * mADCLKPeriod;
    } else {
        start_delay = mtD;
    }
    if(!mIsADReset && (mADCInstance < mADCNum)){
        if(!CheckScanMode(emGroup2,false,true,true)){
            mADSYNTriggerEvent [emGroup2].notify(start_delay,SC_NS);
        }
    }
}//}}}

void Cadc_u2b::ScanGroup3ADSYNTriggerMethod(void)
{//{{{
    double start_delay = 0;
    if (mEnableTimeCalculation) {
        start_delay = emtDCoeffADCLK * mADCLKPeriod;
    } else {
        start_delay = mtD;
    }
    if(!mIsADReset && (mADCInstance < mADCNum)){
        if(!CheckScanMode(emGroup3,false,true,true)){
            mADSYNTriggerEvent [emGroup3].notify(start_delay,SC_NS);
        }
    }
}//}}}

void Cadc_u2b::ScanGroup4ADSYNTriggerMethod(void)
{//{{{
    double start_delay = 0;
    if (mEnableTimeCalculation) {
        start_delay = emtDCoeffADCLK * mADCLKPeriod;
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
void Cadc_u2b::ScanGroup0SoftwareTriggerMethod(void)
{//{{{
    if(!mIsADReset && (mADCInstance < mADCNum)){
        GroupCheck (emGroup0);
    }
}//}}}

void Cadc_u2b::ScanGroup1SoftwareTriggerMethod(void)
{//{{{
    if(!mIsADReset && (mADCInstance < mADCNum)){
        GroupCheck (emGroup1);
    }
}//}}}

void Cadc_u2b::ScanGroup2SoftwareTriggerMethod(void)
{//{{{
    if(!mIsADReset && (mADCInstance < mADCNum)){
        GroupCheck (emGroup2);
    }
}//}}}

void Cadc_u2b::ScanGroup3SoftwareTriggerMethod(void)
{//{{{
    if(!mIsADReset && (mADCInstance < mADCNum)){
        GroupCheck (emGroup3);
    }
}//}}}

void Cadc_u2b::ScanGroup4SoftwareTriggerMethod(void)
{//{{{
    if(!mIsADReset && (mADCInstance < mADCNum)){
        GroupCheck (emGroup4);
    }
}//}}}

void Cadc_u2b::ScanGroupDiagSoftwareTriggerMethod(void)
{//{{{
    if(!mIsADReset && (mADCInstance < mADCNum)){
        GroupCheck (emGroupSGDiag);
    }
}//}}}

//external input port method
void Cadc_u2b::ADmSG0TRGMethod(void)
{//{{{
    if ((!mIsADReset) && (mADCInstance < mADCNum) && (ADmSG0TRG.read() == 1)) {
        if ((mPCLKPeriod > 0) && (mADCLKPeriod > 0)) {
            HardwareTrigger(emGroup0,3);  //group 0, trigger feature 3
        } else {
            re_printf("warning","Input port is triggered while clock frequency is 0.\n");
        }
    }
}//}}}

void Cadc_u2b::ADmSG1TRGMethod(void)
{//{{{
    if ((!mIsADReset) && (mADCInstance < mADCNum) && (ADmSG1TRG.read() == 1)) {
        if ((mPCLKPeriod > 0) && (mADCLKPeriod > 0)) {
            HardwareTrigger(emGroup1,3); //group 1, trigger feature 3
        } else {
            re_printf("warning","Input port is triggered while clock frequency is 0.\n");
        }
    }
}//}}}

void Cadc_u2b::ADmSG2TRGMethod(void)
{//{{{
    if ((!mIsADReset) && (mADCInstance < mADCNum) && (ADmSG2TRG.read() == 1)) {
        if ((mPCLKPeriod > 0) && (mADCLKPeriod > 0)) {
            HardwareTrigger(emGroup2,3); //group 2, trigger feature 3
        } else {
            re_printf("warning","Input port is triggered while clock frequency is 0.\n");
        }
    }
}//}}}

void Cadc_u2b::ADmSG3TRGMethod(void)
{//{{{
    if ((!mIsADReset) && (mADCInstance < mADCNum) && (ADmSG3TRG.read() == 1)) {
        if ((mPCLKPeriod > 0) && (mADCLKPeriod > 0)) {
            HardwareTrigger(emGroup3,3); //group 3, trigger feature 3
        } else {
            re_printf("warning","Input port is triggered while clock frequency is 0.\n");
        }
    }
}//}}}

void Cadc_u2b::ADmSG4TRGMethod(void)
{//{{{
    if ((!mIsADReset) && (mADCInstance < mADCNum) && (ADmSG4TRG.read() == 1)) {
        if ((mPCLKPeriod > 0) && (mADCLKPeriod > 0)) {
            HardwareTrigger(emGroup4,3); //group 4, trigger feature 3
        } else {
            re_printf("warning","Input port is triggered while clock frequency is 0.\n");
        }
    }
}//}}}

void Cadc_u2b::ADmSGDTRGMethod(void)
{//{{{
    if ((!mIsADReset) && (mADCInstance < mADCNum) && (ADmSGDTRG.read() == 1)) {
        if ((mPCLKPeriod > 0) && (mADCLKPeriod > 0)) {
            mSGDiagFlag = true;
            HardwareTrigger(emGroupSGDiag,5); //group SG-Diag, trigger feature 5
        } else {
            re_printf("warning","Input port is triggered while clock frequency is 0.\n");
        }
    }
}//}}}

//counting end method
void Cadc_u2b::ScanGroup3ADtimerTriggerMethod(void)
{//{{{
    if ((!mIsADReset) && (mADCInstance < mADCNum) && (mPCLKPeriod > 0) && (mADCLKPeriod > 0)) {
        re_printf("info","The counting of ADtimer 3 is overflow.\n");
        if(!CheckIsSVMode()){
            HardwareTrigger(emGroup3,0);//group 3, trigger feature 0
        }
    }
}//}}}

void Cadc_u2b::ScanGroup4ADtimerTriggerMethod(void)
{//{{{
    if ((!mIsADReset) && (mADCInstance < mADCNum) && (mPCLKPeriod > 0) && (mADCLKPeriod > 0)) {
        re_printf("info","The counting of ADtimer 4 is overflow.\n");
        if(!CheckIsSVMode()){
            HardwareTrigger(emGroup4,0);//group 4, trigger feature 0
        }
    }
}//}}}

//interrupt ouput method
void Cadc_u2b::INT_MPXmInterruptMethod(void)
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

void Cadc_u2b::ADMPXOutputMethod(void)
{//{{{
    if(mIsPReset){
        MPXCURm.write(0);
        re_printf("info","MPXCUR ports are reset to initial value.\n");
    }else {
        MPXCURm.write((unsigned int)(*MPXCURR1)["MPXCUR"]);
        re_printf("info","MPXCUR ports change value to 0x%X.\n", (unsigned int)(*MPXCURR1)["MPXCUR"]);
    }
}//}}}

void Cadc_u2b::ADPEmInterruptMethod(void)
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

void Cadc_u2b::INT_ADmxINT_ADEmDataMethod(void)
{//{{{
    unsigned int dfent = 0x0;
    unsigned int dftag = 0x0;
#if 0
    unsigned int gtmentsgxe = 0x0;
#else
    // RH850_U2B added
    unsigned int asentsgxe = 0x0;
#endif
    unsigned int data = 0x0;
    if(mIsPReset){
        //1.INT_AD
        INT_ADm0.write(0);
        INT_ADm1.write(0);
        INT_ADm2.write(0);
        INT_ADm3.write(0);
        INT_ADm4.write(0);
        INT_SGDm.write(0);
        DumpInterruptMessage("INT_AD", "0", "is reset to 0");
        DumpInterruptMessage("INT_AD", "1", "is reset to 0");
        DumpInterruptMessage("INT_AD", "2", "is reset to 0");
        DumpInterruptMessage("INT_AD", "3", "is reset to 0");
        DumpInterruptMessage("INT_AD", "4", "is reset to 0");
        DumpInterruptMessage("INT_SD", "", "is reset to 0");
        //INT_ULm.write(0);
        //DumpInterruptMessage("INT_UL", "", "is reset to 0");
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
        }else if (mINT_ADmxActive[emGroupSGDiag]){
            mINT_SDmActiveNum++;
            DumpInterruptMessage("INT_SD", "", "Assert");
        }
        INT_ADm0.write(mINT_ADmxActive[emGroup0]);
        INT_ADm1.write(mINT_ADmxActive[emGroup1]);
        INT_ADm2.write(mINT_ADmxActive[emGroup2]);
        INT_ADm3.write(mINT_ADmxActive[emGroup3]);
        INT_ADm4.write(mINT_ADmxActive[emGroup4]);
        INT_SGDm.write(mINT_ADmxActive[emGroupSGDiag]);

        //2.INT_ADEm
        if((mErrorId & 0x1) == emIDError){
            if(mFinalVC < mVirChannelNum){
                if (mFinalGroup != emGroupSGDiag) {
                    if((*DIR[mFinalVC])["IDEF"] == 1){
                        (*DIR[mFinalVC])["IDEF"] = 0;
                    }
                } else {
                    if((*SGDIAGDIR[mFinalVC])["IDEF"] == 1){
                        (*SGDIAGDIR[mFinalVC])["IDEF"] = 0;
                    }
                }
            }
            if(mFinalVC == emVcPWM && mFinalGroup == emGroup4 && (*PWDDIR)["IDEF"] == 1){
                (*PWDDIR)["IDEF"] = 0;
            }
            if((*IDER)["IDE"] == 0){
                if (mFinalGroup != emGroupSGDiag) {
                    (*IDER)["IDECAP"] = mFinalVC;
                } else {
                    (*IDER)["IDECAP"] = mFinalVC+0x78;
                }
                (*IDER)["IDE"] = 1;
            }
            if(mFinalVC == emVcPWM && mFinalGroup == emGroup4) {
                re_printf("info","The id error occurs in group%d.PWM-Diag.\n", mFinalGroup);
            }
            if(mFinalVC < mVirChannelNum){
                if (mFinalGroup != emGroupSGDiag) {
                    re_printf("info","The id error occurs in group%d.VC%d.\n", mFinalGroup, mFinalVC );
                } else {
                    re_printf("info","The id error occurs in SG-Diag VC%d.\n", mFinalVC );
                }
            }
        } else {
            if(mFinalVC == emVcPWM && mFinalGroup == emGroup4) {
                (*PWDDIR)["IDEF"] = 1;
            }
            if(mFinalVC < mVirChannelNum){
                if (mFinalGroup != emGroupSGDiag) {
                    (*DIR[mFinalVC])["IDEF"] = 1;
                } else {
                    (*SGDIAGDIR[mFinalVC])["IDEF"] = 1;
                }
            }
            (*IDER)["IDE"] = 0;
        }
        if((mErrorId & 0x4) == emOverwriteError && mFinalVC < mVirChannelNum){
            if((*OWER)["OWE"] == 0){
                (*OWER)["OWECAP"] = mFinalVC;
                (*OWER)["OWE"] = 1;
            }
            if (mFinalGroup != emGroupSGDiag) {
                re_printf("info","The overwrite error occurs in group%d.VC%d.\n", mFinalGroup, mFinalVC);
            } else {
                re_printf("info","The overwrite error occurs in SG-Diag VC%d.\n", mFinalVC);
            }
        }
        if((mErrorId & 0x8) == emBoundaryError){
            if(mFinalVC == emVcPWM && mFinalGroup == emGroup4) {
                (*PWVCLMSR)["PWVCLMS"] = 1;
                re_printf("info","The boundary error occurs in group%d.PWM-Diag.\n", mFinalGroup);
            }
            if(mFinalVC < mVirChannelNum){
                if (mFinalGroup != emGroupSGDiag) {
                    SetULErrorStatus(mFinalVC,false);
                    re_printf("info","The boundary error occurs in group%d.VC%d.\n", mFinalGroup, mFinalVC);
                } else {
                    SetULErrorStatus(mFinalVC,true);
                    re_printf("info","The boundary error occurs in SG-Diag VC%d.\n", mFinalVC);
                }
            }
            if((ULIntrEnable(mFinalVC,false) && (mFinalVC < mVirChannelNum) && (mFinalGroup != emGroupSGDiag))
            ||  (ULIntrEnable(mFinalVC,true) && (mFinalVC < mVirChannelNum) && (mFinalGroup == emGroupSGDiag))
               ||((*PWVCLMINTER)["PWADULIE"] == 1 && mFinalVC == emVcPWM && mFinalGroup == emGroup4)){
                //INT_ULm.write(1);
#if 0
                mINT_ULmActiveNum++;
                mINT_ULmActive = true;
                DumpInterruptMessage("INT_UL", "", "Assert");
#else
                mINT_ERRmActive = true;
                mINT_ERRmEvent.notify();
#endif
            }
            if (mFinalGroup != emGroupSGDiag) {
                CheckULCR(false);
            } else {
                CheckULCR(true);
            }
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
        if(!mINT_ERRmProcess) {
            if((mErrorId & ((unsigned int)(*SFTCR)) & 0x05) != 0){
                mINT_ERRmActive = true;
                mINT_ERRmEvent.notify();
            }
        } else {
            re_printf("info","INT_ERRm is in process.\n");
        }
#endif
        if (mFinalGroup != emGroupSGDiag) {
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

        if((mFinalVC < mVirChannelNum) && (mFinalGroup != emGroupSGDiag)) {
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
            if (mFinalGroup != emGroupSGDiag) {
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
            } else {
                if (mFinalVC == 0) {
                    (*SGDIAGDR01)["SGDIAGDR0"] = mFinalData;
                } else if (mFinalVC == 1) {
                    (*SGDIAGDR01)["SGDIAGDR1"] = mFinalData;
                } else { //if (mFinalVC == 2) {
                    (*SGDIAGDR2)["SGDIAGDR2"] = mFinalData;
                }
                (* SGDIAGDIR[mFinalVC])["SGDIAGDRk"]  = mFinalData;
                (* SGDIAGDIR[mFinalVC])["WFLAG"] = 1;
                (* SGDIAGDIR[mFinalVC])["ID"]   = mID;
                (* SGDIAGDIR[mFinalVC])["PRTY"] = prty;
                re_printf("info","The SGDIAGDR%d bit, SGDIAGDIR%d register are updated new data.\n", mFinalVC, mFinalVC);
                if ((*SGDIAGCR)["SDPS"] == 0) {
                    if (mFinalVC == 1) {
                        if (!PinLevelSGDiagCheck()) {
                            (*SGDIAGSRR)["SRR0"] = mFinalDataSRR0;
                            re_printf("info","The SGDIAGSRR.SRR0 bit is updated new data.\n");
                        } else {
                            re_printf("info","The SGDIAGSRR.SRR0 bit retains the previous value because SG-Diag is in pin level self-diagnosis mode.\n");
                        }
                    } else if (mFinalVC == 2){
                        if (!PinLevelSGDiagCheck()) {
                            (*SGDIAGSRR)["SRR1"] = mFinalDataSRR1;
                            re_printf("info","The SGDIAGSRR.SRR1 bit is updated new data.\n");
                        } else {
                            re_printf("info","The SGDIAGSRR.SRR1 bit retains the previous value because SG-Diag is in pin level self-diagnosis mode.\n");
                        }
                    }
                }
            }
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
        if (mFinalGroup != emGroupSGDiag) {
            dfent = mFinalDfent;
            dftag = mFinalDftag;
#if 0
            gtmentsgxe = ((unsigned int)(*GTMENTSGER) >> mFinalGroup) & 0x1;
#else
            asentsgxe = ((unsigned int)(*DFASENTSGER) >> mFinalGroup) & 0x1;
#endif
            data = mFinalData;
            if (mFinalDfmt == 2 || mFinalDfmt == 3) {
                data = data >> 1;
            }
#if 0
            if (dfent == 1 && gtmentsgxe == 1)
#else
            if (dfent == 1 && asentsgxe == 1)
#endif
            {
                if (mFinalDfmt == 1 || mFinalDfmt >= 4) {
                    re_printf("error", "When using ASF, setting to the signed 12 bit integer point format is prohibited.\n"); // RH850_E2x SARAD enhancement
                }
                DFREQm.write(1);
                DFTAGm.write(dftag);
                DFDATAm.write(data);
            }
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
        } else if (mINT_ADmxActive[emGroupSGDiag]) {
            DumpInterruptMessage("INT_SD", "", "Deassert");
            mINT_ADmxActive[emGroupSGDiag] = false;
        }
        INT_ADm0.write(mINT_ADmxActive[emGroup0]);
        INT_ADm1.write(mINT_ADmxActive[emGroup1]);
        INT_ADm2.write(mINT_ADmxActive[emGroup2]);
        INT_ADm3.write(mINT_ADmxActive[emGroup3]);
        INT_ADm4.write(mINT_ADmxActive[emGroup4]);
        INT_SGDm.write(mINT_ADmxActive[emGroupSGDiag]);

        PVCR_ENDm.write(0);
        mErrorId = 0;
        mIsUpperError = false;
        mIsLowerError = false;
        mFinalPhyChannel = 0;

#if 0
        if(mINT_ULmActive){
            DumpInterruptMessage("INT_UL", "", "Deassert");
            mINT_ULmActive = false;
            //INT_ULm.write(0);
        }
#endif

        DFREQm.write(0);
    }
}//}}}

void Cadc_u2b::ADENDm0Method(void)
{//{{{
    if(mIsADReset){
        ADENDm0.write(0);
        if (mADCInstance == 4) {
            re_printf("info","The ADENDA0 output is reset to 0.\n");
        } else {
            re_printf("info","The ADEND%d0 output is reset to 0.\n", mADCInstance);
        }
    }else{
        if(mADENDmxActive[emGroup0]){
            if (mADCInstance == 4) {
                re_printf("info","The ADENDA0 output is set to 1.\n");
            } else {
                re_printf("info","The ADEND%d0 output is set to 1.\n", mADCInstance);
            }
            ADENDm0.write(1);
        }else{
            if (mADCInstance == 4) {
                re_printf("info","The ADENDA0 output is cleared to 0.\n");
            } else {
                re_printf("info","The ADEND%d0 output is cleared to 0.\n", mADCInstance);
            }
            ADENDm0.write(0);
        }
    }
}//}}}

void Cadc_u2b::ADENDm1Method(void)
{//{{{
    if(mIsADReset){
        ADENDm1.write(0);
        if (mADCInstance == 4) {
            re_printf("info","The ADENDA1 output is reset to 0.\n");
        } else {
            re_printf("info","The ADEND%d1 output is reset to 0.\n", mADCInstance);
        }
    }else{
        if(mADENDmxActive[emGroup1]){
            if (mADCInstance == 4) {
                re_printf("info","The ADENDA1 output is set to 1.\n");
            } else {
                re_printf("info","The ADEND%d1 output is set to 1.\n", mADCInstance);
            }
            ADENDm1.write(1);
        }else{
            if (mADCInstance == 4) {
                re_printf("info","The ADENDA1 output is cleared to 0.\n");
            } else {
                re_printf("info","The ADEND%d1 output is cleared to 0.\n", mADCInstance);
            }
            ADENDm1.write(0);
        }
    }
}//}}}

void Cadc_u2b::ADENDm2Method(void)
{//{{{
    if(mIsADReset){
        ADENDm2.write(0);
        if (mADCInstance == 4) {
            re_printf("info","The ADENDA2 output is reset to 0.\n");
        } else {
            re_printf("info","The ADEND%d2 output is reset to 0.\n", mADCInstance);
        }
    }else{
        if(mADENDmxActive[emGroup2]){
            if (mADCInstance == 4) {
                re_printf("info","The ADENDA2 output is set to 1.\n");
            } else {
                re_printf("info","The ADEND%d2 output is set to 1.\n", mADCInstance);
            }
            ADENDm2.write(1);
        }else{
            if (mADCInstance == 4) {
                re_printf("info","The ADENDA2 output is cleared to 0.\n");
            } else {
                re_printf("info","The ADEND%d2 output is cleared to 0.\n", mADCInstance);
            }
            ADENDm2.write(0);
        }
    }
}//}}}

void Cadc_u2b::ADENDm3Method(void)
{//{{{
    if(mIsADReset){
        ADENDm3.write(0);
        if (mADCInstance == 4) {
            re_printf("info","The ADENDA3 output is reset to 0.\n");
        } else {
            re_printf("info","The ADEND%d3 output is reset to 0.\n", mADCInstance);
        }
    }else{
        if(mADENDmxActive[emGroup3]){
            if (mADCInstance == 4) {
                re_printf("info","The ADENDA3 output is set to 1.\n");
            } else {
                re_printf("info","The ADEND%d3 output is set to 1.\n", mADCInstance);
            }
            ADENDm3.write(1);
        }else{
            if (mADCInstance == 4) {
                re_printf("info","The ADENDA3 output is cleared to 0.\n");
            } else {
                re_printf("info","The ADEND%d3 output is cleared to 0.\n", mADCInstance);
            }
            ADENDm3.write(0);
        }
    }
}//}}}

void Cadc_u2b::ADENDm4Method(void)
{//{{{
    if(mIsADReset){
        ADENDm4.write(0);
        if (mADCInstance == 4) {
            re_printf("info","The ADENDA4 output is reset to 0.\n");
        } else {
            re_printf("info","The ADEND%d4 output is reset to 0.\n", mADCInstance);
        }
    }else{
        if(mADENDmxActive[emGroup4]){
            if (mADCInstance == 4) {
                re_printf("info","The ADENDA4 output is set to 1.\n");
            } else {
                re_printf("info","The ADEND%d4 output is set to 1.\n", mADCInstance);
            }
            ADENDm4.write(1);
        }else{
            if (mADCInstance == 4) {
                re_printf("info","The ADENDA4 output is cleared to 0.\n");
            } else {
                re_printf("info","The ADEND%d4 output is cleared to 0.\n", mADCInstance);
            }
            ADENDm4.write(0);
        }
    } 
}//}}}

void Cadc_u2b::IFCProcessMethod(void) 
{//{{{
    if(mIsPReset && (mADCInstance < mADCNum)){
        ADCm_IFDATA.write(0);
    }else{
        if (IFRDm.read() && IFREm.read()) {
            unsigned int vc_number = (unsigned int)IFVCHm.read();
            if (vc_number < mVirChannelNum) {
                unsigned int data = (unsigned int)(*DIR[vc_number])["DRn"];
                if ((*ADCR2)["DFMT"] == 1 || (*ADCR2)["DFMT"] >= 4) {
                    re_printf("error", "When using IFC, setting to the integer point format is prohibited.\n"); 
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

void Cadc_u2b::SVmModeMethod(void)
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

void Cadc_u2b::SVStopMethod(void)
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

void Cadc_u2b::RunRetainedTrigger(const unsigned int group_number)
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
                }else if(mIsRetainedSWTHB[group_number]) {
                    mIsRetainedSWTHB[group_number] = false;
                    if (((*THBCR)["HLDCTE"] == 1) && ((*THBCR)["HLDTE"] == 0)) {
                        mTHBSoftwareTriggerHolding = true;
                        mTHBSoftwareTriggerHoldingEvent.notify();
                    }
                }else {
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
            }else if(mIsRetainedHWTHB[group_number]) {
                mIsRetainedHWTHB[group_number] = false;
                if ((((*THBCR)["HLDCTE"] == 1)&&(*THBCR)["HLDTE"] == 1)&&((*THBCR)["SGS"] == (group_number - 1))) {
                    mTHBPortTriggerHolding = true;
                    mTHBPortTriggerHoldingEvent.notify();
                }
            } else{
                if (group_number != emGroupSGDiag) {
                    if ((mADCInstance == 4) && ((*SGTSEL[group_number]) != 0x10)) {
                        HardwareTrigger(group_number,4);
                    } else {
                        HardwareTrigger(group_number,3);
                    }
                } else {
                    HardwareTrigger(group_number,5);
                }
            }
        }
    }
}//}}}

//tlm_tgt_if function
bool Cadc_u2b::common_tgt_rd(bool is_dbg, unsigned int addr, unsigned char *p_data, unsigned int size )
{ //{{{
    sc_assert(p_data != NULL);
    if(is_dbg){
        // Set debug mode (member variables of Cadc_u2b_regif class)
        return reg_rd_dbg(addr, p_data, size);
    }else{
        bool is_read_success = true;
        unsigned int tmp_add = addr & 0x7FF;

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
            } else if (tmp_add == 0x264) {
                CallBack_SGDIAGDR01();
            } else if (tmp_add == 0x268) {
                CallBack_SGDIAGDR2();
            } else if ((0x404 <= tmp_add) && (tmp_add <= 0x40C)) {
                CallBack_SGDIAGDIRn(tmp_add);
            }
        }
        return is_read_success;
    }
} //}}}

bool Cadc_u2b::common_tgt_wr(bool is_dbg, unsigned int addr, unsigned char *p_data, unsigned int size )
{//{{{
    sc_assert(p_data != NULL);
    if(is_dbg){
        return reg_wr_dbg(addr, p_data, size);
    }else{
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

bool Cadc_u2b::ChkAccess (const bool is_wr, const std::string channel_name, const std::string register_name, const std::string bit_name)
{//{{{
    // This code is used if multiple product is used (eg: U2B, U2B, ...)
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
void Cadc_u2b::AssignTHCircuit(const unsigned int th_channel, const unsigned int group_channel, const unsigned int physic_channel)
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


void Cadc_u2b::CheckSGDiagAddition(void)
{//{{{
    bool isDiagMode = 0;
    if ((*SGDIAGCR)["SDPS"]==0) {
        for (unsigned int i = 0; i < emSGDchannel; i++) {
            if (((*SGDIAGVCR[i])["CNVCLS"] == 0x9) || ((*SGDIAGVCR[i])["CNVCLS"] == 0xA) || ((*SGDIAGVCR[i])["CNVCLS"] == 0xC) || ((*SGDIAGVCR[i])["CNVCLS"] == 0xD)) {
                isDiagMode = true;
                break;
            }
        }
        if ((isDiagMode) && ((*ODCR)["WADDE"] == 1)) {
            for (unsigned int i = 0; i <emSGDchannel; i++) {
                if ((*SGDIAGVCR[i])["CNVCLS"] == 0) {
                    (*SGDIAGVCR[i])["CNVCLS"] = 0x4;
                }
            }
        }
    }
}//}}}

//operation functions
void Cadc_u2b::HardwareTrigger (const unsigned int group_number, const unsigned int trigger)
{//{{{
    double start_delay = 0;
    if (mEnableTimeCalculation) {
        start_delay = emtDCoeffADCLK * mADCLKPeriod;
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
        else if(trigger == 2){
            //2.1.Trigger scanning process
            if(((*THBCR)["HLDCTE"] == 1)&&((*THBCR)["SGS"] == (group_number - 1))&&((*SGCR[group_number])["TRGMD"] == 1)){
                GroupCheck(group_number);
            }

        //3.External input port trigger : ADmSG0
        }
        else if((trigger == 3)&&(group_number == 0)){
            //3.1.Trigger scanning process
            if (mADCInstance == 4) {
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
            else if ((((*THBCR)["HLDCTE"] == 1)&&(*THBCR)["HLDTE"] == 1)&&((*THBCR)["SGS"] == (group_number - 1))) {
                if(!CheckScanMode(group_number,false,false,false,false,true)){
                    mTHBPortTriggerHolding = true;
                    mTHBPortTriggerHoldingEvent.notify();
                }
            //4.3.Trigger counting process or scanning process
            }
            else if ((((*THACR)["HLDCTE"] == 0) && ((*THBCR)["HLDCTE"] == 0)) ||
            (((*THACR)["HLDCTE"] == 0) && ((*THBCR)["SGS"] != (group_number - 1))) ||
            (((*THBCR)["HLDCTE"] == 0)&&((*THACR)["SGS"] != (group_number - 1))) 
            || (((*THBCR)["SGS"] != (group_number -1)) && ((*THACR)["SGS"] != (group_number -1)))
            ) {
                trigger_enable = true;
            }

            if(trigger_enable){
                if (mADCInstance == 4) {
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
        //5.External input port trigger ADCA
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
        //6.External input port trigger : SG-Diag
        else if((trigger == 5)&&(group_number == emGroupSGDiag)){
            if((*SGDIAGCR)["TRGMD"] == 1){
                if(!CheckScanMode(group_number)){
                    GroupCheck(group_number);
                }
            }
        }
    }
}//}}}

void Cadc_u2b::GroupCheck (const unsigned int group_number)
{//{{{
    unsigned int start_vc = 0;
    unsigned int end_vc   = 0;
    bool start_sgd = 0;

    if (mIsUpdateSGVCPR[group_number]) {
        mIsUpdateSGVCPR[group_number] = false;
    }
    //Check the VC condition
    if(mIsScanningProcess[group_number]) {
        if (group_number == emGroupSGDiag) {
            re_printf("warning","Trigger scanning process for SG-Diag is ignored because SG-Diag is in scanning process.\n");

        } else {
            re_printf("warning","Trigger scanning process for group %d is ignored because group %d is in scanning process.\n", group_number, group_number);
        }
        if((unsigned int)((*TOCCR)["TOCE"]) == 1){
            if (group_number != emGroupSGDiag) {
                (*TOCER)["TOCESG"] = (unsigned int)((*TOCER)["TOCESG"]) | (0x1 << group_number);
            } else {
                (*TOCER)["TOCESD"] = 0x1;
            }
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
        if (group_number == emGroupSGDiag) {
            re_printf("info","The SG-Diag is triggered for scanning process.\n");
            CheckSGDiagAddition();
            start_sgd = UpdatePhysicalChannelSGD(false);
        } else {
            re_printf("info","The group %d is triggered for scanning process.\n", group_number);
        }
        if(mPWMFlag && group_number == emGroup4){
            mPWMFlag = false;
            PVCR_VALUEmUpdate();
            start_vc = emVcPWM;
            end_vc = emVcPWM;
        } else if (group_number != emGroupSGDiag) {
            start_vc = (unsigned int)((*SGVCPR[group_number])["VCSP"]);
            end_vc   = (unsigned int)((*SGVCPR[group_number])["VCEP"]);
        }
        if (group_number == emGroupSGDiag) {
            if (start_sgd) {
                (*SGDIAGSR)["SDACT"] = 1;
                (*SGSTR)["SDACT"] = 1;
                if ((unsigned int)(*SGDIAGCR)["SDPS"] == 1) {
                    mStartVCPointer[group_number] = 0;
                    mEndVCPointer[group_number] = 0;
                } else {
                    mStartVCPointer[group_number] = 0;
                    mEndVCPointer[group_number] = 2;
                }
            }
        } else {
            (*SGSR[group_number])["SGACT"] = 1;
            (*SGSTR)["SGACT"] = (unsigned int)((*SGSTR)["SGACT"]) | (0x1 << group_number);
            mStartVCPointer[group_number]  = start_vc;
            mEndVCPointer[group_number]    = end_vc;
        }
        mIsScanningProcess[group_number] = true;
        mSGActive[group_number] = true;

        //Check scanningprocess
        if(mScanningState){
            if ((group_number != emGroupSGDiag) && ((mCurrentGroup < group_number) || (mCurrentGroup == emGroupSGDiag))) {
                mSuspendFlag = true;
                if(SuspendCheck(mCurrentGroup)){
                    mSuspendEvent.notify();
                }
            }
        }else{
            if (!mTHAHoldingProcess && !mTHBHoldingProcess) {
                if (((group_number == emGroupSGDiag) && start_sgd) || (group_number != emGroupSGDiag)) {
                    mScanningEvent.notify();
                }
            }
        }
    }
}//}}}

bool Cadc_u2b::VCCheck (const unsigned int vc_number)
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
    //unsigned int tmp_gctrl = gctrl & 0x1F;
    unsigned int tmp_gctrl = gctrl;
    bool is_correct = true;

    switch (cnvcls) {
        case emTHMode:
            if ((5 < gctrl) && (gctrl < 8)) {
                re_printf("warning","The setting of VCR%d.CNVCLS and VCR%d.GCTRL is not matched in the T&H conversion mode.\n", vc_number, vc_number);
                is_correct = false;
            } else {
                if((gctrl & 0x38) != 0){
                    re_printf("warning","The VCR%d.GCTRL[5:3] bits should be zero.\n", vc_number);
                }
            }
            break;
        //1.3 Get from diagnosis circuit
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

unsigned int Cadc_u2b:: VCAddition (const unsigned int vc_number, const unsigned int group_number)
{//{{{
    unsigned int cnvcls = 0;
    bool addnt = (bool)((*ADCR2)["ADDNT"]);
    unsigned int vc_add_number = 1;
    if(vc_number < mVirChannelNum){
        if (group_number != emGroupSGDiag) {
            cnvcls = (unsigned int)((*VCR[vc_number])["CNVCLS"]);
        } else {
            cnvcls = (unsigned int)((*SGDIAGVCR[vc_number])["CNVCLS"]);
        }
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

unsigned int Cadc_u2b::GetData (const unsigned int group_number, const unsigned int vc_number, const unsigned int vc_add)
{//{{{
    unsigned int cnvcls = 0;
    unsigned int gctrl  = 0;
    if(vc_number == emVcPWM && group_number == emGroup4) {
        cnvcls = emPWSDMode;
        gctrl  = (unsigned int)((*PWDVCR)["GCTRL"]);
    }
    if(vc_number < mVirChannelNum){
        if (group_number != emGroupSGDiag) {
            cnvcls = (unsigned int)((*VCR[vc_number])["CNVCLS"]);
            gctrl  = (unsigned int)((*VCR[vc_number])["GCTRL"]);
        } else {
            cnvcls = (unsigned int)((*SGDIAGVCR[vc_number])["CNVCLS"]);
        }
    }
    unsigned int thgsr  = (unsigned int)(*THGSR);
    double tmp_data = 0;
    unsigned int tdlv   = (unsigned int)(*TDCR)["TDLV"];
    //unsigned int index = gctrl & 0x1F;
    unsigned int index = gctrl;
    double data = 0;
    unsigned int dataSelfPath = 0;
    bool normal_convert = false;
    double ratio = 0;
    if (group_number != emGroupSGDiag) {
        mPhyChannel = index;
    } else {
        if (!mPCCR0_finish) {
            mPhyChannel = mTargetSGD_0-1;
        } else {
            mPhyChannel = emMaxSGDChannel+mTargetSGD_1-1;
        }
    }
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
        else if (cnvcls == emDisconnectionDiagMode || cnvcls == emPdDiagMode) {
            data = 0;
        }
        else if (cnvcls == emPuDiagMode) {
            data = mAVcc;
        }
        else {
            normal_convert = true;
        }
        if(normal_convert) {
            if (group_number != emGroupSGDiag) {
                data = LoadAnalogTerminal(index);
            } else {
                if (!mPCCR0_finish) {
                    data = LoadAnalogTerminal(mTargetSGD_0-1);
                } else {
                    data = LoadAnalogTerminal(emMaxSGDChannel+mTargetSGD_1-1);
                }
            }
            if (((index == emVCCPortNum)
                || (index == emEVCCPortNum)
                || (index == emISOVDDPortNum)
                || (index == emAWOVDDPortNum))
             && (mADCInstance == 0)) { // VCC, EVCC, VDD of RH850_E2x Enhancement
                data = AdjustVMONInput(data, index);
            }
        }
    } else if (cnvcls == emTHMode) {
        gctrl = gctrl & 0x7;
        thgsr = (thgsr >> (2*gctrl))&0x1;
            //check matching between T&H setting and VC setting
        if (mTHChannelState[gctrl] == emTHIdleState) {
            re_printf("warning","The TH channel %d is in IDLE state, input data is all 0.\n", gctrl);
            tmp_data = 0;
        } else if (((unsigned int)((*THACR)["SGS"]) == (unsigned int)((*THBCR)["SGS"])) || (group_number == 0) || ((thgsr == 0) && ((*THACR)["SGS"] != (group_number - 1))) ||
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
        mPhyChannel = mPhyChannelTH[gctrl];
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
        if (group_number != emGroupSGDiag) {
            data = SelfDiagData(index,tdlv);
        } else {
            if (!mPCCR0_finish) {
                data = SelfDiagData(mTargetSGD_0-1,tdlv);
            } else {
                data = SelfDiagData(emMaxSGDChannel + mTargetSGD_1-1,tdlv);
            }
        }
    //} else if (cnvcls == emDisconnectionDiagMode || cnvcls == emPdDiagMode) {
    //    data = 0;
    //} else if (cnvcls == emPuDiagMode) {
    //    data = mAVcc;
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
                #if 1  //update for HWM0.61
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

void Cadc_u2b::ADtimerCheck (const unsigned int group_number)
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

double Cadc_u2b::LoadAnalogTerminal (const unsigned int index)
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
            if (mStrongPullDown[0] && mForcedOpenError) {
                data = 0;
            } else {
                data = ANm12.read();
            }
            break;
        case 7:
            if (mStrongPullDown[1] && mForcedOpenError) {
                data = 0;
            } else {
                data = ANm13.read();
            }
            break;
        case 8:
            if (mStrongPullDown[2] && mForcedOpenError) {
                data = 0;
            } else {
                data = ANm20.read();
            }
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
        case 34:
            data = ANm82.read();
            break;
        case 35:
            data = ANm83.read();
            break;
        case 36:
            data = ANm90.read();
            break;
        case 37:
            data = ANm91.read();
            break;
        case 38:
            data = ANm92.read();
            break;
        case 39:
            data = ANm93.read();
            break;
        default:
            break;
    }

    return data;
}//}}}

double Cadc_u2b::AdjustVMONInput(const double data, const unsigned int index)
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

void Cadc_u2b::ExportData(const unsigned int case_number, const unsigned int group_number, const unsigned int vc_number, unsigned int *data, const unsigned int group_loop )
{//{{{
    unsigned int cnvcls = 0;
    unsigned int gctrl  = 0;
    unsigned int mpxe = 0;
    unsigned int mpxv  = 0;
    unsigned int dfent = 0;
    unsigned int dftag = 0;
    unsigned int vc_add_number = 0;
    unsigned int ullmtbs = 0;
    unsigned int sgdiag_ullmtbs0 = 0;
    unsigned int sgdiag_ullmtbs1 = 0;
    unsigned int ulmtb = 0;
    unsigned int llmtb = 0;
    int ulmtb_sgd = 0;
    int llmtb_sgd = 0;
    unsigned int convt_data = 0;
    int srr_data0 = 0;
    int srr_data1 = 0;
    bool sign_ulmtb_sgd = 0;
    bool sign_llmtb_sgd = 0;
    unsigned int pin_number = 0;
    double finish_delay = 0;
    unsigned int dfmt = (unsigned int)((*ADCR2)["DFMT"]);
    bool isUpperError = false;
    bool isLowerError = false;

    vc_add_number = VCAddition(vc_number, group_number);

    if(vc_number == emVcPWM && group_number == emGroup4) {
        cnvcls = emPWSDMode;
        gctrl  = (unsigned int)((*PWDVCR)["GCTRL"]);
        mpxv  = (unsigned int)((*PWDVCR)["MPXV"]);
        mpxe  = (unsigned int)((*PWDVCR)["MPXE"]);
    }
    if(vc_number < mVirChannelNum){
        if (group_number != emGroupSGDiag) {
            cnvcls = (unsigned int)((*VCR[vc_number])["CNVCLS"]);
            gctrl  = (unsigned int)((*VCR[vc_number])["GCTRL"]);
            mpxv  = (unsigned int)((*VCR[vc_number])["MPXV"]);
#if 0
            dfent = (unsigned int)((*VCR[vc_number])["GTMENT"]);
            dftag = (unsigned int)((*VCR[vc_number])["GTMTAG"]);
#else
            dfent = (unsigned int)((*VCR[vc_number])["DFENT"]);
            dftag = (unsigned int)((*VCR[vc_number])["DFTAG"]);
#endif
        } else {
            cnvcls = (unsigned int)((*SGDIAGVCR[vc_number])["CNVCLS"]);
        }
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
                (*MPXCURR1)["MPXCMD"] = (unsigned int)((*MPXCMDR)["MPXCMD"]);
                (*MPXCURR1)["MPXCUR"] = mpxv;
                (*MPXCURR2)["MPXCUR"] = mpxv;
                mADMPXOutputEvent.notify(); 
                //1.2.INT_MPX
                if (mIsINT_MPXmOperating) {
                    re_printf("warning","INT_MPX%d is not asserted because INT_MPX%d interrupt is in process.\n", mADCInstance, mADCInstance);
                } else if ((unsigned int)(*MPXINTER)["ADMPXIE"] == 1){ 
                    mIsINT_MPXmOperating = true;
                    mINT_MPXmActive = true;
                    mINT_MPXmInterruptEvent.notify();
                }
            }
            //2.ADENDmx
            if (group_number != emGroupSGDiag) {
                for (pin_number = 0; pin_number < emADENDPinNum; pin_number++) {
                    if((vc_number == (unsigned int)((*ADENDP[pin_number])["ENDP"]))
                            && ((mLastVC != vc_number) || (mLastGroup != group_number)))
                    {
                        mADENDmxActive[pin_number] = true;
                        mADENDmxEvent[pin_number].notify();
                    }
                }
            }
            if (mForcedOpenError) {
                if (cnvcls == emDisconnectionDiagMode || cnvcls == emPdDiagMode || cnvcls == emPuDiagMode) {
                    mOpenCircuitError = false;
                } else if (cnvcls != emDisconnectionMode) {
                    mOpenCircuitError = true;
                }
            }
            else {
                mOpenCircuitError = false;
            }
            break;
        case emCorruptConvert:
            //1.Clear ADENDmxEvent
            if (group_number != emGroupSGDiag) {
                for (pin_number = 0; pin_number <emADENDPinNum; pin_number++) {
                    if(vc_number == (unsigned int)((*ADENDP[pin_number])["ENDP"])) {
                        mADENDmxActive[pin_number] = false;
                        mADENDmxEvent[pin_number].notify();
                    }
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
            if (dfmt == 2) {
                mStoreData = mStoreData << 1;
            }
            #if 1 
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
            #if 1 
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
            mFinalVC = vc_number;
            mFinalGroup = mLastGroup;
            if(mTHUpdateChannelState[0] || mTHUpdateChannelState[1] || mTHUpdateChannelState[2] || mTHUpdateChannelState[3] ||
                    mTHUpdateChannelState[4] || mTHUpdateChannelState[5]){
                mTHUpdateChannelStateFlag = true;
                mTHUpdateChannelStateEvent.notify();
            }
            if (group_number !=  emGroupSGDiag) {
                //2.ADENDmx
                for (pin_number = 0; pin_number < emADENDPinNum; pin_number++) {
                    if((vc_number == (unsigned int)((*ADENDP[pin_number])["ENDP"]))
                            && (mSuspendFlag || (mStartVCPointer[group_number] != mEndVCPointer[group_number]) || (group_loop == 1 && (*SGCR[group_number])["SCANMD"] == 0))) {
                        mADENDmxActive[pin_number] = false;
                        mADENDmxEvent[pin_number].notify();
                    }
                }
            }
            //3.INT_ADmx, INT_ADEm, DRi, ,DIRn
            //3.1.Check ID error
            if(mForcedIdError){
                mErrorId = mErrorId | 0x1;
            }
            //3.2.Check Overwire error
            if(vc_number < mVirChannelNum){
                if (group_number != emGroupSGDiag) {
                    if((*DIR[vc_number])["WFLAG"] == 1){
                        mErrorId = mErrorId | 0x4;
                    }
                } else {
                    if((*SGDIAGDIR[vc_number])["WFLAG"] == 1){
                        mErrorId = mErrorId | 0x4;
                    }
                }
            }

            //3.3.Check boundary error
            if(vc_number == emVcPWM && group_number == emGroup4){
                ullmtbs = (unsigned int)((*PWDVCR)["VCULLMTBS"]);
            }
            if(vc_number < mVirChannelNum){
                if (group_number != emGroupSGDiag) {
                    ullmtbs = (unsigned int)((*VCR[vc_number])["VCULLMTBS"]);
                } else {
                    sgdiag_ullmtbs0 = (unsigned int)((*SGDIAGCR)["VCULLMTBS0"]);
                    sgdiag_ullmtbs1 = (unsigned int)((*SGDIAGCR)["VCULLMTBS1"]);
                }
            }
            if ((ullmtbs != 0) && (group_number != emGroupSGDiag)) {
                ulmtb = (unsigned int)(*VCULLMTBR[ullmtbs-1])["VCULMTB"];
                llmtb = (unsigned int)(*VCULLMTBR[ullmtbs-1])["VCLLMTB"];
                ulmtb = CheckVCULLMTBR(ullmtbs - 1, true, ulmtb, vc_add_number,false, false);
                llmtb = CheckVCULLMTBR(ullmtbs - 1, false, llmtb, vc_add_number,false, false);
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
                        if ((mULE_LPSmActive & 0x1F) == 0) {
                            mULE_LPSmActive = 1 << mFinalGroup;
                            mULE_LPSmEvent.notify((unsigned int)finish_delay, SC_NS);
                        }
                    }
                }
            } 
            if ((*SGDIAGCR)["SDPS"] == 0) {
                if (vc_number == 1) {
                    if (!PinLevelSGDiagCheck()) {
                        srr_data0 = (((unsigned int)(*SGDIAGDIR[0])["SGDIAGDRk"])&0x3FFF) - (convt_data&0x3FFF);
                        srr_data0 = 0xFFFF&CheckSSRFormat(srr_data0,vc_add_number);
                        mFinalDataSRR0 = (unsigned int)srr_data0;
                    } else {
                        srr_data0 = (*SGDIAGSRR)["SRR0"];
                    }
                }
                if (vc_number == 2) {
                    if (!PinLevelSGDiagCheck()) {
                        if ((*SGDIAGCR)["SMD"] == 0) {
                            srr_data1 = (0x3FFF&convt_data) - (((unsigned int)(*SGDIAGDIR[1])["SGDIAGDRk"])&0x3FFF);
                        } else {
                            srr_data1 = (((unsigned int)(*SGDIAGDIR[0])["SGDIAGDRk"])&0x3FFF) - (convt_data&0x3FFF);
                        }
                        srr_data1 = 0xFFFF&CheckSSRFormat(srr_data1,vc_add_number);
                        mFinalDataSRR1 = (unsigned int)srr_data1;
                    } else {
                        srr_data1 = (*SGDIAGSRR)["SRR1"];
                    }
                }
            }
            if ((sgdiag_ullmtbs0 != 0) && (group_number == emGroupSGDiag)) {
                ulmtb_sgd = (*VCULLMTBR[sgdiag_ullmtbs0-1])["VCULMTB"];
                llmtb_sgd = (*VCULLMTBR[sgdiag_ullmtbs0-1])["VCLLMTB"];
                sign_ulmtb_sgd = (ulmtb_sgd>>15)&0x1;
                sign_llmtb_sgd = (llmtb_sgd>>15)&0x1;
                if ((*SGDIAGCR)["SDPS"] == 0) {
                    ulmtb_sgd = (int)CheckVCULLMTBR(sgdiag_ullmtbs0 - 1, true, ulmtb_sgd, vc_add_number,true,sign_ulmtb_sgd);
                    llmtb_sgd = (int)CheckVCULLMTBR(sgdiag_ullmtbs0 - 1, false, llmtb_sgd, vc_add_number,true, sign_llmtb_sgd);
                } else {
                    ulmtb_sgd = (unsigned int)CheckVCULLMTBR(sgdiag_ullmtbs0 - 1, true, ulmtb_sgd, vc_add_number,false,sign_ulmtb_sgd);
                    llmtb_sgd = (unsigned int)CheckVCULLMTBR(sgdiag_ullmtbs0 - 1, false, llmtb_sgd, vc_add_number,false, sign_llmtb_sgd);
                }
                if(ulmtb_sgd < llmtb_sgd){
                    re_printf("warning","Upper/Lower Limit Check does not occur because VCULLMTBR%d.VCULMTB < VCULLMTBR%d.VCLLMTB\n",sgdiag_ullmtbs0 - 1, sgdiag_ullmtbs0 - 1);
                }else {
                    mFinalPhyChannel = mPhyChannel;
                    if (((*SGDIAGCR)["SDPS"] == 1) && (!(mFinalPhyChannel& 0x1))) {
                        srr_data0 = (unsigned int)convt_data;
                    }
                    if ((((*SGDIAGCR)["SDPS"] == 0) && (vc_number == 1)) || (((*SGDIAGCR)["SDPS"] == 1) && (!(mFinalPhyChannel& 0x1)))) {
                        if(srr_data0 > ulmtb_sgd) {
                            mErrorId = mErrorId | 0x8;
                            isUpperError = true;
                        }
                        if(srr_data0 < llmtb_sgd) {
                            mErrorId = mErrorId | 0x8;
                            isLowerError = true;
                        }
                    }
                }
            } 
            if ((sgdiag_ullmtbs1 != 0) && (group_number == emGroupSGDiag)) {
                ulmtb_sgd = (*VCULLMTBR[sgdiag_ullmtbs1-1])["VCULMTB"];
                llmtb_sgd = (*VCULLMTBR[sgdiag_ullmtbs1-1])["VCLLMTB"];
                sign_ulmtb_sgd = (ulmtb_sgd>>15)&0x1;
                sign_llmtb_sgd = (llmtb_sgd>>15)&0x1;
                if ((*SGDIAGCR)["SDPS"] == 0) {
                    ulmtb_sgd = (int)CheckVCULLMTBR(sgdiag_ullmtbs1 - 1, true, ulmtb_sgd, vc_add_number,true,sign_ulmtb_sgd);
                    llmtb_sgd = (int)CheckVCULLMTBR(sgdiag_ullmtbs1 - 1, false, llmtb_sgd, vc_add_number,true, sign_llmtb_sgd);
                }else {
                    ulmtb_sgd = (unsigned int)CheckVCULLMTBR(sgdiag_ullmtbs0 - 1, true, ulmtb_sgd, vc_add_number,false,sign_ulmtb_sgd);
                    llmtb_sgd = (unsigned int)CheckVCULLMTBR(sgdiag_ullmtbs0 - 1, false, llmtb_sgd, vc_add_number,false, sign_llmtb_sgd);
                }
                if(ulmtb_sgd < llmtb_sgd){
                    re_printf("warning","Upper/Lower Limit Check does not occur because VCULLMTBR%d.VCULMTB < VCULLMTBR%d.VCLLMTB\n",sgdiag_ullmtbs1 - 1, sgdiag_ullmtbs1 - 1);
                }else {
                    mFinalPhyChannel = mPhyChannel;
                    if (((*SGDIAGCR)["SDPS"] == 1) && ((mFinalPhyChannel& 0x1))) {
                        srr_data1 = (unsigned int)convt_data;
                    }
                    if ((((*SGDIAGCR)["SDPS"] == 0) && (vc_number == 2)) || (((*SGDIAGCR)["SDPS"] == 1) && ((mFinalPhyChannel& 0x1)))) {
                        if(srr_data1 > ulmtb_sgd) {
                            mErrorId = mErrorId | 0x8;
                            isUpperError = true;
                        }
                        if(srr_data1 < llmtb_sgd) {
                            mErrorId = mErrorId | 0x8;
                            isLowerError = true;
                        }
                    }
                }
            }

            //3.4.Check INT_AD interrupt
            if(vc_number < mVirChannelNum) {
                if (group_number != emGroupSGDiag) {
                    if (  (unsigned int)((*VCR[vc_number])["ADIE"]) == 1
                            ||(vc_number == mEndVCPointer[group_number] && (unsigned int)((*SGCR[group_number])["ADIE"]) == 1)
                       ) {
                        mINT_ADmxActive[group_number] = true;
                    }
                } else { 
                    if ((unsigned int)(*SGDIAGCR)["ADIE"] == 1) {
                        if (((*SGDIAGCR)["ISM"] == 0) && (vc_number == mEndVCPointer[group_number]))  {
                            mINT_ADmxActive[emGroupSGDiag] = true;
                        } else if ((((*SGDIAGCR)["ISM"] == 1) && (vc_number == mEndVCPointer[group_number]) && mLastTargetChannel)) {
                            mINT_ADmxActive[emGroupSGDiag] = true;
                        }
                    }
                    if (vc_number == mEndVCPointer[group_number]) {
                        if (mLastTargetChannel) {
                            UpdatePhysicalChannelSGD(true);
                            mLastTargetChannel = false;
                        }
                        if (mSgDiagStop) {
                            mSgDiagStop = false;
                            mPCCR0_finish = false;
                            mTargetSGD_0 = 0;
                            mTargetSGD_1 = 0;
                        }
                    }
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
            if (mForcedBreak) {
                if (cnvcls == emDisconnectionDiagMode || cnvcls == emPdDiagMode || cnvcls == emPuDiagMode) {
                    mStoreData = 0;
                }
            }
            if(mForcedDataPathError) {
                mStoreData  = 0;
            }

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

            if (cnvcls == emDisconnectionMode) {
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

double Cadc_u2b::ADCTimeCalculation (const bool is_start_SG, const unsigned int vc_number, const unsigned int vc_add, const unsigned int group_number)
{//{{{
#if 1 // RH850_U2B
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
        if (group_number != emGroupSGDiag) {
            cnvcls = (unsigned int)((*VCR[vc_number])["CNVCLS"]);
            wtts = (unsigned int)((*VCR[vc_number])["WTTS"]);
        } else {
            cnvcls = (unsigned int)((*SGDIAGVCR[vc_number])["CNVCLS"]);
        }
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
    // MPX additional wait time - RH850_U2B
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

bool Cadc_u2b::TimingCheck (void)
{//{{{
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

    if ((finish_delay + mPCLKPeriod) >= (sample_delay + convert_delay) || (finish_delay + mPCLKPeriod) >= (ex_sample_delay + convert_delay)) {
        re_printf("error","The condition of tED, ExSmpt and tSAR is not correct.\n");
        return false;
    } else {
        return true;
    }
}//}}}

bool Cadc_u2b::VoltageCheck (void)
{//{{{
    if ((mADCInstance >= mADCNum) || ((mAVccMin <= mAVcc) && (mAVcc <= mAVccMax) && (mAvrefhMin <= mAvrefh) && (mAvrefh <= mAVcc))) {
        return true;
    } else {
        re_printf("warning","Setting reference voltages for ADC%d is not correct: mAvrefh:%lld.%06d, mAVcc:%lld.%06d.\n", mADCInstance, (unsigned long long)mAvrefh, FRACTION_TO_INT(mAvrefh), (unsigned long long)mAVcc, FRACTION_TO_INT(mAVcc));
        return false;
    }
}//}}}

bool Cadc_u2b::SuspendCheck (unsigned int group_number)
{//{{{
    unsigned int susmtd = (unsigned int)((*ADCR1)["SUSMTD"]);
    // asynronous suspend
    if (group_number == emGroupSGDiag) {
        return true;
    } else {
        if ((group_number == 0 && susmtd == 1) || (susmtd > 1)) {
            return true;
        } else {
            return false;
        }
    }
}//}}}

//call back functions
void Cadc_u2b::WritingADSYNSTCR (void)
{//{{{
    if ((mPCLKPeriod > 0) && (mADCLKPeriod > 0)) {
        if(TimingCheck() && VoltageCheck()){
            unsigned int group_number = 0;
            for(unsigned int i = 1; i < (emGroupNum - 1 + 1); i++){
                group_number = emGroupNum -1 - i;
                if((*SGCR[group_number])["ADSTARTE"] == 1){
                    mWritingADSYNSTCREvent[group_number].notify((unsigned int)(emtDCoeffPCLK * mPCLKPeriod), SC_NS);
                }
            }
        }
    }else {
        re_printf("warning","Scan group can not start while clock frequency is 0.\n");
    }
}//}}}

void Cadc_u2b::WritingADTSYNSTCR (void)
{//{{{
    if ((mPCLKPeriod > 0) && (mADCLKPeriod > 0)) {
        if(TimingCheck() && VoltageCheck()){
            unsigned int group_number = 0;
            for(unsigned int i = 1; i <= 2; i++){
                group_number = emGroupNum -1 - i;
                if((*SGCR[group_number])["ADTSTARTE"] == 1){
                    ADtimerCheck(group_number);
                }
            }
        }
    }else {
        re_printf("warning","AD timer can not start while clock frequency is 0.\n");
    }
}//}}}

void Cadc_u2b::WritingVMONVDCR (const std::string register_name, const std::string bit_name, bool bit_val)
{//{{{
    if ((mPCLKPeriod > 0) && (mADCLKPeriod > 0)) {
        if (register_name == "VMONVDCR1") {
            mVMONVDCRVal &= 0x2;
            mVMONVDCRVal |= (unsigned int)bit_val;
        } else {
            mVMONVDCRVal &= 0x1;
            mVMONVDCRVal |= (bit_val << 1);
        }
    }
}//}}}

void Cadc_u2b::cb_SGSTCR_SGST (RegCBstr str)
{//{{{
    unsigned int group_number = (unsigned int)(str.channel);
    if((*SGSTCR[group_number])["SGST"] == 1){
        (*SGSTCR[group_number]) = 0;
        if ((mPCLKPeriod > 0) && (mADCLKPeriod > 0)) {
            if(TimingCheck() && VoltageCheck()){
                mWritingSGSTCREvent[group_number].notify((unsigned int)(emtDCoeffPCLK * mPCLKPeriod), SC_NS);
            }
        }else {
            re_printf("warning","Scan group can not start while clock frequency is 0.\n");
        }
    }
}//}}}

void Cadc_u2b::cb_SGSTPCR_SGSTP (RegCBstr str)
{//{{{
#if 1
    if(CheckIsSVMode()){
        re_printf("warning","Setting SGSTPCR.SGSTP bit is prohibited during operation of SVSTOP.\n");
        //mIsRetainedTrigger[str.channel] = false;
        //if(str.channel == emGroup4) {
        //    mIsRetainedPWD = false;
        //}
        //if(mIsRetainedADsyn[str.channel]){
        //    mIsRetainedADsyn[str.channel] = false;
        //}
        //if(mIsRetainedSWTHA[str.channel]){
        //    mIsRetainedSWTHA[str.channel] = false;
        //}
        //if(mIsRetainedSWTHB[str.channel]){
        //    mIsRetainedSWTHB[str.channel] = false;
        //}
        //if(mIsRetainedHWTHA[str.channel]){
        //    mIsRetainedHWTHA[str.channel] = false;
        //}
        //if(mIsRetainedHWTHB[str.channel]){
        //    mIsRetainedHWTHB[str.channel] = false;
        //}
        //if(mIsRetainedSW[str.channel]){
        //    mIsRetainedSW[str.channel] = false;
        //}
    }
#endif
    (*SGSTPCR[str.channel]) = 0;
    if (mScanningState && (mCurrentGroup == str.channel)) {
#if 1
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
#else
            mDisableOneSGFlag = true;

#endif
        re_printf("info","The scanning process of scanning group %d is forced to stop.\n",mCurrentGroup);
    }else {
        mSGActive[str.channel] = false;
        (*SGSR[str.channel])["SGACT"] = 0;
        (*SGSTR)["SGACT"] = (unsigned int)((*SGSTR)["SGACT"]) & (~(0x1 << str.channel));
        mIsScanningProcess[str.channel] = false;
        mGroupSuspendState[str.channel] = false;
    }
}//}}}

void Cadc_u2b::cb_ADHALTR_HALT (RegCBstr str)
{//{{{
   if((*ADHALTR)["HALT"] == 1){
        (*ADHALTR) = 0;
        if ((mPCLKPeriod > 0) && (mADCLKPeriod > 0)) {
#if 1
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
#endif
            mPCCR0_finish = false;
            mTargetSGD_0 = 0;
            mTargetSGD_1 = 0;
            if (mScanningState) {
                mDisableFlag = true;
                for (unsigned int i = 0; i < emGroupNum; i++) {
                    mDisableSGxFlag[i] = true;
                }
                mDisableEvent.notify();
                re_printf("info","The scanning process is forced to stop.\n");
            }
            for (unsigned int i = 3; i < emGroupNum-1; i++) {
                if((*SGSR[i])["ADTACT"] == 1) {
                    (*SGSR[i])["ADTACT"] = 0;
                    (*SGSTR)["ADTACT"] =(unsigned int)( (*SGSTR)["ADTACT"]) & (~(0x1 << (i-3)));
                    mTIMER[i - 3]->StopCounting();
                    mADCStateEvent.notify();
                    re_printf("info","The counting of AD timer %d is forced to stop.\n", i);
                }
            }
        }
   }
}//}}}

void Cadc_u2b::cb_MPXCURCR_MSKCFMT (RegCBstr str)
{//{{{
    bool write_condition = true;
    write_condition = AllSGStopCheck();
    if (((*THACR)["HLDTE"] != 0) || ((*THBCR)["HLDTE"] != 0)) {
        write_condition = false;
    }

    if(write_condition){
        if ((mPCLKPeriod > 0) && (mADCLKPeriod > 0)) {
            unsigned int mpxcurcr = (unsigned int)(*MPXCURCR);
            unsigned int mpxcurr =  (unsigned int)(*MPXCURR1);
            unsigned int mskc_num = 4;
            for(unsigned int i = 0; i < mskc_num; i++){
                if((mpxcurcr & (1<<i)) == 0){
                    mpxcurr = mpxcurr & ((0xFFF0FFFF << (4*i)) | 0xFFF );
                }else{
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

void Cadc_u2b::cb_ADTSTCR_ADTST (RegCBstr str)
{//{{{
    unsigned int index = (unsigned int)(str.channel);
    if((unsigned int)((*PWDCR)["PWE"]) == 0x1 && index == emGroup4){
        re_printf("warning","Writing into ADTSTCR4 register is not affected.\n");
        (*ADTSTCR[index]) = str.pre_data;
    } else {
        if((*ADTSTCR[index])["ADTST"] == 1){
            (*ADTSTCR[index]) = 0;
            if ((mPCLKPeriod > 0) && (mADCLKPeriod > 0)) {
                if(TimingCheck() && VoltageCheck()){
                    ADtimerCheck(index);
                }
            }
        }
    }
}//}}}

void Cadc_u2b::cb_ADTENDCR_ADTEND (RegCBstr str)
{//{{{
    unsigned int index = (unsigned int)(str.channel);
    if((unsigned int)((*PWDCR)["PWE"]) == 0x1 && index == emGroup4){
        re_printf("warning","Writing into ADTENDCR4 register is not affected.\n");
        (*ADTENDCR[index]) = str.pre_data;
    } else {
        if((*ADTENDCR[index])["ADTEND"] == 1){
            (*ADTENDCR[index]) = 0;
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

void Cadc_u2b::CallBack_DRi (const unsigned int addr)
{//{{{
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

void Cadc_u2b::CallBack_SGDIAGDIRn (const unsigned int addr)
{//{{{
    if (mADCInstance == 4) {
        re_printf("warning","SG-Diag is not supported in ADCA. Reading SGDIAGDR01 register is ignored.\n");
    } else {
        if ((mPCLKPeriod > 0) && (mADCLKPeriod > 0)) {
            unsigned int real_vc_number = (unsigned int)((addr - 0x404) >> 2);
            if(mIsADPEmOperating){
                re_printf("warning","Reading SGDIAGDIR%d register is ignored because ADPEm interrupt is in process.\n", real_vc_number);
            }else{
                if(!CheckIsSVMode()) {
                    if((*SFTCR)["RDCLRE"] == 1){
                        if ((real_vc_number & 0x1) == 1) {
                            (*SGDIAGDR01)["SGDIAGDR1"] = 0;
                            re_printf("info","The SGDIAGDR01.SGDIAGDR1, SGDIAGDIR1 registers are cleared to 0.\n");
                        } else if ((real_vc_number & 0x1) == 0){
                            (*SGDIAGDR01)["SGDIAGDR0"] = 0;
                            re_printf("info","The SGDIAGDR01.SGDIAGDR0, SGDIAGDIR0 registers are cleared to 0.\n");
                        } else if ((real_vc_number & 0x1) == 2){
                            (*SGDIAGDR2)["SGDIAGDR2"] = 0;
                            re_printf("info","The SGDIAGDR2.SGDIAGDR2, SGDIAGDIR2 registers are cleared to 0.\n");
                        }
                        (*SGDIAGDIR[real_vc_number]) = 0;
                    }else {
                        (*SGDIAGDIR[real_vc_number])["WFLAG"] = 0;
                        re_printf("info","The SGDIAGDIR%d.WFLAG is cleared to 0.\n", real_vc_number);
                    }
                }

                //call interrupt
                if(mForcedParityError ){
                    re_printf("info","The parity error occurs in SGDIAGVC%d.\n", real_vc_number);
                    if((*PER)["PE"] == 0){
                        (*PER)["PE"] = 1;
                        (*PER)["PECAP"] = real_vc_number+0x78;
                    }
                    if((*SFTCR)["PEIE"] == 1){
                        mADPEmActive = true;
                        mIsADPEmOperating = true;
                        mADPEmInterruptEvent.notify();
                    }
                }
            }
        }
    }
}//}}}

void Cadc_u2b::CallBack_DIRn (const unsigned int addr)
{//{{{
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

void Cadc_u2b::CallBack_SGDIAGDR01 (void)
{//{{{
    if (mADCInstance == 4) {
        re_printf("warning","SG-Diag is not supported in ADCA. Reading SGDIAGDR01 register is ignored.\n");
    } else {
        if ((mPCLKPeriod > 0) && (mADCLKPeriod > 0)) {
            if(mIsADPEmOperating){
                re_printf("warning","Reading SGDIAGDR01 register is ignored because ADPEm interrupt is in process.\n");
            }else{
                if(!CheckIsSVMode()) {
                    if((*SFTCR)["RDCLRE"] == 1){
                        (*SGDIAGDR01) = 0;
                        (*SGDIAGDIR[0]) = 0;
                        (*SGDIAGDIR[1]) = 0;
                        re_printf("info","The SGDIAGDR01, SGDIAGDIR0 and SGDIAGDIR1 registers are cleared to 0.\n");
                    }else {
                        (*SGDIAGDIR[0])["WFLAG"] = 0;
                        (*SGDIAGDIR[1])["WFLAG"] = 0;
                        re_printf("info","The SGDIAGDIR0.WFLAG and SGIDAGDIR1.WFLAG are cleared to 0.\n");
                    }
                }

                //call interrupt
                if(mForcedParityError){
                    re_printf("info","The parity error occurs when reading SGDIAGDR01.\n");
                    if((*PER)["PE"] == 0){
                        (*PER)["PE"] = 1;
                        (*PER)["PECAP"] = 0x7A;
                    }
                    if((*SFTCR)["PEIE"] == 1){
                        mADPEmActive = true;
                        mIsADPEmOperating = true;
                        mADPEmInterruptEvent.notify();
                    }
                }
            }
        }
    }
}//}}}

void Cadc_u2b::CallBack_SGDIAGDR2 (void)
{//{{{
    if (mADCInstance == 4) {
        re_printf("warning","SG-Diag is not supported in ADCA. Reading SGDIAGDR2 register is ignored.\n");
    } else {
        if ((mPCLKPeriod > 0) && (mADCLKPeriod > 0)) {
            if(mIsADPEmOperating){
                re_printf("warning","Reading SGDIAGDR2 register is ignored because ADPEm interrupt is in process.\n");
            }else{
                if(!CheckIsSVMode()) {
                    if((*SFTCR)["RDCLRE"] == 1){
                        (*SGDIAGDR2) = 0;
                        (*SGDIAGDIR[2]) = 0;
                        re_printf("info","The SGDIAGDR2 and SGDIAGDIR2 registers are cleared to 0.\n");
                    }else {
                        (*SGDIAGDIR[2])["WFLAG"] = 0;
                        re_printf("info","The SGDIAGDIR2.WFLAG is cleared to 0.\n");
                    }
                }

                //call interrupt
                if(mForcedParityError){
                    re_printf("info","The parity error occurs when reading SGDIAGDR2.\n");
                    if((*PER)["PE"] == 0){
                        (*PER)["PE"] = 1;
                        (*PER)["PECAP"] = 0x7A;
                    }
                    if((*SFTCR)["PEIE"] == 1){
                        mADPEmActive = true;
                        mIsADPEmOperating = true;
                        mADPEmInterruptEvent.notify();
                    }
                }
            }
        }
    }
}//}}}

void Cadc_u2b::CallBack_PWDDR (void)
{//{{{
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

void Cadc_u2b::CallBack_PWDDIR (void)
{//{{{
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

void Cadc_u2b::cb_ADCR1_SUSMTD (RegCBstr str)
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

void Cadc_u2b::cb_ADCR2_DFMT (RegCBstr str)
{//{{{
#if 1
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

void Cadc_u2b::cb_ECR_IDEC (RegCBstr str)
{//{{{
    unsigned int tocesgc = (unsigned int)((*ECR)["TOCESGC"]);
    unsigned int tocesg = (unsigned int)((*TOCER)["TOCESG"]);
    if ((mPCLKPeriod > 0) && (mADCLKPeriod > 0)) {
        for(unsigned int group_number = 0; group_number < emGroupNum-1; group_number++){
            if(((tocesgc >> group_number) & 0x1) == 1 && ((tocesg >> group_number) & 0x1) == 1){
                (*TOCER)["TOCESG"] =(unsigned int)( (*TOCER)["TOCESG"]) & (~(0x1 << group_number));
                re_printf("info","The TOCER.TOCESG[%d] bit is cleared to 0.\n",group_number);
            }
        }
        if(((*ECR)["TOCESDC"] == 1) && ((*TOCER)["TOCESD"] == 1)){
            (*TOCER)["TOCESD"] = 0;
            re_printf("info","The TOCER.TOCESD bit is cleared to 0.\n");
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

 void Cadc_u2b::cb_SGCR_ADIE (RegCBstr str)
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

void Cadc_u2b::cb_SGMCYCR_MCYC (RegCBstr str)
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

void Cadc_u2b::cb_SGVCPR_VCSP (RegCBstr str)
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

void Cadc_u2b::cb_SGVCSP_VCSP (RegCBstr str)
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

void Cadc_u2b::cb_SGVCEP_VCEP (RegCBstr str)
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

void Cadc_u2b::cb_ADTIPR_ADTIP (RegCBstr str)
{//{{{
    if((*SGSR[str.channel])["ADTACT"] != 0){
        re_printf("warning","Writing into ADTIPR%d register is not affected.\n", str.channel);
        (*ADTIPR[str.channel]) = str.pre_data;
    }
}//}}}

void Cadc_u2b::cb_ADTPRR_ADTPR (RegCBstr str)
{//{{{
    if((*SGSR[str.channel])["ADTACT"] != 0){
        re_printf("warning", "Writing into ADTPRR%d register is not affected.\n", str.channel);
        (*ADTPRR[str.channel]) = str.pre_data;
    }
}//}}}

void Cadc_u2b::cb_SFTCR_IDEIE(RegCBstr str)
{//{{{
    if(!AllSGStopCheck()){
        re_printf("warning", "Writing into SFTCR register is not affected.\n");
        (*SFTCR) = str.pre_data;
    }
}//}}}

void Cadc_u2b::cb_TOCCR_TOCE(RegCBstr str)
{//{{{
    if(!AllSGStopCheck()){
        re_printf("warning", "Writing into TOCCR register is not affected.\n");
        (*TOCCR) = str.pre_data;
    }
}//}}}

void Cadc_u2b::cb_TOCER_TOCESD(RegCBstr str)
{//{{{
}//}}}

void Cadc_u2b::cb_WAITTR_WAITTIME(RegCBstr str)
{//{{{
    if(!AllSGStopCheck()){
        re_printf("warning", "Writing into WAITTR%d register is not affected.\n",str.channel);
        (*WAITTR[str.channel]) = str.pre_data;
    }
}//}}}

void Cadc_u2b::cb_EMUCR_SVSDIS(RegCBstr str)
{//{{{
    if(!AllSGStopCheck()){
        (*EMUCR) = str.pre_data;
    }
    re_printf("warning", "Writing into EMUCR register is not affected because this register is not supported.\n");
}//}}}

void Cadc_u2b::cb_TDCR_TDLV(RegCBstr str)
{//{{{
    if(!AllSGStopCheck()){
        re_printf("warning", "Should not update value of TDCR register while scan group is operating.\n");
        //(*TDCR) = str.pre_data;
    }
}//}}}

#if 1
void Cadc_u2b::cb_ODCR_WADDE(RegCBstr str)
{//{{{
    if(!AllSGStopCheck()){
        re_printf("warning", "Writing into ODCR register is not affected.\n");
        (*ODCR) = str.pre_data;
    }
}//}}}
#endif

void Cadc_u2b::cb_DFASENTSGER_ASENTSG0E(RegCBstr str)
{//{{{
    if(!AllSGStopCheck()){
        re_printf("warning", "Writing into DFASENTSGER register is not affected.\n");
        (*DFASENTSGER) = str.pre_data;
    }
}//}}}

#if 0
// RH850_U2B added
void Cadc_u2b::cb_GTMENTSGER_GTMENTSG0E(RegCBstr str)
{//{{{
    if(!AllSGStopCheck()){
        re_printf("warning", "Writing into GTMENTSGER register is not affected.\n");
        (*GTMENTSGER) = str.pre_data;
    }
}//}}}
#endif

void Cadc_u2b::cb_MPXINTER_ADMPXIE(RegCBstr str)
{//{{{
    if(!AllSGStopCheck()){
        re_printf("warning", "Writing into MPXINTER register is not affected.\n");
        (*MPXINTER) = str.pre_data;
    }
}//}}}

void Cadc_u2b::cb_MPXOWR_MPXOW(RegCBstr str)
{//{{{
    re_printf("warning", "Writing into MPXOWR register is not affected.\n");
    (*MPXOWR) = 0;
}//}}}

void Cadc_u2b::cb_VCR_GCTRL(RegCBstr str)
{//{{{
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
        if (mADCInstance == 4) {
            re_printf("warning", "T&H is not supported in ADCA. VCR%d.CNVCLS is prohibited to be 0x1.\n",str.channel);
            (*VCR[str.channel])["CNVCLS"] = ((str.pre_data >> 11) & 0xF);
        }
    }
    for (unsigned int group_number = 0; group_number <emGroupNum-1; group_number++) {
        if (((unsigned int)((*SGVCPR[group_number])["VCSP"]) <= str.channel) && ( str.channel <= (unsigned int)((*SGVCPR[group_number])["VCEP"]))
         && ((*SGSR[group_number])["SGACT"] != 0)) {
            re_printf("warning","Writing into VCR%d register is not affected.\n", str.channel);
            (*VCR[str.channel]) = str.pre_data;
            break;
        }
    }
}//}}}

void Cadc_u2b::cb_SMPCR_SMPT(RegCBstr str)
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

void Cadc_u2b::cb_VCULLMTBR_VCULMTB(RegCBstr str)
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

void Cadc_u2b::cb_VCLMSCR1_VC00LMSC(RegCBstr str)
{//{{{
    for (unsigned int i=0;i<32;i++) {
        if ((((unsigned int)(*VCLMSCR1) >> i)&0x1) == 0x1) {
            (*VCLMSR1) = (unsigned int)(*VCLMSR1) & (~(1 << i));
            re_printf("info", "The VCLMSR1.VC%2dLMS bit is clear to 0.\n", i);
        }
    }
    *VCLMSCR1 = 0;
}//}}}

void Cadc_u2b::cb_VCLMSCR2_VC32LMSC(RegCBstr str)
{//{{{
    for (unsigned int i=0;i<32;i++) {
        if ((((unsigned int)(*VCLMSCR2) >> i)&0x1) == 0x1) {
            (*VCLMSR2) = (unsigned int)(*VCLMSR2) & (~(1 << i));
            re_printf("info", "The VCLMSR2.VC%2dLMS bit is clear to 0.\n", i);
        }
    }
    *VCLMSCR2 = 0;
}//}}}

#if 0
void Cadc_u2b::cb_VCLMSCR3_VC64LMSC(RegCBstr str)
{//{{{
    for (unsigned int i=0;i<32;i++) {
        if ((((unsigned int)(*VCLMSCR3) >> i)&0x1) == 0x1) {
            (*VCLMSR3) = (unsigned int)(*VCLMSR3) & (~(1 << i));
            re_printf("info", "The VCLMSR3.VC%2dLMS bit is clear to 0.\n", i);
        }
    }
    *VCLMSCR3 = 0;
}//}}}
#endif 

void Cadc_u2b::cb_PWVCLMSCR_PWVCLMSC(RegCBstr str)
{//{{{
    if ((*PWVCLMSCR)["PWVCLMSC"] == 0x1) {
        (*PWVCLMSR)["PWVCLMS"] = 0x0;
        re_printf("info", "The PWVCLMSR.PWVCLMS bit is clear to 0.\n");
    }
    (*PWVCLMSCR) = 0;
}//}}}

void Cadc_u2b::cb_PWDCR_PWE(RegCBstr str)
{//{{{
    if ((mADCInstance == 3) || (mADCInstance == 4)) {
        re_printf("warning","PWM-Diag is not supported in ADC3 and ADCA. Writing into PWDCR register is not affected.\n");
        (*PWDCR) = 0;
    }
    else {
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
    }
}//}}}

void Cadc_u2b::cb_PWDSGCR_TRGMD(RegCBstr str)
{//{{{
    if ((mADCInstance == 3) || (mADCInstance == 4)) {
        re_printf("warning","PWM-Diag is not supported in ADC3 and ADCA. Writing into PWDSGCR register is not affected.\n");
        (*PWDSGCR) = 0;
    }
    else {
        if(str.data != str.pre_data) {
            if (((*SGSR[emGroup4])["SGACT"] != 0 && (str.pre_data & 0x1) == 0 && (str.data & 0x1) == 1) ||
                    ((unsigned int)((*PWDCR)["PWE"]) == 0x0)) {
                re_printf("warning","Writing into PWDSGCR register is not affected.\n");
                (*PWDSGCR) = str.pre_data;
            }
        }
    }
}//}}}

void Cadc_u2b::cb_PWVCLMINTER_PWADULIE(RegCBstr str)
{//{{{
    if(str.data != str.pre_data) {
        if (!AllSGStopCheck()) {
            re_printf("warning","Writing into PWVCLMINTER register is not affected.\n");
            (*PWVCLMINTER) = str.pre_data;
        }
    }
}//}}}

void Cadc_u2b::cb_SGULCCR_SGULCC(RegCBstr str)
{//{{{
    for(unsigned int i=0;i<emGroupNum-1;i++) {
        if((((unsigned int)((*SGULCCR)["SGULCC"]) >> i) & 0x1) == 0x1){
            (*SGULCR[i]) = 0x0;
            re_printf("info", "The SGULCR%d register is clear to 0.\n", i);
        }
    }
    if ((unsigned int)(*SGULCCR)["SDULCC"] == 0x1) {
        (*SGDIAGULCR) = 0x0;
        re_printf("info", "The SGDIAGULCR register is clear to 0.\n");
    }
    (*SGULCCR) = 0x0;
}//}}}

void Cadc_u2b::cb_VCLMASCR_ALLMSC(RegCBstr str)
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

void Cadc_u2b::cb_SGDIAGVCR_CNVCLS(RegCBstr str)
{//{{{ 
    unsigned int sgd_cnvcls = (unsigned int)((*SGDIAGVCR[str.channel])["CNVCLS"]);
    if (mADCInstance == 4) {
        re_printf("warning","SG-Diag is not supported in ADCA. Writing into SGDIAGVCR%d register is not affected.\n",str.channel);
        (*SGDIAGVCR[str.channel]) = 0;
    } else {
        if((*SGDIAGSR)["SDACT"] != 0) {
            re_printf("warning","Writing into SGDIAGVCR%d is not affected.\n",str.channel);
            (*SGDIAGVCR[str.channel]) = str.pre_data & 0xFF;
        } else {
            if((sgd_cnvcls == 1) || (sgd_cnvcls == 2) || (sgd_cnvcls == 3) || (sgd_cnvcls == 5) ||
               (sgd_cnvcls == 6) || (sgd_cnvcls == 8) || (sgd_cnvcls == 0xB) || (sgd_cnvcls == 0xE) || (sgd_cnvcls == 0xF)) {
                re_printf("warning","Setting SGDIAGVCR%d.CNVCLS = %x is prohibited.\n",str.channel, sgd_cnvcls);
                (*SGDIAGVCR[str.channel]) = str.pre_data & 0xFF;
            }
        }
    }
}//}}}

void Cadc_u2b::cb_SGDIAGSTCR_SGST(RegCBstr str)
{//{{{
    if (mADCInstance == 4) {
        re_printf("warning","SG-Diag is not supported in ADCA. Writing into SGDIAGSTCR register is not affected.\n");
        (*SGDIAGSTCR) = 0;
    } else {
        if ((*SGDIAGSTCR)["SGST"] == 0x1) {
            (*SGDIAGSTCR) = 0;
            if ((mPCLKPeriod > 0) && (mADCLKPeriod > 0)) {
                if(TimingCheck() && VoltageCheck()){
                    mWritingSGSTCREvent[emGroupSGDiag].notify((unsigned int)(emtDCoeffPCLK * mPCLKPeriod), SC_NS);
                }
            }else {
                re_printf("warning","Scan group can not start while clock frequency is 0.\n");
            }
        }
    }
}//}}}

void Cadc_u2b::cb_SGDIAGSTPCR_SGSTP(RegCBstr str)
{//{{{
    if (mADCInstance == 4) {
        re_printf("warning","SG-Diag is not supported in ADCA. Writing into SGDIAGSTPCR register is not affected.\n");
        (*SGDIAGSTPCR) = 0;
    } else {
        if (CheckIsSVMode()) {
            re_printf("warning","Setting SGDIAGSTPCR.SGSTP bit is prohibited during operation of SVSTOP.\n");
            //mIsRetainedTrigger[emGroupSGDiag] = false;
            //if(mIsRetainedSW[emGroupSGDiag]){
            //    mIsRetainedSW[emGroupSGDiag] = false;
            //}
        }
        (*SGDIAGSTPCR) = 0;
        if (mScanningState && (mCurrentGroup == emGroupSGDiag)) {
            if (CheckIsSVMode()) {
                mSGActive[mCurrentGroup] = false;
                (*SGDIAGSR)["SDACT"] = 0;
                (*SGSTR)["SDACT"] = 0;
                mIsScanningProcess[mCurrentGroup] = false;
                mGroupSuspendState[mCurrentGroup] = false;
                mDisableFlag = true;
                mDisableSGxFlag[mCurrentGroup] = true;
                mDisableEvent.notify();
            } else {
                mSgDiagStop = true;
                re_printf("info","The scanning process of scanning group SG-Diag is forced to stop.\n");
            }
        } else {
            (*SGSTR)["SDACT"] = 0;
            mSGActive[emGroupSGDiag] = false;
            (*SGDIAGSR)["SDACT"] = 0;
            mIsScanningProcess[emGroupSGDiag] = false;
            mGroupSuspendState[emGroupSGDiag] = false;
        }
    }
}//}}}

void Cadc_u2b::cb_SGDIAGCR_TRGMD(RegCBstr str)
{//{{{
    unsigned int cur_vcullmtbs1 = ((unsigned int)(str.data >> 20) & 0xF);
    unsigned int pre_vcullmtbs1 = ((unsigned int)(str.pre_data >> 20) & 0xF);
    unsigned int cur_vcullmtbs0 = ((unsigned int)(str.data >> 16) & 0xF);
    unsigned int pre_vcullmtbs0 = ((unsigned int)(str.pre_data >> 16) & 0xF);
    unsigned int pre_trgmd    = ((unsigned int)(str.pre_data) & 0x1);
    unsigned int cur_trgmd    = ((unsigned int)(str.data) & 0x1);
    unsigned int pre_adie     = ((unsigned int)(str.pre_data >> 8) & 0x1);
    unsigned int cur_adie     = ((unsigned int)(str.data >> 8) & 0x1);
    unsigned int pre_smd      = ((unsigned int)(str.pre_data >> 12) & 0x1);
    unsigned int cur_smd      = ((unsigned int)(str.data >> 12) & 0x1);
    unsigned int pre_ism      = ((unsigned int)(str.pre_data >> 10) & 0x1);
    unsigned int cur_ism      = ((unsigned int)(str.data >> 10) & 0x1);
    unsigned int pre_sdps     = ((unsigned int)(str.pre_data >> 9) & 0x1);
    unsigned int cur_sdps     = ((unsigned int)(str.data >> 9) & 0x1);
    if (mADCInstance == 4) {
        re_printf("warning","SG-Diag is not supported in ADCA. Writing into SGDIAGCR register is not affected.\n");
        (*SGDIAGCR) = 0;
    } else {
        if((*SGDIAGSR)["SDACT"] == 0) {
            if ((cur_vcullmtbs1 & 0xF) > 8) {
                re_printf("warning", "SGDIAGCR.VCULLMTBS1 is set to 0 due to written above value 8 is prohibited.\n");
                (*SGDIAGCR)["VCULLMTBS1"] = 0;
            }
            if ((cur_vcullmtbs0 & 0xF) > 8) {
                re_printf("warning", "SGDIAGCR.VCULLMTBS0 is set to 0 due to written above value 8 is prohibited.\n");
                (*SGDIAGCR)["VCULLMTBS0"] = 0;
            }
        } else {
            if ((pre_trgmd & 0x1) == 0x0 && (cur_trgmd & 0x1) == 0x1) {
                re_printf("warning","Writing into SGDIAGCR.TRGMD bit is not affected.\n");
                (*SGDIAGCR)["TRGMD"] = pre_trgmd;
            }
            if (pre_adie != cur_adie) {
                re_printf("warning","Writing into SGDIAGCR.ADIE bit is not affected.\n");
                (*SGDIAGCR)["ADIE"] = pre_adie;
            }
            if (pre_smd != cur_smd) {
                re_printf("warning","Writing into SGDIAGCR.SMD bit is not affected.\n");
                (*SGDIAGCR)["SMD"] = pre_smd;
            }
            if (pre_ism != cur_ism) {
                re_printf("warning","Writing into SGDIAGCR.ISM bit is not affected.\n");
                (*SGDIAGCR)["ISM"] = pre_ism;
            }
            if (pre_sdps != cur_sdps) {
                re_printf("warning","Writing into SGDIAGCR.SPDS bit is not affected.\n");
                (*SGDIAGCR)["SDPS"] = pre_sdps;
            }
            if (pre_vcullmtbs1 != cur_vcullmtbs1) {
                re_printf("warning","Writing into SGDIAGCR.VCULLMTBS1 bit is not affected.\n");
                (*SGDIAGCR)["VCULLMTBS1"] = pre_vcullmtbs1;
            }
            if (pre_vcullmtbs0 != cur_vcullmtbs0) {
                re_printf("warning","Writing into SGDIAGCR.VCULLMTBS0 bit is not affected.\n");
                (*SGDIAGCR)["VCULLMTBS0"] = pre_vcullmtbs0;
            }
        }
    }
}//}}}

void Cadc_u2b::cb_SGDIAGPCCR1_SDPCE32(RegCBstr str)
{//{{{
    if (mADCInstance != 3) {
        re_printf("warning","SGDIAGPCCR1 register is supported in ADC3 only. Writing into SGDIAGPCCR1 register is not affected.\n");
        (*SGDIAGPCCR1) = 0;
    } else {
        if((*SGDIAGSR)["SDACT"] == 0) {
            mTargetSGD_1 = 0;
            mPCCR0_finish = false;
        } else {
            re_printf("warning","Writing into SGDIAGPCCR1 register is not affected.\n");
            (*SGDIAGPCCR1) = str.pre_data;
        }
    }
}//}}}

void Cadc_u2b::cb_SGDIAGPCCR0_SDPCE0(RegCBstr str)
{//{{{
    if (mADCInstance == 4) {
        re_printf("warning","SG-Diag is not supported in ADCA. Writing into SGDIAGPCCR1 register is not affected.\n");
        (*SGDIAGPCCR0) = 0;
    } else {
        if((*SGDIAGSR)["SDACT"] == 0) {
            mTargetSGD_0 = 0;
            mPCCR0_finish = false;
        } else {
            re_printf("warning","Writing into SGDIAGPCCR0 register not affected.\n");
            (*SGDIAGPCCR0) = str.pre_data;
        }
    }
}//}}}

void Cadc_u2b::cb_SDVCLMINTER_SDADULIE(RegCBstr str)
{//{{{
    if (mADCInstance == 4) {
        re_printf("warning","SG-Diag is not supported in ADCA. Writing into SDVCLMINTER register is not affected.\n");
        (*SDVCLMINTER) = 0;
    } else {
        if (!AllSGStopCheck()) {
            re_printf("warning", "Writing into SDVCLMINTER register is not affected.\n");
            (*SDVCLMINTER) = str.pre_data;
        }
    }
}//}}}

void Cadc_u2b::cb_SDVCLMSCR_SDVC0LMSC(RegCBstr str)
{//{{{
    if (mADCInstance == 4) {
        re_printf("warning","SG-Diag is not supported in ADCA. Writing into SDVCLMSCR register is not affected.\n");
        (*SDVCLMSCR) = 0;
    } else {
        if ((*SDVCLMSCR)["SDVC0LMSC"] == 1) {
            (*SDVCLMSR)["SDVC0LMS"] = 0;
            re_printf("info", "The SDVCLMSR.SDVC0LMS bit is clear to 0.\n");
        } else if ((*SDVCLMSCR)["SR0LMSC"] == 1) {
            (*SDVCLMSR)["SR0LMS"] = 0;
            re_printf("info", "The SDVCLMSR.SR0LMS bit is clear to 0.\n");
        } else if ((*SDVCLMSCR)["SR1LMSC"] == 1) {
            (*SDVCLMSR)["SR1LMS"] = 0;
            re_printf("info", "The SDVCLMSR.SR1LMS bit is clear to 0.\n");
        }
        (*SDVCLMSCR) = 0;
    }
}//}}}

void Cadc_u2b::cb_STPDCR_SPDE(RegCBstr str)
{//{{{
    unsigned int pre_stpdcr = str.pre_data;
    unsigned int cur_stpdcr = str.data;
    if (mADCInstance == 1) {
        if ((str.channel >= emSTPDchannel) || (!AllSGStopCheck())) {
            re_printf("warning", "Writting to STPDCR%d is not affected.\n",str.channel);
            *STPDCR[str.channel] = 0;
        } else {
//            if ((*STPDCR[str.channel]) = 0x1) {
            if (((pre_stpdcr&0x1) == 0) && ((cur_stpdcr&0x1)==0x1)) {
                if(str.channel == 0) {
                    for (unsigned int i = 0; i < mVirChannelNum; i ++) {
                        if (((*VCR[i])["CNVCLS"] == 0) && (((*VCR[i])["GCTRL"])== 6)) {
                            mStrongPullDown[str.channel] = 1;
                            re_printf("info", "STPDCR%d is enabled.\n",str.channel);
                            break;
                        }
                    }
                } else if (str.channel == 1) {
                    for (unsigned int i = 0; i < mVirChannelNum; i ++) {
                        if (((*VCR[i])["CNVCLS"] == 0) && (((*VCR[i])["GCTRL"])== 7)) {
                            mStrongPullDown[str.channel] = 1;
                            re_printf("info", "STPDCR%d is enabled.\n",str.channel);
                            break;
                        }
                    }
                } else {
                    for (unsigned int i = 0; i < mVirChannelNum; i ++) {
                        if (((*VCR[i])["CNVCLS"] == 0) && (((*VCR[i])["GCTRL"])== 8)) {
                            mStrongPullDown[str.channel] = 1;
                            re_printf("info", "STPDCR%d is enabled.\n",str.channel);
                            break;
                        }
                    }
                }
                if (!mStrongPullDown[str.channel]) {
                    re_printf("info", "There is no target physical channel selected by STPDCR%d\n",str.channel);
                }
            }
        }
    } else {
        re_printf("warning", "STPDCR%d register is supported in ADC1 only. Writing to STPDCR%d register is not affected.\n",str.channel, str.channel);
        *STPDCR[str.channel] = 0;
    }
}//}}}

//reset functions
void Cadc_u2b::EnableReset(const bool is_active,const unsigned int port_reset)
{//{{{
    if (mADCInstance >= mADCNum) {
        return;
    }
    if(is_active){
        if(port_reset == emADC_AD_RESET || port_reset == emADC_AD_RESETA){
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
            CancelAllEvents();

            mIsADReset = true;
            mADResetEvent.notify();
        }
        if(port_reset == emADC_P_RESET || port_reset == emADC_P_RESETA){
            mIsPReset = true;
            mPResetEvent.notify();
        }

    }else{
        if(port_reset == emADC_AD_RESET || port_reset == emADC_AD_RESETA){
            mIsADReset = false;
            mADCState = emADCIdleState;
            re_printf("info","The ADC%d state changes from RESET state to IDLE state.\n", mADCInstance);
        }
        if(port_reset == emADC_P_RESET || port_reset == emADC_P_RESETA){
            mIsPReset = false;
        }
    }

    if(port_reset == emADC_P_RESET || port_reset == emADC_P_RESETA){
        //reset register
        Cadc_u2b_regif::EnableReset(is_active);
    }

    if(port_reset == emADC_AD_RESET || port_reset == emADC_AD_RESETA){
        //reset timer
        for(unsigned int index = 0; index < 2; index++){
            mTIMER[index]->EnableReset(is_active);
        }
    }
}//}}}

void Cadc_u2b::TriggerADCAMethod(const unsigned int index)
{//{{{
    int group_number;
    if (mADCInstance == 4) {
        switch(index) {
            case 0:
                for (group_number = emGroupNum -2 ; group_number >= 0; group_number--) {
                    if ((*SGTSEL[group_number]) == 0x1) {
                        HardwareTrigger(group_number,4);
                    }
                }
                break;
            case 1:
                for (group_number = emGroupNum -2; group_number >= 0; group_number--) {
                    if ((*SGTSEL[group_number]) == 0x2) {
                        HardwareTrigger(group_number,4);
                    }
                }
                break;
            case 2:
                for (group_number = emGroupNum -2; group_number >= 0; group_number--) {
                    if ((*SGTSEL[group_number]) == 0x4) {
                        HardwareTrigger(group_number,4);
                    }
                }
                break;
            case 3:
                for (group_number = emGroupNum -2; group_number >= 0; group_number--) {
                    if ((*SGTSEL[group_number]) == 0x8) {
                        HardwareTrigger(group_number,4);
                    }
                }
                break;
            case 4:
                for (group_number = emGroupNum -2; group_number >= 0; group_number--) {
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

void Cadc_u2b::GlobalInit(void)
{//{{{
#if 1
    mEnterSVModeFlag = false;
    mExitSVModeFlag = false;
#endif 
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
    mSGDiagFlag = false;
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
    // RH850_U2B added
    mINT_ERRmActive = false;
    mINT_ERRmProcess = false;
#endif
    mID = 0;
    mStoreData = 0;
    mFinalData = 0;
    mFinalDataSRR0 = 0;
    mFinalDataSRR1 = 0;
    mINT_MPXmActiveNum = 0;
#if 0
    mINT_ULmActiveNum = 0;
    mINT_ADEmActiveNum = 0;
#else
    // RH850_U2B added
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
//        mINT_SDmActive                     = false;
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
        mIsHWTrigger[group_number] = false;
#if 1
        mIsRetainedTrigger[group_number] = false;
        mIsRetainedADsyn[group_number] = false;
        mIsRetainedSWTHA[group_number] = false;
        mIsRetainedSWTHB[group_number] = false;
        mIsRetainedHWTHA[group_number] = false;
        mIsRetainedHWTHB[group_number] = false;
        mIsRetainedSW[group_number] = false;
#endif
    }
#if 1
    mIsRetainedPWD = false;
#endif 
    mCurrentGroup = 0;
    mIsSuspendSGDiag = 0;
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
    mPCCR0_finish = false;
    mLastTargetChannel = false;
    mTargetSGD_0 = 0;
    mTargetSGD_1 = 0;
    mSgDiagStop = 0;
    mStrongPullDown[0] = 0;
    mStrongPullDown[1] = 0;
    mStrongPullDown[2] = 0;
}//}}}

//information function
void Cadc_u2b::DumpInfo (const char *type, const char *message, ... )
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

void Cadc_u2b::DumpStatInfo (void)
{//{{{
//1.Dump information
    if (mADCInstance < mADCNum) {
        if (mADCInstance != 4 ) {
            DumpInfo("StatInfo", "Info[%s](ADC%d).\n",sc_time_stamp().to_string().c_str(), mADCInstance);
            DumpInfo("StatInfo", " INT_MPX%d active number : %d\n", mADCInstance, mINT_MPXmActiveNum);
            DumpInfo("StatInfo", " ADPE%d active number : %d\n", mADCInstance, mADPEmActiveNum);
#if 0
            DumpInfo("StatInfo", " INT_UL%d active number : %d\n", mADCInstance, mINT_ULmActiveNum);
            DumpInfo("StatInfo", " INT_ADE%d active number : %d\n", mADCInstance, mINT_ADEmActiveNum);
#else
            DumpInfo("StatInfo", " INT_ERR%d active number : %d\n", mADCInstance, mINT_ERRmActiveNum);
#endif
            DumpInfo("StatInfo", " INT_AD%d0 active number : %d\n", mADCInstance, mINT_ADm0ActiveNum);
            DumpInfo("StatInfo", " INT_AD%d1 active number : %d\n", mADCInstance, mINT_ADm1ActiveNum);
            DumpInfo("StatInfo", " INT_AD%d2 active number : %d\n", mADCInstance, mINT_ADm2ActiveNum);
            DumpInfo("StatInfo", " INT_AD%d3 active number : %d\n", mADCInstance, mINT_ADm3ActiveNum);
            DumpInfo("StatInfo", " INT_AD%d4 active number : %d\n", mADCInstance, mINT_ADm4ActiveNum);
            DumpInfo("StatInfo", " INT_SD active number : %d\n", mINT_SDmActiveNum);
            DumpInfo("StatInfo", "EndInfo.\n");
        } else {
            DumpInfo("StatInfo", "Info[%s](ADCA).\n",sc_time_stamp().to_string().c_str());
            DumpInfo("StatInfo", " INT_MPXA active number : %d\n", mINT_MPXmActiveNum);
            DumpInfo("StatInfo", " ADPEA active number : %d\n", mADPEmActiveNum);
#if 0
            DumpInfo("StatInfo", " INT_ULA active number : %d\n",  mINT_ULmActiveNum);
            DumpInfo("StatInfo", " INT_ADEA active number : %d\n", mINT_ADEmActiveNum);
#else
            DumpInfo("StatInfo", " INT_ERRA active number : %d\n", mINT_ERRmActiveNum);
#endif
            DumpInfo("StatInfo", " INT_ADA0 active number : %d\n", mINT_ADm0ActiveNum);
            DumpInfo("StatInfo", " INT_ADA1 active number : %d\n", mINT_ADm1ActiveNum);
            DumpInfo("StatInfo", " INT_ADA2 active number : %d\n", mINT_ADm2ActiveNum);
            DumpInfo("StatInfo", " INT_ADA3 active number : %d\n", mINT_ADm3ActiveNum);
            DumpInfo("StatInfo", " INT_ADA4 active number : %d\n", mINT_ADm4ActiveNum);
            DumpInfo("StatInfo", " INT_SD active number : %d\n", mINT_SDmActiveNum);
            DumpInfo("StatInfo", "EndInfo.\n");
        }
    }

    //2.Reset information variable
    mINT_MPXmActiveNum = 0;
    mADPEmActiveNum   = 0;
#if 0
    mINT_ULmActiveNum = 0;
    mINT_ADEmActiveNum    = 0;
#else
    mINT_ERRmActiveNum    = 0;
#endif
    mINT_ADm0ActiveNum   = 0;
    mINT_ADm1ActiveNum   = 0;
    mINT_ADm2ActiveNum   = 0;
    mINT_ADm3ActiveNum   = 0;
    mINT_ADm4ActiveNum   = 0;
    mINT_SDmActiveNum    = 0;
}//}}}

void Cadc_u2b::DumpADCActivity (const double start_time, const double end_time)
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
            if (mFinalGroup != emGroupSGDiag) {
                re_printf("info","The value of virtual channel register VCR: 0x%X.\n", (unsigned int)(*VCR[mFinalVC]));
            } else {
                re_printf("info","The value of virtual channel register SGDIAGVCR: 0x%X.\n", (unsigned int)(*SGDIAGVCR[mFinalVC]));
            }
        }
        if (mFinalGroup != emGroupSGDiag) {
            re_printf("info","The scanning group: %d.\n", mFinalGroup);
        } else {
            re_printf("info","The scanning group: SG-Diag.\n");
        }
    }
}//}}}

void Cadc_u2b::DumpInterruptMessage (const std::string intr_name, const std::string intr_number, const std::string issue)
{//{{{
    if(mDumpInterrupt){
        re_printf("info","INT [SAR AD: %s%d%s] %s.\n", intr_name.c_str(), mADCInstance, intr_number.c_str(), issue.c_str());
    }
}//}}}

void Cadc_u2b::HWTriggerScanningMethod (void)
{//{{{
    if ((!mIsADReset) && (mADCInstance < mADCNum) && (mPCLKPeriod > 0) && (mADCLKPeriod > 0)) {
        for (unsigned int i = 0; i < emGroupNum; i++) {
            if (mIsHWTrigger[i]) {
                GroupCheck(i);
                mIsHWTrigger[i] = false;
            }
        }
    }
}//}}}

void Cadc_u2b::SettingZeroClock (void)
{//{{{
    // Cancel all events
    CancelAllEvents();

    // Stop operation
    mZeroClockEvent.notify();

    // Disable all Timers
    mTIMER[0]->StopCounting();
    mTIMER[1]->StopCounting();
}//}}}

void Cadc_u2b::CancelAllEvents (void)
{//{{{
    mScanningEvent.cancel();
    mSuspendEvent.cancel();
    for(unsigned int group_number = 0; group_number < emGroupNum; group_number++){
        mADENDmxEvent[group_number].cancel();
        mWritingADSYNSTCREvent[group_number].cancel();
        mWritingSGSTCREvent[group_number].cancel();
        mSGSTTriggerEvent[group_number].cancel();
        mADSYNTriggerEvent[group_number].cancel();
        if ((group_number >= emGroup3) && (group_number != emGroupSGDiag)) {
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
    // RH850_U2B added
    mULE_LPSmEvent.cancel();
    mINT_ERRmEvent.cancel();
#endif
    mBoundaryExcessEvent.cancel();
    mVCENDNoticeEvent.cancel();
    mSVModeEvent.cancel();
}//}}}

void Cadc_u2b::SetULErrorStatus(unsigned int channel, bool isSGD) 
{//{{{
    if (!isSGD) {
        if (channel < 32) {
            (*VCLMSR1) = (unsigned int)(*VCLMSR1) | (1 << channel);
        } else if (channel < 64) {
            (*VCLMSR2) = (unsigned int)(*VCLMSR2) | (1 << (channel-32));
        }
    } else {
        if ((*SGDIAGCR)["SDPS"] == 1 ) {
            (*SDVCLMSR)["SDVC0LMS"] = 1;
        } else {
            (*SDVCLMSR) = (unsigned int)(*SDVCLMSR) | (1<<channel);
        }
    }
}//}}}

bool Cadc_u2b::ULIntrEnable(unsigned int channel, bool isSGD) 
{//{{{
    if (!isSGD) {
        if (channel < 32) {
            if (((unsigned int)(*VCLMINTER1) & (1 << channel)) != 0) {
                return true;
            }
        } else if (channel < 64) {
            if (((unsigned int)(*VCLMINTER2) & (1 << (channel-32))) != 0) {
                return true;
            }
        } 
    } else {
        if (((unsigned int)(*SDVCLMINTER) & 0x1)== 1) {
            return true;
        }
    }
    return false;
}//}}}

double Cadc_u2b::GetCurTime(void)
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
Cadc_u2b::Ctimer::Ctimer(sc_module_name name, const unsigned int timer_num): Cgeneral_timer(name)
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
Cadc_u2b::Ctimer::~Ctimer()
{//{{{
}//}}}

//Functions
void Cadc_u2b::Ctimer::InitializeGeneralTimer(const unsigned int start_value, const unsigned int end_value)
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

void Cadc_u2b::Ctimer::EnableReset (const bool is_active)
{//{{{
    mCountingEvent.cancel();
    Cgeneral_timer::EnableReset(is_active);
}//}}}

void Cadc_u2b::Ctimer::cmpMatchOutputHandling( bool value)
{//{{{
    StartCounting(mNextValue);
    mADtimerEndCountingEvent[mTimerNum].notify();
}//}}}

void Cadc_u2b::Ctimer::StartCounting (const unsigned int start_value)
{//{{{
    mStartValue = start_value;
    mCountingEvent.notify(SC_ZERO_TIME);
}//}}}

void Cadc_u2b::Ctimer::CountingMethod()
{//{{{
    InitializeGeneralTimer(mStartValue, 0);
    Cgeneral_timer::setClkCountPeriod(mPeriod);
    Cgeneral_timer::setSTR(true);
}//}}}

void Cadc_u2b::Ctimer::StopCounting ()
{//{{{
    Cgeneral_timer::setSTR(false);
}//}}}

void Cadc_u2b::PVCR_VALUEmUpdate ()
{//{{{
   unsigned int pvcrValue = PVCR_VALUEm.read();
   (*PWDVCR)["GCTRL"]      = (pvcrValue     ) & 0x3F;
   (*PWDVCR)["VCULLMTBS"]  = (pvcrValue >> 6) & 0xF;
   (*PWDVCR)["MPXV"]       = (pvcrValue >>10) & 0x7;
   (*PWDVCR)["MPXE"]       = (pvcrValue >>13) & 0x1;
   (*PWDVCR)["WTTS"]       = (pvcrValue >>14) & 0xF;
}//}}}

void Cadc_u2b::PVCR_TRGmMethod ()
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
void Cadc_u2b::INT_ADEmThread ()
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
// RH850_U2B added
void Cadc_u2b::ULE_LPSmMethod(void)
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

void Cadc_u2b::INT_ERRmThread(void)
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

bool Cadc_u2b::AllSGStopCheck ()
{//{{{
    for(unsigned int group_number = 0; group_number < emGroupNum-1; group_number++){
        if ((*SGSR[group_number])["SGACT"] != 0) {
            return false;
        }
    }
    if ((*SGDIAGSR)["SDACT"] != 0) {
        return false;
    }
    return true;
}//}}}

bool Cadc_u2b::PinLevelSGDiagCheck(void)
{//{{{
    for (unsigned int vc = 0; vc < emVCSGDiag; vc++) {
        if ((*SGDIAGVCR[vc])["CNVCLS"] == emTerminalDiagMode) {
            return true;
        }
    }
    return false;
}//}}}


bool Cadc_u2b::AllHWTriggerDisableCheck ()
{//{{{
    for(unsigned int group_number = 0; group_number < emGroupNum-1; group_number++){
        if ((*SGCR[group_number])["TRGMD"] != 0) {
            return false;
        }
    }
    return true;
}//}}}

bool Cadc_u2b::UpdatePhysicalChannelSGD (bool last_channel)
{//{{{
    unsigned int mPCCR0_value = (unsigned int)(*SGDIAGPCCR0);
    unsigned int mPCCR1_value = (unsigned int)(*SGDIAGPCCR1);
    bool mPCCR0_done = 0;
    if (last_channel) {
        mPCCR0_finish = 0;
        mTargetSGD_0 = 0;
        mTargetSGD_1 = 0;
        return true;
    } else {
        if ((mPCCR0_value == 0 ) && (mPCCR1_value == 0)) {
            re_printf("warning","There is no target physical channel selected for SG-Diag.\n");
            return false;
        } else {
            if (!mPCCR0_finish) {
                if (mPCCR0_value != 0) {
                    for (unsigned int i = mTargetSGD_0; i < emMaxSGDChannel; i++) {
                        if (((mPCCR0_value >> i) & 0x1) == 0x1) {
                            mTargetSGD_0 = i+1;
                            mPCCR0_done = true;
                            break;
                        }

                    }
                    if (!mPCCR0_done) {
                        mPCCR0_finish = 1;
                    }
                } else {
                    mPCCR0_finish = 1;
                }
            } 
            if (mPCCR0_finish) {
                if (mPCCR1_value != 0) {
                    for (unsigned int i = mTargetSGD_1; i < emMaxSGDChannel; i++) {
                        if (((mPCCR1_value >> i) & 0x1) == 0x1) {
                            mTargetSGD_1 = i+1;
                            break;
                        }
                    }
                }
            }
            if (mPCCR1_value != 0) {
                for (unsigned int i = 0; i <emMaxSGDChannel; i++) {
                    if (((mPCCR1_value >> (31-i)) & 0x1) == 1){
                        if (i == 32-mTargetSGD_1) {
                            mLastTargetChannel = true;
                        }
                        break;
                    }
                }
            } else {
                for (unsigned int i = 0; i <emMaxSGDChannel; i++) {
                    if (((mPCCR0_value >> (31-i)) & 0x1) == 1){
                        if (i == 32-mTargetSGD_0) {
                            mLastTargetChannel = true;
                        }
                        break;
                    }
                }
            }
            return true;
        }
    }
}//}}}

unsigned int Cadc_u2b::CheckVCULLMTBR(const unsigned int index, const bool isUpper, const unsigned int lmtb, const unsigned int vcAddNumber, bool isSGD, bool sign_bit)
{//{{{
#if 1
    unsigned int checkedData = 0;
    unsigned int dfmt = (unsigned int)((*ADCR2)["DFMT"]);
    if (!isSGD) {
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
    } else {
        switch(dfmt) {
            case 0:
            case 2:
                checkedData = CheckLMTBFormat(lmtb,vcAddNumber,0xFFFE,0xFFFC,0xFFF8);
                break;
            case 1:
            case 4:
            case 5:
                if (sign_bit) {
                    checkedData = CheckLMTBFormat_SGD(lmtb,true,vcAddNumber,0xC000,0xE000,0xF000);
                } else {
                    checkedData = CheckLMTBFormat_SGD(lmtb,true,vcAddNumber,0x0000,0x0000,0x0000);
                }
                break;
            case 6:
            case 7:
                if (sign_bit) {
                    checkedData = CheckLMTBFormat_SGD(lmtb,false,vcAddNumber,0xF000,0xF800,0xFC00);
                } else {
                    checkedData = CheckLMTBFormat_SGD(lmtb,false,vcAddNumber,0x0000,0x0000,0x0000);
                }
                break;
            default: // 3
                checkedData = CheckLMTBFormat(lmtb,vcAddNumber,0xFFF8,0xFFF0,0xFFE0);
                break;
        }
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

int Cadc_u2b::GetSSRData(const int data,unsigned int index, bool sign, bool fix) 
{//{{{
    unsigned int tmp_sign;
    unsigned int tmp_result;
    int tmp_data;
    switch(index) {
        case 10:
            tmp_sign = 0xFC00;
            tmp_result = data&0x3FF;
            break;
        case 11:
            tmp_sign = 0xF800;
            tmp_result = data&0x7FF;
            break;
        case 12:
            tmp_sign = 0xF000;
            tmp_result = data&0xFFF;
            break;
        case 13:
            tmp_sign = 0xE000;
            tmp_result = data&0x1FFF;
            break;
        default: //case 14
            tmp_sign = 0xC000;
            tmp_result = data&0x3FFF;
            break;
    }
    if (!sign) {
        tmp_sign = 0;
    }
    tmp_data = tmp_sign|tmp_result;
    if (fix) {
        switch (index) {
            case 10:
                tmp_data = tmp_data << 5;
                break;
            case 11:
                tmp_data = tmp_data << 4;
                break;
            case 12:
                tmp_data = tmp_data << 3;
                break;
            case 13:
                tmp_data = tmp_data << 2;
                break;
            default: //case 14
                tmp_data = tmp_data << 1;
                break;
        }
    }
    return tmp_data;
}//}}}

int Cadc_u2b::CheckSSRFormat(const int data, const unsigned int vcAddNumber) 
{//{{{
    int tmp_data;
    bool sign_bit = (data>>14)&0x1;
    unsigned int dfmt = (unsigned int)((*ADCR2)["DFMT"]);
    if (vcAddNumber == 1) {
        switch (dfmt) {
            case 0:
            case 2:
                tmp_data = GetSSRData(data,12,sign_bit,true);
                break;
            case 1:
            case 4:
            case 5:
                tmp_data = GetSSRData(data,12,sign_bit,false);
                break;
            case 6:
            case 7:
                tmp_data = GetSSRData(data,10,sign_bit,false);
                break;
            default: // case 3
                tmp_data = GetSSRData(data,10,sign_bit,true);
                break;
        }
     } else if (vcAddNumber == 2) {
        switch (dfmt) {
            case 0:
            case 2:
                tmp_data = GetSSRData(data,13,sign_bit,true);
                break;
            case 1:
            case 4:
            case 5:
                tmp_data = GetSSRData(data,13,sign_bit,false);
                break;
            case 6:
            case 7:
                tmp_data = GetSSRData(data,11,sign_bit,false);
                break;
            default: // case 3
                tmp_data = GetSSRData(data,11,sign_bit,true);
                break;
        }
     } else if (vcAddNumber == 4) {
        switch (dfmt) {
            case 0:
            case 2:
                tmp_data = GetSSRData(data,14,sign_bit,true);
                break;
            case 1:
            case 4:
            case 5:
                tmp_data = GetSSRData(data,14,sign_bit,false);
                break;
            case 6:
            case 7:
                tmp_data = GetSSRData(data,12,sign_bit,false);
                break;
            default: //case 3
                tmp_data = GetSSRData(data,12,sign_bit,true);
                break;
        }
     }
     return tmp_data;
}//}}}

double Cadc_u2b::SelfDiagData(unsigned int index, unsigned int tdlv)
{//{{{
    double data;
    bool is_index_odd = (bool)(index & 0x1);
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
    return data;
}//}}}

unsigned int Cadc_u2b::CheckLMTBFormat_SGD(const unsigned int lmtb, bool twelve_bit, const unsigned int vcAddNumber
      , const unsigned int dataFormat4, const unsigned int dataFormat2, const unsigned int dataFormat1)
{//{{{
    unsigned int data;
    if (twelve_bit) {
        if (vcAddNumber == 4) {
            if ((lmtb & 0xC000) !=  dataFormat4) {
                data = (lmtb & 0x3FFF) | dataFormat4;
            } else {
                data = lmtb;
            } 
        } else if (vcAddNumber == 2) {
            if ((lmtb & 0xE000) !=  dataFormat2) {
                data = (lmtb & 0x1FFF) | dataFormat2;
            } else {
                data = lmtb;
            } 
        } else {
            if ((lmtb & 0xF000) !=  dataFormat1) {
                data = (lmtb & 0x0FFF) | dataFormat1;
            } else {
                data = lmtb;
            } 
        }
    } else {
        if (vcAddNumber == 4) {
            if ((lmtb & 0xF000) !=  dataFormat4) {
                data = (lmtb & 0x0FFF) | dataFormat4;
            } else {
                data = lmtb;
            } 
        } else if (vcAddNumber == 2) {
            if ((lmtb & 0xF800) !=  dataFormat2) {
                data = (lmtb & 0x7FF) | dataFormat2;
            } else {
                data = lmtb;
            } 
        } else {
            if ((lmtb & 0xFC00) !=  dataFormat1) {
                data = (lmtb & 0x03FF) | dataFormat1;
            } else {
                data = lmtb;
            } 
        }
    }
    return data;
}//}}}

bool Cadc_u2b::CheckIsSVMode()
{//{{{
    return ((unsigned int)((*SGSTR)["SVSACT"]) == 0x1 && (unsigned int)((*EMUCR)["SVSDIS"]) == 0x0); 
}//}}}

bool Cadc_u2b::CheckScanMode(const unsigned int group_number, const bool isPWD,
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
                if(isTHB) {
                    mIsRetainedSWTHB[group_number] = true;
                }
            }else {
                if(isTHA) {
                    mIsRetainedHWTHA[group_number] = true;
                }
                if(isTHB) {
                    mIsRetainedHWTHB[group_number] = true;
                }
            }
            mIsRetainedTrigger[group_number] = true;
        }
        return true;
    } else {
        return false;
   }
}//}}}

unsigned int Cadc_u2b::CheckLMTBFormat(const unsigned int lmtb, const unsigned int vcAddNumber
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

void Cadc_u2b::CheckULCR(bool isSGD)
{//{{{
    if(mIsUpperError) {
        if (!isSGD) {
            if((unsigned int)((*SGULCR[mFinalGroup])["UE"]) == 0x0){
                (*SGULCR[mFinalGroup])["UE"] = 0x1;
            }
        } else {
            if((unsigned int)(*SGDIAGULCR)["UE"] == 0x0) {
                (*SGDIAGULCR)["UE"] = 0x1;
            }
        }
    }
    if(mIsLowerError) {
        if (!isSGD) {
            if((unsigned int)((*SGULCR[mFinalGroup])["LE"]) == 0x0){
                (*SGULCR[mFinalGroup])["LE"] = 0x1;
            }
        } else {
            if((unsigned int)(*SGDIAGULCR)["LE"] == 0x0) {
                (*SGDIAGULCR)["LE"] = 0x1;
            }
        }
    }
    if (!isSGD) {
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
    } else {
        if((unsigned int)((*SGDIAGULCR)["ULE"]) == 0x0){
            (*SGDIAGULCR)["ULE"] = 0x1;
            if(mFinalPhyChannel != 0x3F){
                (*SGDIAGULCR)["ULPC"] = mFinalPhyChannel;
            }else {
                re_printf("warning","Physical channel is invalid when checking Upper/Lower Limit.\n");
            }
        }
    }
}//}}}


// vim600: set foldmethod=marker :
