// ---------------------------------------------------------------------
// $Id: EMU3S_APB_DEC.h,v 1.5 2020/11/18 18:52:19 thiepnguyen Exp $
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
#ifndef __EMU3S_APB_DEC_H__
#define __EMU3S_APB_DEC_H__

#include "global.h"
#include "BusBridgeModuleBase.h"
#include "BusBridgeSlaveBase.h"
#include "BusMasterBase.h"
#include "rvc_common_model.h"
#include "EMU3S_APB_DEC_Func.h"

template<unsigned int BUSWIDTH, unsigned int S_NUM> class BusBridgeSlaveBase;
template<unsigned int BUSWIDTH, unsigned int S_NUM> class BusMasterBase;
template<unsigned int BUSWIDTH> class TlmTargetSocket;
template<unsigned int BUSWIDTH> class TlmInitiatorSocket;

class EMU3S_APB_DEC_Func;

// class EMU3S_APB_DEC 
class EMU3S_APB_DEC :
     public BusBridgeModuleBase<BUS_WIDTH_APB,BUS_WIDTH_APB>
    ,public BusBridgeSlaveBase<BUS_WIDTH_APB,1>
    ,public BusMasterBase<BUS_WIDTH_APB, 1>
    ,public rvc_common_model
{
    public:
    sc_in<uint64>          PCLK;                    // APB bus maser frequency port
    sc_in<bool>            PRESETn;                 // reset port

    TlmTargetSocket<BUS_WIDTH_APB>    *tsp_cpu;     // target socket for sub CPU
    TlmInitiatorSocket<BUS_WIDTH_APB> *isp_cpu;     // initiator socket
    
    SC_HAS_PROCESS(EMU3S_APB_DEC);
    EMU3S_APB_DEC(sc_module_name name, SIM_MODE_T simmode);
    ~EMU3S_APB_DEC( void );

    sc_in<bool>     EMU3_STOP_REQ;
    sc_out<bool>    APBDEC_STOP_ACK;

    private:
    enum eResetActiveLevel {                        // Define active level of reset signal
        emResetActive = 0
    };

    EMU3S_APB_DEC_Func *mEMU3S_APB_DEC_Func;

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
    
    bool            mapb_stop_ack;
    bool            mis_waiting_completion;
    sc_event        mwrite_apb_stop_ack_event;
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
#endif //__EMU3S_APB_DEC_H__
