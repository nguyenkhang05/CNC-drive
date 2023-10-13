// ---------------------------------------------------------------------
// $Id: ERRCTRL.h,v 1.1 2014/03/12 09:30:34 duymai Exp $
//
// Copyright(c) 2014 Renesas Electronics Corporation
// Copyright(c) 2014 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------
#ifndef __ERRCTRL_H__
#define __ERRCTRL_H__
#include "BusSlaveFuncIf.h"
#include "errctrl_regif.h"

class PE_ERR_CTRL_AgentController;
struct Cerrinfo;
/// ERRCTRL model class
class ERRCTRL:  public Cerrctrl_regif,
                public BusSlaveFuncIf
{
public:
    ERRCTRL (std::string name,
             PE_ERR_CTRL_AgentController *AgentControllerPtr);
    ~ERRCTRL (void);

private:
    std::string mModuleName;
    PE_ERR_CTRL_AgentController *mAgentController;

    void cb_PGERRSTATCTL_CLRE(RegCBstr str);

public:
    void Reset (const std::string rst_name, const bool is_active);
    void DumpStatusInfo (void);
    void RegisterHandler (const std::vector<std::string> &args);
    void RecordErrTransInfo(const Cerrinfo err_info);

    //virtual functions of BusSlaveFuncIf class
    void read (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug);
    void write (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug);

};
#endif //__ERRCTRL_H__
// vim600: set foldmethod=marker :
