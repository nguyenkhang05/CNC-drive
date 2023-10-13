// ---------------------------------------------------------------------
// $Id: SEG_Func.h,v 1.7 2015/06/12 08:20:40 dungvannguyen Exp $
//
// Copyright(c) 2014-2015 Renesas Electronics Corporation
// Copyright(c) 2014-2015 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------
#ifndef __SEGFUNC_H__
#define __SEGFUNC_H__
#include "BusSlaveFuncIf.h"
#include "seg_regif.h"

class SEG_AgentController;
struct Csyserrvci;
struct Csyserrtcm;
struct Csyserripg;

class SEG_Func: public Cseg_regif
               ,public BusSlaveFuncIf
{
public:
    SEG_Func (std::string name,
              SEG_AgentController *SEG_AgentControllerPtr,
              std::string SEG_VERSION = "G3M_V30");
    ~SEG_Func (void);

    void StartOfSimulation (void);
    void Reset (const std::string reset_name, const bool is_active);
    void DumpStatusInfo (void);
    void RegisterHandler (const std::vector<std::string> &args);
    void SetSEGCONTInitValue (const unsigned int init_val);
    void UpdateSEGFLAGRegister (const bool is_signal, const std::string error_type);
    void RetainErrInfo (void);
    void ExchangeErrorInfo (const Csyserrvci vcierrinfo, const Csyserrtcm tcmerrinfo, const Csyserripg ipgerrinfo);
    //Virtual functions of BusSlaveFuncIf class
    void read (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug);
    void write (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug);

private:
    SEG_AgentController *mSEG_AgentController;
    std::string mModuleName;//Module name
    std::string mVersion;// SEG version (G3M_V10, G3M_V20, G3M_V30, G3K_PCU, G3MH, G3KH)
    bool mNoticeVciErr;//Error is VCI NOTICE
    bool mNoticeTcmErr;//Error is TCM NOTICE
    unsigned int mPreviousSEGFLAGValue;//Previous value of SEGFLAG
    bool mIsSignal;//Error come from error input signal
    bool mIsStartedSimulation;//Simulation status
    unsigned int mSEGCONTInitValue;//Initial value of SEGCONT register
    bool mG3MHVciError;//Vci error for G3Mh version
    bool mG3MHTcmError;//Tcm error for G3Mh version
    bool mG3MHIpgError;//Ipg error for G3Mh version
    Csyserrvci *mVciErrInfoValue;//Error info from VCI
    Csyserrtcm *mTcmErrInfoValue;//Error info from TCM
    Csyserripg *mIpgErrInfoValue;//Error info from IPG

    void ProcessNotifyingError (const bool is_err_request_output_signal);
    void UpdateErrorInfo (void);
    //Declare virtual functions of seg_regif class
    void cb_SEGFLAG_VCIF (RegCBstr str);
};
#endif
