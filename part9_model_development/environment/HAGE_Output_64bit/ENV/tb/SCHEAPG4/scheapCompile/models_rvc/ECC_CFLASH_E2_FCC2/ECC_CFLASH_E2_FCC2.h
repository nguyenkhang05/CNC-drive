// ---------------------------------------------------------------------
// $Id: ECC_CFLASH_E2_FCC2.h,v 1.10 2019/09/06 08:33:53 landang Exp $
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
#ifndef __ECC_CFLASH_E2_FCC2_H__
#define __ECC_CFLASH_E2_FCC2_H__

#include "rvc_common_model.h"
#include "global.h"
#include "re_define.h"
#include "BusSlaveBase.h"
#include "ECC_CFLASH_E2_FCC2_AgentController.h"
#include "ECC_def.h"

#define DEFAULT_AWIDTH_MSB 27   // update from 25 to 27.
#define DEFAULT_AWIDTH_LSB 4    // update from 2 to 4.
#define DEFAULT_CAP_DEPTH 4
#define DEFAULT_ERRINT_INIT 0x87
#define HIGHEST_PORT_INDEX 0x10 // of SAXI2FAXI: slave port 16


class ECCCNT_CFP_PEn_FCC2_Func;
class ECCCNT_CFS_FCC2_Func;
class ECCCNT_CFC_FCC2_Func;
class MECCCAP_CFL_FCC2_Func;


class ECC_CFLASH_E2_FCC2: public sc_module
            ,public ECC_CFLASH_E2_FCC2_AgentController
            ,public BusSlaveBase<32, PE_MAX_NUM + CL_MAX_NUM + 2> // PE_NUM socket for CFP, 1 for CFS, CFC_NUM for CFC, 1 for CFL (MECCCAP)
            ,public rvc_common_model
{
private:
    uint8_t PE_NUM;
    uint8_t CFC_NUM;
public:
    // Clock/Reset
    sc_in<sc_dt::uint64>    pclk;           // clock
    sc_in<bool>             presetn;        // reset
    sc_in<sc_dt::uint64>    sys_clk;        // sys clock
    sc_in<bool>             sys_resetz;     // sys reset

    // ECCCNT CFP
    sc_out<bool>            **eccdisable_cfp;// detect/correct prohibition
    sc_out<bool>            **secdisable_cfp;// 1bit correct prohibition

    // ECCCNT CFS
    sc_out<bool>            eccdisable_cfs; // detect/correct prohibition
    sc_out<bool>            secdisable_cfs; // 1bit correct prohibition


    // ECCCNT CFC
    sc_out<bool>            **apedisable_cfc;     // address parity error prohibition
    sc_out<bool>            **blankerrormaskenable_cfc; // blank error maskable
    sc_out<bool>            **eccdisable_cfc; // detect/correct prohibition
    sc_out<bool>            **secdisable_cfc; // 1bit correct prohibition


    // MECCCAP_CFL
    sc_out<bool>            errovfout1_CF;  // FIFO overflow
    sc_out<bool>            errovfout0_CF;  // Address register buffer overflow
    sc_out<bool>            apeerrout_CF;   // Address parity error
    sc_out<bool>            dederrout_CF;   // Fatal error
    sc_out<bool>            sederrout_CF;   // 1bit error

    // Key open
    sc_in<bool>             keyopen;        // keyopen to write protected registers

    TlmTargetSocket<32>     *ts_ecccnt_cfp[PE_MAX_NUM]; // target sockets 
    TlmTargetSocket<32>     *ts_ecccnt_cfs; // target socket
    TlmTargetSocket<32>     *ts_ecccnt_cfc[CL_MAX_NUM]; // target socket
    TlmTargetSocket<32>     *ts_mecccap_cf; // target socket

    ECCCNT_CFP_PEn_FCC2_Func     **mpECCCNT_CFPFunc;   // Pointer of ECCCNT_CFP_PEn Func
    ECCCNT_CFS_FCC2_Func         *mpECCCNT_CFSFunc;  // Pointer of ECCCNT_CFS Func
    ECCCNT_CFC_FCC2_Func         **mpECCCNT_CFCFunc;  // Pointer of ECCCNT_CFC Func
    MECCCAP_CFL_FCC2_Func        *mpMECCCAP_CFLFunc; // Pointer of MECCCAP_CFL Func

private:
    // Enums
    enum eTgtInfo{
        emECCCNT_CFSTgtId   = PE_MAX_NUM
        ,emECCCNT_CFCTgtId  = PE_MAX_NUM + 1
        ,emMECCCAP_CFTgtId  = PE_MAX_NUM + CL_MAX_NUM + 1
        ,emNumTgt           = PE_MAX_NUM + CL_MAX_NUM + 2
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

    enum eCFLASHInfo{
        emTotalSlavePorts= PE_MAX_NUM*2 + 1 // Total slave ports
        ,emNumMaskedArea = 2            // Number of masked area in model
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

    bool            mECCDIS_CFP[PE_MAX_NUM];    // Store value of eccdisable_cfp
    bool            mSECDIS_CFP[PE_MAX_NUM];    // Store value of secdisable_cfp
    bool            mBLANKMASKENABLE_CFC[CL_MAX_NUM]; // Store value of blankerrormaskenable_cfc
    bool            mAPEDIS_CFC[CL_MAX_NUM];    // Store value of apedisable_cfc
    bool            mECCDIS_CFC[CL_MAX_NUM];    // Store value of eccdisable_cfc
    bool            mSECDIS_CFC[CL_MAX_NUM];    // Store value of secdisable_cfc
    bool            mECCDIS_CFS;    // Store value of eccdisable_cfs
    bool            mSECDIS_CFS;    // Store value of secdisable_cfs

    bool            mKeyOpen;       // Store value of keycode port
    bool            mERROVFOUT0_CF; // Store value of errovfout0_CF
    bool            mERROVFOUT1_CF; // Store value of errovfout1_CF
    bool            mSEDERROUT_CF;  // Store value of sederrout_CF
    bool            mDEDERROUT_CF;  // Store value of dederrout_CF
    bool            mAPEERROUT_CF;  // Store value of apeerrout_CF

    bool            mHas2bitError;      // Indicate has 2bit error in highest priority slave port
    bool            mHasAddrParityError;// Indicate has address parity error in highest priority slave port
    unsigned int    mFatalErrorModAddr; // Store fatal address needed to be captured
    unsigned int    mFatalEDLCode;      // Store EDL code needed to be captured

    bool            mIsEDLConfig;   // Indicate EDL is configured from Python IF or not
    bool            mIsAddrConfig;  // Indicate Error Address is configured from Python IF or not
    eErrorType      mErrorType;     // Error type
    unsigned int    mErrorEDL;      // Error EDL
    unsigned int    mErrorAddress;  // Error address
    std::string     mStrEDL;        // String of EDL (used in info message)

    ErrorInfo       m1bitErrorList[emTotalSlavePorts];  // List of 1bit error input
    ErrorInfo       mFatalErrorList[emTotalSlavePorts*2]; // List of fatal error input
    unsigned int    mSize1bitErrorList;                 // Indicate number of error in 1bit error list
    unsigned int    mSizeFatalErrorList;                // Indicate number of error in fatal error list
    std::queue<ErrorInfo>               m1bitErrorFIFO; // 1bit Error input FIFO
    std::map<std::string, unsigned int> mEDLmap;        // EDL map in CFLASH

    // Events
    sc_event mClkZeroEvent[emNumClk];           // Notify clock is zero

    sc_event mResetHardEvent[emNumReset];       // Call HandleResetHardMethod when reset is active
    sc_event mResetCmdEvent[emNumReset];        // Call HandleResetCmdMethod when AssertReset is called
    sc_event mResetCmdCancelEvent[emNumReset];  // Event to cancel reset by command

    sc_event mWriteDisPort_CFPEvent[PE_MAX_NUM];    // Trigger WriteDisPort_CFPMethod
    sc_event mWriteDisPort_CFSEvent;            // Trigger WriteDisPort_CFSMethod
    sc_event mWriteDisPort_CFCEvent[CL_MAX_NUM];            // Trigger WriteDisPort_CFCMethod
    sc_event mWriteERROVFOUT0_CFEvent;          // Trigger WriteERROVFOUT0_CFMethod
    sc_event mWriteERROVFOUT1_CFEvent;          // Trigger WriteERROVFOUT1_CFMethod
    sc_event mWriteSEDERROUT_CFEvent;           // Trigger WriteSEDERROUT_CFMethod
    sc_event mNegateERROVFOUT0_CFEvent;         // Trigger NegateERROVFOUT0_CFMethod
    sc_event mNegateERROVFOUT1_CFEvent;         // Trigger NegateERROVFOUT1_CFMethod
    sc_event mAssertSEDERROUT_CFEvent;          // Trigger AssertSEDERROUT_CFMethod
    sc_event mNegateSEDERROUT_CFEvent;          // Trigger NegateSEDRROUT_CFMethod
    sc_event mProcess1bitErrorEvent;            // Trigger Process1bitErrorMethod
    sc_event mProcessFatalErrorEvent;           // Trigger ProcessFatalErrorMethod
    sc_event mSetFatalErrorStatusEvent;         // Trigger SetFatalErrorStatusMethod
    sc_event mNotifyFatalErrorEvent;            // Trigger NotifyFatalErrorMethod
    sc_event mNegateDEDERROUT_CFEvent;          // Trigger NegateDEDERROUT_CFMethod
    sc_event mNegateAPEERROUT_CFEvent;          // Trigger NegateAPEERROUT_CFMethod
    sc_event mWriteDEDERROUT_CFEvent;           // Trigger WriteDEDERROUT_CFMethod
    sc_event mWriteAPEERROUT_CFEvent;           // Trigger WriteAPEERROUT_CFMethod
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
    void WriteDisPort_CFPMethod(unsigned int pe_id);
    void WriteDisPort_CFSMethod();
    void WriteDisPort_CFCMethod(unsigned int cfc_id);
    void WriteERROVFOUT0_CFMethod();
    void WriteERROVFOUT1_CFMethod();
    void WriteSEDERROUT_CFMethod();
    void AssertSEDERROUT_CFMethod();
    void NegateSEDRROUT_CFMethod();
    void NegateERROVFOUT0_CFMethod();
    void NegateERROVFOUT1_CFMethod();
    void Set1bitErrorStatusThread();
    void Process1bitErrorMethod();
    void ProcessFatalErrorMethod();
    void SetFatalErrorStatusMethod();
    void NotifyFatalErrorMethod();
    void NegateDEDERROUT_CFMethod();
    void NegateAPEERROUT_CFMethod();
    void WriteDEDERROUT_CFMethod();
    void WriteAPEERROUT_CFMethod();

    // Functions
    void Initialize (void);
    void EnableReset (const std::string reset_name, const bool is_active);
    void CancelEvents();
    bool CheckClockPeriod (const std::string clock_name);

    void CreateEDLMap();
    bool CheckNoFatalStatus();
    bool CheckMaskAddress( const unsigned int address);
    bool Check1bitAddressNotCaptured(unsigned int mod_address);
    void Update1bitErrorList();
    void UpdateFatalErrorList();
    std::string CombineStringUint(std::string name, unsigned int index);
    unsigned int ShiftAddress(unsigned int address);

    void AssertERROVFOUT0_CF();
    void AssertERROVFOUT1_CF();
    // Overwrite firtual functions of ECC_CFLASH_E2_FCC2_AgentController
    void WriteDisPort_CFP(unsigned int pe_id, unsigned int value);
    void WriteDisPort_CFS(unsigned int value);
    void WriteDisPort_CFC(unsigned int cfc_id, unsigned int value);
    unsigned int GetCapDepth();
    bool GetKeyOpen();
    bool GetResetStatus (const std::string reset_name);
    unsigned int GetAWIDTH_MSB();

public:
    SC_HAS_PROCESS (ECC_CFLASH_E2_FCC2);
    ECC_CFLASH_E2_FCC2 (sc_module_name name, const uint8_t iPE_NUM, const uint8_t iCFC_NUM, const unsigned int rLatency, const unsigned int wLatency);
    ~ECC_CFLASH_E2_FCC2 ();

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
#endif //__ECC_CFLASH_E2_FCC2_H__
