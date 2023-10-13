// ---------------------------------------------------------------------
// $Id: HI20FSGD2_ERRSLV_Func.h,v 1.2 2019/11/28 09:56:56 hadinhtran Exp $
//
// Copyright(c) 2019 Renesas Electronics Corporation
// Copyright(c) 2019 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------

#ifndef __HI20FSGD2_ERRSLV_FUNC_H__
#define __HI20FSGD2_ERRSLV_FUNC_H__

#include "BusSlaveFuncIf.h"
#include "hi20fsgd2_errslv_regif.h"
#include "HI20FSGD2_ERRSLV_AgentController.h"

class HI20FSGD2_ERRSLV_Func: public Chi20fsgd2_errslv_regif,
                public BusSlaveFuncIf
{
public:
    HI20FSGD2_ERRSLV_Func (std::string name, HI20FSGD2_ERRSLV_AgentController *HI20FSGD2_ERRSLV_AgentControllerPtr);
    ~HI20FSGD2_ERRSLV_Func(void);

    void EnableReset (const bool is_active);
    void RegisterHandler (const std::vector<std::string> &args);

    /// Virtual functions of BusSlaveFuncIf class
    void read(unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug);
    void write(unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug);

    void SetVar(const std::string name, unsigned int val);
    void SetOverFlowBit(void);
    unsigned int GetAccessInfo(const std::string name, unsigned int val);
    
    bool IsTheFirstERR(void);
    bool IsTheSameSPID(unsigned int spid_err);

private:
    // Variables
    HI20FSGD2_ERRSLV_AgentController     *mpHI20FSGD2_ERRSLV_AgentController;        // HI20FSGD2_ERRSLV Agent controller pointer
    std::string                   mModuleName;                       // Module name
    
    unsigned int                    mTypeADDR;
    bool                            mTypeSEC;
    bool                            mTypeDBG;
    bool                            mTypeUM;
    unsigned int                    mTypeSPID;
    bool                            mTypeWRITE;

    // Function
    void Initialize(void);

    // Overwrite callback function
    void cb_PBGOVFCLR_CLRO(RegCBstr str);
    void cb_PBGSPIDERRCLR_SPIDCLR(RegCBstr str);
    void cb_PBGKCPROT_KCPROT(RegCBstr str);
};
#endif //__HI20FSGD2_ERRSLV_FUNC_H__

