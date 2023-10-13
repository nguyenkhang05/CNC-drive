// ---------------------------------------------------------------------
// $Id: QLAPBFICTLAV1_Sub.h,v 1.2 2020/03/13 02:51:46 duongphan Exp $
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
#ifndef __QLAPBFICTLAV1_SUB_H__
#define __QLAPBFICTLAV1_SUB_H__

#include "QLAPBFICTLAV1_Sub_AgentController.h"

class QLAPBFICTLAV1_Sub_Func;
class QLAPBFICTLAV1_Sub:    public sc_module,
                            public QLAPBFICTLAV1_Sub_AgentController
{
friend class QLAPBFICTLAV1;
friend class QLAPBFICTLAV1_Func;
public:
    SC_HAS_PROCESS (QLAPBFICTLAV1_Sub);
    QLAPBFICTLAV1_Sub ( sc_module_name name,
                        QLAPBFICTLAV1 *iQLAPBFICTLAV1_Ptr,
                        unsigned int ch_id);
    ~QLAPBFICTLAV1_Sub (void);
    /// Declare input signals
    sc_in<bool>     FCLATEDSI;      //Edge detect input signal
    sc_in<bool>     FCLATNFSI;      //Input signal after noise filter
    sc_in<bool>     FCLATSI;        //Input signal (direct signal from I/O)
    /// Declare output signals
    sc_out<bool>    FCLATINTR;      //Rising edge select signal
    sc_out<bool>    FCLATINTF;      //Falling edge select signal
    sc_out<bool>    FCLATSO;        //Filtered/bypassed output signal
    sc_out<bool>    FCLATEDSO;      //Edge/Level detection signal

    void EnableReset (const std::string reset_name, const bool is_active);
    void CancelEventsZeroClock (void);

private:
    QLAPBFICTLAV1_Sub_Func *mQLAPBFICTLAV1_Sub_Func;      // QLAPBFICTLAV1_Sub_Func class instance

    /// Declare variables
    std::string mModuleName;
    bool mFCLATINTRValue;//Value of FCLATINTR ports
    bool mFCLATINTFValue;//Value of FCLATINTF ports
    bool mFCLATSOValue;//Value of FCLATSO ports
    bool mFCLATEDSOValue;//Value of FCLATEDSO ports
private:
    /// Declare events
    sc_event    mBitFCLA0INTLSettingGetsEffective;
    sc_event    mBitFCLA0INTRSettingGetsEffective;
    sc_event    mFCLATSOPortsWriteEvent;
    sc_event    mFCLATNFSIPortValueCheckAtSyncTimeEvent;
    sc_event    mFCLATEDSOPortWriteEvent;
    sc_event    mFCLATINTFPortWriteEvent;
    sc_event    mFCLATINTRPortWriteEvent;
    sc_event    mFCLATEDSOPortWriteUpEvent;
    sc_event    mFCLATEDSOPortWriteDownEvent;
    /// Declare Method/Thread
    void HandleFCLATEDSIMethod (void); //Handler input FCLATEDSI port
    void HandleFCLATNFSIMethod (void); //Handler input FCLATNFSI port
    void HandleFCLATSIMethod (void); //Handler input FCLATSI port
    void HandleActiveEffectiveINTLMethod (void); //Handler setting Effect INTL bit
    void HandleUpLevelDetectMethod (void);
    void HandleDowLevelDetectMethod (void);
    void HandleActiveEffectiveINTRMethod (void); //Handler setting Effect INTR bit
    void WriteFCLATSOPortsMethod (void);
    void CheckFCLATNFSIPortValueAtSyncTimeMethod (void);
    void WriteFCLATEDSOPortsMethod (void);
    void WriteFCLATINTFPortsMethod (void);
    void WriteFCLATINTRPortsMethod (void);

    /// Internal function
    void CancelEvents (void);
    void Initialize (void);
    void PortValueAtsmpresz (void);
    void NotifyEffectiveBitSetting (const std::string effective_name);
    void NotifyEdgeDetectorSignal (const std::string edge_name);
    void NotifyFilterSignal (const std::string signal_name);
    void NotifyEdgeSignal (void);
    sc_time EffectivePeriodsSmpclk(void);
};
#endif //__QLAPBFICTLAV1_SUB_H__
// vim600: set foldmethod=marker :
