// ---------------------------------------------------------------------
// $Id: G4_PEGCAP_U2A_Reg.h,v 1.6 2020/01/14 02:39:30 nhutminhdang Exp $
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
#ifndef __G4_PEGCAP_U2A_Reg_H__
#define __G4_PEGCAP_U2A_Reg_H__

#include "g4_pegcap_u2a_regif.h"
#include "G4_PEG_ERR_INFO.h"
#include "G4_PEGCAP.h"
#include "G4_PEGCAP_Func.h"

// class G4_PEGCAP_Func;

class G4_PEGCAP_U2A_Reg: public Cg4_pegcap_u2a_regif
{
public:
    SC_HAS_PROCESS(G4_PEGCAP_U2A_Reg);
    /// Constructor & Destructor
    G4_PEGCAP_U2A_Reg(std::string name, G4_PEGCAP_Func* parent_func);
    ~G4_PEGCAP_U2A_Reg();

    /// Functions
    void EnableReset(const bool is_active);
    void RegisterHandler(const std::vector<std::string> &args);
    void SetERR_INFO(G4_PEG_Cerrinfo err_info, bool same_spid);    
    bool IsSameSPID(unsigned int cur_SPID);
    void ClearPEGSPIDERRCLR();

    /// Virtual functions of BusSlaveFuncIf class
    void read(unsigned int address, unsigned char *p_data, unsigned int size, bool debug);
    void write(unsigned int address, unsigned char *p_data, unsigned int size, bool debug);

private:
    /// Variables
    G4_PEGCAP_Func   *mpParentFunc;
    std::string      mModuleName;               // Module name
    unsigned int     mSpidclr;                  // Value of PEGSPIDERRCLR register
    unsigned int     mErrorSPID;                // SPID error
    bool             mIsClearFlag;              // Clearing flag
    bool             mIsDetectSameSPIDFlag;     // Error is detected in same spid
    
    /// Functions
    void Initialize();
    
    /// Overwrite virtual functions of Cg4_pegcap_u2a_regif class
    void cb_PEGOVFCLR_CLRO(RegCBstr str);
    void cb_PEGSPIDERRCLR_SPIDCLR(RegCBstr str);
};
#endif //__G4_PEGCAP_U2A_Reg_H__
