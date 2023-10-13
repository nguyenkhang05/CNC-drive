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
#include "./fs_controler.h"
#include "./micro_arch_db.h"


class CsIbuf;

class Ccedar : public FsCtrl {
public:
    Ccedar (uint32_t pe_id, ContextId vc_id, ContextId tc_id,
            FsBus* fsbus, Icache* ic, CIntc1* intc1, DbgBreak* dbgbrk, ProcElement* pe,
            GRegFile* grf, SRegFile* srf,
            NcSRegFile* nc_srf,
            GRegFileSet* grfs);
    ~Ccedar();
};
