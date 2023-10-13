// -----------------------------------------------------------------------------
// $Id: ATU5_TIMERG.h 1196 2019-10-17 05:33:04Z chuonghoangle $
//
// Copyright(c) 2018-2019 Renesas Electronics Corporation
// Copyright(c) 2018-2019 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// -----------------------------------------------------------------------------
#ifndef ATU5_TIMERG_ATU5_TIMERG_H_
#define ATU5_TIMERG_ATU5_TIMERG_H_

#include "ATU5_TIMERG_Common.h"
#include "ATU5_TIMERG_AgentController.h"
#include "ATU5_TIMERG_Func.h"

class ATU5_Base;

/**@brief top-module of Timer G block
 */
class ATU5_TIMERG: public sc_module
                 , public ATU5_TIMERG_AgentController
{
friend class ATU5_Base;

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

    // allow enable, disable from the ATU5_Base-wrapper
    void EnableTIG(const bool& is_enable);
    // the internal Function Model
    ATU5_TIMERG_Func* pFuncModel; ///< instance of functional part

//------------------------------------------------------------------------------
// internal sub-modules, signals, variables
//------------------------------------------------------------------------------
private:

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
    SC_HAS_PROCESS (ATU5_TIMERG);
    ATU5_TIMERG(sc_module_name iname);
    virtual ~ATU5_TIMERG();

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

#endif /* ATU5_TIMERG_ATU5_TIMERG_H_ */
//------------------------------------------------------------------------------
// EOF

