/*
 * (c) 2011,2012 Renesas Electronics Corporation
 * RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY
 * This program must be used solely for the purpose for which
 * it was furnished by Renesas Electronics Corporation. No part of this
 * program may be reproduced or disclosed to others, in any
 * form, without the prior written permission of Renesas Electronics
 * Corporation.
 */

#pragma once

#include <vector>

class WRegLine;

/*!
  @brief CPUレジスタクラス
 */
class GRegFile {
private:
    PhysAddr m_pc;
    std::vector<RegData>* m_gregs;
    std::vector<WRegLine*>* m_wregs;
    const ContextId m_gr_ctid;
    uint32_t m_wreg_size;

    void CreateWRegArray (uint32_t size);
    void DeleteWRegArray (void);
    void ResetWRegArray  (void);

public:
    void WritePC (PhysAddr pc) { m_pc = pc; }  //! PCは呼出元で最下位が0となるよう注意
    PhysAddr ReadPC (void) const { return m_pc; }
    void Write (CReg regid, RegData data);
    RegData Read (CReg regid) const;
    void WriteWR (uint32_t regid, uint32_t pos, RegData data);
    RegData ReadWR (uint32_t regid, uint32_t pos) const;
    void Reset (void);

    void ConfigWRegArray (uint32_t size);

    GRegFile (ContextId ctid, GRegFile* nc_grf, uint32_t mpx_reg_size);
    ~GRegFile ();
};


class GRegFileSet {
private:
    std::vector<GRegFile*> m_grfs;
    ContextId GetLastId (void) const;
    bool HasContext (ContextId ctxt_id) const;

public:
    void WritePC (ContextId ctxt_id, PhysAddr pc);
    RegData ReadPC (ContextId ctxt_id) const;
    void Write (ContextId ctxt_id, CReg regid, RegData data);
    RegData Read (ContextId ctxt_id, CReg regid) const;
    void Insert (GRegFile* grf);
    GRegFile* GetGrfPtr (ContextId ctxt_id) const;
    void Reset (void);

    explicit GRegFileSet (GRegFile* grf);
    ~GRegFileSet ();
};
