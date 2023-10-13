// ---------------------------------------------------------------------
// $Id: ECC_DMADTS_RAM_E2.h,v 1.4 2019/09/06 08:49:03 landang Exp $
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
#ifndef __ECC_DMADTS_RAM_E2_H__
#define __ECC_DMADTS_RAM_E2_H__

#include "re_define.h"
#include "BusSlaveBase.h"
#include "rvc_common_model.h"
#include "ECC_DMADTS_RAM_E2_AgentController.h"
#include "ECC_def.h"

#define DEFAULT_AWIDTH_MSB_DR 11
#define DEFAULT_AWIDTH_LSB_DR 2
#define DEFAULT_CAP_DEPTH_DR 1
#define DEFAULT_ERRINT_INIT_DR 0x8B
#define HIGHEST_PORT_INDEX_DR 0x0 // 1 slave port input only

#define DEFAULT_AWIDTH_MSB_DE 11
#define DEFAULT_AWIDTH_LSB_DE 2
#define DEFAULT_CAP_DEPTH_DE 1
#define DEFAULT_ERRINT_INIT_DE 0x83
#define HIGHEST_PORT_INDEX_DE 0x0 // 1 slave port input only

#define DEFAULT_AWIDTH_MSB_DA 10
#define DEFAULT_AWIDTH_LSB_DA 3
#define DEFAULT_CAP_DEPTH_DA 1
#define DEFAULT_ERRINT_INIT_DA 0x83
#define HIGHEST_PORT_INDEX_DA 0x0 // 1 slave port input only
#define nSubDMADTS 2


class ECCCNT_DTS_Func;
class ECCCNT_DMDEn_Func;
class ECCCNT_DMDAn_Func;
class MECCCAP_DTS_Func;
class MECCCAP_DMDEn_Func;
class MECCCAP_DMDAn_Func;

class ECC_DMADTS_RAM_E2: public sc_module
            ,public ECC_DMADTS_RAM_E2_AgentController
            ,public BusSlaveBase<32, nSubDMADTS*4 + 2> // 1 for ECCCNT DTS, nSubDMADTS for ECCCNT DMDEn, nSubDMADTS for ECCCNT DMDAn, same for MECCCAP blocks
            ,public rvc_common_model
{
public:
    // Clock/Reset
    sc_in<sc_dt::uint64>    pclk;           // clock
    sc_in<bool>             presetn;        // reset
    sc_in<sc_dt::uint64>    sys_clk;        // sys clock
    sc_in<bool>             sys_resetz;     // sys reset

    // ECCCNT DTS
    sc_out<bool>            eccdisable_dr;     // detect/correct prohibition
    sc_out<bool>            secdisable_dr;     // 1bit correct prohibition
    sc_out<bool>            afedisable_dr;     // address feedback error detection prohibition

    // ECCCNT DMDEn
    sc_out<bool>            *eccdisable_de[nSubDMADTS];        // detect/correct prohibition
    sc_out<bool>            *secdisable_de[nSubDMADTS];        // 1bit correct prohibition

    // ECCCNT DMDAn
    sc_out<bool>            *eccdisable_da[nSubDMADTS];        // detect/correct prohibition
    sc_out<bool>            *secdisable_da[nSubDMADTS];        // 1bit correct prohibition

    // MECCCAP DTS
    sc_out<bool>            errovfout1_DR;     // FIFO overflow
    sc_out<bool>            errovfout0_DR;     // Address register buffer overflow
    sc_out<bool>            afeerrout_DR;      // Address feedback error
    sc_out<bool>            dederrout_DR;      // 2bit error
    sc_out<bool>            sederrout_DR;      // 1bit error

    // MECCCAP DMDEn
    sc_out<bool>            *errovfout1_DE[nSubDMADTS];         // FIFO overflow
    sc_out<bool>            *errovfout0_DE[nSubDMADTS];         // Address register buffer overflow
    sc_out<bool>            *dederrout_DE[nSubDMADTS];          // 2bit error
    sc_out<bool>            *sederrout_DE[nSubDMADTS];          // 1bit error

    // MECCCAP DMDAn
    sc_out<bool>            *errovfout1_DA[nSubDMADTS];         // FIFO overflow
    sc_out<bool>            *errovfout0_DA[nSubDMADTS];         // Address register buffer overflow
    sc_out<bool>            *dederrout_DA[nSubDMADTS];          // 2bit error
    sc_out<bool>            *sederrout_DA[nSubDMADTS];          // 1bit error

    TlmTargetSocket<32>     *ts_ecccnt_dr;      // target sockets 
    TlmTargetSocket<32>     *ts_ecccnt_de[nSubDMADTS];      // target sockets 
    TlmTargetSocket<32>     *ts_ecccnt_da[nSubDMADTS];      // target sockets 
    TlmTargetSocket<32>     *ts_mecccap_dr;     // target socket
    TlmTargetSocket<32>     *ts_mecccap_de[nSubDMADTS];         // target socket
    TlmTargetSocket<32>     *ts_mecccap_da[nSubDMADTS];         // target socket

    ECCCNT_DTS_Func         *mpECCCNT_DTSFunc;  // Pointer of ECCCNT_DTS Func
    ECCCNT_DMDEn_Func       *mpECCCNT_DMDEFunc[nSubDMADTS]; // Pointer of ECCCNT_DMDEn Func
    ECCCNT_DMDAn_Func       *mpECCCNT_DMDAFunc[nSubDMADTS]; // Pointer of ECCCNT_DMDAn Func
    MECCCAP_DTS_Func        *mpMECCCAP_DTSFunc; // Pointer of MECCCAP_DTS Func
    MECCCAP_DMDEn_Func      *mpMECCCAP_DMDEFunc[nSubDMADTS];    // Pointer of MECCCAP_DMDEn Func
    MECCCAP_DMDAn_Func      *mpMECCCAP_DMDAFunc[nSubDMADTS];    // Pointer of MECCCAP_DMDAn Func

    // Key open
    sc_in<bool>             keyopen;        // keyopen to write protected registers

private:
    // Enums
    enum eTgtInfo{
        emNumTgt           = nSubDMADTS*4 + 2
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
        emTotalSlavePorts_DR   = 1  // Total slave ports of DR
        ,emTotalSlavePorts_DE  = 1  // Total slave ports of DE
        ,emTotalSlavePorts_DA  = 1  // Total slave ports of DA
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

    unsigned int    mAWIDTH_MSB_DR;    // Store AWIDTH_MSB parameter of DR
    unsigned int    mAWIDTH_LSB_DR;    // Store AWIDTH_LSB parameter of DR
    unsigned int    mCAP_DEPTH_DR;     // Store CAP_DEPTH parameter of DR
    unsigned int    mERRINT_INIT_DR;   // Store ERRINT_INIT parameter of DR

    unsigned int    mAWIDTH_MSB_DE;    // Store AWIDTH_MSB parameter of DE
    unsigned int    mAWIDTH_LSB_DE;    // Store AWIDTH_LSB parameter of DE
    unsigned int    mCAP_DEPTH_DE;     // Store CAP_DEPTH parameter of DE
    unsigned int    mERRINT_INIT_DE;   // Store ERRINT_INIT parameter of DE
    
    unsigned int    mAWIDTH_MSB_DA;    // Store AWIDTH_MSB parameter of DA
    unsigned int    mAWIDTH_LSB_DA;    // Store AWIDTH_LSB parameter of DA
    unsigned int    mCAP_DEPTH_DA;     // Store CAP_DEPTH parameter of DA
    unsigned int    mERRINT_INIT_DA;   // Store ERRINT_INIT parameter of DA

    bool            mDumpInterrupt;     // Enable dump interrupt info or not

    bool            mEnableRegisterMessage; // Is enable regisyer message or not
    sc_dt::uint64   mTimeResolutionValue;   // Time resolution value of simulation
    sc_time_unit    mTimeResolutionUnit;    // Time resolution unit of simulation

    bool            mECCDIS_DR;    // Store value of eccdisable of DR
    bool            mSECDIS_DR;    // Store value of secdisable of DR
    bool            mAFEDIS_DR;    // Store value of afedisable of DR

    bool            mECCDIS_DE[nSubDMADTS];    // Store value of eccdisable of DE
    bool            mSECDIS_DE[nSubDMADTS];    // Store value of secdisable of DE

    bool            mECCDIS_DA[nSubDMADTS];    // Store value of eccdisable of DA
    bool            mSECDIS_DA[nSubDMADTS];    // Store value of secdisable of DA

    bool            mKeyOpen;               // Store value of keycode port

    bool            mERROVFOUT0_DR;    // Store value of errovfout0_DR
    bool            mERROVFOUT1_DR;    // Store value of errovfout1_DR
    bool            mSEDERROUT_DR;     // Store value of sederrout_DR
    bool            mDEDERROUT_DR;     // Store value of dederrout_DR
    bool            mAFEERROUT_DR;     // Store value of afeerrout_DR

    bool            mERROVFOUT0_DE[nSubDMADTS];// Store value of errovfout0_DE
    bool            mERROVFOUT1_DE[nSubDMADTS];// Store value of errovfout1_DE
    bool            mSEDERROUT_DE[nSubDMADTS]; // Store value of sederrout_DE
    bool            mDEDERROUT_DE[nSubDMADTS]; // Store value of dederrout_DE

    bool            mERROVFOUT0_DA[nSubDMADTS];// Store value of errovfout0_DA
    bool            mERROVFOUT1_DA[nSubDMADTS];// Store value of errovfout1_DA
    bool            mSEDERROUT_DA[nSubDMADTS]; // Store value of sederrout_DA
    bool            mDEDERROUT_DA[nSubDMADTS]; // Store value of dederrout_DA

    bool            mHas2bitError_DR;               // Indicate has 2bit error in highest priority slave port
    bool            mHasAddrFeedbackError_DR;       // Indicate has address feedback error in highest priority slave port
    unsigned int    mFatalErrorModAddr_DR;          // Fatal error address to capture
    unsigned int    mFatalEDLCode_DR;               // Fatal EDL to capture

    bool            mHas2bitError_DE[nSubDMADTS];       // Indicate has 2bit error in highest priority slave port
    unsigned int    mFatalErrorModAddr_DE[nSubDMADTS];  // Fatal error address to capture
    unsigned int    mFatalEDLCode_DE[nSubDMADTS];       // Fatal EDL to capture

    bool            mHas2bitError_DA[nSubDMADTS];             // Indicate has 2bit error in highest priority slave port
    unsigned int    mFatalErrorModAddr_DA[nSubDMADTS];  // Fatal error address to capture
    unsigned int    mFatalEDLCode_DA[nSubDMADTS];       // Fatal EDL to capture

    bool            mIsEDLConfig;       // Indicate EDL is configured from Python IF or not
    bool            mIsAddrConfig;      // Indicate Error Address is configured from Python IF or not
    eErrorType      mErrorType;         // Error type
    unsigned int    mErrorEDL;          // Error EDL
    unsigned int    mErrorAddress;      // Error address
    std::string     mCurConfigSubName;  // Indicate LR0/LR1 is now configured error info
    unsigned int    mSubId;              // Indicate current sub_id will receive error
    std::string     mStrEDL;            // Store input EDL in command Python

    ErrorInfo       m1bitErrorList_DR[emTotalSlavePorts_DR];    // List of 1bit error input of DR
    ErrorInfo       mFatalErrorList_DR[emTotalSlavePorts_DR*2];   // List of fatal error input of DR
    unsigned int    mSize1bitErrorList_DR;                      // Indicate number of error in 1bit error list
    unsigned int    mSizeFatalErrorList_DR;                     // Indicate number of error in fatal error list
    std::queue<ErrorInfo>               m1bitErrorFIFO_DR;      // 1bit Error input FIFO of each MECCCAP of DR
    std::map<std::string, unsigned int> mEDLmap_DR;             // EDL map in DR

    ErrorInfo       m1bitErrorList_DE[nSubDMADTS][emTotalSlavePorts_DE];  // List of 1bit error input of DE
    ErrorInfo       mFatalErrorList_DE[nSubDMADTS][emTotalSlavePorts_DE]; // List of fatal error input of DE
    unsigned int    mSize1bitErrorList_DE[nSubDMADTS];                     // Indicate number of error in 1bit error list
    unsigned int    mSizeFatalErrorList_DE[nSubDMADTS];                    // Indicate number of error in fatal error list
    std::queue<ErrorInfo>               m1bitErrorFIFO_DE[nSubDMADTS];     // 1bit Error input FIFO of each MECCCAP of DE
    std::map<std::string, unsigned int> mEDLmap_DE;                    // EDL map in DE

    ErrorInfo       m1bitErrorList_DA[nSubDMADTS][emTotalSlavePorts_DA];  // List of 1bit error input of DA
    ErrorInfo       mFatalErrorList_DA[nSubDMADTS][emTotalSlavePorts_DA]; // List of fatal error input of DA
    unsigned int    mSize1bitErrorList_DA[nSubDMADTS];                     // Indicate number of error in 1bit error list
    unsigned int    mSizeFatalErrorList_DA[nSubDMADTS];                    // Indicate number of error in fatal error list
    std::queue<ErrorInfo>               m1bitErrorFIFO_DA[nSubDMADTS];     // 1bit Error input FIFO of each MECCCAP of DA
    std::map<std::string, unsigned int> mEDLmap_DA;                    // EDL map in DA

    // Events
    sc_event mClkZeroEvent[emNumClk];           // Notify clock is zero

    sc_event mResetHardEvent[emNumReset];       // Call HandleResetHardMethod when reset is active
    sc_event mResetCmdEvent[emNumReset];        // Call HandleResetCmdMethod when AssertReset is called
    sc_event mResetCmdCancelEvent[emNumReset];  // Event to cancel reset by command

    sc_event mWriteDisPort_DREvent;             // Trigger WriteDisPort_DRMethod
    sc_event mWriteERROVFOUT0_DREvent;          // Trigger WriteERROVFOUT0_DRMethod
    sc_event mWriteERROVFOUT1_DREvent;          // Trigger WriteERROVFOUT1_DRMethod
    sc_event mNegateERROVFOUT0_DREvent;         // Trigger NegateERROVFOUT0_DRMethod
    sc_event mNegateERROVFOUT1_DREvent;         // Trigger NegateERROVFOUT1_DRMethod
    sc_event mWriteSEDERROUT_DREvent;           // Trigger WriteSEDERROUT_DRMethod
    sc_event mAssertSEDERROUT_DREvent;          // Trigger AssertSEDERROUT_DRMethod
    sc_event mNegateSEDERROUT_DREvent;          // Trigger NegateSEDERROUT_DRMethod
    sc_event mNegateDEDERROUT_DREvent;          // Trigger NegateDEDERROUT_DRMethod
    sc_event mNegateAFEERROUT_DREvent;          // Trigger NegateAFEERROUT_DRMethod
    sc_event mWriteDEDERROUT_DREvent;           // Trigger WriteDEDERROUT_DRMethod
    sc_event mWriteAFEERROUT_DREvent;           // Trigger WriteAFEERROUT_DRMethod
    sc_event mProcess1bitError_DREvent;         // Trigger Process1bitError_DRMethod
    sc_event mProcessFatalError_DREvent;        // Trigger ProcessFatalError_DRMethod
    sc_event mSetFatalErrorStatus_DREvent;      // Trigger SetFatalErrorStatus_DRMethod
    sc_event mNotifyFatalError_DREvent;         // Trigger NotifyFatalError_DRMethod

    sc_event mWriteDisPort_DEEvent[nSubDMADTS];     // Trigger WriteDisPort_DEMethod
    sc_event mWriteERROVFOUT0_DEEvent[nSubDMADTS];  // Trigger WriteERROVFOUT0_DEMethod
    sc_event mWriteERROVFOUT1_DEEvent[nSubDMADTS];  // Trigger WriteERROVFOUT1_DEMethod
    sc_event mNegateERROVFOUT0_DEEvent[nSubDMADTS]; // Trigger NegateERROVFOUT0_DEMethod
    sc_event mNegateERROVFOUT1_DEEvent[nSubDMADTS]; // Trigger NegateERROVFOUT1_DEMethod
    sc_event mWriteSEDERROUT_DEEvent[nSubDMADTS];   // Trigger WriteSEDERROUT_DEMethod
    sc_event mAssertSEDERROUT_DEEvent[nSubDMADTS];  // Trigger AssertSEDERROUT_DEMethod
    sc_event mNegateSEDERROUT_DEEvent[nSubDMADTS];  // Trigger NegateSEDERROUT_DEMethod
    sc_event mNegateDEDERROUT_DEEvent[nSubDMADTS];  // Trigger NegateDEDERROUT_DEMethod
    sc_event mWriteDEDERROUT_DEEvent[nSubDMADTS];   // Trigger WriteDEDERROUT_DEMethod
    sc_event mProcess1bitError_DEEvent[nSubDMADTS];    // Trigger Process1bitError_DEMethod
    sc_event mProcessFatalError_DEEvent[nSubDMADTS];   // Trigger ProcessFatalError_DEMethod
    sc_event mSetFatalErrorStatus_DEEvent[nSubDMADTS]; // Trigger SetFatalErrorStatus_DEMethod
    sc_event mNotifyFatalError_DEEvent[nSubDMADTS];    // Trigger NotifyFatalError_DEMethod

    sc_event mWriteDisPort_DAEvent[nSubDMADTS];     // Trigger WriteDisPort_DAMethod
    sc_event mWriteERROVFOUT0_DAEvent[nSubDMADTS];  // Trigger WriteERROVFOUT0_DAMethod
    sc_event mWriteERROVFOUT1_DAEvent[nSubDMADTS];  // Trigger WriteERROVFOUT1_DAMethod
    sc_event mNegateERROVFOUT0_DAEvent[nSubDMADTS]; // Trigger NegateERROVFOUT0_DAMethod
    sc_event mNegateERROVFOUT1_DAEvent[nSubDMADTS]; // Trigger NegateERROVFOUT1_DAMethod
    sc_event mWriteSEDERROUT_DAEvent[nSubDMADTS];   // Trigger WriteSEDERROUT_DAMethod
    sc_event mAssertSEDERROUT_DAEvent[nSubDMADTS];  // Trigger AssertSEDERROUT_DAMethod
    sc_event mNegateSEDERROUT_DAEvent[nSubDMADTS];  // Trigger NegateSEDERROUT_DAMethod
    sc_event mNegateDEDERROUT_DAEvent[nSubDMADTS];  // Trigger NegateDEDERROUT_DAMethod
    sc_event mWriteDEDERROUT_DAEvent[nSubDMADTS];   // Trigger WriteDEDERROUT_DAMethod
    sc_event mProcess1bitError_DAEvent[nSubDMADTS];    // Trigger Process1bitError_DAMethod
    sc_event mProcessFatalError_DAEvent[nSubDMADTS];   // Trigger ProcessFatalError_DAMethod
    sc_event mSetFatalErrorStatus_DAEvent[nSubDMADTS]; // Trigger SetFatalErrorStatus_DAMethod
    sc_event mNotifyFatalError_DAEvent[nSubDMADTS];    // Trigger NotifyFatalError_DAMethod

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

    void WriteDisPort_DRMethod();
    void WriteERROVFOUT0_DRMethod();
    void WriteERROVFOUT1_DRMethod();
    void NegateERROVFOUT0_DRMethod();
    void NegateERROVFOUT1_DRMethod();
    void WriteSEDERROUT_DRMethod();
    void AssertSEDERROUT_DRMethod();
    void NegateSEDERROUT_DRMethod();
    void NegateDEDERROUT_DRMethod();
    void NegateAFEERROUT_DRMethod();
    void WriteDEDERROUT_DRMethod();
    void WriteAFEERROUT_DRMethod();
    void Process1bitError_DRMethod();
    void ProcessFatalError_DRMethod();
    void SetFatalErrorStatus_DRMethod();
    void NotifyFatalError_DRMethod();

    void WriteDisPort_DEMethod(unsigned int sub_id);
    void WriteERROVFOUT0_DEMethod(unsigned int sub_id);
    void WriteERROVFOUT1_DEMethod(unsigned int sub_id);
    void NegateERROVFOUT0_DEMethod(unsigned int sub_id);
    void NegateERROVFOUT1_DEMethod(unsigned int sub_id);
    void WriteSEDERROUT_DEMethod(unsigned int sub_id);
    void AssertSEDERROUT_DEMethod(unsigned int sub_id);
    void NegateSEDERROUT_DEMethod(unsigned int sub_id);
    void NegateDEDERROUT_DEMethod(unsigned int sub_id);
    void WriteDEDERROUT_DEMethod(unsigned int sub_id);
    void Process1bitError_DEMethod(unsigned int sub_id);
    void ProcessFatalError_DEMethod(unsigned int sub_id);
    void SetFatalErrorStatus_DEMethod(unsigned int sub_id);
    void NotifyFatalError_DEMethod(unsigned int sub_id);

    void WriteDisPort_DAMethod(unsigned int sub_id);
    void WriteERROVFOUT0_DAMethod(unsigned int sub_id);
    void WriteERROVFOUT1_DAMethod(unsigned int sub_id);
    void NegateERROVFOUT0_DAMethod(unsigned int sub_id);
    void NegateERROVFOUT1_DAMethod(unsigned int sub_id);
    void WriteSEDERROUT_DAMethod(unsigned int sub_id);
    void AssertSEDERROUT_DAMethod(unsigned int sub_id);
    void NegateSEDERROUT_DAMethod(unsigned int sub_id);
    void NegateDEDERROUT_DAMethod(unsigned int sub_id);
    void WriteDEDERROUT_DAMethod(unsigned int sub_id);
    void Process1bitError_DAMethod(unsigned int sub_id);
    void ProcessFatalError_DAMethod(unsigned int sub_id);
    void SetFatalErrorStatus_DAMethod(unsigned int sub_id);
    void NotifyFatalError_DAMethod(unsigned int sub_id);

    void Set1bitErrorStatusThread();

    // Functions
    void Initialize (void);
    void EnableReset (const std::string reset_name, const bool is_active);
    void CancelEvents();
    bool CheckClockPeriod (const std::string clock_name);

    bool CheckNoFatalStatus_DR();
    bool Check1bitAddressNotCaptured_DR(unsigned int mod_address);
    void Update1bitErrorList_DR();
    void UpdateFatalErrorList_DR();

    bool CheckNoFatalStatus_DE(unsigned int sub_id);
    bool Check1bitAddressNotCaptured_DE(unsigned int sub_id, unsigned int mod_address);
    void Update1bitErrorList_DE(unsigned int sub_id);
    void UpdateFatalErrorList_DE(unsigned int sub_id);

    bool CheckNoFatalStatus_DA(unsigned int sub_id);
    bool Check1bitAddressNotCaptured_DA(unsigned int sub_id, unsigned int mod_address);
    void Update1bitErrorList_DA(unsigned int sub_id);
    void UpdateFatalErrorList_DA(unsigned int sub_id);

    std::string CombineStringUint(std::string name, unsigned int index);
    unsigned int ShiftAddress(std::string sub_name, unsigned int address);
    void CreateEDLMap();
    std::string CalculateEDLCode();
    bool CheckAnyFIFOHasElement();

    // Overwrite firtual functions of ECC_DMADTS_RAM_E2_AgentController
    void WriteDisPort_DR(unsigned int value);
    void WriteDisPort_DE(unsigned int sub_id, unsigned int value);
    void WriteDisPort_DA(unsigned int sub_id, unsigned int value);
    unsigned int GetCapDepth(std::string sub_name);
    void AssertERROVFOUT0_DR();
    void AssertERROVFOUT1_DR();
    void AssertERROVFOUT0_DE(unsigned int sub_id);
    void AssertERROVFOUT1_DE(unsigned int sub_id);
    void AssertERROVFOUT0_DA(unsigned int sub_id);
    void AssertERROVFOUT1_DA(unsigned int sub_id);
    bool GetKeyOpen();
    bool GetResetStatus (const std::string reset_name);

public:
    SC_HAS_PROCESS (ECC_DMADTS_RAM_E2);
    ECC_DMADTS_RAM_E2 (sc_module_name name, const unsigned int rLatency, const unsigned int wLatency);
    ~ECC_DMADTS_RAM_E2 ();

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
#endif //__ECC_DMADTS_RAM_E2_H__
