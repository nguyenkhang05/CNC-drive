// ---------------------------------------------------------------------
// $Id: HI20FSGD2_ERRSLV.h,v 1.1 2019/11/26 04:05:44 hadinhtran Exp $
//
// Copyright(c) 2019 Renesas Electronics Corporation
// Copyright(c) 2019 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------

#ifndef __HI20FSGD2_ERRSLV_H__
#define __HI20FSGD2_ERRSLV_H__

#include "global.h"
#include "rvc_common_model.h"
#include "BusSlaveBase.h"
#include "HI20FSGD2_ERRSLV_AgentController.h"
#include "HI20FSGD2_ERR_INFO.h"

class HI20FSGD2_ERRSLV_Func;

// Description: Declaration of HI20FSGD2_ERRSLV class
class HI20FSGD2_ERRSLV: public sc_module
            ,public HI20FSGD2_ERRSLV_AgentController
            ,public BusSlaveBase<BUS_WIDTH_APB, 1>
            ,public rvc_common_model
{
public:
    // Ports signal
    sc_in<sc_dt::uint64>                PCLK;                       // clock
    sc_in<bool>                         Reset;                      // reset
    sc_in<HI20FSGD2_Cerrinfo>           **PSLVERR;                  // Error Info
    sc_out<bool>                        KCPROT_KCE;                 // Key code bit
    sc_out<bool>                        PBG_ERR;                    // interrupt

    // Sockets
    TlmTargetSocket<BUS_WIDTH_APB>      *ts;                        // target sockets to access register

    // Class pointer
    HI20FSGD2_ERRSLV_Func                      *mpHI20FSGD2_ERRSLVFunc;             // Pointer of HI20FSGD2_ERRSLV_Func

    SC_HAS_PROCESS (HI20FSGD2_ERRSLV);
    // Constructor
    HI20FSGD2_ERRSLV (sc_module_name name, const unsigned int rLatency, const unsigned int wLatency, const unsigned int iGUARD_NUM);
    ~HI20FSGD2_ERRSLV ();

    // PythonIF
    void SetMessageLevel (const std::string msg_lv);
    void DumpRegisterRW (const std::string is_enable);
    void DumpInterrupt (const std::string is_enable);
    void EnableRegisterMessage(const std::string is_enable);

    void AssertReset (const std::string reset_name, const double start_time, const double period);
    void SetCLKFreq (const std::string clock_name, const sc_dt::uint64  freq, const std::string unit);
    void GetCLKFreq (const std::string clock_name);
    void ForceRegister (const std::string reg_name, const unsigned int reg_value);
    void ReleaseRegister (const std::string reg_name);
    void WriteRegister (const std::string reg_name, const unsigned int reg_value);
    void ReadRegister (const std::string reg_name);
    void ListRegister (void);
    void Help (const std::string type);

private:
    // Variables
    std::ostringstream                  port_name;                         // Port name of PSLVERR
    sc_dt::uint64                       mPCLKPeriod;                       // Period of PCLK
    sc_dt::uint64                       mPCLKOrgFreq;                      // Previous frequency of PCLK
    sc_dt::uint64                       mPCLKFreq;                         // Frequency of PCLK
    std::string                         mPCLKFreqUnit;                     // Frequency unit of PCLK
    double                              mStartPCLKTime;                    // Start time of PCLK
    sc_dt::uint64                       mPrePCLKPeriod;                    // Previous period of PCLK
                                                                        
    sc_dt::uint64                       mTimeResolutionValue;              // Time resolution value of simulation
    sc_time_unit                        mTimeResolutionUnit;               // Time resolution unit of simulation
                                                                        
    bool                                mResetCurVal;                      // Store current value of reset signals
    bool                                mResetPreVal;                      // Store previous value of reset signals
    bool                                mIsResetHardActive;                // Is reset by port
    bool                                mIsResetCmdReceive;                // Is receive cmd reset
    bool                                mIsResetCmdActive;                 // Is reset by cmd
    double                              mResetCmdPeriod;                   // Period of cmd reset
    bool                                mEnableRegisterMessage;            // Enable/disable info/warning/error message of register IF
    bool                                mDumpInterrupt;                    // Enable/Disable dumping the interrupt info
    unsigned int                        mErrorAddr;                        // Error address
    unsigned int                        mGuardNum;                         // Guard number
    bool                                mGuard_Error_value;                // Store value to write to INTECM0CFGERR port
                                                                        
    // Events                                                           
    sc_event                            mResetHardEvent;                   // Call HandleResetHardMethod when reset is active
    sc_event                            mResetCmdEvent;                    // Call HandleResetCmdMethod when AssertReset is called
    sc_event                            mResetCmdCancelEvent;              // Event to cancel reset by command
                                                                        
    sc_event                            mWriteGuardErrorEvent;             // Write port INTECM0CFGERR event
    sc_event                            mAssertGuardErrorEvent;            // Assert interrupt event
    sc_event                            mDeassertGuardErrorEvent;          // De-assert interrupt event
    sc_event                            mTransErrorEvent;                  // Transport error event

    // Threads/Methods
    void HandlePCLKSignalMethod(void);
    void HandleResetSignalMethod(void);

    void WriteGuardErrorMethod(void);
    void AssertGuardErrorMethod(void);
    void DeassertGuardErrorMethod(void);
    void HandleErrorMethod(const unsigned int guard_id);
    void NotifyTransErrorMethod(void);

    // Reset handler
    void HandleResetHardMethod(void);
    void HandleResetCmdMethod(void);
    void CancelResetCmdMethod(void);

    // Functions
    void Initialize(void);
    void EnableReset(const bool is_active);

    void CancelEvents(void);
    void DumpInterruptMsg (const std::string intr_name, const bool value);
    void SetKceBit(const bool value);

    // Overwrite functions of Agent Controller class
    bool CheckClockPeriod(void);
    bool GetResetStatus(void);

};
#endif //__HI20FSGD2_ERRSLV_H__
