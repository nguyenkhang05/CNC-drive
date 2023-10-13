// ---------------------------------------------------------------------
// $Id: ECC_LRAM_E2.h,v 1.1 2019/09/26 10:07:33 landang Exp $
//
// Copyright(c) 2017 Renesas Electronics Corporation
// Copyright(c) 2017 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------
#ifndef __ECC_LRAM_E2_H__
#define __ECC_LRAM_E2_H__

#include "global.h"
#include "re_define.h"
#include "BusSlaveBase.h"
#include "ECC_LRAM_E2_AgentController.h"
#include "ECC_def.h"
#include "rvc_common_model.h"

#define DEFAULT_AWIDTH_MSB_LR0 14
#define DEFAULT_AWIDTH_LSB_LR0 2
#define DEFAULT_CAP_DEPTH_LR0 8
#define DEFAULT_ERRINT_INIT_LR0 0x8F
#define HIGHEST_PORT_INDEX_LR0 0x4 // of bank3

#define DEFAULT_AWIDTH_MSB_LR1 21
#define DEFAULT_AWIDTH_LSB_LR1 2
#define DEFAULT_CAP_DEPTH_LR1 8
#define DEFAULT_ERRINT_INIT_LR1 0x87
#define HIGHEST_PORT_INDEX_LR1 0x7 // because support max pe number (8)

class ECCCNT_LR0_PEn_Func;
class MECCCAP_LR0_PEn_Func;
class MECCCAP_LR1_Func;


class ECC_LRAM_E2: public sc_module
            ,public ECC_LRAM_E2_AgentController
            ,public BusSlaveBase<32, PE_MAX_NUM*2 + 1> // PE_NUM socket for ECCCNT LR0, PE_NUM socket for MECCCAP LR0, 1 for MECCCAP LRA
            ,public rvc_common_model
{
private:
    uint8_t PE_NUM;
public:
    // Clock/Reset
    sc_in<sc_dt::uint64>    pclk;           // clock
    sc_in<bool>             presetn;        // reset
    sc_in<sc_dt::uint64>    sys_clk;        // sys clock
    sc_in<bool>             sys_resetz;     // sys reset

    // ECCCNT LR0 PEn
    sc_out<bool>            **eccdisable;    // detect/correct prohibition
    sc_out<bool>            **secdisable;    // 1bit correct prohibition
    sc_out<bool>            **afedisable;    // address feedback error detection prohibition

    // MECCCAP LR0
    sc_out<bool>            **errovfout1_LR0;// FIFO overflow
    sc_out<bool>            **errovfout0_LR0;// Address register buffer overflow
    sc_out<bool>            **afeerrout_LR0; // Address feedback error
    sc_out<bool>            **dederrout_LR0; // Fatal error
    sc_out<bool>            **sederrout_LR0; // 1bit error

    // MECCCAP_LR1
    sc_out<bool>            errovfout1_LR1;  // FIFO overflow
    sc_out<bool>            errovfout0_LR1;  // Address register buffer overflow
    sc_out<bool>            dederrout_LR1;   // Fatal error
    sc_out<bool>            sederrout_LR1;   // 1bit error

    // Key open
    sc_in<bool>             keyopen;        // keyopen to write protected registers


    TlmTargetSocket<32>     *ts_ecccnt_lr[PE_MAX_NUM];  // target sockets 
    TlmTargetSocket<32>     *ts_mecccap_lr0[PE_MAX_NUM];// target socket
    TlmTargetSocket<32>     *ts_mecccap_lr1; // target socket
    
    ECCCNT_LR0_PEn_Func     **mpECCCNT_LR0Func;  // Pointer of ECCCNT_LR0_PEn Func
    MECCCAP_LR0_PEn_Func    **mpMECCCAP_LR0Func; // Pointer of MECCCAP_LR0_PEn Func
    MECCCAP_LR1_Func        *mpMECCCAP_LR1Func; // Pointer of MECCCAP_LR1 Func

private:
    // Enums
    enum eTgtInfo{
        emNumTgt           = PE_MAX_NUM*2 + 1
    };

    enum eClockInfo{
        empclkId        = 0
        ,emsys_clkId    = 1
        ,emNumClk       = 2
    };

    enum eResetInfo{
        empresetnId     = 0
        ,emsys_resetzId = 1
        ,emNumReset     = 2
    };

    enum eLRAMInfo{
        emTotalSlavePorts_LR0   = 5     // Total slave ports of LR0
        ,emTotalSlavePorts_LR1  = PE_MAX_NUM// Total slave ports (in used) of LR1
        ,emNumMaskedArea        = 2     // Number of masked area in model
    };


    // Variables
    sc_dt::uint64   mClkPeriod[emNumClk];       // Period of clock
    sc_dt::uint64   mClkOrgFreq[emNumClk];      // Previous frequency of clock
    sc_dt::uint64   mClkFreq[emNumClk];         // Frequency of clock
    std::string     mClkFreqUnit[emNumClk];     // Frequency unit of clock
    double          mStartClkTime[emNumClk];    // Start time of clock

    bool            mResetCurVal[emNumReset]; //Store current value of reset signals
    bool            mResetPreVal[emNumReset]; //Store previous value of reset signals
    bool            mIsResetHardActive[emNumReset]; // Is reset by port
    bool            mIsResetCmdReceive[emNumReset]; // Is receive cmd reset
    bool            mIsResetCmdActive[emNumReset];  // Is reset by cmd
    double          mResetCmdPeriod[emNumReset];    // Period of cmd reset

    unsigned int    mAWIDTH_MSB_LR0;    // Store AWIDTH_MSB parameter of LR0
    unsigned int    mAWIDTH_LSB_LR0;    // Store AWIDTH_LSB parameter of LR0
    unsigned int    mCAP_DEPTH_LR0;     // Store CAP_DEPTH parameter of LR0
    unsigned int    mERRINT_INIT_LR0;   // Store ERRINT_INIT parameter of LR0

    unsigned int    mAWIDTH_MSB_LR1;    // Store AWIDTH_MSB parameter of LR1
    unsigned int    mAWIDTH_LSB_LR1;    // Store AWIDTH_LSB parameter of LR1
    unsigned int    mCAP_DEPTH_LR1;     // Store CAP_DEPTH parameter of LR1
    unsigned int    mERRINT_INIT_LR1;   // Store ERRINT_INIT parameter of LR1

    bool            mDumpInterrupt;     // Enable dump interrupt message
    
    bool            mEnableRegisterMessage; // Is enable regisyer message or not
    sc_dt::uint64   mTimeResolutionValue;   // Time resolution value of simulation
    sc_time_unit    mTimeResolutionUnit;    // Time resolution unit of simulation

    bool            mECCDIS[PE_MAX_NUM];    // Store value of eccdisable
    bool            mSECDIS[PE_MAX_NUM];    // Store value of secdisable
    bool            mAFEDIS[PE_MAX_NUM];    // Store value of afedisable
    bool            mKeyOpen;               // Store value of keycode port
    bool            mERROVFOUT0_LR0[PE_MAX_NUM];// Store value of errovfout0_LR0
    bool            mERROVFOUT1_LR0[PE_MAX_NUM];// Store value of errovfout1_LR0
    bool            mSEDERROUT_LR0[PE_MAX_NUM]; // Store value of sederrout_LR0
    bool            mDEDERROUT_LR0[PE_MAX_NUM]; // Store value of dederrout_LR0
    bool            mAFEERROUT_LR0[PE_MAX_NUM]; // Store value of afeerrout_LR0
    bool            mERROVFOUT0_LR1;    // Store value of errovfout0_LR1
    bool            mERROVFOUT1_LR1;    // Store value of errovfout1_LR1
    bool            mSEDERROUT_LR1;     // Store value of sederrout_LR1
    bool            mDEDERROUT_LR1;     // Store value of dederrout_LR1

    bool            mHas2bitError_LR0[PE_MAX_NUM];              // Indicate has 2bit error in highest priority slave port
    bool            mHasAddrFeedbackError_LR0[PE_MAX_NUM];      // Indicate has address feedback error in highest priority slave port
    unsigned int    mFatalErrorModAddr_LR0[PE_MAX_NUM];         // Fatal error address to capture
    unsigned int    mFatalEDLCode_LR0[PE_MAX_NUM];              // Fatal EDL to capture

    bool            mHas2bitError_LR1;             // Indicate has 2bit error in highest priority slave port
    unsigned int    mFatalErrorModAddr_LR1;        // Fatal error address to capture
    unsigned int    mFatalEDLCode_LR1;             // Fatal EDL to capture

    bool            mIsEDLConfig;       // Indicate EDL is configured from Python IF or not
    bool            mIsAddrConfig;      // Indicate Error Address is configured from Python IF or not
    eErrorType      mErrorType;         // Error type
    unsigned int    mErrorEDL;          // Error EDL
    unsigned int    mErrorAddress;      // Error address
    std::string     mCurConfigSubName;  // Indicate LR0/LR1 is now configured error info
    unsigned int    mPeId;              // Indicate current pe_id will receive error
    std::string     mStrEDL;            // Store input EDL in command Python

    ErrorInfo       m1bitErrorList_LR0[PE_MAX_NUM][emTotalSlavePorts_LR0];  // List of 1bit error input of LR0
    ErrorInfo       mFatalErrorList_LR0[PE_MAX_NUM][emTotalSlavePorts_LR0*3]; // List of fatal error input of LR0
    unsigned int    mSize1bitErrorList_LR0[PE_MAX_NUM];                     // Indicate number of error in 1bit error list
    unsigned int    mSizeFatalErrorList_LR0[PE_MAX_NUM];                    // Indicate number of error in fatal error list
    std::queue<ErrorInfo>               m1bitErrorFIFO_LR0[PE_MAX_NUM];     // 1bit Error input FIFO of each MECCCAP of LR0
    std::map<std::string, unsigned int> mEDLmap_LR0;                    // EDL map in LR0

    ErrorInfo       m1bitErrorList_LR1[emTotalSlavePorts_LR1];  // List of 1bit error input of LR1
    ErrorInfo       mFatalErrorList_LR1[emTotalSlavePorts_LR1*2]; // List of fatal error input of LR1
    unsigned int    mSize1bitErrorList_LR1;                     // Indicate number of error in 1bit error list
    unsigned int    mSizeFatalErrorList_LR1;                    // Indicate number of error in fatal error list
    std::queue<ErrorInfo>               m1bitErrorFIFO_LR1;     // 1bit Error input FIFO of each MECCCAP of LR1
    std::map<std::string, unsigned int> mEDLmap_LR1;            // EDL map in LR1

    // Events
    sc_event mClkZeroEvent[emNumClk];           // Notify clock is zero

    sc_event mResetHardEvent[emNumReset];       // Call HandleResetHardMethod when reset is active
    sc_event mResetCmdEvent[emNumReset];        // Call HandleResetCmdMethod when AssertReset is called
    sc_event mResetCmdCancelEvent[emNumReset];  // Event to cancel reset by command

    sc_event mWriteProhibitionEvent[PE_MAX_NUM];    // Trigger WriteProhibitionPortMethod
    sc_event mWriteERROVFOUT0_LR0Event[PE_MAX_NUM]; // Trigger WriteERROVFOUT0_LR0Method
    sc_event mWriteERROVFOUT1_LR0Event[PE_MAX_NUM]; // Trigger WriteERROVFOUT1_LR0Method
    sc_event mNegateERROVFOUT0_LR0Event[PE_MAX_NUM];// Trigger NegateERROVFOUT0_LR0Method
    sc_event mNegateERROVFOUT1_LR0Event[PE_MAX_NUM];// Trigger NegateERROVFOUT1_LR0Method
    sc_event mWriteSEDERROUT_LR0Event[PE_MAX_NUM];  // Trigger WriteSEDERROUT_LR0Method
    sc_event mAssertSEDERROUT_LR0Event[PE_MAX_NUM]; // Trigger AssertSEDERROUT_LR0Method
    sc_event mNegateSEDERROUT_LR0Event[PE_MAX_NUM]; // Trigger NegateSEDERROUT_LR0Method
    sc_event mNegateDEDERROUT_LR0Event[PE_MAX_NUM]; // Trigger NegateDEDERROUT_LR0Method
    sc_event mWriteDEDERROUT_LR0Event[PE_MAX_NUM];  // Trigger WriteDEDERROUT_LR0Method
    sc_event mProcess1bitError_LR0Event[PE_MAX_NUM];    // Trigger Process1bitError_LR0Method
    sc_event mProcessFatalError_LR0Event[PE_MAX_NUM];   // Trigger ProcessFatalError_LR0Method
    sc_event mSetFatalErrorStatus_LR0Event[PE_MAX_NUM]; // Trigger SetFatalErrorStatus_LR0Method
    sc_event mNotifyFatalError_LR0Event[PE_MAX_NUM];    // Trigger NotifyFatalError_LR0Method
    sc_event mNegateAFEERROUT_LR0Event[PE_MAX_NUM];     // Trigger NegateAFEERROUT_LR0Method
    sc_event mWriteAFEERROUT_LR0Event[PE_MAX_NUM];      // Trigger WriteAFEERROUT_LR0Method

    sc_event mWriteERROVFOUT0_LR1Event;         // Trigger WriteERROVFOUT0_LR1Method
    sc_event mWriteERROVFOUT1_LR1Event;         // Trigger WriteERROVFOUT1_LR1Method
    sc_event mNegateERROVFOUT0_LR1Event;        // Trigger NegateERROVFOUT0_LR1Method
    sc_event mNegateERROVFOUT1_LR1Event;        // Trigger NegateERROVFOUT1_LR1Method
    sc_event mWriteSEDERROUT_LR1Event;          // Trigger WriteSEDERROUT_LR1Method
    sc_event mAssertSEDERROUT_LR1Event;         // Trigger AssertSEDERROUT_LR1Method
    sc_event mNegateSEDERROUT_LR1Event;         // Trigger NegateSEDERROUT_LR1Method
    sc_event mNegateDEDERROUT_LR1Event;         // Trigger NegateDEDERROUT_LR1Method
    sc_event mWriteDEDERROUT_LR1Event;          // Trigger WriteDEDERROUT_LR1Method
    sc_event mProcess1bitError_LR1Event;        // Trigger Process1bitError_LR1Method
    sc_event mProcessFatalError_LR1Event;       // Trigger ProcessFatalError_LR1Method
    sc_event mSetFatalErrorStatus_LR1Event;     // Trigger SetFatalErrorStatus_LR1Method
    sc_event mNotifyFatalError_LR1Event;        // Trigger NotifyFatalError_LR1Method

    sc_event mSet1bitErrorStatusEvent;          // Trigger Set1bitErrorStatusThread



    // Threads/Methods
    void MonitorpclkMethod ();
    void Monitorsys_clkMethod ();

    void MonitorpresetnMethod ();
    void Monitorsys_resetzMethod ();
    void HandleResetHardMethod (const unsigned int reset_id);
    void HandleResetCmdMethod (const unsigned int reset_id);
    void CancelResetCmdMethod (const unsigned int reset_id);

    void MonitorkeyopenMethod();
    void WriteProhibitionPortMethod(unsigned int pe_id);
    void WriteERROVFOUT0_LR0Method(unsigned int pe_id);
    void WriteERROVFOUT1_LR0Method(unsigned int pe_id);
    void NegateERROVFOUT0_LR0Method(unsigned int pe_id);
    void NegateERROVFOUT1_LR0Method(unsigned int pe_id);
    void WriteSEDERROUT_LR0Method(unsigned int pe_id);
    void AssertSEDERROUT_LR0Method(unsigned int pe_id);
    void NegateSEDERROUT_LR0Method(unsigned int pe_id);
    void NegateDEDERROUT_LR0Method(unsigned int pe_id);
    void NegateAFEERROUT_LR0Method(unsigned int pe_id);
    void WriteDEDERROUT_LR0Method(unsigned int pe_id);
    void WriteAFEERROUT_LR0Method(unsigned int pe_id);
    void Process1bitError_LR0Method(unsigned int pe_id);
    void ProcessFatalError_LR0Method(unsigned int pe_id);
    void SetFatalErrorStatus_LR0Method(unsigned int pe_id);
    void NotifyFatalError_LR0Method(unsigned int pe_id);
    void WriteERROVFOUT0_LR1Method();
    void WriteERROVFOUT1_LR1Method();
    void NegateERROVFOUT0_LR1Method();
    void NegateERROVFOUT1_LR1Method();
    void WriteSEDERROUT_LR1Method();
    void AssertSEDERROUT_LR1Method();
    void NegateSEDERROUT_LR1Method();
    void NegateDEDERROUT_LR1Method();
    void WriteDEDERROUT_LR1Method();
    void Process1bitError_LR1Method();
    void ProcessFatalError_LR1Method();
    void SetFatalErrorStatus_LR1Method();
    void NotifyFatalError_LR1Method();
    void Set1bitErrorStatusThread();

    // Functions
    void Initialize (void);
    void EnableReset (const std::string reset_name, const bool is_active);
    void CancelEvents();
    bool CheckClockPeriod (const std::string clock_name);

    void CreateEDLMap();
    bool CheckNoFatalStatus_LR0(unsigned int pe_id);
    bool CheckNoFatalStatus_LR1();
    bool CheckMaskAddress_LR0(const unsigned int pe_id, const unsigned int address);
    bool CheckMaskAddress_LR1(const unsigned int address);
    bool Check1bitAddressNotCaptured_LR0(unsigned int pe_id, unsigned int mod_address);
    bool Check1bitAddressNotCaptured_LR1(unsigned int mod_address);
    void Update1bitErrorList_LR0(unsigned int pe_id);
    void UpdateFatalErrorList_LR0(unsigned int pe_id);
    void Update1bitErrorList_LR1();
    void UpdateFatalErrorList_LR1();
    std::string CombineStringUint(std::string name, unsigned int index);
    unsigned int ShiftAddress(std::string sub_name, unsigned int address);
    void DumpInfo(const char *type, const char *message, ...);
    std::string CalculateEDLCode();
    bool CheckAnyFIFOHasElement();

    // Overwrite firtual functions of ECC_LRAM_E2_AgentController
    void WriteProhibitionSignal(unsigned int pe_id, unsigned int value);
    unsigned int GetCapDepth(std::string sub_name);
    void AssertERROVFOUT0_LR0(unsigned int pe_id);
    void AssertERROVFOUT1_LR0(unsigned int pe_id);
    void AssertERROVFOUT0_LR1();
    void AssertERROVFOUT1_LR1();
    bool GetKeyOpen();
    bool GetResetStatus (const std::string reset_name);
    unsigned int GetAWIDTH_MSB(std::string sub_name);

public:
    SC_HAS_PROCESS (ECC_LRAM_E2);
    ECC_LRAM_E2 (sc_module_name name, const uint8_t iPE_NUM, const unsigned int rLatency, const unsigned int wLatency);
    ~ECC_LRAM_E2 ();

    // PythonIF
    void EnableRegisterMessage(const std::string is_enable);
    void SetMessageLevel (const std::string msg_lv);
    void DumpRegisterRW (const std::string is_enable);
    void DumpInterrupt (const std::string is_enable);
    void AWIDTH_MSB (const std::string sub_name, const unsigned int value);
    void AWIDTH_LSB (const std::string sub_name, const unsigned int value);
    void CAP_DEPTH (const std::string sub_name, const unsigned int value);
    void ERRINT_INIT (const std::string sub_name, const unsigned int value);
    void IssueError (const unsigned int errortype);
    void SetErrorAddr (const unsigned int addr_value);
    void SetEDL (const std::string edl, const unsigned int pe_index);
    void DumpStatusInfo ();
    void AssertReset (const std::string reset_name, const double start_time, const double period);
    void SetCLKFreq (const std::string clock_name, const sc_dt::uint64  freq, const std::string unit);
    void GetCLKFreq (const std::string clock_name);
    void ForceRegister (const std::string block_name, const unsigned int block_index, const std::string reg_name, const unsigned int reg_value);
    void ReleaseRegister (const std::string block_name, const unsigned int block_index, const std::string reg_name);
    void WriteRegister (const std::string block_name, const unsigned int block_index, const std::string reg_name, const unsigned int reg_value);
    void ReadRegister (const std::string block_name, const unsigned int block_index, const std::string reg_name);
    void ListRegister (void);
    void Help (const std::string type);
};
#endif //__ECC_LRAM_E2_H__
