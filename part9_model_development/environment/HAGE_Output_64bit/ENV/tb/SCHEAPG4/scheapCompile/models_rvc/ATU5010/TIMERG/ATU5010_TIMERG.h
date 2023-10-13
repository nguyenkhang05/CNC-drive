// -----------------------------------------------------------------------------
// $Id: ATU5010_TIMERG.h,v 1.0 621 2016-06-16 10:51:02Z CuongPH8 $
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
#ifndef ATU5010_TIMERG_ATU5010_TIMERG_H_
#define ATU5010_TIMERG_ATU5010_TIMERG_H_

#include "ATU5010_TIMERG_Common.h"
#include "ATU5010_TIMERG_AgentController.h"
#include "ATU5010_TIMERG_Func.h"

class ATU5010;
class ATU5010_Func;
/**@brief top-module of Timer G block
 */
class ATU5010_TIMERG: public sc_module
                 , public ATU5010_TIMERG_AgentController
{
friend class ATU5010;
friend class ATU5010_Func;
//------------------------------------------------------------------------------
// public interfaces: in/out/inout ports, sockets, events notifying the exterior, APIs
//------------------------------------------------------------------------------
public:
    // replace the use of pclk port and clk_bus[CONFIG::CLK_CNT_NUM] ports
    void setTimeUnit     (const sc_time_unit& tunit);
    void setPCLKPeriod   (const double& tperiod);
    void setClkBusPeriod (const double& tperiod, const uint8_t& clkIdx);

    // replacement of reset port
    void EnableReset (const bool is_active);

    // input/outputs
    sc_in<bool>*  events [CONFIG::EVT_NUM];     ///< event inputs
    sc_out<bool>* cmi_g  [CONFIG::SUBBLOCK_NUM];///< output interrupts

    // allow enable, disable from the ATU5010-wrapper
    void EnableTIG(const bool& is_enable);

//------------------------------------------------------------------------------
// internal sub-modules, signals, variables
//------------------------------------------------------------------------------
private:
    // the internal Function Model
    ATU5010_TIMERG_Func* pFuncModel; ///< instance of functional part

//------------------------------------------------------------------------------
// internal processes
//------------------------------------------------------------------------------
private:
    void CancelEvents ();
    void Initialize   ();

    void cmi_g_scdmth    (const uint8_t& sblk_id);
    void events_bus_scdmth(const uint8_t& evIdx);
//------------------------------------------------------------------------------
// public interface
//------------------------------------------------------------------------------
public:
    SC_HAS_PROCESS (ATU5010_TIMERG);
    ATU5010_TIMERG(sc_module_name iname);
    virtual ~ATU5010_TIMERG();

//------------------------------------------------------------------------------
// PythonIF
//    internal unit does NOT need implement Python interface
//------------------------------------------------------------------------------
public:

//------------------------------------------------------------------------------
// helper methods
//------------------------------------------------------------------------------
private:

};

#endif /* ATU5010_TIMERG_ATU5010_TIMERG_H_ */
//------------------------------------------------------------------------------
// EOF

