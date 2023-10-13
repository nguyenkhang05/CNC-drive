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

#ifndef __ADC_H__
#define __ADC_H__
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
#include "adc_regif.h"
#include "Cgeneral_timer.h"
#include "re_define.h"

/// ADC model class
class Cadc: public sc_module
           ,public Cadc_regif
{

#include "adc_cmdif.h"
friend class Csarad;

    /// TIMER model class
    class Ctimer: public Cgeneral_timer
    {
    friend class Cadc;

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
    SC_HAS_PROCESS(Cadc);
    Ctimer *mTIMER [2];
    Cadc(sc_module_name name, unsigned int adc_instance);
    ~Cadc();

    //Port declaration
    sc_in<bool> ADmSG0TRG;
    sc_in<bool> ADmSG1TRG;
    sc_in<bool> ADmSG2TRG;
    sc_in<bool> ADmSG3TRG;
    sc_in<bool> ADmSG4TRG;

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
    /// SARAD4 add
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

    // RH850_E2x added for IFC
    sc_in< sc_uint<6> > IFVCHm;
    sc_in<bool>IFRDm;
    sc_in<bool>IFREm;

    sc_out<bool> INT_ADm0;
    sc_out<bool> INT_ADm1;
    sc_out<bool> INT_ADm2;
    sc_out<bool> INT_ADm3;
    sc_out<bool> INT_ADm4;
    sc_out<bool> INT_MPXm;
    sc_out<bool> INT_ADEm;
    sc_out<bool> ADPEm;
    sc_out<bool> ADENDm0;
    sc_out<bool> ADENDm1;
    sc_out<bool> ADENDm2;
    sc_out<bool> ADENDm3;
    sc_out<bool> ADENDm4;
    // RH850_E2x added
    sc_out<bool> DFREQm;
    sc_out< sc_uint<4> > DFTAGm;
    sc_out< sc_uint<16> > DFDATAm;
    // RH850_E2x added for IFC
    sc_out< sc_uint<16> > ADCm_IFDATA;

    // RH850_E2x Enhancement
//    sc_out<bool> *VCEND[emMaxVirChannel];
    sc_out<bool> *VCEND[48];
    sc_out<bool> VCULMO;
    sc_out<bool> VCLLMO;

    // RH850_E2x FCC1/FCC2
    sc_out<bool> ADMPX0;
    sc_out<bool> ADMPX1;
    sc_out<bool> ADMPX2;
    sc_out<bool> ADMPX3;
    sc_out<bool> ADMPX4;

private:
    //Enum
     enum eADCConstance{ 
          emNanoSecond = 1000000000
         ,emGroupNum   = 5
         ,emIDbit      = 5
         ,emDRbit      = 17
         ,emTHchannel  = 6
         ,emFixFormat  = 32768 //2^15
         ,emIntergerFormat  = 4096 //2^12
         ,emFixFormatAllOne = 32760 //0x7FF8
         ,emIntergerFormatAllOne = 4095 //0xFFF
         ,emConstConvert = 2097152
         ,emGroup0 = 0
         ,emGroup1 = 1
         ,emGroup2 = 2
         ,emGroup3 = 3
         ,emGroup4 = 4
         ,emMaxPhyChannel = 32
         ,emMaxVirChannel = 48
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
        ,emDiagnosisMode = 3
        ,emAdditionMode = 4
        ,emMPXMode = 5
        ,emMPXAdditionMode = 6
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
    };

    enum eSecondaryVoltageAnalogPort {
        emVCCPortNum  = 28
       ,emEVCCPortNum = 29
       ,emVDDPortNum  = 30
    };

    //Variable
    unsigned int mADCInstance;

    // Reset
    bool mIsReset;
    bool mResetFlag;
    bool mDisableFlag;
    bool mDisableSGxFlag[emGroupNum];

    //T&H function
    unsigned int mTHChannelState [emTHchannel];
    double mTHSamplingStartTime [emTHchannel];
    bool mTHUpdateChannelState [emTHchannel];
    bool mTHUpdateInput [emTHchannel];
    double mIntTHSample [emTHchannel];
    unsigned int mTHzAssignChannel[emTHchannel];

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
    bool mADPEmActive ;
    unsigned int mErrorId ;
    bool mIsADPEmOperating;
    bool mIsINT_MPXmOperating;
    bool mINT_ADmxINT_ADEmDataActive ;
    bool mINT_ADmxActive [emGroupNum]; 
    bool mADENDmxActive [emGroupNum];
    unsigned int mID ;
    unsigned int mStoreData ;
    unsigned int mFinalData ;

    unsigned int mINT_MPXmActiveNum;
    unsigned int mINT_ADEmActiveNum;
    unsigned int mADPEmActiveNum;
    unsigned int mINT_ADm0ActiveNum;
    unsigned int mINT_ADm1ActiveNum;
    unsigned int mINT_ADm2ActiveNum;
    unsigned int mINT_ADm3ActiveNum;
    unsigned int mINT_ADm4ActiveNum;
    bool mflag_1st_16bit ; //flag when read 1st 16bit DR
    bool mflag_2nd_16bit ; //flag when read 2nd 16bit DR
    unsigned int mFormat_temp[40] ; //store old format
    unsigned int mAD[40] ; //store AD[13:0]

    //VC and Scanning
    unsigned int mEndVCPointer [emGroupNum] ;
    unsigned int mStartVCPointer [emGroupNum];
    bool mSGActive [emGroupNum];
    unsigned int mSuspendVC [emGroupNum] ;
    unsigned int mLastMultipleNumber [emGroupNum];
    bool mGroupSuspendState [emGroupNum] ;
    // RH850_E2x added
    bool mIsUpdateSGVCPR[emGroupNum];   // SGVCPRx is updated

    unsigned int mCurrentGroup ;
    unsigned int mLastGroup ;
    unsigned int mLastVC ;
    unsigned int mFinalVC;
    unsigned int mFinalGroup;
    unsigned int mVMONVDCRVal;
    unsigned int mConvtData;
    unsigned int mULMTB;
    unsigned int mLLMTB;
    unsigned int mCurVC;
    bool mSuspendFlag;
    bool mScanningState;
    bool mOpenCircuitError;

    //Parameter and Information
    double mAvrefh;
    double mAVcc;
    bool mForcedIdError;
    bool mForcedParityError;
    bool mForcedOpenError;
    bool mForcedBreak;
    double mtD;
    double mtED;
    unsigned int mExSmpt;
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

    /// SARAD3
    bool mIsHWTrigger [emGroupNum];           // SGx is trigger by HW or AD Timer
    double mAVccMax;
    double mAVccMin;
    double mAvrefhMin;

    /// SARAD4 add
    unsigned int mADCNum;               // Number of ADC unit
    unsigned int mPhyChannelNum;        // Number of physical channel
    unsigned int mVirChannelNum;        // Number of virtual channel
    unsigned int mMPXInput;             // Analog input for MPX
    //// Added P1M-E and CC-Cube D1D2
    std::string mTargetProduct; 
    unsigned int mTHChannelNum;         // TH channel

    //Event
    sc_event mResetEvent;
    sc_event mDisableEvent;
    sc_event mWritingADSYNSTCREvent [emGroupNum];
    sc_event mWritingSGSTCREvent[emGroupNum];
    
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
    sc_event mADMPXOutputEvent; //RH850-E2x FCC1/FCC2 added  ouput method for ADMPXn0->4
    sc_event mINT_MPXmInterruptEvent;
    sc_event mADPEmInterruptEvent;
    sc_event mADENDmxEvent [emGroupNum];

    //State event
    sc_event mADCChangeStateEvent;
    sc_event mADCStateEvent;

    /// SARAD3
    sc_event mHWTriggerEvent [emGroupNum];          // SGx is trigger by HW or AD Timer

    /// SARAD4 add - fix 0-frequency issue
    sc_event mZeroClockEvent;

    sc_event mVCENDNoticeEvent;

    //============== function==================//
    //tlm function
    bool common_tgt_rd(bool is_dbg, unsigned int addr, unsigned char *p_data, unsigned int size );
    bool common_tgt_wr(bool is_dbg, unsigned int addr, unsigned char *p_data, unsigned int size ); 
    bool ChkAccess (const bool is_wr, const std::string channel_name, const std::string register_name, const std::string bit_name);
    //handleCommand
    void AssignTHCircuit(const unsigned int th_channel, const unsigned int group_channel, const unsigned int physic_channel);

    //operation function
    void HardwareTrigger (const unsigned int group_number, const unsigned int trigger);
    void GroupCheck (const unsigned int group_number);
    bool VCCheck (const unsigned int vc_number);
    unsigned int VCAddition (const unsigned int vc_number);
    unsigned int  GetData (const unsigned int group_number, const unsigned int vc_number, const unsigned int vc_add);
    void ADtimerCheck (const unsigned int group_number);
    double LoadAnalogTerminal (const unsigned int index);
    void ExportData(const unsigned int case_number, const unsigned int group_number, const unsigned int vc_number, unsigned int *data, const unsigned int group_loop);
    double ADCTimeCalculation (const unsigned int vc_number, unsigned int vc_add);
    bool TimingCheck (void);
    bool VoltageCheck (void);
    bool SuspendCheck (unsigned int group_number);
    bool CheckWriteCondition (const unsigned int group_index, const bool is_SGCRwrite, const unsigned int preTRGMDvalue, const unsigned int preADSTARTEvalue);
    bool CheckAllScanGroupStop ();
    double AdjustVMONInput (const double data, const unsigned int index);
    unsigned int GetAD (const unsigned int vc_Format, const unsigned int vc_Data);
    unsigned int ChkWarning (const unsigned int Last_Format, const unsigned int Current_Format);
    unsigned int GetNewData (const unsigned int Current_Format, const unsigned int vc_read_number);
    void ReConvertDR (unsigned int addr);
    void ReConvertDIR (unsigned int addr);
    /// SARAD4 add - fix 0-frequency issue
    void SettingZeroClock (void);
    void CancelAllEvents (void);
    // RH850_E2x added
    void SetULErrorStatus(unsigned int channel);
    bool ULIntrEnable(unsigned int channel);
    double GetCurTime(void);
    bool IsTargetProductE2x(void);

    //call_back function
    void WritingADSYNSTCR (void);
    void WritingADTSYNSTCR (void);
    bool WritingVMONVDCR (const std::string register_name, const std::string bit_name, bool bit_val);
    void cb_VCR_GCTRL(RegCBstr str);
    void cb_SGSTCR_SGST (RegCBstr str);
    void cb_SGSTPCR_SGSTP (RegCBstr str);
    void cb_ADHALTR_HALT (RegCBstr str);
    void cb_ADCR1_SUSMTD (RegCBstr str);
    void cb_MPXCURCR_MSKCFMT (RegCBstr str);
    void cb_ADTSTCR_ADTST (RegCBstr str);
    void cb_ADTENDCR_ADTEND (RegCBstr str);
    void CallBack_DRi (const unsigned int addr) ;
    void CallBack_DIRn (const unsigned int addr) ;
    void cb_SFTCR_ULEIE  (RegCBstr str);
    void cb_TDCR_TDE     (RegCBstr str);
    void cb_ODCR_ODDE    (RegCBstr str);
    void cb_ULLMTBR_ULMTB(RegCBstr str);
    void cb_ECR_ULEC (RegCBstr str);
    void cb_ADCR2_DFMT (RegCBstr str);
    void cb_THSMPSTCR_SMPST (RegCBstr str);
    void cb_THSTPCR_THSTP (RegCBstr str);
    void cb_THCR_ASMPMSK (RegCBstr str);
    void cb_THAHLDSTCR_HLDST (RegCBstr str);
    void cb_THBHLDSTCR_HLDST (RegCBstr str);
    void cb_THACR_SGS (RegCBstr str);
    void cb_THBCR_SGS (RegCBstr str);
    void cb_THER_TH0E (RegCBstr str);
    void cb_THGSR_TH0GS (RegCBstr str);
    void cb_SGCR_ADIE (RegCBstr str);
    void cb_SGMCYCR_MCYC (RegCBstr str);
    void cb_SGVCPR_VCSP (RegCBstr str);
    void cb_ADTIPR_ADTIP (RegCBstr str);
    void cb_ADTPRR_ADTPR (RegCBstr str);
    void cb_SGVCSP_VCSP(RegCBstr str);
    void cb_SGVCEP_VCEP(RegCBstr str);
    void cb_SGVCOWR_VCOW(RegCBstr str);
    void cb_ULLMSR_ULS(RegCBstr str);
    void cb_SMPCR_SMPT(RegCBstr str);
    void cb_VCULLMTBR_VCULMTB(RegCBstr str);
    void cb_VCLMSCR1_VC00LMSC(RegCBstr str);
    void cb_VCLMSCR2_VC32LMSC(RegCBstr str);
    void cb_ADOPDIG_ADOPDIGn0(RegCBstr str);
    void cb_MPXOWR_MPXOW(RegCBstr str);
    void cb_DFASENTSGER_DFENTSG4E(RegCBstr str);
    void cb_MPXINTER_ADMPXIE(RegCBstr str);
    void cb_ADENDP_ENDP(RegCBstr str);
    // RH850_E2x SARAD enhancement
    void cb_MPXCMDR_MPXCMD(RegCBstr str);
    void cb_VCLMINTER1_ADUL00IE(RegCBstr str);
    void cb_VCLMINTER2_ADUL32IE(RegCBstr str);

    //reset_function
    void EnableReset(const bool is_active);
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
    void ADmSG0TRGMethod(void);
    void ADmSG1TRGMethod(void);
    void ADmSG2TRGMethod(void);
    void ADmSG3TRGMethod(void);
    void ADmSG4TRGMethod(void);
    void ScanGroup3ADtimerTriggerMethod(void);
    void ScanGroup4ADtimerTriggerMethod(void);
    void VCENDNoticeThread(void);
    
    //output method
    void INT_MPXmInterruptMethod(void);
    void ADPEmInterruptMethod(void);
    void INT_ADmxINT_ADEmDataMethod(void);
    void ADMPXOutputMethod(void);//RH850-E2x FCC1/FCC2 added  ouput method for ADMPXn0->4
    void ADENDm0Method(void);
    void ADENDm1Method(void);
    void ADENDm2Method(void);
    void ADENDm3Method(void);
    void ADENDm4Method(void);
    // RH850_E2x added
    void IFCProcessMethod(void);

    /// SARAD3
    void HWTriggerScanningMethod(void);            // SGx is trigger by HW or AD Timer
};

#endif //__ADC_H__
