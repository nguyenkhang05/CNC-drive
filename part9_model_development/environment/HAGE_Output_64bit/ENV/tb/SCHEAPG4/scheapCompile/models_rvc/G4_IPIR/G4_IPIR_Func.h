// ---------------------------------------------------------------------
// $Id: G4_IPIR_Func.h,v 1.7 2019/07/10 11:25:13 nhuvo Exp $
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
#ifndef __G4_IPIR_FUNC_H__
#define __G4_IPIR_FUNC_H__

#include "BusSlaveFuncIf.h"
#include "g4_ipir_regif.h"
#include "G4_IPIR_AgentController.h"

#define START_OFFSET_SELF_AREA  0x0     // Start offset of self-area
#define SELF_AREA_SIZE          0x800   // Size of whole self area (all PEs)
#define SELF_MASK_ADDRESS       0xFFF   // Mask address to check self-area
#define SIZE_PE_SELF_REG        0x100   // Size of register area of each PE
#define REG_MASK                0xFF    // Mask for 8 bits of register

class G4_IPIR_Func: public Cg4_ipir_regif
                ,public BusSlaveFuncIf
{
public:
    SC_HAS_PROCESS (G4_IPIR_Func);
    G4_IPIR_Func(std::string name, G4_IPIR_AgentController* IPIRAgentController);
    ~G4_IPIR_Func();

    void EnableReset(const bool is_active);
    void RegisterHandler(const std::vector<std::string> &args);

    // Virtual functions of BusSlaveFuncIf class
    void read(unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug);
    void write(unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug);

    // APIs
    void ClearRegsChannel(int channel);

private:
    // Variables
    G4_IPIR_AgentController   *mpIPIRAgentController;    // IPIR Agent controller pointer
    bool    maInterrupt[PE_MAX_NUM][INTC_IPIR_CH_NUM];  // Interrupt array 
    std::string mModuleName;    // Module name

    // Functions
    void Initialize();
    bool ConvertAddress(TlmBasicPayload& trans, unsigned int& converted_address);
    unsigned int get_reg_group(unsigned int pe, unsigned int ch);
    unsigned int get_pe_index(unsigned int reg_group);
    unsigned int get_ch_index(unsigned int reg_group);

    // Callback
    void cb_IPInFCLRm_IPInFCLRm(RegCBstr str);
    void cb_IPInREQm_IPInREQm(RegCBstr str);
    void cb_IPInRCLRm_IPInRCLRm(RegCBstr str);

};
#endif //__G4_IPIR_FUNC_H__
