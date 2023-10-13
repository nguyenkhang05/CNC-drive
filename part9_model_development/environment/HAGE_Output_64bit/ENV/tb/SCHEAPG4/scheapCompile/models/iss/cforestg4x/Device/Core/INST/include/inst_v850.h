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
#include "./sregfile.h"
#include "./fpu_device.h"
#include "./vregdata64.h"

#include "./SoftFloatAdapter.h"

class TraceCtrl;
class GRegFile;
class Lsu;
class Ccedar;
class GRegFileSet;
class Icache;
class Btb;
class FsCtrl;
class WRegData64;
class WRegData128;


class InstV850 {
public:
    void InstExec (int32_t inst_id, int64_t inst_code) {
        (this->*m_instfunc[inst_id]) (inst_code);
    }

    InstV850 (TraceCtrl* trace, GRegFile* gr, SRegFile* sr,
              Lsu* lsu, Btb* btb, NcSRegFile* nc_srf,
              GRegFileSet* grfs, Icache* ic, FsCtrl* fsctrl,
              ArchClass arch_class);
    ~InstV850 ();

    // @@ 他の場所に移したほうがよい
    void ExceptionEI (uint32_t exp_cause, PhysAddr ret_pc,
                      uint32_t hndl_ofst, bool is_excp, bool is_table);
    void ExceptionFE (uint32_t exp_cause, PhysAddr ret_pc,
                      uint32_t hndl_ofst, bool is_excp);
    void ExceptionDB (uint32_t exp_cause, PhysAddr ret_pc,
                      uint32_t hndl_ofst, bool is_excp, uint32_t exp_code);

    void SetTraceCtrl (TraceCtrl* trace);
    void UpdatePswReg (void);

#ifdef SUPPORT_SC_HEAP
    uint32_t m_rmw_buf;
    uint32_t GetReadValRMW (void) const { return m_rmw_buf; }

    void Inst_CAXI_MW (uint64_t opcode, uint32_t read_data);
    void Inst_CLR1_B3_MW (uint64_t opcode, uint32_t read_data);
    void Inst_CLR1_MW (uint64_t opcode, uint32_t read_data);
    void Inst_TST1_B3_MW (uint64_t opcode, uint32_t read_data);
    void Inst_TST1_MW (uint64_t opcode, uint32_t read_data);
    void Inst_SET1_B3_MW (uint64_t opcode, uint32_t read_data);
    void Inst_SET1_MW (uint64_t opcode, uint32_t read_data);
    void Inst_NOT1_B3_MW (uint64_t opcode, uint32_t read_data);
    void Inst_NOT1_MW (uint64_t opcode, uint32_t read_data);
#endif // SUPPORT_SC_HEAP

    static const uint32_t VAL_CC4_BR = 0x5;

private:
    static const uint32_t R_SP = R3;
    static const uint32_t R_EP = R30;
    static const MemData DUMMY_DATA = 0x0UL;

#if defined(COMP_RUNNER)
    static const int32_t NUM_MAC_COMMIT = 3;
    static const int32_t NUM_SWITCH_PRE_LSAB = 2;
    static const int32_t NUM_SWITCH_POST_LSAB = 3;
    static const int32_t NUM_CALL_PRE_LSAB = 1;
    static const int32_t NUM_CALL_POST_LSAB = 2;
#endif // #if defined(COMP_RUNNER)

    enum MeiType {
        MEI_NOEIINT = 0,
        MEI_EIINT_TABLE = 1,
        MEI_EIINT_REGBANK = 2
    };

    enum MemAccessSign {
        MA_SIGN   = 0,
        MA_UNSIGN = 1,
    };

    TraceCtrl* m_trace;
    GRegFile*  m_grf;
    SRegFile*  m_srf;
    Lsu*       m_lsu;
    Btb*       m_btb;
    FpuOpeDevice*  m_fpu_ope;
    FpSimdOpeDevice*  m_fpsimd_ope;
    NcSRegFile*  m_nc_srf;
    GRegFileSet* m_grfs;
    Icache* m_ic;
    FsCtrl* m_fsctrl;

    SRegBasePSW*  m_psw;
    SRegFPSR* m_fpsr;
    SRegFXSR* m_fxsr;

#include "./inst_id_list.h"
#include "./inst_declaration.h"

    typedef void (InstV850::*INSTP) (int64_t opcode);
    INSTP m_instfunc[TOTAL_INST_NUM+1];

    void SetFuncTable (void);

    inline uint32_t BitExtract (uint32_t x, uint32_t l, uint32_t r) const {
        return ((x >> r) & ~(~0x0U << (l - r + 1)));
    }

    inline uint32_t OperandExtract (uint64_t x, uint32_t l, uint32_t r) const {
        return static_cast<uint32_t> ((x >> r) & ~(~0x0ULL << (l - r + 1)));
    }

    inline uint32_t SignExtend (uint32_t val, uint32_t width) const {
        uint32_t msb = 0x01U << (width - 1);
        return ((val ^ msb) - msb);
    }

    inline bool IsCarryAdd (uint32_t x, uint32_t y, uint32_t z) const {
        uint32_t both_minus = (x & y);  // negative + negative -> carry : 負数同士の加算 -> carry
        uint32_t ovflw = (x ^ y) & ~z;  // different-signs & result_is_positive -> carry : 異なる符号 & 結果が正 -> carry
        return (((both_minus | ovflw) >> 31) & 0x01U);
    }

    inline bool IsCarrySub (uint32_t x, uint32_t y) const {
        return (x < y);
    }

    template<class T> inline bool IsOvflwAddPlus (T x, T y, T z) const {
        // Positive+Positive=Negative -> overflow : 正+正=負 -> overflow
        return (( (~x & ~y &  z) >> (sizeof (z) * 8 - 1)) & 0x01U);
    }

    template<class T> inline bool IsOvflwAddMinus (T x, T y, T z) const {
        // Negative+Negative=Positive -> overflow : 負+負=正 -> overflow
        return (( ( x &  y & ~z) >> (sizeof (z) * 8 - 1)) & 0x01U);
    }

    template<class T> inline bool IsOvflwSubPlus (T x, T y, T z) const {
        // Positive-Negative=Negative -> overflow : 正-負=負 -> overflow
        return (( (~x &  y &  z) >> (sizeof (z) * 8 - 1)) & 0x01U);
    }

    template<class T> inline bool IsOvflwSubMinus (T x, T y, T z) const {
        // Negative-Positive=Positive -> overflow : 負-正=正 -> overflow
        return (( ( x & ~y & ~z) >> (sizeof (z) * 8 - 1)) & 0x01U);
    }

    template<class T> inline bool IsOvflwAdd (T x, T y, T z) const {
        return (IsOvflwAddPlus (x, y, z) | IsOvflwAddMinus (x, y, z));
    }

    template<class T> inline bool IsOvflwSub (T x, T y, T z) const {
        return (IsOvflwSubPlus (x, y, z) | IsOvflwSubMinus (x, y, z));
    }

    template<class TInt>
    void ExecDIV (uint64_t opcode, bool divh_flag, bool op2_flag);

    bool IsPswCondMatch (uint32_t cc4);
    void SaturateAdd (uint32_t x, uint32_t y, uint32_t dst_reg);
    void SaturateSub (uint32_t x, uint32_t y, uint32_t dst_reg);
    typedef bool PrepResult;
    static const PrepResult PREP_OK = true;
    static const PrepResult PREP_NG = false;
    PrepResult PrepareStack (uint32_t list12, uint32_t imm5);
    RegData DisposeStack (uint32_t list12, uint32_t imm5, bool *exception);
    void Inst_BINS_Common (uint32_t reg1, uint32_t reg2,
                           uint32_t msb, uint32_t lsb);

    uint32_t MTypeAddressing (uint32_t reg1, uint32_t reg2, uint32_t* next_addr);
    uint32_t RTypeAddressing (uint32_t reg1, uint32_t reg2, uint32_t *next_v2);

    bool IsCoproUuusableException (CfSReg sregno) const;
    bool IsPrivUserException (CfSReg sregno, PrivLevel priv) const;
    bool IsMemExceptionIcLine (PhysAddr addr) const;

    bool IsFpuExceptionPermitted (uint32_t exp_code) const;
    bool IsFpSimdExceptionPermitted (uint32_t exp_code) const;
    bool IsHostTransitionException (uint32_t exp_code) const;

    void CauseFpuSingleException ();
    void CauseFpuDoubleException ();
    void CauseFpSimdException ();
    void SetMemErrorInfo (PhysAddr addr, CReg regid, ByteSize size,
                          MemAccessSign sign, MemAccessCommand cmd, MeiType mei_type = MEI_NOEIINT);

    void SetFpsrXC (uint32_t exp_code);
    void SetFpsrXP (uint32_t exp_code);
    void SetFxsrXC (uint32_t exp_code);
    void SetFxsrXP (uint32_t exp_code);
    void SetFxxc (uint32_t way, uint32_t exp_code);
    void SetFxxp (uint32_t way, uint32_t exp_code);

    void SetPswh (uint32_t pswh);

    void SetPswUM  (bool data);
    void SetPswEBV (bool data);
    void SetPswNP  (bool data);
    void SetPswEP  (bool data);
    void SetPswID  (bool data);
    void SetPswSAT (bool data);
    void SetPswCY  (bool data);
    void SetPswOV  (bool data);
    void SetPswS   (bool data);
    void SetPswZ   (bool data);
    void SetPswEIMASK (uint32_t data);
    bool GetPswSAT (void) const;
    bool GetPswCY  (void) const;
    bool GetPswOV  (void) const;
    bool GetPswS   (void) const;
    bool GetPswZ   (void) const;

    void GRegWritePC (PhysAddr pc);
    PhysAddr GRegReadPC (void) const;
    void TcGRegWritePC (ContextId tcid, PhysAddr pc);
    PhysAddr TcGRegReadPC (ContextId tcid) const;

    void GRegWrite   (uint32_t regno, RegData   data);
    void GRegWrite64 (uint32_t regno, RegData64 data64);
    void TcGRegWrite (ContextId tcid, uint32_t regno, RegData data);
    RegData   GRegRead   (uint32_t regno) const;
    RegData64 GRegRead64 (uint32_t regno) const;
    RegData   TcGRegRead (ContextId tcid, uint32_t regno) const;
    void WRegWriteBody    (uint32_t regno, uint32_t pos, RegData data);
    void WRegWrite32 (uint32_t regno, uint32_t pos, RegData data);
    void WRegWrite64L (uint32_t regno, RegData64 data);
    void WRegWrite64H (uint32_t regno, RegData64 data);
    void WRegWrite128DW (uint32_t regno, RegData64 dw1, RegData64 dw0);
    void WRegWrite128W (uint32_t regno, RegData w3, RegData w2, RegData w1, RegData w0);

    RegData   WRegReadBody (uint32_t regno, uint32_t pos) const;
    RegData WRegRead32 (uint32_t regno, uint32_t pos) const;
    RegData64 WRegRead64L (uint32_t regno) const;
    RegData64 WRegRead64H (uint32_t regno) const;
    void WRegRead128DW (uint32_t regno, RegData64* dw1, RegData64* dw0) const;
    void WRegRead128W (uint32_t regno, RegData* w3, RegData* w2, RegData* w1, RegData* w0) const;

    void SRegWrite (CfSReg sregno, RegData data);
    void SRegWriteWithSel (uint32_t selid, uint32_t regid, RegData data);

    RegData SRegRead (CfSReg sregno) const;
    RegData SRegReadWithSel (uint32_t selid, uint32_t regid) const;

    void CacheCHBII (uint32_t data);
    void CacheCIBII (uint32_t data);
    void CacheCFALI (uint32_t data);
    void CacheCISTI (uint32_t data);
    void CacheCILDI (uint32_t data);
    void CacheCLL   (void);

    enum ClipSignType {
        CLIP_SIGN = true,
        CLIP_UNSIGN = false,
    };

    bool UpdateExpRegisterFXU (uint32_t total_exp_code, std::vector<uint32_t> *exp_code_way);

    template <ByteSize size, ClipSignType is_sign>
    RegData ClipCommon (RegData data);

    template <class TVRegData>
    TVRegData CMPFS (const TVRegData &a, const TVRegData &b, uint32_t fcond, bool *is_permitted);
    template <class TVRegData>
    TVRegData ROUNDFSW (const TVRegData &a, bool *is_permitted);
    template <class TVRegData>
    TVRegData TRNCFSW (const TVRegData &a, bool *is_permitted);
    template <class TVRegData>
    TVRegData CEILFSW (const TVRegData &a, bool *is_permitted);
    template <class TVRegData>
    TVRegData FLOORFSW (const TVRegData &a, bool *is_permitted);
    template <class TVRegData>
    TVRegData CVTFSW (const TVRegData &a, bool *is_permitted);
    template <class TVRegData>
    TVRegData ROUNDFSUW (const TVRegData &a, bool *is_permitted);
    template <class TVRegData>
    TVRegData TRNCFSUW (const TVRegData &a, bool *is_permitted);
    template <class TVRegData>
    TVRegData CEILFSUW (const TVRegData &a, bool *is_permitted);
    template <class TVRegData>
    TVRegData FLOORFSUW (const TVRegData &a, bool *is_permitted);
    template <class TVRegData>
    TVRegData CVTFSUW (const TVRegData &a, bool *is_permitted);
    template <class TVRegData>
    TVRegData CVTFWS (const TVRegData &a, bool *is_permitted);
    template <class TVRegData>
    TVRegData CVTFUWS (const TVRegData &a, bool *is_permitted);
    template <class TVRegData>
    TVRegData CVTFHS (const TVRegData &a, bool *is_permitted);
    template <class TVRegData>
    TVRegData CVTFSH (const TVRegData &a, bool *is_permitted);
    template <class TVRegData>
    TVRegData ABSFS (const TVRegData &a);
    template <class TVRegData>
    TVRegData NEGFS (const TVRegData &a);
    template <class TVRegData>
    TVRegData SQRTFS (const TVRegData &a, bool *is_permitted);
    template <class TVRegData>
    TVRegData RECIPFS (const TVRegData &a, bool *is_permitted);
    template <class TVRegData>
    TVRegData RSQRTFS (const TVRegData &a, bool *is_permitted);
    template <class TVRegData>
    TVRegData ADDFS (const TVRegData &a, const TVRegData &b, bool *is_permitted);
    template <class TVRegData>
    TVRegData SUBFS (const TVRegData &a, const TVRegData &b, bool *is_permitted);
    template <class TVRegData>
    TVRegData MULFS (const TVRegData &a, const TVRegData &b, bool *is_permitted);
    template <class TVRegData>
    TVRegData MAXFS (const TVRegData &a, const TVRegData &b, bool *is_permitted);
    template <class TVRegData>
    TVRegData MINFS (const TVRegData &a, const TVRegData &b, bool *is_permitted);
    template <class TVRegData>
    TVRegData DIVFS (const TVRegData &a, const TVRegData &b, bool *is_permitted);
    template <class TVRegData>
    TVRegData FMAFS (const TVRegData &a, const TVRegData &b, const TVRegData &c, bool *is_permitted);
    template <class TVRegData>
    TVRegData FMSFS (const TVRegData &a, const TVRegData &b, const TVRegData &c, bool *is_permitted);
    template <class TVRegData>
    TVRegData FNMAFS (const TVRegData &a, const TVRegData &b, const TVRegData &c, bool *is_permitted);
    template <class TVRegData>
    TVRegData FNMSFS (const TVRegData &a, const TVRegData &b, const TVRegData &c, bool *is_permitted);
    template <class TVRegData>
    TVRegData CMOVW (const TVRegData &a, const TVRegData &b, const TVRegData &c);
    template <class TVRegData>
    TVRegData FLPVS (const TVRegData &a, uint32_t imm);
    template <class TVRegData>
    TVRegData ADDRFS (const TVRegData &a, const TVRegData &b, bool *is_permitted);
    template <class TVRegData>
    TVRegData SUBRFS (const TVRegData &a, const TVRegData &b, bool *is_permitted);
    template <class TVRegData>
    TVRegData MULRFS (const TVRegData &a, const TVRegData &b, bool *is_permitted);
    template <class TVRegData>
    TVRegData MAXRFS (const TVRegData &a, const TVRegData &b, bool *is_permitted);
    template <class TVRegData>
    TVRegData MINRFS (const TVRegData &a, const TVRegData &b, bool *is_permitted);
    template <class TVRegData>
    TVRegData ADDXFS (const TVRegData &a, const TVRegData &b, bool *is_permitted);
    template <class TVRegData>
    TVRegData SUBXFS (const TVRegData &a, const TVRegData &b, bool *is_permitted);
    template <class TVRegData>
    TVRegData MULXFS (const TVRegData &a, const TVRegData &b, bool *is_permitted);
    template <class TVRegData>
    TVRegData ADDSUBFS (const TVRegData &a, const TVRegData &b, bool *is_permitted);
    template <class TVRegData>
    TVRegData SUBADDFS (const TVRegData &a, const TVRegData &b, bool *is_permitted);
    template <class TVRegData>
    TVRegData ADDSUBXFS (const TVRegData &a, const TVRegData &b, bool *is_permitted);
    template <class TVRegData>
    TVRegData SUBADDXFS (const TVRegData &a, const TVRegData &b, bool *is_permitted);
    template <class TVRegData>
    TVRegData ADDSUBNFS (const TVRegData &a, const TVRegData &b, bool *is_permitted);
    template <class TVRegData>
    TVRegData SUBADDNFS (const TVRegData &a, const TVRegData &b, bool *is_permitted);
    template <class TVRegData>
    TVRegData ADDSUBNXFS (const TVRegData &a, const TVRegData &b, bool *is_permitted);
    template <class TVRegData>
    TVRegData SUBADDNXFS (const TVRegData &a, const TVRegData &b, bool *is_permitted);
    uint32_t SHFLVW (uint32_t sel, const WRegData64 &a, const WRegData64 &b);
    uint32_t SHFLVW (uint32_t sel, const WRegData128 &a, const WRegData128 &b);

    bool PushRegBody (PhysAddr addr, RegData date);
    bool PushRegBank (uint32_t bank_pos, RegData psw_val);
    bool PopGRegBody (PhysAddr addr, CReg reg);
    bool PopSRegBody (PhysAddr addr, CfSReg reg, bool is_update_sreg);
    bool PopRegBank (uint32_t bank_pos);

    bool HasPrivileged (PrivLevel req_priv, PrivLevel context_priv) const {
        return ((req_priv & context_priv) != PRIV_PERMIT_NONE);
    }

    bool IsPieException (PrivLevel inst_priv) const;

    std::string GetPrivName (PrivLevel priv) const {
        std::string priv_str;
        switch (priv) {
        case PRIV_PERMIT_UM:
        case PRIV_HAS_UM:
            priv_str = "UM";
            break;
        case PRIV_PERMIT_SV:
        case PRIV_HAS_SV:
            priv_str = "SV";
            break;
        case PRIV_PERMIT_HV:
        case PRIV_HAS_HV:
            priv_str = "HV";
            break;
        case PRIV_PERMIT_DBUM:
            priv_str = "DB";
            break;
        case PRIV_HAS_DBUM:
            priv_str = "DBUM";
            break;
        case PRIV_PERMIT_DBSV:
        case PRIV_HAS_DBSV:
        case PRIV_PERMIT_DBHV:
        //case PRIV_HAS_DBHV: // same value with PRIV_PERMIT_DBHV
            priv_str = "DB";
            break;
        default:
            priv_str = "Unknown"; // don't come here
            break;
        }
        return priv_str;
    }


    std::vector<CfSReg> m_gsr_vec;
    void SetupGsrVec (void);

    enum IsMdpCheck {
    MDP_CHECK_SKIP = false,
    MDP_CHECK_NEED = true,
    };
    bool StmBody (PhysAddr addr, CfSReg reg, IsMdpCheck is_check_mdp);
    bool LdmBody (PhysAddr addr, CfSReg reg, IsMdpCheck is_check_mdp, bool is_update_sreg);

    InstV850 ();
};
