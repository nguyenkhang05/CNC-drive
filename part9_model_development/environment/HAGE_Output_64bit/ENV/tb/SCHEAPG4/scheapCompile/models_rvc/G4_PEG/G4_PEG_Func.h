// ---------------------------------------------------------------------
// $Id: G4_PEG_Func.h,v 1.6 2020/01/14 02:39:30 nhutminhdang Exp $
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
#ifndef __G4_PEG_Func_H__
#define __G4_PEG_Func_H__

#include "global.h"
#include "BusSlaveFuncIf.h"
#include "G4_PEG_AgentController.h"

class G4_PEG_E2x_Reg;
class G4_PEG_U2A_Reg;

class G4_PEG_Func: public BusSlaveFuncIf
{
public:
    /// Constructor & Destructor
    G4_PEG_Func (std::string name, std::string kind, G4_PEG_AgentController *G4_PEG_AgentControllerPtr,unsigned int PEG_ID);
    virtual ~G4_PEG_Func (void);
    
    /// Functions
    void EnableReset (const bool is_active);
    void RegisterHandler (const std::vector<std::string>& args);
    void SetPeId (unsigned int peid);
    
    /// Overwite virtual functions of G4_PEG_AgentController class
    unsigned int getPEGSPID_SPID (unsigned int channel_id);
    bool getPEGROT_GEN (unsigned int channel_id);
    bool getPEGROT_DBG (unsigned int channel_id);
    bool getPEGROT_UM(unsigned int  channel_id);
    bool getPEGROT_WG (unsigned int channel_id);
    bool getPEGROT_RG (unsigned int channel_id);
    unsigned int getPEGBAD_BAD (unsigned int channel_id);
    unsigned int getPEGADV_ADV (unsigned int channel_id);
    
    /// Overwite virtual functions of BusSlaveFuncIf class
    void read (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug);
    void write (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug);
    
private:
    /// Variables
    G4_PEG_AgentController *mG4_PEG_AgentController;   // Agentcontroller pointer
    G4_PEG_U2A_Reg         *mpG4_PEG_U2A_RegBlock;     // Block to process related to register of G4_PEG of U2A platform
    G4_PEG_E2x_Reg         *mpG4_PEG_E2x_RegBlock;     // Block to process related to register of G4_PEG of E2x platform
    
    unsigned int mPEID;                                // PEG ID
    std::string mModuleName;                           // Module name
    std::string mKind;                                 // Kind of platform
};
#endif //__G4_PEG_Func_H__
