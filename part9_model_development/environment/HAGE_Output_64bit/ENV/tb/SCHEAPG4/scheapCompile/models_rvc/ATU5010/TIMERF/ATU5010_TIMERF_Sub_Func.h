// ---------------------------------------------------------------------
// $Id: ATU5010_TIMERF_Sub_Func.h,v 1.1 2016/04/25 08:21:28 binhnguyen Exp $
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
#ifndef __ATU5010_TIMERF_SUB_FUNC_H__
#define __ATU5010_TIMERF_SUB_FUNC_H__
#include "atu5010_timerf_sub_regif.h"
#include "OSCI2.h"

class ATU5010_TIMERF_Sub_AgentController;

class ATU5010_TIMERF_Sub_Func: public Catu5010_timerf_sub_regif
{
public:
    ATU5010_TIMERF_Sub_Func (std::string name, ATU5010_TIMERF_Sub_AgentController *ATU5010_TIMERF_Sub_AgentControllerPtr);
    ~ATU5010_TIMERF_Sub_Func (void);

    void Reset (const bool is_active);
    void RegisterHandler (const std::vector<std::string> &args);

    void read (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug);
    void write (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug);

    /// Public functions
    bool SelectAinput();
    unsigned int SelectTIAinput();
    unsigned int SelectClock();

    unsigned int OperationMode ();
    unsigned int EdgeSelect ();
    unsigned int GetRegValue (const std::string name);
    void SetRegValue (const std::string name, unsigned int value);
    void SetICFF (unsigned int value);
    bool ICIEnabled ();
    bool OVFAEnabled ();
    bool OVFBEnabled ();
    bool OVFCEnabled ();
    void SetOVFA (unsigned int value);
    void SetOVFB (unsigned int value);
    void SetOVFC (unsigned int value);
    void SetNCNTFA (unsigned int value);
    void SetNCNTFB (unsigned int value);
    unsigned int GetNCNTFA (void);
    unsigned int GetNCNTFB (void);
    unsigned int GetNCRFA (void);
    unsigned int GetNCRFB (void);
    void Initialize (void);

private:

    ATU5010_TIMERF_Sub_AgentController *mATU5010_TIMERF_Sub_AgentController;
    std::string mModuleName;        //Module name

    unsigned int   mBGRAF;
    unsigned int   mBGRCF;
    unsigned int   mBGRDF;
    /// Private functions

    /// Declare virtual functions of ATU5010_TIMERF_Sub_regif class
    void cb_TCR1F_MDF(RegCBstr str);
    void cb_BKCRF_BKENAF(RegCBstr str);
    void cb_TSCRF_OVFCCF(RegCBstr str);
    void cb_TSCRF_OVFCBF(RegCBstr str);
    void cb_TSCRF_OVFCAF(RegCBstr str);
    void cb_TSCRF_ICFCF(RegCBstr str);
    void cb_GRAF_GRAF(RegCBstr str);
    void cb_GRCF_GRCF(RegCBstr str);
    void cb_GRDF_GRDF(RegCBstr str);
    void cb_CDRF_CDRF(RegCBstr str);
};

#endif
