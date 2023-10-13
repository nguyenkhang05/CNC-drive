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

#ifndef __ADC_U2B_H__
#define __ADC_U2B_H__
#include "systemc.h"
#include <iterator>
#include <cerrno>
#include <string>
#include <cstdarg>
#include <fstream>
#include <sstream>
#ifdef _WIN32
    #include <windows.h>
#endif
#include "adc_u2b_regif.h"
#include "Cgeneral_timer.h"
#include "re_define.h"

#ifndef FRACTION_TO_INT
#define FRACTION_TO_INT(d) ((unsigned int)((d - (unsigned long long)(d))*1000000))
#endif

/// ADC_U2B model class
class Cadc_u2b: public sc_module
           ,public Cadc_u2b_regif
{

#include "adc_u2b_cmdif.h"
friend class Csarad_u2b;

    /// TIMER model class
    class Ctimer: public Cgeneral_timer
    {
    friend class Cadc_u2b;

    public:
        SC_HAS_PROCESS(Ctimer);
        Ctimer(sc_module_name name, const unsigned int timer_num);
        ~Ctimer();

    private:
        //enum
        enum eTIMER_CONSTANCE {
            emClkDivide = 1
        };
        sc_event mADtimerEndCountingEvent [2];
        sc_event mCountingEvent;
        //variables
        unsigned int mStartValue;
        unsigned int mNextValue;
        unsigned int mTimerNum;
        double mPeriod;
        //functions
        void InitializeGeneralTimer(const unsigned int start_value, const unsigned int end_value);
        void EnableReset (const bool is_active);
        void cmpMatchOutputHandling( bool value);
        void StartCounting (const unsigned int start_value);
        void StopCounting (void);
        void CountingMethod (void);
    };

public:
    SC_HAS_PROCESS(Cadc_u2b);
    Ctimer *mTIMER [2];
    Cadc_u2b(sc_module_name name, unsigned int adc_instance);
    ~Cadc_u2b();

    //Port declaration
    sc_in<bool> ADmSG0TRG;
    sc_in<bool> ADmSG1TRG;
    sc_in<bool> ADmSG2TRG;
    sc_in<bool> ADmSG3TRG;
    sc_in<bool> ADmSG4TRG;
    sc_in<bool> ADmSGDTRG;
    sc_in< unsigned int > PVCR_VALUEm;
    sc_in<bool> PVCR_TRGm;
    sc_in<bool> ADmTAUD2I7;
    sc_in<bool> ADmTAUD2I15;
    sc_in<bool> ADmTAUJ2;
    sc_in<bool> ADmTAUJ3;
    sc_in<bool> ADmLPS;

    sc_in< double > ANm00;
    sc_in< double > ANm01;
    sc_in< double > ANm02;
    sc_in< double > ANm03;
    sc_in< double > ANm10;
    sc_in< double > ANm11;
    sc_in< double > ANm12;
    sc_in< double > ANm13;
    sc_in< double > ANm20;
    sc_in< double > ANm21;
    sc_in< double > ANm22;
    sc_in< double > ANm23;
    sc_in< double > ANm30;
    sc_in< double > ANm31;
    sc_in< double > ANm32;
    sc_in< double > ANm33;
    sc_in< double > ANm40;
    sc_in< double > ANm41;
    sc_in< double > ANm42;
    sc_in< double > ANm43;
    sc_in< double > ANm50;
    sc_in< double > ANm51;
    sc_in< double > ANm52;
    sc_in< double > ANm53;
    sc_in< double > ANm60;
    sc_in< double > ANm61;
    sc_in< double > ANm62;
    sc_in< double > ANm63;
    sc_in< double > ANm70;
    sc_in< double > ANm71;
    sc_in< double > ANm72;
    sc_in< double > ANm73;
    sc_in< double > ANm80;
    sc_in< double > ANm81;
    sc_in< double > ANm82;
    sc_in< double > ANm83;
    sc_in< double > ANm90;
    sc_in< double > ANm91;
    sc_in< double > ANm92;
    sc_in< double > ANm93;

    // RH850_E2x added for IFC
    sc_in< sc_uint<7> > IFVCHm;
    sc_in<bool>IFRDm;
    sc_in<bool>IFREm;
    sc_in<bool>SV_MODEm;
    sc_out<bool> INT_ADm0;
    sc_out<bool> INT_ADm1;
    sc_out<bool> INT_ADm2;
    sc_out<bool> INT_ADm3;
    sc_out<bool> INT_ADm4;
    sc_out<bool> INT_MPXm;
    sc_out<bool> INT_SGDm;
    sc_out<bool> ADPEm;
    sc_out<bool> ADENDm0;
    sc_out<bool> ADENDm1;
    sc_out<bool> ADENDm2;
    sc_out<bool> ADENDm3;
    sc_out<bool> ADENDm4;
    sc_out<bool> INT_ERRm;
    sc_out<bool> DFREQm;
    sc_out< sc_uint<4> > DFTAGm;
    sc_out< sc_uint<16> > DFDATAm;
    sc_out< sc_uint<16> > ADCm_IFDATA;

    sc_out < bool > PVCR_ENDm;
    sc_out < unsigned short > PVCR_PWDDRm;
    sc_out<bool> *VCEND[96];
    sc_out<bool> PWM_VCEND;
    sc_out<bool> VCULMO;
    sc_out<bool> VCLLMO;

    sc_out< sc_uint<3> > MPXCURm;
    sc_out < unsigned char > ULE_LPSm;

private:
    //Enum
     enum eADCConstance{ 
          emNanoSecond = 1000000000
         ,emGroupNum   = 6
         ,emADENDPinNum = 5
         ,emIDbit      = 5 
         ,emDRbit      = 15 //Confirm
         ,emMPXVbit    = 3
         ,emTHchannel  = 6
         ,emSTPDchannel = 3
         ,emSGDchannel = 3
//         ,emTxSELNum   = 6
         ,emFixFormat  = 32768 //2^15 //Confirm
         ,emIntegerFormat  = 4096 //2^12 //Confirm
         ,emFixFormatAllOne = 32760 //0x7FF8 //Confirm
         ,emIntegerFormatAllOne = 4095 //0xFFF //Confirm
         ,emConstConvert = 2097152
         ,emGroup0 = 0
         ,emGroup1 = 1
         ,emGroup2 = 2
         ,emGroup3 = 3
         ,emGroup4 = 4
         ,emGroupSGDiag = 5
         ,emVCSGDiag = 3
         ,emMaxSGDChannel = 32
         ,emMaxPhyChannel = 40
         ,emMaxVirChannel = 96 
         ,emHWTriggerSelectNum = 5 
     };
     enum eError{
          emIDError = 1 
         ,emOverwriteError = 4
         ,emBoundaryError = 8
         ,emADEAssert = 16 
     };
     enum eMode{
         emNormalMode = 0
        ,emTHMode = 1
        ,emExSamplingPeriodMode = 2
        ,emDiagnosisMode = 3
        ,emAdditionMode = 4
        ,emMPXMode = 5
        ,emMPXAdditionMode = 6
        ,emTerminalDiagMode = 7
        ,emDisconnectionMode = 8
        ,emPdMode = 9
        ,emPuMode = 10
        ,emDisconnectionDiagMode = 11
        ,emPdDiagMode = 12
        ,emPuDiagMode = 13
        ,emDataPathMode = 14
        ,emPWSDMode = 15
     };
     enum eTHState{
         emTHIdleState = 0
        ,emTHSamplingState = 1
        ,emTHHoldingState = 2
     };
     enum eConvertState{
         emStartConvert = 0
        ,emCorruptConvert = 1
        ,emFinishConvert = 2
     };

     enum eADCState{
         emADCResetState = 0
        ,emADCIdleState = 1
        ,emADCTrackingState = 2
        ,emADCTrackingScanningState = 4
        ,emADCScanningState = 3
     };

    enum eDelayTimeCoeff {
        emtDCoeffPCLK = 3
       ,emtDCoeffADCLK = 5
       ,emtSPLCoeff = 18
       ,emtSARCoeff = 22
       ,emtEDCoeffPCLK = 3
       ,emtEDCoeffADCLK = 3
       ,emtTHDCoeff = 7
       ,emtTHSDCoeff = 5
       ,emtTHLDCoeff = 10
       ,emtTSPLCoeff = 27
       ,emtSVSACTCoeff = 2
    };

    enum eSecondaryVoltageAnalogPort {
        emVCCPortNum  = 20
       ,emEVCCPortNum = 21
       ,emISOVDDPortNum  = 22
       ,emAWOVDDPortNum  = 23
    };

    enum eADC_RESET {
        emADC_P_RESET = 0
       ,emADC_AD_RESET = 1
       //,emADC_ALL_RESET = 2
       ,emADC_P_RESETA = 2
       ,emADC_AD_RESETA = 3
    };

    enum eSpecialVirtualChannel {
        emVcPWM = 112  // 70H
    };

    //Variable
    unsigned int mADCInstance;

    // Reset
    bool mIsPReset;
    bool mIsADReset;
    bool mResetFlag;
    bool mDisableFlag;
    bool mDisableOneSGFlag;
    bool mDisableSGxFlag[emGroupNum];
    bool mEnterSVModeFlag;
    bool mExitSVModeFlag;
    bool mPWMFlag;
    bool mSGDiagFlag;

    //T&H function
    unsigned int mTHChannelState [emTHchannel];
    double mTHSamplingStartTime [emTHchannel];
    bool mTHUpdateChannelState [emTHchannel];
    bool mTHUpdateInput [emTHchannel];
    double mIntTHSample [emTHchannel];
    unsigned int THzAssignChannel[emTHchannel];

    bool mTHUpdateChannelStateFlag;
    bool mTHStop;
    bool mTHSamplingManual;
    bool mTHEnable;
    bool mTHASoftwareTriggerHolding;
    bool mTHBSoftwareTriggerHolding;
    bool mTHAPortTriggerHolding;
    bool mTHBPortTriggerHolding;
    bool mTHAHoldingProcess;
    bool mTHBHoldingProcess;
    bool mIsScanningProcess[emGroupNum];

    unsigned int mGroupANumber ;
    unsigned int mGroupBNumber ;

    //Output
    bool mINT_MPXmActive ;
#if 0
    bool mINT_ULmActive ;
#else
    unsigned char mULE_LPSmActive;
#endif
    bool mADPEmActive ;
    unsigned int mErrorId ;
    bool mIsUpperError;
    bool mIsLowerError;
    unsigned int mPhyChannelTH [emTHchannel];
    unsigned int mPhyChannel;
    unsigned int mFinalPhyChannel;
    bool mIsADPEmOperating;
    bool mIsINT_MPXmOperating;
    bool mINT_ADmxINT_ADEmDataActive ;
    unsigned int mTargetSGD_0;
    unsigned int mTargetSGD_1;
    bool mPCCR0_finish;
    bool mLastTargetChannel;
#if 0
    bool mINT_ADEmActive ;
    bool mINT_ADEmProcess ;
#else
    // RH850_U2B added
    bool mINT_ERRmActive ;
    bool mINT_ERRmProcess ;
#endif
    bool mINT_ADmxActive [emGroupNum]; 
    bool mADENDmxActive [emGroupNum-1];
    bool mPWMEndActive ;
    unsigned int mID ;
    unsigned int mStoreData ;
    unsigned int mFinalData ;
    unsigned int mFinalDataSRR0 ;
    unsigned int mFinalDataSRR1 ;

    unsigned int mINT_MPXmActiveNum;
#if 0
    unsigned int mINT_ADEmActiveNum;
#else
    // RH850_U2B added
    unsigned int mINT_ERRmActiveNum;
#endif
    unsigned int mADPEmActiveNum;
    unsigned int mINT_ADm0ActiveNum;
    unsigned int mINT_ADm1ActiveNum;
    unsigned int mINT_ADm2ActiveNum;
    unsigned int mINT_ADm3ActiveNum;
    unsigned int mINT_ADm4ActiveNum;
    unsigned int mINT_SDmActiveNum;

    //VC and Scanning
    unsigned int mEndVCPointer [emGroupNum] ;
    unsigned int mStartVCPointer [emGroupNum];
    bool mSGActive [emGroupNum];
    unsigned int mSuspendVC [emGroupNum] ;
    unsigned int mLastMultipleNumber [emGroupNum];
    bool mGroupSuspendState [emGroupNum] ;
    bool mIsUpdateSGVCPR[emGroupNum];   // SGVCPRx is updated
    bool mIsStartSG[emGroupNum];
    bool mIsSuspendSGDiag;

    unsigned int mCurrentGroup ;
    unsigned int mLastGroup ;
    unsigned int mLastVC ;
    unsigned int mFinalVC;
    unsigned int mFinalGroup;
    unsigned int mVMONVDCRVal;
    bool mULMTB;
    bool mLLMTB;
    unsigned int mCurVC;
    bool mSuspendFlag;
    bool mScanningState;
    bool mOpenCircuitError;
    bool mSgDiagStop;
    bool mStrongPullDown[emSTPDchannel];

    //Parameter and Information
    double mAvrefh;
    double mAVcc;
    bool mForcedIdError;
    bool mForcedParityError;
    bool mForcedOpenError;
    bool mForcedBreak;
    bool mForcedDataPathError;
    double mtD;
    double mtED;
    double mtWait;
    unsigned int mExSmpt;
    unsigned int mSmpt;
    double mtSAR;
    double mtTHLD;
    double mtTHSD;
    double mtTSPL;
    double mPCLKPeriod;
    double mADCLKPeriod;
    bool mDumpInterrupt;
    bool mEnableConvertInfo;
    bool mEnableTimeCalculation;

    //State
    unsigned int mADCState;
    bool mADCChangeState;

    bool mIsHWTrigger [emGroupNum-1];           // SGx is trigger by HW or AD Timer
    bool mIsRetainedTrigger [emGroupNum];
    bool mIsRetainedADsyn [emGroupNum-1];
    bool mIsRetainedSW [emGroupNum];
    bool mIsRetainedSWTHA [emGroupNum-1];
    bool mIsRetainedSWTHB [emGroupNum-1];
    bool mIsRetainedHWTHA [emGroupNum-1];
    bool mIsRetainedHWTHB [emGroupNum-1];
    bool mIsRetainedPWD;
    double mAVccMax;
    double mAVccMin;
    double mAvrefhMin;

    unsigned int mADCNum;               // Number of ADC unit
    unsigned int mPhyChannelNum;        // Number of physical channel
    unsigned int mVirChannelNum;        // Number of virtual channel
    unsigned int mMPXInput;             // Analog input for MPX
    std::string mTargetProduct; 
    unsigned int mTHChannelNum;         // TH channel
    bool mIsDisconnectionMode;
    unsigned int mHWTriggerSelectNum;    //Trigger input for ADCA

    unsigned int mFinalCnvcls;
    unsigned int mFinalMpxv;
    unsigned int mFinalMpxe;
    unsigned int mFinalDfent;
    unsigned int mFinalDftag;
    unsigned int mFinalDfmt;

    //Event
    sc_event mPResetEvent;
    sc_event mADResetEvent;
    sc_event mDisableEvent;
    sc_event mWritingADSYNSTCREvent [emGroupNum-1];
    sc_event mWritingSGSTCREvent[emGroupNum];
    sc_event mSGSTTriggerEvent [emGroupNum];
    sc_event mADSYNTriggerEvent[emGroupNum-1];
    
    //Boundary excess event
    sc_event mBoundaryExcessEvent;
    //Event scanning
    sc_event mScanningEvent;
    sc_event mSuspendEvent;

    //Event sampling and holding
    sc_event mTHUpdateInputEvent;
    sc_event mTHSamplingEvent;
    sc_event mTHStopEvent;
    sc_event mTHEnableEvent;
    sc_event mTHASoftwareTriggerHoldingEvent;
    sc_event mTHBSoftwareTriggerHoldingEvent;
    sc_event mTHAPortTriggerHoldingEvent;
    sc_event mTHBPortTriggerHoldingEvent;
    sc_event mTHUpdateChannelStateEvent;
    sc_event mTHGAHoldingEvent;
    sc_event mTHGBHoldingEvent;

    //Ouput event 
    sc_event mINT_ADmxINT_ADEmDataEvent;
    sc_event mADMPXOutputEvent; 
    sc_event mINT_MPXmInterruptEvent;
    sc_event mADPEmInterruptEvent;
    sc_event mADENDmxEvent [emGroupNum-1];
#if 0
    sc_event mINT_ADEmEvent;
#else
    sc_event mULE_LPSmEvent;
    sc_event mINT_ERRmEvent;
#endif

    //State event
    sc_event mADCChangeStateEvent;
    sc_event mADCStateEvent;

    sc_event mHWTriggerEvent [emGroupNum-1];          // SGx is trigger by HW or AD Timer

    sc_event mZeroClockEvent;

    sc_event mVCENDNoticeEvent;

    sc_event mSVModeEvent;

    //============== function==================//
    //tlm function
    bool common_tgt_rd(bool is_dbg, unsigned int addr, unsigned char *p_data, unsigned int size );
    bool common_tgt_wr(bool is_dbg, unsigned int addr, unsigned char *p_data, unsigned int size ); 
    bool ChkAccess (const bool is_wr, const std::string channel_name, const std::string register_name, const std::string bit_name);
    //handleCommand
    void AssignTHCircuit(const unsigned int th_channel, const unsigned int group_channel, const unsigned int physic_channel);

    //operation function
    void GroupCheck (const unsigned int group_number);
    void HardwareTrigger (const unsigned int group_number, const unsigned int trigger);
    bool VCCheck (const unsigned int vc_number);
    unsigned int VCAddition (const unsigned int vc_number, const unsigned int group_number);
    unsigned int  GetData (const unsigned int group_number, const unsigned int vc_number, const unsigned int vc_add);
    void ADtimerCheck (const unsigned int group_number);
    double LoadAnalogTerminal (const unsigned int index);
    void ExportData(const unsigned int case_number, const unsigned int group_number, const unsigned int vc_number, unsigned int *data, const unsigned int group_loop);
    double ADCTimeCalculation (const bool is_start_SG,const unsigned int vc_number, const unsigned int vc_add, const unsigned int group_number);
    bool TimingCheck (void);
    bool VoltageCheck (void);
    bool SuspendCheck (unsigned int group_number);
    double AdjustVMONInput (const double data, const unsigned int index);
    void SettingZeroClock (void);
    void CancelAllEvents (void);
    void SetULErrorStatus(unsigned int channel, bool isSGD);
    bool ULIntrEnable(unsigned int channel, bool isSGD);
    double GetCurTime(void);
    void CheckSGDiagAddition(void);
    bool AllSGStopCheck (void);
    bool PinLevelSGDiagCheck(void);
    bool AllHWTriggerDisableCheck (void);
    bool UpdatePhysicalChannelSGD(bool last_channel);
    unsigned int CheckVCULLMTBR(const unsigned int index, const bool isUpper, const unsigned int lmtb, const unsigned int vcAddNumber, bool isSGD, bool sign_bit);
    unsigned int CheckLMTBFormat_SGD(const unsigned int lmtb, bool twelve_bit, const unsigned int vcAddNumber,const unsigned int dataFormat4, const unsigned int dataFormat2, const unsigned int dataFormat1);
    double SelfDiagData(unsigned int index, unsigned int tdlv);
    int GetSSRData(const int data,unsigned int index, bool sign, bool fix);
    int CheckSSRFormat(const int data, const unsigned int vcAddNumber);
    bool CheckIsSVMode(void);
    bool CheckScanMode(const unsigned int group_number, const bool isPWD = false, const bool isSWTrigger = false, 
            const bool isSyn = false, const bool isTHA = false, const bool isTHB = false);
    void RunRetainedTrigger(const unsigned int group_number);

    //call_back function
    void WritingADSYNSTCR (void);
    void WritingADTSYNSTCR (void);
    void WritingVMONVDCR (const std::string register_name, const std::string bit_name, bool bit_val);
    void cb_VCR_GCTRL(RegCBstr str);
    void cb_SGDIAGVCR_CNVCLS(RegCBstr str);
    void cb_SGSTCR_SGST(RegCBstr str);
    void cb_SGSTPCR_SGSTP(RegCBstr str);
    void cb_SGCR_ADIE(RegCBstr str);
    void cb_SGVCPR_VCSP(RegCBstr str);
    void cb_SGMCYCR_MCYC(RegCBstr str);
    void cb_SGVCSP_VCSP(RegCBstr str);
    void cb_SGVCEP_VCEP(RegCBstr str);
    void cb_ADTSTCR_ADTST(RegCBstr str);
    void cb_ADTENDCR_ADTEND(RegCBstr str);
    void cb_ADTIPR_ADTIP(RegCBstr str);
    void cb_ADTPRR_ADTPR(RegCBstr str);
    void cb_PWDCR_PWE(RegCBstr str);
    void cb_PWDSGCR_TRGMD(RegCBstr str);
    void cb_SGDIAGSTCR_SGST(RegCBstr str);
    void cb_SGDIAGSTPCR_SGSTP(RegCBstr str);
    void cb_SGDIAGCR_TRGMD(RegCBstr str);
    void cb_SGDIAGPCCR0_SDPCE0(RegCBstr str);
    void cb_SGDIAGPCCR1_SDPCE32(RegCBstr str);
    void cb_ADHALTR_HALT(RegCBstr str);
    void cb_ADCR1_SUSMTD(RegCBstr str);
    void cb_ADCR2_DFMT(RegCBstr str);
    void cb_SMPCR_SMPT(RegCBstr str);
    void cb_MPXCURCR_MSKCFMT(RegCBstr str);
    void cb_MPXINTER_ADMPXIE(RegCBstr str);
    void cb_MPXCURR2_MPXCUR(RegCBstr str);
    void cb_MPXCMDR_MPXCMD(RegCBstr str);
    void cb_MPXOWR_MPXOW(RegCBstr str);
    void cb_TOCER_TOCESD(RegCBstr str);
    void cb_DFASENTSGER_ASENTSG0E(RegCBstr str);
    void CallBack_DRi (const unsigned int addr) ;
    void CallBack_DIRn (const unsigned int addr) ;
    void CallBack_SGDIAGDIRn (const unsigned int addr) ;
    void CallBack_SGDIAGDR01(void);
    void CallBack_SGDIAGDR2(void);
    void CallBack_PWDDR (void) ;
    void CallBack_PWDDIR (void) ;
#if 0
//    void cb_GTMENTSGER_GTMENTSG0E(RegCBstr str);
#endif
    void cb_ADENDP_ENDP(RegCBstr str);
    void cb_THSMPSTCR_SMPST(RegCBstr str);
    void cb_THSTPCR_THSTP(RegCBstr str);
    void cb_THCR_ASMPMSK(RegCBstr str);
    void cb_THAHLDSTCR_HLDST(RegCBstr str);
    void cb_THBHLDSTCR_HLDST(RegCBstr str);
    void cb_THGSR_TH0GS(RegCBstr str);
    void cb_THER_TH0E(RegCBstr str);
    void cb_THACR_SGS(RegCBstr str);
    void cb_THBCR_SGS(RegCBstr str);
    void cb_WAITTR_WAITTIME(RegCBstr str);
    void cb_EMUCR_SVSDIS(RegCBstr str);
    void cb_TDCR_TDLV(RegCBstr str);
    void cb_ODCR_WADDE(RegCBstr str);
    void cb_SFTCR_IDEIE(RegCBstr str);
    void cb_TOCCR_TOCE(RegCBstr str);
    void cb_ECR_IDEC(RegCBstr str);
    void cb_STPDCR_SPDE(RegCBstr str);
    void cb_VCULLMTBR_VCULMTB(RegCBstr str);
    void cb_VCLMINTER1_ADUL00IE(RegCBstr str);
    void cb_VCLMINTER2_ADUL32IE(RegCBstr str);
    void cb_PWVCLMINTER_PWADULIE(RegCBstr str);
    void cb_SDVCLMINTER_SDADULIE(RegCBstr str);
    void cb_VCLMSCR1_VC00LMSC(RegCBstr str);
    void cb_VCLMSCR2_VC32LMSC(RegCBstr str);
    void cb_PWVCLMSCR_PWVCLMSC(RegCBstr str);
    void cb_SDVCLMSCR_SDVC0LMSC(RegCBstr str);
    void cb_SGULCCR_SGULCC(RegCBstr str);
    void cb_VCLMASCR_ALLMSC(RegCBstr str);
    void cb_SGTSEL_TxSEL0(RegCBstr str);

    //reset_function
    void EnableReset(const bool is_active,const unsigned int port_reset);
    void MuxADC2Trigger(const unsigned int index);
    void GlobalInit(void);

    //information
    void DumpInfo (const char *type, const char *message, ...);
    void DumpStatInfo (void);
    void DumpInterruptMessage (const std::string intr_name, const std::string intr_number, const std::string issue);
    void DumpADCActivity (const double start_time, const double end_time);

    //====================== method ==================//
    //T&H method
    void THCircuitMethod(void);
    void THAHoldingEndMethod (void);
    void THBHoldingEndMethod (void);
    void THzMethod (const unsigned int index);
    void TriggerADCAMethod (const unsigned int index);

    //ADC state method
    void ADCStateMethod (void);

    //scaning method
    void ScanningThread(void);
    void BoundaryExcessThread(void);
    void ScanGroup0SoftwareTriggerMethod(void);
    void ScanGroup1SoftwareTriggerMethod(void);
    void ScanGroup2SoftwareTriggerMethod(void);
    void ScanGroup3SoftwareTriggerMethod(void);
    void ScanGroup4SoftwareTriggerMethod(void);
    void ScanGroupDiagSoftwareTriggerMethod(void);
    void ScanGroup0SGSTTriggerMethod(void);
    void ScanGroup1SGSTTriggerMethod(void);
    void ScanGroup2SGSTTriggerMethod(void);
    void ScanGroup3SGSTTriggerMethod(void);
    void ScanGroup4SGSTTriggerMethod(void);
    void ScanGroupDiagSGSTTriggerMethod(void);
    void ScanGroup0ADSYNTriggerMethod(void);
    void ScanGroup1ADSYNTriggerMethod(void);
    void ScanGroup2ADSYNTriggerMethod(void);
    void ScanGroup3ADSYNTriggerMethod(void);
    void ScanGroup4ADSYNTriggerMethod(void);
    void ADmSG0TRGMethod(void);
    void ADmSG1TRGMethod(void);
    void ADmSG2TRGMethod(void);
    void ADmSG3TRGMethod(void);
    void ADmSG4TRGMethod(void);
    void ADmSGDTRGMethod(void);
    void ScanGroup3ADtimerTriggerMethod(void);
    void ScanGroup4ADtimerTriggerMethod(void);
    void VCENDNoticeThread(void);
    
    //output method
    void INT_MPXmInterruptMethod(void);
    void ADPEmInterruptMethod(void);
    void INT_ADmxINT_ADEmDataMethod(void);
    void ADMPXOutputMethod(void);
    void ADENDm0Method(void);
    void ADENDm1Method(void);
    void ADENDm2Method(void);
    void ADENDm3Method(void);
    void ADENDm4Method(void);
#if 0
    void INT_ADEmThread(void);
#else
    // RH850_U2B added
    void ULE_LPSmMethod(void);
    void INT_ERRmThread(void);
#endif
    // RH850_E2x added
    void IFCProcessMethod(void);
    void SVmModeMethod(void);
    void SVStopMethod(void);
    void HWTriggerScanningMethod(void);            // SGx is trigger by HW or AD Timer

    void PVCR_VALUEmUpdate(void);
    void PVCR_TRGmMethod(void);

    void CheckULCR(bool isSGD);
    unsigned int CheckLMTBFormat(const unsigned int lmtb, const unsigned int vcAddNumber
            , const unsigned int dataFormat4, const unsigned int dataFormat2, const unsigned int dataFormat1);
};

#endif //__ADC_U2B_H__
