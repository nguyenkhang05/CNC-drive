// ---------------------------------------------------------------------
// $Id: ATU5_TIMERF_Func.h 1196 2019-10-17 05:33:04Z chuonghoangle $
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
#ifndef __ATU5_TIMERF_FUNC_H__
#define __ATU5_TIMERF_FUNC_H__

#include "ATU5_TIMERF_Sub.h"
#include "ATU5_TIMERF_Sub_Func.h"

#include "atu5_timerf_regif.h"

class ATU5_TIMERF_AgentController;

class ATU5_TIMERF_Func: public Catu5_timerf_regif
{
public:
    ATU5_TIMERF_Func (std::string name, ATU5_TIMERF_AgentController *ATU5_TIMERF_AgentControllerPtr , unsigned int subblock);
    ~ATU5_TIMERF_Func (void);

    void Reset (const bool is_active);
    void RegisterHandler (const bool is_com_reg, const unsigned int sub_id, const std::vector<std::string> &args);

    /// Virtual functions of BusSlaveFuncIf class
    void read (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug);
    void write (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug);

    //variables
    bool mPVFCRF01writable;
    
    bool SubblockIsEnable(const unsigned int sub_id);
    bool GRDFCMEnabled(void);
    bool BKCRWENEnabled(void);
    bool IsNoiseCancelEn(const unsigned int sub_id);
    unsigned int GetNoiseCancelMode(const unsigned int sub_id);
private:
    ATU5_TIMERF_AgentController *mATU5_TIMERF_AgentController;
    std::string mModuleName;        //Module name

    /// Declare virtual functions of ATU5_TIMERF_regif class
    void cb_TSTRF_STRF(RegCBstr str);
    void cb_PVFCRF_PVFCRKEY(RegCBstr str);
};

#endif
