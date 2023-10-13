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
#include <map>
#include <string>
#include "./forest_common.h"

class SRegFile;
class VcSRegFile;

class RegID {
private:
    // SRegの記録用レジスタ番号をGReg番号と被らないようにする
    static const uint32_t REC_SREG_OFFSET = 0x00000100;

    // 複数コンテキストの記録用レジスタ番号を被らないようにする
    static const uint32_t REC_TC_OFFSET = 0x00010000;
    static const uint32_t REC_VC_OFFSET = 0x00001000;

    typedef std::map<int32_t, CfSReg> CfSRegIdMap;
    static CfSRegIdMap m_cfsrid_map;

    typedef std::vector<CReg> SRegIdVec;
    static SRegIdVec m_sregid_vec;

#ifndef COMP_RUNNER
    static CReg GetSRegId (uint32_t selid, uint32_t regid);
#endif //#if defined(COMP_RUNNER)

    typedef std::map <CReg, std::string> RegNameMap;
    static RegNameMap m_regname_map;
    static void AddName (CReg regid, const std::string regname);

public:
    static CfSReg GetCfSRegId (CReg sregid);
    static CfSReg GetCfSRegIdFromSelId (uint32_t selid, uint32_t regid);
    static void AddSRegIdMap (uint32_t selid, uint32_t regid, CfSReg cfsrid);
    static void InitIdMap (void);
    static CReg GetSRegId (CfSReg cfsrid) { return m_sregid_vec[cfsrid]; }
#if defined(COMP_RUNNER)
    static CReg GetSRegId (uint32_t selid, uint32_t regid);
#endif //#if defined(COMP_RUNNER)
    static bool IsSReg (CReg regid) { return regid >= static_cast<int32_t>(REC_SREG_OFFSET); }
    static uint32_t GetRecTcOffset (ContextId ctxt_id);
    static uint32_t GetRecVcOffset (ContextId ctxt_id);
    static CReg GetRecRegId (CReg regid);
    static ContextId GetRecTcId (CReg regid);

    static std::string GetName (CReg regid);
    static CReg GetId (const std::string regname);
    static void InitRegNameMap (const SRegFile* srf);
};
