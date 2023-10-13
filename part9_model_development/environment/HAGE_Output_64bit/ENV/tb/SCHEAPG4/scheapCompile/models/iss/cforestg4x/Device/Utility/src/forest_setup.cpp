/*
 * (c) 2011,2012,2013 Renesas Electronics Corporation
 * RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY
 * This program must be used solely for the purpose for which
 * it was furnished by Renesas Electronics Corporation. No part of this
 * program may be reproduced or disclosed to others, in any
 * form, without the prior written permission of Renesas Electronics
 * Corporation.
 */

#include <stdlib.h>
#include "./config.h"
#include "./forest_utility.h"
#include "./forest_cmdopt.h"
#include "./micro_arch_db.h"
#include "./forest_setup.h"
#include "./forest_message.h"


ForestSetup::ForestSetup (ForestUtil* forest_util, ForestCmdOpt* cmdopt)
    : m_forest_util (forest_util), m_cmdopt (cmdopt)
{
    m_forest_util->SetAdditonalFetchStage (m_cmdopt->GetAdditonalFetchStage ());
#ifdef ENABLE_PEID_POS_CTRL
    m_forest_util->SetPeidPosG4x (m_cmdopt->IsPeidPosG4x ());
#endif // #ifdef ENABLE_PEID_POS_CTRL
    m_forest_util->SetDcacheHitLatency (m_cmdopt->GetDcacheHitLatency ());
#ifdef USE_DBUF_CMDOPT
    m_forest_util->SetDbufDepth (m_cmdopt->GetDbufDepth ());
    m_forest_util->SetDbufLineShift (m_cmdopt->GetDbufLineShift ());
    m_forest_util->SetDbufFillBurstNum (m_cmdopt->GetDbufFillBurstNum ());
#endif // USE_DBUF_CMDOPT
    m_forest_util->SetDbufPreloadType (m_cmdopt->GetDbufPreloadType ());
#ifdef USE_IC_LINE_SIZE_CMDOPT
    m_forest_util->SetIcLineByte (m_cmdopt->GetIcLineByte ());
#endif //#ifdef USE_IC_LINE_SIZE_CMDOPT
    m_forest_util->SetFlashWidthByte (m_cmdopt->GetFlashWidthByte ());
    m_forest_util->SetRasNum (m_cmdopt->GetRasNum ());
    m_forest_util->SetTso (m_cmdopt->IsTso ());

#ifdef ENABLE_EXE_ORDER_CTRL
    m_forest_util->SetExecOrderOoOG4MH (m_cmdopt->IsExecOrderOoOG4MH ());
#endif // #ifdef ENABLE_EXE_ORDER_CTRL

    m_forest_util->SetWorkPerfInfoWithPerfcnt (m_cmdopt->IsPerfInfoWorkWithPerfcnt ());

#ifdef CLUSTER
    m_forest_util->SetBusTrace (m_cmdopt->GetBusTrace());
#endif // CLUSTER

}


void ForestSetup::DefaultSetup (void)
{
    // スレッド別メッセージ出力を設定 (for CfMsg::TPrint)
    if (m_cmdopt->IsOutMulti ()) {
        if (m_forest_util->OpenSeparatedFileOut (m_cmdopt->GetOutFname ()) == false) {
            exit (EXIT_NORMAL);
        }
    }

    m_forest_util->SetShowStatistics (m_cmdopt->GetShowStatistics ());

    m_forest_util->EnableHaltBreak (m_cmdopt->IsEnabledHaltBreak ());

    m_forest_util->SetMemInit (m_cmdopt->GetMemInit ());
    uint32_t last_peid = m_forest_util->GetPeLastId ();
    for (uint32_t peid = 0; peid <= last_peid; peid++) {
        if (m_forest_util->IsValidPeId (peid)) {
            if (m_cmdopt->GetEnableIC () != ForestCmdOpt::CMD_OPT_NOSEL) {
                if (m_cmdopt->GetEnableIC () == ForestCmdOpt::CMD_OPT_ENABLE) {
                    m_forest_util->SetExtPin (peid, EXT_PIN_ICHEN, 1);
                } else {
                    m_forest_util->SetExtPin (peid, EXT_PIN_ICHEN, 0);
                }
            }
            m_forest_util->SetExtPin (peid, EXT_PIN_SNZ_PERIOD, m_cmdopt->GetSnoozePeriod ());
            m_forest_util->SetExtPin (peid, EXT_PIN_HAS_FPU, m_cmdopt->HasFpu ());
            m_forest_util->SetExtPin (peid, EXT_PIN_HAS_FXU, m_cmdopt->HasFxu());
            m_forest_util->SetExtPin (peid, EXT_PIN_HAS_MPU, m_cmdopt->HasMpu ());
            m_forest_util->SetExtPin (peid, EXT_PIN_HAS_HV, m_cmdopt->HasHv ());
        }
    }
    if (m_cmdopt->IsEnabedStopio () == true) {
        m_forest_util->EnableBreakAccStopio ();
    }
    m_forest_util->SetDebugMode (m_cmdopt->GetDebugMode ());
    m_forest_util->SetBreakMax (m_cmdopt->GetMaxStep ());
    m_forest_util->SetRomWait (m_cmdopt->GetRomWait ());
    m_forest_util->SetDcacheFillLatency (m_cmdopt->GetDcacheFillLatency ());
    m_forest_util->SetFpuLatencyOffset (m_cmdopt->GetFpuLatencyOffset ());
    m_forest_util->SetDivfLatencyOffset (m_cmdopt->GetDivfLatencyOffset ());

    if (m_cmdopt->GetUartOption () == ForestCmdOpt::UART_BOTH_OPT) {
        m_forest_util->SetIsUartInstance (true);
        m_forest_util->SetUartSocketPort (m_cmdopt->GetUartSocketPort ());
        m_forest_util->SetUartChannelNum (m_cmdopt->GetUartChannelNum ());
    }

    if (m_cmdopt->IsEnabledIssTrace () == true) {
        m_forest_util->EnableIssTrace ();
    }

#ifdef CLUSTER
    m_forest_util->SetBusTrace (m_cmdopt->GetBusTrace());
#endif // CLUSTER

    // 周辺モデルインスタンスを生成＆バス結線
    m_forest_util->GenPeripherals ();
}
