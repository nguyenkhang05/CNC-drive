// ---------------------------------------------------------------------
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
// ---------------------------------------------------------------------

#ifndef __SARAD113x_H__
#define __SARAD113x_H__
#include "systemc.h"
#include "sarad113x_regif.h"
#include "sarad113x_fsm.h"
#include "tlm_tgt_if.h"
#include <iterator>

/// SARAD113x model class
class Csarad113x: public sc_module
                   , public Csarad113x_regif
                   , public vpcl::tlm_tgt_if<32 >
{
#include "sarad113x_cmdif.h"
#include "sarad113x_fsmif.h"
public:    
    // Input ports
    sc_in <sc_dt::uint64> pclk;
    sc_in <sc_dt::uint64> ADCLK;
    sc_in <bool> preset_n;
    sc_in <double> AVREFH0;

    sc_in <bool> TSN_TRG;
    sc_in <bool> SG1_TRG;
    sc_in <bool> SG2_TRG;
    sc_in <bool> SG3_TRG;
    sc_in <bool> PVCR_TRG;

    sc_in <double> ANI00;
    sc_in <double> ANI01;
    sc_in <double> ANI02;
    sc_in <double> ANI03;
    sc_in <double> ANI04;
    sc_in <double> ANI05;
    sc_in <double> ANI06;
    sc_in <double> ANI07;
    sc_in <double> ANI08;
    sc_in <double> ANI09;
    sc_in <double> ANI10;
    sc_in <double> ANI11;
    sc_in <double> ANI12;
    sc_in <double> ANI13;
    sc_in <double> ANI14;
    sc_in <double> ANI15;
    sc_in <double> ANI16;
    sc_in <double> ANI17;
    sc_in <double> ANI18;
    sc_in <double> ANI19;
    sc_in <double> ANI20;
    sc_in <double> ANI21;
    sc_in <double> ANI22;
    sc_in <double> ANI23;
    sc_in <double> ANI24;
    sc_in <double> ANI25;
    sc_in <double> ANI26;
    sc_in <double> ANI27;
    sc_in <double> ANI28;
    sc_in <double> ANI29;
    sc_in <double> ANI30;
    sc_in <double> ANI31;
    sc_in <double> ANI32;
    sc_in <double> ANI33;
    sc_in <double> ANI34;
    sc_in <double> ANI35;
    sc_in <double> TSN_ANI; // Dedicated port for TSN

    sc_in <sc_uint<12> > PVCR_VALUE;
    sc_in <sc_uint<16> > ADOP_OPA1_DATA;
    sc_in <sc_uint<16> > ADOP_OPA2_DATA;
    sc_in <sc_uint<16> > ADOP_OPA3_DATA;

    // Output ports
    sc_out <bool> INT_TSN;
    sc_out <bool> INT_SG1;
    sc_out <bool> INT_SG2;
    sc_out <bool> INT_SG3;
    sc_out <bool> INT_ADE;
    sc_out <bool> ADCATCNV0;
    sc_out <bool> ADCATCNV1;
    sc_out <bool> ADCATCNV2;
    sc_out <bool> ADCATCNV3;
    sc_out <bool> ADCATCNV4;
    sc_out <bool> ULE;
    sc_out <sc_uint<3> > PVCR_MUXCUR;
    sc_out <bool> PVCR_END;
    sc_out <bool> TSN_TS_EN;
    sc_out <bool> TSN_TSSW;
    sc_out <bool> TSN_TSSW_DISCH;
    sc_out <bool> TSN_TSMASK;
    sc_out <bool> TSN_SELF_DIAG;
    sc_out <sc_uint<2> > TSN_TRIM;
    sc_out <bool> ADOP_OPA1_PSEL;
    sc_out <bool> ADOP_OPA1_WEN;
    sc_out <bool> ADOP_OPA2_PSEL;
    sc_out <bool> ADOP_OPA2_WEN;
    sc_out <bool> ADOP_OPA3_PSEL;
    sc_out <bool> ADOP_OPA3_WEN;

    SC_HAS_PROCESS(Csarad113x);
    Csarad113x(sc_module_name name);
    ~Csarad113x();

    // Event and function for wrapper
    sc_event mWrittenPWDATAEvent;
    unsigned int GetWrittenData(void);

private:
    //Enum
    enum eSARAD113x_CONSTANCE {
        emNanoSecond = 1000000000
       ,emMaxPhyChannel = 36 
       ,emMaxVirChannel = 50
       ,emMaxMultiCyc   = 0x03
       ,emMinSmpNum     = 0x12
       ,emMinStartTSNConv = 200000 // Minimum time (200us) to start A/D conversion of TSN group (from writing to TSNCR)
       ,emDGOUTNum      = 3        // Number of DGOUTSH input value for self-diagnosis
       ,emDRMask0       = 0xFFFF0000
       ,emDRMask1       = 0x0000FFFF
       ,emTSNOWECAP     = 60        // Overwrite error capture value of TSN
       ,emMaxADData     = 0xFFF     // Maximum value of A/D conversion
       ,emMinTHSmpTime  = 450       // Minimum T&H sampling time (450ns)
    };
    enum eADConversionType {
        em12bit         = 0         // A/D conversion 12-bit
       ,em10bit         = 1         // A/D conversion 10-bit
       ,em12bitCoff     = 4096      // 2^12
    };
    enum eADAlignType {
        emRightAlign    = 0         // Right align (store to bits 27-16/11-0 or 25-16/9-0 )
       ,emLeftAlign     = 1         // Left align  (store to bits 31-20/15-4 or 31-22/15-6)
    };
    enum eScanningGroupNum {
        emTSNSG = 0       // Scanning group 0
       ,emSG1   = 1       // Scanning group 1  
       ,emSG2   = 2       // Scanning group 2  
       ,emSG3   = 3       // Scanning group 3  
       ,emPWDSG = 4       // Scanning group 4  
       ,emAllSG = 5       // All scanning group (0~4)
       ,emAllSGn = 6      // All scanning group n (n = 1~3)
    };
    enum eTHChannel {
        emTHCh0 = 0       // T&H channel 0 (ANI00)
       ,emTHCh1 = 1       // T&H channel 1 (ANI01)
       ,emTHCh2 = 2       // T&H channel 2 (ANI02)
       ,emTHCh3 = 3       // T&H channel 3 (ANI03)
       ,emTHCh4 = 4       // T&H channel 4 (ANI04)
       ,emTHCh5 = 5       // T&H channel 5 (ANI05)
       ,emAllTHCh = 6       // All T&H channel
    };
    enum eTHGroup {
        emTHGroupA = 0       // T&H group A
       ,emTHGroupB = 1       // T&H group B
       ,emAllTHGroup = 2     // All T&H group
       ,emDisableTH = 3      // Disable T&H
    };
    enum ePrioritySet {
        emPriority0 = 0x43210       // Priority of scanning group: PWDSG>SG3>SG2>SG1>TSNSG
       ,emPriority1 = 0x32401       // Priority of scanning group: SG3>SG2>PWDSG>TSNSG>SG1
    };
    enum eSuspendMode {
        emSyncMode   = 0x0          //  Synchronous suspend mode
       ,emHybridMode = 0x1          //  Synchronous suspend mode for interrupting 2-4, Asynchronous suspend mode for interrupting 0-1
       ,emAsyncMode  = 0x2          //  Asynchronous suspend mode
    };
    enum eTSNControlState {
        emAssertTSSW
       ,emAssertTSSW_DISCH_EN
       ,emDeassertTSSW_DISCH_EN
       ,emAssertTSMASK
       ,emDeassertTSMASK
       ,emDeassertTSSW
       ,emDeassertAll
    };
    enum eScanningMode {
        emMultiCycleMode        // Multi cycle mode SGCRn.SCANMD = 0 (n=1~3)
       ,emContinuousMode        // Continuous mode SGCRn.SCANMD = 1 (n=1~3)
    };
    enum eInputPortGroupNum {
        emANIGroup1 = 16           // For ANI00-ANI15
       ,emANIGroup2 = 20           // For ANI16-ANI35
    };
    enum eUpperLowerBoundCheck {
        emDisableULCheck = 0x0  // Do not check Upper/Lower bound
       ,emULCheck0  = 0x1       // Check Upper/Lower bound with ULLMTBR0
       ,emULCheck1  = 0x2       // Check Upper/Lower bound with ULLMTBR1
       ,emULCheck2  = 0x3       // Check Upper/Lower bound with ULLMTBR2
       ,emMinULMTB10bit = 0x3   // Fixed value of ULLMTBRn.ULMTB[1:0] in 10bit A/D conversion
       ,emMinLLMTB10bit = 0x0   // Fixed value of ULLMTBRn.LLMTB[1:0] in 10bit A/D conversion
    };
    enum eSelfDiagPortNum {
        emSelfNum36 = 36        // Self-diagnosis port number 36 (connect to DGOUTAD)
    };
    enum eVCCheckStatus {       // Indicate status when checking to continue A/D conversion
        emSuspend
       ,emLastVC
       ,emContinue
    };
    enum eTriggerType {
        emHWTrigger             // H/W trigger
       ,emSWTrigger             // S/W trigger
       ,emOtherTrigger          // Hold start, hold complete trigger
    };
    //Variable 
    bool mIsInitialize;
    bool mIsRefVolUpdate;
    unsigned int mResetPeriod;
    bool mSARPortResetFlag;
    bool mSARCmdResetFlag;

    bool mIsOperating; // Inidicate SARAD113x is operating
    bool mADOPControlVal[3];
    bool mADCATCNVnVal[emAllSG];
    bool mIntrVal[emAllSG];
    bool mULEVal;
    bool mADEVal;
    unsigned int mPVCR_MUXCURVal;
    unsigned int mPrioritySet;

    double mAccessTimeTSNCR;

    unsigned int mCurrentSG;
    unsigned int mPreviousVC;
    unsigned int mCurrentStartVC[emAllSG];
    unsigned int mNextVC[emAllSG];
    unsigned int mFirstVC[emAllSG];
    unsigned int mLastVC[emAllSG];
    unsigned int mSGACTVal[emAllSG];
    unsigned int mSHACTVal;
    unsigned int mScanFreqCount[emAllSG];
    unsigned int mCurrentTrigger;
    unsigned int mTSNStateControl;
    unsigned int mRepetitionTime;
    unsigned int mRepetitionCount;
    unsigned int mADData;
    unsigned int mULError;
    bool mIsEnableStart[emAllSG];
    bool mIsScanning[emAllSG];
    bool mIsSuspend[emAllSG];
    bool mIsFirstTimeConv[emAllSG];
    bool mIsHWTrigger[emAllSG];
    bool mIsSWTrigger[emAllSG];
    bool mIsLastRepetition;
    double mStartTHSamplingTime[emAllTHCh];
    double mHoldPortVal[emAllTHCh];
    double mCurrentAnalogVal;
    double mDGOUTAD;
    double mDGOUTSH[emDGOUTNum];
    double mStartTimeVC;

    unsigned int mINTActiveNum[emAllSG];
    unsigned int mINTADEActiveNum;
    unsigned int mULEActiveNum;
    // Variable for wrapper
    unsigned int mPWDATAVal;

    //Event
    sc_event mCmdResetEvent;
    sc_event mCmdCancelResetEvent;
    sc_event mAssertResetEvent;
    sc_event mZeroClockEvent;
    sc_event mSuspendEvent;
    sc_event mClearDREvent[(emMaxVirChannel+1)/2];
    sc_event mClearDIREvent[emMaxVirChannel];
    #ifdef SARAD113x_ENABLE_TSN
    sc_event mClearTSNDREvent;
    sc_event mClearTSNDIREvent;
    sc_event mWriteTSNControlEvent;
    sc_event mWriteTSN_TS_ENEvent;
    sc_event mWriteTSN_SELF_DIAGEvent;
    #endif
    sc_event mClearPWDDREvent;
    sc_event mClearPWDDIREvent;
    sc_event mWriteADOPControlEvent;
    sc_event mWritePVCR_MUXCUREvent;
    sc_event mWriteADCATCNVControlEvent[emAllSG];
    sc_event mWriteSGEndInterruptEvent[emAllSG];
    sc_event mWriteULEInterruptEvent;
    sc_event mWriteADEInterruptEvent;
    sc_event mHWTriggerEvent[emAllSG];
    sc_event mSWTriggerEvent[emAllSG];
    sc_event mCheckTriggerMethodEvent;
    sc_event mUpdateSGACTEvent[emAllSG];
    sc_event mUpdateSHACTEvent;
    sc_event mStartTHSamplingEvent;
    sc_event mStartVCSamplingEvent;
    sc_event mStartVCConversionEvent;
    sc_event mEndVCConversionEvent;
    sc_event mUpdateConversionDataEvent[emAllSG];

    //Parameter and Information
    double mPreAvrefh;
    double mPreAVcc;
    double mPreAVss;
    unsigned int mPreEX_HLD_CDT;
    unsigned int mPreEX_CNVT;
    double mPretD;
    double mPretPWDD;
    double mPretED;
    bool   mPreEnableTimeCalculation;
    double mPCLKPeriod;
    double mADCLKPeriod;

    //tlm function
    void tgt_acc (tlm::tlm_generic_payload &trans, sc_time &t);
    unsigned int  tgt_acc_dbg(tlm::tlm_generic_payload &trans);

    double GetTimeResolution(void);
    void SetLatency_TLM(const double pclk_period, const bool is_constructor);
    void SetCLKfreq(std::string clk_name, double clk_freq);
    std::string CommandCB(const std::vector<std::string>& args);

    //handle_command function
    void DumpStatInfo(void);
    void AssertReset(const double start_time, const double period);

    //Normal function
    void EnableReset(const bool is_active);
    unsigned int GetTRGMD(const unsigned int sg);
    void Initialize(void);
    void InitOperation(void);
    void ClearScanningEndFlag(const unsigned int sg);
    void WriteADOPControl(const unsigned int sg);
    double NextPCLKPosedge(double offset);
    double NextADCLKPosedge(double offset);
    double GetANIPortVal(unsigned int port_index);
    void UpdateSelfDiag(void);

    void SetCurrentSG(unsigned int group,bool is_th);
    void StartScanning(unsigned int sg, bool is_th);
    void FinishScanning(unsigned int sg);
    bool CheckSGSetting(unsigned int sg, bool is_th);
    void SuspendScanning(unsigned int group, bool is_th);
    void StartVCConv(unsigned int sg);
    void FinishVCConv(unsigned int sg);
    void UpdateInternalCount(unsigned int sg);
    bool CheckSuspend(unsigned int trg_num, unsigned int current_sg);
    unsigned int IsLastVC(unsigned int sg);
    void CheckTrigger(void);
    bool ComparePriority(unsigned int check_sg, unsigned int current_sg);
    void SetStartSmpTime(unsigned int channel);
    bool CheckSmpTime(unsigned int group, bool hw_trg);
    bool IsReset(void);
    bool CheckTH(unsigned int sg);
    bool CheckEnableTH(unsigned int group);
    bool CheckTHStart(unsigned int group);
    void StartHoldProcess(unsigned int group);
    void HoldPortVal(unsigned int group);
    void DelayEndHolding(unsigned int group);
    void ResumeTH(void);
    void StartTrigger(unsigned int sg, double delay_time);
    bool IsAutoStartSampling(void);
    bool CheckEnableStart(unsigned int sg);
    bool CheckHoldStart(unsigned int group, bool is_trg, eTriggerType trg_type);
    bool CheckHoldComplete(unsigned int group, eTriggerType trg_type);
    void EndHolding(unsigned int group);
    void HWTrigger(unsigned int sg);
    void SWTrigger(unsigned int sg);
    bool IsContinuousMode(unsigned int sg);
    void FinishTHConversion(void);
    void StopOperation(void);
    void AssertADCATCNVTH(unsigned int sg);
    double GetSampleTime(unsigned int sg);
    double GetConversionTime(void);
    unsigned int ADConvert(double value, unsigned int sg, unsigned int vc_num);
    bool StoreADData(unsigned int data, unsigned int sg, unsigned int vc_num);
    unsigned int GetRepetitionTime(unsigned int sg);
    unsigned int GetMPXE(unsigned int sg, unsigned int vc_num);
    unsigned int GetMPXV(unsigned int sg, unsigned int vc_num);
    unsigned int GetADIE(unsigned int sg, unsigned int vc_num);
    unsigned int GetULS(unsigned int sg, unsigned int vc_num);
    unsigned int GetGCTRL(unsigned int sg, unsigned int vc_num);
    unsigned int GetCNVCLSSelfDiag(unsigned int vc_num);
    void ClearDRProcess(unsigned int sg, unsigned int channel);
    void ClearDIRProcess(unsigned int sg, unsigned int vc_num);
    void PrintVCmessage(std::string msg, unsigned int sg, unsigned int vc_num);
    double GetCurTime(void);
    
    //information
    void DumpInfo (const char *type, const char *message, ...);
    void DumpInterruptMessage (const std::string intr_name, const bool is_assert);
    void DumpActivity(unsigned int sg, unsigned int vc_num, double start_time);

    //method
    void PCLKMethod(void);
    void ADCLKMethod(void);
    void ResetMethod(void);
    void AssertResetMethod(void);
    void DeAssertResetMethod(void);
    #ifdef SARAD113x_ENABLE_TSN
    void TSNTRGMethod(void);
    void WriteTSN_TS_ENMethod(void);
    void WriteTSN_SELF_DIAGMethod(void);
    void WriteTSNControlMethod(void);
    #endif
    void SG1TRGMethod(void);
    void SG2TRGMethod(void);
    void SG3TRGMethod(void);
    void PVCRTRGMethod(void);
    void PVCR_VALUEMethod(void);
    void WriteADOPControlMethod(void);
    void WritePVCR_MUXCURMethod(void);
    void WriteADCATCNVControlMethod(unsigned int sg);
    void WriteSGEndInterruptMethod(unsigned int sg);
    void WriteULEInterruptMethod(void);
    void WriteADEInterruptMethod(void);
    void AVREFHMethod(void);
    void InitialAVREFHMethod(void);
    void HWTriggerProcessMethod(unsigned int sg);
    void SWTriggerProcessMethod(unsigned int sg);
    void CheckTriggerMethod(void);
    void UpdateSGACTMethod(unsigned int sg);
    void UpdateSHACTMethod(void);
    void StartTHSamplingMethod(void);
    void VCSamplingMethod(void);
    void VCConversionMethod(void);
    void VCEndConversionMethod(void);
    void UpdateConversionDataMethod(unsigned int sg);
    void ClearDRMethod(unsigned int channel);
    #ifdef SARAD113x_ENABLE_TSN
    void ClearTSNDRMethod(void);
    void ClearTSNDIRMethod(void);
    #endif
    void ClearPWDDRMethod(void);
    void ClearDIRMethod(unsigned int vc);
    void ClearPWDDIRMethod(void);

    // Callback functions
    bool CheckAccess(const unsigned int sg, vpcl::re_register* reg, RegCBstr str);
    void cb_VCR_GCTRL(RegCBstr str);
    void cb_TSNVCR_ULS(RegCBstr str);
    void cb_DR_DR0(RegCBstr str);
    void cb_PWDTSNDR_TSNDR(RegCBstr str);
    void cb_DIR_DR(RegCBstr str);
    void cb_TSNDIR_TSNDR(RegCBstr str);
    void cb_PWDDIR_PWDDR(RegCBstr str);
    void cb_ADHALTR_HALT(RegCBstr str);
    void cb_ADCR_SUSMTD(RegCBstr str);
    void cb_TSNCR_TSNEN(RegCBstr str);
    void cb_THSMPSTCR_SMPST(RegCBstr str);
    void cb_THCR_ASMPMSK(RegCBstr str);
    void cb_THAHLDSTCR_HLDST(RegCBstr str);
    void cb_THBHLDSTCR_HLDST(RegCBstr str);
    void cb_THACR_SGS(RegCBstr str);
    void cb_THBCR_SGS(RegCBstr str);
    void cb_THER_TH0E(RegCBstr str);
    void cb_THGSR_TH0GS(RegCBstr str);
    void cb_SFTCR_OWEIE(RegCBstr str);
    void cb_ULLMTBR_ULMTB(RegCBstr str);
    void cb_ECR_ULEC(RegCBstr str);
    void cb_DGCTL0_PSEL0(RegCBstr str);
    void cb_DGCTL1_CDG00(RegCBstr str);
    void cb_PDCTL1_PDNA00(RegCBstr str);
    void cb_PDCTL2_PDNB00(RegCBstr str);
    void cb_SMPCR_SMPT(RegCBstr str);
    void cb_TSNSMPCR_TSNSMPT(RegCBstr str);
    void cb_EMUCR_SVSDIS(RegCBstr str);
    void cb_SGPRCR_SGPR0(RegCBstr str);
    void cb_SGSTCR_SGSTn(RegCBstr str);
    void cb_TSNSGSTCR_TSNSGST(RegCBstr str);
    void cb_PWDSGSTCR_PWDSGST(RegCBstr str);
    void cb_SGCR_ADIE(RegCBstr str);
    void cb_TSNSGCR_TSNTRGMD(RegCBstr str);
    void cb_PWDSGCR_PWDTRGMD(RegCBstr str);
    void cb_SGSEFCR_SEFCn(RegCBstr str);
    void cb_TSNSGSEFCR_TSNSEFC(RegCBstr str);
    void cb_PWDSGSEFCR_PWDSEFC(RegCBstr str);
    void cb_SGVCSP_VCSP(RegCBstr str);
    void cb_SGVCEP_VCEP(RegCBstr str);
    void cb_SGMCYCR_MCYC(RegCBstr str);
    void cb_SGTSEL_TxSEL00(RegCBstr str);
};

#endif //__SARAD113x_H__

