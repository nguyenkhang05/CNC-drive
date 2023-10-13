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

#include <vector>
#include "./forest_common.h"

#if _UNIT_TEST
class FsCtrl_Interface;
#else
class FsCtrl;
#endif
class SRegFile;
class NcSRegFile;
class Icache;

class ProcElementBase {
public:
    virtual CoreType GetCoreType (void) const = 0;
    virtual CoreVersion GetCoreVersion (void) const = 0;
#if _UNIT_TEST
    virtual FsCtrl_Interface* GetHtPtr (void) const = 0;
#else
    virtual FsCtrl* GetHtPtr (void) const = 0;
#endif
    virtual SRegFile* GetSrfPtr (ContextId tc_id) const = 0;
    virtual NcSRegFile* GetNcSrfPtr (void) const = 0;
    virtual uint32_t GetExtPin (ExtPinType pin) const = 0;
    virtual bool IsHvEnabled (void) const = 0;
    virtual bool IsHostMode (void) const = 0;
#if _UNIT_TEST
    typedef std::vector<FsCtrl_Interface*> HtVec;
#else
    typedef std::vector<FsCtrl*> HtVec;
#endif
    virtual HtVec* GetHtsPtr (void) = 0;
    virtual bool HasFpu (void) const = 0;
    virtual bool HasFxu (void) const = 0;
    virtual uint32_t GetNumMpuRegion (void) const = 0;
    virtual bool HasMpu (void) const = 0;
#ifdef ENABLE_PEID_POS_CTRL
    virtual bool IsPeidPosG4x (void) const = 0;
#endif // #ifdef ENABLE_PEID_POS_CTRL
    virtual Icache* GetIcPtr (void) const = 0;
    virtual uint16_t GetSnoozePeriod (void) const = 0;
    virtual ArchClass GetArchClass (void) const = 0;

    virtual void UpdateHvStatus (void) = 0;
    virtual void ChangeToHostMode (void) = 0;
    virtual void ChangeToGuestMode (void) = 0;
    virtual void UpdateSideband (void) = 0;
    virtual void UpdateHtActive (void) const = 0;
    //void UpdateRunnableByIntreq (void);
    virtual void UpdatePrivLevel (void) const = 0;
    virtual void ClearBtb (void) const = 0;
    virtual void ReallocMpuRegion (void) = 0;
    virtual void ChangeMpuBank (uint32_t bank) = 0;
    virtual void NoticeCpuGuestModeToIntc (bool is_gm) = 0;
    virtual void NoticeCpuGpidToIntc (uint32_t gpid) = 0;
    virtual ~ProcElementBase () {}
};
