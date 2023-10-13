// ---------------------------------------------------------------------
// $Id: DNFP010_E2_Sub.h 5605 2017-03-17 06:43:09Z ChinhTD $
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
#ifndef __DNFP010_E2_SUB_H__
#define __DNFP010_E2_SUB_H__

#include "DNFP010_E2_Sub_AgentController.h"
#include "DNFP010_E2_AgentController.h"


class DNFP010_E2_Sub_Func;

class DNFP010_E2_Sub: public sc_module,
            		  public DNFP010_E2_Sub_AgentController
{
friend class DNFP010_E2;
private:

    DNFP010_E2_Sub_Func *mDNFP010_E2_Sub_Func;      // DNFP010_E2_Sub_Func class instance


    /// Declare variables
    std::string mModuleName;

    sc_event        mCheckDNFIN_syncLevelInActiveClockEvent;

    unsigned int    mNRMstate;                      //0: idle, 1: in assert state, 2: in deassert state

    sc_event        mNFENchangedEvent;
    sc_event        mCheckChangeStateToIdleEvent;
    double          mTimePRSChangePoint;              //The time PRS is written
    double          mTimeAssertNRMPoint;              //The time Noise removal signal is asserted
    double          mTimeDeassertNRMPoint;            //The time Noise removal signal is deasserted

    double          mTimeAssertDNFIN_syncPoint;       //The time DNFEDGEIN_sync is asserted
    double          mTimeDeassertDNFIN_syncPoint;     //The time DNFEDGEIN_sync is deasserted

    bool            mDNFIN_value;
    sc_signal<bool> DNFIN_sync_sig;
    bool            mDNFIN_sync_sig_value;
    sc_event        mWriteDNFIN_sync_sigEvent;

    sc_signal<bool> noise_eliminated_sig;
    bool            mnoise_eliminated_sig_value;
    sc_event        mWritenoise_eliminated_sigEvent;

    /// Declare events
    sc_event    mAssertDNFIN_sync0Event;
    sc_event    mDeassertDNFIN_sync0Event;
    sc_event    mAssertDNFIN_syncEvent;
    sc_event    mDeassertDNFIN_syncEvent;

    sc_event    mAssertnoise_eliminated_sigEvent;
    sc_event    mDeassertnoise_eliminated_sigEvent;

    bool    mDNFFSOUT_value;
    bool    mDNFFAOUT_value;
    sc_event    mWriteDNFFSOUTEvent;
    sc_event    mWriteDNFFAOUTEvent;

public:

    /// Declare input signals
    sc_in<bool>     DNFIN;

    /// Declare output signals
    sc_out<bool>    DNFFSOUT;
    sc_out<bool>    DNFFAOUT;

    SC_HAS_PROCESS (DNFP010_E2_Sub);
    DNFP010_E2_Sub (sc_module_name name,
                DNFP010_E2_AgentController *DNFP010_E2_AgentController_Ptr,
                unsigned int ch_id);
    ~DNFP010_E2_Sub (void);



    void EnableReset (const std::string reset_name, const bool is_active);

private:
    void HandleDNFINMethod (void);
    void AssertDNFIN_syncMethod (void);
    void DeassertDNFIN_syncMethod (void);
    void AssertDNFIN_sync0Method (void);
    void DeassertDNFIN_sync0Method (void);
    void WriteDNFIN_syncMethod (void);

    void Assertnoise_eliminated_sigMethod (void);
    void Deassertnoise_eliminated_sigMethod (void);
    void Writenoise_eliminated_sigMethod (void);

    void WriteDNFFSOUTMethod (void);
    void WriteDNFFAOUTMethod (void);

    bool GetNFEN(void);
    unsigned int GetSLST(void);
    unsigned int GetPRS(void);

    void CancelEvents (void);
    void Initialize (void);

    void HandleNFENchangedMethod();
    void CheckDNFIN_syncLevelInActiveClockMethod();
    void CheckChangeStateToIdleMethod();
    void NRMStateMachineMethod();

    /// Virtual functions of DNFP010_E2_Sub_AgentController class
    void SetTimePRSChangePoint (void);
    void NotifyEvent (void);
};
#endif
