// ---------------------------------------------------------------------
// $Id: G4_IPIR.h,v 1.7 2019/07/10 11:25:13 nhuvo Exp $
//
// Copyright(c) 2016-2019 Renesas Electronics Corporation
// Copyright(c) 2016-2019 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------
#ifndef __G4_IPIR_H__
#define __G4_IPIR_H__

#include "re_define.h"
#include "BusSlaveBase.h"
#include "G4_IPIR_AgentController.h"
#include "rvc_common_model.h"

class G4_IPIR_Func;

class G4_IPIR: public sc_module
            ,public G4_IPIR_AgentController
            ,public BusSlaveBase<BUS_WIDTH_VCI32,1>
            ,public rvc_common_model
{
public:
    // Ports
    sc_in<sc_dt::uint64>    clk;    // Clock
    sc_in<bool>             resetz; // Reset
    sc_in<bool>*            ipir_init_ch[INTC_IPIR_CH_NUM]; // Initial channel
    sc_in<bool>             ext_err;// External error
    
    sc_out<bool>*           ipir_int_pe_ch[PE_MAX_NUM][INTC_IPIR_CH_NUM];// Output interrupt

    G4_IPIR_Func            *mpG4_IPIRFunc; // Pointer of IPIR Func

    TlmTargetSocket<BUS_WIDTH_VCI32>    *ts;    // Target socket to access register
private:
    // Enums

    // Variables
    bool            mIsresetzInit;      // Is PRESET initialized
    sc_dt::uint64   mclkPeriod;         // Period of clk
    sc_dt::uint64   mclkOrgFreq;        // Previous frequency of clk
    sc_dt::uint64   mclkFreq;           // Frequency of clk
    std::string     mclkFreqUnit;       // Frequency unit of clk
    bool            mIsResetHardActive; // Is reset by port
    bool            mIsResetCmdReceive; // Is receive cmd reset
    bool            mIsResetCmdActive;  // Is reset by cmd
    double          mResetCmdPeriod;    // Period of cmd reset
    sc_dt::uint64   mTimeResolutionValue;   // Time resolution value of simulation
    sc_time_unit    mTimeResolutionUnit;    // Time resolution unit of simulation

    bool            maInterrupt[PE_MAX_NUM][INTC_IPIR_CH_NUM];// Store values of interrupts for writing output port
    bool            mDumpInterrupt; // Enable dump interrupt or not
    bool            mExtErr;        // Has External error or not
    bool            mEnableRegisterMessage;
    std::map<unsigned int, unsigned int>    mPEMap;     // Map to get PE num from BMID
    std::map<unsigned int, unsigned int>    mBMIDMap;   // Map to get BMID from PE num
    std::map<unsigned int, bool>            mActiveMap; // Map to get Active attribute from BMID

    // Events
    sc_event meResetCmd;        // Event to reset by command 
    sc_event meResetCmdCancel;  // Event to cancel reset by command

    sc_event meWriteInterrupt;  // Event to write output interrupts


    // Threads/Methods
    void InitChannelMethod();
    void ExtErrMethod();
    void WriteInterruptMethod();

    void clkMethod();
    void resetzMethod();
    void HandleResetCmdMethod();
    void CancelResetCmdMethod();

    // Functions
    void Initialize(void);
    void EnableReset(const bool is_active);
    bool get_active_attribute(unsigned int pe_num);
    void readCfgFile( const char *filename);
    void get_G4CPU_PE_INFO(const char *filename);
    void get_G4IPIR_MODE_BMID_PE(const char *filename);

    // Overwrite firtual functions of G4_IPIR_AgentController
    void DriveInterrupt(bool int_array[PE_MAX_NUM][INTC_IPIR_CH_NUM]);
    bool GetExtErr();
    bool GetPeNum(unsigned int bmid, unsigned int& pe_num);
    sc_dt::uint64 GetClockPeriod();
    bool GetResetStatus();
public:
    SC_HAS_PROCESS(G4_IPIR);
    G4_IPIR(sc_module_name name, const char* pCfgFileName, const unsigned int rLatency, const unsigned int wLatency);
    ~G4_IPIR();

    // PythonIF
    void SetMessageLevel (const std::string msg_lv);
    void DumpRegisterRW (const std::string is_enable);
    void DumpInterrupt (const std::string is_enable);
    void AssertReset (const std::string reset_name, const double start_time, const double period);
    void SetCLKFreq (const std::string clock_name, const sc_dt::uint64  freq, const std::string unit);
    void GetCLKFreq (const std::string clock_name);
    void ForceRegister (const std::string reg_name, const unsigned int reg_value);
    void ReleaseRegister (const std::string reg_name);
    void WriteRegister (const std::string reg_name, const unsigned int reg_value);
    void ReadRegister (const std::string reg_name);
    void ListRegister (void);
    void Help (const std::string type);
    void EnableRegisterMessage(const std::string is_enable);
};
#endif //__G4_IPIR_H__
