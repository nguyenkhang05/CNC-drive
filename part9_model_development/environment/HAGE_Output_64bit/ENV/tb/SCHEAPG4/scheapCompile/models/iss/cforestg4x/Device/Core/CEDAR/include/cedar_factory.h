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

class CforestUtility;
class FsCtrl;
class FsBus;
class Llsc;

class CedarFactory {
public:
    static ProcElement* CreatePE (uint32_t pe_id, uint32_t clid, CoreType core_type, CoreVersion core_version,
            ArchClass arch_class, FsBus* fsbus, Llsc* llsc);
    static FsCtrl* GetHtPtr (ProcElement* pe);
    static void SetScId (FsCtrl* ht, int32_t sc_id);
    static void DeletePE (ProcElement* pe);
};
