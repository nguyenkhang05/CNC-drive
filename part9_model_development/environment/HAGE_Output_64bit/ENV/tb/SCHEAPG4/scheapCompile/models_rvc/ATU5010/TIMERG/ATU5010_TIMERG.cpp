// -----------------------------------------------------------------------------
// $Id: ATU5010_TIMERG.cpp,v 1.0 621 2016-06-16 10:51:02Z CuongPH8 $
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
#include "ATU5010_TIMERG.h"

//------------------------------------------------------------------------------
/**@brief public constructor
 * Detailed steps as follows:
 */
ATU5010_TIMERG::ATU5010_TIMERG(sc_module_name iname)
: sc_module (iname)
, ATU5010_TIMERG_AgentController ()
, pFuncModel (NULL)
{
    //-------------------------------------
    /// -# Initialize input/output signals
    for(uint8_t evIdx = 0;  evIdx < CONFIG::EVT_NUM; ++evIdx) {
        events[evIdx] = new sc_in<bool>(sc_core::sc_gen_unique_name("ext_evt"));
        sc_assert(events[evIdx] != NULL);
    }
    for(uint8_t irqIdx = 0;  irqIdx < CONFIG::SUBBLOCK_NUM; ++irqIdx) {
        cmi_g[irqIdx] = new sc_out<bool>(sc_core::sc_gen_unique_name("cmi_g"));
        sc_assert(cmi_g[irqIdx] != NULL);
    }

    //-------------------------------------
    /// -# instantiate the function model
    pFuncModel = new ATU5010_TIMERG_Func ((string)iname, this);
    sc_assert(pFuncModel != NULL);

    //-------------------------------------
    /// -# register processes
    // handle events
    for (uint8_t evIdx = 0; evIdx < CONFIG::EVT_NUM; evIdx++) {
        sc_core::sc_spawn_options opt;
        sc_core::sc_spawn(sc_bind(&ATU5010_TIMERG::events_bus_scdmth
                                 ,this
                                 ,evIdx)
                         ,sc_core::sc_gen_unique_name("events_bus_scdmth")
                                                     ,&opt);
    }
    // generate a pulse of interrupt on compare-match event
    for (uint8_t sblkIdx = 0; sblkIdx < CONFIG::SUBBLOCK_NUM; sblkIdx++) {
        sc_core::sc_spawn_options opt;
        sc_core::sc_spawn(sc_bind(&ATU5010_TIMERG::cmi_g_scdmth
                                 ,this
                                 ,sblkIdx)
                         ,sc_core::sc_gen_unique_name("cmi_g_scdmth")
                                                     ,&opt);
    }

    Initialize ();
}
//------------------------------------------------------------------------------
/**@brief public destructor
 * Detailed steps as follows:
 */
ATU5010_TIMERG::~ATU5010_TIMERG() {
    if (pFuncModel != NULL)
        delete pFuncModel;

    for(uint8_t evIdx = 0;  evIdx < CONFIG::EVT_NUM; ++evIdx) {
        if (events[evIdx] != NULL)
            delete events[evIdx];
    }

    for(uint8_t irqIdx = 0;  irqIdx < CONFIG::SUBBLOCK_NUM; ++irqIdx) {
        if (cmi_g[irqIdx] != NULL)
            delete cmi_g[irqIdx];
    }
}
//------------------------------------------------------------------------------
/**@brief the method handles PCLK
 */
void ATU5010_TIMERG::setPCLKPeriod (const double& tperiod) {
    pFuncModel->setPCLKPeriod(tperiod);
}
//------------------------------------------------------------------------------
/**@brief the method handles PCLK
 */
void ATU5010_TIMERG::setTimeUnit (const sc_time_unit& tunit) {
    pFuncModel->multi_timer.set_time_unit(tunit);
}
//------------------------------------------------------------------------------
/**@brief the dynamic method create handling input events
 * @param event_idx index of desired event
 */
void ATU5010_TIMERG::setClkBusPeriod (const double& tperiod, const uint8_t& clkIdx) {
    /// save input values
    pFuncModel->mClkBusPeriods[clkIdx] = tperiod;

    /// check start & stop condition
    if (!pFuncModel->InResetState() && !pFuncModel->InNoClkState()) {
        pFuncModel->multi_timer.set_clk_div_period(tperiod, clkIdx);
    }
}
//------------------------------------------------------------------------------
/**@brief the dynamic method create handling input events
 * @param event_idx index of desired event
 */
void ATU5010_TIMERG::events_bus_scdmth(const uint8_t& evIdx) {
    while (1) {
        wait(events[evIdx]->value_changed_event());

        #if 0
        cout << endl << "@[" << std::setfill (' ') << std::setw(7) << sc_time_stamp().to_default_time_units() << "]";
        cout << "[" << "input event" << "]";
        cout << "[" << "id: "    << (uint32_t)evIdx << "]";
        cout << "[" << "value: " << (uint32_t)events[evIdx]->read() << "]";
        cout << endl;
        #endif

        pFuncModel->multi_timer.set_ext_event_synced(events[evIdx]->read()
                                                    ,evIdx);
    }
}
//------------------------------------------------------------------------------
/**@brief the dynamic method creates a pulse of interrupt request on output
 *        signal cmi_g #sblk_id
 * @param sblk_id id of desired sub-block
 */
void ATU5010_TIMERG::cmi_g_scdmth(const uint8_t& sblk_id) {
    while (1) {
        wait(*(pFuncModel->multi_timer.pCompareMatchEvents_synced[sblk_id]));
        if (pFuncModel->isEnabledInterrupt(sblk_id)) {
            cmi_g[sblk_id]->write(true);
            wait(pFuncModel->multi_timer.to_next_sync_point_pclk());
            cmi_g[sblk_id]->write(false);
        }
    }
}
//------------------------------------------------------------------------------
/**@brief asserting reset
 * @note this replace the use of reset port
 * @param is_active
 */
void ATU5010_TIMERG::EnableReset (const bool is_active) {
    /// this class process cancel all events and initialize state when it's change
    /// into reset state from other states
    /// and then, change into reset state
    if (is_active && !pFuncModel->InResetState()) {
        CancelEvents();
        Initialize();
    }

    /// this class passes reset request to its sub instances
    pFuncModel->EnableReset(is_active);
}
//------------------------------------------------------------------------------
/**@brief initialize output ports, internal values  in this module
 *        when (ATU5010 top wrapper) asserting reset
 * @note this replace the use of reset port
 * @param is_active
 */
void ATU5010_TIMERG::Initialize () {
    /// reset internal status attributes

    /// reset output ports
    for (uint8_t irqIdx = 0; irqIdx < CONFIG::SUBBLOCK_NUM; ++irqIdx) {
        cmi_g[irqIdx]->initialize(false);
    }
}
//------------------------------------------------------------------------------
/**@brief cancel all events in this module when setting clock frequencies
 *        or asserting reset (ATU5010 top wrapper)
 * @note this replace the use of reset port
 * @param is_active
 */
void ATU5010_TIMERG::CancelEvents () {}
//------------------------------------------------------------------------------
/// @todo comment me
void ATU5010_TIMERG::EnableTIG(const bool& is_enable) {
    /// save input
    pFuncModel->multi_timer.set_str_mst_async(is_enable);

    if (!pFuncModel->InResetState() && !pFuncModel->InNoClkState()) {
        pFuncModel->state = CONFIG::OPERATING;
    }
}

//------------------------------------------------------------------------------
// EOF
