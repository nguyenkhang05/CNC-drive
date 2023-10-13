// ---------------------------------------------------------------------
// $Id: G4_PEGCAP_E2x_Reg.h,v 1.5 2020/01/14 02:39:30 nhutminhdang Exp $
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
#ifndef __G4_PEGCAP_E2x_Reg_H__
#define __G4_PEGCAP_E2x_Reg_H__

#include "g4_pegcap_e2x_regif.h"
#include "G4_PEG_ERR_INFO.h"

class G4_PEGCAP_E2x_Reg: public Cg4_pegcap_e2x_regif
{
public:
    SC_HAS_PROCESS(G4_PEGCAP_E2x_Reg);
    /// Constructor & Destructor
    G4_PEGCAP_E2x_Reg(std::string name);
    ~G4_PEGCAP_E2x_Reg();

    /// Functions
    void EnableReset(const bool is_active);
    void RegisterHandler(const std::vector<std::string> &args);
    void SetERR_INFO(G4_PEG_Cerrinfo err_info);    

    /// Virtual functions of BusSlaveFuncIf class
    void read(unsigned int address, unsigned char *p_data, unsigned int size, bool debug);
    void write(unsigned int address, unsigned char *p_data, unsigned int size, bool debug);

private:
    /// Variables
    std::string     mModuleName;       // Module name
    
    /// Overwrite virtual functions of Cg4_pegcap_e2x_regif class
    void cb_PEGERRCLR_CLRO(RegCBstr str);
};
#endif //__G4_PEGCAP_E2x_Reg_H__
