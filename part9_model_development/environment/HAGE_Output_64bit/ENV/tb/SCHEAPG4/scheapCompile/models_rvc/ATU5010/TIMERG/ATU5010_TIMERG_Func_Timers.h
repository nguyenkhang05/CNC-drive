// -----------------------------------------------------------------------------
// $Id: ATU5010_TIMERG_Func_Timers.h,v 1.1 621 2017-06-28 10:51:02Z CuongPH8 $
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
#ifndef ATU5010_TIMERG_ATU5010_TIMERG_FUNC_TIMERS_H_
#define ATU5010_TIMERG_ATU5010_TIMERG_FUNC_TIMERS_H_

#include <stdint.h>
#include <iostream>
using std::cout;
using std::endl;
using std::ios;
#include <iomanip> // std::setw
#include <string>
using std::string;
#include <sstream>
using std::stringstream;

#include "Cgeneral_timer.h"

class ATU5010_TIMERG_Func_Timers: protected Cgeneral_timer {
//------------------------------------------------------------------------------
// public interface
//------------------------------------------------------------------------------
public:
    // event outputs
    sc_event** pCompareMatchEvents_synced;
    sc_event** pOverflowEvents_synced;
    sc_event** pUnderflowEvents_synced;
//------------------------------------------------------------------------------
// internal attributes
//------------------------------------------------------------------------------
private:
    // structural constants
    uint8_t CHAN_NUM;    ///< number of channels in multi-channel timers, this parameters is only meaningful for channel #0
    uint8_t CLK_DIV_NUM; ///< number derivative clocks, this parameters is only meaningful for channel #0
    uint8_t EXT_EVT_NUM; ///< number derivative clocks, this parameters is only meaningful for channel #0
    // structural variables
    ATU5010_TIMERG_Func_Timers** pChannels; ///< private pointer is only meaningful for channel #0, allows the channel #0 access into the others
    ATU5010_TIMERG_Func_Timers*  pChannel0;
    uint8_t                   id;
    // reset input
    bool      isResetActive_preSync;
    // clock inputs
    double*   pClkPeriods_synced;
    sc_dt::uint64* pClkDivs_synced;
    sc_time*  pSyncPoints;
    uint8_t   clkDivSel_preSync;
    // start-stop control inputs
    bool      isStartMaster_preSync;
    bool      isStartChan_preSync;
    // external event sync control inputs
    bool      extEventDetectFlag_synced;
    bool      extEventStartMode_preSync;
    uint8_t   extEventSelect_preSync;
    bool*     pExtEvents_synced;
    // compare-match value & reloading compare value control inputs
    uint32_t  compVal_preSync;
    bool      reloadEn_preSync;   ///< enable reload operation
    uint32_t  reloadVal_preSync;  ///< value reload into CR register whenever compare-match occurs and reload operation is allowed.
    // override the existent flags of Cgeneral_timer
    bool CMF; // compare match flag, override the existent of Cgeneral_timer
    bool OVF; // overflow flag, override the existent of Cgeneral_timer
    bool OVFCheck; // overflow flag, override the existent of Cgeneral_timer
    // Update with new General timer
    double    int_Time;
    double    comp_Time;
    bool      is_int;
    bool      is_comp;
//------------------------------------------------------------------------------
// internal methods
//------------------------------------------------------------------------------
private:
    ATU5010_TIMERG_Func_Timers(ATU5010_TIMERG_Func_Timers* ipChannel0
                           ,const uint8_t&           channel_id
                           ,const sc_module_name&    iname);
//------------------------------------------------------------------------------
// public interface
//------------------------------------------------------------------------------
public:
    SC_HAS_PROCESS(ATU5010_TIMERG_Func_Timers);
    ATU5010_TIMERG_Func_Timers(const uint8_t&        iChanNum
                           ,const uint8_t&        iClkDivNum
                           ,const uint8_t&        iExtEvtNum
                           ,const sc_module_name& iname);
    virtual ~ATU5010_TIMERG_Func_Timers();

    sc_time   to_next_sync_point_pclk();
    sc_time   to_next_sync_point(const uint8_t& clkIdx);
    bool      is_sync_point (const uint8_t& clkIdx);
//------------------------------------------------------------------------------
// override base classes' virtual methods
//------------------------------------------------------------------------------
private:
    void cmpMatchOutputHandling (bool value);
    void interruptHandling ();

    sc_event  preOverflow_syncedEvent;  ///< counter overflow  event
    void      preOverflow_syncedThread(const uint8_t& channel_id);
    sc_event  startCnt_syncedEvent;
    void      startCnt_syncedThread(const uint8_t& channel_id);
    sc_event  compareMatch_syncedEvent;   ///< compare match event
    void      compareMatch_syncedThread(const uint8_t& channel_id);

    bool     is_countable_preSync(const uint8_t&  channel_id);
    bool     is_event_detected_preSync(const uint8_t&  channel_id);
//------------------------------------------------------------------------------
// getters/setters
//------------------------------------------------------------------------------
public:
    // reset the counter (EnableReset)
    void enable_reset_async(const bool is_active);

    // timer operation mode
    void set_ext_event_synced (const bool&    value
                              ,const uint8_t& evt_idx);
    void set_ext_event_start_mode(const bool&    is_ext_event_sync_mode
                                 ,const uint8_t& channel_id);
    bool get_ext_event_detect_flag(const uint8_t& channel_id);
    void set_ext_event_detect_flag(const bool& val, const uint8_t& channel_id);
    void ext_event_start_cnt(const uint8_t& channel_id);



    // timer start STR (StartStop)
    void set_str_mst_async(const bool& value);
    void set_str_async(const bool& value
                      ,const uint8_t& channel_id);
    // counter value CNT (SetCounterInitialValue/GetCounterValue)
    void     set_cnt(const uint32_t& value
                    ,const uint8_t&  channel_id);
    uint32_t get_cnt(const uint8_t&  channel_id);
    // compare value constant COR (SetCompareMatchValue/GetCompareMatchValue)
    void     set_cor(const uint32_t& value
                    ,const uint8_t&  channel_id);
    uint32_t get_cor(const uint8_t&  channel_id);
    // clock count period (SetCounterPeriod)
    void   set_clk_div_period  (const double&  clk_period
                               ,const uint8_t& clkIdx);
    void   set_pclk_period     (const double&  clk_period);
    // time unit
    void         set_time_unit(const sc_time_unit& time_unit);
    // clock divisor
    void     set_clk_div_sel(const uint8_t&  clkDivSel
                            ,const uint8_t&  channel_id);
    // compare match flag
    bool get_cmf   (const uint8_t& channel_id);
    void clear_cmf (const uint8_t& channel_id);
    void set_cmf   (const bool& val, const uint8_t& channel_id);
    // overflow flag
    bool get_ovff  (const uint8_t& channel_id);
    void clear_ovff(const uint8_t& channel_id);
    void set_ovff  (const bool& val, const uint8_t& channel_id);
    // reload enable setting
    void set_rl_en (const bool&    irlen
                   ,const uint8_t& channel_id);
    // reload variable
    void     set_rl_val (const uint32_t& irlen
                        ,const uint8_t&  channel_id);
//------------------------------------------------------------------------------
// helper methods
//------------------------------------------------------------------------------
public:
    // common setting (SetupGeneralTimer)
    void common_settings();
};

#endif /* ATU5010_TIMERG_ATU5010_TIMERG_FUNC_TIMERS_H_ */
//------------------------------------------------------------------------------
// EOF
