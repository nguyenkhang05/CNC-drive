// ---------------------------------------------------------------------
// $Id: DNFP020_E2_Sub.h 2428 2017-03-20 08:21:18Z ChinhTD $
//
// Copyright(c) 2017 Renesas Electronics Corporation
// Copyright(c) 2017 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------
#ifndef __DNFP020_E2_SUB_H__
#define __DNFP020_E2_SUB_H__

#include "DNFP020_E2_Sub_AgentController.h"
#include "DNFP020_E2_AgentController.h"


class DNFP020_E2_Sub_Func;

class DNFP020_E2_Sub: public sc_module,
            public DNFP020_E2_Sub_AgentController
{
    friend class DNFP020_E2;
private:

    DNFP020_E2_Sub_Func *mDNFP020_E2_Sub_Func;      // DNFP020_E2_Sub_Func class instance


    /// Declare variables

    std::string mModuleName;
    sc_event        mUpdateEDFRegEvent;                 //Update EDF register event
    sc_event        mCheckChangeStateToIdleEvent;
    double          mTimeAssertNRMPoint;                //The time Noise removal signal is asserted
    double          mTimeDeassertNRMPoint;              //The time Noise removal signal is deasserted
    double          mTimeAssertDNFEDGEIN_syncPoint;     //The time DNFEDGEIN_sync is asserted
    double          mTimeDeassertDNFEDGEIN_syncPoint;   //The time DNFEDGEIN_sync is deasserted

    unsigned int    mNRMstate;                      //0: idle, 1: in assert state, 2: in deassert state
    sc_event        mCheckDNFEDGEIN_syncLevelInActiveClockEvent;
    sc_event        mNFENchangedEvent;
    double          mTimePRSChangePoint;              //The time PRS is written

    sc_signal<bool> edge_detecting_signal;
    bool            medge_detecting_signal_value;
    sc_event        medge_detecting_signalEvent;

    bool            mDNFEDGEIN_value;
    sc_signal<bool> DNFEDGEIN_sync_sig;
    bool            mDNFEDGEIN_sync_sig_value;
    sc_event        mWriteDNFEDGEIN_sync_sigEvent;

    sc_signal<bool> noise_eliminated_sig;
    bool            mnoise_eliminated_sig_value;
    sc_event        mWritenoise_eliminated_sigEvent;

    /// Declare events
    sc_event    mAssertDNFEDGEIN_sync0Event;
    sc_event    mDeassertDNFEDGEIN_sync0Event;
    sc_event    mAssertDNFEDGEIN_syncEvent;
    sc_event    mDeassertDNFEDGEIN_syncEvent;

    sc_event    mAssertnoise_eliminated_sigEvent;
    sc_event    mDeassertnoise_eliminated_sigEvent;

    sc_event    mAssertINTEvent;
    sc_event    mDeassertINTEvent;

public:

    /// Declare input signals
    sc_in<bool>     DNFEDGEIN;

    SC_HAS_PROCESS (DNFP020_E2_Sub);
    DNFP020_E2_Sub (sc_module_name name,
                DNFP020_E2_AgentController *DNFP020_E2_AgentController_Ptr,
                unsigned int ch_id);
    ~DNFP020_E2_Sub (void);

    void EnableReset (const std::string reset_name, const bool is_active);

private:

    void Handleedge_detecting_signalMethod (void);

    void HandleDNFEDGEINMethod (void);
    void AssertDNFEDGEIN_sync0Method (void);
    void DeassertDNFEDGEIN_sync0Method (void);
    void AssertDNFEDGEIN_syncMethod (void);
    void DeassertDNFEDGEIN_syncMethod (void);
    void WriteDNFEDGEIN_syncMethod (void);

    void Assertnoise_eliminated_sigMethod (void);
    void Deassertnoise_eliminated_sigMethod (void);
    void Writenoise_eliminated_sigMethod (void);

    void Writeedge_detecting_signalMethod(void);

    bool GetNFEN(void);
    void SetEDFMethod(void);
    unsigned int GetSLST(void);
    unsigned int GetPRS(void);
    unsigned int GetDMD(void);

    void CancelEvents (void);
    void Initialize (void);

    void HandleNFENchangedMethod();
    void CheckDNFEDGEIN_syncLevelInActiveClockMethod();
    void CheckChangeStateToIdleMethod();
    void NRMStateMachineMethod();
    /// Virtual functions of DNFP020_E2_Sub_AgentController class
    void SetTimePRSChangePoint (void);
    void NotifyEvent (const unsigned int event);

};
#endif
