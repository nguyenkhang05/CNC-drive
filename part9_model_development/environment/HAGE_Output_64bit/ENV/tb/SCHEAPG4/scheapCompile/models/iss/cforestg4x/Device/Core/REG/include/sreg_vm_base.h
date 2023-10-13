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

#include <string>
#include <sstream>
#include "./forest_common.h"

class ProcElementBase;

/*!
  \@brief System register base class
 */
class SRegBase {
public:
    virtual void Reset (void) = 0;
    virtual void Write (RegData data, PrivLevel priv, ContextId ctxt) = 0;
    virtual RegData Read (PrivLevel priv, ContextId ctxt) const = 0;
    virtual std::string GetName (void) const = 0;
    virtual CfSReg GetCfSRegId (void) const = 0;
    virtual CoproType GetCoproType (void) const = 0;

    virtual void WriteBody (RegData data) = 0;
    virtual RegData ReadBody (void) const = 0;
    virtual bool IsPrivileged (PrivLevel priv) const  = 0;
    virtual bool HasPrivileged (PrivLevel req_priv, PrivLevel context_priv) const = 0;
    virtual bool IsPie (PrivLevel priv) const = 0;
    virtual void PrintPrivWarning (void) const = 0;

    virtual ~SRegBase () {}
};


class SRegBasicImpl : public SRegBase {
public:
    virtual CoproType GetCoproType (void) const { return COPRO_NO; }
    virtual ~SRegBasicImpl () {}

    void WriteBody (RegData data) { Write (data, PRIV_PERMIT_DBHV, NC); }
    RegData ReadBody (void) const { return Read (PRIV_PERMIT_DBHV, NC); }

    bool IsPrivileged (PrivLevel priv) const {
        bool is_priv = HasPrivileged (m_priv, priv);
        if (! is_priv) { PrintPrivWarning(); }
        return is_priv;
    }

    bool HasPrivileged (PrivLevel req_priv, PrivLevel context_priv) const {
        return ((req_priv & context_priv) != PRIV_PERMIT_NONE);
    }

    bool IsPie (PrivLevel priv) const {
        if ((m_pie_priv & priv) == PRIV_PERMIT_NONE) {
            PrintPrivWarning();
            return true;
        } else {
            return false;
        }
    }

protected:
    const ContextId m_sr_ctid;
    ProcElementBase* m_pe;

    SRegBasicImpl (ContextId ctid, PrivLevel priv, PrivLevel pie_priv, ProcElementBase* pe)
        : m_sr_ctid (ctid), m_pe (pe), m_priv (priv), m_pie_priv (pie_priv) {}

private:
    const PrivLevel m_priv;
    const PrivLevel m_pie_priv;

    void PrintPrivWarning (void) const {
#if defined (SUPPORT_CUBESUITE) || defined (SUPPORT_SC_HEAP)
#else // #if defined (SUPPORT_CUBESUITE) || defined (SUPPORT_SC_HEAP)
        CfMsg::DPrint (MSG_INF, "--:--:--: <Warning: plivilege is lacked for %s access>\n",
                       GetName().c_str());
#endif // #if defined (SUPPORT_CUBESUITE) || defined (SUPPORT_SC_HEAP)
    }
};


/*!
  @brief EIPC register class
 */
class SRegBaseEIPC : public SRegBasicImpl {
public:
    virtual void SetEIPC (RegData eipc) = 0;
    virtual RegData GetEIPC (void) const = 0;
    virtual void Reset (void) = 0;
    virtual void Write (RegData data, PrivLevel priv, ContextId ctxt) = 0;
    virtual RegData Read (PrivLevel priv, ContextId ctxt) const = 0;
    virtual std::string GetName (void) const = 0;
    virtual CfSReg GetCfSRegId (void) const = 0;
    virtual ~SRegBaseEIPC() {}
    SRegBaseEIPC (ContextId ctid, PrivLevel priv, PrivLevel priv_pie, ProcElementBase* pe) :
        SRegBasicImpl (ctid, priv, priv_pie, pe) {}
};

/*!
  @brief xxPSW Base register class
 */
class SRegBasePSW : public SRegBasicImpl {
public:
    virtual void SetUM (bool um) = 0;
    virtual void SetEIMASK (RegData eimask) = 0;
    virtual void SetCU (RegData cu) = 0;
    virtual void SetEBV (bool ebv) = 0;
    virtual void SetNP (bool np) = 0;
    virtual void SetEP (bool ep) = 0;
    virtual void SetID (bool id) = 0;
    virtual void SetSAT (bool sat) = 0;
    virtual void SetCY (bool cy) = 0;
    virtual void SetOV (bool ov) = 0;
    virtual void SetS (bool s) = 0;
    virtual void SetZ (bool z) = 0;
    virtual bool GetUM (void) const = 0;
    virtual RegData GetEIMASK (void) const = 0;
    virtual RegData GetCU (void) const = 0;
    virtual bool GetEBV (void) const = 0;
    virtual bool GetNP (void) const = 0;
    virtual bool GetEP (void) const = 0;
    virtual bool GetID (void) const = 0;
    virtual bool GetSAT (void) const = 0;
    virtual bool GetCY (void) const = 0;
    virtual bool GetOV (void) const = 0;
    virtual bool GetS (void) const = 0;
    virtual bool GetZ (void) const = 0;
    virtual void Reset (void) = 0;
    virtual void Write (RegData data, PrivLevel priv, ContextId ctxt) = 0;
    virtual RegData Read (PrivLevel priv, ContextId ctxt) const = 0;
    virtual std::string GetName (void) const = 0;
    virtual CfSReg GetCfSRegId (void) const = 0;
    virtual ~SRegBasePSW () {}
    SRegBasePSW (ContextId ctid, PrivLevel priv, PrivLevel priv_pie, ProcElementBase* pe) :
        SRegBasicImpl (ctid, priv, priv_pie, pe) {}
};


/*!
  @brief FEPC register class
 */
class SRegBaseFEPC : public SRegBasicImpl {
public:
    virtual void SetFEPC (RegData fepc) = 0;
    virtual RegData GetFEPC (void) const = 0;
    virtual void Reset (void) = 0;
    virtual void Write (RegData data, PrivLevel priv, ContextId ctxt) = 0;
    virtual RegData Read (PrivLevel priv, ContextId ctxt) const = 0;
    virtual std::string GetName (void) const = 0;
    virtual CfSReg GetCfSRegId (void) const = 0;
    virtual ~SRegBaseFEPC () {}
    SRegBaseFEPC (ContextId ctid, PrivLevel priv, PrivLevel priv_pie, ProcElementBase* pe) :
        SRegBasicImpl (ctid, priv, priv_pie, pe) {}
};


/*!
  @brief EIIC register class
 */
class SRegBaseEIIC : public SRegBasicImpl {
public:
    virtual void SetEIIC (RegData eiic) = 0;
    virtual RegData GetEIIC (void) const = 0;
    virtual void Reset (void) = 0;
    virtual void Write (RegData data, PrivLevel priv, ContextId ctxt) = 0;
    virtual RegData Read (PrivLevel priv, ContextId ctxt) const = 0;
    virtual std::string GetName (void) const = 0;
    virtual CfSReg GetCfSRegId (void) const = 0;
    virtual ~SRegBaseEIIC () {}
    SRegBaseEIIC (ContextId ctid, PrivLevel priv, PrivLevel priv_pie, ProcElementBase* pe) :
        SRegBasicImpl (ctid, priv, priv_pie, pe) {}
};


/*!
  @brief FEIC register class
 */
class SRegBaseFEIC : public SRegBasicImpl {
public:
    virtual void SetFEIC (RegData eiic) = 0;
    virtual RegData GetFEIC (void) const = 0;
    virtual void Reset (void) = 0;
    virtual void Write (RegData data, PrivLevel priv, ContextId ctxt) = 0;
    virtual RegData Read (PrivLevel priv, ContextId ctxt) const = 0;
    virtual std::string GetName (void) const = 0;
    virtual CfSReg GetCfSRegId (void) const = 0;
    virtual ~SRegBaseFEIC () {}
    SRegBaseFEIC (ContextId ctid, PrivLevel priv, PrivLevel priv_pie, ProcElementBase* pe) :
        SRegBasicImpl (ctid, priv, priv_pie, pe) {}
};

/*!
  @brief SPID register class
 */
class SRegBaseSPID : public SRegBasicImpl {
public:
    virtual void SetSPID (RegData spid) = 0;
    virtual RegData GetSPID (void) const = 0;
    virtual void Reset (void) = 0;
    virtual void Write (RegData data, PrivLevel priv, ContextId ctxt) = 0;
    virtual RegData Read (PrivLevel priv, ContextId ctxt) const = 0;
    virtual std::string GetName (void) const = 0;
    virtual CfSReg GetCfSRegId (void) const = 0;
    virtual ~SRegBaseSPID () {}
    SRegBaseSPID (ContextId ctid, PrivLevel priv, PrivLevel priv_pie, ProcElementBase* pe) :
        SRegBasicImpl (ctid, priv, priv_pie, pe) {}
};


/*!
  @brief SPIDLIST register class
 */
class SRegBaseSPIDLIST : public SRegBasicImpl {
public:
    virtual void SetSPIDLIST (RegData spidlist) = 0;
    virtual RegData GetSPIDLIST (void) const = 0;
    virtual void Reset (void) = 0;
    virtual void Write (RegData data, PrivLevel priv, ContextId ctxt) = 0;
    virtual RegData Read (PrivLevel priv, ContextId ctxt) const = 0;
    virtual std::string GetName (void) const = 0;
    virtual CfSReg GetCfSRegId (void) const = 0;
    virtual ~SRegBaseSPIDLIST () {}
    SRegBaseSPIDLIST (ContextId ctid, PrivLevel priv, PrivLevel priv_pie, ProcElementBase* pe) :
        SRegBasicImpl (ctid, priv, priv_pie, pe) {}
};


/*!
  @brief PEID register class
 */
class SRegBasePEID : public SRegBasicImpl {
public:
    virtual void SetPEID (RegData spid) = 0;
    virtual RegData GetPEID (void) const = 0;
    virtual void Reset (void) = 0;
    virtual void Write (RegData data, PrivLevel priv, ContextId ctxt) = 0;
    virtual RegData Read (PrivLevel priv, ContextId ctxt) const = 0;
    virtual std::string GetName (void) const = 0;
    virtual CfSReg GetCfSRegId (void) const = 0;
    virtual ~SRegBasePEID () {}
    SRegBasePEID (ContextId ctid, PrivLevel priv, PrivLevel priv_pie, ProcElementBase* pe) :
        SRegBasicImpl (ctid, priv, priv_pie, pe) {}
};


/*!
  @brief EBASE register class
 */
class SRegBaseEBASE : public SRegBasicImpl {
public:
    virtual void SetEBASE (RegData ebase) = 0;
    virtual void SetDV (bool dv) = 0;
    virtual void SetRINT (bool rint) = 0;
    virtual RegData GetEBASE (void) const = 0;
    virtual bool GetDV (void) const = 0;
    virtual bool GetRINT (void) const = 0;
    virtual void Reset (void) = 0;
    virtual void Write (RegData data, PrivLevel priv, ContextId ctxt) = 0;
    virtual RegData Read (PrivLevel priv, ContextId ctxt) const = 0;
    virtual std::string GetName (void) const = 0;
    virtual CfSReg GetCfSRegId (void) const = 0;
    virtual ~SRegBaseEBASE () {}
    SRegBaseEBASE (ContextId ctid, PrivLevel priv, PrivLevel priv_pie, ProcElementBase* pe) :
        SRegBasicImpl (ctid, priv, priv_pie, pe) {}
};


/*!
  @brief INTBP register class
 */
class SRegBaseINTBP : public SRegBasicImpl {
public:
    virtual void SetINTBP (RegData intbp) = 0;
    virtual RegData GetINTBP (void) const = 0;
    virtual void Reset (void) = 0;
    virtual void Write (RegData data, PrivLevel priv, ContextId ctxt) = 0;
    virtual RegData Read (PrivLevel priv, ContextId ctxt) const = 0;
    virtual std::string GetName (void) const = 0;
    virtual CfSReg GetCfSRegId (void) const = 0;
    virtual ~SRegBaseINTBP () {}
    SRegBaseINTBP (ContextId ctid, PrivLevel priv, PrivLevel priv_pie, ProcElementBase* pe) :
        SRegBasicImpl (ctid, priv, priv_pie, pe) {}
};


/*!
  @brief INTCFG register class
 */
class SRegBaseINTCFG : public SRegBasicImpl {
public:
    virtual void SetULNR (RegData ulnr) = 0;
    virtual void SetEPL (bool epl) = 0;
    virtual void SetISPC (bool ispc) = 0;
    virtual RegData GetULNR (void) const = 0;
    virtual bool GetEPL (void) const = 0;
    virtual bool GetISPC (void) const = 0;
    virtual void Reset (void) = 0;
    virtual void Write (RegData data, PrivLevel priv, ContextId ctxt) = 0;
    virtual RegData Read (PrivLevel priv, ContextId ctxt) const = 0;
    virtual std::string GetName (void) const = 0;
    virtual CfSReg GetCfSRegId (void) const = 0;
    virtual ~SRegBaseINTCFG () {}
    SRegBaseINTCFG (ContextId ctid, PrivLevel priv, PrivLevel priv_pie, ProcElementBase* pe) :
        SRegBasicImpl (ctid, priv, priv_pie, pe) {}
};


/*!
  @brief PLMR register class
 */
class SRegBasePLMR : public SRegBasicImpl {
public:
    virtual void SetPLM (RegData plm) = 0;
    virtual RegData GetPLM (void) const = 0;
    virtual void Reset (void) = 0;
    virtual void Write (RegData data, PrivLevel priv, ContextId ctxt) = 0;
    virtual RegData Read (PrivLevel priv, ContextId ctxt) const = 0;
    virtual std::string GetName (void) const = 0;
    virtual CfSReg GetCfSRegId (void) const = 0;
    virtual ~SRegBasePLMR () {}
    SRegBasePLMR (ContextId ctid, PrivLevel priv, PrivLevel priv_pie, ProcElementBase* pe) :
        SRegBasicImpl (ctid, priv, priv_pie, pe) {}
};


/*!
  @brief MPCFG register class
 */
class SRegBaseMPCFG : public SRegBasicImpl {
public:
    virtual void SetDMDP (bool dmdp) = 0;
    virtual void SetNBK (RegData nbk) = 0;
    virtual void SetARCH (RegData arch) = 0;
    virtual void SetHBE (RegData hbe) = 0;
    virtual void SetNMPUE (RegData nmpue) = 0;
    virtual bool GetDMDP (void) const = 0;
    virtual RegData GetNBK (void) const = 0;
    virtual RegData GetARCH (void) const = 0;
    virtual RegData GetHBE (void) const = 0;
    virtual RegData GetNMPUE (void) const = 0;
    virtual void Reset (void) = 0;
    virtual void Write (RegData data, PrivLevel priv, ContextId ctxt) = 0;
    virtual RegData Read (PrivLevel priv, ContextId ctxt) const = 0;
    virtual std::string GetName (void) const = 0;
    virtual CfSReg GetCfSRegId (void) const = 0;
    virtual ~SRegBaseMPCFG () {}
    SRegBaseMPCFG (ContextId ctid, PrivLevel priv, PrivLevel priv_pie, ProcElementBase* pe) :
        SRegBasicImpl (ctid, priv, priv_pie, pe) {}
};


/*!
  @brief MPM register class
 */
class SRegBaseMPM : public SRegBasicImpl {
public:
    virtual void SetGMPE (bool gmpe) = 0;
    virtual void SetSVP (bool svp) = 0;
    virtual void SetMPE (bool mpe) = 0;
    virtual bool GetGMPE (void) const = 0;
    virtual bool GetSVP (void) const = 0;
    virtual bool GetMPE (void) const = 0;
    virtual void Reset (void) = 0;
    virtual void Write (RegData data, PrivLevel priv, ContextId ctxt) = 0;
    virtual RegData Read (PrivLevel priv, ContextId ctxt) const = 0;
    virtual std::string GetName (void) const = 0;
    virtual CfSReg GetCfSRegId (void) const = 0;
    virtual ~SRegBaseMPM () {};
    SRegBaseMPM (ContextId ctid, PrivLevel priv, PrivLevel priv_pie, ProcElementBase* pe) :
        SRegBasicImpl (ctid, priv, priv_pie, pe) {}
};


/*!
  @brief EIWR register class
 */
class SRegBaseEIWR : public SRegBasicImpl {
public:
    virtual void SetEIWR (RegData fewr) = 0;
    virtual RegData GetEIWR (void) const = 0;
    virtual void Reset (void) = 0;
    virtual void Write (RegData data, PrivLevel priv, ContextId ctxt) = 0;
    virtual RegData Read (PrivLevel priv, ContextId ctxt) const = 0;
    virtual std::string GetName (void) const = 0;
    virtual CfSReg GetCfSRegId (void) const = 0;
    virtual ~SRegBaseEIWR () {}
    SRegBaseEIWR(ContextId ctid, PrivLevel priv, PrivLevel priv_pie, ProcElementBase* pe) :
        SRegBasicImpl (ctid, priv, priv_pie, pe) {}
};


/*!
  @brief FEWR register class
 */
class SRegBaseFEWR : public SRegBasicImpl {
public:
    virtual void SetFEWR (RegData fewr) = 0;
    virtual RegData GetFEWR (void) const = 0;
    virtual void Reset (void) = 0;
    virtual void Write (RegData data, PrivLevel priv, ContextId ctxt) = 0;
    virtual RegData Read (PrivLevel priv, ContextId ctxt) const = 0;
    virtual std::string GetName (void) const = 0;
    virtual CfSReg GetCfSRegId (void) const = 0;
    virtual ~SRegBaseFEWR () {}
    SRegBaseFEWR(ContextId ctid, PrivLevel priv, PrivLevel priv_pie, ProcElementBase* pe) :
        SRegBasicImpl (ctid, priv, priv_pie, pe) {}
};


/*!
  @brief SVLOCK register base class
 */
class SRegBaseSVLOCK : public SRegBasicImpl {
public:
    virtual void SetSVL(bool svl) = 0;
    virtual bool GetSVL(void) const = 0;
    virtual void Reset(void) = 0;
    virtual void Write(RegData data, PrivLevel priv, ContextId ctxt) = 0;
    virtual RegData Read(PrivLevel priv, ContextId ctxt) const = 0;
    virtual std::string GetName(void) const = 0;
    virtual CfSReg GetCfSRegId(void) const = 0;
    virtual ~SRegBaseSVLOCK () {}
    SRegBaseSVLOCK(ContextId ctid, PrivLevel priv, PrivLevel priv_pie, ProcElementBase* pe) :
        SRegBasicImpl (ctid, priv, priv_pie, pe) {}
};


/*!
  @brief MEA register base class
 */
class SRegBaseMEA : public SRegBasicImpl {
public:
    virtual void SetMEA(RegData mea) = 0;
    virtual RegData GetMEA(void) const = 0;
    virtual void Reset(void) = 0;
    virtual void Write(RegData data, PrivLevel priv, ContextId ctxt) = 0;
    virtual RegData Read(PrivLevel priv, ContextId ctxt) const = 0;
    virtual std::string GetName(void) const = 0;
    virtual CfSReg GetCfSRegId(void) const = 0;
    virtual ~SRegBaseMEA () {}
    SRegBaseMEA(ContextId ctid, PrivLevel priv, PrivLevel priv_pie, ProcElementBase* pe) :
        SRegBasicImpl (ctid, priv, priv_pie, pe) {}
};


/*!
  @brief MEI register base class
 */
class SRegBaseMEI : public SRegBasicImpl {
public:
    virtual void SetLEN(RegData len) = 0;
    virtual void SetREG(RegData reg) = 0;
    virtual void SetDS(RegData ds) = 0;
    virtual void SetU(bool u) = 0;
    virtual void SetITYPE(RegData itype) = 0;
    virtual void SetRW(bool rw) = 0;
    virtual RegData GetLEN(void) const = 0;
    virtual RegData GetREG(void) const = 0;
    virtual RegData GetDS(void) const = 0;
    virtual bool GetU(void) const = 0;
    virtual RegData GetITYPE(void) const = 0;
    virtual bool GetRW(void) const = 0;
    virtual void Reset(void) = 0;
    virtual void Write(RegData data, PrivLevel priv, ContextId ctxt) = 0;
    virtual RegData Read(PrivLevel priv, ContextId ctxt) const = 0;
    virtual std::string GetName(void) const = 0;
    virtual CfSReg GetCfSRegId(void) const = 0;
    virtual ~SRegBaseMEI () {}
    SRegBaseMEI(ContextId ctid, PrivLevel priv, PrivLevel priv_pie, ProcElementBase* pe) :
        SRegBasicImpl (ctid, priv, priv_pie, pe) {}
};
