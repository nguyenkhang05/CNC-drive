// ----------------------------------------------------------------------
// $Id$
//
// Copyright(c) 2012 Renesas Electronics Corporation
// Copyright(c) 2012 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ----------------------------------------------------------------------

#ifndef __ECM_H__
#define __ECM_H__
#include "systemc.h"
#include <sstream>
#include "tlm.h"
#include "tlm_tgt_if.h"
#include "Cgeneral_timer.h"
#include "ecm_regif.h"
#include "re_define.h"
#include <iterator>

//===============Cecm class=======================
class Cecm: public Cecm_regif
           ,public Cgeneral_timer
{
#include "ecm_cmdif.h"
friend class Cecm_wp;

private:
    //Declare enum type
    enum eECM {
        emStatusRegLength   = 32        //The length of error status register
        ,emErrorFactor      = 93        //The number of factor
        ,emFixCmdRegValue   = 165       //The fix value of command register (0xA5)
    };
    enum eRegisterArea {
        emMasterArea        = 0         //The register area of Master block
        ,emCheckerArea      = 1         //The register area of Checker block
        ,emCommonArea       = 2         //The common register area
        ,emNoneArea         = 3         //None area is chosen
    };
    enum eDelayTimer {
        emClkDivide         = 1         //The clock divide - a parameter need to set into StartStopCounter
    };
    enum eECMErrorOutputLevel {
        emECMErrorOutputActiveLevel         = 0
        ,emECMErrorOutputInactiveLevel      = 1
    };
    enum eECMInterruptLevel {
        emECMInterruptActiveLevel           = 1
        ,emECMInterruptInactiveLevel        = 0
    };
    enum eECMInternalResetLevel {
        emECMInternalResetActiveLevel       = 0
        ,emECMInternalResetInactiveLevel     = 1
    };

    //Structure
    struct ECMErrorInputElement {
        sc_in<bool>* errorin;               //The sgaterrin signal
        sc_out<bool>* pseudo_error_out;     //The sgatpe signal
        bool errorin_pre;                   //The previous value of errorin
        vpcl::bit_info* esstr;              //The corresponding ESSTR bit
        vpcl::bit_info* micfg;              //The corresponding MICFG bit
        vpcl::bit_info* nmicfg;             //The corresponding NMICFG bit
        vpcl::bit_info* ircfg;              //The corresponding IRCFG bit
        vpcl::bit_info* emk;                //The corresponding EMK bit
        vpcl::bit_info* esstc;              //The corresponding clear status bit
        vpcl::bit_info* pe;                 //The corresponding PE bit
        vpcl::bit_info* midtmcfg;          //The corresponding maskable interrupt DTMCFG bit
        vpcl::bit_info* nmidtmcfg;           //The corresponding non-maskable interrupt DTMCFG bit
        ECMErrorInputElement(sc_in<bool>* errorin, sc_out<bool>* pseudo_error_out, bool errorin_pre,  
        vpcl::bit_info* esstr, vpcl::bit_info* micfg, vpcl::bit_info* nmicfg, vpcl::bit_info* ircfg,
        vpcl::bit_info* emk, vpcl::bit_info* esstc, vpcl::bit_info* pe, vpcl::bit_info* midtmcfg, vpcl::bit_info* nmidtmcfg){
            if ((errorin != NULL) && (pseudo_error_out != NULL)   
            && (esstr != NULL) && (micfg != NULL) && (nmicfg != NULL) && (ircfg != NULL) && (emk != NULL) && (esstc != NULL) && (pe != NULL)
            && (midtmcfg != NULL) && (nmidtmcfg != NULL)){
                this->errorin = errorin;
                this->pseudo_error_out = pseudo_error_out;
                this->errorin_pre = errorin_pre;
                this->esstr = esstr;
                this->micfg = micfg;
                this->nmicfg = nmicfg;
                this->ircfg = ircfg;
                this->emk = emk;
                this->esstc = esstc;
                this->pe = pe;
                this->midtmcfg = midtmcfg;
                this->nmidtmcfg = nmidtmcfg;
            }
        }
    };
    std::vector<ECMErrorInputElement> mErrorInputTable;

    std::vector<sc_out<bool>* > mForcedPortArray;

    struct ProtectedRegisterElement {
        std::string register_name;      //The register name
        unsigned int addr;              //The address of this register
        eRegisterArea register_area;    //The register area
        ProtectedRegisterElement(std::string register_name, unsigned int addr, eRegisterArea register_area){
            this->register_name = register_name;
            this->addr = addr;
            this->register_area = register_area;
        }
    };
    std::vector<ProtectedRegisterElement> mProtectedRegisterTable;

    //Declare event
    sc_event        mWriteSGATERROZPortEvent;
    sc_event        mWriteSGATERROUTZPortEvent;
    sc_event        mWriteSGATRESZPortEvent;
    sc_event        mWriteSGATIPortEvent;
    sc_event        mWriteSGATNMIPortEvent;
    sc_event        mWriteSGATPSOUTPortEvent;
    sc_event        mWriteDTMTRGOUTPortEvent;
    sc_event        mWriteErrOutMskClrPortEvent;
    sc_event        mClearErrorStatusEvent;
    sc_event        mCombineSignalEvent;
    sc_event        mECMInitializeEvent;
    sc_event        mEnableClearOutputErrEvent;
    sc_event        mWritePseudoErrorOutEvent;

    //Declare internal variable
    unsigned int                mNorFinishStep;         //Finished step in NORMAL protection sequence
    eRegisterArea               mNorCurRegArea;         //The current protected register area in NORMAL protection sequence
    std::string                 mNorCurRegister;        //The name of written register in NORMAL protection sequence
    std::string                 mNorProtectedRegister;  //The name of current protected register in NORMAL protection sequence
    unsigned int                mRequestValue;          //The request value user want to write in protected register
    unsigned int                mBreFinishStep;         //Finished step in BREAK state of protection sequence
    eRegisterArea               mBreCurRegArea;         //The current protected register area in BREAK state of protection sequence
    std::string                 mBreCurRegister;        //The name of written register in BREAK state of protection sequence
    std::string                 mBreProtectedRegister;  //The name of current protected register in BREAK state of protection sequence
    bool                        mSGATI;                 //The value of sgati
    bool                        mSGATNMI;               //The value of sgatnmi
    bool                        mSGATRESZ;              //The value of sgatresz port
    bool                        mSGATERROZ;             //The value of sgaterroz port
    bool                        mSGATERROUTZ;           //The value of sgaterroutz port
    bool                        mSyncSignal;            //The value of 'sync' signal
    bool                        mSGATPSOUT;             //The value of sgatpsout
    bool                        mDTMTRGOUT;             //The value of dtmtrgout
    bool                        mIsECMReset;            //Indicate ECM (Master/Checker) is in reset period
    bool                        mHoldSignal;            //Variable shows status of enabling clear error output signal
    double                      mCountClock;            //Store value of cntclk
    double                      mPCLKClock;             //Store value of pclk
    double                      mRetartCounterTime;     //Time when one error occurs 
    unsigned int                mECMEOUTCLRT;
    unsigned int                mECMmESSTR0;
    unsigned int                mECMmESSTR1;
    unsigned int                mECMmESSTR2;
    bool                        mPseudoErrorArray[emErrorFactor];

    //ECM inherited virtual functions
    void cmpMatchOutputHandling (bool value);

    //ECM Method
    void ECMReceiveErrorInputMethod(void);
    void LoopBackSignalProcessingMethod(void);
    void ClearBreakInfoMethod(void);
    void ECMInitializeMethod(void);
    void WriteSGATERROZPortMethod(void);
    void WriteSGATERROUTZPortMethod(void);
    void WriteSGATRESZPortMethod(void);
    void WriteSGATIPortMethod(void);
    void WriteSGATNMIPortMethod(void);
    void WriteDTMTRGOUTPortMethod(void);
    void WriteSGATPSOUTPortMethod(void);
    void WriteErrOutMskClrPortMethod(void);
    void resstg1zMethod(void);
    void resstg1z_pclkinMethod(void);
    void sgattinMethod(void);
    void sgatpsinMethod(void);
    void ClearErrorStatusMethod(void);
    void DelayTimerConfigureMethod(void);
    void EnableClearOutputErrMethod(void);
    void ResetForErrpinMethod(void); 
    void WritePseudoErrorOutMethod(void);


    //ECM internal functions
    bool CheckProtectedRegister(unsigned int addr);
    bool WriteProtectedRegister(unsigned int addr, unsigned int data, unsigned int size);
    bool CheckBreakProtectionSequence(unsigned int addr, unsigned int data, unsigned int size);
    bool CheckNormalProtectionSequence(unsigned int addr, unsigned int data, unsigned int size);
    bool ProtectionErrorProcess(const bool is_break, unsigned int data);
    void IssueErrorOutput(void);
    void CalculateErrorOutput(void);
    void IssueInterrupt(void);
    void IssueInternalReset(void);
    void StartStopCounter(const bool start, const double clk_period);
    void InitializeGeneralTimer(void);
    void ErrorInputTableConstruction(void);
    void ProtectedRegisterTableConstruction(void);
    void UpdateErrorStatus(void);
    void ECMEnableReset(const bool is_active);
    void ECMSetZeroClock(void);
    bool ECMCheckZeroClock(const bool dump_warning);
    bool ECM_common_tgt_rd(const bool is_rd_dbg, unsigned int addr, unsigned char *p_data, unsigned int size);
    bool ECM_common_tgt_wr(const bool is_wr_dbg, unsigned int addr, unsigned char *p_data, unsigned int size);
    double GetCurTime(void);

    //Register IF callback functions
    void cb_ECMmESET_ECMmEST(RegCBstr str);
    void cb_ECMmECLR_ECMmECT(RegCBstr str);
    void cb_ECMMICFG0_ECMMIE000(RegCBstr str);
    void cb_ECMMICFG1_ECMMIE100(RegCBstr str);
    void cb_ECMMICFG2_ECMMIE200(RegCBstr str);
    void cb_ECMNMICFG0_ECMNMIE000(RegCBstr str);
    void cb_ECMNMICFG1_ECMNMIE100(RegCBstr str);
    void cb_ECMNMICFG2_ECMNMIE200(RegCBstr str);
    void cb_ECMIRCFG0_ECMIRE000(RegCBstr str);
    void cb_ECMIRCFG1_ECMIRE100(RegCBstr str);
    void cb_ECMIRCFG2_ECMIRE200(RegCBstr str);
    void cb_ECMEMK0_ECMEMK000(RegCBstr str);
    void cb_ECMEMK1_ECMEMK100(RegCBstr str);
    void cb_ECMEMK2_ECMEMK200(RegCBstr str);
    void cb_ECMESSTC0_ECMCLSSE000(RegCBstr str);
    void cb_ECMESSTC1_ECMCLSSE100(RegCBstr str);
    void cb_ECMESSTC2_ECMCLSSE200(RegCBstr str);
    void cb_ECMPE0_ECMPE000(RegCBstr str);
    void cb_ECMPE1_ECMPE100(RegCBstr str);
    void cb_ECMPE2_ECMPE200(RegCBstr str);
    void cb_ECMDTMCTL_ECMSTP(RegCBstr str);
    void cb_ECMDTMR_ECMDTMR(RegCBstr str);
    void cb_ECMDTMCMP_ECMDTMCMP(RegCBstr str);
    void cb_ECMDTMCFG0_ECMDTMCFG000(RegCBstr str);
    void cb_ECMDTMCFG1_ECMDTMCFG100(RegCBstr str);
    void cb_ECMDTMCFG2_ECMDTMCFG200(RegCBstr str);
    void cb_ECMDTMCFG3_ECMDTMCFG300(RegCBstr str);
    void cb_ECMDTMCFG4_ECMDTMCFG400(RegCBstr str);
    void cb_ECMDTMCFG5_ECMDTMCFG500(RegCBstr str);
    void cb_ECMEOUTCLRT_ECMEOUTCLRT(RegCBstr str);

public:
    //Declare input ports
    sc_in<bool>             preset_n;
    sc_in<sc_dt::uint64 >   pclk;
    sc_in<bool>             erroutresz;
    sc_in<sc_dt::uint64 >   cntclk;
    //sgaterrinN (N = 00 .. 92)
    //{{{
    sc_in<bool>             sgaterrin00;
    sc_in<bool>             sgaterrin01;
    sc_in<bool>             sgaterrin02;
    sc_in<bool>             sgaterrin03;
    sc_in<bool>             sgaterrin04;
    sc_in<bool>             sgaterrin05;
    sc_in<bool>             sgaterrin06;
    sc_in<bool>             sgaterrin07;
    sc_in<bool>             sgaterrin08;
    sc_in<bool>             sgaterrin09;
    sc_in<bool>             sgaterrin10;
    sc_in<bool>             sgaterrin11;
    sc_in<bool>             sgaterrin12;
    sc_in<bool>             sgaterrin13;
    sc_in<bool>             sgaterrin14;
    sc_in<bool>             sgaterrin15;
    sc_in<bool>             sgaterrin16;
    sc_in<bool>             sgaterrin17;
    sc_in<bool>             sgaterrin18;
    sc_in<bool>             sgaterrin19;
    sc_in<bool>             sgaterrin20;
    sc_in<bool>             sgaterrin21;
    sc_in<bool>             sgaterrin22;
    sc_in<bool>             sgaterrin23;
    sc_in<bool>             sgaterrin24;
    sc_in<bool>             sgaterrin25;
    sc_in<bool>             sgaterrin26;
    sc_in<bool>             sgaterrin27;
    sc_in<bool>             sgaterrin28;
    sc_in<bool>             sgaterrin29;
    sc_in<bool>             sgaterrin30;
    sc_in<bool>             sgaterrin31;
    sc_in<bool>             sgaterrin32;
    sc_in<bool>             sgaterrin33;
    sc_in<bool>             sgaterrin34;
    sc_in<bool>             sgaterrin35;
    sc_in<bool>             sgaterrin36;
    sc_in<bool>             sgaterrin37;
    sc_in<bool>             sgaterrin38;
    sc_in<bool>             sgaterrin39;
    sc_in<bool>             sgaterrin40;
    sc_in<bool>             sgaterrin41;
    sc_in<bool>             sgaterrin42;
    sc_in<bool>             sgaterrin43;
    sc_in<bool>             sgaterrin44;
    sc_in<bool>             sgaterrin45;
    sc_in<bool>             sgaterrin46;
    sc_in<bool>             sgaterrin47;
    sc_in<bool>             sgaterrin48;
    sc_in<bool>             sgaterrin49;
    sc_in<bool>             sgaterrin50;
    sc_in<bool>             sgaterrin51;
    sc_in<bool>             sgaterrin52;
    sc_in<bool>             sgaterrin53;
    sc_in<bool>             sgaterrin54;
    sc_in<bool>             sgaterrin55;
    sc_in<bool>             sgaterrin56;
    sc_in<bool>             sgaterrin57;
    sc_in<bool>             sgaterrin58;
    sc_in<bool>             sgaterrin59;
    sc_in<bool>             sgaterrin60;
    sc_in<bool>             sgaterrin61;
    sc_in<bool>             sgaterrin62;
    sc_in<bool>             sgaterrin63;
    sc_in<bool>             sgaterrin64;
    sc_in<bool>             sgaterrin65;
    sc_in<bool>             sgaterrin66;
    sc_in<bool>             sgaterrin67;
    sc_in<bool>             sgaterrin68;
    sc_in<bool>             sgaterrin69;
    sc_in<bool>             sgaterrin70;
    sc_in<bool>             sgaterrin71;
    sc_in<bool>             sgaterrin72;
    sc_in<bool>             sgaterrin73;
    sc_in<bool>             sgaterrin74;
    sc_in<bool>             sgaterrin75;
    sc_in<bool>             sgaterrin76;
    sc_in<bool>             sgaterrin77;
    sc_in<bool>             sgaterrin78;
    sc_in<bool>             sgaterrin79;
    sc_in<bool>             sgaterrin80;
    sc_in<bool>             sgaterrin81;
    sc_in<bool>             sgaterrin82;
    sc_in<bool>             sgaterrin83;
    sc_in<bool>             sgaterrin84;
    sc_in<bool>             sgaterrin85;
    sc_in<bool>             sgaterrin86;
    sc_in<bool>             sgaterrin87;
    sc_in<bool>             sgaterrin88;
    sc_in<bool>             sgaterrin89;
    sc_in<bool>             sgaterrin90;
    sc_in<bool>             sgaterrin91;
    sc_in<bool>             sgaterrin92;
    //}}}
    sc_in<bool>             sgatpsin;
    sc_in<bool>             sgaterrlbz;
    sc_in<bool>             sgattin;
    sc_in<bool>             dtmtrgin;
    sc_in<bool>             resstg1z;
    sc_in<bool>             resstg1z_pclkin;
    sc_in<bool>             svaccess;
    sc_in<bool>             errout_clear_mask_in; 

    //Declare output ports
    //sgatpeN (N = 000 .. 092)
    //{{{
    sc_out<bool>            sgatpe000;
    sc_out<bool>            sgatpe001;
    sc_out<bool>            sgatpe002;
    sc_out<bool>            sgatpe003;
    sc_out<bool>            sgatpe004;
    sc_out<bool>            sgatpe005;
    sc_out<bool>            sgatpe006;
    sc_out<bool>            sgatpe007;
    sc_out<bool>            sgatpe008;
    sc_out<bool>            sgatpe009;
    sc_out<bool>            sgatpe010;
    sc_out<bool>            sgatpe011;
    sc_out<bool>            sgatpe012;
    sc_out<bool>            sgatpe013;
    sc_out<bool>            sgatpe014;
    sc_out<bool>            sgatpe015;
    sc_out<bool>            sgatpe016;
    sc_out<bool>            sgatpe017;
    sc_out<bool>            sgatpe018;
    sc_out<bool>            sgatpe019;
    sc_out<bool>            sgatpe020;
    sc_out<bool>            sgatpe021;
    sc_out<bool>            sgatpe022;
    sc_out<bool>            sgatpe023;
    sc_out<bool>            sgatpe024;
    sc_out<bool>            sgatpe025;
    sc_out<bool>            sgatpe026;
    sc_out<bool>            sgatpe027;
    sc_out<bool>            sgatpe028;
    sc_out<bool>            sgatpe029;
    sc_out<bool>            sgatpe030;
    sc_out<bool>            sgatpe031;
    sc_out<bool>            sgatpe032;
    sc_out<bool>            sgatpe033;
    sc_out<bool>            sgatpe034;
    sc_out<bool>            sgatpe035;
    sc_out<bool>            sgatpe036;
    sc_out<bool>            sgatpe037;
    sc_out<bool>            sgatpe038;
    sc_out<bool>            sgatpe039;
    sc_out<bool>            sgatpe040;
    sc_out<bool>            sgatpe041;
    sc_out<bool>            sgatpe042;
    sc_out<bool>            sgatpe043;
    sc_out<bool>            sgatpe044;
    sc_out<bool>            sgatpe045;
    sc_out<bool>            sgatpe046;
    sc_out<bool>            sgatpe047;
    sc_out<bool>            sgatpe048;
    sc_out<bool>            sgatpe049;
    sc_out<bool>            sgatpe050;
    sc_out<bool>            sgatpe051;
    sc_out<bool>            sgatpe052;
    sc_out<bool>            sgatpe053;
    sc_out<bool>            sgatpe054;
    sc_out<bool>            sgatpe055;
    sc_out<bool>            sgatpe056;
    sc_out<bool>            sgatpe057;
    sc_out<bool>            sgatpe058;
    sc_out<bool>            sgatpe059;
    sc_out<bool>            sgatpe060;
    sc_out<bool>            sgatpe061;
    sc_out<bool>            sgatpe062;
    sc_out<bool>            sgatpe063;
    sc_out<bool>            sgatpe064;
    sc_out<bool>            sgatpe065;
    sc_out<bool>            sgatpe066;
    sc_out<bool>            sgatpe067;
    sc_out<bool>            sgatpe068;
    sc_out<bool>            sgatpe069;
    sc_out<bool>            sgatpe070;
    sc_out<bool>            sgatpe071;
    sc_out<bool>            sgatpe072;
    sc_out<bool>            sgatpe073;
    sc_out<bool>            sgatpe074;
    sc_out<bool>            sgatpe075;
    sc_out<bool>            sgatpe076;
    sc_out<bool>            sgatpe077;
    sc_out<bool>            sgatpe078;
    sc_out<bool>            sgatpe079;
    sc_out<bool>            sgatpe080;
    sc_out<bool>            sgatpe081;
    sc_out<bool>            sgatpe082;
    sc_out<bool>            sgatpe083;
    sc_out<bool>            sgatpe084;
    sc_out<bool>            sgatpe085;
    sc_out<bool>            sgatpe086;
    sc_out<bool>            sgatpe087;
    sc_out<bool>            sgatpe088;
    sc_out<bool>            sgatpe089;
    sc_out<bool>            sgatpe090;
    sc_out<bool>            sgatpe091;
    sc_out<bool>            sgatpe092;
    //}}}
    sc_out<bool>            sgatpsout;
    sc_out<bool>            sgaterroz;
    sc_out<bool>            sgaterroutz;
    sc_out<bool>            sgati;
    sc_out<bool>            sgatnmi;
    sc_out<bool>            sgatresz;
    sc_out<bool>            dtmtrgout;
    sc_out<bool>            errout_clear_mask_out;

    //Construct and Destruct
    SC_HAS_PROCESS(Cecm);
    Cecm(sc_module_name name);
    ~Cecm();
};

//===============Cecm_wp class=======================
class Cecm_wp: public sc_module 
              ,public vpcl::tlm_tgt_if<32, tlm::tlm_base_protocol_types, 3>
{
#include "ecm_wp_cmdif.h"
private:
    //Declare enum type
    enum eECM_WP {
        emSizeofCommon                      = 256   //The size of common register map
        ,emSizeofMaster                     = 256   //The size of master register map
        ,emSizeofChecker                    = 256   //The size of checker register map
        ,emNumOfState                       = 5     //The number of state
        ,emNanoSecond                       = 1000000000
    };
    enum eState {
         emIdleState                    = 0
        ,emErrorInputProcessingState    = 1
        ,emECMProcessingState           = 2
        ,emOutputSignalCombinationState = 3
        ,emResettingState               = 4
    };
    enum eECMId {
        emCommonId = 0
       ,emMasterId = 1
       ,emSlaveId  = 2
       ,emMaxId    = 3
    };

    //Structures
    struct ExternalErrorInputElement {
        sc_in<bool>* external_error;
        std::string error_name;
        bool pre_value;
        ExternalErrorInputElement(sc_in<bool>* external_error, std::string error_name, bool pre_value){
            if (external_error != NULL){
                this->external_error = external_error;
            }
            this->error_name = error_name;
            this->pre_value = pre_value;
        }
    };
    std::vector<ExternalErrorInputElement> mExternalErrorInputTable;

    std::string mStateArray[emNumOfState];

    //Declare event
    sc_event        mCmdResetEvent;
    sc_event        mCancelCmdResetEvent;
    sc_event        mAssertResetEvent;
    sc_event        mInitializeEvent;
    sc_event        mWriteOutputPortEvent;
    sc_event        mReceiveExternalErrorEvent;
    sc_event        mReceivePCLKEvent;
    sc_event        mReceiveSVACCESSEvent;
    sc_event        mComparatorTestEvent;

    //Declare internal variable
    eState          mState;             // State of ECM_WP  
    bool            mPortReset;         // Indicate ECM_WP is resetting by preset_n port
    bool            mCmdReset;          // Indicate ECM_WP is resetting by handleCommand
    bool            mIsCmdResetStatus;  // Indicate handleCommand reset status
    double          mResetPeriod;       // handleCommand reset period time
    bool            mECMTI;             // store the value of ecmti port
    bool            mECMTNMI;           // store the value of ecmtnmi port
    bool            mECMTERROZ;         // store the value of ecmterroz port
    bool            mECMTERROUTZ_M;     // store the value of ecmterroutz_m port
    bool            mECMTERROUTZ_C;     // store the value of ecmterroutz_c port
    bool            mECMTRESZ;          // store the value of ecmtresz port
    bool            mIsInitialize;
    double          mFreqPCLK;
    double          mFreqCNTCLK;
    bool            mSgoterrin;          // The internal error of ECM_WP
    //sgaterrinN_sig (N = 00 .. 92)
    //{{{
    sc_signal<bool> sgaterrin00_sig;
    sc_signal<bool> sgaterrin01_sig;
    sc_signal<bool> sgaterrin02_sig;
    sc_signal<bool> sgaterrin03_sig;
    sc_signal<bool> sgaterrin04_sig;
    sc_signal<bool> sgaterrin05_sig;
    sc_signal<bool> sgaterrin06_sig;
    sc_signal<bool> sgaterrin07_sig;
    sc_signal<bool> sgaterrin08_sig;
    sc_signal<bool> sgaterrin09_sig;
    sc_signal<bool> sgaterrin10_sig;
    sc_signal<bool> sgaterrin11_sig;
    sc_signal<bool> sgaterrin12_sig;
    sc_signal<bool> sgaterrin13_sig;
    sc_signal<bool> sgaterrin14_sig;
    sc_signal<bool> sgaterrin15_sig;
    sc_signal<bool> sgaterrin16_sig;
    sc_signal<bool> sgaterrin17_sig;
    sc_signal<bool> sgaterrin18_sig;
    sc_signal<bool> sgaterrin19_sig;
    sc_signal<bool> sgaterrin20_sig;
    sc_signal<bool> sgaterrin21_sig;
    sc_signal<bool> sgaterrin22_sig;
    sc_signal<bool> sgaterrin23_sig;
    sc_signal<bool> sgaterrin24_sig;
    sc_signal<bool> sgaterrin25_sig;
    sc_signal<bool> sgaterrin26_sig;
    sc_signal<bool> sgaterrin27_sig;
    sc_signal<bool> sgaterrin28_sig;
    sc_signal<bool> sgaterrin29_sig;
    sc_signal<bool> sgaterrin30_sig;
    sc_signal<bool> sgaterrin31_sig;
    sc_signal<bool> sgaterrin32_sig;
    sc_signal<bool> sgaterrin33_sig;
    sc_signal<bool> sgaterrin34_sig;
    sc_signal<bool> sgaterrin35_sig;
    sc_signal<bool> sgaterrin36_sig;
    sc_signal<bool> sgaterrin37_sig;
    sc_signal<bool> sgaterrin38_sig;
    sc_signal<bool> sgaterrin39_sig;
    sc_signal<bool> sgaterrin40_sig;
    sc_signal<bool> sgaterrin41_sig;
    sc_signal<bool> sgaterrin42_sig;
    sc_signal<bool> sgaterrin43_sig;
    sc_signal<bool> sgaterrin44_sig;
    sc_signal<bool> sgaterrin45_sig;
    sc_signal<bool> sgaterrin46_sig;
    sc_signal<bool> sgaterrin47_sig;
    sc_signal<bool> sgaterrin48_sig;
    sc_signal<bool> sgaterrin49_sig;
    sc_signal<bool> sgaterrin50_sig;
    sc_signal<bool> sgaterrin51_sig;
    sc_signal<bool> sgaterrin52_sig;
    sc_signal<bool> sgaterrin53_sig;
    sc_signal<bool> sgaterrin54_sig;
    sc_signal<bool> sgaterrin55_sig;
    sc_signal<bool> sgaterrin56_sig;
    sc_signal<bool> sgaterrin57_sig;
    sc_signal<bool> sgaterrin58_sig;
    sc_signal<bool> sgaterrin59_sig;
    sc_signal<bool> sgaterrin60_sig;
    sc_signal<bool> sgaterrin61_sig;
    sc_signal<bool> sgaterrin62_sig;
    sc_signal<bool> sgaterrin63_sig;
    sc_signal<bool> sgaterrin64_sig;
    sc_signal<bool> sgaterrin65_sig;
    sc_signal<bool> sgaterrin66_sig;
    sc_signal<bool> sgaterrin67_sig;
    sc_signal<bool> sgaterrin68_sig;
    sc_signal<bool> sgaterrin69_sig;
    sc_signal<bool> sgaterrin70_sig;
    sc_signal<bool> sgaterrin71_sig;
    sc_signal<bool> sgaterrin72_sig;
    sc_signal<bool> sgaterrin73_sig;
    sc_signal<bool> sgaterrin74_sig;
    sc_signal<bool> sgaterrin75_sig;
    sc_signal<bool> sgaterrin76_sig;
    sc_signal<bool> sgaterrin77_sig;
    sc_signal<bool> sgaterrin78_sig;
    sc_signal<bool> sgaterrin79_sig;
    sc_signal<bool> sgaterrin80_sig;
    sc_signal<bool> sgaterrin81_sig;
    sc_signal<bool> sgaterrin82_sig;
    sc_signal<bool> sgaterrin83_sig;
    sc_signal<bool> sgaterrin84_sig;
    sc_signal<bool> sgaterrin85_sig;
    sc_signal<bool> sgaterrin86_sig;
    sc_signal<bool> sgaterrin87_sig;
    sc_signal<bool> sgaterrin88_sig;
    sc_signal<bool> sgaterrin89_sig;
    sc_signal<bool> sgaterrin90_sig;
    sc_signal<bool> sgaterrin91_sig;
    sc_signal<bool> master_sgaterrin92_sig;
    sc_signal<bool> checker_sgaterrin92_sig;  
    //}}}
    sc_signal<bool> master_sgaterroz_sig;
    sc_signal<bool> master_sgaterroutz_sig;
    sc_signal<bool> master_sgati_sig;
    sc_signal<bool> master_sgatnmi_sig;
    sc_signal<bool> master_sgatresz_sig;
    sc_signal<bool> checker_sgaterroz_sig;
    sc_signal<bool> checker_sgaterroutz_sig;
    sc_signal<bool> checker_sgati_sig;
    sc_signal<bool> checker_sgatnmi_sig;
    sc_signal<bool> checker_sgatresz_sig;
    sc_signal<bool> errout_clear_mask_sig;
    sc_signal<bool> errout_clear_mask_checker;

    //master_sgatpeN_sig (N = 000 .. 092)
    //{{{
    sc_signal<bool> master_sgatpe000_sig;
    sc_signal<bool> master_sgatpe001_sig;
    sc_signal<bool> master_sgatpe002_sig;
    sc_signal<bool> master_sgatpe003_sig;
    sc_signal<bool> master_sgatpe004_sig;
    sc_signal<bool> master_sgatpe005_sig;
    sc_signal<bool> master_sgatpe006_sig;
    sc_signal<bool> master_sgatpe007_sig;
    sc_signal<bool> master_sgatpe008_sig;
    sc_signal<bool> master_sgatpe009_sig;
    sc_signal<bool> master_sgatpe010_sig;
    sc_signal<bool> master_sgatpe011_sig;
    sc_signal<bool> master_sgatpe012_sig;
    sc_signal<bool> master_sgatpe013_sig;
    sc_signal<bool> master_sgatpe014_sig;
    sc_signal<bool> master_sgatpe015_sig;
    sc_signal<bool> master_sgatpe016_sig;
    sc_signal<bool> master_sgatpe017_sig;
    sc_signal<bool> master_sgatpe018_sig;
    sc_signal<bool> master_sgatpe019_sig;
    sc_signal<bool> master_sgatpe020_sig;
    sc_signal<bool> master_sgatpe021_sig;
    sc_signal<bool> master_sgatpe022_sig;
    sc_signal<bool> master_sgatpe023_sig;
    sc_signal<bool> master_sgatpe024_sig;
    sc_signal<bool> master_sgatpe025_sig;
    sc_signal<bool> master_sgatpe026_sig;
    sc_signal<bool> master_sgatpe027_sig;
    sc_signal<bool> master_sgatpe028_sig;
    sc_signal<bool> master_sgatpe029_sig;
    sc_signal<bool> master_sgatpe030_sig;
    sc_signal<bool> master_sgatpe031_sig;
    sc_signal<bool> master_sgatpe032_sig;
    sc_signal<bool> master_sgatpe033_sig;
    sc_signal<bool> master_sgatpe034_sig;
    sc_signal<bool> master_sgatpe035_sig;
    sc_signal<bool> master_sgatpe036_sig;
    sc_signal<bool> master_sgatpe037_sig;
    sc_signal<bool> master_sgatpe038_sig;
    sc_signal<bool> master_sgatpe039_sig;
    sc_signal<bool> master_sgatpe040_sig;
    sc_signal<bool> master_sgatpe041_sig;
    sc_signal<bool> master_sgatpe042_sig;
    sc_signal<bool> master_sgatpe043_sig;
    sc_signal<bool> master_sgatpe044_sig;
    sc_signal<bool> master_sgatpe045_sig;
    sc_signal<bool> master_sgatpe046_sig;
    sc_signal<bool> master_sgatpe047_sig;
    sc_signal<bool> master_sgatpe048_sig;
    sc_signal<bool> master_sgatpe049_sig;
    sc_signal<bool> master_sgatpe050_sig;
    sc_signal<bool> master_sgatpe051_sig;
    sc_signal<bool> master_sgatpe052_sig;
    sc_signal<bool> master_sgatpe053_sig;
    sc_signal<bool> master_sgatpe054_sig;
    sc_signal<bool> master_sgatpe055_sig;
    sc_signal<bool> master_sgatpe056_sig;
    sc_signal<bool> master_sgatpe057_sig;
    sc_signal<bool> master_sgatpe058_sig;
    sc_signal<bool> master_sgatpe059_sig;
    sc_signal<bool> master_sgatpe060_sig;
    sc_signal<bool> master_sgatpe061_sig;
    sc_signal<bool> master_sgatpe062_sig;
    sc_signal<bool> master_sgatpe063_sig;
    sc_signal<bool> master_sgatpe064_sig;
    sc_signal<bool> master_sgatpe065_sig;
    sc_signal<bool> master_sgatpe066_sig;
    sc_signal<bool> master_sgatpe067_sig;
    sc_signal<bool> master_sgatpe068_sig;
    sc_signal<bool> master_sgatpe069_sig;
    sc_signal<bool> master_sgatpe070_sig;
    sc_signal<bool> master_sgatpe071_sig;
    sc_signal<bool> master_sgatpe072_sig;
    sc_signal<bool> master_sgatpe073_sig;
    sc_signal<bool> master_sgatpe074_sig;
    sc_signal<bool> master_sgatpe075_sig;
    sc_signal<bool> master_sgatpe076_sig;
    sc_signal<bool> master_sgatpe077_sig;
    sc_signal<bool> master_sgatpe078_sig;
    sc_signal<bool> master_sgatpe079_sig;
    sc_signal<bool> master_sgatpe080_sig;
    sc_signal<bool> master_sgatpe081_sig;
    sc_signal<bool> master_sgatpe082_sig;
    sc_signal<bool> master_sgatpe083_sig;
    sc_signal<bool> master_sgatpe084_sig;
    sc_signal<bool> master_sgatpe085_sig;
    sc_signal<bool> master_sgatpe086_sig;
    sc_signal<bool> master_sgatpe087_sig;
    sc_signal<bool> master_sgatpe088_sig;
    sc_signal<bool> master_sgatpe089_sig;
    sc_signal<bool> master_sgatpe090_sig;
    sc_signal<bool> master_sgatpe091_sig;
    sc_signal<bool> master_sgatpe092_sig;
    //}}}
    //checker_sgatpeN_sig (N = 000 .. 092)
    //{{{
    sc_signal<bool> checker_sgatpe000_sig;
    sc_signal<bool> checker_sgatpe001_sig;
    sc_signal<bool> checker_sgatpe002_sig;
    sc_signal<bool> checker_sgatpe003_sig;
    sc_signal<bool> checker_sgatpe004_sig;
    sc_signal<bool> checker_sgatpe005_sig;
    sc_signal<bool> checker_sgatpe006_sig;
    sc_signal<bool> checker_sgatpe007_sig;
    sc_signal<bool> checker_sgatpe008_sig;
    sc_signal<bool> checker_sgatpe009_sig;
    sc_signal<bool> checker_sgatpe010_sig;
    sc_signal<bool> checker_sgatpe011_sig;
    sc_signal<bool> checker_sgatpe012_sig;
    sc_signal<bool> checker_sgatpe013_sig;
    sc_signal<bool> checker_sgatpe014_sig;
    sc_signal<bool> checker_sgatpe015_sig;
    sc_signal<bool> checker_sgatpe016_sig;
    sc_signal<bool> checker_sgatpe017_sig;
    sc_signal<bool> checker_sgatpe018_sig;
    sc_signal<bool> checker_sgatpe019_sig;
    sc_signal<bool> checker_sgatpe020_sig;
    sc_signal<bool> checker_sgatpe021_sig;
    sc_signal<bool> checker_sgatpe022_sig;
    sc_signal<bool> checker_sgatpe023_sig;
    sc_signal<bool> checker_sgatpe024_sig;
    sc_signal<bool> checker_sgatpe025_sig;
    sc_signal<bool> checker_sgatpe026_sig;
    sc_signal<bool> checker_sgatpe027_sig;
    sc_signal<bool> checker_sgatpe028_sig;
    sc_signal<bool> checker_sgatpe029_sig;
    sc_signal<bool> checker_sgatpe030_sig;
    sc_signal<bool> checker_sgatpe031_sig;
    sc_signal<bool> checker_sgatpe032_sig;
    sc_signal<bool> checker_sgatpe033_sig;
    sc_signal<bool> checker_sgatpe034_sig;
    sc_signal<bool> checker_sgatpe035_sig;
    sc_signal<bool> checker_sgatpe036_sig;
    sc_signal<bool> checker_sgatpe037_sig;
    sc_signal<bool> checker_sgatpe038_sig;
    sc_signal<bool> checker_sgatpe039_sig;
    sc_signal<bool> checker_sgatpe040_sig;
    sc_signal<bool> checker_sgatpe041_sig;
    sc_signal<bool> checker_sgatpe042_sig;
    sc_signal<bool> checker_sgatpe043_sig;
    sc_signal<bool> checker_sgatpe044_sig;
    sc_signal<bool> checker_sgatpe045_sig;
    sc_signal<bool> checker_sgatpe046_sig;
    sc_signal<bool> checker_sgatpe047_sig;
    sc_signal<bool> checker_sgatpe048_sig;
    sc_signal<bool> checker_sgatpe049_sig;
    sc_signal<bool> checker_sgatpe050_sig;
    sc_signal<bool> checker_sgatpe051_sig;
    sc_signal<bool> checker_sgatpe052_sig;
    sc_signal<bool> checker_sgatpe053_sig;
    sc_signal<bool> checker_sgatpe054_sig;
    sc_signal<bool> checker_sgatpe055_sig;
    sc_signal<bool> checker_sgatpe056_sig;
    sc_signal<bool> checker_sgatpe057_sig;
    sc_signal<bool> checker_sgatpe058_sig;
    sc_signal<bool> checker_sgatpe059_sig;
    sc_signal<bool> checker_sgatpe060_sig;
    sc_signal<bool> checker_sgatpe061_sig;
    sc_signal<bool> checker_sgatpe062_sig;
    sc_signal<bool> checker_sgatpe063_sig;
    sc_signal<bool> checker_sgatpe064_sig;
    sc_signal<bool> checker_sgatpe065_sig;
    sc_signal<bool> checker_sgatpe066_sig;
    sc_signal<bool> checker_sgatpe067_sig;
    sc_signal<bool> checker_sgatpe068_sig;
    sc_signal<bool> checker_sgatpe069_sig;
    sc_signal<bool> checker_sgatpe070_sig;
    sc_signal<bool> checker_sgatpe071_sig;
    sc_signal<bool> checker_sgatpe072_sig;
    sc_signal<bool> checker_sgatpe073_sig;
    sc_signal<bool> checker_sgatpe074_sig;
    sc_signal<bool> checker_sgatpe075_sig;
    sc_signal<bool> checker_sgatpe076_sig;
    sc_signal<bool> checker_sgatpe077_sig;
    sc_signal<bool> checker_sgatpe078_sig;
    sc_signal<bool> checker_sgatpe079_sig;
    sc_signal<bool> checker_sgatpe080_sig;
    sc_signal<bool> checker_sgatpe081_sig;
    sc_signal<bool> checker_sgatpe082_sig;
    sc_signal<bool> checker_sgatpe083_sig;
    sc_signal<bool> checker_sgatpe084_sig;
    sc_signal<bool> checker_sgatpe085_sig;
    sc_signal<bool> checker_sgatpe086_sig;
    sc_signal<bool> checker_sgatpe087_sig;
    sc_signal<bool> checker_sgatpe088_sig;
    sc_signal<bool> checker_sgatpe089_sig;
    sc_signal<bool> checker_sgatpe090_sig;
    sc_signal<bool> checker_sgatpe091_sig;
    sc_signal<bool> checker_sgatpe092_sig;
    //}}}
    sc_signal<bool> checker_psout_sig;
    sc_signal<bool> master_psout_sig;
    sc_signal<bool> checker_trgout_sig;
    sc_signal<bool> master_trgout_sig;
    sc_signal<bool> dtmtrgout_sig;

    //ECM_WP Method
    void ReceiveExternalErrorMethod(void);
    void ResetMethod(void);
    void InitializeMethod(void);
    void WriteOutputPortMethod(void);
    void CmdResetMethod(void);
    void CancelCmdResetMethod(void);
    void PCLKPeriodMethod(void);
    void CounterPeriodMethod(void);
    void CombineSignalMethod(void);
    void svaccessMethod(void);
    void ComparatorTestMethod(void);
    void ECMterrin92MaskMethod(void);

    //ECM_WP internal functions
    void SetLatency_TLM(const unsigned int id, const double PCLK_period, const bool is_constructor);
    double GetTimeResolution(void);
    void ExternalErrorInputTableConstruction(void);
    void AssertReset(double delay, double period);
    void SetCLKfreq(std::string clk_name, double clk_freq);
    void DumpStatInfo(void);
    void DumpEnableTransInfo(void);
    void UpdateState(eState prev_state, eState next_state);
    void EnableReset(const bool is_active);
    void ConnectPort(void);
    void InitializeInternalSignal(void);
    void DumpInfo(const char *type, const char *message,...);

    //tlm_tgt_if api functions
    void tgt_acc(unsigned int id, tlm::tlm_generic_payload &trans, sc_time &t);
    unsigned int tgt_acc_dbg(unsigned int id, tlm::tlm_generic_payload &trans);

public:
    //Declare input ports
    sc_in<bool>             preset_n;
    sc_in<bool>             cntclk_preset_n;
    sc_in<sc_dt::uint64 >   pclk;
    sc_in<bool>             erroutresz ;
    sc_in<bool>             cntclk_erroutresz;
    sc_in<sc_dt::uint64 >   cntclk; 
    //ecmterrinN (N = 00 .. 91)
    //{{{
    sc_in<bool>             ecmterrin0;
    sc_in<bool>             ecmterrin1;
    sc_in<bool>             ecmterrin2;
    sc_in<bool>             ecmterrin3;
    sc_in<bool>             ecmterrin4;
    sc_in<bool>             ecmterrin5;
    sc_in<bool>             ecmterrin6;
    sc_in<bool>             ecmterrin7;
    sc_in<bool>             ecmterrin8;
    sc_in<bool>             ecmterrin9;
    sc_in<bool>             ecmterrin10;
    sc_in<bool>             ecmterrin11;
    sc_in<bool>             ecmterrin12;
    sc_in<bool>             ecmterrin13;
    sc_in<bool>             ecmterrin14;
    sc_in<bool>             ecmterrin15;
    sc_in<bool>             ecmterrin16;
    sc_in<bool>             ecmterrin17;
    sc_in<bool>             ecmterrin18;
    sc_in<bool>             ecmterrin19;
    sc_in<bool>             ecmterrin20;
    sc_in<bool>             ecmterrin21;
    sc_in<bool>             ecmterrin22;
    sc_in<bool>             ecmterrin23;
    sc_in<bool>             ecmterrin24;
    sc_in<bool>             ecmterrin25;
    sc_in<bool>             ecmterrin26;
    sc_in<bool>             ecmterrin27;
    sc_in<bool>             ecmterrin28;
    sc_in<bool>             ecmterrin29;
    sc_in<bool>             ecmterrin30;
    sc_in<bool>             ecmterrin31;
    sc_in<bool>             ecmterrin32;
    sc_in<bool>             ecmterrin33;
    sc_in<bool>             ecmterrin34;
    sc_in<bool>             ecmterrin35;
    sc_in<bool>             ecmterrin36;
    sc_in<bool>             ecmterrin37;
    sc_in<bool>             ecmterrin38;
    sc_in<bool>             ecmterrin39;
    sc_in<bool>             ecmterrin40;
    sc_in<bool>             ecmterrin41;
    sc_in<bool>             ecmterrin42;
    sc_in<bool>             ecmterrin43;
    sc_in<bool>             ecmterrin44;
    sc_in<bool>             ecmterrin45;
    sc_in<bool>             ecmterrin46;
    sc_in<bool>             ecmterrin47;
    sc_in<bool>             ecmterrin48;
    sc_in<bool>             ecmterrin49;
    sc_in<bool>             ecmterrin50;
    sc_in<bool>             ecmterrin51;
    sc_in<bool>             ecmterrin52;
    sc_in<bool>             ecmterrin53;
    sc_in<bool>             ecmterrin54;
    sc_in<bool>             ecmterrin55;
    sc_in<bool>             ecmterrin56;
    sc_in<bool>             ecmterrin57;
    sc_in<bool>             ecmterrin58;
    sc_in<bool>             ecmterrin59;
    sc_in<bool>             ecmterrin60;
    sc_in<bool>             ecmterrin61;
    sc_in<bool>             ecmterrin62;
    sc_in<bool>             ecmterrin63;
    sc_in<bool>             ecmterrin64;
    sc_in<bool>             ecmterrin65;
    sc_in<bool>             ecmterrin66;
    sc_in<bool>             ecmterrin67;
    sc_in<bool>             ecmterrin68;
    sc_in<bool>             ecmterrin69;
    sc_in<bool>             ecmterrin70;
    sc_in<bool>             ecmterrin71;
    sc_in<bool>             ecmterrin72;
    sc_in<bool>             ecmterrin73;
    sc_in<bool>             ecmterrin74;
    sc_in<bool>             ecmterrin75;
    sc_in<bool>             ecmterrin76;
    sc_in<bool>             ecmterrin77;
    sc_in<bool>             ecmterrin78;
    sc_in<bool>             ecmterrin79;
    sc_in<bool>             ecmterrin80;
    sc_in<bool>             ecmterrin81;
    sc_in<bool>             ecmterrin82;
    sc_in<bool>             ecmterrin83;
    sc_in<bool>             ecmterrin84;
    sc_in<bool>             ecmterrin85;
    sc_in<bool>             ecmterrin86;
    sc_in<bool>             ecmterrin87;
    sc_in<bool>             ecmterrin88;
    sc_in<bool>             ecmterrin89;
    sc_in<bool>             ecmterrin90;
    sc_in<bool>             ecmterrin91;
    //}}}
    sc_in<bool>             ecmterrlbz;
    sc_in<bool>             ecmttin;
    sc_in<bool>             resstg1z;
    sc_in<bool>             pclkin_resstg1z;
    sc_in<bool>             svaccess;
    sc_in<bool>             ecmterrin92msk_m;
    sc_in<bool>             ecmterrin92msk_c;

    //Declare output ports
    sc_out<bool>            ecmterroz;
    sc_out<bool>            ecmterroutz;
    sc_out<bool>            ecmti;
    sc_out<bool>            ecmtnmi;
    sc_out<bool>            ecmtresz;

    //Declare Master, Checker
    Cecm            *mMaster;
    Cecm            *mChecker;

    //Public functions
    std::string GetMasterCheckerName(const bool is_master) const;

    //Construct and Destruct
    SC_HAS_PROCESS(Cecm_wp);
    Cecm_wp(sc_module_name name);
    ~Cecm_wp();
};
#endif //__ECM_H__

// vim600: set foldmethod=marker :
