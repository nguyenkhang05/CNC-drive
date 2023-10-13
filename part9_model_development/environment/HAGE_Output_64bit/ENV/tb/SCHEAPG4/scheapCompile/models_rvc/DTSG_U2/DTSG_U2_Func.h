// ---------------------------------------------------------------------
// $Id: DTSG_U2_Func.h,v 1.7 2020/01/07 04:00:23 synguyen Exp $
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
#ifndef __DTSG_U2_FUNC_H__
#define __DTSG_U2_FUNC_H__

#include "BusSlaveFuncIf.h"
#include "dtsg_u2_regif.h"
#include "global.h"

class DTSG_U2_AgentController;
class DTSG_U2_Func: public Cdtsg_u2_regif,
    public BusSlaveFuncIf
{
public:
    DTSG_U2_Func (std::string name, DTSG_U2_AgentController *DTSG_U2_AgentControllerPtr, unsigned int numOfCh);
    ~DTSG_U2_Func (void);

    void Reset (const bool is_active);
    void RegisterHandler (const std::vector<std::string> &args);
    /// Virtual functions of BusSlaveFuncIf class
    void read (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug);
    void write (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug);
    void GetDTSGPROT(const unsigned int channel_index, unsigned int& mpid, bool& gen, bool& dbg, bool& um, bool& wg, bool& rg);
    void SetVar (std::string bit_name, unsigned int val);
    unsigned int GetSPIDERR();
    unsigned int GetDTSGMPID_SPID(const unsigned int channel_index);
    
private:
    DTSG_U2_AgentController *mDTSG_U2_AgentController;
    std::string  mModuleName;        //Module name
    unsigned int mNumOfChannel;
    bool         mIsReset;
    /// Declare virtual functions of dtsg_u2_regif class
    void cb_DTSGOVFCLR_CLRO(RegCBstr str);
    void cb_DTSGSPIDERRCLR_SPIDCLRE(RegCBstr str);
    void cb_DTSGKCPROT_KCPROT(RegCBstr str);
    void cb_DTSGMPID_SPID(RegCBstr str);
    void cb_DTSGPROT_GR_MPID(RegCBstr str);
    void cb_DTSGPROT__MPID(RegCBstr str);

};
#endif //__DTSG_U2_FUNC_H__

