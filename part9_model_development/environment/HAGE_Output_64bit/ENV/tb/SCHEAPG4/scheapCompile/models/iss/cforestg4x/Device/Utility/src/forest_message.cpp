/*
 * (c) 2011,2012,2013 Renesas Electronics Corporation
 * RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY
 * This program must be used solely for the purpose for which
 * it was furnished by Renesas Electronics Corporation. No part of this
 * program may be reproduced or disclosed to others, in any
 * form, without the prior written permission of Renesas Electronics
 * Corporation.
 */

#include <stdio.h>
#include <cstdarg>
#include "./config.h"
#include "./forest_utility.h"
#include "./forest_internals.h"
#include "./forest_message.h"
#include "./trace_info.h"


void CfMsg::DPrint (MessageLevel msglvl, const char* fmt, ...)
{
#if defined(COMP_RUNNER)
#else // defined(COMP_RUNNER)
    if (msglvl == MSG_INF && (p_forest_util->GetDebugMode() & DBG_ON) == DBG_OFF) {
        // debug off 時は INFレベルメッセージを表示しない
        return;
    }
#endif // defined(COMP_RUNNER)

    if (p_internals->GetMessageLevel() >= msglvl) {
        FILE* fp = p_internals->GetFileOut();
        SysTimePrint (fp);
        va_list argptr;
        va_start (argptr, fmt);
        vfprintf (fp, fmt, argptr);
        va_end (argptr);

        fflush (fp);
    }
}


// DPrintML doesn't display systime (doesn't call SysTimePrint()).
// This is used to generate 1 line by multiple calls of DPrint.
void CfMsg::DPrintML (MessageLevel msglvl, const char* fmt, ...)
{
#if defined(COMP_RUNNER)
#else // defined(COMP_RUNNER)
    if (msglvl == MSG_INF && (p_forest_util->GetDebugMode() & DBG_ON) == DBG_OFF) {
        // debug off 時は INFレベルメッセージを表示しない
        return;
    }
#endif // defined(COMP_RUNNER)

    if (p_internals->GetMessageLevel() >= msglvl) {
        FILE* fp = p_internals->GetFileOut();
        // SysTimePrint (fp);
        va_list argptr;
        va_start (argptr, fmt);
        vfprintf (fp, fmt, argptr);
        va_end (argptr);

        fflush (fp);
    }
}


void CfMsg::TPrint (MessageLevel msglvl, const TraceCtrl* tr, const char* fmt, ...)
{
#if defined(COMP_RUNNER)
#else // defined(COMP_RUNNER)
    if (msglvl == MSG_INF) {
        uint32_t  peid = tr->GetPeId ();
        if ((p_internals->GetPeDebugMode (peid) & DBG_ON) == DBG_OFF) {
            // debug off 時は INFレベルメッセージを表示しない
            return;
        }
    }
#endif // defined(COMP_RUNNER)

    if (p_internals->GetMessageLevel() >= msglvl) {
        uint32_t  peid = (tr->GetPeId () & CL_PEID_MASK);
        PrivLevel priv = tr->GetPrivLevel ();
        FILE* fp = p_internals->GetFilePointer (peid);
        SysTimePrint (fp);
#ifdef CLUSTER
        uint32_t  clid = tr->GetClId ();
        fprintf (fp, "C%d:", clid);
#endif // CLUSTER
        fprintf (fp, "P%d:%c: ",
                 peid,
                 GetPrivName (priv)
        );
        va_list argptr;
        va_start (argptr, fmt);
        vfprintf (fp, fmt, argptr);
        va_end (argptr);

        fflush (fp);
    }
}


void CfMsg::CachePrint (const char* fmt, ...)
{
    if ((p_forest_util->GetDebugMode() & DBG_CACHE) != DBG_OFF) {
        FILE* fp = p_internals->GetFileOut();
        SysTimePrint (fp);
        va_list argptr;
        va_start (argptr, fmt);
        vfprintf (fp, fmt, argptr);
        va_end (argptr);

        fflush (fp);
    }
}


void CfMsg::TimerPrint (const char* fmt, ...)
{
    if ((p_forest_util->GetDebugMode() & DBG_TIMER) != DBG_OFF) {
        FILE* fp = p_internals->GetFileOut();
        SysTimePrint (fp);
        va_list argptr;
        va_start (argptr, fmt);
        vfprintf (fp, fmt, argptr);
        va_end (argptr);

        fflush (fp);
    }
}


void CfMsg::IntcPrint (const char* fmt, ...)
{
    if ((p_forest_util->GetDebugMode() & DBG_INTC) != DBG_OFF) {
        FILE* fp = p_internals->GetFileOut();
        SysTimePrint (fp);
        va_list argptr;
        va_start (argptr, fmt);
        vfprintf (fp, fmt, argptr);
        va_end (argptr);

        fflush (fp);
    }
}


void CfMsg::StallPrint (const char* fmt, ...)
{
    if ((p_forest_util->GetDebugMode() & DBG_STALL) != DBG_OFF) {
        FILE* fp = p_internals->GetFileOut();
        SysTimePrint (fp);
        va_list argptr;
        va_start (argptr, fmt);
        vfprintf (fp, fmt, argptr);
        va_end (argptr);

        fflush (fp);
    }
}


void CfMsg::PerfcntPrint (const char* fmt, ...)
{
    if ((p_forest_util->GetDebugMode() & DBG_PERF) != DBG_OFF) {
        FILE* fp = p_internals->GetFileOut();
        SysTimePrint (fp);
        va_list argptr;
        va_start (argptr, fmt);
        vfprintf (fp, fmt, argptr);
        va_end (argptr);

        fflush (fp);
    }
}


void CfMsg::SysTimePrint (FILE* fp)
{
    if (p_internals->IsDispSystime()) {
        fprintf (fp, "%4" PRIu64 " ", p_internals->GetSystemTime());
    }
}
