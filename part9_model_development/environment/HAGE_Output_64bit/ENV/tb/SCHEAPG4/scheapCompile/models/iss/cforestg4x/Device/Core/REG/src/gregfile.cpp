/*
 * (c) 2011,2012,2013 Renesas Electronics Corporation
 * RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY
 * This program must be used solely for the purpose for which
 * it was furnished by Renesas Electronics Corporation. No part of this
 * program may be reproduced or disclosed to others, in any
 * form, without the prior written permission of Renesas Electronics
 * Corporation.
 */

#include <functional>
#include <algorithm>
#include <vector>
#include "./config.h"
#include "./forest_common.h"
#include "./gregfile.h"
#include "./wregdata128.h"


void GRegFile::Write (CReg regid, RegData data)
{
    if (likely (regid != R0)) {
        (*m_gregs)[regid] = data;
    }
}


RegData GRegFile::Read (CReg regid) const
{
    return (*m_gregs)[regid];
}


RegData GRegFile::ReadWR (uint32_t regid, uint32_t pos) const
{
    return (*m_wregs)[regid]->GetData (pos);
}


void GRegFile::WriteWR (uint32_t regid, uint32_t pos, RegData data)
{
    (*m_wregs)[regid]->SetData (pos, data);
}


void GRegFile::Reset (void)
{
    m_gregs->assign (GREG_NUM, 0x0);
    ResetWRegArray ();
    // m_pc のReset処理は HTのResetで行う
}


void GRegFile::ConfigWRegArray (uint32_t size)
{
    if (m_wreg_size == size) return;

    m_wreg_size = size;

    DeleteWRegArray ();
    CreateWRegArray (size);
    ResetWRegArray ();
}


void GRegFile::CreateWRegArray (uint32_t size)
{
    for (uint32_t i = 0; i < WREG_NUM; i++) {
        m_wregs->push_back (new WRegLine (size));
    }

    if (size == WRegLine::FXU4_HALF) {
        // WR0-15.w{2,3}とWR16-31.w{0,1}は同一資源
        for (uint32_t i = 0; i < 16; i++) {
            (*m_wregs)[i]->SetPtr (2, (*m_wregs)[i+16]->GetPtr (0));
            (*m_wregs)[i]->SetPtr (3, (*m_wregs)[i+16]->GetPtr (1));
        }
    }
}


void GRegFile::DeleteWRegArray (void)
{
    std::vector<WRegLine*>::iterator ite = m_wregs->begin ();
    while (ite != m_wregs->end ()) {
        delete (*ite);
        ++ ite;
    }
    m_wregs->clear();
}


void GRegFile::ResetWRegArray (void)
{
    std::vector<WRegLine*>::iterator ite = m_wregs->begin ();
    while (ite != m_wregs->end ()) {
        (*ite)->ResetData ();
        ++ ite;
    }
}


GRegFile::GRegFile (ContextId ctid, GRegFile* nc_grf, uint32_t fxu_reg_size)
    : m_pc (0), m_gr_ctid (ctid)
{
    if (ctid == TC0) {
        m_gregs = nc_grf->m_gregs;
        m_wregs = nc_grf->m_wregs;
    } else {
        m_gregs = new std::vector<RegData> (GREG_NUM, 0x0);
        m_wregs = new std::vector<WRegLine*>;
        m_wreg_size = fxu_reg_size;
        CreateWRegArray (m_wreg_size);
        ResetWRegArray ();
    }
}


GRegFile::~GRegFile ()
{
    if (m_gr_ctid != TC0) {
        delete m_gregs;
        DeleteWRegArray ();
        delete m_wregs;
    }
}


void GRegFileSet::WritePC (ContextId ctxt_id, PhysAddr pc)
{
    if (likely (HasContext(ctxt_id) == true)) {
        GetGrfPtr(ctxt_id)->WritePC (pc);
    }
}


PhysAddr GRegFileSet::ReadPC (ContextId ctxt_id) const
{
    if (likely (HasContext(ctxt_id) == true)) {
        return GetGrfPtr(ctxt_id)->ReadPC ();
    } else {
        return 0;
    }
}


void GRegFileSet::Write (ContextId ctxt_id, CReg regid, RegData data)
{
    if (likely (HasContext(ctxt_id) == true)) {
        GetGrfPtr(ctxt_id)->Write (regid, data);
    }
}


RegData GRegFileSet::Read (ContextId ctxt_id, CReg regid) const
{
    if (likely (HasContext(ctxt_id) == true)) {
        return GetGrfPtr(ctxt_id)->Read (regid);
    } else {
        return 0;
    }
}


void GRegFileSet::Insert (GRegFile* grf)
{
    m_grfs.push_back (grf);
}


ContextId GRegFileSet::GetLastId (void) const
{
    //! NCのみ構成での戻り値は -1
    return static_cast<ContextId> (m_grfs.size() - 2);
}


bool GRegFileSet::HasContext (ContextId ctxt_id) const
{
    return (ctxt_id >= NC && ctxt_id <= GetLastId ());
}


GRegFile* GRegFileSet::GetGrfPtr (ContextId ctxt_id) const
{
    //! NC=-1,TC=0 なので添字を補正
    return m_grfs[ctxt_id+1];
}


void GRegFileSet::Reset (void)
{
    std::for_each (m_grfs.begin(), m_grfs.end(), std::mem_fun (&GRegFile::Reset));
}


GRegFileSet::GRegFileSet (GRegFile* grf)
{
    m_grfs.push_back (grf);
}


GRegFileSet::~GRegFileSet ()
{
    for (std::vector<GRegFile*>::iterator it = m_grfs.begin(); it != m_grfs.end(); ++it) {
        delete *it;
    }
}
