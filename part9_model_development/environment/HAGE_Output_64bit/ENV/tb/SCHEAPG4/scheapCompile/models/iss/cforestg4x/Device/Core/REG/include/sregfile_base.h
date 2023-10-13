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
class SRegBase;

class SRegFileBase {
protected:
    std::vector<SRegBase*> m_srf;
public:
    bool HasSReg (CReg sregid) const;
    SRegBase* GetSrPtr (CReg sregid) const { return m_srf[sregid]; }
    void SetSrPtr (CReg sregid, SRegBase* sreg_base) { m_srf[sregid] = sreg_base; }
    void Reset (void);

    explicit SRegFileBase (SRegBase* sreg_init) { m_srf.assign (CF_SR_GUARD, sreg_init); }
    ~SRegFileBase () { m_srf.clear(); }
};


template <class TSrf>
class SRegFileSetTmpl {
private:
    std::vector<TSrf*> m_srfs;
public:
    void Insert (TSrf* srf);
    ContextId GetLastId (void) const;
    bool HasContext (ContextId ctxt_id) const;
    TSrf* GetSrfPtr (ContextId ctxt_id) const;
    void Reset (void);
    explicit SRegFileSetTmpl (TSrf* srf);
    ~SRegFileSetTmpl ();
private:
};


class VcSRegFile;
class TcSRegFile;
class SRegFile;
typedef SRegFileSetTmpl<VcSRegFile> VcSRegFileSet;
typedef SRegFileSetTmpl<TcSRegFile> TcSRegFileSet;
typedef SRegFileSetTmpl<SRegFile>   SRegFileSet;
