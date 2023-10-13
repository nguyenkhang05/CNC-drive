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
#include "./forest_common.h"


// 例外情報クラス
class ExpInfo {
public:
    // 例外タイプ
    enum ExpType {
        EXP_T_ABORT,  // 中断型
        EXP_T_CMPLT,  // 完了型
        EXP_T_REEXE,  // 再実行型
        EXP_T_SP,     // 特殊
    };

    // 例外レベル
    enum ExpLevel {
        EXP_LV_EI,     // EIレベル例外
        EXP_LV_FE,     // FEレベル例外
        EXP_LV_DB,     // DBレベル例外
        EXP_LV_RS,     // リセットレベル例外
    };

    class ExpTable {
    public:
        ExpType   m_type;        // 種別
        ExpLevel  m_level;       // 例外レベル
        ExpCause  m_cause;       // 例外要因コード
        int32_t   m_hdl_offset;  // ハンドラオフセット
        std::string m_name;      // 出力名
    };

    static const ExpTable m_exp_table[];

    static ExpType  GetExpType   (ExpCode exp_code);
    static ExpLevel GetExpLevel  (ExpCode exp_code);
    static ExpCause GetExpCause  (ExpCode exp_code);
    static int32_t  GetHdlOffset (ExpCode exp_code);
    static std::string GetName   (ExpCode exp_code);
};
