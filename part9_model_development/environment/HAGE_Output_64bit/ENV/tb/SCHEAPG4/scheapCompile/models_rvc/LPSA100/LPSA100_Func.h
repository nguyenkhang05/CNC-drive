// ---------------------------------------------------------------------
// $Id: LPSA100_Func.h,v 1.4 2020/01/13 02:56:08 hadinhtran Exp $
//
// Copyright(c) 2018-2020 Renesas Electronics Corporation
// Copyright(c) 2018-2020 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------
#ifndef __LPSA100_FUNC_H__
#define __LPSA100_FUNC_H__

#include "BusSlaveFuncIf.h"
#include "LPSA100_AgentController.h"
#include "lpsa100_regif.h"
#include "LPS_def.h"

class LPSA100_Func: public BusSlaveFuncIf
               ,public Clpsa100_regif
{
private:
    /// Variables
    LPSA100_AgentController *mpLPSA100AgentController;      // Pointer of LPSA100AgentController
    std::string mModuleName;                                // Module name
    bool mEventFlag;                                        // Status of DINEVF bit (is cleared or not)
    std::map<unsigned int, std::string> mINTTAUJMap;        // Map value of INTTAUJ to compare with input
    unsigned int mTJIS;                                     // Value of TIJS[2:0] bit
    unsigned int mNUMDP;                                    // Value of NUMDP[2:0] bit
    bool         mAPEN;                                     // Value of APEN bit
    bool         mDPEN;                                     // Value of DPEN bit

    /// Functions
    void Initialize();

    /// Overwrite virtual functions of Clpsa100_regif class
    void cb_SCTLR_DPEN(RegCBstr str);
    void cb_EVFR_DINEVF(RegCBstr str);
    void cb_DPSELR0_D0EN(RegCBstr str);
    void cb_DPSELRM_D1EN(RegCBstr str);
    void cb_DPSELRH_D5EN(RegCBstr str);
    void cb_DPDSR0_D0(RegCBstr str);
    void cb_DPDSRM_D1(RegCBstr str);
    void cb_DPDSRH_D5(RegCBstr str);
    void cb_CNTVAL_CNT0(RegCBstr str);
    
public:
    SC_HAS_PROCESS (LPSA100_Func);

    /// Constructor & Destructor
    LPSA100_Func(std::string name, LPSA100_AgentController* LPSA100AgentController);
    ~LPSA100_Func();

    /// Functions
    void EnableReset(const bool is_active);
    void RegisterHandler(const std::vector<std::string> &args);
    void SetDPDIMR(uint32_t input_value, unsigned int time);
    void SetDINEVF(bool comparison_result);
    void SetOperationStatus(bool value);
    bool CheckTJIS(std::string inst_name);
    bool GetOperationStatus();
    unsigned int GetNUMDP();
    unsigned int GetDnEN(unsigned int time);
    unsigned int GetDn(unsigned int time);
    unsigned int GetDnM(unsigned int time);
    unsigned int GetCNT0();
    unsigned int GetCNT1();

    /// Virtual functions of BusSlaveFuncIf class
    void read(unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug);
    void write(unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug);
};
#endif //__LPSA100_FUNC_H__
