/*
 * (c) 2011,2012,2013 Renesas Electronics Corporation
 * RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY
 * This program must be used solely for the purpose for which
 * it was furnished by Renesas Electronics Corporation. No part of this
 * program may be reproduced or disclosed to others, in any
 * form, without the prior written permission of Renesas Electronics
 * Corporation.
 */

#include <algorithm>
#include <vector>
#include <functional>
#include "./config.h"
#include "./forest_common.h"
#include "./sregfile.h"
#include "./proc_element_base.h"


NcSRegFile::NcSRegFile (ContextId ctxt_id, ProcElementBase* pe, SRegBase* sreg_init)
    : SRegFileBase (sreg_init)
{
    // InitSRegs (ctxt_id, pe);
}


NcSRegFile::~NcSRegFile ()
{
    for (CReg srid = CF_SR_DUMMY + 1; srid < CF_SR_GUARD; ++srid) {
        if (HasSReg (srid) == true) {
            delete m_srf[srid];
        }
    }
}


bool SRegFileBase::HasSReg (CReg sregid) const
{
    //! 存在しないレジスタは CF_SR_DUMMYへのポインタで初期化されている
    return (m_srf[sregid] != m_srf[CF_SR_DUMMY]);
}


void SRegFileBase::Reset (void)
{
    std::for_each (m_srf.begin(), m_srf.end(), std::mem_fun (&SRegBase::Reset));
}


void SRegFile::LinkSRegs (SRegFileBase* srf)
{
    for (CReg srid = 0; srid < CF_SR_GUARD; ++srid) {
        if (srf->HasSReg (srid) == true) {
            m_srf[srid] = srf->GetSrPtr (srid);
        }
    }
}


SRegFile::SRegFile (NcSRegFile* nc_srf)
    : SRegFileBase (nc_srf->GetSrPtr (CF_SR_DUMMY))
{
    LinkSRegs (nc_srf);
}


template <class TSrf>
void SRegFileSetTmpl<TSrf>::Insert (TSrf* srf)
{
    m_srfs.push_back (srf);
}


template <class TSrf>
ContextId SRegFileSetTmpl<TSrf>::GetLastId (void) const
{
    //! NCのみ構成での戻り値は -1
    return static_cast<ContextId> (m_srfs.size() - 2);
}


template <class TSrf>
bool SRegFileSetTmpl<TSrf>::HasContext (ContextId ctxt_id) const
{
    return (ctxt_id >= NC && ctxt_id <= GetLastId ());
}


template <class TSrf>
TSrf* SRegFileSetTmpl<TSrf>::GetSrfPtr (ContextId ctxt_id) const
{
    //! NC=-1,TC=0 なので添字を補正
    return m_srfs[ctxt_id+1];
}


template <class TSrf>
void SRegFileSetTmpl<TSrf>::Reset (void)
{
    std::for_each (m_srfs.begin(), m_srfs.end(), std::mem_fun (&SRegFileBase::Reset));
}


template <class TSrf>
SRegFileSetTmpl<TSrf>::SRegFileSetTmpl (TSrf* srf)
{
    m_srfs.push_back (srf);
}


template <class TSrf>
SRegFileSetTmpl<TSrf>::~SRegFileSetTmpl ()
{
    for (typename std::vector<TSrf*>::iterator it = m_srfs.begin(); it != m_srfs.end(); ++it) {
        delete *it;
    }
}


// template explicit instanciation
template void        SRegFileSet  ::Insert (SRegFile*   srf);
template bool        SRegFileSet  ::HasContext (ContextId ctxt_id) const;
template SRegFile*   SRegFileSet  ::GetSrfPtr  (ContextId ctxt_id) const;
template ContextId   SRegFileSet  ::GetLastId  (void) const;
template void        SRegFileSet  ::Reset (void);
template             SRegFileSet  :: SRegFileSetTmpl (SRegFile* srf);
template             SRegFileSet  ::~SRegFileSetTmpl ();
