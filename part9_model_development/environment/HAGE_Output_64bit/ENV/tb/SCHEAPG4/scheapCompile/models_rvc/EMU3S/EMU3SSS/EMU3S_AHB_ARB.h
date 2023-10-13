// ---------------------------------------------------------------------
// $Id: EMU3S_AHB_ARB.h,v 1.5 2020/11/18 18:52:19 thiepnguyen Exp $
//
// Copyright(c) 2020 Renesas Electronics Corporation
// Copyright(c) 2020 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------
#ifndef __EMU3S_AHB_ARB_H__
#define __EMU3S_AHB_ARB_H__

#include "global.h"
#include "BusBridgeModuleBase.h"
#include "BusBridgeSlaveBase.h"
#include "BusMasterBase.h"
#include "rvc_common_model.h"
#include "EMU3S_AHB_ARB_Func.h"

template<unsigned int BUSWIDTH, unsigned int S_NUM> class BusBridgeSlaveBase;
template<unsigned int BUSWIDTH, unsigned int S_NUM> class BusMasterBase;
template<unsigned int BUSWIDTH> class TlmTargetSocket;
template<unsigned int BUSWIDTH> class TlmInitiatorSocket;

class EMU3S_AHB_ARB_Func;

// class EMU3S_AHB_ARB 
class EMU3S_AHB_ARB :
     public BusBridgeModuleBase<BUS_WIDTH_AHB,BUS_WIDTH_AHB>
    ,public BusBridgeSlaveBase<BUS_WIDTH_AHB,1>
    ,public BusMasterBase<BUS_WIDTH_AHB, 1>
    ,public rvc_common_model
{
    public:
    sc_in<uint64>          PCLK;                    // APB bus maser frequency port
    sc_in<bool>            PRESETn;                 // reset port
    
    TlmTargetSocket<BUS_WIDTH_APB>    *tsh;         // target socket for sub CPU
    TlmInitiatorSocket<BUS_WIDTH_APB> *ish;         // initiator socket
    
    SC_HAS_PROCESS(EMU3S_AHB_ARB);
    EMU3S_AHB_ARB(sc_module_name name, SIM_MODE_T simmode);
    ~EMU3S_AHB_ARB( void );

    sc_in<bool>     EMU3_STOP_REQ;
    sc_out<bool>    AHBARB_STOP_ACK;

    private:
    enum eResetActiveLevel {                        // Define active level of reset signal
        emResetActive = 0
    };

    EMU3S_AHB_ARB_Func *mEMU3S_AHB_ARB_Func;

    // Variables
    bool            mResetCurVal;                   //Store current value of reset signals
    bool            mResetPreVal;                   //Store previous value of reset signals
    bool            mIsResetHardActive;             //Reset operation status of reset signals
    sc_event        mResetHardEvent; 

    sc_dt::uint64   mPCLKPeriod;                    //Period of PCLK
    sc_dt::uint64   mPCLKFreq;                      //Frequency of PCLK
    std::string     mPCLKFreqUnit;                  //Frequency unit of PCLK
    double          mStartPCLKTime;                 //Start time of clock

    sc_dt::uint64   mTimeResolutionValue;           //Time resolution value of simulation
    sc_time_unit    mTimeResolutionUnit;            //Time resolution unit of simulation

    bool            mahb_stop_ack;
    bool            mis_waiting_completion;
    sc_event        mwrite_ahb_stop_ack_event;
    sc_event        mzero_clock_event;

    void HandleStopRequestMethod( void );

    void Initialize( void );
    void HandlePCLKSignalMethod( void );
    void HandlePRESETNSignalMethod( void );
    void SetCLKFreq(const std::string clock_name, const sc_dt::uint64 freq, const std::string unit);
    void EnableReset (const std::string reset_name, const bool is_active);
    void HandleResetHardMethod( void );
    void HandleStopAckMethod( void );
    void HandleBusStateMethod( void );
};
#endif //__EMU3S_AHB_ARB_H__
