// ---------------------------------------------------------------------
// $Id: SPIDCTL.h,v 1.3 2021/11/19 11:19:37 minhquocha Exp $
//
// Copyright(c) 2020-2021 Renesas Electronics Corporation
// Copyright(c) 2020-2021 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------
// Description: Copyright of SPIDCTL.h
// Ref: {SPIDCTL_UD_Copyright_001, SPIDCTL_UD_ATT1_010}
#ifndef __SPIDCTL_H__
#define __SPIDCTL_H__
#include "global.h"
#include "BusSlaveBase.h"
#include "SPIDCTL_AgentController.h"
#include <vector>


#define ZERO_SPIDMASK       0x0   // Zero SPID mask: not allow any SPID number

class SPIDCTL_Func;

// Description: Declaration of SPIDCTL class
// Ref: {SPIDCTL_UD_ATT1_004, SPIDCTL_UD_ReferenceDoc_001, SPIDCTL_UD_BlockDiagram_001, SPIDCTL_UD_Ports_001, SPIDCTL_UD_Ports_002, SPIDCTL_UD_Ports_003, SPIDCTL_UD_Ports_004}
class SPIDCTL : public sc_module
    , public SPIDCTL_AgentController
    , public BusSlaveBase<BUS_WIDTH_APB, 1>
{
public:
    // Clock/Reset
    sc_in<sc_dt::uint64>    PCLK;                 //clock port
    sc_in<bool>             Reset;                //reset port

    sc_out<unsigned char>   *BMSPIDOUT[MAX_SPID];  //SPID out port

    // Socket
    TlmTargetSocket<BUS_WIDTH_APB>  *ts;          //target socket to access register

    // Class SPIDCTL_Func pointer
    SPIDCTL_Func            *mpSPIDCTLFunc;       //pointer of SPIDCTL_Func

    SC_HAS_PROCESS(SPIDCTL);
    //Constructor/Destructor
    SPIDCTL (sc_module_name name, const unsigned int rLatency, const unsigned int wLatency, const char *config_file);
    ~SPIDCTL();

    // Python IF
    void SetMessageLevel(const std::string msg_lv);
    void DumpRegisterRW(const std::string is_enable);
    void EnableRegisterMessage(const std::string is_enable);

    void DumpStatusInfo(void);
    void AssertReset(const std::string reset_name, const double start_time, const double period);
    void SetCLKFreq(const std::string clock_name, const sc_dt::uint64 freq, const std::string unit);
    void GetCLKFreq(const std::string clock_name);
    void ForceRegister(const std::string reg_name, const unsigned int reg_value);
    void ReleaseRegister(const std::string reg_name);
    void WriteRegister(const std::string reg_name, const unsigned int reg_value);
    void ReadRegister(const std::string reg_name);
    void ListRegister(void);
    void Help(const std::string type);

private:
    // Variables
    sc_dt::uint64   mPCLKPeriod;                    //Period of PCLK
    sc_dt::uint64   mPCLKOrgFreq;                   //Previous frequency of PCLK
    sc_dt::uint64   mPCLKFreq;                      //Frequency of PCLK
    std::string     mPCLKFreqUnit;                  //Frequency unit of PCLK
    double          mStartClkTime;                  // Start time of clock

    sc_dt::uint64   mTimeResolutionValue;           //Time resolution value of simulation
    sc_time_unit    mTimeResolutionUnit;            //Time resolution unit of simulation

    bool            mResetCurVal;                   //Store current value of reset signals
    bool            mResetPreVal;                   //Store previous value of reset signals
    bool            mIsResetHardActive;             //Is reset by port
    bool            mIsResetCmdReceive;             //Is receive cmd reset
    bool            mIsResetCmdActive;              //Is reset by cmd
    double          mResetCmdPeriod;                //Period of cmd reset
    bool            mEnableRegisterMessage;         //Enable/disable info/warning/error message of register IF

    bool            mIsSPIDMaskLock[MAX_SPID];
    bool            mKeyCode;

    unsigned char   mBMSPIDOUTInitVal[MAX_SPID];    //Initial values for BMSPIDOUT ports

    // Events
    sc_event        mResetHardEvent;                //Call HandleResetHardMethod when reset is active
    sc_event        mResetCmdEvent;                 //Call HandleResetCmdMethod when AssertReset is called
    sc_event        mResetCmdCancelEvent;           //Event to cancel reset by command
    sc_event        mEventUpdateSPID[MAX_SPID];     //Event SPID value/SPID value mask is updated into SPID_Reg/SPIDMSK_Reg

    // Threads/Methods
    void HandlePCLKSignalMethod(void);
    void HandleResetSignalMethod(void);
    void HandleSPIDCTLMethod(unsigned int spid_id);

    // Reset handler
    void HandleResetHardMethod(void);
    void HandleResetCmdMethod(void);
    void CancelResetCmdMethod(void);

    // Functions
    void Initialize(void);
    void EnableReset(const bool is_active);
    void CancelEvents(void);

    // Overwrite functions of Agent Controller class
    bool GetResetStatus(void);
    bool CheckClockPeriod(void);
    void NotifyUpdateSPIDValue(unsigned int Index);
    bool IsKeyCodeEnable(void);
    void SetKeyCodeEnable(bool isEnable);
    bool IsMaskLocked(unsigned int RegIndex);
    void SetMaskLocked(unsigned int RegIndex, bool isLocked);

    //Handle config file
    bool GetArrCfgValue(std::string s, unsigned int *arr);
    void ReadConfigFile(const char *config_file);
    std::string  GetStrBetweenTwoStr(const std::string &s, const std::string &start_delim, const std::string &stop_delim);

};
#endif // __SPIDCTL_H__
