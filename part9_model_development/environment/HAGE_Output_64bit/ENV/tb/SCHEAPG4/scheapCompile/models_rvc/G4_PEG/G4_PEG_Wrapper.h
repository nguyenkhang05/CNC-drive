// ---------------------------------------------------------------------
// $Id: G4_PEG_Wrapper.h,v 1.8 2020/01/14 02:39:30 nhutminhdang Exp $
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

#ifndef __G4_PEG_WRAPPER_H__
#define __G4_PEG_WRAPPER_H__

#include <systemc.h>
#include <iostream>
#include <cstring>
#include <iterator>
#include <map>
#include <stdarg.h>
#include "re_define.h"
#include "tlm.h"      
#include "OSCI2.h"    
#include "global.h"
#include "G4_PEG_ERR_INFO.h"
#include "rvc_common_model.h"

class G4_PEG;
class G4_PEGCAP;

class G4_PEG_Wrapper: public sc_module, public rvc_common_model

{
#include "G4_PEG_Wrapper_cmdif.h"                                          
public:             
    /// Variables                          
    G4_PEG          *mG4_PEG;                   // G4_PEG class instance
    G4_PEGCAP       *mG4_PEGCAP_S;              // G4_PEGCAP class instance
    G4_PEGCAP       *mG4_PEGCAP_M;              // G4_PEGCAP class instance
    
    /// Constructor & Destructor
     SC_HAS_PROCESS(G4_PEG_Wrapper);
    G4_PEG_Wrapper(sc_module_name name, const char* configfile_name, unsigned int rLatency, unsigned int wLatency, unsigned int PEG_ID);
    ~G4_PEG_Wrapper(void);
    
    /// Clock/ Reset
    sc_in<sc_dt::uint64>        PCLK;
    sc_in<bool>                 presetz;
    
    /// Input/ Output ports
    sc_in<G4_PEG_Cerrinfo>      ERR_INFO;
    sc_out<G4_PEG_Cerrinfo>     PEG_ERR_M;
    sc_out<bool>                GRDERR_S;
    sc_out<bool>                GRDERR_M;
    
    /// Signals
    sc_signal<G4_PEG_Cerrinfo>  PEG_ERR_S_sig;
    
    /// Socket declaration
    TlmTargetSocket<BUS_WIDTH_APB> tsp;         // target socket access register block
    TlmTargetSocket<BUS_WIDTH_APB> ts_s;        // target socket access register block
    TlmTargetSocket<BUS_WIDTH_APB> ts_m;        // target socket access register block
                                               
    TlmTargetSocket<BUS_WIDTH_VCI> tsv;         // data target socket will be fowarded
    TlmInitiatorSocket<BUS_WIDTH_VCI> is;       // initiator socket
    
    /// PythonIF functions
    void SetMessageLevel (const std::string msg_lv);
    void DumpRegisterRW (const std::string is_enable);
    void DumpInterrupt (const std::string is_enable);
    void EnableRegisterMessage(const std::string is_enable);
    void AssertReset (const std::string reset_name, const double start_time, const double period);
    void SetCLKFreq (const std::string clock_name, const sc_dt::uint64 freq, const std::string unit);
    void GetCLKFreq (const std::string clock_name);
    void ForceRegister (const std::string module_id, const std::string reg_name, const unsigned int reg_value);
    void ReleaseRegister (const std::string module_id, const std::string reg_name);
    void WriteRegister (const std::string module_id, const std::string reg_name, const unsigned int reg_value);
    void ReadRegister (const std::string module_id, const std::string reg_name);
    void ListRegister (void);
    void Help (const std::string type);
    void SetPeId (unsigned int peid);
    
private:
    /// Clock/Reset
    bool            mIspresetzInit;             // Is presetz initialized
    sc_dt::uint64   mPCLKPeriod;                // Period value of clock
    sc_dt::uint64   mPCLKFreq;                  // Frequency value of clock
    sc_dt::uint64   mPCLKOrgFreq;               // Original frequency of clock
    std::string     mPCLKFreqUnit;              // Frequency unit of clock
                                                   
    /// Variables                                   
    sc_dt::uint64   mTimeResolutionValue;       // Time resolution value of simulation
    sc_time_unit    mTimeResolutionUnit;        // Time resolution unit of simulation
                                                   
    bool            mIsResetCmdReceive;         // Receive status of AssertReset command of reset signals
    bool            mIsResetCmdActive;          // Active status of AssertReset command of reset signals
    bool            mIsResetHardActive;         // Is reset by port
    double          mResetCmdPeriod;            // Period of AssertReset command of reset signals    
    
    bool            mDumpInterrupt;             // Enable/Disable dumping the interrupt info
    bool            mEnableRegisterMessage;     // Enable/disable info/warning/error message of register IF
    std::string     mKind;                      // Kind of platform
    unsigned int    mPEID;                      // ID of PE
    unsigned int    mChannelNum;                // Number of channel
     G4_PEG_Cerrinfo mPEG_ERR_M;
                                                   
    /// Events                                     
    sc_event mResetCmdEvent;                    // Call HandleResetCmdMethod when AssertReset is called
    sc_event mResetCmdCancelEvent;              // Call CancelResetCmdMethod when AssertReset is over
      
    /// Methods
    void MonitoPCLKMethod (void);
    void MonitorpresetzMethod (void);
    void HandleResetCmdMethod (void);
    void CancelResetCmdMethod (void);

    /// Functions
    void EnableReset (const bool is_active);
    void RegisterHandler (const std::string module_id, const std::vector<std::string> cmd);
    void PCLKUpdate (void);
    void UpdateResetStatus(void);
};
#endif //__G4_PEG_WRAPPER_H__
