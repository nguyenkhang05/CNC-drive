// ---------------------------------------------------------------------
// $Id: ECC_ICACHE_E2.h,v 1.1 2019/09/26 10:07:26 landang Exp $
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
#ifndef __ECC_ICACHE_E2_H__
#define __ECC_ICACHE_E2_H__

#include "global.h"
#include "re_define.h"
#include "BusSlaveBase.h"
#include "ECC_ICACHE_E2_AgentController.h"
#include "ECC_def.h"
#include "rvc_common_model.h"

#define DEFAULT_AWIDTH_MSB_IT 11
#define DEFAULT_AWIDTH_LSB_IT 5
#define DEFAULT_CAP_DEPTH_IT 1
#define DEFAULT_ERRINT_INIT_IT 0x0
#define HIGHEST_PORT_INDEX_IT 0x3 // 4 slave ports

#define DEFAULT_AWIDTH_MSB_ID 11
#define DEFAULT_AWIDTH_LSB_ID 3
#define DEFAULT_CAP_DEPTH_ID 1
#define DEFAULT_ERRINT_INIT_ID 0x0
#define HIGHEST_PORT_INDEX_ID 0x3 // 4 slave ports


class ECCCNT_IT_PEn_Func;
class ECCCNT_ID_PEn_Func;
class MECCCAP_IT_PEn_Func;
class MECCCAP_ID_PEn_Func;


class ECC_ICACHE_E2: public sc_module
            ,public ECC_ICACHE_E2_AgentController
            ,public BusSlaveBase<32, PE_MAX_NUM*4> // each ecccnt/mecccap it/id has PE_NUM PE
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

    // ECCCNT IT PEn
    sc_out<bool>            **eccdisable_it;     // detect/correct prohibition
    sc_out<bool>            **afedisable_it;     // address feedback error detection prohibition

    // ECCCNT ID PEn
    sc_out<bool>            **eccdisable_id;     // detect/correct prohibition
    sc_out<bool>            **afedisable_id;     // address feedback error detection prohibition

    // MECCCAP IT PEn
    sc_out<bool>            **errovfout1_IT;         // FIFO overflow
    sc_out<bool>            **errovfout0_IT;         // Address register buffer overflow
    sc_out<bool>            **dederrout_IT;          // 2bit error
    sc_out<bool>            **afeerrout_IT;          // Address feedback error
    sc_out<bool>            **sederrout_IT;          // 1bit error

    // MECCCAP ID PEn
    sc_out<bool>            **errovfout1_ID;         // FIFO overflow
    sc_out<bool>            **errovfout0_ID;         // Address register buffer overflow
    sc_out<bool>            **dederrout_ID;          // 2bit error
    sc_out<bool>            **afeerrout_ID;          // Address feedback error
    sc_out<bool>            **sederrout_ID;          // 1bit error

    // Key open
    sc_in<bool>             keyopen;        // keyopen to write protected registers


    TlmTargetSocket<32>     *ts_ecccnt_it[PE_MAX_NUM];      // target sockets for ECCCNT IT PEn 
    TlmTargetSocket<32>     *ts_ecccnt_id[PE_MAX_NUM];      // target sockets for ECCCNT ID PEn
    TlmTargetSocket<32>     *ts_mecccap_it[PE_MAX_NUM];     // target sockets for MECCCAP IT PEn
    TlmTargetSocket<32>     *ts_mecccap_id[PE_MAX_NUM];     // target sockets for MECCCAP ID PEn

    ECCCNT_IT_PEn_Func      **mpECCCNT_ITFunc;   // Pointer of ECCCNT IT PEn Func
    ECCCNT_ID_PEn_Func      **mpECCCNT_IDFunc;   // Pointer of ECCCNT ID PEn Func
    MECCCAP_IT_PEn_Func     **mpMECCCAP_ITFunc;  // Pointer of MECCCAP IT PEn Func
    MECCCAP_ID_PEn_Func     **mpMECCCAP_IDFunc;  // Pointer of MECCCAP ID PEn Func

private:
    // Enums
    enum eTgtInfo{
        emNumTgt           = PE_MAX_NUM*4
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

    enum eSlavePortInfo{
        emTotalSlavePorts_IT   = 4  // Total slave ports of IT
        ,emTotalSlavePorts_ID  = 4  // Total slave ports of ID
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

    unsigned int    mAWIDTH_MSB_IT;     // Store AWIDTH_MSB parameter of IT
    unsigned int    mAWIDTH_LSB_IT;     // Store AWIDTH_LSB parameter of IT
    unsigned int    mCAP_DEPTH_IT;      // Store CAP_DEPTH parameter of IT
    unsigned int    mERRINT_INIT_IT;    // Store ERRINT_INIT parameter of IT

    unsigned int    mAWIDTH_MSB_ID;     // Store AWIDTH_MSB parameter of ID
    unsigned int    mAWIDTH_LSB_ID;     // Store AWIDTH_LSB parameter of ID
    unsigned int    mCAP_DEPTH_ID;      // Store CAP_DEPTH parameter of ID
    unsigned int    mERRINT_INIT_ID;    // Store ERRINT_INIT parameter of ID

    bool            mDumpInterrupt;     // Enable dump interrupt info or not

    bool            mEnableRegisterMessage; // Is enable regisyer message or not
    sc_dt::uint64   mTimeResolutionValue;   // Time resolution value of simulation
    sc_time_unit    mTimeResolutionUnit;    // Time resolution unit of simulation

    bool            mECCDIS_IT[PE_MAX_NUM]; // Store value of eccdisable of IT
    bool            mAFEDIS_IT[PE_MAX_NUM]; // Store value of afedisable of IT

    bool            mECCDIS_ID[PE_MAX_NUM]; // Store value of eccdisable of ID
    bool            mAFEDIS_ID[PE_MAX_NUM]; // Store value of afedisable of ID

    bool            mKeyOpen;               // Store value of keycode port

    bool            mERROVFOUT0_IT[PE_MAX_NUM]; // Store value of errovfout0_IT
    bool            mERROVFOUT1_IT[PE_MAX_NUM]; // Store value of errovfout1_IT
    bool            mSEDERROUT_IT[PE_MAX_NUM];  // Store value of sederrout_IT
    bool            mDEDERROUT_IT[PE_MAX_NUM];  // Store value of dederrout_IT
    bool            mAFEERROUT_IT[PE_MAX_NUM];  // Store value of afeerrout_IT

    bool            mERROVFOUT0_ID[PE_MAX_NUM]; // Store value of errovfout0_ID
    bool            mERROVFOUT1_ID[PE_MAX_NUM]; // Store value of errovfout1_ID
    bool            mSEDERROUT_ID[PE_MAX_NUM];  // Store value of sederrout_ID
    bool            mDEDERROUT_ID[PE_MAX_NUM];  // Store value of dederrout_ID
    bool            mAFEERROUT_ID[PE_MAX_NUM];  // Store value of afeerrout_ID

    bool            mHas2bitError_IT[PE_MAX_NUM];               // Indicate has 2bit error in highest priority slave port
    bool            mHasAddrFeedbackError_IT[PE_MAX_NUM];       // Indicate has address feedback error in highest priority slave port
    unsigned int    mFatalErrorModAddr_IT[PE_MAX_NUM];          // Fatal error address to capture
    unsigned int    mFatalEDLCode_IT[PE_MAX_NUM];               // Fatal EDL to capture

    bool            mHas2bitError_ID[PE_MAX_NUM];               // Indicate has 2bit error in highest priority slave port
    bool            mHasAddrFeedbackError_ID[PE_MAX_NUM];       // Indicate has address feedback error in highest priority slave port
    unsigned int    mFatalErrorModAddr_ID[PE_MAX_NUM];          // Fatal error address to capture
    unsigned int    mFatalEDLCode_ID[PE_MAX_NUM];               // Fatal EDL to capture

    bool            mIsEDLConfig;       // Indicate EDL is configured from Python IF or not
    bool            mIsAddrConfig;      // Indicate Error Address is configured from Python IF or not
    eErrorType      mErrorType;         // Error type
    unsigned int    mErrorEDL;          // Error EDL
    unsigned int    mErrorAddress;      // Error address
    std::string     mCurConfigSubName;  // Indicate LR0/LR1 is now configured error info
    unsigned int    mPeId;              // Indicate current pe_id will receive error
    std::string     mStrEDL;            // Store input EDL in command Python

    ErrorInfo       m1bitErrorList_IT[PE_MAX_NUM][emTotalSlavePorts_IT];  // List of 1bit error input of IT
    ErrorInfo       mFatalErrorList_IT[PE_MAX_NUM][emTotalSlavePorts_IT*2]; // List of fatal error input of IT
    unsigned int    mSize1bitErrorList_IT[PE_MAX_NUM];                     // Indicate number of error in 1bit error list
    unsigned int    mSizeFatalErrorList_IT[PE_MAX_NUM];                    // Indicate number of error in fatal error list
    std::queue<ErrorInfo>               m1bitErrorFIFO_IT[PE_MAX_NUM];     // 1bit Error input FIFO of each MECCCAP of IT
    std::map<std::string, unsigned int> mEDLmap_IT;                    // EDL map in IT

    ErrorInfo       m1bitErrorList_ID[PE_MAX_NUM][emTotalSlavePorts_ID];  // List of 1bit error input of ID
    ErrorInfo       mFatalErrorList_ID[PE_MAX_NUM][emTotalSlavePorts_ID*2]; // List of fatal error input of ID
    unsigned int    mSize1bitErrorList_ID[PE_MAX_NUM];                     // Indicate number of error in 1bit error list
    unsigned int    mSizeFatalErrorList_ID[PE_MAX_NUM];                    // Indicate number of error in fatal error list
    std::queue<ErrorInfo>               m1bitErrorFIFO_ID[PE_MAX_NUM];     // 1bit Error input FIFO of each MECCCAP of ID
    std::map<std::string, unsigned int> mEDLmap_ID;                    // EDL map in ID

    // Events
    sc_event mClkZeroEvent[emNumClk];           // Notify clock is zero

    sc_event mResetHardEvent[emNumReset];       // Call HandleResetHardMethod when reset is active
    sc_event mResetCmdEvent[emNumReset];        // Call HandleResetCmdMethod when AssertReset is called
    sc_event mResetCmdCancelEvent[emNumReset];  // Event to cancel reset by command

    sc_event mWriteDisPort_ITEvent[PE_MAX_NUM];         // Trigger WriteDisPort_ITMethod
    sc_event mWriteERROVFOUT0_ITEvent[PE_MAX_NUM];      // Trigger WriteERROVFOUT0_ITMethod
    sc_event mWriteERROVFOUT1_ITEvent[PE_MAX_NUM];      // Trigger WriteERROVFOUT1_ITMethod
    sc_event mNegateERROVFOUT0_ITEvent[PE_MAX_NUM];     // Trigger NegateERROVFOUT0_ITMethod
    sc_event mNegateERROVFOUT1_ITEvent[PE_MAX_NUM];     // Trigger NegateERROVFOUT1_ITMethod
    sc_event mWriteSEDERROUT_ITEvent[PE_MAX_NUM];       // Trigger WriteSEDERROUT_ITMethod
    sc_event mAssertSEDERROUT_ITEvent[PE_MAX_NUM];      // Trigger AssertSEDERROUT_ITMethod
    sc_event mNegateSEDERROUT_ITEvent[PE_MAX_NUM];      // Trigger NegateSEDERROUT_ITMethod
    sc_event mNegateDEDERROUT_ITEvent[PE_MAX_NUM];      // Trigger NegateDEDERROUT_ITMethod
    sc_event mNegateAFEERROUT_ITEvent[PE_MAX_NUM];      // Trigger NegateAFEERROUT_ITMethod
    sc_event mWriteDEDERROUT_ITEvent[PE_MAX_NUM];       // Trigger WriteDEDERROUT_ITMethod
    sc_event mWriteAFEERROUT_ITEvent[PE_MAX_NUM];       // Trigger WriteAFEERROUT_ITMethod
    sc_event mProcess1bitError_ITEvent[PE_MAX_NUM];     // Trigger Process1bitError_ITMethod
    sc_event mProcessFatalError_ITEvent[PE_MAX_NUM];    // Trigger ProcessFatalError_ITMethod
    sc_event mSetFatalErrorStatus_ITEvent[PE_MAX_NUM];  // Trigger SetFatalErrorStatus_ITMethod
    sc_event mNotifyFatalError_ITEvent[PE_MAX_NUM];     // Trigger NotifyFatalError_ITMethod

    sc_event mWriteDisPort_IDEvent[PE_MAX_NUM];         // Trigger WriteDisPort_IDMethod
    sc_event mWriteERROVFOUT0_IDEvent[PE_MAX_NUM];      // Trigger WriteERROVFOUT0_IDMethod
    sc_event mWriteERROVFOUT1_IDEvent[PE_MAX_NUM];      // Trigger WriteERROVFOUT1_IDMethod
    sc_event mNegateERROVFOUT0_IDEvent[PE_MAX_NUM];     // Trigger NegateERROVFOUT0_IDMethod
    sc_event mNegateERROVFOUT1_IDEvent[PE_MAX_NUM];     // Trigger NegateERROVFOUT1_IDMethod
    sc_event mWriteSEDERROUT_IDEvent[PE_MAX_NUM];       // Trigger WriteSEDERROUT_IDMethod
    sc_event mAssertSEDERROUT_IDEvent[PE_MAX_NUM];      // Trigger AssertSEDERROUT_IDMethod
    sc_event mNegateSEDERROUT_IDEvent[PE_MAX_NUM];      // Trigger NegateSEDERROUT_IDMethod
    sc_event mNegateDEDERROUT_IDEvent[PE_MAX_NUM];      // Trigger NegateDEDERROUT_IDMethod
    sc_event mNegateAFEERROUT_IDEvent[PE_MAX_NUM];      // Trigger NegateAFEERROUT_IDMethod
    sc_event mWriteDEDERROUT_IDEvent[PE_MAX_NUM];       // Trigger WriteDEDERROUT_IDMethod
    sc_event mWriteAFEERROUT_IDEvent[PE_MAX_NUM];       // Trigger WriteAFEERROUT_IDMethod
    sc_event mProcess1bitError_IDEvent[PE_MAX_NUM];     // Trigger Process1bitError_IDMethod
    sc_event mProcessFatalError_IDEvent[PE_MAX_NUM];    // Trigger ProcessFatalError_IDMethod
    sc_event mSetFatalErrorStatus_IDEvent[PE_MAX_NUM];  // Trigger SetFatalErrorStatus_IDMethod
    sc_event mNotifyFatalError_IDEvent[PE_MAX_NUM];     // Trigger NotifyFatalError_IDMethod

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

    void WriteDisPort_ITMethod(unsigned int pe_id);
    void WriteERROVFOUT0_ITMethod(unsigned int pe_id);
    void WriteERROVFOUT1_ITMethod(unsigned int pe_id);
    void NegateERROVFOUT0_ITMethod(unsigned int pe_id);
    void NegateERROVFOUT1_ITMethod(unsigned int pe_id);
    void WriteSEDERROUT_ITMethod(unsigned int pe_id);
    void AssertSEDERROUT_ITMethod(unsigned int pe_id);
    void NegateSEDERROUT_ITMethod(unsigned int pe_id);
    void NegateDEDERROUT_ITMethod(unsigned int pe_id);
    void NegateAFEERROUT_ITMethod(unsigned int pe_id);
    void WriteDEDERROUT_ITMethod(unsigned int pe_id);
    void WriteAFEERROUT_ITMethod(unsigned int pe_id);
    void Process1bitError_ITMethod(unsigned int pe_id);
    void ProcessFatalError_ITMethod(unsigned int pe_id);
    void SetFatalErrorStatus_ITMethod(unsigned int pe_id);
    void NotifyFatalError_ITMethod(unsigned int pe_id);

    void WriteDisPort_IDMethod(unsigned int pe_id);
    void WriteERROVFOUT0_IDMethod(unsigned int pe_id);
    void WriteERROVFOUT1_IDMethod(unsigned int pe_id);
    void NegateERROVFOUT0_IDMethod(unsigned int pe_id);
    void NegateERROVFOUT1_IDMethod(unsigned int pe_id);
    void WriteSEDERROUT_IDMethod(unsigned int pe_id);
    void AssertSEDERROUT_IDMethod(unsigned int pe_id);
    void NegateSEDERROUT_IDMethod(unsigned int pe_id);
    void NegateDEDERROUT_IDMethod(unsigned int pe_id);
    void NegateAFEERROUT_IDMethod(unsigned int pe_id);
    void WriteDEDERROUT_IDMethod(unsigned int pe_id);
    void WriteAFEERROUT_IDMethod(unsigned int pe_id);
    void Process1bitError_IDMethod(unsigned int pe_id);
    void ProcessFatalError_IDMethod(unsigned int pe_id);
    void SetFatalErrorStatus_IDMethod(unsigned int pe_id);
    void NotifyFatalError_IDMethod(unsigned int pe_id);

    void Set1bitErrorStatusThread();

    // Functions
    void Initialize (void);
    void EnableReset (const std::string reset_name, const bool is_active);
    void CancelEvents();
    bool CheckClockPeriod (const std::string clock_name);

    bool CheckNoFatalStatus_IT(unsigned int pe_id);
    bool Check1bitAddressNotCaptured_IT(unsigned int pe_id, unsigned int mod_address);
    void Update1bitErrorList_IT(unsigned int pe_id);
    void UpdateFatalErrorList_IT(unsigned int pe_id);

    bool CheckNoFatalStatus_ID(unsigned int pe_id);
    bool Check1bitAddressNotCaptured_ID(unsigned int pe_id, unsigned int mod_address);
    void Update1bitErrorList_ID(unsigned int pe_id);
    void UpdateFatalErrorList_ID(unsigned int pe_id);

    std::string CombineStringUint(std::string name, unsigned int index);
    unsigned int ShiftAddress(std::string sub_name, unsigned int address);
    void DumpInfo(const char *type, const char *message, ...);
    std::string CalculateEDLCode();
    void CreateEDLMap();
    bool CheckAnyFIFOHasElement();

    // Overwrite firtual functions of ECC_ICACHE_E2_AgentController
    void WriteDisPort_IT(unsigned int pe_id, unsigned int value);
    void WriteDisPort_ID(unsigned int pe_id, unsigned int value);
    unsigned int GetCapDepth(std::string sub_name);
    void AssertERROVFOUT0_IT(unsigned int pe_id);
    void AssertERROVFOUT1_IT(unsigned int pe_id);
    void AssertERROVFOUT0_ID(unsigned int pe_id);
    void AssertERROVFOUT1_ID(unsigned int pe_id);
    bool GetKeyOpen();
    bool GetResetStatus (const std::string reset_name);

public:
    SC_HAS_PROCESS (ECC_ICACHE_E2);
    ECC_ICACHE_E2 (sc_module_name name,const uint8_t iPE_NUM, const unsigned int rLatency, const unsigned int wLatency);
    ~ECC_ICACHE_E2 ();

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
#endif //__ECC_ICACHE_E2_H__
