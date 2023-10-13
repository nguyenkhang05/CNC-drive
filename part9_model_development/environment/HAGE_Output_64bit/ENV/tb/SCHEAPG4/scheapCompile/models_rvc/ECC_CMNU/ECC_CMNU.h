// ---------------------------------------------------------------------
// $Id: ECC_CMNU.h,v 1.4 2015/11/06 02:01:36 ngankimtran2 Exp $
//
// Copyright(c) 2014 - 2015 Renesas Electronics Corporation
// Copyright(c) 2014 - 2015 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------
#ifndef __ECC_CMNU_H__
#define __ECC_CMNU_H__
#include "BusSlaveBase.h"
#include "ECC_CMNU_AgentController.h"

class ECC_CMNU_Func;
class ECC_CMNU: public sc_module,
              public ECC_CMNU_AgentController,
              public BusSlaveBase<32,1>
{
private:
    enum eResetActiveLevel {
        emResetActive               = 0,
        emResetSignalNum            = 2
    };//Define active level of reset signal
    enum eBankNo {
        emBank1                     = 1,    //for CFB_G3KH, TCM_G3KH, CFB_X2X, ICDM_G3MH, ICTM_G3MH versions
        emBank2                     = 2,    //for CFB_G3MH, GRAM_G3MH, TCM_G3MH versions
        emBankNo                    = 4     //for G3M_V30 version
    };//Define bank memory number
    enum eFifo {
        emFifoDepth                 = 4
    };//Define FIFO depth
    enum eErrorType {
        emAddrECC2BitError          = 0,
        emAddrECC1BitError          = 1,
        emDataECC2BitError          = 2,
        emDataECC1BitError          = 3,
        emAddrParityErrorRead       = 4,
        emAddrParityErrorWrite      = 5,
        emParityBitError            = 6,
        emDataECC2BitErrorExtension = 7,
        emDataECC1BitErrorExtension = 8,
        emErrorTypeNum              = 9
    };//Define error type
    enum eSignalId {
        emADEDERROUT                = 0,
        emASEDERROUT                = 1,
        emDEDERROUT                 = 2,
        emSEDERROUT                 = 3,
        emAPERROUT                  = 4,
        emBPERROUT                  = 5,
        emERROVFOUT                 = 6,
        emMaxSignalID               = 7
    };//Define signal id for error output

public:
    //Declare input signals
    sc_in<sc_dt::uint64> pclk;
    sc_in<sc_dt::uint64> sys_clk;
    sc_in<bool> presetz;
    sc_in<bool> sys_resetz;

    //Declare output signals
    sc_out<bool> aeccdisable;
    sc_out<bool> asecdisable;
    sc_out<bool> eccdisable;
    sc_out<bool> secdisable;

    sc_out<bool> adederrout;
    sc_out<bool> asederrout;
    sc_out<bool> dederrout;
    sc_out<bool> sederrout;
    sc_out<bool> aperrout;
    sc_out<bool> bperrout;
    sc_out<bool> errovfout;

    //Declare target socket
    TlmTargetSocket<32> *ts;

    SC_HAS_PROCESS (ECC_CMNU);

    ECC_CMNU (sc_module_name name, 
        unsigned int rLatency,
        unsigned int wLatency,
        unsigned int ECCCTLInitial = 0,
        unsigned int ERRINTInitial = 0,
        bool SupDupsed = false,
        unsigned int addr_awidth = 32,
        unsigned int addr_awidth_lsb = 0,
        std::string model_version = "G3M_V30");
    ~ECC_CMNU (void);

    //Declare public functions
    void SetMessageLevel (const std::string msg_lv);
    void DumpRegisterRW (const std::string is_enable);
    void AssertReset (const std::string reset_name, const double start_time, const double period);
    void SetCLKFreq (const std::string clock_name, const sc_dt::uint64  freq, const std::string unit);
    void GetCLKFreq (const std::string clock_name);
    void ForceRegister (const std::string reg_name, const unsigned int reg_value);
    void ReleaseRegister (const std::string reg_name);
    void WriteRegister (const std::string reg_name, const unsigned int reg_value);
    void ReadRegister (const std::string reg_name);
    void ListRegister (void);
    void Help (const std::string type);
    void UseFifo (const std::string is_use);
    void UseApbEcc (const std::string is_use);
    void IssueError (const unsigned int bank_no, const unsigned int error_type);
    void SetErrorAddr (const unsigned int bank_no, const unsigned int addr_value);

private:
    ECC_CMNU_Func       *mECCCMNUFunc;                          //ECC_CMNU_Func class instance

    //Used for clock setting
    sc_dt::uint64       mPCLKPeriod;                            //Period value of pclk clock
    sc_dt::uint64       mSYS_CLKPeriod;                         //Period value of sys_clk clock
    sc_dt::uint64       mPCLKFreq;                              //Frequency value of pclk clock (Hz)
    sc_dt::uint64       mPCLKOrgFreq;                           //Original frequency value of pclk clock (Hz)
    sc_dt::uint64       mSYS_CLKFreq;                           //Frequency value of sys_clk clock (Hz)
    sc_dt::uint64       mSYS_CLKOrgFreq;                        //Original frequency value of sys_clk clock (Hz)
    std::string         mPCLKFreqUnit;                          //Frequency unit of pclk clock
    std::string         mSYS_CLKFreqUnit;                       //Frequency unit of sys_clk clock
    sc_dt::uint64       mTimeResolutionValue;                   //Time resolution value of simulation
    sc_time_unit        mTimeResolutionUnit;                    //Time resolution unit of simulation

    //Used for reset handling
    bool                mResetCurVal[emResetSignalNum];         //Store current value of presetz or sys_resetz
    bool                mResetPreVal[emResetSignalNum];         //Store previous value of presetz or sys_resetz
    bool                mIsResetHardActive[emResetSignalNum];   //Reset operation status of presetz or sys_resetz signal
    bool                mIsResetCmdReceive[emResetSignalNum];   //Receive status of AssertReset command of presetz or sys_resetz
    bool                mIsResetCmdActive[emResetSignalNum];    //Active status of AssertReset command of presetz or sys_resetz
    double              mResetCmdPeriod[emResetSignalNum];      //Period of AssertReset command of presetz or sys_resetz

    //Used for parameters setting by Heap configure and Python IF
    std::string         mECCCMNUVersion;                        //ECC_CMNU model's version (G3M_V30, TCM_G3KH, CFB_G3KH)
    unsigned int        mAWIDTH;                                //The bit width of captured address
    unsigned int        mAWIDTH_LSB;                            //The low bit of captured address
    bool                mIsUseFifo;                             //Enable/disable FIFO using
    bool                mIsUseApbEcc;                           //Enable/Disable APB_ECC using
    //Used for error address/status capturing
    unsigned char       mReqId[emBankNo];                       //Request ID of FIFO
    unsigned char       mRespId[emBankNo];                      //Respond ID of FIFO
    unsigned int        mErrAddrFFIn[emBankNo][emFifoDepth];    //Error address input according to bank number - used FIFO
    unsigned int        mErrAddrIn[emBankNo];                   //Error address input according to bank number - unused FIFO
    unsigned int        mECCCTLOut;                             //ECC control output signals
    bool                mErrOut[emMaxSignalID];                 //The value of error output signals
    ErrorInfoStructure  mErrorInfo[emBankNo];                   //Error information structure according to bank number

    sc_event            mResetHardEvent[emResetSignalNum];      //Call HandleResetHardMethod when presetz or sys_reset is active
    sc_event            mResetCmdEvent[emResetSignalNum];       //Call HandleResetCmdMethod when AssertReset is called
    sc_event            mResetCmdCancelEvent[emResetSignalNum]; //Call CancelResetCmdMethod when AssertReset is over
    sc_event            mECCDisableSignalsControlEvent;         //Call ControlECCDisableSignalsMethod() at PCLK posedge
    sc_event            mErrInputEvent[emBankNo];               //Call CaptureErrorInputMethod when an error is detected
    sc_event            mErrAddrInputEvent[emBankNo];           //Call CaptureErrAddrInputMethod when error address is captured
    sc_event            mRisingErrOutEvent[emMaxSignalID];      //Call RisingErrOutSignalsMethod to control error output signal in pclk domain
    sc_event            mFallingErrOutEvent[emMaxSignalID];     //Call FallingErrOutSignalsMethod to control error output signal in pclk domain
    sc_event            mErrorOutputWriteEvent[emMaxSignalID];  //Call WriteErrOutSignalsMethod to control error output signal in pclk domain

    void Initialize (void);
    void InitializeOutputSignals (void);
    void CheckFreq (sc_dt::uint64 &freq_out, std::string &unit_out, sc_dt::uint64 freq_in, std::string unit_in);
    void GetTimeResolution (sc_dt::uint64 &resolution_value, sc_time_unit &resolution_unit);
    void SeparateString (std::vector<std::string> &vtr, const std::string msg);
    double CalculateCLKPosEdge (const std::string clock_name, const bool add_period, const double time_point);
    bool IsValidErrorBank (const unsigned int bank_no);
    bool IsValidErrorType (const unsigned int error_type);

    void EnableReset (const bool is_active, const unsigned int reset_id);

    void HandleResetCmdMethod (const unsigned int reset_id);
    void CancelResetCmdMethod (const unsigned int reset_id);
    void HandlePRESETZSignalMethod (void);
    void HandleSYS_RESETZSignalMethod (void);
    void HandleResetHardMethod (const unsigned int reset_id);
    void HandlePCLKSignalMethod (void);
    void HandleSYS_CLKSignalMethod (void);
    void WriteECCDisableSignalsMethod (void);
    void CaptureErrAddrInputMethod (const unsigned int bank_no);
    void CaptureErrInputMethod (const unsigned int bank_no);
    void RisingErrOutSignalsMethod (const unsigned int signal_id);
    void FallingErrOutSignalsMethod (const unsigned int signal_id);
    void WriteErrOutSignalsMethod (const unsigned int signal_id);

    //Declare virtual functions of ECC_CMNU_AgentController class
    bool CheckClockPeriod (const std::string clock_name);
    void ControlECCDisableSignals (void);
    void ControlErrorOutputSignals (const std::string signal_name);
};
#endif
