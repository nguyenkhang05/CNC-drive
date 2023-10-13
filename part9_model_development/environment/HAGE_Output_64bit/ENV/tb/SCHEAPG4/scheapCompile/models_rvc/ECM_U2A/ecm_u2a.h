// ----------------------------------------------------------------------
// $Id: $
//
// Copyright(c) 2017-2020 Renesas System Design Co., Ltd.
// Copyright(c) 2017-2020 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ----------------------------------------------------------------------

#ifndef __ECM_U2_H__
#define __ECM_U2_H__
#include "systemc.h"
#include <sstream>
#include "tlm.h"
#include "tlm_tgt_if.h"
#include "Cgeneral_timer.h"
#include "ecm_u2a_regif.h"
#include "re_define.h"
#include <iterator>
#include <string>
#include <map>

class Cecm_u2a_wp;
//===============Cecm_u2a class=======================
class Cecm_u2a: public Cecm_u2a_regif
           ,public Cgeneral_timer
{
#include "ecm_u2a_cmdif.h"
friend class Cecm_u2a_wp;

private:
    enum eECM {
         emNumErrInPort          = 331                   // The number of errin port array
        ,emPseudoErrorStartID    = 11 
        ,emCompareErrorId   = 20
        ,emNumErrorFactor   = 352       //The number of factor
        ,emNumStatusReg     = 11
        ,emNumPE            = 4
        ,emAllUnit          = 0xFFFFFFFF
        ,emPEStartErrorId  = 224
        ,emPETotalError    = 32
    };
    //Declare enum type
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
        ,emECMInternalResetInactiveLevel    = 1
    };

    Cecm_u2a_wp* mWrapper;

    //Structure
    struct ECMErrorInputElement {
        sc_in<bool>*    errorin;            //The sgaterrin signal
        sc_out<bool>*   pseudo_error_out;   //The sgatpe signal
        bool            errorin_pre;        //The previous value of errorin
        vpcl::bit_info* esstr;              //The corresponding ESSTR bit
        vpcl::bit_info* misfg;              //The corresponding MISFG bit
        vpcl::bit_info* incfg[emNumPE];    //The corresponding NMICFG bit to PE
        vpcl::bit_info* ircfg;              //The corresponding IRCFG bit
        vpcl::bit_info* emk;                //The corresponding EMK bit to appl0
        vpcl::bit_info* etmk[emNumPE];      //The corresponding EMK bit to appln
        vpcl::bit_info* esstc;              //The corresponding clear status bit
        vpcl::bit_info* pe;                 //The corresponding PE bit
        vpcl::bit_info* dtmcfg;             //The corresponding dedicated delay timer config
        vpcl::bit_info* dtmcfgn[emNumPE];   //The corresponding delay timer config for each PE
        ECMErrorInputElement(sc_in<bool>* errorin, sc_out<bool>* pseudo_error_out, bool errorin_pre,
        vpcl::bit_info* esstr, vpcl::bit_info* misfg, vpcl::bit_info* nmicfg0, vpcl::bit_info* nmicfg1, vpcl::bit_info* nmicfg2, vpcl::bit_info* nmicfg3, 
        vpcl::bit_info* ircfg, vpcl::bit_info* emk, vpcl::bit_info* etmk0, vpcl::bit_info* etmk1, vpcl::bit_info* etmk2, vpcl::bit_info* etmk3, vpcl::bit_info* esstc, vpcl::bit_info* pe,
        vpcl::bit_info* dtmcfg, vpcl::bit_info* dtmcfg0, vpcl::bit_info* dtmcfg1, vpcl::bit_info* dtmcfg2, vpcl::bit_info* dtmcfg3){
            if ((errorin != NULL) && (pseudo_error_out != NULL)   
            && (esstr != NULL) && (misfg != NULL) && (nmicfg0 != NULL) && (nmicfg1 != NULL) && (nmicfg2 != NULL) && (nmicfg3 != NULL) 
            && (ircfg != NULL) && (emk != NULL) && (etmk0 != NULL) && (etmk1 != NULL) && (etmk2 != NULL) && (etmk3 != NULL) &&  (esstc != NULL) && (pe != NULL) 
            && (dtmcfg != NULL) && (dtmcfg0 != NULL) && (dtmcfg1 != NULL) && (dtmcfg2 != NULL) && (dtmcfg3 != NULL) ){
                this->errorin = errorin;
                this->pseudo_error_out = pseudo_error_out;
                this->errorin_pre = errorin_pre;
                this->esstr = esstr;
                this->misfg = misfg;
                this->incfg[0] = nmicfg0;
                this->incfg[1] = nmicfg1;
                this->incfg[2] = nmicfg2;
                this->incfg[3] = nmicfg3;
                this->ircfg = ircfg;
                this->emk = emk;
                this->etmk[0] = etmk0;
                this->etmk[1] = etmk1;
                this->etmk[2] = etmk2;
                this->etmk[3] = etmk3;
                this->esstc = esstc;
                this->pe = pe;
                this->dtmcfg = dtmcfg;
                this->dtmcfgn[0] = dtmcfg0;
                this->dtmcfgn[1] = dtmcfg1;
                this->dtmcfgn[2] = dtmcfg2;
                this->dtmcfgn[3] = dtmcfg3;
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

    enum eListSpecialError {
         emStartIDNoneInterrupt     = 0     // Error factor 33, 34
        ,emStartIDNoneDelayTimer    = 2     // Error factor 33, 34, 48->53, 229, 261, 293, 325
        ,emStartIDNoneErrorOutput   = 14    // Error factor 255, 287, 319, 351 
        ,emStartIDNoneErrorTrigger  = 18    // Error factor 11, 33, 34, 255, 287, 319, 351
        ,emStartIDNoneInternalReset = 25    // Error factor 33, 34
        ,emStartIDCheckAfterReset   = 27    // Error factor 33, 34 (can be actived when reseting - except Application reset)
        ,emTotalSpecialError        = 29
    };
    unsigned int mListSpecialError[emTotalSpecialError];

    //Declare event
    sc_event mWriteSGATERROZPortEvent;
    sc_event mWriteSGATERROUTZPortEvent;
    sc_event mWriteSGATRESZPortEvent;
    sc_event mCombineSGATIPEPortEvent;
    sc_event mWriteSGATIPEPortEvent;
    sc_event mWriteSGATNMIPortEvent;
    sc_event mWriteDTMTRGOUTPortEvent[2];
    sc_event mNegateInterruptEvent;
    sc_event mWriteErrOutMskClrPortEvent[emNumPE+1];
    sc_event mCombineSignalEvent;
    sc_event mECMInitializeEvent;
    sc_event mEnableClearOutputErrEvent[emNumPE+1];
    sc_event mWritePseudoErrorOutEvent;
    sc_event mUpdateDTMSTACNTCLKBitEvent;
    sc_event mWriteDCLSErrorPortEvent;
    sc_event mUnlockECMEOCCFGEvent;
    sc_event mProcessECMmECTBitEvent[emNumPE+1];
    sc_event mDelayTimerStopEvent;
    sc_event mWriteCompareMaskEvent;

    //Declare internal variable
    bool            mSGATI[emNumPE];        //The value of sgati
    bool            mSGATIPEArray[emNumPE]; //The value of sgati_pe1..8
    bool            mSGATNMI[emNumPE];      //The value of sgatnmi
    bool            mSGATRESZ;              //The value of sgatresz port
    bool            mSGATERROZ;             //The value of sgaterroz port
    bool            mSGATERROZn[emNumPE];   //The value of sgaterroz port
    bool            mSGATERROUTZ;           //The value of sgaterroutz port
    bool            mSyncSignal;            //The value of 'sync' signal
    bool            mSyncSignalPe[emNumPE]; //The value of 'sync' signal
    bool            mIsECMReset;            //Indicate ECM (Master/Checker) is in reset period
    bool            mIsECMerroutreszReset;  //Indicate ECM (Master/Checker) is in erroutresz reset period
    bool            mIsECMresstg1zReset;    //Indicate ECM (Master/Checker) is in resstg1z reset period
    bool            mIsECMpclkin_resstg1zReset;  //Indicate ECM (Master/Checker) is in pclkin_resstg1z reset period
    bool            mHoldSignal[emNumPE+1];            //Variable shows status of enabling clear error output signal
    double          mCountClock;            //Store value of cntclk
    double          mPCLKClock;             //Store value of pclk
    double          mRestartCounterTime[emNumPE+1];     //Time when one error occurs 
    unsigned int    mECMEOCCFG;
    unsigned int    mECMEOCCFGn[emNumPE];
    unsigned int    mECMmESSTR[emNumStatusReg];         //Store values of status register (ECMmESSTRn)
    bool            mPseudoErrorArray[emNumErrorFactor];   //Store value to write pseudo-error signal
    bool            mIsLockReg;             //Indicate protection registers are lock or not
    bool            mERROUTPEArray[emNumPE];//Store value to write to errout_pe1..8
    bool            mStartSimulation;
    bool            mIsDTMActive;

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
    void WriteSGATNMIPortMethod(void);
    void WriteDTMTRGOUTPortMethod(bool dtmtrgout_val);
    void WriteErrOutMskClrPortMethod(unsigned i);
    void WriteCompareMaskMethod(void);
    void resstg1zMethod(void);
    void resstg1z_pclkinMethod(void);
    void sgattinMethod(void);
    void sgatpsinMethod(void);
    void DelayTimerConfigureMethod(void);
    void DelayTimerStopMethod(void);
    void EnableClearOutputErrMethod(unsigned i);
    void ResetForErrpinMethod(void); 
    void WritePseudoErrorOutMethod(void);
    void CombineSGATIPEPortMethod(void);
    void WriteSGATIPEPortMethod(void);
    void WriteDCLSErrorPortMethod(void);
    void NegateInterruptMethod(void);
    void UpdateDTMSTACNTCLKBitMethod(void);
    void ProcessECMmECTBitMethod(unsigned i);
    void ClearGlobalErrorMethod(void);


    //ECM internal functions
    void start_of_simulation(void);
    bool CheckProtectedRegister(unsigned int addr);
    std::string GetProtectedRegisterName(unsigned int addr);
    bool WriteProtectedRegister(unsigned int addr, unsigned int data, unsigned int size);
    bool CheckBreakProtectionSequence(unsigned int addr, unsigned int data, unsigned int size);
    bool CheckNormalProtectionSequence(unsigned int addr, unsigned int data, unsigned int size);
    bool ProtectionErrorProcess(const bool is_break, unsigned int data);
    void IssueErrorOutput(unsigned int src = emAllUnit);
    void CalculateErrorOutput(unsigned int src = emAllUnit);
    void IssueInterrupt(void);
    void IssueInternalReset(void);
    void StartStopCounter(const bool start);
    void InitializeGeneralTimer(void);
    void InitializeRegistersMethod(void);
    void ErrorInputTableConstruction(void);
    void ProtectedRegisterTableConstruction(void);
    void UpdateErrorStatus(void);
    void ECMEnableReset(const bool is_active);
    void ECMSetZeroClock(const std::string clk_name);
    bool ECMCheckZeroClock(const std::string clk_name);
    bool ECM_common_tgt_rd(const bool is_rd_dbg, unsigned int addr, unsigned char *p_data, unsigned int size);
    bool ECM_common_tgt_wr(const bool is_wr_dbg, unsigned int addr, unsigned char *p_data, unsigned int size);
    unsigned int GetRegisterIndex(unsigned int factor_index);
    std::string GetBitName( const char name[], unsigned int bit_index, unsigned int reg_index);
    double GetCurTime(void);
    bool isDCLSError (unsigned int error_idx);
    unsigned int mapDCLS_PEOUT (unsigned int error_idx);
    bool CheckSpecialError(unsigned int error_index, eListSpecialError startID, eListSpecialError endID);
    //Register IF callback functions
    void cb_ECMmESET_ECMmEST(RegCBstr str);
    void cb_ECMmECLR_ECMmECT(RegCBstr str);
    void cb_ECMmESET0_ECMmEST(RegCBstr str);
    void cb_ECMmECLR0_ECMmECT(RegCBstr str);
    void cb_ECMmESET1_ECMmEST(RegCBstr str);
    void cb_ECMmECLR1_ECMmECT(RegCBstr str);
    void cb_ECMmESET2_ECMmEST(RegCBstr str);
    void cb_ECMmECLR2_ECMmECT(RegCBstr str);
    void cb_ECMmESET3_ECMmEST(RegCBstr str);
    void cb_ECMmECLR3_ECMmECT(RegCBstr str);
    void cb_ECMISCFG_ECMIS00(RegCBstr str);
    void cb_ECMINCFG0_ECMIE00(RegCBstr str);
    void cb_ECMINCFG1_ECMIE00(RegCBstr str);
    void cb_ECMINCFG2_ECMIE00(RegCBstr str);
    void cb_ECMINCFG3_ECMIE00(RegCBstr str);
    void cb_ECMIRCFG_ECMIRE00(RegCBstr str);
    void cb_ECMEMK_ECMEMK00(RegCBstr str);
    void cb_ECMETMK0_ECMETMK00(RegCBstr str);
    void cb_ECMETMK1_ECMETMK00(RegCBstr str);
    void cb_ECMETMK2_ECMETMK00(RegCBstr str);
    void cb_ECMETMK3_ECMETMK00(RegCBstr str);
    void cb_ECMESSTC_ECMCLSSE00(RegCBstr str);
    void cb_ECMKCPROT_KCE(RegCBstr str);
    void cb_ECMPE_ECMPE00(RegCBstr str);
    void cb_ECMDTMCTL_DTMSTP(RegCBstr str);
    void cb_ECMDTMR_ECMDTMR(RegCBstr str);
    void cb_ECMDTMCMP_ECMDTMCMP(RegCBstr str);
    void cb_ECMDTMCFG_ECMDTME00(RegCBstr str);
    void cb_ECMDTMCFG0_ECMDTME00(RegCBstr str);
    void cb_ECMDTMCFG1_ECMDTME00(RegCBstr str);
    void cb_ECMDTMCFG2_ECMDTME00(RegCBstr str);
    void cb_ECMDTMCFG3_ECMDTME00(RegCBstr str);
    void cb_ECMEOCCFG_ECMEOUTCLRT(RegCBstr str);
    void cb_ECMETCCFG_ECMEOUTCLRT(RegCBstr str);
    void cb_ECMPEM_MSKC(RegCBstr str);
    void cbDBG_ECMDTMR_ECMDTMR(RegCBstr str);

public:
    //Declare input ports
    sc_in<bool>             erroutresz;
    //sgaterrinN (N = 00 .. 343)
    sc_in<bool>             *sgaterrin[emNumErrorFactor];
    sc_in<bool>             sgaterrlbz;
    sc_in<bool>             sgattin;
    sc_in<bool>             dtmtrgin;
    sc_in<bool>             resstg1z;
    sc_in<bool>             resstg1z_pclkin;
    sc_in<bool>             *errout_clear_mask_in[emNumPE+1]; 
    sc_in<bool>             errin_mask_reset; 

    //Declare output ports
    //sgatpeN (N = 000 .. 343)
    sc_out<bool>    *sgatpe[emNumErrorFactor];
    sc_out<bool>    sgaterroz;
    sc_out<bool>    *sgaterroz_n[emNumPE];
    sc_out<bool>    sgaterroutz;
    sc_out<bool>    *sgati_pe[emNumPE];
    sc_out<bool>    *sgatnmi[emNumPE];
    sc_out<bool>    sgatresz;
    sc_out<bool>    dtmtrgout;
    sc_out<bool>    *errout_clear_mask_out[emNumPE+1];
    sc_out<bool>    *errout_pe[emNumPE];
    sc_out<bool>    pseudo_compare_msk_m; 
    sc_out<bool>    pseudo_compare_msk_c; 


    //Construct and Destruct
    SC_HAS_PROCESS(Cecm_u2a);
    Cecm_u2a(sc_module_name name, Cecm_u2a_wp* wrapper);
    ~Cecm_u2a();
};

//===============Cecm_u2a_wp class=======================
class Cecm_u2a_wp: public sc_module 
              ,public vpcl::tlm_tgt_if<32, tlm::tlm_base_protocol_types, 3>
{
#include "ecm_u2a_wp_cmdif.h"
private:
    //Declare enum type
    enum eECM {
         emNumErrInPort         = 331                   // The number of errin port array
        ,emNumErrorFactor       = 352       //The number of factor
        ,emNumPE                = 4
        ,emPseudoErrorStartID   = 11 
        ,emCompareErrorId       = 20
        ,emSoftwareAlarm        = 192
    };
    enum eECM_WP {
        emOffsetOfCommon    = 256   //The offset of common from based of each M/C
        ,emNumOfState       = 5     //The number of state
        ,emNanoSecond       = 1000000000
    };
    enum eState {
         emIdleState                    = 0
        ,emErrorInputProcessingState    = 1
        ,emECMProcessingState           = 2
        ,emOutputSignalCombinationState = 3
        ,emResettingState               = 4
    };
    enum eECMId {
        emMasterId = 0
       ,emSlaveId  = 1
       ,emCommonId = 2
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
    sc_event    mCmdResetEvent;
    sc_event    mCancelCmdResetEvent;
    sc_event    mAssertResetEvent;
    sc_event    mWriteOutputPortEvent;
    sc_event    mReceiveExternalErrorEvent;
    sc_event    mComparatorTestEvent;

    //Declare internal variable
    eState      mState;             // State of ECM_WP  
    bool        mPortReset;         // Indicate ECM_WP is resetting by preset_n port
    bool        mCmdReset;          // Indicate ECM_WP is resetting by handleCommand
    bool        mIsCmdResetStatus;  // Indicate handleCommand reset status
    double      mResetPeriod;       // handleCommand reset period time
    bool        mECMTI_PE[emNumPE];     // store the value of ecmti_pe1..8 ports
    bool        mECMDCLSINT[emNumPE];   // store the value of ecmdclsint1..8 ports
    bool        mECMTNMI[emNumPE];           // store the value of ecmtnmi port
    bool        mECMTERROZ;         // store the value of ecmterroz port
    bool        mECMTERROZn[emNumPE];         // store the value of ecmterroz0 port
    bool        mECMTRESZ;          // store the value of ecmtresz port
    bool        mIsInitialize;
    double      mFreqPCLK;
    double      mFreqCNTCLK;
    bool        mSgoterrin;          // The internal error of ECM_WP
    //sgaterrinN_sig (N = 20 .. 351)
    sc_signal<bool> *sgaterrin_sig[emNumErrorFactor]; // 000-351
    sc_signal<bool> master_sgaterrin020_sig;// counted 8 PE to 020
    sc_signal<bool> checker_sgaterrin020_sig;  

    sc_signal<bool> master_sgaterroz_sig;
    sc_signal<bool> *master_sgaterroz_sig_n[emNumPE];
    sc_signal<bool> master_sgatresz_sig;
    sc_signal<bool> checker_sgaterroz_sig;
    sc_signal<bool> *checker_sgaterroz_sig_n[emNumPE];
    sc_signal<bool> checker_sgatresz_sig;
    sc_signal<bool> *errout_clear_mask_sig[emNumPE+1];
    sc_signal<bool> *errout_clear_mask_checker[emNumPE+1];
    sc_signal<bool> master_ecmterrin020msk_m;
    sc_signal<bool> master_ecmterrin020msk_c;
    sc_signal<bool> checker_ecmterrin020msk_m;
    sc_signal<bool> checker_ecmterrin020msk_c;

    //master_sgatpeN_sig (N = 000 .. 351)
    sc_signal<bool> *master_sgatpe_sig[emNumErrorFactor];
    sc_signal<bool> *master_sgatnmi_sig[emNumPE];

    //checker_sgatpeN_sig (N = 000 .. 351)
    sc_signal<bool> *checker_sgatpe_sig[emNumErrorFactor];
    sc_signal<bool> *checker_sgatnmi_sig[emNumPE];

    sc_signal<bool, SC_UNCHECKED_WRITERS> checker_trgout_sig;
    sc_signal<bool, SC_UNCHECKED_WRITERS> master_trgout_sig;
    sc_signal<bool> dtmtrgout_sig;

    sc_signal<bool> *master_dclsint_sig[emNumPE];
    sc_signal<bool> *checker_dclsint_sig[emNumPE];

    sc_signal<bool> *master_sgati_pe_sig[emNumPE];
    sc_signal<bool> *checker_sgati_pe_sig[emNumPE];

    //ECM_WP Method
    void ReceiveExternalErrorMethod(void);
    void ResetMethod(void);
    void WriteOutputPortMethod(void);
    void CmdResetMethod(void);
    void CancelCmdResetMethod(void);
    void PCLKPeriodMethod(void);
    void CounterPeriodMethod(void);
    void CombineSignalMethod(void);
    void ComparatorTestMethod(void);

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
    sc_in<sc_dt::uint64 >   pclk;
    sc_in<sc_dt::uint64 >   cntclk; 
    sc_in<bool>             preset_n;
    sc_in<bool>             cntclk_preset_n;
    sc_in<bool>             erroutresz;
    sc_in<bool>             cntclk_erroutresz;
    sc_in<bool>             resstg1z;
    sc_in<bool>             pclkin_resstg1z;
    //ecmterrinN (N = 000 .. 331)
    sc_in<bool>             *ecmterrin[emNumErrInPort];

    sc_in<bool>             ecmterrlbz_m;
    sc_in<bool>             ecmterrlbz_c;
    sc_in<bool>             ecmttin;
    sc_in<bool>             errin_mask_reset;

    //Declare output ports
    sc_out<bool>            ecmterroz;
    sc_out<bool>            *ecmterroz_pe[emNumPE];
    sc_out<bool>            ecmterroutz;
    sc_out<bool>            *ecmti_pe[emNumPE];
    sc_out<bool>            *ecmtnmi[emNumPE];
    sc_out<bool>            ecmtresz;
    sc_out<bool>            *ecmdclsint[emNumPE];
    sc_out<bool>            ecmterroutz_m;
    sc_out<bool>            ecmterroutz_c;

    //Declare Master, Checker
    Cecm_u2a            *mMaster;
    Cecm_u2a            *mChecker;

    sc_core::sc_attribute<std::string> TARGET_PRODUCT;

    //Public functions
    std::string GetMasterCheckerName(const bool is_master) const;

    //Construct and Destruct
    SC_HAS_PROCESS(Cecm_u2a_wp);
    Cecm_u2a_wp(sc_module_name name);
    ~Cecm_u2a_wp();

    #ifdef __WAVE_DUMP_DEBUG__
    void vcd_trace(sc_trace_file *tf) {
        if (tf) {
            std::string nm = std::string(name());
            sc_trace(tf, pclk                   , nm + ".pclk");
            sc_trace(tf, cntclk                 , nm + ".cntclk"); 
            sc_trace(tf, preset_n               , nm + ".preset_n");
            sc_trace(tf, cntclk_preset_n        , nm + ".cntclk_preset_n");
            sc_trace(tf, erroutresz             , nm + ".erroutresz");
            sc_trace(tf, cntclk_erroutresz      , nm + ".cntclk_erroutresz");
            sc_trace(tf, resstg1z               , nm + ".resstg1z");
            sc_trace(tf, pclkin_resstg1z        , nm + ".pclkin_resstg1z");
            for (unsigned i = 0; i <= emNumErrInPort; i++) {
                std::ostringstream str_ecmterrin;
                str_ecmterrin<<".ecmterrin"<<i;
                sc_trace(tf, *ecmterrin[i]                        , nm + str_ecmterrin.str()); 
            }
            sc_trace(tf, ecmterrlbz_m     , nm + ".ecmterrlbz_m");
            sc_trace(tf, ecmterrlbz_c     , nm + ".ecmterrlbz_c");
            sc_trace(tf, ecmttin          , nm + ".ecmttin");
            sc_trace(tf, errin_mask_reset , nm + ".errin_mask_reset");
            sc_trace(tf, ecmterroz        , nm + ".ecmterroz");
            for (unsigned i = 0; i < emNumPE; i++) {
                std::ostringstream str_ecm;
                str_ecm<<".ecmterroz_pe"<<i;
                sc_trace(tf, *ecmterroz_pe[i]       , nm + str_ecm.str());

                std::ostringstream str_ecm_dcls;
                str_ecm_dcls << ".ecmdclsint" << i;
                sc_trace(tf, *ecmdclsint[i]   , nm + str_ecm_dcls.str());
            }
            sc_trace(tf, ecmterroutz      , nm + ".ecmterroutz");
            sc_trace(tf, ecmtresz         , nm + ".ecmtresz");
            sc_trace(tf, ecmterroutz_m    , nm + ".ecmterroutz_m");
            sc_trace(tf, ecmterroutz_c    , nm + ".ecmterroutz_c");
        }
    }
    #endif
};
#endif //__ECM_U2_H__

// vim600: set foldmethod=marker :
