// ---------------------------------------------------------------------
// $Id: G4_PEG.h,v 1.9 2020/01/14 02:39:30 nhutminhdang Exp $
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

#ifndef __G4_PEG_H__
#define __G4_PEG_H__

#include "BusBridgeModuleBase.h"
#include "BusBridgeSlaveBase.h"
#include "BusMasterBase.h"
#include "BusSlaveBase.h"
#include "G4_PEG_AgentController.h"
#include "G4_PEG_ERR_INFO.h"
#include "rvc_common_model.h"

template<unsigned int BUSWIDTH, unsigned int S_NUM> class BusBridgeSlaveBase;
template<unsigned int BUSWIDTH, unsigned int S_NUM> class BusMasterBase;
template<unsigned int BUSWIDTH> class TlmTargetSocket;
template<unsigned int BUSWIDTH> class TlmInitiatorSocket;

class G4_PEG_Func;
class G4_PEG_DataHandler;

class G4_PEG:  public G4_PEG_AgentController,
               public rvc_common_model,
               public BusBridgeModuleBase<BUS_WIDTH_VCI,BUS_WIDTH_VCI>,
               public BusBridgeSlaveBase<BUS_WIDTH_VCI,1>,
               public BusMasterBase<BUS_WIDTH_VCI,1>,
               public BusSlaveBase<BUS_WIDTH_APB,1>
{       
public:
    SC_HAS_PROCESS(G4_PEG);
    
    /// Constructor & Destructor
    G4_PEG(sc_module_name name, std::string kind, unsigned int rLatency, unsigned int wLatency, unsigned int PEG_ID = 0, unsigned int CHANNEL_NUM = 4);
    ~G4_PEG(void);
    
    /// Clock/ Reset
    sc_in<sc_dt::uint64>    PCLK;
    
    /// Input/ Output ports
    sc_out<G4_PEG_Cerrinfo> PEG_ERR_S;
    sc_out<G4_PEG_Cerrinfo> PEG_ERR_M;
    
    /// Socket declaration
    TlmTargetSocket<BUS_WIDTH_APB>     *tsp;
    TlmTargetSocket<BUS_WIDTH_VCI>     *tsv;
    TlmInitiatorSocket<BUS_WIDTH_VCI>  *is;
    
    /// Functions
    void EnableReset (const bool is_active);
    void CancelEvents (void);
    void SetPeId (unsigned int peid);
    void RegisterHandler (const std::vector<std::string> cmd);
    void DumpInterrupt (const bool is_enable);
    
    /// Overwite virtual function of G4_PEG_AgentController
    unsigned int getPEGSPID_SPID(unsigned intchannel_id);
    bool getPEGROT_GEN(unsigned intchannel_id);
    bool getPEGROT_DBG(unsigned intchannel_id);
    bool getPEGROT_UM(unsigned intchannel_id);
    bool getPEGROT_WG(unsigned intchannel_id);
    bool getPEGROT_RG(unsigned intchannel_id);
    unsigned int getPEGBAD_BAD(unsigned intchannel_id);
    unsigned int getPEGADV_ADV(unsigned intchannel_id);
    
    void SetCLKPeriod (const sc_dt::uint64 clk_period, const double time_point);
    void SetResetStatus(bool mIsResetCmdActive, bool mIsResetHardActive);

private:
    /// Variables
    G4_PEG_Func        *mG4_PEG_Func;              // G4_PEG_Func class instance
    G4_PEG_DataHandler *mPEG_DataHandler;          // G4_PEG_DataHandler class instance
    
    /// Events
    sc_event           mWritePEG_ERR_SEvent;       // Trigger WritePEG_ERR_SMethod
    sc_event           mWritePEG_ERR_MEvent;       // Trigger WritePEG_ERR_MMethod
    sc_event           mNegatePEG_ERR_SEvent;      // Trigger NegatePEG_ERR_SMethod
    sc_event           mNegatePEG_ERR_MEvent;      // Trigger NegatePEG_ERR_MMethod
                       
    /// Variables      
    sc_dt::uint64      mTimeResolutionValue;       // Time resolution value of simulation
    sc_time_unit       mTimeResolutionUnit;        // Time resolution unit of simulation
    sc_dt::uint64      mPCLKPeriod;                // Period value of clock
    double             mStartPCLKTime;             // Start time of PCLK
    bool               mIsResetHardActive;         // Is reset by port
    bool               mIsResetCmdActive;          // Is reset by cmd

    G4_PEG_Cerrinfo    mPEG_ERR_S;                 // Value of PEG_ERR_S port
    G4_PEG_Cerrinfo    mPEG_ERR_M;                 // Value of PEG_ERR_M port
    bool               mPEG_ERR_S_asserted;        // State of PEG_ERR_S port 
    bool               mPEG_ERR_M_asserted;        // State of PEG_ERR_M port 
    unsigned int       mChannelNum;                // Number of channel
    unsigned int       mPEID;                      // PE ID
    bool               mRstState;                  // Reset State
    bool               mDumpInterrupt;             // Enable/Disable dumping the interrupt info
    std::string        mKind;                      // Kind of platform

    /// Methods
    void HandleErrorMethod (void);
    void WritePEG_ERR_SMethod (void);
    void WritePEG_ERR_MMethod (void);
    void NegatePEG_ERR_SMethod (void);
    void NegatePEG_ERR_MMethod (void);
    
    /// Internal functions
    void Initialize(void);
    bool CheckClockPeriod ();
    bool GetResetStatus();
};
#endif //__G4_PEG_H__
