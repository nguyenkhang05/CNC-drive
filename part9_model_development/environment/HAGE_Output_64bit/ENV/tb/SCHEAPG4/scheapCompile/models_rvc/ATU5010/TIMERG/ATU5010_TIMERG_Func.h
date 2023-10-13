// -----------------------------------------------------------------------------
// $Id: ATU5010_TIMERG_Func.h,v 1.0 621 2016-06-15 10:51:02Z CuongPH8 $
//
// Copyright(c) 2016 Renesas Electronics Corporation
// Copyright(c) 2016 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// -----------------------------------------------------------------------------
#ifndef ATU5010_TIMERG_ATU5010_TIMERG_FUNC_H_
#define ATU5010_TIMERG_ATU5010_TIMERG_FUNC_H_

/// common information shared cross over hierarchical design
#include "atu5010_timerg_regif.h"
#include "ATU5010_TIMERG_Common.h"

/// the wrapper class
#include "ATU5010_TIMERG.h"

/// support implementing the role of a *register block*
#include "ATU5010_TIMERG_Func_Timers.h"


/**@brief ATU5010_TIMERG_Func is a core, a function model, a register block,
 *        a bus-slave-FUNCTION model
 *
 */
class ATU5010_TIMERG_Func: public Catu5010_timerg_regif {
//------------------------------------------------------------------------------
// public attributes
//------------------------------------------------------------------------------
public:
    double   mPCLKPeriod; ///< hold value of period of the input PCLK clock
    bool     mReset;      ///< hold value of input reset
    double   mClkBusPeriods[CONFIG::CLK_CNT_NUM];///< period of clock in bus clock

    CONFIG::state_t state;///< model is in reset cycles, stopped or started state

    ATU5010_TIMERG_Func_Timers multi_timer;         ///< instance of the multi-timer
//------------------------------------------------------------------------------
// internal attributes
//------------------------------------------------------------------------------
private:
    ATU5010_TIMERG* pWrapper;   ///< implement relationship between the core (a function model, a register block, a bus-slave-FUNCTION model) and the wrapper (a Command Agent Controller, a Python Interface,
    string       mModuleName; ///< name of the function model

//------------------------------------------------------------------------------
// public interface
//------------------------------------------------------------------------------
public:
    ATU5010_TIMERG_Func(string       iname
                    ,ATU5010_TIMERG* pWrapper);
    virtual ~ATU5010_TIMERG_Func();

    void EnableReset    (const bool is_active);
    bool InResetState();
    bool InNoClkState();
    bool isEnabledInterrupt(const uint8_t& sblkIdx);
    void setPCLKPeriod     (const double& tperiod);

    void RegisterHandler (const bool                      is_com_reg
                         ,const unsigned int              sub_id
                         ,const std::vector<std::string>& args);
//------------------------------------------------------------------------------
// helper methods
//------------------------------------------------------------------------------
private:
    void CancelEvents();
    void Initialize  ();
//------------------------------------------------------------------------------
// implement the role of a BusSlaveFuncIf the interface allows read/write
// access into register block from the Initiator via the BusSlaveIf (the wrapper)
//------------------------------------------------------------------------------
public:
    void read  (unsigned int        offsetAddress
               ,TlmBasicPayload&    trans
               ,BusTime_t*          t
               ,bool                debug);
    void write (unsigned int        offsetAddress
               ,TlmBasicPayload&    trans
               ,BusTime_t*          t
               ,bool                debug);

//------------------------------------------------------------------------------
// implement pure virtual call-back function, the means allowing base class
// register block Catu5010_timerg_regif inform the derived class
// Because of these are used internally in communication between the base and
// the derived class, they are private.
//------------------------------------------------------------------------------
private:
    void cb_TSTRG_STRG   (RegCBstr str);
    void cb_TCRG_EVSYMG  (RegCBstr str);
    void cb_TCRG_CKSELG  (RegCBstr str);
    void cb_TSRG_CMFG    (RegCBstr str);
    void cb_TSRG_OVFG    (RegCBstr str);
    void cb_TSRG_EVDTFG  (RegCBstr str);
    void cb_TSCRG_CMFCG  (RegCBstr str);
    void cb_TSCRG_OVFCG  (RegCBstr str);
    void cb_TCNTG_TCNTG  (RegCBstr str);
    void cb_OCRG_OCRG    (RegCBstr str);
    void cb_RLDG_RLDG    (RegCBstr str);
    void cb_RLDCRG_RLENG (RegCBstr str);
//------------------------------------------------------------------------------
};


#endif /* ATU5010_TIMERG_ATU5010_TIMERG_FUNC_H_ */
//------------------------------------------------------------------------------
// EOF
