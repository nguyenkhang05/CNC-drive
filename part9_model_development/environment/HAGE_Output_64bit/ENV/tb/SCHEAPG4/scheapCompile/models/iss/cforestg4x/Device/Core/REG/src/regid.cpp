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
#include <string>
#include <sstream>
#include "./config.h"
#include "./regid.h"
#include "./sregfile.h"


RegID::CfSRegIdMap RegID::m_cfsrid_map;
RegID::SRegIdVec RegID::m_sregid_vec (CF_SR_GUARD);
RegID::RegNameMap RegID::m_regname_map;


CReg RegID::GetSRegId (uint32_t selid, uint32_t regid)
{
    return REC_SREG_OFFSET + (selid << 5) + regid;
}


uint32_t RegID::GetRecTcOffset (ContextId ctxt_id)
{
    // 0:標準アクセス  1:NCアクセス  2以降:TC0,TC1,...,TC15
    return (ctxt_id == NC) ? REC_TC_OFFSET : REC_TC_OFFSET * (ctxt_id + 2);
}


uint32_t RegID::GetRecVcOffset (ContextId ctxt_id)
{
    // 0:標準アクセス  1:NCアクセス  2以降:VC0,VC1,...,VC7
    return (ctxt_id == NC) ? REC_VC_OFFSET : REC_VC_OFFSET * (ctxt_id + 2);
}


CReg RegID::GetRecRegId (CReg regid)
{
    return regid & (REC_VC_OFFSET - 1);
}


ContextId RegID::GetRecTcId (CReg regid)
{
    // 0:標準アクセス  1:NCアクセス  2以降:TC0,TC1,...,TC15
    uint32_t tc_field = regid / REC_TC_OFFSET;
    if (tc_field >= 2) {
        return tc_field - 2;
    } else {
        return NC;
    }
}


CfSReg RegID::GetCfSRegId (CReg sregid)
{
    CfSRegIdMap::const_iterator itr = m_cfsrid_map.find (sregid);
    if (itr != m_cfsrid_map.end()) {
        return itr->second;
    } else {
        // 見つからなかった
        return CF_SR_DUMMY;
    }
}


CfSReg RegID::GetCfSRegIdFromSelId (uint32_t selid, uint32_t regid)
{
    CReg sregid = GetSRegId (selid, regid);
    return GetCfSRegId (sregid);
}


void RegID::AddSRegIdMap (uint32_t selid, uint32_t regid, CfSReg cfsrid)
{
    CReg sregid = GetSRegId (selid, regid);
    m_cfsrid_map.insert (CfSRegIdMap::value_type (sregid, cfsrid));
    m_sregid_vec[cfsrid] = sregid;
}


std::string RegID::GetName (CReg regid)
{
    if (regid < static_cast<int32_t>(REC_VC_OFFSET)) {
        // 通常アクセス
        RegNameMap::const_iterator itr = m_regname_map.find (regid);
        if (itr != m_regname_map.end()) {
            return itr->second;
        } else {
            return "";
        }

    } else {
        // 他コンテキスト資源へのアクセス
        // ctxt_id = 0:自資源 1:NC 2-:vcid/tcid+2
        std::ostringstream ctxt_str;
        if (regid >= static_cast<int32_t>(REC_TC_OFFSET)) {
            uint32_t ctxt_id = regid / REC_TC_OFFSET;
            if (ctxt_id < 2) {
                ctxt_str << "NC:";
            } else {
                ctxt_str << 'T' << (ctxt_id - 2) << ':';
            }
        } else {
            uint32_t ctxt_id = regid / REC_VC_OFFSET;
            if (ctxt_id < 2) {
                ctxt_str << "NC:";
            } else {
                ctxt_str << 'V' << (ctxt_id - 2) << ':';
            }
        }

        CReg masked_regid = regid & (REC_VC_OFFSET - 1);
        RegNameMap::const_iterator itr = m_regname_map.find (masked_regid);
        if (itr != m_regname_map.end()) {
            ctxt_str << itr->second;
            return ctxt_str.str();
        } else {
            return "";
        }
    }
}


CReg RegID::GetId (const std::string regname)
{
    // レジスタ名を小文字に変換してから一致検索
    std::string s0 = regname;
    transform (s0.begin (), s0.end (), s0.begin (), tolower);

    RegNameMap::const_iterator itr = m_regname_map.begin();
    while ( itr != m_regname_map.end() ) {
        std::string s1 = itr->second;
        transform (s1.begin (), s1.end (), s1.begin (), tolower);
        if (s0 == s1) {
            return itr->first;
        }
        ++ itr;
    }

    // 見つからなかった
    return GR_NONE;
}


void RegID::AddName (CReg regid, const std::string regname)
{
    m_regname_map.insert (RegNameMap::value_type (regid, regname));
}


void RegID::InitRegNameMap (const SRegFile* srf)
{
    if (m_regname_map.size() != 0) return;

    AddName (R0,  "r0");
    AddName (R1,  "r1");
    AddName (R2,  "r2");
    AddName (R3,  "r3");
    AddName (R4,  "r4");
    AddName (R5,  "r5");
    AddName (R6,  "r6");
    AddName (R7,  "r7");
    AddName (R8,  "r8");
    AddName (R9,  "r9");
    AddName (R10, "r10");
    AddName (R11, "r11");
    AddName (R12, "r12");
    AddName (R13, "r13");
    AddName (R14, "r14");
    AddName (R15, "r15");
    AddName (R16, "r16");
    AddName (R17, "r17");
    AddName (R18, "r18");
    AddName (R19, "r19");
    AddName (R20, "r20");
    AddName (R21, "r21");
    AddName (R22, "r22");
    AddName (R23, "r23");
    AddName (R24, "r24");
    AddName (R25, "r25");
    AddName (R26, "r26");
    AddName (R27, "r27");
    AddName (R28, "r28");
    AddName (R29, "r29");
    AddName (R30, "r30");
    AddName (R31, "r31");

    AddName (WR0,  "wr0");
    AddName (WR1,  "wr1");
    AddName (WR2,  "wr2");
    AddName (WR3,  "wr3");
    AddName (WR4,  "wr4");
    AddName (WR5,  "wr5");
    AddName (WR6,  "wr6");
    AddName (WR7,  "wr7");
    AddName (WR8,  "wr8");
    AddName (WR9,  "wr9");
    AddName (WR10, "wr10");
    AddName (WR11, "wr11");
    AddName (WR12, "wr12");
    AddName (WR13, "wr13");
    AddName (WR14, "wr14");
    AddName (WR15, "wr15");
    AddName (WR16, "wr16");
    AddName (WR17, "wr17");
    AddName (WR18, "wr18");
    AddName (WR19, "wr19");
    AddName (WR20, "wr20");
    AddName (WR21, "wr21");
    AddName (WR22, "wr22");
    AddName (WR23, "wr23");
    AddName (WR24, "wr24");
    AddName (WR25, "wr25");
    AddName (WR26, "wr26");
    AddName (WR27, "wr27");
    AddName (WR28, "wr28");
    AddName (WR29, "wr29");
    AddName (WR30, "wr30");
    AddName (WR31, "wr31");

    // PC は特別扱い
    AddName (PC,   "pc");

    // PSWビット
    AddName (HMPSW_VM,  "vm");
    AddName (HMPSW_UM,  "um");
    AddName (HMPSW_EIMASK, "eimask");
    AddName (HMPSW_HVC, "hvc");
    AddName (HMPSW_EBV, "ebv");
    AddName (HMPSW_SS,  "ss");
    AddName (HMPSW_CM,  "cm");
    AddName (HMPSW_S,   "s");
    AddName (HMPSW_NP,  "np");
    AddName (HMPSW_EP,  "ep");
    AddName (HMPSW_ID,  "id");
    AddName (HMPSW_SAT, "sat");
    AddName (HMPSW_CY,  "cy");
    AddName (HMPSW_OV,  "ov");
    AddName (HMPSW_S,   "s");
    AddName (HMPSW_Z,   "z");

    AddName (GMPSW_VM,  "vm");
    AddName (GMPSW_UM,  "um");
    AddName (GMPSW_EIMASK, "eimask");
    AddName (GMPSW_HVC, "hvc");
    AddName (GMPSW_EBV, "ebv");
    AddName (GMPSW_SS,  "ss");
    AddName (GMPSW_CM,  "cm");
    AddName (GMPSW_S,   "s");
    AddName (GMPSW_NP,  "np");
    AddName (GMPSW_EP,  "ep");
    AddName (GMPSW_ID,  "id");
    AddName (GMPSW_SAT, "sat");
    AddName (GMPSW_CY,  "cy");
    AddName (GMPSW_OV,  "ov");
    AddName (GMPSW_S,   "s");
    AddName (GMPSW_Z,   "z");

    // システムレジスタ
    for (CReg srid = CF_SR_DUMMY + 1; srid < CF_SR_GUARD; ++srid) {
        if (srf->HasSReg(srid) == true) {
            CReg sregid = GetSRegId (static_cast<CfSReg> (srid));
            AddName (sregid, srf->GetSrPtr(srid)->GetName());
        }
    }
}
