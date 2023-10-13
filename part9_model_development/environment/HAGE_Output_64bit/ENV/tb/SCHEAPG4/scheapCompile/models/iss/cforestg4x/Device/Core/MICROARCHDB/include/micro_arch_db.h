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

// @@ 依存関係を解消できたものから class内に移動すること

/* 命令グループ列挙型 */
enum InstGroupType {
    IGRP_UNDEF = 0,
    IGRP_AL    = 1,
    IGRP_ALC   = 2,
    IGRP_BIT   = 3,
    IGRP_BRA   = 4,
    IGRP_BSC   = 5,
    IGRP_CAC   = 6,
    IGRP_DBG   = 7,
    IGRP_DV    = 8,
    IGRP_DVQ   = 9,
    IGRP_FPU   = 10,
    IGRP_SIMD  = 11,
    IGRP_HT    = 12,
    IGRP_HV    = 13,
    IGRP_LD    = 14,
    IGRP_LOG   = 15,
    IGRP_LP    = 16,
    IGRP_MAC   = 17,
    IGRP_MUL   = 18,
    IGRP_SAT   = 19,
    IGRP_SHI   = 20,
    IGRP_SP    = 21,
    IGRP_ST    = 22,
    IGRP_NUM   = 23 // guard
};


/* 引数フォーマット列挙型 */
#include "./inst_argtype.h"


// 並列発行グループ列挙型
// 命令グループ変更時、csの同時発行テーブルだけでなく、
// Trace表示のグループ名文字列も更新すること。
enum ParaGroupType {
    PGRP_1_1 = 0,
    PGRP_1_2 = 1,
    PGRP_1_3 = 2,
    PGRP_1_4 = 3,
    PGRP_1_5 = 4,
    PGRP_2_1 = 5,
    PGRP_2_2 = 6,
    PGRP_2_3 = 7,
    PGRP_2_4 = 8,
    PGRP_2_5 = 9,
    PGRP_2_6 = 10,
    PGRP_2_7 = 11,
    PGRP_2_8 = 12,
    PGRP_3_1 = 13,
    PGRP_3_2 = 14,
    PGRP_3_3 = 15,
    PGRP_3_4 = 16,
    PGRP_4_1 = 17,
    PGRP_4_2 = 18,
    PGRP_4_3 = 19,
    PGRP_4_4 = 20,
    PGRP_5_1 = 21,
    PGRP_5_2 = 22,
    PGRP_5_3 = 23,
    PGRP_5_4 = 24,
    PGRP_5_5 = 25,
    PGRP_6_1 = 26,
    PGRP_7_1 = 27,
    PGRP_8_1 = 28,
    PGRP_8_2 = 29,
    PGRP_9_1 = 30,
    PGRP_9_2 = 31,
    PGRP_9_3 = 32,
    PGRP_9_4 = 33,
    PGRP_UNDEF = 34, // 未定義
    PGRP_NUM = 35   // 番兵
};


enum ResvResType {
    RS_RES_INT   = 0,
    RS_RES_LS    = 1,
    RS_RES_FPU   = 2,
    RS_RES_SIMD  = 3,
    RS_RES_DP    = 4,
    RS_RES_FXUMOV= 5,
    RS_RES_FXUOP = 6,
    RS_RES_UNDEF = 7,
    RS_RES_NUM   = 8
};


enum ExecResType {
    EX_RES_INT   = 0,
    EX_RES_BRU   = 1,
    EX_RES_SRU   = 2,
    EX_RES_DIV   = 3,
    EX_RES_MUL   = 4,
    EX_RES_LS    = 5,
    EX_RES_FPU   = 6,
    EX_RES_SIMD  = 7,
    EX_RES_DP    = 8,
    EX_RES_FXUMOV= 9,
    EX_RES_FXUOP = 10,
    EX_RES_UNDEF = 11,
    EX_RES_NUM   = 12
};


enum PswDepType {
    PSW_DEP_NONE = 0,
    PSW_DEP_USE  = 1,
    PSW_DEP_SET  = 2,
    PSW_DEP_SET_USE = 3
};


enum CycleCalcType {
    CCT_NORM     = 0,  // 特殊条件なし
    CCT_GRP_UNC  = 1,  // Unconditional bcondでは命令グループ変化
    CCT_LDXX_SELID = 2,  // G3M:MPU, G3MH:selid=0/1/2/3/5へのldxxなら命令グループ／issuerate変化
    CCT_LAT_MV   = 3,  // MOV命令の0latency forward
    CCT_LAT_MVR0 = 4,  // MOVEA/MOVHIのsrc=R0なら0latency forward
    CCT_LAT_DISP = 5,  // Dispose
    CCT_LAT_PREP = 6,  // Prepare
    CCT_LAT_POP  = 7,  // Pop
    CCT_LAT_PUSH = 8,  // Push
    CCT_LAT_SW   = 9,  // Switch
    CCT_LAT_CALL = 10, // callt/hvcall/syscall
    CCT_LAT_DVQ  = 11, // divq
    CCT_LAT_DVQU = 12, // divqu
    CCT_UP_FPCC  = 13, // FPCCの更新サイクルを記録 for TRFSR
    CCT_UP_FPCCD  = 14, // FPCCの更新サイクルを記録 for TRFSR
    CCT_USE_FPCC = 15, // FPCC依存ストールを計算 for TRFSR
    CCT_FPU_FPP  = 16, // FPU none RN divided instructions
    CCT_FPU_2RS  = 17, // FPU RN divided instructions
    CCT_VLD_INDEX = 18, // インデックスレジスタ更新付きVLD命令
    CCT_SYNC     = 19, // SYNC命令の発行レートを計算
    CCT_RMW      = 20, // BitOp命令の発行レートを計算
    CCT_CMOV     = 21, // CMOV命令でREG1/2のうち使用されない方のレイテンシを加算
    CCT_CMOVF    = 22, // CMOVF命令でREG1/2のうち使用されない方のレイテンシを加算
    CCT_CMOVFD   = 23, // CMOVF命令でREG1/2のうち使用されない方のレイテンシを加算
    CCT_INV_BUF  = 24, // LDL.W/STC.W命令でWTBUFを無効化(Bitop/CAXIも同じ）
    CCT_LONG_INST = 25, // 32bit超命令の同時発行切り替え for G3H
    CCT_DIVF     = 26, // IVF/SQRTF/RECIPF/RSQRTF to DIVF/SQRTF/RECIPF/RSQRTF repeat rate
    CCT_DIVFD    = 27, // IVF/SQRTF/RECIPF/RSQRTF to DIVF/SQRTF/RECIPF/RSQRTF repeat rate
    CCT_MULFD    = 28, // MULFD to MULFD repeat rate
    CCT_DW       = 29,  // DoubleWord ld/st
    CCT_LAT_DIV  = 31, // DIV issue rate is changed in G3MH.
    CCT_NOP      = 32, // NOP can be issued in parallel in G3MH.
    CCT_MAC      = 33, // MAC repeat rate in G3MH
    CCT_LDV_QW   = 34, // インデックスレジスタ更新付きVLD命令
    CCT_STV_QW   = 35, // インデックスレジスタ更新付きVST命令
    CCT_LOAD_INCDEC = 36, // The load target regiser is different in LOAD with inc/dec (for SCHEAP)
    CCT_STC      = 37, // STC.x : Latency/issue/repeat is different between success or fail.
    CCT_LAT_RESBANK  = 38, // RESBANK
    CCT_CACHE    = 39, // CACHE/PREF instructions for IC FILL
    CCT_RET_SYNC = 40, // Synchronization in EIRET/FERET/DBRET
    CCT_STMMP    = 41, // STM.MP
    CCT_LDM      = 42, // LDM.GSR/LDM.MP
    CCT_NUM      = 43  // 番兵
};

// @@ Static class にしたが、ヘテロマルチのとき、大丈夫？
// @@ PE内のメンバとしたほうがよいかも

enum ArchDbType {
    ARCH_DB_G3M  = 0,
    ARCH_DB_G3MH = 1,
    ARCH_DB_G3KH = 2,
    ARCH_DB_NUM  = 3
};

enum FusionInstType {
    FUSION_UNDEF  = 0,
    FUSION_ALU1ST = 1,
    FUSION_ALU1ST_MOV2ND = 2,
    FUSION_ALU2ND = 3,
    FUSION_MOV1ST = 4,
};

enum IdDividedType {
    IDD_UNDEF = 0,
    IDD_1 = 1,
    IDD_2 = 2,
    IDD_3 = 3,
    IDD_4 = 4,
    IDD_5 = 5,
};

enum RnDividedType {
    RND_UNDEF = 0,
    RND_2 = 1,

};

enum DpDividedType {
    DPD_UNDEF = 0,
    DPD_LSAL = 1,
};

enum WbDividedType {
    WBD_UNDEF = 0,
    WBD_2 = 1,
};

enum ExLimitType {
    EX_LIMIT_UNDEF = 0,
    EX_LIMIT_BRU   = 1,
    EX_LIMIT_DIV   = 2,
    EX_LIMIT_MUL   = 3,
    EX_LIMIT_SRU   = 4,
    EX_LIMIT_LSBR  = 5,
};

enum SingleCmtType {
    SINGLE_CM_UNDEF      = 0,
    SINGLE_CM_SINGLE     = 1,
    SINGLE_CM_SINGLE_JMP = 2,
};



class MicroArchDB {
private:
    // cedar_arch_info_gen.plのSRC_REG_NUM/DST_REG_NUMと一致させること
    static const int32_t SRC_REG_NUM = 32;
    static const int32_t DST_REG_NUM = 32;
    static const int32_t ISSUE_RES_NUM = 2;

    class InstInfo {
    public:
        BranchType    branch_type[ARCH_DB_NUM]; // 分岐命令タイプ
        CoproType     copro_type;           // コプロセッサ種別
        ArgFormat     arg_format;           /* 引数フォーマットテーブル */
        ParaGroupType para_group;           // 並列発行グループ
        ParaGroupType para_group_alt;       // 並列発行グループ(特殊条件用)
        ParaGroupType para_group_g3kh;       // 並列発行グループ(特殊条件用)
        FusionInstType fusion_inst_type;    // Fusion instruction type
        IdDividedType id_divided;           // ID divided instruction
        RnDividedType rn_divided;           // RN divided instruction
        DpDividedType dp_divided;           // DP divided instruction
        WbDividedType wb_divided;           // WB divided instruction
        ExLimitType   ex_limit;             // Ex issue limitation
        SingleCmtType single_cmt;           // Single commit instruction
        InstGroupType inst_group;           // 命令グループ
        CycleCalcType cycle_calc_type;      // サイクル計算の特殊条件
        LoadType      load_type;            // サイクル計算の特殊条件
        ResvResType   resv_res_type[ISSUE_RES_NUM]; // RS resource
        ExecResType   exec_res_type[ISSUE_RES_NUM]; // EX resource
        PswDepType    psw_dep_type;         // PSW依存
        ByteSize      inst_size;            // 命令サイズ
        int32_t       issue_rate[ARCH_DB_NUM]; // 発行レート
        PrivLevel     priv_type;            // 特権命令タイプ
        uint32_t      mei_inst_type;        // MEI命令種別コード
        uint32_t      mdp_exp_cause_bitpos; // MDP例外要因コードビット番号
        int32_t       src_reg_stage[ARCH_DB_NUM][SRC_REG_NUM]; // レジスタ読み込みステージ
        int32_t       dst_reg_stage[ARCH_DB_NUM][DST_REG_NUM]; // レジスタ書き込みステージ
        int32_t       wb_stage[DST_REG_NUM];       /* レジスタライトバックステージ */
        const char*   instname;             // 関数名
        const char*   operand_format;       // オペランドフォーマット
    };

public:
    static const int32_t END_ENTRY = -1;

    static int32_t       GetInstID       (uint32_t opcode);
    static BranchType    GetBranchType   (int id, ArchDbType arch_type)
    {
        return m_inst_info[id].branch_type[arch_type];
    }
    static CoproType     GetCoproType    (int id)
    {
        return m_inst_info[id].copro_type;
    }
    static ArgFormat     GetArgFormat    (int id)
    {
        return m_inst_info[id].arg_format;
    }
    static ParaGroupType GetParaGroup    (int id)
    {
        return m_inst_info[id].para_group;
    }
    static ParaGroupType GetParaGroupAlt (int id)
    {
        return m_inst_info[id].para_group_alt;
    }
    static ParaGroupType GetParaGroupG3KH (int id)
    {
        return m_inst_info[id].para_group_g3kh;
    }
    static FusionInstType GetFusionInstType (int id)
    {
        return m_inst_info[id].fusion_inst_type;
    }
    static IdDividedType GetIdDividedType (int id)
    {
        return m_inst_info[id].id_divided;
    }
    static RnDividedType GetRnDividedType (int id)
    {
        return m_inst_info[id].rn_divided;
    }
    static DpDividedType GetDpDividedType (int id)
    {
        return m_inst_info[id].dp_divided;
    }
    static WbDividedType GetWbDividedType (int id)
    {
        return m_inst_info[id].wb_divided;
    }
    static ExLimitType GetExLimitType (int id)
    {
        return m_inst_info[id].ex_limit;
    }
    static SingleCmtType GetSingleCmtType (int id)
    {
        return m_inst_info[id].single_cmt;
    }
    static InstGroupType GetInstGroup    (int id)
    {
        return m_inst_info[id].inst_group;
    }
    static CycleCalcType GetCycleCalcType (int id)
    {
        return m_inst_info[id].cycle_calc_type;
    }
    static LoadType      GetLoadType     (int id)
    {
        return m_inst_info[id].load_type;
    }
    static ResvResType   GetResvResType  (int id, int pos)
    {
        return m_inst_info[id].resv_res_type[pos];
    }
    static ExecResType   GetExecResType  (int id, int pos)
    {
        return m_inst_info[id].exec_res_type[pos];
    }
    static PswDepType    GetPswDepType   (int id)
    {
        return m_inst_info[id].psw_dep_type;
    }
    static ByteSize      GetInstSize     (int id)
    {
        return m_inst_info[id].inst_size;
    }
    static const char*   GetInstName     (int id)
    {
        return m_inst_info[id].instname;
    }
    static const char*   GetOperandFormat (int id)
    {
        return m_inst_info[id].operand_format;
    }
    static int32_t       GetIssueRate    (int id, ArchDbType arch_type)
    {
        return m_inst_info[id].issue_rate[arch_type];
    }
    static PrivLevel     GetPrivType     (int id)
    {
        return m_inst_info[id].priv_type;
    }
    static uint32_t      GetMeiInstType  (int id)
    {
        return m_inst_info[id].mei_inst_type;
    }
    static uint32_t      GetMdpExpCauseBitPos (int id)
    {
        return m_inst_info[id].mdp_exp_cause_bitpos;
    }
    static int32_t       GetRegSrcStage  (int id, int r, ArchDbType arch_type)
    {
        return m_inst_info[id].src_reg_stage[arch_type][r];
    }
    static int32_t       GetRegDstStage  (int id, int r, ArchDbType arch_type)
    {
        return m_inst_info[id].dst_reg_stage[arch_type][r];
    }
    static int32_t       GetRegWbStage   (int id, int r)
    {
        return m_inst_info[id].wb_stage[r];
    }

private:
    // デコードキー列挙型
    enum DecodeKey {
        DECKEY_A,  DECKEY_B,  DECKEY_C,  DECKEY_D,  DECKEY_E,  DECKEY_F,
        DECKEY_A0, DECKEY_B0, DECKEY_C0, DECKEY_D0, DECKEY_E0, DECKEY_F0,
        DECKEY_J,  DECKEY_K,  DECKEY_S,  DECKEY_T,
        DECKEY_U,  DECKEY_V,  DECKEY_W,
        DEC_END  = -1
    };

    // 命令テーブル構造体
    class CodeTable {
    public:
        DecodeKey deckey;
        int32_t id;
        int32_t pos;
    };

    static const InstInfo  m_inst_info[];
    static const CodeTable m_code_table[];

    inline static uint32_t BitExtract (uint32_t x, uint32_t l, uint32_t r) {
        return ((x >> r) & ~(~0x0U << (l - r + 1)));
    }
};
