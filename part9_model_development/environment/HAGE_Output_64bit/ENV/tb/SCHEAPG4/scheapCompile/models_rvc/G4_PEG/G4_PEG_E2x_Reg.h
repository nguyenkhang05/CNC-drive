// ---------------------------------------------------------------------
// $Id: G4_PEG_E2x_Reg.h,v 1.5 2020/01/14 02:39:30 nhutminhdang Exp $
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
#ifndef __G4_PEG_E2X_Reg_H__
#define __G4_PEG_E2X_Reg_H__

#include "g4_peg_e2x_regif.h"

class G4_PEG_E2x_Reg: public Cg4_peg_e2x_regif
{
public:
    SC_HAS_PROCESS(G4_PEG_E2x_Reg);
    /// Constructor & Destructor
    G4_PEG_E2x_Reg(std::string name);
    ~G4_PEG_E2x_Reg();

    /// Functions
    void EnableReset(const bool is_active);
    void RegisterHandler(const std::vector<std::string> &args);
    
    unsigned int getPEGSPID_SPID(unsigned int channel_id);
    bool getPEGROT_GEN(unsigned int channel_id);
    bool getPEGROT_DBG(unsigned int channel_id);
    bool getPEGROT_UM(unsigned int channel_id);
    bool getPEGROT_WG(unsigned int channel_id);
    bool getPEGROT_RG(unsigned int channel_id);
    unsigned int getPEGBAD_BAD(unsigned int channel_id);
    unsigned int getPEGADV_ADV(unsigned int channel_id);

    /// Overwrite virtual functions of BusSlaveFuncIf class
    void read(unsigned int address, unsigned char *p_data, unsigned int size, bool debug);
    void write(unsigned int address, unsigned char *p_data, unsigned int size, bool debug);

private:
    /// Enum
    enum eRegInfo{
        emKeyCode = 0xA5A5A500
    };
    
    /// Variables
    std::string   mModuleName;     // Module name

    /// Overwrite virtual functions of Cg4_peg_e2x_regif class
    void cb_PEGKCPROT_KCPROT(RegCBstr str);
    void cb_PEGPROT_GEN(RegCBstr str);
    void cb_PEGSPID_SPID(RegCBstr str);
    void cb_PEGBAD_BAD(RegCBstr str);
    void cb_PEGADV_ADV(RegCBstr str);
};
#endif //__G4_PEG_E2x_Reg_H__
