// ---------------------------------------------------------------------
// $Id: ECC_CRAM_E2.h,v 1.1 2019/09/26 10:07:06 landang Exp $
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
#ifndef __ECC_CRAM_E2_H__
#define __ECC_CRAM_E2_H__

#include "global.h"
#include "rvc_common_model.h"
#include "re_define.h"
#include "BusSlaveBase.h"
#include "ECC_CRAM_E2_AgentController.h"
#include "ECC_def.h"

#define DEFAULT_AWIDTH_MSB 19
#define DEFAULT_AWIDTH_LSB 2
#define DEFAULT_CAP_DEPTH 8
#define DEFAULT_ERRINT_INIT 0x8F
#define HIGHEST_PORT_INDEX 0x13 // of CRam1U: slave port 19

class ECCCNT_CRCn_Func;
class ECCCNT_CRA_Func;
class MECCCAP_CR_Func;

class ECC_CRAM_E2: public sc_module
            ,public ECC_CRAM_E2_AgentController
            ,public BusSlaveBase<32, PE_MAX_NUM+2> // PE_NUM socket for ECCCNT CRCn, 1 for ECCCNT CRA, 1 for MECCCAP CR
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

    // ECCCNT CRC
    sc_out<bool>            **afedisable;    // Address feedback prohibition

    // ECCCNT CRA
    sc_out<bool>            eccdisable; // Detect/correct prohibition
    sc_out<bool>            secdisable; // 1bit correct prohibition

    // MECCCAP_CR
    sc_out<bool>            errovfout1_CR;  // FIFO overflow
    sc_out<bool>            errovfout0_CR;  // Address register buffer overflow
    sc_out<bool>            afeerrout_CR;   // Address feedback error
    sc_out<bool>            dederrout_CR;   // Fatal error
    sc_out<bool>            sederrout_CR;   // 1bit error

    // Key open
    sc_in<bool>             keyopen;        // keyopen to write protected registers

    TlmTargetSocket<32>     *ts_ecccnt_crc[PE_MAX_NUM]; // target sockets 
    TlmTargetSocket<32>     *ts_ecccnt_cra; // target socket
    TlmTargetSocket<32>     *ts_mecccap_cr; // target socket

    ECCCNT_CRCn_Func        **mpECCCNT_CRCFunc;  // Pointer of ECCCNT_CRCn Func
    ECCCNT_CRA_Func         *mpECCCNT_CRAFunc;          // Pointer of ECCCNT_CRA Func
    MECCCAP_CR_Func         *mpMECCCAP_CRFunc;          // Pointer of MECCCAP_CR Func

private:
    // Enums
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

    enum eCRAMInfo{
        emTotalSlavePorts= PE_MAX_NUM + 8   // Total slave ports
        ,emNumMaskedArea = 2            // Number of masked area in model
    };

    enum eGeneralInfo{
        emNumTgt = PE_MAX_NUM + 2
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

    unsigned int    mAWIDTH_MSB;    // Store AWIDTH_MSB parameter
    unsigned int    mAWIDTH_LSB;    // Store AWIDTH_LSB parameter
    unsigned int    mCAP_DEPTH;     // Store CAP_DEPTH parameter
    unsigned int    mERRINT_INIT;   // Store ERRINT_INIT parameter
    bool            mDumpInterrupt; // Dump interrupt info or not
    
    bool            mEnableRegisterMessage; // Is enable regisyer message or not
    sc_dt::uint64   mTimeResolutionValue;   // Time resolution value of simulation
    sc_time_unit    mTimeResolutionUnit;    // Time resolution unit of simulation

    bool            mAFEDIS[PE_MAX_NUM];    // Store value of afedisable
    bool            mECCDIS;    // Store value of eccdisable
    bool            mSECDIS;    // Store value of secdisable
    bool            mKeyOpen;       // Store value of keycode port
    bool            mERROVFOUT0_CR; // Store value of errovfout0_CR
    bool            mERROVFOUT1_CR; // Store value of errovfout1_CR
    bool            mSEDERROUT_CR;  // Store value of sederrout_CR
    bool            mDEDERROUT_CR;  // Store value of dederrout_CR
    bool            mAFEERROUT_CR;  // Store value of afeerrout_CR

    bool            mHas2bitError;      // Indicate has 2bit error in highest priority slave port
    bool            mHasAddrFeedbackError;// Indicate has address feedback error in highest priority slave port
    unsigned int    mFatalErrorModAddr; // Store fatal address needed to be captured
    unsigned int    mFatalEDLCode;      // Store EDL code needed to be captured

    bool            mIsEDLConfig;   // Indicate EDL is configured from Python IF or not
    bool            mIsAddrConfig;  // Indicate Error Address is configured from Python IF or not
    eErrorType      mErrorType;     // Error type
    unsigned int    mErrorEDL;      // Error EDL
    unsigned int    mErrorAddress;  // Error address
    std::string     mStrEDL;        // String of EDL (used in info message)
    unsigned int    mPeId;          // pe index

    ErrorInfo       m1bitErrorList[emTotalSlavePorts];  // List of 1bit error input
    ErrorInfo       mFatalErrorList[emTotalSlavePorts*3]; // List of fatal error input
    unsigned int    mSize1bitErrorList;                 // Indicate number of error in 1bit error list
    unsigned int    mSizeFatalErrorList;                // Indicate number of error in fatal error list
    std::queue<ErrorInfo>               m1bitErrorFIFO; // 1bit Error input FIFO
    std::map<std::string, unsigned int> mEDLmap;        // EDL map in CRAM

    // Events
    sc_event mClkZeroEvent[emNumClk];           // Notify clock is zero

    sc_event mResetHardEvent[emNumReset];       // Call HandleResetHardMethod when reset is active
    sc_event mResetCmdEvent[emNumReset];        // Call HandleResetCmdMethod when AssertReset is called
    sc_event mResetCmdCancelEvent[emNumReset];  // Event to cancel reset by command

    sc_event mWriteDisPort_CRCEvent[PE_MAX_NUM];    // Trigger WriteDisPort_CRCMethod
    sc_event mWriteDisPort_CRAEvent;            // Trigger WriteDisPort_CRAMethod
    sc_event mWriteERROVFOUT0_CREvent;          // Trigger WriteERROVFOUT0_CRMethod
    sc_event mWriteERROVFOUT1_CREvent;          // Trigger WriteERROVFOUT1_CRMethod
    sc_event mWriteSEDERROUT_CREvent;           // Trigger WriteSEDERROUT_CRMethod
    sc_event mNegateERROVFOUT0_CREvent;         // Trigger NegateERROVFOUT0_CRMethod
    sc_event mNegateERROVFOUT1_CREvent;         // Trigger NegateERROVFOUT1_CRMethod
    sc_event mAssertSEDERROUT_CREvent;          // Trigger AssertSEDERROUT_CRMethod
    sc_event mNegateSEDERROUT_CREvent;          // Trigger NegateSEDRROUT_CRMethod
    sc_event mProcess1bitErrorEvent;            // Trigger Process1bitErrorMethod
    sc_event mProcessFatalErrorEvent;           // Trigger ProcessFatalErrorMethod
    sc_event mSetFatalErrorStatusEvent;         // Trigger SetFatalErrorStatusMethod
    sc_event mNotifyFatalErrorEvent;            // Trigger NotifyFatalErrorMethod
    sc_event mNegateDEDERROUT_CREvent;          // Trigger NegateDEDERROUT_CRMethod
    sc_event mNegateAFEERROUT_CREvent;          // Trigger NegateAFEERROUT_CRMethod
    sc_event mWriteDEDERROUT_CREvent;           // Trigger WriteDEDERROUT_CRMethod
    sc_event mWriteAFEERROUT_CREvent;           // Trigger WriteAFEERROUT_CRMethod
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
    void WriteDisPort_CRCMethod(unsigned int pe_id);
    void WriteDisPort_CRAMethod();
    void WriteERROVFOUT0_CRMethod();
    void WriteERROVFOUT1_CRMethod();
    void WriteSEDERROUT_CRMethod();
    void AssertSEDERROUT_CRMethod();
    void NegateSEDRROUT_CRMethod();
    void NegateERROVFOUT0_CRMethod();
    void NegateERROVFOUT1_CRMethod();
    void Set1bitErrorStatusThread();
    void Process1bitErrorMethod();
    void ProcessFatalErrorMethod();
    void SetFatalErrorStatusMethod();
    void NotifyFatalErrorMethod();
    void NegateDEDERROUT_CRMethod();
    void NegateAFEERROUT_CRMethod();
    void WriteDEDERROUT_CRMethod();
    void WriteAFEERROUT_CRMethod();

    // Functions
    void Initialize (void);
    void EnableReset (const std::string reset_name, const bool is_active);
    void CancelEvents();
    bool CheckClockPeriod (const std::string clock_name);

    void CreateEDLMap();
    std::string CalculateEDLCode();
    bool CheckNoFatalStatus();
    bool CheckMaskAddress( const unsigned int address);
    bool Check1bitAddressNotCaptured(unsigned int mod_address);
    void Update1bitErrorList();
    void UpdateFatalErrorList();
    std::string CombineStringUint(std::string name, unsigned int index);
    unsigned int ShiftAddress(unsigned int address);
    void DumpInfo(const char *type, const char *message, ...);

    void AssertERROVFOUT0_CR();
    void AssertERROVFOUT1_CR();
    // Overwrite firtual functions of ECC_CRAM_E2_AgentController
    void WriteDisPort_CRC(unsigned int pe_id, unsigned int value);
    void WriteDisPort_CRA(unsigned int value);
    unsigned int GetCapDepth();
    bool GetKeyOpen();
    bool GetResetStatus (const std::string reset_name);
    unsigned int GetAWIDTH_MSB();

public:
    SC_HAS_PROCESS (ECC_CRAM_E2);
    ECC_CRAM_E2 (sc_module_name name, const uint8_t iPE_NUM, const unsigned int rLatency, const unsigned int wLatency);
    ~ECC_CRAM_E2 ();

    // PythonIF
    void EnableRegisterMessage(const std::string is_enable);
    void SetMessageLevel (const std::string msg_lv);
    void DumpRegisterRW (const std::string is_enable);
    void DumpInterrupt (const std::string is_enable);
    void AWIDTH_MSB (const unsigned int value);
    void AWIDTH_LSB (const unsigned int value);
    void CAP_DEPTH (const unsigned int value);
    void ERRINT_INIT (const unsigned int value);
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
#endif //__ECC_CRAM_E2_H__
