// ---------------------------------------------------------------------
// $Id: ATU5010_TIMERF_Func.h,v 1.0 2016/06/13 09:36:51 ChinhTran Exp $
//
// Copyright(c) 2016 Renesas Electronics Corporation
// Copyright(c) 2016 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------
#ifndef __ATU5010_TIMERF_FUNC_H__
#define __ATU5010_TIMERF_FUNC_H__

#include "ATU5010_TIMERF_Sub.h"
#include "ATU5010_TIMERF_Sub_Func.h"
#include "atu5010_timerf_regif.h"

class ATU5010_TIMERF_AgentController;
class ATU5010_TIMERF_Func: public Catu5010_timerf_regif
{
public:
    ATU5010_TIMERF_Func (std::string name, ATU5010_TIMERF_AgentController *ATU5010_TIMERF_AgentControllerPtr);
    ~ATU5010_TIMERF_Func (void);

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
    ATU5010_TIMERF_AgentController *mATU5010_TIMERF_AgentController;
    std::string mModuleName;        //Module name

    /// Declare virtual functions of ATU5010_TIMERF_regif class

    void cb_TSTRF_STRF0(RegCBstr str);
    void cb_TSTRF_STRF1(RegCBstr str);
    void cb_TSTRF_STRF2(RegCBstr str);
    void cb_TSTRF_STRF3(RegCBstr str);
    void cb_TSTRF_STRF4(RegCBstr str);
    void cb_TSTRF_STRF5(RegCBstr str);
    void cb_TSTRF_STRF6(RegCBstr str);
    void cb_TSTRF_STRF7(RegCBstr str);
    void cb_TSTRF_STRF8(RegCBstr str);
    void cb_TSTRF_STRF9(RegCBstr str);
    void cb_TSTRF_STRF10(RegCBstr str);
    void cb_TSTRF_STRF11(RegCBstr str);
    void cb_TSTRF_STRF12(RegCBstr str);
    void cb_TSTRF_STRF13(RegCBstr str);
    void cb_TSTRF_STRF14(RegCBstr str);
    void cb_TSTRF_STRF15(RegCBstr str);
  	void cb_TSTRF_STRF16(RegCBstr str);
  	void cb_TSTRF_STRF17(RegCBstr str);
  	void cb_TSTRF_STRF18(RegCBstr str);
  	void cb_TSTRF_STRF19(RegCBstr str);
    void cb_PVFCRF_PVFCRKEY(RegCBstr str);
};

#endif
