/*
 * (c) 2011,2012,2013 Renesas Electronics Corporation
 * RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY
 * This program must be used solely for the purpose for which
 * it was furnished by Renesas Electronics Corporation. No part of this
 * program may be reproduced or disclosed to others, in any
 * form, without the prior written permission of Renesas Electronics
 * Corporation.
 */

#pragma once

#include "./forest_common.h"


class TraceCtrl;


class CfMsg {
public:
#if defined(_UNIT_TEST)
    static void DPrint (MessageLevel msglvl, const char* fmt, ...) {};
    static void DPrintML (MessageLevel msglvl, const char* fmt, ...) {};
    static void TPrint (MessageLevel msglvl, const TraceCtrl* tr, const char* fmt, ...) {};
    static void CachePrint (const char* fmt, ...) {};
    static void TimerPrint (const char* fmt, ...) {};
    static void IntcPrint (const char* fmt, ...) {};
    static void StallPrint (const char* fmt, ...) {};
    static void PerfcntPrint (const char* fmt, ...) {};
    static void SysTimePrint (FILE* fp) {};
#else
    static void DPrint (MessageLevel msglvl, const char* fmt, ...);
    static void DPrintML (MessageLevel msglvl, const char* fmt, ...);
    static void TPrint (MessageLevel msglvl, const TraceCtrl* tr, const char* fmt, ...);
    static void CachePrint (const char* fmt, ...);
    static void TimerPrint (const char* fmt, ...);
    static void IntcPrint (const char* fmt, ...);
    static void StallPrint (const char* fmt, ...);
    static void PerfcntPrint (const char* fmt, ...);
    static void SysTimePrint (FILE* fp);
#endif

    static char GetPrivName (PrivLevel priv) {
        char priv_char;
        switch (priv) {
        case PRIV_PERMIT_UM:
        case PRIV_HAS_UM:
            priv_char = 'U';
            break;
        case PRIV_PERMIT_SV:
        case PRIV_HAS_SV:
            priv_char = 'S';
            break;
        case PRIV_PERMIT_HV:
        case PRIV_HAS_HV:
            priv_char = 'H';
            break;
        case PRIV_PERMIT_DBUM:
        case PRIV_HAS_DBUM:
        case PRIV_PERMIT_DBSV:
        case PRIV_HAS_DBSV:
        case PRIV_PERMIT_DBHV:
        //case PRIV_HAS_DBHV: // same value with PRIV_PERMIT_DBHV
            priv_char = 'D';
            break;
        default:
            priv_char = 'X'; // don't come here
            break;
        }
        return priv_char;
    }
};


