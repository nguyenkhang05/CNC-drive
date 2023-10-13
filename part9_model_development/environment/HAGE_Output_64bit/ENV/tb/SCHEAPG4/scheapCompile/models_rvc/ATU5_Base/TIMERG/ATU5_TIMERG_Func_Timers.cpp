// -----------------------------------------------------------------------------
// $Id: ATU5_TIMERG_Func_Timers.cpp 1196 2019-10-17 05:33:04Z chuonghoangle $
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
#include "ATU5_TIMERG_Func_Timers.h"
#include <cmath>
//------------------------------------------------------------------------------
/**@brief public constructor of the first channel #0 of multi-channel timer
 * @param[in] iname name of the first channel #0 of the multi-channel timer
 * @return none
 * @details
 * Detailed steps as follows:
 */
ATU5_TIMERG_Func_Timers::ATU5_TIMERG_Func_Timers(const uint8_t&        iChanNum
                                                ,const uint8_t&        iClkDivNum
                                                ,const uint8_t&        iExtEvtNum
                                                ,const sc_module_name& iname)
: Cgeneral_timer (iname)
// event outputs
, pCompareMatchEvents_synced (NULL)
, pOverflowEvents_synced     (NULL)
, pUnderflowEvents_synced    (NULL)
// structural constants
, CHAN_NUM     (1)
, CLK_DIV_NUM  (1)
, EXT_EVT_NUM  (1)
// structural variables
, pChannels (NULL)
, pChannel0 (this)
, id        (0)
// reset input
, isResetActive_preSync(false)
// clock inputs
, pClkPeriods_synced   (NULL)
, pClkDivs_synced      (NULL)
, pSyncPoints          (NULL)
, clkDivSel_preSync (0)
// start-stop control inputs
, isStartMaster_preSync  (false)
, isStartChan_preSync    (false)
// external event sync control inputs
, extEventDetectFlag_synced (false)
, extEventStartMode_preSync  (false)
, extEventSelect_preSync    (0)
, pExtEvents_synced        (NULL)
// compare-match value & reloading compare value control inputs
, compVal_preSync        ((uint32_t)0xFFFFFFFF)
, reloadEn_preSync       (false)
, reloadVal_preSync      ((uint32_t)0xFFFFFFFF)
// override the existent flags of Cgeneral_timer
, CMF (false)
, OVF (false)
, OVFCheck (false)
, int_Time (0)
, comp_Time (0)
, is_int (false)
, is_comp (false)
{   
    isset_flagOvf = false;
    isclear_flagOvf = false;
    isset_flagCmf = false;
    isclear_flagCmf = false;
    sc_assert(pChannel0 == this);

    if (iChanNum    > CHAN_NUM)
        CHAN_NUM    = iChanNum;
    if (iClkDivNum  > CLK_DIV_NUM)
        CLK_DIV_NUM = iClkDivNum;
    if (iExtEvtNum  > EXT_EVT_NUM)
        EXT_EVT_NUM = iExtEvtNum;

    /// -# instantiate the others pChannels by invoking the private constructor
    // allocate pointers
    pChannels = new ATU5_TIMERG_Func_Timers*[CHAN_NUM];
    sc_assert(pChannels != NULL);

    pClkPeriods_synced = new double[CLK_DIV_NUM+1];
    sc_assert(pClkPeriods_synced != NULL);
    pClkDivs_synced = new uint32_t[CLK_DIV_NUM+1];
    sc_assert(pClkDivs_synced != NULL);
    pSyncPoints = new sc_time[CLK_DIV_NUM+1];
    sc_assert(pSyncPoints != NULL);
    for (uint8_t clkIdx = 0; clkIdx < (CLK_DIV_NUM+1); ++clkIdx) {
        pClkPeriods_synced[clkIdx] = 0;
        pClkDivs_synced   [clkIdx] = 0;
        pSyncPoints[clkIdx] = sc_time(0, SC_NS);
    }

    pExtEvents_synced = new bool[EXT_EVT_NUM];
    sc_assert(pExtEvents_synced != NULL);
    for (uint8_t evId = 0; evId < EXT_EVT_NUM; ++evId) {
        pExtEvents_synced[evId] = false;
    }

    pCompareMatchEvents_synced = new sc_event*[CHAN_NUM];
    pOverflowEvents_synced     = new sc_event*[CHAN_NUM];
    pUnderflowEvents_synced    = new sc_event*[CHAN_NUM];
    sc_assert(pCompareMatchEvents_synced != NULL);
    sc_assert(pOverflowEvents_synced     != NULL);
    sc_assert(pUnderflowEvents_synced    != NULL);
    //
    pChannels[0]                  =   this;
    pCompareMatchEvents_synced[0] = &(this->compareMatch_syncedEvent);
    pOverflowEvents_synced    [0] = &(this->preOverflow_syncedEvent);
    //
    for (uint8_t chIdx = 1; chIdx < CHAN_NUM; ++chIdx) {
        /// -# instantiate the others pChannels by invoking the private constructor
        pChannels[chIdx] = new ATU5_TIMERG_Func_Timers(this
                                                      ,chIdx
                                                      ,sc_gen_unique_name(string("").append(basename()).append("_subchannel").c_str()));
        sc_assert(pChannels[chIdx] != NULL);
        pCompareMatchEvents_synced[chIdx] = &(pChannels[chIdx]->compareMatch_syncedEvent);
        pOverflowEvents_synced    [chIdx] = &(pChannels[chIdx]->preOverflow_syncedEvent);
    }

    /// -# common settings for all channels
    common_settings();

    /// -# register processes
    for (uint8_t channel_id = 0; channel_id < CHAN_NUM; ++channel_id) {
        sc_core::sc_spawn_options opt;
        sc_core::sc_spawn(sc_bind(&ATU5_TIMERG_Func_Timers::startCnt_syncedThread
                                 ,this
                                 ,channel_id)
                         ,sc_core::sc_gen_unique_name("startCnt_syncedThread")
                                                     ,&opt);
    }
    for (uint8_t channel_id = 0; channel_id < CHAN_NUM; ++channel_id) {
        sc_core::sc_spawn_options opt;
        sc_core::sc_spawn(sc_bind(&ATU5_TIMERG_Func_Timers::compareMatch_syncedThread
                                 ,this
                                 ,channel_id)
                         ,sc_core::sc_gen_unique_name("compareMatch_syncedThread")
                                                     ,&opt);
    }
    for (uint8_t channel_id = 0; channel_id < CHAN_NUM; ++channel_id) {
        sc_core::sc_spawn_options opt;
        sc_core::sc_spawn(sc_bind(&ATU5_TIMERG_Func_Timers::preOverflow_syncedThread
                                 ,this
                                 ,channel_id)
                         ,sc_core::sc_gen_unique_name("overflow_syncedThread")
                                                     ,&opt);
    }

    for (uint8_t channel_id = 0; channel_id < CHAN_NUM; ++channel_id) {
        sc_core::sc_spawn_options opt;
        sc_core::sc_spawn(sc_bind(&ATU5_TIMERG_Func_Timers::prioritymethod
                                 ,this
                                 ,channel_id)
                         ,sc_core::sc_gen_unique_name("prioritymethod")
                                                     ,&opt);
	
	
    }

}
//------------------------------------------------------------------------------
/**@brief private constructor of the rest instance of multi-channel timer
 * @param[in] iname name of the other channels in multi-channel timer
 * @param[in] channelId the dummy input is only to make this constructor
 *            different from the public constructor of the first instance in
 *            multi-channel timer.
 * @return none
 * @details
 *      private constructor prevents the mistaken usage of user, user can only
 *      instantiate the multi-channel timer.
 * Detailed steps as follow
 */
ATU5_TIMERG_Func_Timers::ATU5_TIMERG_Func_Timers(ATU5_TIMERG_Func_Timers* ipChannel0
                                                ,const uint8_t&           channel_id
                                                ,const sc_module_name&    iname)
: Cgeneral_timer            (iname)
// event outputs
, pCompareMatchEvents_synced(NULL)
, pOverflowEvents_synced    (NULL)
, pUnderflowEvents_synced   (NULL)
// structural constants
, CHAN_NUM                  (0)
, CLK_DIV_NUM               (0)
, EXT_EVT_NUM               (0)
// structural variables
, pChannels                 (NULL)
, pChannel0                 (ipChannel0)
, id                        (channel_id)
// reset input
, isResetActive_preSync     (false)
// clock inputs
, pClkPeriods_synced        (NULL)
, pClkDivs_synced           (NULL)
, pSyncPoints               (NULL)
, clkDivSel_preSync         (0)
// start-stop control inputs
, isStartMaster_preSync  (false)
, isStartChan_preSync    (false)
// external event sync control inputs
, extEventDetectFlag_synced (false)
, extEventStartMode_preSync  (false)
, extEventSelect_preSync    (0)
, pExtEvents_synced         (NULL)
// compare-match value & reloading compare value control inputs
, compVal_preSync        ((uint32_t)0xFFFFFFFF)
, reloadEn_preSync       (false)
, reloadVal_preSync      ((uint32_t)0xFFFFFFFF)
// override the existent flags of Cgeneral_timer
, CMF (false)
, OVF (false)
, OVFCheck (false)
{
    // event outputs
    sc_assert(pCompareMatchEvents_synced == NULL);
    sc_assert(pOverflowEvents_synced     == NULL);
    sc_assert(pUnderflowEvents_synced    == NULL);
    // structural constants
    sc_assert(CHAN_NUM    == 0);
    sc_assert(CLK_DIV_NUM == 0);
    // structural variables
    sc_assert(pChannels == NULL);
    sc_assert(pChannel0 != this);
    sc_assert(pChannel0 != NULL);
    // clock inputs
    sc_assert(pClkPeriods_synced == NULL);
    sc_assert(pClkDivs_synced    == NULL);
    sc_assert(pSyncPoints        == NULL);
    // external event sync control inputs
    sc_assert(pExtEvents_synced  == NULL);
}
//------------------------------------------------------------------------------
/**@brief the common destructor of all channels of multi-channel timer
 * @return none
 * @details
 * Detailed steps as follow
 */
ATU5_TIMERG_Func_Timers::~ATU5_TIMERG_Func_Timers() {
    sc_assert(pChannel0->pChannels[id] == this);

    /// -# from the channel #0 view point: delete array of clock periods
    if (pClkPeriods_synced != NULL) {
        sc_assert((pChannel0 == this) && (pChannels[0] == this));
        delete []pClkPeriods_synced;
    }
    /// -# from the channel #0 view point: delete array of clock sync points
    if (pClkDivs_synced != NULL) {
        sc_assert((pChannel0 == this) && (pChannels[0] == this));
        delete []pClkDivs_synced;
    }
    /// -# from the channel #0 view point: delete array of clock sync points
    if (pSyncPoints != NULL) {
        sc_assert((pChannel0 == this) && (pChannels[0] == this));
        delete []pSyncPoints;
    }
    /// -# from the channel #0 view point: delete array of external event sync control inputs
    if (pExtEvents_synced != NULL) {
        sc_assert((pChannel0 == this) && (pChannels[0] == this));
        delete []pExtEvents_synced;
    }
    /// -# from the channel #0 view point: delete array of compare match event pointer
    if (pCompareMatchEvents_synced != NULL) {
        sc_assert((pChannel0 == this) && (pChannels[0] == this));
        delete[] pCompareMatchEvents_synced;
    }
    /// -# from the channel #0 view point: delete array of overflow event pointer
    if (pOverflowEvents_synced != NULL) {
        sc_assert((pChannel0 == this) && (pChannels[0] == this));
        delete[] pOverflowEvents_synced;
    }
    /// -# from the channel #0 view point: delete array of under event pointer
    if (pUnderflowEvents_synced != NULL) {
        sc_assert((pChannel0 == this) && (pChannels[0] == this));
        delete[] pUnderflowEvents_synced;
    }
    /// -# from the channel #0 view point: delete other channels
    if (pChannels != NULL) {
        sc_assert((pChannel0 == this) && (pChannels[0] == this));
        for (uint8_t chIdx = 1; chIdx < CHAN_NUM; ++chIdx)
            if (pChannels[chIdx] != NULL)
                delete pChannels[chIdx];
        delete[] pChannels;
    }
}
//------------------------------------------------------------------------------
/**@brief Overriding base-class's virtual method, this handles compare match
 *        output event
 * @details
 *     Notice that this function is available for all channels, but from the user
 *     perspective, none of them is accessible. The only way to access into
 *     other channels is indirectly via event array pCompareMatchEvents of
 *     the channel *0*
 * Detailed steps as follow:
 */
void ATU5_TIMERG_Func_Timers::cmpMatchOutputHandling (bool value) {
    #if 0
    cout << endl << "@[" << std::setfill (' ') << std::setw(7) << sc_time_stamp().to_default_time_units() << "]";
    cout << "[" << "triggering compare match event" << "]";
    cout << "[" << "reload enable" << reloadEn_preSync << "]";
    cout << "[" << "reload value"  << reloadEn_preSync << "]";
    cout << "[" << "CMF:"  << (uint32_t)getSR().gtCMF << "]";
    #endif

    /// -# issue an interrupt indirectly via compareMatch_scev event
    if ((comp_Time == sc_time_stamp().to_double()) && (is_comp)) {
        return;
    } else {
        comp_Time = sc_time_stamp().to_double();
        is_comp = true;
        CMF = true;
        compareMatch_syncedEvent.notify(pChannel0->to_next_sync_point_pclk());
    }
}
//------------------------------------------------------------------------------
/**@brief Overriding base-class's virtual method, this handles interrupt event
 * @details
 *     Notice that this function is available for all channels, but from the user
 *     perspective, none of them is accessible. The only way to access into
 *     other channels is indirectly via event array pOverflowEvents of
 *     the channel *0*
 */
void ATU5_TIMERG_Func_Timers::interruptHandling () {
    if ((int_Time == sc_time_stamp().to_double()) && (is_int)) {
        return;
    } else {
        int_Time = sc_time_stamp().to_double();
        is_int = true;
        if (getSR().gtOVF && getSR().gtCMF) {
            clearOVF();
        } else if (getSR().gtOVF && !getSR().gtCMF) {
            preOverflow_syncedEvent.notify(pChannel0->to_next_sync_point(clkDivSel_preSync));
        }
        if (getSR().gtOVF) {
            clearOVF();
            OVFCheck = true;
            pChannel0->set_cnt(0xFFFFFFFF, id);
        }
    }
}
//------------------------------------------------------------------------------
/**@brief reset all channels to initial state
 * @param is_active whether reset is active or not
 */
void ATU5_TIMERG_Func_Timers::enable_reset_async (const bool is_active) {
    sc_assert((pChannel0 == this) && (pChannels[0] == this));

    for (uint8_t channel_id = 0; channel_id < CHAN_NUM; ++channel_id) {
        ATU5_TIMERG_Func_Timers* pChan = pChannels[channel_id];
        assert(pChan != NULL);
        pChan->EnableReset(is_active);
        // reset input
        pChan->isResetActive_preSync = is_active;
        if (is_active) {
            // clock inputs
            pChan->clkDivSel_preSync     = 0;
            // start-stop control inputs
            pChan->isStartMaster_preSync = false;
            pChan->isStartChan_preSync   = false;
            // external event sync control inputs
            pChan->extEventDetectFlag_synced = false;
            pChan->extEventStartMode_preSync  = false;
            pChan->extEventSelect_preSync    = 0;
            // compare-match value & reloading compare value control inputs
            pChan->compVal_preSync   = (uint32_t)0xFFFFFFFF;
            pChan->reloadEn_preSync  = false;
            pChan->reloadVal_preSync = (uint32_t)0xFFFFFFFF;
            // flags overriding the existent flags of Cgeneral_timer
            pChan->OVF = false;
            pChan->OVFCheck = false;
            // event outputs
            pChan->preOverflow_syncedEvent  .cancel();
            pChan->startCnt_syncedEvent     .cancel();
            pChan->compareMatch_syncedEvent .cancel();
            pChan->setSTR(false);
            pChan->int_Time = 0;
            pChan->comp_Time = 0;
            pChan->is_int = false;
            pChan->is_comp = false;
	    pChan->mPriorityEvent.cancel();		// added  for update version 0.080
	    pChan->isset_flagOvf = false;		// added  for update version 0.080
	    pChan->isclear_flagOvf = false;		//  Added  for update version 0.080
	    pChan->isset_flagCmf = false;		//  Added  for update version 0.080
	    pChan->isclear_flagCmf = false;		//  Added  for update version 0.080

        }
    }
}
//------------------------------------------------------------------------------
bool ATU5_TIMERG_Func_Timers::is_event_detected_preSync(const uint8_t&  channel_id) {
    sc_assert((pChannel0 == this) && (pChannels[0] == this));
    ATU5_TIMERG_Func_Timers* pChan = pChannels[channel_id];
    assert(pChan != NULL);
    if (  !pChan->isResetActive_preSync
       &&  isStartMaster_preSync
       &&  pChan->isStartChan_preSync
       &&  pChan->extEventStartMode_preSync
       &&  pExtEvents_synced[extEventSelect_preSync]
       &&  pClkPeriods_synced[CLK_DIV_NUM] != 0   // PCLK
    ) return true;
    return false;
}
//------------------------------------------------------------------------------
bool ATU5_TIMERG_Func_Timers::is_countable_preSync(const uint8_t& channel_id) {
    sc_assert((pChannel0 == this) && (pChannels[0] == this));
    ATU5_TIMERG_Func_Timers* pChan = pChannels[channel_id];
    assert(pChan != NULL);
    #if 0
    cout << endl << "@[" << std::setfill (' ') << std::setw(7) << sc_time_stamp().to_default_time_units() << "]";
    cout << "[" << "channel:" << (uint32_t)channel_id << " is runnable" << "]";
    cout << "[" << "reset:" << (uint32_t)pChan->isResetActive_preSync << "]";
    cout << "[" << "started:" << (uint32_t)pChan->isStartChan_preSync << "]";
    cout << "[" << "selector:" << (uint32_t)pChan->clkDivSel_preSync << "]";
    cout << "[" << "selected divisor:" <<     pClkDivs_synced   [pChan->clkDivSel_preSync]  << "]";
    cout << "[" << "selected count clock:" << pClkPeriods_synced[pChan->clkDivSel_preSync] << "]";
    cout << "[" << "current value:" << pChan->getCNT() << "]";
    #endif
    
    bool is_ext_event_sync_mode_countable = false;
    if (pChan->extEventStartMode_preSync) { // external event sync mode is started
        if (pChan->extEventDetectFlag_synced)  { // external event is detected
            is_ext_event_sync_mode_countable = 1;
        } else {
            is_ext_event_sync_mode_countable = pExtEvents_synced[extEventSelect_preSync];
        }
    } else {
        is_ext_event_sync_mode_countable = 1;
    }

    if (  !pChan->isResetActive_preSync
       &&  isStartMaster_preSync
       &&  pChan->isStartChan_preSync
       &&  is_ext_event_sync_mode_countable
       &&  pClkPeriods_synced[pChan->clkDivSel_preSync] != 0   // selected clock
       &&  pClkDivs_synced   [pChan->clkDivSel_preSync] != 0   // selected divisor
       &&  pClkPeriods_synced[CLK_DIV_NUM]              != 0   // PCLK
    ) return true;
    return false;
}
//------------------------------------------------------------------------------
void ATU5_TIMERG_Func_Timers::set_ext_event_synced(const bool&    value
                                                  ,const uint8_t& evt_idx)
{
    sc_assert((pChannel0 == this) && (pChannels[0] == this));
    if (evt_idx >= EXT_EVT_NUM)
        return;
    pExtEvents_synced[evt_idx] = value;

    if (extEventSelect_preSync != evt_idx)
        return;

    for (uint8_t channel_id = 0; channel_id < CHAN_NUM; ++channel_id) {
        ext_event_start_cnt(channel_id);
    }
}
//------------------------------------------------------------------------------
void ATU5_TIMERG_Func_Timers::set_ext_event_start_mode(const bool&    is_ext_event_sync_mode
                                                      ,const uint8_t& channel_id)
{
    sc_assert((pChannel0 == this) && (pChannels[0] == this));
    pChannels[channel_id]->extEventStartMode_preSync = is_ext_event_sync_mode;

    #if 0
    cout << endl << "@[" << std::setfill (' ') << std::setw(7) << sc_time_stamp().to_default_time_units() << "]";
    cout << "[" << "change operation mode (event sync):" << uint32_t(pChannels[channel_id]->extEventStartMode_preSync)  << "]";
    #endif

    ext_event_start_cnt(channel_id);
}
//------------------------------------------------------------------------------
void ATU5_TIMERG_Func_Timers::ext_event_start_cnt(const uint8_t& channel_id) {
    sc_assert((pChannel0 == this) && (pChannels[0] == this));
    ATU5_TIMERG_Func_Timers* pChan = pChannels[channel_id];
    assert(pChan != NULL);
    if (!pChan->extEventDetectFlag_synced && is_event_detected_preSync(channel_id)) {
        pChan->extEventDetectFlag_synced = true;
    }

    pChan->startCnt_syncedEvent.cancel();
    if (is_countable_preSync(channel_id)) {
        if (!pChan->getCR().gtSTR) {
            pChan->startCnt_syncedEvent.notify(to_next_sync_point(pChan->clkDivSel_preSync));
        }
    } else {
        pChan->startCnt_syncedEvent.cancel();
        pChan->setSTR(false);
    }
}
//------------------------------------------------------------------------------
bool ATU5_TIMERG_Func_Timers::get_ext_event_detect_flag(const uint8_t& channel_id) {
    sc_assert((pChannel0 == this) && (pChannels[0] == this));
    return pChannels[channel_id]->extEventDetectFlag_synced;
}
//------------------------------------------------------------------------------
void ATU5_TIMERG_Func_Timers::set_ext_event_detect_flag(const bool& val, const uint8_t& channel_id) {
    sc_assert((pChannel0 == this) && (pChannels[0] == this));
    pChannels[channel_id]->extEventDetectFlag_synced = val;
}
//------------------------------------------------------------------------------
/**@brief set value of the internal counter of channel *channel_id*
 * @details
 *     Notice that this function is available for all channels, but from the user
 *     perspective, only channel *0* is accessible. The only way to access into
 *     other channels is to access indirectly via the channel *0*
 * @return none
 */
void ATU5_TIMERG_Func_Timers::set_str_async(const bool&     isStartChan_preSync  ///< [in] set value
                                           ,const uint8_t&  channel_id           ///< [in] id of the channel to be set count value
                                           )
{
    sc_assert((pChannel0 == this) && (pChannels[0] == this));
    ATU5_TIMERG_Func_Timers* pChan = pChannels[channel_id];
    assert(pChan != NULL);
    pChan->isStartChan_preSync = isStartChan_preSync;

    if (!isStartChan_preSync) {
        pChan->extEventDetectFlag_synced = false;
    }

    if (is_countable_preSync(channel_id)) {
        pChan->startCnt_syncedEvent.notify(to_next_sync_point(pChan->clkDivSel_preSync));
    } else {
        pChan->startCnt_syncedEvent.cancel();
        pChan->setSTR(false);
    }
}
//------------------------------------------------------------------------------
void ATU5_TIMERG_Func_Timers::set_str_mst_async(const bool& isStartMaster) {
    sc_assert((pChannel0 == this) && (pChannels[0] == this));

    for (uint8_t channel_id = 0; channel_id < CHAN_NUM; ++channel_id) {
        ATU5_TIMERG_Func_Timers* pChan = pChannels[channel_id];
        assert(pChan != NULL);
        pChan->isStartMaster_preSync = isStartMaster;

        if (is_countable_preSync(channel_id)) {
            pChan->startCnt_syncedEvent.notify(to_next_sync_point(pChan->clkDivSel_preSync));
        } else {
            pChan->startCnt_syncedEvent.cancel();
            pChan->setSTR(false);
        }
    }
}
//------------------------------------------------------------------------------
/**@brief set value of the internal counter of channel *channel_id*
 * @details
 *     Notice that this function is available for all channels, but from the user
 *     perspective, only channel *0* is accessible. The only way to access into
 *     other channels is to access indirectly via the channel *0*
 * @return none
 */
void ATU5_TIMERG_Func_Timers::set_cnt(const uint32_t& value      ///< [in] set value
                                     ,const uint8_t&  channel_id ///< [in] id of the channel to be set count value
                                     ) {
    sc_assert((pChannel0 == this) && (pChannels[0] == this));
    ATU5_TIMERG_Func_Timers* pChan = pChannels[channel_id];
    assert(pChan != NULL);
    /// set the counter value, (includes the case set initial value), it's
    /// recommended to restart the general counter.
    /// due to the test case in which: set value of counter to what exceeds value
    /// of compare match value, the compare match still happens if we don't
    /// start the counter.
    /// the following flow: get current run status,  set new value, set run
    /// status again is verified.
    pChan->setCNT(value);
    pChan->startCnt_syncedEvent.cancel();
    pChan->setSTR(false);
    if (is_countable_preSync(channel_id)) {
        pChan->startCnt_syncedEvent.notify(to_next_sync_point(pChan->clkDivSel_preSync));
    } else {
        pChan->startCnt_syncedEvent.cancel();
        pChan->setSTR(false);
    }
}
//------------------------------------------------------------------------------
/**@brief get value of the internal counter of channel *channel_id*
 * @details
 *     Notice that this function is available for all channels, but from the user
 *     perspective, only channel *0* is accessible. The only way to access into
 *     other channels is to access indirectly via the channel *0*
 * @param[in] channel_id id of the desired channel
 * @return value of the internal counter of channel *channel_id*
 */
uint32_t ATU5_TIMERG_Func_Timers::get_cnt(const uint8_t& channel_id) {
    sc_assert((pChannel0 == this) && (pChannels[0] == this));
    return pChannels[channel_id]->getCNT();
}
//------------------------------------------------------------------------------
/**@brief set value of constant compare register of channel *channel_id*
 * @details
 *     Notice that this function is available for all channels, but from the user
 *     perspective, only channel *0* is accessible. The only way to access into
 *     other channels is to access indirectly via the channel *0*
 * @param[in] value set value
 * @param[in] channel_id id of the channel to be set count value
 * @return none
 */
void ATU5_TIMERG_Func_Timers::set_cor(const uint32_t& value
                                     ,const uint8_t&  channel_id) {
    sc_assert((pChannel0 == this) && (pChannels[0] == this));
    ATU5_TIMERG_Func_Timers* pChan = pChannels[channel_id];
    assert(pChan != NULL);
    pChan->startCnt_syncedEvent.cancel();
    pChan->setSTR(false);
    pChan->setCOR(value);

    if (is_countable_preSync(channel_id)) {
        pChan->startCnt_syncedEvent.notify(to_next_sync_point(pChan->clkDivSel_preSync));
    } else {
        pChan->startCnt_syncedEvent.cancel();
        pChan->setSTR(false);
    }
}
//------------------------------------------------------------------------------
/**@brief get value of constant compare register of channel *channel_id*
 * @details
 *     Notice that this function is available for all channels, but from the user
 *     perspective, only channel *0* is accessible. The only way to access into
 *     other channels is to access indirectly via the channel *0*
 * @param[in] channel_id id of the desired channel
 * @return value of constant compare register of channel *channel_id*
 */
uint32_t ATU5_TIMERG_Func_Timers::get_cor(const uint8_t& channel_id) {
    sc_assert((pChannel0 == this) && (pChannels[0] == this));

    return pChannels[channel_id]->getCOR();
}
//------------------------------------------------------------------------------
void ATU5_TIMERG_Func_Timers::set_pclk_period(const double& clk_period) {
    sc_assert((pChannel0 == this) && (pChannels[0] == this));

    pClkPeriods_synced[CLK_DIV_NUM] = clk_period;
    pSyncPoints       [CLK_DIV_NUM] = sc_time_stamp();
    pClkDivs_synced   [CLK_DIV_NUM] = 1;

    for (uint8_t clkIdx = 0; clkIdx < CLK_DIV_NUM; ++clkIdx) {
        if (pClkPeriods_synced[CLK_DIV_NUM] <= 0 || pClkPeriods_synced[clkIdx] <= 0) {
            pClkDivs_synced[clkIdx] = 1;
        } else {
            pClkDivs_synced[clkIdx] = uint32_t(floor(0.5 + pClkPeriods_synced[clkIdx] / pClkPeriods_synced[CLK_DIV_NUM]));
        }
    }

    for (uint8_t channel_id = 0; channel_id < CHAN_NUM; ++channel_id) {
        ATU5_TIMERG_Func_Timers* pChan = pChannels[channel_id];
        pChan->setClkCountPeriod(clk_period);

        pChan->startCnt_syncedEvent.cancel();
        pChan->setSTR(false);
        if ((pClkDivs_synced[pChan->clkDivSel_preSync])!=0) { // keeps the old CKS value when new CKS value is 0
            pChan->setCKS(pClkDivs_synced[pChan->clkDivSel_preSync]);
        }

        if (is_countable_preSync(channel_id)) {
            pChan->startCnt_syncedEvent.notify(SC_ZERO_TIME);
        }
    }
}
//------------------------------------------------------------------------------
void ATU5_TIMERG_Func_Timers::set_clk_div_period(const double&  clk_period
                                                ,const uint8_t& clkIdx)
{
    sc_assert((pChannel0 == this) && (pChannels[0] == this));
    sc_assert(clkIdx < CLK_DIV_NUM);

    bool isFreqOrEdgeChanged = (pClkPeriods_synced[clkIdx] >  0) && (clk_period >  0);
    bool isSyncPoint         = is_sync_point(clkIdx);

    #if 0
    cout << endl << "@[" << std::setfill (' ') << std::setw(7) << sc_time_stamp().to_default_time_units() << "]";
    cout << "[" << "change in divided clock period" << "]";
    cout << "[" << "clock index:" << (uint32_t)clkIdx << "]";
    cout << "[" << "channel: all" << "]";
    cout << "[" << "from:"     << pClkPeriods_synced[clkIdx] << "]";
    cout << "[" << "to:"       << clk_period << "]";
    cout << "[" << "clock is " << (isTurnOn             ? "turn on"
                                  :isTurnOff            ? "turn off"
                                  :isFreqOrEdgeChanged  ? "changed in frequency or position of edge"
                                  :                       "fault")
         << "]";
    cout << "[is sync point:" << uint32_t(isSyncPoint) <<"]";
    #endif

    if (clk_period < 0) {
        pClkPeriods_synced[clkIdx] = 0;
    } else {
        pClkPeriods_synced[clkIdx] = clk_period;
    }
    pSyncPoints       [clkIdx] = sc_time_stamp();
    if (pClkPeriods_synced[CLK_DIV_NUM] <= 0 || pClkPeriods_synced[clkIdx] <= 0) {
        pClkDivs_synced   [clkIdx] = 1;
    } else {
        pClkDivs_synced   [clkIdx] = uint32_t(floor(0.5 + pClkPeriods_synced[clkIdx] / pClkPeriods_synced[CLK_DIV_NUM]));
    }

    for (uint8_t channel_id = 0; channel_id < CHAN_NUM; ++channel_id) {
        ATU5_TIMERG_Func_Timers* pChan = pChannels[channel_id];
        assert(pChan != NULL);
        if (pChan->clkDivSel_preSync == clkIdx) {
            uint32_t currentCNT = pChan->getCNT();
            pChan->startCnt_syncedEvent.cancel();
            pChan->setSTR(false);
            if ((pClkDivs_synced[pChan->clkDivSel_preSync]) != 0) { // keeps the old CKS value when new CKS value is 0
                pChan->setCKS(pClkDivs_synced[pChan->clkDivSel_preSync]);
            }

            if (is_countable_preSync(channel_id)) {
                if (isFreqOrEdgeChanged && isSyncPoint) {
                    pChan->setSTR(true);
                    pChan->setCNT(currentCNT);
                }
                pChan->startCnt_syncedEvent.notify(SC_ZERO_TIME);
            }
        } // end-if
    } // end-for
}
//------------------------------------------------------------------------------
/**@brief set clock period of the internal counters of all channels
 * @details
 *     Notice that this function is available for all channels, but from the user
 *     perspective, only channel *0* is accessible. The only way to access into
 *     other channels is to access indirectly via the channel *0*
 * @return none
 */
//------------------------------------------------------------------------------
void ATU5_TIMERG_Func_Timers::set_clk_div_sel(const uint8_t&  iClkDivSel
                                             ,const uint8_t&  channel_id)
{
    sc_assert((pChannel0 == this) && (pChannels[0] == this));
    ATU5_TIMERG_Func_Timers* pChan = pChannels[channel_id];
    assert(pChan != NULL);
    uint8_t validClkDivSel = 0;
    if (iClkDivSel < CLK_DIV_NUM) {
        validClkDivSel = iClkDivSel;
    }

    #if 0
    bool isFreqOrEdgeChanged = (pClkPeriods_synced[pChan->clkDivSel_preSync] >  0) && (pClkPeriods_synced[validClkDivSel] >  0);
    bool isSyncPoint         = is_sync_point(pChan->clkDivSel_preSync);

    cout << endl << "@[" << std::setfill (' ') << std::setw(7) << sc_time_stamp().to_default_time_units() << "]";
    cout << "[" << "change clock selector" << "]";
    cout << "[" << "channel:"  << (uint32_t)channel_id << "]";
    cout << "[" << "from:"     << (uint32_t)pChan->clkDivSel_preSync << "]";
    cout << "[" << "to:"       << (uint32_t)validClkDivSel << "]";
    cout << "[" << "clock is " << (isTurnOn             ? "turn on"
                                  :isTurnOff            ? "turn off"
                                  :isFreqOrEdgeChanged  ? "changed in frequency or position of edge"
                                  :                       "fault")
         << "]";
    cout << "[is sync point:" << uint32_t(isSyncPoint) <<"]";
    #endif

    pChan->clkDivSel_preSync = validClkDivSel;
    pChan->startCnt_syncedEvent.cancel();
    pChan->setSTR(false);
    if ((pClkDivs_synced[pChan->clkDivSel_preSync]) !=0 ) { // keeps the old CKS value when new CKS value is 0
        pChan->setCKS(pClkDivs_synced[pChan->clkDivSel_preSync]);
    }

    if ((is_countable_preSync(channel_id)) != 0) {
        pChan->startCnt_syncedEvent.notify(SC_ZERO_TIME);
    }
}
//------------------------------------------------------------------------------
/**@brief set time unit of all channels
 * @details
 *     Notice that this function is available for all channels, but from the user
 *     perspective, only channel *0* is accessible. The only way to access into
 *     other channels is to access indirectly via the channel *0*
 * @param[in] time_unit time unit
 * @return none
 */
void ATU5_TIMERG_Func_Timers::set_time_unit(const sc_time_unit& time_unit) {
    sc_assert((pChannel0 == this) && (pChannels[0] == this));

    for (uint8_t channel_id = 0; channel_id < CHAN_NUM; ++channel_id) {
        pChannels[channel_id]->setTimeUnit(time_unit);
    }
}
//------------------------------------------------------------------------------
/**@brief get status of compare-match flag of channel *channel_id*
 * @details
 *     Notice that this function is available for all channels, but from the user
 *     perspective, only channel *0* is accessible. The only way to access into
 *     other channels is to access indirectly via the channel *0*
 * @param[in] channel_id id of the desired channel
 * @return none
 */
bool ATU5_TIMERG_Func_Timers::get_cmf (const uint8_t& channel_id) {
    sc_assert((pChannel0 == this) && (pChannels[0] == this));

    return (pChannels[channel_id]->CMF);
}
//------------------------------------------------------------------------------
/**@brief clear status of compare-match flag of channel *channel_id*
 * @details
 *     Notice that this function is available for all channels, but from the user
 *     perspective, only channel *0* is accessible. The only way to access into
 *     other channels is to access indirectly via the channel *0*
 * @param[in] channel_id id of the desired channel
 * @return none
 */
void ATU5_TIMERG_Func_Timers::clear_cmf (const uint8_t& channel_id) {
    sc_assert((pChannel0 == this) && (pChannels[0] == this));
    pChannels[channel_id]->CMF = false;
    pChannels[channel_id]->clearCMF();
    pChannels[channel_id]->isclear_flagCmf = true;	// add for update to version 0.080
    pChannels[channel_id]->mPriorityEvent.notify(SC_ZERO_TIME);	// added for update to version 0.080
}
//------------------------------------------------------------------------------
void ATU5_TIMERG_Func_Timers::set_cmf (const bool& val, const uint8_t& channel_id) {
    sc_assert((pChannel0 == this) && (pChannels[0] == this));
    pChannels[channel_id]->CMF = false;
    pChannels[channel_id]->isset_flagCmf = true; // added for update to version 0.080
    if(val){
       pChannels[channel_id]->mPriorityEvent.notify(SC_ZERO_TIME);  // added for update to version 0.080
    }
}
//------------------------------------------------------------------------------
/**@brief get status of overflow flag of channel *channel_id*
 * @details
 *     Notice that this function is available for all channels, but from the user
 *     perspective, only channel *0* is accessible. The only way to access into
 *     other channels is to access indirectly via the channel *0*
 * @param[in] channel_id id of the desired channel
 * @return none
 */
bool ATU5_TIMERG_Func_Timers::get_ovff (const uint8_t& channel_id) {
    sc_assert((pChannel0 == this) && (pChannels[0] == this));

    return pChannels[channel_id]->OVF;
}
//------------------------------------------------------------------------------
/**@brief clear status of overflow flag of channel *channel_id*
 * @details
 *     Notice that this function is available for all channels, but from the user
 *     perspective, only channel *0* is accessible. The only way to access into
 *     other channels is to access indirectly via the channel *0*
 * @param[in] channel_id id of the desired channel
 * @return none
 */
void ATU5_TIMERG_Func_Timers::clear_ovff (const uint8_t& channel_id) {
    sc_assert((pChannel0 == this) && (pChannels[0] == this));
    pChannels[channel_id]->OVF = false;
    pChannels[channel_id]->isclear_flagOvf = true; // added for update to version 0.080
    pChannels[channel_id]->mPriorityEvent.notify(SC_ZERO_TIME);  // added for update to version 0.080
}
//------------------------------------------------------------------------------
void ATU5_TIMERG_Func_Timers::set_ovff (const bool& val, const uint8_t& channel_id) {
    sc_assert((pChannel0 == this) && (pChannels[0] == this));
    pChannels[channel_id]->OVF = false; 
    pChannels[channel_id]->isset_flagOvf = true; // added for update to version 0.080
    if (val){ 
        pChannels[channel_id]->mPriorityEvent.notify(SC_ZERO_TIME);  // added for update to version 0.080
    }
}
//------------------------------------------------------------------------------
/**@brief set enable condition of reload operation of channel *channel_id*
 * @details
 *     Notice that this function is available for all channels, but from the user
 *     perspective, only channel *0* is accessible. The only way to access into
 *     other channels is to access indirectly via the channel *0*
 * @param[in] channel_id id of the desired channel
 * @return none
 */
void ATU5_TIMERG_Func_Timers::set_rl_en (const bool&    irlen
                                        ,const uint8_t& channel_id) {
    sc_assert((pChannel0 == this) && (pChannels[0] == this));
    pChannels[channel_id]->reloadEn_preSync = irlen;
}
//------------------------------------------------------------------------------
/**@brief set reload value of reload operation of channel *channel_id*
 * @details
 *     Notice that this function is available for all channels, but from the user
 *     perspective, only channel *0* is accessible. The only way to access into
 *     other channels is to access indirectly via the channel *0*
 * @param[in] channel_id id of the desired channel
 * @return none
 */
void ATU5_TIMERG_Func_Timers::set_rl_val (const uint32_t& irl_val
                                         ,const uint8_t&  channel_id) {
    sc_assert((pChannel0 == this) && (pChannels[0] == this));

    pChannels[channel_id]->reloadVal_preSync = irl_val;
}
//------------------------------------------------------------------------------
/**@brief common settings for all channels
 */
void ATU5_TIMERG_Func_Timers::common_settings() {
    sc_assert((pChannel0 == this) && (pChannels[0] == this));
    
    for (uint8_t channel_id = 0; channel_id < CHAN_NUM; ++channel_id) {
        ATU5_TIMERG_Func_Timers* pChan = pChannels[channel_id];
        assert(pChan != NULL);
        //---------------------------------------
        /// -# set count direction
        pChan->setCounterDirection(emCountUp);
        //---------------------------------------
        /// -# initialize counter register (CNT)
        ///    CNT Counter counts internal clock.
        pChan->setCNT((uint32_t)0);
        //---------------------------------------
        /// -# initialize constant register  (COR)
        pChan->setCOR((uint32_t)0xFFFFFFFF);
        //---------------------------------------
        /// -# initialize input capture register (PR)
        pChan->clearPR();
        //---------------------------------------
        /// -# initialize Control register(CR)
        ///    Control register controls selection of input clock into timer counter, and election of the mode etc.
        /// -# field CMIE
        ///    Compare match interrupt enable/disable
        ///    0: Compare match interrupt disable
        ///    1: Compare match interrupt enable
        pChan->setCMIE(true);
        /// -# field OVIE
        ///    Overflow interrupt enable/disable
        ///    0: Overflow interrupt disable
        ///    1: Overflow interrupt enable
        pChan->setOVIE(true);
        /// -# field ICPE
        ///    Input capture interrupt enable/disable
        ///    0: Input capture interrupt disable
        ///    1: Input capture interrupt enable
        pChan->setICPE(true);
        /// -# field UNIE
        ///    Underflow interrupt enable/disable
        ///    0: Underflow interrupt disable
        ///    1: Underflow interrupt enable
        pChan->setUNIE(true);
        /// -# field CKS
        ///    Select input clock
        ///    inner clock = input clock / CKS
        pChan->setCKS(1);
        /// -# field CKEG
        ///    Select counting clock edge
        ///    0: count at rising clock edge
        ///    1: count at falling clock edge
        ///    2: count at both rising and falling clock edge
        pChan->setCKEG(emRising);
        /// -# field STR
        ///    Counter operation
        ///    0: Stop counter
        ///    1: Start counter
        pChan->setSTR(false);
        /// -# field IE
        ///    Input capture control
        ///    0: Disable input capture operation
        ///    1: Enable input capture operation
        pChan->setIE(1);
        /// -# field CCLR
        ///    Counter clear factor
        ///    00: Prohibit
        ///    01: Internal factor
        ///    10: External factor
        pChan->setCCLR(emInFactor); // the timer clears values by itself when compare match occurs
        pChan->setCCLR(emProhibit); // the timer keeps its value when compare match occurs
        /// -# field CMS
        ///    Counter size (unit is bit)
        ///    11: Both
        pChan->setCMS(32);
        /// -# field CMM
        ///    Counter mode
        ///    0: Free run
        ///    1: One shot
        pChan->setCMM(emFreeRun);
        //---------------------------------------
        /// -# initialize status register(SR)
        /// -# field compare match flag
        pChan->clearCMF();
        /// -# field overflow flag
        pChan->clearOVF();
        /// -# field input capture flag
        pChan->clearICF();
        /// -# field underflow flag
        pChan->clearUNF();
        /// -# field the number of compare match gtSR.gtNumOfCMF
        /// -# field the number of overflow gtSR.gtNumOfOVF
        /// -# field the number of underflow gtSR.gtNumOfUNF
        /// -# field the number of input capture gtSR.gtNumOfICF
        pChan->clearAllFlag();

        pChan->enableSupportDecoupling(false);
    }
}
//------------------------------------------------------------------------------
sc_time ATU5_TIMERG_Func_Timers::to_next_sync_point_pclk() {
    sc_assert((pChannel0 == this) && (pChannels[0] == this));
    return(to_next_sync_point(CLK_DIV_NUM));
}
//------------------------------------------------------------------------------
sc_time ATU5_TIMERG_Func_Timers::to_next_sync_point(const uint8_t& clkIdx) {
    sc_assert((pChannel0 == this) && (pChannels[0] == this));
    sc_assert(clkIdx <= CLK_DIV_NUM);

    if (clkIdx > CLK_DIV_NUM) {
        return (SC_ZERO_TIME);
    } else if (pClkPeriods_synced[clkIdx] <= 0) {
        return (SC_ZERO_TIME);
    } else {
        sc_time clk_period(pClkPeriods_synced[clkIdx], getTimeUnit());
        sc_time now(sc_time_stamp());
        double period_num = 1 + floor((now-pSyncPoints[clkIdx]) / clk_period);
        return ((clk_period*period_num) - (now-pSyncPoints[clkIdx]));
    }
}
//------------------------------------------------------------------------------
bool ATU5_TIMERG_Func_Timers::is_sync_point (const uint8_t& clkIdx) {
    sc_assert((pChannel0 == this) && (pChannels[0] == this));
    sc_assert(clkIdx <= CLK_DIV_NUM);

    if (clkIdx > CLK_DIV_NUM) {
        return false;
    } else if (pClkPeriods_synced[clkIdx] <= 0) {
        return false;
    } else {
        sc_time clk_period(pClkPeriods_synced[clkIdx], getTimeUnit());
        sc_time now(sc_time_stamp());

        uint32_t round_period_num = (uint32_t)floor(0.5 + (now - pSyncPoints[clkIdx]) / clk_period);
        uint32_t floor_period_num = (uint32_t)floor(      (now - pSyncPoints[clkIdx]) / clk_period);
        uint32_t ceil_period_num  = (uint32_t)ceil (      (now - pSyncPoints[clkIdx]) / clk_period);

        return    (round_period_num == floor_period_num)
               && (round_period_num == ceil_period_num);
    }
}
//------------------------------------------------------------------------------
/**@brief common settings for all channels
 */
void ATU5_TIMERG_Func_Timers::startCnt_syncedThread(const uint8_t& channel_id) {
    sc_assert((pChannel0 == this) && (pChannels[0] == this));
    ATU5_TIMERG_Func_Timers* pChan = pChannels[channel_id];
    assert(pChan != NULL);
    while (1) {
        wait(pChan->startCnt_syncedEvent);

        #if 0
        cout << endl << "@[" << std::setfill (' ') << std::setw(7) << sc_time_stamp().to_default_time_units() << "]";
        cout << "[" << "start the timer " << (uint32_t)channel_id  << "]";
        cout << "[" << "initial value:"   << pChan->getCNT()       << "]";
        #endif


        #if 0
        cout << endl << "@[" << std::setfill (' ') << std::setw(7) << sc_time_stamp().to_default_time_units() << "]";
        cout << "[" << "before start" << "]";
        cout << "[" << "channel:" << (uint32_t)channel_id << " is runnable" << "]";
        cout << "[" << "reset:" << (uint32_t)pChan->isResetActive_preSync << "]";
        cout << "[" << "master start:" << (uint32_t)isStartChan_preSync << "]";
        cout << "[" << "channel start:" << (uint32_t)pChan->isStartChan_preSync << "]";
        cout << "[" << "selector:" << (uint32_t)pChan->clkDivSel_preSync << "]";
        cout << "[" << "selected divisor:" <<     pClkDivs_synced   [pChan->clkDivSel_preSync]  << "]";
        cout << "[" << "divisor inside:" <<     pChan->getCR().gtCKS << "]";
        cout << "[" << "selected count clock:" << pClkPeriods_synced[pChan->clkDivSel_preSync] << "]";
        cout << "[" << "current count value:" << pChan->getCNT() << "]";
        #endif

        uint32_t currentCNT = pChan->getCNT();
        if (pChan->getCR().gtSTR) {
            // currently,  timer has been started, restart again to synchronize start-time point again
            pChan->setSTR(true);
            pChan->setCNT(currentCNT);
        } else {
            // currently,  timer is stopped, start
            pChan->setSTR(true);
            if (pChan->getCounterDirection() == emCountUp) {
                pChan->setCNT(currentCNT + 1);
            } else {
                pChan->setCNT(currentCNT - 1);
            }
        }

        #if 0
        cout << endl << "@[" << std::setfill (' ') << std::setw(7) << sc_time_stamp().to_default_time_units() << "]";
        cout << "[" << "after start" << "]";
        cout << "[" << "channel:" << (uint32_t)channel_id << " is runnable" << "]";
        cout << "[" << "reset:" << (uint32_t)pChan->isResetActive_preSync << "]";
        cout << "[" << "master start:" << (uint32_t)isStartChan_preSync << "]";
        cout << "[" << "channel start:" << (uint32_t)pChan->isStartChan_preSync << "]";
        cout << "[" << "selector:" << (uint32_t)pChan->clkDivSel_preSync << "]";
        cout << "[" << "selected divisor:" <<     pClkDivs_synced   [pChan->clkDivSel_preSync]  << "]";
        cout << "[" << "divisor inside:" <<     pChan->getCR().gtCKS << "]";
        cout << "[" << "selected count clock:" << pClkPeriods_synced[pChan->clkDivSel_preSync] << "]";
        cout << "[" << "current count value:" << pChan->getCNT() << "]";
        #endif


        if (pChan->OVFCheck && pChan->getCNT() == 0) {
            pChan->OVF = true;
            pChan->OVFCheck = false;
        }

        #if 0
        cout << endl << "@[" << std::setfill (' ') << std::setw(7) << sc_time_stamp().to_default_time_units() << "]";
        cout << "[" << "right after being started the timer " << (uint32_t)channel_id  << "]";
        cout << "[" << "value:"   << pChan->getCNT()       << "]";
        #endif

    }
}
//------------------------------------------------------------------------------
void ATU5_TIMERG_Func_Timers::compareMatch_syncedThread(const uint8_t& channel_id) {
    sc_assert((pChannel0 == this) && (pChannels[0] == this));
    ATU5_TIMERG_Func_Timers* pChan = pChannels[channel_id];
    assert(pChan != NULL);
    while (1) {
        wait(pChan->compareMatch_syncedEvent);

        #if 0
        cout << endl << "@[" << std::setfill (' ') << std::setw(7) << sc_time_stamp().to_default_time_units() << "]";
        cout << "[" << "process compare match event" << "]";
        cout << "[" << "timer" << (uint32_t)channel_id << "]";
        #endif

        #if 0
        cout << endl << "@[" << std::setfill (' ') << std::setw(7) << sc_time_stamp().to_default_time_units() << "]";
        cout << "[" << "process compare match event" << "]";
        cout << "[" << "timer" << (uint32_t)channel_id << "]";
        cout << "[" << "reload enable" << (uint32_t)pChan->reloadEn_preSync << "]";
        cout << "[" << "reload value"  << (uint32_t)pChan->reloadEn_preSync << "]";
        cout << "[" << "CMF:"  << (uint32_t)pChan->getSR().gtCMF << "]";
        #endif

        /// check the synchronized divisor and reset the counter to 1 if
        /// the clock divisor = 1 (count CLK == PCLK)
        if (pChan->getCR().gtCKS == 1) {
            pChan->setCNT(1);
        } else {
            pChan->setCNT(0);
        }
        pChan->startCnt_syncedEvent.notify(to_next_sync_point(pChan->clkDivSel_preSync));

        /// update the reload value
        if (pChan->reloadEn_preSync) {
            set_cor(pChan->reloadVal_preSync, channel_id);
        }
    }
}
//------------------------------------------------------------------------------
void ATU5_TIMERG_Func_Timers::preOverflow_syncedThread(const uint8_t& channel_id) {
    sc_assert((pChannel0 == this) && (pChannels[0] == this));
    ATU5_TIMERG_Func_Timers* pChan = pChannels[channel_id];
    assert(pChan != NULL);
    while (1) {
        wait(pChan->preOverflow_syncedEvent);

        #if 0
        cout << endl << "@[" << std::setfill (' ') << std::setw(7) << sc_time_stamp().to_default_time_units() << "]";
        cout << "[" << "process pre-overflow event" << "]";
        cout << "[" << "timer" << (uint32_t)channel_id << "]";
        cout << "[" << "counter value:" << (uint32_t)pChan->getCNT() << "]";
        #endif
    }
}

void ATU5_TIMERG_Func_Timers::prioritymethod(const uint8_t& channel_id) // Added for update version 0.080
{
     sc_assert((pChannel0 == this) && (pChannels[0] == this));
     ATU5_TIMERG_Func_Timers* pChan = pChannels[channel_id];
     assert(pChan != NULL);
     while (1){
       wait(pChan->mPriorityEvent);
       if (!pChan->isclear_flagOvf && pChan->isset_flagOvf){
	  pChan->OVF = true;
       }
       if (!isclear_flagCmf && isset_flagCmf){
          pChan->CMF = true;
       }
       pChan->isset_flagOvf = false;
       pChan->isclear_flagOvf = false;
       pChan->isset_flagCmf = false;
       pChan->isclear_flagCmf = false; 
    }
}
//------------------------------------------------------------------------------
// EOF
