// ---------------------------------------------------------------------
// $Id: G4_PEG_DataHandler.h,v 1.8 2020/01/14 02:39:30 nhutminhdang Exp $
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

#ifndef __G4_PEG_DataHandler_H__
#define __G4_PEG_DataHandler_H__
#include "BusBridgeCoreBase.h"

class G4_PEG_AgentController;

class G4_PEG_DataHandler : public BusBridgeCoreBase<BUS_WIDTH_VCI,BUS_WIDTH_VCI>
{
public:
    G4_PEG_DataHandler(std::string name
                       ,G4_PEG_AgentController *G4_PEG_AgentControllerPtr
                       ,BusBridgeModuleBase<BUS_WIDTH_VCI,BUS_WIDTH_VCI> *Parent
                       ,BusBridgeSlaveIf<BUS_WIDTH_VCI> *SlaveIf
                       ,BusMasterIf<BUS_WIDTH_VCI> *MasterIf
                       ,SIM_MODE_T simmode
                       ,unsigned int CHANNEL_NUM);
    
    virtual ~G4_PEG_DataHandler();

    /// Variables
    sc_event mTransErrorEvent;                          // Event to notify to tranfer error
    
    /// Functions
    void EnableReset (const bool is_active);
    void SetCLKPeriod (const sc_dt::uint64 clk_period);
    
    unsigned int GetmADDR();
    unsigned int GetmSPID();
    unsigned int GetmBMID();
    bool GetmDBG();
    bool GetmUM();
    bool GetmSecure();
    bool GetmWRITE();
    bool IsSpcLd();

private:
    /// Enum
    enum eTransType{
        emREAD,
        emWRITE
    };
    
    /// Variables
    G4_PEG_AgentController    *mG4_PEG_AgentController;

    /// Overwrite virtual functions of BusBridgeCoreBase class
    void read(unsigned int offsetAddress,TlmBasicPayload& trans,BusTime_t* t,bool debug);
    void write(unsigned int offsetAddress,TlmBasicPayload& trans,BusTime_t* t,bool debug);

    /// Internal functions
    bool CheckTransAddress(const unsigned int trans_addr, unsigned int ch_id);
    bool AccessJudgment(TlmBasicPayload &trans, bool debug);
    bool ChannelJudgment(bool debug, int channel_id);
    void CancelEvents (void);
    void Initialize (void);
    
    /// Variables
    sc_dt::uint64  mPCLKPeriod;             // Period of PCLK
    unsigned int   mChannelNum;             // Number of channel
    bool           mRstState;               // Status of reset
    bool           bus_master_UM;           // User mode of bus master
    unsigned char  bus_master_SPID;         // SPID of bus master
    unsigned char  bus_master_PEID;         // PEID of bus master 
    bool           bus_master_SEC;         // Non-secure master mode of bus master 
    bool           mTrans_type;             // Issued transaction type (Read/ Write)
    bool           mIsSpcLd;                // Transaction is speculative/ un-speculative read
    
    /// Infomation of error
    unsigned int   mADDR;                   // Error address
    unsigned int   mSPID;                   // Error SPID
    unsigned int   mBMID;                   // Bus master ID
    bool           mDBG;                    // Debug mode
    bool           mUM;                     // User master mode
    bool           mWRITE;                  // Transaction type (is Write or not)
    bool           mSecure;                 // Non-secure master mode
    bool           mGEN;                    // Guard setting
    bool           mWG;                     // Master writable
    bool           mRG;                     // Master readable
};
#endif //__G4_PEG_DataHandler_H__
