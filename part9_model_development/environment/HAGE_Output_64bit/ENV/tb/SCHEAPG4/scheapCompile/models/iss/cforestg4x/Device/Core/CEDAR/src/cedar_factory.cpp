/*
 * (c) 2011,2012,2013 Renesas Electronics Corporation
 * RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY
 * This program must be used solely for the purpose for which
 * it was furnished by Renesas Electronics Corporation. No part of this
 * program may be reproduced or disclosed to others, in any
 * form, without the prior written permission of Renesas Electronics
 * Corporation.
 */

#include "./config.h"
#include "./proc_element.h"
#include "./fs_controler.h"
#include "./cedar_factory.h"


ProcElement* CedarFactory::CreatePE (uint32_t peid, uint32_t clid, CoreType core_type, CoreVersion core_version,
        ArchClass arch_class, FsBus* fsbus, Llsc* llsc)
{
    ProcElement* pe = new ProcElement (peid, clid, core_type, core_version, arch_class, fsbus, llsc);
    return pe;
}


FsCtrl* CedarFactory::GetHtPtr (ProcElement* pe)
{
    return pe->GetHtPtr ();
}


void CedarFactory::SetScId (FsCtrl* ht, int32_t sc_id)
{
    ht->SetScId (sc_id);
}


void CedarFactory::DeletePE (ProcElement* pe)
{
    delete pe; //対応するHTも削除される
}
