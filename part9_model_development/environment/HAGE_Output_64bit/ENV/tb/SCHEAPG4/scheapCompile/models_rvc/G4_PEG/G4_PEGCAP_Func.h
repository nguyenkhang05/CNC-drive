// ---------------------------------------------------------------------
// $Id: G4_PEGCAP_Func.h,v 1.7 2020/01/14 02:39:30 nhutminhdang Exp $
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
#ifndef __G4_PEGCAP_FUNC_H__
#define __G4_PEGCAP_FUNC_H__

#include "global.h"
#include "BusSlaveFuncIf.h"
#include "G4_PEGCAP_AgentController.h"
#include "G4_PEG_ERR_INFO.h"

class G4_PEGCAP_E2x_Reg;
class G4_PEGCAP_U2A_Reg;

class G4_PEGCAP_Func: public BusSlaveFuncIf
{
public:
    SC_HAS_PROCESS(G4_PEGCAP_Func);
    /// Constructor & Destructor
    G4_PEGCAP_Func (std::string name, std::string kind, G4_PEGCAP_AgentController *G4_PEGCAP_AgentControllerPtr);
    virtual ~G4_PEGCAP_Func (void);
    
    /// Functions
    void EnableReset (const bool is_active);
    void RegisterHandler (const std::vector<std::string> &args);
    void SetERR_INFO(G4_PEG_Cerrinfo err_info, bool same_spid = false);    
    bool IsSameSPID(unsigned int cur_SPID);
    void NotifyClearPEGSPIDERRCLREvent();
    void ClearPEGSPIDERRCLR();
    
    /// Virtual functions of BusSlaveFuncIf class
    void read (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug);
    void write (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug);
private:
    /// Variables
    G4_PEGCAP_AgentController *mG4_PEGCAP_AgentController;   // Agentcontroller pointer
    G4_PEGCAP_U2A_Reg         *mpG4_PEGCAP_U2A_RegBlock;     // Block to process related to register of G4_PEGCAP of U2A platform
    G4_PEGCAP_E2x_Reg         *mpG4_PEGCAP_E2x_RegBlock;     // Block to process related to register of G4_PEGCAP of E2x platform
   
    std::string mModuleName;                           // Module name
    std::string mKind;                                 // Kind of platform
};
#endif //__G4_PEGCAP_FUNC_H__
