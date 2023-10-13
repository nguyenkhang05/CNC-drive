/*
 * (c) 2011,2012,2013 Renesas Electronics Corporation
 * RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY
 * This program must be used solely for the purpose for which
 * it was furnished by Renesas Electronics Corporation. No part of this
 * program may be reproduced or disclosed to others, in any
 * form, without the prior written permission of Renesas Electronics
 * Corporation.
 */

#include <string>
#include "./config.h"
#include "./forest_common.h"
#include "./exp_info.h"


ExpInfo::ExpType ExpInfo::GetExpType (ExpCode exp_code)
{
    return m_exp_table[exp_code].m_type;
}


ExpInfo::ExpLevel ExpInfo::GetExpLevel  (ExpCode exp_code)
{
    return m_exp_table[exp_code].m_level;
}


ExpCause ExpInfo::GetExpCause  (ExpCode exp_code)
{
    return m_exp_table[exp_code].m_cause;
}


int32_t ExpInfo::GetHdlOffset (ExpCode exp_code)
{
    return m_exp_table[exp_code].m_hdl_offset;
}


std::string ExpInfo::GetName (ExpCode exp_code)
{
    return m_exp_table[exp_code].m_name;
}


// 例外情報テーブル初期化
const ExpInfo::ExpTable ExpInfo::m_exp_table[] = {
//  {ExpType,      ExpLevel,  ExpCause, hdl_offset, name}
    { EXP_T_SP,    EXP_LV_RS, 0x00000, 0x0000, "NONE" },
    { EXP_T_ABORT, EXP_LV_RS, 0x00000, 0x0000, "RESET" },
    { EXP_T_ABORT, EXP_LV_DB, 0x000BF, 0x00B0, "DBNMI" },
    { EXP_T_ABORT, EXP_LV_DB, 0x000BC, 0x00B0, "RLB" },
    { EXP_T_ABORT, EXP_LV_DB, 0x000B0, 0x00B0, "DBINT" },
    { EXP_T_ABORT, EXP_LV_FE, 0x000E0, 0x00E0, "FENMI" },
    { EXP_T_ABORT, EXP_LV_FE, 0x00010, 0x0010, "SYSERR" },
    { EXP_T_ABORT, EXP_LV_FE, 0x0001C, 0x0010, "SYSERR_RB" },
    { EXP_T_ABORT, EXP_LV_FE, 0x000F0, 0x00F0, "FEINT" },
    { EXP_T_ABORT, EXP_LV_FE, 0x000F0, 0x00F0, "GMFEINT" },
    { EXP_T_ABORT, EXP_LV_EI, 0x0D800, 0x00D0, "BGFEINT" },
    { EXP_T_ABORT, EXP_LV_EI, 0x00072, 0x0070, "FPI" },
    { EXP_T_ABORT, EXP_LV_EI, 0x01000, 0x0100, "EIINT" },
    { EXP_T_ABORT, EXP_LV_EI, 0x01000, 0x0100, "GMEIINT" },
    { EXP_T_ABORT, EXP_LV_EI, 0x0D000, 0x00D0, "BGEIINT" },
    { EXP_T_ABORT, EXP_LV_FE, 0x00010, 0x0010, "SYSERR(G)" },
    { EXP_T_ABORT, EXP_LV_DB, 0x000B6, 0x00B0, "LSAB" },
    { EXP_T_ABORT, EXP_LV_FE, 0x0009D, 0x0090, "MDP(EIINT)" },
    { EXP_T_ABORT, EXP_LV_FE, 0x0009D, 0x0090, "MDP(GMEIINT)" },
    { EXP_T_ABORT, EXP_LV_FE, 0x00095, 0x0090, "MDP(EIINT)" },
    { EXP_T_ABORT, EXP_LV_FE, 0x00095, 0x0090, "MDP(GMEIINT)" },
    { EXP_T_REEXE, EXP_LV_DB, 0x000B5, 0x00B0, "PCB" },
    { EXP_T_REEXE, EXP_LV_DB, 0x000B5, 0x00B0, "LSAB" },
    { EXP_T_REEXE, EXP_LV_DB, 0x000B5, 0x00B0, "AE" },
    { EXP_T_REEXE, EXP_LV_FE, 0x00098, 0x0090, "MIP" },
    { EXP_T_REEXE, EXP_LV_FE, 0x00090, 0x0090, "MIP" },
    { EXP_T_REEXE, EXP_LV_FE, 0x00010, 0x0010, "SYSERR_IF" },
    { EXP_T_REEXE, EXP_LV_FE, 0x0001D, 0x0010, "SYSERR_REEXEC" },
    { EXP_T_REEXE, EXP_LV_FE, 0x00060, 0x0060, "RIE" },
    { EXP_T_REEXE, EXP_LV_FE, 0x00080, 0x0080, "UCPOP0" },
    { EXP_T_REEXE, EXP_LV_FE, 0x00081, 0x0080, "UCPOP1" },
    { EXP_T_REEXE, EXP_LV_FE, 0x00082, 0x0080, "UCPOP2" },
    { EXP_T_REEXE, EXP_LV_FE, 0x000A0, 0x00A0, "PIE" },
    { EXP_T_REEXE, EXP_LV_FE, 0x000C0, 0x00C0, "MAE" },
    { EXP_T_REEXE, EXP_LV_FE, 0x00099, 0x0090, "MDP" },
    { EXP_T_REEXE, EXP_LV_FE, 0x00091, 0x0090, "MDP" },
    { EXP_T_REEXE, EXP_LV_EI, 0x00071, 0x0070, "FPE" },
    { EXP_T_REEXE, EXP_LV_EI, 0x00075, 0x0070, "FXE" },
    { EXP_T_CMPLT, EXP_LV_DB, 0x000B1, 0x00B0, "DBTRAP" },
    { EXP_T_CMPLT, EXP_LV_EI, 0x0F000, 0x0020, "HVTRAP" },
    { EXP_T_CMPLT, EXP_LV_EI, 0x08000, 0x0000, "SYSCALL" },
    { EXP_T_CMPLT, EXP_LV_FE, 0x00031, 0x0030, "FETRAP" },
    { EXP_T_CMPLT, EXP_LV_EI, 0x00040, 0x0040, "TRAP0" },
    { EXP_T_CMPLT, EXP_LV_EI, 0x00050, 0x0050, "TRAP1" },
};
