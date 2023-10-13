// ---------------------------------------------------------------------
// $Id: G4_PEGCAP.h,v 1.9 2020/01/14 02:39:30 nhutminhdang Exp $
//
// Copyright(c) 2016-2020 Renesas Electronics Corporation
// Copyright(c) 2016-2020 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------

#ifndef __G4_PEGCAP_H__
#define __G4_PEGCAP_H__

#include "BusSlaveBase.h"
#include "G4_PEGCAP_AgentController.h"
#include "G4_PEG_ERR_INFO.h"
#include "global.h"
#include "rvc_common_model.h"

class G4_PEGCAP_Func;
class G4_PEGCAP:  public sc_module,
                  public rvc_common_model,
                  public G4_PEGCAP_AgentController,
                  public BusSlaveBase<BUS_WIDTH_APB,1>
{   
public:
    SC_HAS_PROCESS(G4_PEGCAP);
    
    /// Constructor & Destructor
    G4_PEGCAP(sc_module_name name, std::string kind, unsigned int rLatency, unsigned int wLatency);
    ~G4_PEGCAP(void);
    
    /// Clock/ Reset
    sc_in<sc_dt::uint64>        PCLK;       // Operating clock
    sc_in<bool>                 presetz;    // Reset
    
    /// Input/ Output ports
    sc_in<G4_PEG_Cerrinfo>      ERR_INFO;   // Infomation of error
    sc_out<bool>                GRDERR;     // Interrupt
    
    /// Socket declaration
    TlmTargetSocket<BUS_WIDTH_APB> *ts;
    
    /// Functions
    void DumpInterrupt (const bool is_enable);
    void RegisterHandler (const std::vector<std::string> cmd);
    void EnableReset (const bool is_active);
    void CancelEvents (void);
    void SetCLKPeriod (const sc_dt::uint64 clk_period, const double time_point);
    void SetResetStatus(bool mIsResetCmdActive, bool mIsResetHardActive);
    
    /// Methods
    void ClearPEGSPIDERRCLRMethod();
    
    /// Overwrite virtual functions of G4_PEGCAP_AgentController
    void NotifyClearPEGSPIDERRCLREvent();

private:
    /// Variables
    G4_PEGCAP_Func  *mG4_PEGCAP_Func;             // G4_PEGCAP_Func class instance
                                                  
    /// Events                                    
    sc_event        mWriteGRDERREvent;            // Trigger WriteGRDERRMethod
    sc_event        mNegateGRDERREvent;           // Trigger NegateGRDERRMethod
    sc_event        mClearPEGSPIDERRCLREvent;     // Trigger ClearPEGSPIDERRCLRMethod
    bool            mIsResetHardActive;           // Is reset by port
    bool            mIsResetCmdActive;            // Is reset by cmd
                                                  
    /// Variables                                 
    sc_dt::uint64   mTimeResolutionValue;         // Time resolution value of simulation
    sc_time_unit    mTimeResolutionUnit;          // Time resolution unit of simulation
    bool            mDumpInterrupt;               // Enable/Disable dumping the interrupt info
    sc_dt::uint64   mPCLKPeriod;                  // Period of PCLK
    double          mStartPCLKTime;               // Start time of PCLK
    bool            mRstState;                    // Current state is reseted or not
    bool            mGRDERR;                      // Value of GRDERR port
    std::string     mKind;                        // Kind of platform

    /// Methods
    void HandleERR_INFOMethod(void);
    void WriteGRDERRMethod();
    void NegateGRDERRMethod();
    
    /// Internal functions
    void Initialize(void);
    
    /// Overwrite virtual functions of G4_PEGCAP_AgentController
    bool CheckClockPeriod ();
    bool GetResetStatus();
};
#endif //__G4_PEGCAP_H__
