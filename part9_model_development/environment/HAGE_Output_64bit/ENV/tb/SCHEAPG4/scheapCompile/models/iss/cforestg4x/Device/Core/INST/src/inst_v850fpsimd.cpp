/*
 * (c) 2011,2012 Renesas Electronics Corporation
 * RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY
 * This program must be used solely for the purpose for which
 * it was furnished by Renesas Electronics Corporation. No part of this
 * program may be reproduced or disclosed to others, in any
 * form, without the prior written permission of Renesas Electronics
 * Corporation.
 */

#include <tuple>
#include "./config.h"
#include "./forest_common.h"
#include "./inst_v850.h"
#include "./trace_info.h"
#include "./gregfile.h"
#include "./sregfile.h"
#include "./wregdata128.h"
#include "./lsu.h"


/*!
 * @brief Executes the instruction of CMPFS4. : CMPFS4命令
 * @param opcode Operation Code. : 命令コード
 */
void InstV850::Inst_CMPFS4 (uint64_t opcode)
{
    uint32_t vreg1 = OperandExtract (opcode,  4,  0);
    uint32_t vreg2 = OperandExtract (opcode, 15, 11);
    uint32_t vreg3 = OperandExtract (opcode, 31, 27);
    uint32_t fcond = OperandExtract (opcode, 20, 17);

    uint64_t v1_dw0, v1_dw1;
    WRegRead128DW (vreg1, &v1_dw1, &v1_dw0);
    WRegData128 v1 (v1_dw1, v1_dw0);

    uint64_t v2_dw0, v2_dw1;
    WRegRead128DW (vreg2, &v2_dw1, &v2_dw0);
    WRegData128 v2 (v2_dw1, v2_dw0);

    bool is_exp_permitted = false;
    WRegData128 res = CMPFS (v1, v2, fcond, &is_exp_permitted);

    if (! is_exp_permitted) {
        WRegWrite128DW (vreg3, res.Get64(1), res.Get64(0));
    }
    m_trace->RecordSReg<TraceCtrl::REG_WRITE> (CF_SR_FXSR, m_srf->SrFXSR()->ReadBody());
}


/*!
 * @brief Executes the instruction of ROUNDFSW4. : ROUNDFSW4命令
 * @param opcode Operation Code. : 命令コード
 */
void InstV850::Inst_ROUNDFSW4 (uint64_t opcode)
{
    uint32_t vreg2 = OperandExtract (opcode, 15, 11);
    uint32_t vreg3 = OperandExtract (opcode, 31, 27);

    uint64_t v2_dw0, v2_dw1;
    WRegRead128DW (vreg2, &v2_dw1, &v2_dw0);
    WRegData128 v2 (v2_dw1, v2_dw0);

    bool is_exp_permitted = false;
    WRegData128 res = ROUNDFSW (v2, &is_exp_permitted);

    if (! is_exp_permitted) {
        WRegWrite128DW (vreg3, res.Get64(1), res.Get64(0));
    }
    m_trace->RecordSReg<TraceCtrl::REG_WRITE> (CF_SR_FXSR, m_srf->SrFXSR()->ReadBody());
}


/*!
 * @brief Executes the instruction of TRNCFSW4. : TRNCFSW4命令
 * @param opcode Operation Code. : 命令コード
 */
void InstV850::Inst_TRNCFSW4 (uint64_t opcode)
{
    uint32_t vreg2 = OperandExtract (opcode, 15, 11);
    uint32_t vreg3 = OperandExtract (opcode, 31, 27);

    uint64_t v2_dw0, v2_dw1;
    WRegRead128DW (vreg2, &v2_dw1, &v2_dw0);
    WRegData128 v2 (v2_dw1, v2_dw0);

    bool is_exp_permitted = false;
    WRegData128 res = TRNCFSW (v2, &is_exp_permitted);

    if (! is_exp_permitted) {
        WRegWrite128DW (vreg3, res.Get64(1), res.Get64(0));
    }
    m_trace->RecordSReg<TraceCtrl::REG_WRITE> (CF_SR_FXSR, m_srf->SrFXSR()->ReadBody());
}


/*!
 * @brief Executes the instruction of CEILFSW4. : CEILFSW4命令
 * @param opcode Operation Code. : 命令コード
 */
void InstV850::Inst_CEILFSW4 (uint64_t opcode)
{
    uint32_t vreg2 = OperandExtract (opcode, 15, 11);
    uint32_t vreg3 = OperandExtract (opcode, 31, 27);

    uint64_t v2_dw0, v2_dw1;
    WRegRead128DW (vreg2, &v2_dw1, &v2_dw0);
    WRegData128 v2 (v2_dw1, v2_dw0);

    bool is_exp_permitted = false;
    WRegData128 res = CEILFSW (v2, &is_exp_permitted);

    if (! is_exp_permitted) {
        WRegWrite128DW (vreg3, res.Get64(1), res.Get64(0));
    }
    m_trace->RecordSReg<TraceCtrl::REG_WRITE> (CF_SR_FXSR, m_srf->SrFXSR()->ReadBody());
}


/*!
 * @brief Executes the instruction of FLOORFSW4. : FLOORFSW4命令
 * @param opcode Operation Code. : 命令コード
 */
void InstV850::Inst_FLOORFSW4 (uint64_t opcode)
{
    uint32_t vreg2 = OperandExtract (opcode, 15, 11);
    uint32_t vreg3 = OperandExtract (opcode, 31, 27);

    uint64_t v2_dw0, v2_dw1;
    WRegRead128DW (vreg2, &v2_dw1, &v2_dw0);
    WRegData128 v2 (v2_dw1, v2_dw0);

    bool is_exp_permitted = false;
    WRegData128 res = FLOORFSW (v2, &is_exp_permitted);

    if (! is_exp_permitted) {
        WRegWrite128DW (vreg3, res.Get64(1), res.Get64(0));
    }
    m_trace->RecordSReg<TraceCtrl::REG_WRITE> (CF_SR_FXSR, m_srf->SrFXSR()->ReadBody());
}


/*!
 * @brief Executes the instruction of CVTFSW4. : CVTFSW4命令
 * @param opcode Operation Code. : 命令コード
 */
void InstV850::Inst_CVTFSW4 (uint64_t opcode)
{
    uint32_t vreg2 = OperandExtract (opcode, 15, 11);
    uint32_t vreg3 = OperandExtract (opcode, 31, 27);

    uint64_t v2_dw0, v2_dw1;
    WRegRead128DW (vreg2, &v2_dw1, &v2_dw0);
    WRegData128 v2 (v2_dw1, v2_dw0);

    bool is_exp_permitted = false;
    WRegData128 res = CVTFSW (v2, &is_exp_permitted);

    if (! is_exp_permitted) {
        WRegWrite128DW (vreg3, res.Get64(1), res.Get64(0));
    }
    m_trace->RecordSReg<TraceCtrl::REG_WRITE> (CF_SR_FXSR, m_srf->SrFXSR()->ReadBody());
}


/*!
 * @brief Executes the instruction of ROUNDFSUW4. : ROUNDFSUW4命令
 * @param opcode Operation Code. : 命令コード
 */
void InstV850::Inst_ROUNDFSUW4 (uint64_t opcode)
{
    uint32_t vreg2 = OperandExtract (opcode, 15, 11);
    uint32_t vreg3 = OperandExtract (opcode, 31, 27);

    uint64_t v2_dw0, v2_dw1;
    WRegRead128DW (vreg2, &v2_dw1, &v2_dw0);
    WRegData128 v2 (v2_dw1, v2_dw0);

    bool is_exp_permitted = false;
    WRegData128 res = ROUNDFSUW (v2, &is_exp_permitted);

    if (! is_exp_permitted) {
        WRegWrite128DW (vreg3, res.Get64(1), res.Get64(0));
    }
    m_trace->RecordSReg<TraceCtrl::REG_WRITE> (CF_SR_FXSR, m_srf->SrFXSR()->ReadBody());
}


/*!
 * @brief Executes the instruction of TRNCFSUW4. : TRNCFSUW4命令
 * @param opcode Operation Code. : 命令コード
 */
void InstV850::Inst_TRNCFSUW4 (uint64_t opcode)
{
    uint32_t vreg2 = OperandExtract (opcode, 15, 11);
    uint32_t vreg3 = OperandExtract (opcode, 31, 27);

    uint64_t v2_dw0, v2_dw1;
    WRegRead128DW (vreg2, &v2_dw1, &v2_dw0);
    WRegData128 v2 (v2_dw1, v2_dw0);

    bool is_exp_permitted = false;
    WRegData128 res = TRNCFSUW (v2, &is_exp_permitted);

    if (! is_exp_permitted) {
        WRegWrite128DW (vreg3, res.Get64(1), res.Get64(0));
    }
    m_trace->RecordSReg<TraceCtrl::REG_WRITE> (CF_SR_FXSR, m_srf->SrFXSR()->ReadBody());
}


/*!
 * @brief Executes the instruction of CEILFSUW4. : CEILFSUW4命令
 * @param opcode Operation Code. : 命令コード
 */
void InstV850::Inst_CEILFSUW4 (uint64_t opcode)
{
    uint32_t vreg2 = OperandExtract (opcode, 15, 11);
    uint32_t vreg3 = OperandExtract (opcode, 31, 27);

    uint64_t v2_dw0, v2_dw1;
    WRegRead128DW (vreg2, &v2_dw1, &v2_dw0);
    WRegData128 v2 (v2_dw1, v2_dw0);

    bool is_exp_permitted = false;
    WRegData128 res = CEILFSUW (v2, &is_exp_permitted);

    if (! is_exp_permitted) {
        WRegWrite128DW (vreg3, res.Get64(1), res.Get64(0));
    }
    m_trace->RecordSReg<TraceCtrl::REG_WRITE> (CF_SR_FXSR, m_srf->SrFXSR()->ReadBody());
}


/*!
 * @brief Executes the instruction of FLOORFSUW4. : FLOORFSUW4命令
 * @param opcode Operation Code. : 命令コード
 */
void InstV850::Inst_FLOORFSUW4 (uint64_t opcode)
{
    uint32_t vreg2 = OperandExtract (opcode, 15, 11);
    uint32_t vreg3 = OperandExtract (opcode, 31, 27);

    uint64_t v2_dw0, v2_dw1;
    WRegRead128DW (vreg2, &v2_dw1, &v2_dw0);
    WRegData128 v2 (v2_dw1, v2_dw0);

    bool is_exp_permitted = false;
    WRegData128 res = FLOORFSUW (v2, &is_exp_permitted);

    if (! is_exp_permitted) {
        WRegWrite128DW (vreg3, res.Get64(1), res.Get64(0));
    }
    m_trace->RecordSReg<TraceCtrl::REG_WRITE> (CF_SR_FXSR, m_srf->SrFXSR()->ReadBody());
}


/*!
 * @brief Executes the instruction of CVTFSUW4. : CVTFSUW4命令
 * @param opcode Operation Code. : 命令コード
 */
void InstV850::Inst_CVTFSUW4 (uint64_t opcode)
{
    uint32_t vreg2 = OperandExtract (opcode, 15, 11);
    uint32_t vreg3 = OperandExtract (opcode, 31, 27);

    uint64_t v2_dw0, v2_dw1;
    WRegRead128DW (vreg2, &v2_dw1, &v2_dw0);
    WRegData128 v2 (v2_dw1, v2_dw0);

    bool is_exp_permitted = false;
    WRegData128 res = CVTFSUW (v2, &is_exp_permitted);

    if (! is_exp_permitted) {
        WRegWrite128DW (vreg3, res.Get64(1), res.Get64(0));
    }
    m_trace->RecordSReg<TraceCtrl::REG_WRITE> (CF_SR_FXSR, m_srf->SrFXSR()->ReadBody());
}


/*!
 * @brief Executes the instruction of CVTFWS4. : CVTFWS4命令
 * @param opcode Operation Code. : 命令コード
 */
void InstV850::Inst_CVTFWS4 (uint64_t opcode)
{
    uint32_t vreg2 = OperandExtract (opcode, 15, 11);
    uint32_t vreg3 = OperandExtract (opcode, 31, 27);

    uint64_t v2_dw0, v2_dw1;
    WRegRead128DW (vreg2, &v2_dw1, &v2_dw0);
    WRegData128 v2 (v2_dw1, v2_dw0);

    bool is_exp_permitted = false;
    WRegData128 res = CVTFWS (v2, &is_exp_permitted);

    if (! is_exp_permitted) {
        WRegWrite128DW (vreg3, res.Get64(1), res.Get64(0));
    }
    m_trace->RecordSReg<TraceCtrl::REG_WRITE> (CF_SR_FXSR, m_srf->SrFXSR()->ReadBody());
}


/*!
 * @brief Executes the instruction of CVTFUWS4. : CVTFUWS4命令
 * @param opcode Operation Code. : 命令コード
 */
void InstV850::Inst_CVTFUWS4 (uint64_t opcode)
{
    uint32_t vreg2 = OperandExtract (opcode, 15, 11);
    uint32_t vreg3 = OperandExtract (opcode, 31, 27);

    uint64_t v2_dw0, v2_dw1;
    WRegRead128DW (vreg2, &v2_dw1, &v2_dw0);
    WRegData128 v2 (v2_dw1, v2_dw0);

    bool is_exp_permitted = false;
    WRegData128 res = CVTFUWS (v2, &is_exp_permitted);

    if (! is_exp_permitted) {
        WRegWrite128DW (vreg3, res.Get64(1), res.Get64(0));
    }
    m_trace->RecordSReg<TraceCtrl::REG_WRITE> (CF_SR_FXSR, m_srf->SrFXSR()->ReadBody());
}


/*!
 * @brief Executes the instruction of CVTFHS4. : CVTFHS4命令
 * @param opcode Operation Code. : 命令コード
 */
void InstV850::Inst_CVTFHS4 (uint64_t opcode)
{
    uint32_t vreg2 = OperandExtract (opcode, 15, 11);
    uint32_t vreg3 = OperandExtract (opcode, 31, 27);

    uint64_t v2_dw0, v2_dw1;
    WRegRead128DW (vreg2, &v2_dw1, &v2_dw0);
    WRegData128 v2 (v2_dw1, v2_dw0);

    bool is_exp_permitted = false;
    WRegData128 res = CVTFHS (v2, &is_exp_permitted);

    if (! is_exp_permitted) {
        WRegWrite128DW (vreg3, res.Get64(1), res.Get64(0));
    }
    m_trace->RecordSReg<TraceCtrl::REG_WRITE> (CF_SR_FXSR, m_srf->SrFXSR()->ReadBody());
}


/*!
 * @brief Executes the instruction of CVTFSH4. : CVTFSH4命令
 * @param opcode Operation Code. : 命令コード
 */
void InstV850::Inst_CVTFSH4 (uint64_t opcode)
{
    uint32_t vreg2 = OperandExtract (opcode, 15, 11);
    uint32_t vreg3 = OperandExtract (opcode, 31, 27);

    uint64_t v2_dw0, v2_dw1;
    WRegRead128DW (vreg2, &v2_dw1, &v2_dw0);
    WRegData128 v2 (v2_dw1, v2_dw0);

    bool is_exp_permitted = false;
    WRegData128 res = CVTFSH (v2, &is_exp_permitted);

    if (! is_exp_permitted) {
        WRegWrite128DW (vreg3, res.Get64(1), res.Get64(0));
    }
    m_trace->RecordSReg<TraceCtrl::REG_WRITE> (CF_SR_FXSR, m_srf->SrFXSR()->ReadBody());
}


/*!
 * @brief Executes the instruction of ABSFS4. : ABSFS4命令
 * @param opcode Operation Code. : 命令コード
 */
void InstV850::Inst_ABSFS4 (uint64_t opcode)
{
    uint32_t vreg2 = OperandExtract (opcode, 15, 11);
    uint32_t vreg3 = OperandExtract (opcode, 31, 27);

    uint64_t v2_dw0, v2_dw1;
    WRegRead128DW (vreg2, &v2_dw1, &v2_dw0);
    WRegData128 v2 (v2_dw1, v2_dw0);

    WRegData128 res = ABSFS (v2);

    WRegWrite128DW (vreg3, res.Get64(1), res.Get64(0));

    m_trace->RecordSReg<TraceCtrl::REG_WRITE> (CF_SR_FXSR, m_srf->SrFXSR()->ReadBody());
}


/*!
 * @brief Executes the instruction of NEGFS4. : NEGFS4命令
 * @param opcode Operation Code. : 命令コード
 */
void InstV850::Inst_NEGFS4 (uint64_t opcode)
{
    uint32_t vreg2 = OperandExtract (opcode, 15, 11);
    uint32_t vreg3 = OperandExtract (opcode, 31, 27);

    uint64_t v2_dw0, v2_dw1;
    WRegRead128DW (vreg2, &v2_dw1, &v2_dw0);
    WRegData128 v2 (v2_dw1, v2_dw0);

    WRegData128 res = NEGFS (v2);

    WRegWrite128DW (vreg3, res.Get64(1), res.Get64(0));

    m_trace->RecordSReg<TraceCtrl::REG_WRITE> (CF_SR_FXSR, m_srf->SrFXSR()->ReadBody());
}


/*!
 * @brief Executes the instruction of SQRTFS4. : SQRTFS4命令
 * @param opcode Operation Code. : 命令コード
 */
void InstV850::Inst_SQRTFS4 (uint64_t opcode)
{
    uint32_t vreg2 = OperandExtract (opcode, 15, 11);
    uint32_t vreg3 = OperandExtract (opcode, 31, 27);

    uint64_t v2_dw0, v2_dw1;
    WRegRead128DW (vreg2, &v2_dw1, &v2_dw0);
    WRegData128 v2 (v2_dw1, v2_dw0);

    bool is_exp_permitted = false;
    WRegData128 res = SQRTFS (v2, &is_exp_permitted);

    if (! is_exp_permitted) {
        WRegWrite128DW (vreg3, res.Get64(1), res.Get64(0));
    }
    m_trace->RecordSReg<TraceCtrl::REG_WRITE> (CF_SR_FXSR, m_srf->SrFXSR()->ReadBody());
}


/*!
 * @brief Executes the instruction of RECIPFS4. : RECIPFS4命令
 * @param opcode Operation Code. : 命令コード
 */
void InstV850::Inst_RECIPFS4 (uint64_t opcode)
{
    uint32_t vreg2 = OperandExtract (opcode, 15, 11);
    uint32_t vreg3 = OperandExtract (opcode, 31, 27);

    uint64_t v2_dw0, v2_dw1;
    WRegRead128DW (vreg2, &v2_dw1, &v2_dw0);
    WRegData128 v2 (v2_dw1, v2_dw0);

    bool is_exp_permitted = false;
    WRegData128 res = RECIPFS (v2, &is_exp_permitted);

    if (! is_exp_permitted) {
        WRegWrite128DW (vreg3, res.Get64(1), res.Get64(0));
    }
    m_trace->RecordSReg<TraceCtrl::REG_WRITE> (CF_SR_FXSR, m_srf->SrFXSR()->ReadBody());
}


/*!
 * @brief Executes the instruction of RSQRTFS4. : RSQRTFS4命令
 * @param opcode Operation Code. : 命令コード
 */
void InstV850::Inst_RSQRTFS4 (uint64_t opcode)
{
    uint32_t vreg2 = OperandExtract (opcode, 15, 11);
    uint32_t vreg3 = OperandExtract (opcode, 31, 27);

    uint64_t v2_dw0, v2_dw1;
    WRegRead128DW (vreg2, &v2_dw1, &v2_dw0);
    WRegData128 v2 (v2_dw1, v2_dw0);

    bool is_exp_permitted = false;
    WRegData128 res = RSQRTFS (v2, &is_exp_permitted);

    if (! is_exp_permitted) {
        WRegWrite128DW (vreg3, res.Get64(1), res.Get64(0));
    }
    m_trace->RecordSReg<TraceCtrl::REG_WRITE> (CF_SR_FXSR, m_srf->SrFXSR()->ReadBody());
}


/*!
 * @brief Executes the instruction of ADDFS4. : ADDFS4命令
 * @param opcode Operation Code. : 命令コード
 */
void InstV850::Inst_ADDFS4 (uint64_t opcode)
{
    uint32_t vreg1 = OperandExtract (opcode,  4,  0);
    uint32_t vreg2 = OperandExtract (opcode, 15, 11);
    uint32_t vreg3 = OperandExtract (opcode, 31, 27);

    uint64_t v1_dw0, v1_dw1;
    WRegRead128DW (vreg1, &v1_dw1, &v1_dw0);
    WRegData128 v1 (v1_dw1, v1_dw0);

    uint64_t v2_dw0, v2_dw1;
    WRegRead128DW (vreg2, &v2_dw1, &v2_dw0);
    WRegData128 v2 (v2_dw1, v2_dw0);

    bool is_exp_permitted = false;
    WRegData128 res = ADDFS (v1, v2, &is_exp_permitted);

    if (! is_exp_permitted) {
        WRegWrite128DW (vreg3, res.Get64(1), res.Get64(0));
    }
    m_trace->RecordSReg<TraceCtrl::REG_WRITE> (CF_SR_FXSR, m_srf->SrFXSR()->ReadBody());
}


/*!
 * @brief Executes the instruction of SUBFS4. : SUBFS4命令
 * @param opcode Operation Code. : 命令コード
 */
void InstV850::Inst_SUBFS4 (uint64_t opcode)
{
    uint32_t vreg1 = OperandExtract (opcode,  4,  0);
    uint32_t vreg2 = OperandExtract (opcode, 15, 11);
    uint32_t vreg3 = OperandExtract (opcode, 31, 27);

    uint64_t v1_dw0, v1_dw1;
    WRegRead128DW (vreg1, &v1_dw1, &v1_dw0);
    WRegData128 v1 (v1_dw1, v1_dw0);

    uint64_t v2_dw0, v2_dw1;
    WRegRead128DW (vreg2, &v2_dw1, &v2_dw0);
    WRegData128 v2 (v2_dw1, v2_dw0);

    bool is_exp_permitted = false;
    WRegData128 res = SUBFS (v1, v2, &is_exp_permitted);

    if (! is_exp_permitted) {
        WRegWrite128DW (vreg3, res.Get64(1), res.Get64(0));
    }
    m_trace->RecordSReg<TraceCtrl::REG_WRITE> (CF_SR_FXSR, m_srf->SrFXSR()->ReadBody());
}


/*!
 * @brief Executes the instruction of MULFS4. : MULFS4命令
 * @param opcode Operation Code. : 命令コード
 */
void InstV850::Inst_MULFS4 (uint64_t opcode)
{
    uint32_t vreg1 = OperandExtract (opcode,  4,  0);
    uint32_t vreg2 = OperandExtract (opcode, 15, 11);
    uint32_t vreg3 = OperandExtract (opcode, 31, 27);

    uint64_t v1_dw0, v1_dw1;
    WRegRead128DW (vreg1, &v1_dw1, &v1_dw0);
    WRegData128 v1 (v1_dw1, v1_dw0);

    uint64_t v2_dw0, v2_dw1;
    WRegRead128DW (vreg2, &v2_dw1, &v2_dw0);
    WRegData128 v2 (v2_dw1, v2_dw0);

    bool is_exp_permitted = false;
    WRegData128 res = MULFS (v1, v2, &is_exp_permitted);

    if (! is_exp_permitted) {
        WRegWrite128DW (vreg3, res.Get64(1), res.Get64(0));
    }
    m_trace->RecordSReg<TraceCtrl::REG_WRITE> (CF_SR_FXSR, m_srf->SrFXSR()->ReadBody());
}


/*!
 * @brief Executes the instruction of MAXFS4. : MAXFS4命令
 * @param opcode Operation Code. : 命令コード
 */
void InstV850::Inst_MAXFS4 (uint64_t opcode)
{
    uint32_t vreg1 = OperandExtract (opcode,  4,  0);
    uint32_t vreg2 = OperandExtract (opcode, 15, 11);
    uint32_t vreg3 = OperandExtract (opcode, 31, 27);

    uint64_t v1_dw0, v1_dw1;
    WRegRead128DW (vreg1, &v1_dw1, &v1_dw0);
    WRegData128 v1 (v1_dw1, v1_dw0);

    uint64_t v2_dw0, v2_dw1;
    WRegRead128DW (vreg2, &v2_dw1, &v2_dw0);
    WRegData128 v2 (v2_dw1, v2_dw0);

    bool is_exp_permitted = false;
    WRegData128 res = MAXFS (v1, v2, &is_exp_permitted);

    if (! is_exp_permitted) {
        WRegWrite128DW (vreg3, res.Get64(1), res.Get64(0));
    }
    m_trace->RecordSReg<TraceCtrl::REG_WRITE> (CF_SR_FXSR, m_srf->SrFXSR()->ReadBody());
}


/*!
 * @brief Executes the instruction of MINFS4. : MINFS4命令
 * @param opcode Operation Code. : 命令コード
 */
void InstV850::Inst_MINFS4 (uint64_t opcode)
{
    uint32_t vreg1 = OperandExtract (opcode,  4,  0);
    uint32_t vreg2 = OperandExtract (opcode, 15, 11);
    uint32_t vreg3 = OperandExtract (opcode, 31, 27);

    uint64_t v1_dw0, v1_dw1;
    WRegRead128DW (vreg1, &v1_dw1, &v1_dw0);
    WRegData128 v1 (v1_dw1, v1_dw0);

    uint64_t v2_dw0, v2_dw1;
    WRegRead128DW (vreg2, &v2_dw1, &v2_dw0);
    WRegData128 v2 (v2_dw1, v2_dw0);

    bool is_exp_permitted = false;
    WRegData128 res = MINFS (v1, v2, &is_exp_permitted);

    if (! is_exp_permitted) {
        WRegWrite128DW (vreg3, res.Get64(1), res.Get64(0));
    }
    m_trace->RecordSReg<TraceCtrl::REG_WRITE> (CF_SR_FXSR, m_srf->SrFXSR()->ReadBody());
}


/*!
 * @brief Executes the instruction of DIVFS4. : DIVFS4命令
 * @param opcode Operation Code. : 命令コード
 */
void InstV850::Inst_DIVFS4 (uint64_t opcode)
{
    uint32_t vreg1 = OperandExtract (opcode,  4,  0);
    uint32_t vreg2 = OperandExtract (opcode, 15, 11);
    uint32_t vreg3 = OperandExtract (opcode, 31, 27);

    uint64_t v1_dw0, v1_dw1;
    WRegRead128DW (vreg1, &v1_dw1, &v1_dw0);
    WRegData128 v1 (v1_dw1, v1_dw0);

    uint64_t v2_dw0, v2_dw1;
    WRegRead128DW (vreg2, &v2_dw1, &v2_dw0);
    WRegData128 v2 (v2_dw1, v2_dw0);

    bool is_exp_permitted = false;
    WRegData128 res = DIVFS (v1, v2, &is_exp_permitted);

    if (! is_exp_permitted) {
        WRegWrite128DW (vreg3, res.Get64(1), res.Get64(0));
    }
    m_trace->RecordSReg<TraceCtrl::REG_WRITE> (CF_SR_FXSR, m_srf->SrFXSR()->ReadBody());
}


/*!
 * @brief Executes the instruction of FMAFS4. : FMAFS4命令
 * @param opcode Operation Code. : 命令コード
 */
void InstV850::Inst_FMAFS4 (uint64_t opcode)
{
    uint32_t vreg1 = OperandExtract (opcode,  4,  0);
    uint32_t vreg2 = OperandExtract (opcode, 15, 11);
    uint32_t vreg3 = OperandExtract (opcode, 31, 27);

    uint64_t v1_dw0, v1_dw1;
    WRegRead128DW (vreg1, &v1_dw1, &v1_dw0);
    WRegData128 v1 (v1_dw1, v1_dw0);

    uint64_t v2_dw0, v2_dw1;
    WRegRead128DW (vreg2, &v2_dw1, &v2_dw0);
    WRegData128 v2 (v2_dw1, v2_dw0);

    uint64_t v3_dw0, v3_dw1;
    WRegRead128DW (vreg3, &v3_dw1, &v3_dw0);
    WRegData128 v3 (v3_dw1, v3_dw0);

    bool is_exp_permitted = false;
    WRegData128 res = FMAFS (v1, v2, v3, &is_exp_permitted);

    if (! is_exp_permitted) {
        WRegWrite128DW (vreg3, res.Get64(1), res.Get64(0));
    }
    m_trace->RecordSReg<TraceCtrl::REG_WRITE> (CF_SR_FXSR, m_srf->SrFXSR()->ReadBody());
}


/*!
 * @brief Executes the instruction of FMSFS4. : FMSFS4命令
 * @param opcode Operation Code. : 命令コード
 */
void InstV850::Inst_FMSFS4 (uint64_t opcode)
{
    uint32_t vreg1 = OperandExtract (opcode,  4,  0);
    uint32_t vreg2 = OperandExtract (opcode, 15, 11);
    uint32_t vreg3 = OperandExtract (opcode, 31, 27);

    uint64_t v1_dw0, v1_dw1;
    WRegRead128DW (vreg1, &v1_dw1, &v1_dw0);
    WRegData128 v1 (v1_dw1, v1_dw0);

    uint64_t v2_dw0, v2_dw1;
    WRegRead128DW (vreg2, &v2_dw1, &v2_dw0);
    WRegData128 v2 (v2_dw1, v2_dw0);

    uint64_t v3_dw0, v3_dw1;
    WRegRead128DW (vreg3, &v3_dw1, &v3_dw0);
    WRegData128 v3 (v3_dw1, v3_dw0);

    bool is_exp_permitted = false;
    WRegData128 res = FMSFS (v1, v2, v3, &is_exp_permitted);

    if (! is_exp_permitted) {
        WRegWrite128DW (vreg3, res.Get64(1), res.Get64(0));
    }
    m_trace->RecordSReg<TraceCtrl::REG_WRITE> (CF_SR_FXSR, m_srf->SrFXSR()->ReadBody());
}


/*!
 * @brief Executes the instruction of FNMAFS4. : FNMAFS4命令
 * @param opcode Operation Code. : 命令コード
 */
void InstV850::Inst_FNMAFS4 (uint64_t opcode)
{
    uint32_t vreg1 = OperandExtract (opcode,  4,  0);
    uint32_t vreg2 = OperandExtract (opcode, 15, 11);
    uint32_t vreg3 = OperandExtract (opcode, 31, 27);

    uint64_t v1_dw0, v1_dw1;
    WRegRead128DW (vreg1, &v1_dw1, &v1_dw0);
    WRegData128 v1 (v1_dw1, v1_dw0);

    uint64_t v2_dw0, v2_dw1;
    WRegRead128DW (vreg2, &v2_dw1, &v2_dw0);
    WRegData128 v2 (v2_dw1, v2_dw0);

    uint64_t v3_dw0, v3_dw1;
    WRegRead128DW (vreg3, &v3_dw1, &v3_dw0);
    WRegData128 v3 (v3_dw1, v3_dw0);

    bool is_exp_permitted = false;
    WRegData128 res = FNMAFS (v1, v2, v3, &is_exp_permitted);

    if (! is_exp_permitted) {
        WRegWrite128DW (vreg3, res.Get64(1), res.Get64(0));
    }
    m_trace->RecordSReg<TraceCtrl::REG_WRITE> (CF_SR_FXSR, m_srf->SrFXSR()->ReadBody());
}


/*!
 * @brief Executes the instruction of FNMSFS4. : FNMSFS4命令
 * @param opcode Operation Code. : 命令コード
 */
void InstV850::Inst_FNMSFS4 (uint64_t opcode)
{
    uint32_t vreg1 = OperandExtract (opcode,  4,  0);
    uint32_t vreg2 = OperandExtract (opcode, 15, 11);
    uint32_t vreg3 = OperandExtract (opcode, 31, 27);

    uint64_t v1_dw0, v1_dw1;
    WRegRead128DW (vreg1, &v1_dw1, &v1_dw0);
    WRegData128 v1 (v1_dw1, v1_dw0);

    uint64_t v2_dw0, v2_dw1;
    WRegRead128DW (vreg2, &v2_dw1, &v2_dw0);
    WRegData128 v2 (v2_dw1, v2_dw0);

    uint64_t v3_dw0, v3_dw1;
    WRegRead128DW (vreg3, &v3_dw1, &v3_dw0);
    WRegData128 v3 (v3_dw1, v3_dw0);

    bool is_exp_permitted = false;
    WRegData128 res = FNMSFS (v1, v2, v3, &is_exp_permitted);

    if (! is_exp_permitted) {
        WRegWrite128DW (vreg3, res.Get64(1), res.Get64(0));
    }
    m_trace->RecordSReg<TraceCtrl::REG_WRITE> (CF_SR_FXSR, m_srf->SrFXSR()->ReadBody());
}


/*!
 * @brief Executes the instruction of FLPVS4. : FLPVS4命令
 * @param opcode Operation Code. : 命令コード
 */
void InstV850::Inst_FLPVS4 (uint64_t opcode)
{
    uint32_t vreg2 = OperandExtract (opcode, 15, 11);
    uint32_t vreg3 = OperandExtract (opcode, 31, 27);
    uint32_t imm2  = OperandExtract (opcode,  1,  0);

    uint64_t v2_dw0, v2_dw1;
    WRegRead128DW (vreg2, &v2_dw1, &v2_dw0);
    WRegData128 v2 (v2_dw1, v2_dw0);

    WRegData128 res = FLPVS (v2, imm2);

    WRegWrite128DW (vreg3, res.Get64(1), res.Get64(0));
    m_trace->RecordSReg<TraceCtrl::REG_WRITE> (CF_SR_FXSR, m_srf->SrFXSR()->ReadBody());
}


/*!
 * @brief Executes the instruction of ADDRFS4. : ADDRFS4命令
 * @param opcode Operation Code. : 命令コード
 */
void InstV850::Inst_ADDRFS4 (uint64_t opcode)
{
    uint32_t vreg1 = OperandExtract (opcode,  4,  0);
    uint32_t vreg2 = OperandExtract (opcode, 15, 11);
    uint32_t vreg3 = OperandExtract (opcode, 31, 27);

    uint64_t v1_dw0, v1_dw1;
    WRegRead128DW (vreg1, &v1_dw1, &v1_dw0);
    WRegData128 v1 (v1_dw1, v1_dw0);

    uint64_t v2_dw0, v2_dw1;
    WRegRead128DW (vreg2, &v2_dw1, &v2_dw0);
    WRegData128 v2 (v2_dw1, v2_dw0);

    bool is_exp_permitted = false;
    WRegData128 res = ADDRFS (v1, v2, &is_exp_permitted);

    if (! is_exp_permitted) {
        WRegWrite128DW (vreg3, res.Get64(1), res.Get64(0));
    }
    m_trace->RecordSReg<TraceCtrl::REG_WRITE> (CF_SR_FXSR, m_srf->SrFXSR()->ReadBody());
}


/*!
 * @brief Executes the instruction of SUBRFS4. : SUBRFS4命令
 * @param opcode Operation Code. : 命令コード
 */
void InstV850::Inst_SUBRFS4 (uint64_t opcode)
{
    uint32_t vreg1 = OperandExtract (opcode,  4,  0);
    uint32_t vreg2 = OperandExtract (opcode, 15, 11);
    uint32_t vreg3 = OperandExtract (opcode, 31, 27);

    uint64_t v1_dw0, v1_dw1;
    WRegRead128DW (vreg1, &v1_dw1, &v1_dw0);
    WRegData128 v1 (v1_dw1, v1_dw0);

    uint64_t v2_dw0, v2_dw1;
    WRegRead128DW (vreg2, &v2_dw1, &v2_dw0);
    WRegData128 v2 (v2_dw1, v2_dw0);

    bool is_exp_permitted = false;
    WRegData128 res = SUBRFS (v1, v2, &is_exp_permitted);

    if (! is_exp_permitted) {
        WRegWrite128DW (vreg3, res.Get64(1), res.Get64(0));
    }
    m_trace->RecordSReg<TraceCtrl::REG_WRITE> (CF_SR_FXSR, m_srf->SrFXSR()->ReadBody());
}


/*!
 * @brief Executes the instruction of MULRFS4. : MULRFS4命令
 * @param opcode Operation Code. : 命令コード
 */
void InstV850::Inst_MULRFS4 (uint64_t opcode)
{
    uint32_t vreg1 = OperandExtract (opcode,  4,  0);
    uint32_t vreg2 = OperandExtract (opcode, 15, 11);
    uint32_t vreg3 = OperandExtract (opcode, 31, 27);

    uint64_t v1_dw0, v1_dw1;
    WRegRead128DW (vreg1, &v1_dw1, &v1_dw0);
    WRegData128 v1 (v1_dw1, v1_dw0);

    uint64_t v2_dw0, v2_dw1;
    WRegRead128DW (vreg2, &v2_dw1, &v2_dw0);
    WRegData128 v2 (v2_dw1, v2_dw0);

    bool is_exp_permitted = false;
    WRegData128 res = MULRFS (v1, v2, &is_exp_permitted);

    if (! is_exp_permitted) {
        WRegWrite128DW (vreg3, res.Get64(1), res.Get64(0));
    }
    m_trace->RecordSReg<TraceCtrl::REG_WRITE> (CF_SR_FXSR, m_srf->SrFXSR()->ReadBody());
}


/*!
 * @brief Executes the instruction of MAXRFS4. : MAXRFS4命令
 * @param opcode Operation Code. : 命令コード
 */
void InstV850::Inst_MAXRFS4 (uint64_t opcode)
{
    uint32_t vreg1 = OperandExtract (opcode,  4,  0);
    uint32_t vreg2 = OperandExtract (opcode, 15, 11);
    uint32_t vreg3 = OperandExtract (opcode, 31, 27);

    uint64_t v1_dw0, v1_dw1;
    WRegRead128DW (vreg1, &v1_dw1, &v1_dw0);
    WRegData128 v1 (v1_dw1, v1_dw0);

    uint64_t v2_dw0, v2_dw1;
    WRegRead128DW (vreg2, &v2_dw1, &v2_dw0);
    WRegData128 v2 (v2_dw1, v2_dw0);

    bool is_exp_permitted = false;
    WRegData128 res = MAXRFS (v1, v2, &is_exp_permitted);

    if (! is_exp_permitted) {
        WRegWrite128DW (vreg3, res.Get64(1), res.Get64(0));
    }
    m_trace->RecordSReg<TraceCtrl::REG_WRITE> (CF_SR_FXSR, m_srf->SrFXSR()->ReadBody());
}


/*!
 * @brief Executes the instruction of MINRFS4. : MINRFS4命令
 * @param opcode Operation Code. : 命令コード
 */
void InstV850::Inst_MINRFS4 (uint64_t opcode)
{
    uint32_t vreg1 = OperandExtract (opcode,  4,  0);
    uint32_t vreg2 = OperandExtract (opcode, 15, 11);
    uint32_t vreg3 = OperandExtract (opcode, 31, 27);

    uint64_t v1_dw0, v1_dw1;
    WRegRead128DW (vreg1, &v1_dw1, &v1_dw0);
    WRegData128 v1 (v1_dw1, v1_dw0);

    uint64_t v2_dw0, v2_dw1;
    WRegRead128DW (vreg2, &v2_dw1, &v2_dw0);
    WRegData128 v2 (v2_dw1, v2_dw0);

    bool is_exp_permitted = false;
    WRegData128 res = MINRFS (v1, v2, &is_exp_permitted);

    if (! is_exp_permitted) {
        WRegWrite128DW (vreg3, res.Get64(1), res.Get64(0));
    }
    m_trace->RecordSReg<TraceCtrl::REG_WRITE> (CF_SR_FXSR, m_srf->SrFXSR()->ReadBody());
}


/*!
 * @brief Executes the instruction of ADDXFS4. : ADDXFS4命令
 * @param opcode Operation Code. : 命令コード
 */
void InstV850::Inst_ADDXFS4 (uint64_t opcode)
{
    uint32_t vreg1 = OperandExtract (opcode,  4,  0);
    uint32_t vreg2 = OperandExtract (opcode, 15, 11);
    uint32_t vreg3 = OperandExtract (opcode, 31, 27);

    uint64_t v1_dw0, v1_dw1;
    WRegRead128DW (vreg1, &v1_dw1, &v1_dw0);
    WRegData128 v1 (v1_dw1, v1_dw0);

    uint64_t v2_dw0, v2_dw1;
    WRegRead128DW (vreg2, &v2_dw1, &v2_dw0);
    WRegData128 v2 (v2_dw1, v2_dw0);

    bool is_exp_permitted = false;
    WRegData128 res = ADDXFS (v1, v2, &is_exp_permitted);

    if (! is_exp_permitted) {
        WRegWrite128DW (vreg3, res.Get64(1), res.Get64(0));
    }
    m_trace->RecordSReg<TraceCtrl::REG_WRITE> (CF_SR_FXSR, m_srf->SrFXSR()->ReadBody());
}


/*!
 * @brief Executes the instruction of SUBXFS4. : SUBXFS4命令
 * @param opcode Operation Code. : 命令コード
 */
void InstV850::Inst_SUBXFS4 (uint64_t opcode)
{
    uint32_t vreg1 = OperandExtract (opcode,  4,  0);
    uint32_t vreg2 = OperandExtract (opcode, 15, 11);
    uint32_t vreg3 = OperandExtract (opcode, 31, 27);

    uint64_t v1_dw0, v1_dw1;
    WRegRead128DW (vreg1, &v1_dw1, &v1_dw0);
    WRegData128 v1 (v1_dw1, v1_dw0);

    uint64_t v2_dw0, v2_dw1;
    WRegRead128DW (vreg2, &v2_dw1, &v2_dw0);
    WRegData128 v2 (v2_dw1, v2_dw0);

    bool is_exp_permitted = false;
    WRegData128 res = SUBXFS (v1, v2, &is_exp_permitted);

    if (! is_exp_permitted) {
        WRegWrite128DW (vreg3, res.Get64(1), res.Get64(0));
    }
    m_trace->RecordSReg<TraceCtrl::REG_WRITE> (CF_SR_FXSR, m_srf->SrFXSR()->ReadBody());
}


/*!
 * @brief Executes the instruction of MULXFS4. : MULXFS4命令
 * @param opcode Operation Code. : 命令コード
 */
void InstV850::Inst_MULXFS4 (uint64_t opcode)
{
    uint32_t vreg1 = OperandExtract (opcode,  4,  0);
    uint32_t vreg2 = OperandExtract (opcode, 15, 11);
    uint32_t vreg3 = OperandExtract (opcode, 31, 27);

    uint64_t v1_dw0, v1_dw1;
    WRegRead128DW (vreg1, &v1_dw1, &v1_dw0);
    WRegData128 v1 (v1_dw1, v1_dw0);

    uint64_t v2_dw0, v2_dw1;
    WRegRead128DW (vreg2, &v2_dw1, &v2_dw0);
    WRegData128 v2 (v2_dw1, v2_dw0);

    bool is_exp_permitted = false;
    WRegData128 res = MULXFS (v1, v2, &is_exp_permitted);

    if (! is_exp_permitted) {
        WRegWrite128DW (vreg3, res.Get64(1), res.Get64(0));
    }
    m_trace->RecordSReg<TraceCtrl::REG_WRITE> (CF_SR_FXSR, m_srf->SrFXSR()->ReadBody());
}


/*!
 * @brief Executes the instruction of ADDSUBFS4. : ADDSUBFS4命令
 * @param opcode Operation Code. : 命令コード
 */
void InstV850::Inst_ADDSUBFS4 (uint64_t opcode)
{
    uint32_t vreg1 = OperandExtract (opcode,  4,  0);
    uint32_t vreg2 = OperandExtract (opcode, 15, 11);
    uint32_t vreg3 = OperandExtract (opcode, 31, 27);

    uint64_t v1_dw0, v1_dw1;
    WRegRead128DW (vreg1, &v1_dw1, &v1_dw0);
    WRegData128 v1 (v1_dw1, v1_dw0);

    uint64_t v2_dw0, v2_dw1;
    WRegRead128DW (vreg2, &v2_dw1, &v2_dw0);
    WRegData128 v2 (v2_dw1, v2_dw0);

    bool is_exp_permitted = false;
    WRegData128 res = ADDSUBFS (v1, v2, &is_exp_permitted);

    if (! is_exp_permitted) {
        WRegWrite128DW (vreg3, res.Get64(1), res.Get64(0));
    }
    m_trace->RecordSReg<TraceCtrl::REG_WRITE> (CF_SR_FXSR, m_srf->SrFXSR()->ReadBody());
}


/*!
 * @brief Executes the instruction of SUBADDFS4. : SUBADDFS4命令
 * @param opcode Operation Code. : 命令コード
 */
void InstV850::Inst_SUBADDFS4 (uint64_t opcode)
{
    uint32_t vreg1 = OperandExtract (opcode,  4,  0);
    uint32_t vreg2 = OperandExtract (opcode, 15, 11);
    uint32_t vreg3 = OperandExtract (opcode, 31, 27);

    uint64_t v1_dw0, v1_dw1;
    WRegRead128DW (vreg1, &v1_dw1, &v1_dw0);
    WRegData128 v1 (v1_dw1, v1_dw0);

    uint64_t v2_dw0, v2_dw1;
    WRegRead128DW (vreg2, &v2_dw1, &v2_dw0);
    WRegData128 v2 (v2_dw1, v2_dw0);

    bool is_exp_permitted = false;
    WRegData128 res = SUBADDFS (v1, v2, &is_exp_permitted);

    if (! is_exp_permitted) {
        WRegWrite128DW (vreg3, res.Get64(1), res.Get64(0));
    }
    m_trace->RecordSReg<TraceCtrl::REG_WRITE> (CF_SR_FXSR, m_srf->SrFXSR()->ReadBody());
}


/*!
 * @brief Executes the instruction of ADDSUBXFS4. : ADDSUBXFS4命令
 * @param opcode Operation Code. : 命令コード
 */
void InstV850::Inst_ADDSUBXFS4 (uint64_t opcode)
{
    uint32_t vreg1 = OperandExtract (opcode,  4,  0);
    uint32_t vreg2 = OperandExtract (opcode, 15, 11);
    uint32_t vreg3 = OperandExtract (opcode, 31, 27);

    uint64_t v1_dw0, v1_dw1;
    WRegRead128DW (vreg1, &v1_dw1, &v1_dw0);
    WRegData128 v1 (v1_dw1, v1_dw0);

    uint64_t v2_dw0, v2_dw1;
    WRegRead128DW (vreg2, &v2_dw1, &v2_dw0);
    WRegData128 v2 (v2_dw1, v2_dw0);

    bool is_exp_permitted = false;
    WRegData128 res = ADDSUBXFS (v1, v2, &is_exp_permitted);

    if (! is_exp_permitted) {
        WRegWrite128DW (vreg3, res.Get64(1), res.Get64(0));
    }
    m_trace->RecordSReg<TraceCtrl::REG_WRITE> (CF_SR_FXSR, m_srf->SrFXSR()->ReadBody());
}


/*!
 * @brief Executes the instruction of SUBADDXFS4. : SUBADDXFS4命令
 * @param opcode Operation Code. : 命令コード
 */
void InstV850::Inst_SUBADDXFS4 (uint64_t opcode)
{
    uint32_t vreg1 = OperandExtract (opcode,  4,  0);
    uint32_t vreg2 = OperandExtract (opcode, 15, 11);
    uint32_t vreg3 = OperandExtract (opcode, 31, 27);

    uint64_t v1_dw0, v1_dw1;
    WRegRead128DW (vreg1, &v1_dw1, &v1_dw0);
    WRegData128 v1 (v1_dw1, v1_dw0);

    uint64_t v2_dw0, v2_dw1;
    WRegRead128DW (vreg2, &v2_dw1, &v2_dw0);
    WRegData128 v2 (v2_dw1, v2_dw0);

    bool is_exp_permitted = false;
    WRegData128 res = SUBADDXFS (v1, v2, &is_exp_permitted);

    if (! is_exp_permitted) {
        WRegWrite128DW (vreg3, res.Get64(1), res.Get64(0));
    }
    m_trace->RecordSReg<TraceCtrl::REG_WRITE> (CF_SR_FXSR, m_srf->SrFXSR()->ReadBody());
}


/*!
 * @brief Executes the instruction of ADDSUBNFS4. : ADDSUBNFS4命令
 * @param opcode Operation Code. : 命令コード
 */
void InstV850::Inst_ADDSUBNFS4 (uint64_t opcode)
{
    uint32_t vreg1 = OperandExtract (opcode,  4,  0);
    uint32_t vreg2 = OperandExtract (opcode, 15, 11);
    uint32_t vreg3 = OperandExtract (opcode, 31, 27);

    uint64_t v1_dw0, v1_dw1;
    WRegRead128DW (vreg1, &v1_dw1, &v1_dw0);
    WRegData128 v1 (v1_dw1, v1_dw0);

    uint64_t v2_dw0, v2_dw1;
    WRegRead128DW (vreg2, &v2_dw1, &v2_dw0);
    WRegData128 v2 (v2_dw1, v2_dw0);

    bool is_exp_permitted = false;
    WRegData128 res = ADDSUBNFS (v1, v2, &is_exp_permitted);

    if (! is_exp_permitted) {
        WRegWrite128DW (vreg3, res.Get64(1), res.Get64(0));
    }
    m_trace->RecordSReg<TraceCtrl::REG_WRITE> (CF_SR_FXSR, m_srf->SrFXSR()->ReadBody());
}


/*!
 * @brief Executes the instruction of SUBADDNFS4. : SUBADDNFS4命令
 * @param opcode Operation Code. : 命令コード
 */
void InstV850::Inst_SUBADDNFS4 (uint64_t opcode)
{
    uint32_t vreg1 = OperandExtract (opcode,  4,  0);
    uint32_t vreg2 = OperandExtract (opcode, 15, 11);
    uint32_t vreg3 = OperandExtract (opcode, 31, 27);

    uint64_t v1_dw0, v1_dw1;
    WRegRead128DW (vreg1, &v1_dw1, &v1_dw0);
    WRegData128 v1 (v1_dw1, v1_dw0);

    uint64_t v2_dw0, v2_dw1;
    WRegRead128DW (vreg2, &v2_dw1, &v2_dw0);
    WRegData128 v2 (v2_dw1, v2_dw0);

    bool is_exp_permitted = false;
    WRegData128 res = SUBADDNFS (v1, v2, &is_exp_permitted);

    if (! is_exp_permitted) {
        WRegWrite128DW (vreg3, res.Get64(1), res.Get64(0));
    }
    m_trace->RecordSReg<TraceCtrl::REG_WRITE> (CF_SR_FXSR, m_srf->SrFXSR()->ReadBody());
}


/*!
 * @brief Executes the instruction of ADDSUBNXFS4. : ADDSUBNXFS4命令
 * @param opcode Operation Code. : 命令コード
 */
void InstV850::Inst_ADDSUBNXFS4 (uint64_t opcode)
{
    uint32_t vreg1 = OperandExtract (opcode,  4,  0);
    uint32_t vreg2 = OperandExtract (opcode, 15, 11);
    uint32_t vreg3 = OperandExtract (opcode, 31, 27);

    uint64_t v1_dw0, v1_dw1;
    WRegRead128DW (vreg1, &v1_dw1, &v1_dw0);
    WRegData128 v1 (v1_dw1, v1_dw0);

    uint64_t v2_dw0, v2_dw1;
    WRegRead128DW (vreg2, &v2_dw1, &v2_dw0);
    WRegData128 v2 (v2_dw1, v2_dw0);

    bool is_exp_permitted = false;
    WRegData128 res = ADDSUBNXFS (v1, v2, &is_exp_permitted);

    if (! is_exp_permitted) {
        WRegWrite128DW (vreg3, res.Get64(1), res.Get64(0));
    }
    m_trace->RecordSReg<TraceCtrl::REG_WRITE> (CF_SR_FXSR, m_srf->SrFXSR()->ReadBody());
}


/*!
 * @brief Executes the instruction of SUBADDNXFS4. : SUBADDNXFS4命令
 * @param opcode Operation Code. : 命令コード
 */
void InstV850::Inst_SUBADDNXFS4 (uint64_t opcode)
{
    uint32_t vreg1 = OperandExtract (opcode,  4,  0);
    uint32_t vreg2 = OperandExtract (opcode, 15, 11);
    uint32_t vreg3 = OperandExtract (opcode, 31, 27);

    uint64_t v1_dw0, v1_dw1;
    WRegRead128DW (vreg1, &v1_dw1, &v1_dw0);
    WRegData128 v1 (v1_dw1, v1_dw0);

    uint64_t v2_dw0, v2_dw1;
    WRegRead128DW (vreg2, &v2_dw1, &v2_dw0);
    WRegData128 v2 (v2_dw1, v2_dw0);

    bool is_exp_permitted = false;
    WRegData128 res = SUBADDNXFS (v1, v2, &is_exp_permitted);

    if (! is_exp_permitted) {
        WRegWrite128DW (vreg3, res.Get64(1), res.Get64(0));
    }
    m_trace->RecordSReg<TraceCtrl::REG_WRITE> (CF_SR_FXSR, m_srf->SrFXSR()->ReadBody());
}


/*!
 * @brief Executes the instruction of TRFSRVW4. : TRFSRVW4命令
 * @param opcode Operation Code. : 命令コード
 */
void InstV850::Inst_TRFSRVW4 (uint64_t opcode)
{
    uint32_t vreg2 = OperandExtract (opcode, 15, 11);
    uint32_t imm3  = OperandExtract (opcode, 29, 27);

    uint32_t w[4];
    WRegRead128W (vreg2, &w[3], &w[2], &w[1], &w[0]);
    std::vector<bool> tmp;
    for (uint32_t i = 0; i < 4; i++) {
        bool r = (w[i] == 0) ? 0 : 1;
        tmp.push_back (r);
    }

    switch (imm3) {
    case 0: SetPswZ (tmp[0]); break;
    case 1: SetPswZ (tmp[1]); break;
    case 2: SetPswZ (tmp[2]); break;
    case 3: SetPswZ (tmp[3]); break;
    case 4: SetPswZ (tmp[3] & tmp[2] & tmp[1] & tmp[0]); break;
    case 5: SetPswZ (tmp[3] | tmp[2] | tmp[1] | tmp[0]); break;
    case 6: SetPswZ (0); break;
    case 7: SetPswZ (0); break;
    default: break;
    }
}


/*!
 * @brief Executes the instruction of MOVVW4. : MOVVW4命令
 * @param opcode Operation Code. : 命令コード
 */
void InstV850::Inst_MOVVW4 (uint64_t opcode)
{
    uint32_t vreg2 = OperandExtract (opcode, 15, 11);
    uint32_t vreg3 = OperandExtract (opcode, 31, 27);

    uint64_t v2_dw0, v2_dw1;
    WRegRead128DW (vreg2, &v2_dw1, &v2_dw0);

    WRegWrite128DW (vreg3, v2_dw1, v2_dw0);
}


/*!
 * @brief Executes the instruction of LDVQW. : LDVQW命令
 * @param opcode Operation Code. : 命令コード
 */
void InstV850::Inst_LDVQW_D16 (uint64_t opcode)
{
    uint32_t reg1  = OperandExtract (opcode,  4,  0);
    uint32_t vreg2 = OperandExtract (opcode, 31, 27);
    uint32_t disp  = OperandExtract (opcode, 47, 36) << 4;

    uint32_t v1     = GRegRead (reg1);
    uint32_t s_disp = SignExtend (disp, 16);
    PhysAddr addr = v1 + s_disp;

#ifdef ENABLE_DBG_BREAK
    if (unlikely (m_lsu->IsHitReadBrk (addr, SIZE_128BIT) == true)) {
        return;
    }
#endif // ENABLE_DBG_BREAK

    if (unlikely (m_lsu->IsMemExceptionFXU<DATA_READ> (addr, SIZE_128BIT) == true)) {
        SetMemErrorInfo (addr, vreg2, SIZE_128BIT, MA_SIGN, DATA_READ);
        return;
    }

    MemData64 res_l;
    MemData64 res_h;
    std::tie (res_h, res_l) = m_lsu->MemRead128 (addr);

    WRegWrite128DW (vreg2, res_h, res_l);
}


/*!
 * @brief Executes the instruction of LDVDW. : LDVDW命令
 * @param opcode Operation Code. : 命令コード
 */
void InstV850::Inst_LDVDW_D16 (uint64_t opcode)
{
    uint32_t reg1   = OperandExtract (opcode,  4,  0);
    uint32_t vreg2  = OperandExtract (opcode, 31, 27);
    uint32_t valid0 = OperandExtract (opcode, 21, 21);
    uint32_t valid1 = OperandExtract (opcode, 22, 22);
    uint32_t disp   = OperandExtract (opcode, 47, 35) << 3;

    uint32_t v1     = GRegRead (reg1);
    uint32_t s_disp = SignExtend (disp, 16);
    PhysAddr addr   = v1 + s_disp;

    uint64_t v2_dw0, v2_dw1;
    WRegRead128DW (vreg2, &v2_dw1, &v2_dw0);
    WRegData128 v2 (v2_dw1, v2_dw0);

#ifdef ENABLE_DBG_BREAK
    if (unlikely (m_lsu->IsHitReadBrk (addr, SIZE_64BIT) == true)) {
        return;
    }
#endif // ENABLE_DBG_BREAK

    if (unlikely (m_lsu->IsMemExceptionFXU<DATA_READ> (addr, SIZE_64BIT) == true)) {
        SetMemErrorInfo (addr, vreg2, SIZE_64BIT, MA_SIGN, DATA_READ);
        return;
    }

    MemData64 res = m_lsu->MemRead64 (addr);

    uint32_t res_l = static_cast<uint32_t> ((res >>  0) & 0xFFFFFFFFU);
    uint32_t res_h = static_cast<uint32_t> ((res >> 32) & 0xFFFFFFFFU);

    if (valid0 == 1) {
        v2.Set32 (0, res_l);
        v2.Set32 (1, res_h);
    }
    if (valid1 == 1) {
        v2.Set32 (2, res_l);
        v2.Set32 (3, res_h);
    }

    WRegWrite128DW (vreg2, v2.Get64 (1), v2.Get64 (0));
}


/*!
 * @brief Executes the instruction of LDVW. : LDVW命令
 * @param opcode Operation Code. : 命令コード
 */
void InstV850::Inst_LDVW_D16 (uint64_t opcode)
{
    uint32_t reg1   = OperandExtract (opcode,  4,  0);
    uint32_t vreg2  = OperandExtract (opcode, 31, 27);
    uint32_t valid0 = OperandExtract (opcode, 21, 21);
    uint32_t valid1 = OperandExtract (opcode, 22, 22);
    uint32_t valid2 = OperandExtract (opcode, 23, 23);
    uint32_t valid3 = OperandExtract (opcode, 24, 24);
    uint32_t disp   = OperandExtract (opcode, 47, 34) << 2;

    uint32_t v1     = GRegRead (reg1);
    uint32_t s_disp = SignExtend (disp, 16);
    PhysAddr addr   = v1 + s_disp;

    uint64_t v2_dw0, v2_dw1;
    WRegRead128DW (vreg2, &v2_dw1, &v2_dw0);
    WRegData128 v2 (v2_dw1, v2_dw0);

#ifdef ENABLE_DBG_BREAK
    if (unlikely (m_lsu->IsHitReadBrk (addr, SIZE_32BIT) == true)) {
        return;
    }
#endif // ENABLE_DBG_BREAK

    if (unlikely (m_lsu->IsMemExceptionFXU<DATA_READ> (addr, SIZE_32BIT) == true)) {
        SetMemErrorInfo (addr, vreg2, SIZE_32BIT, MA_SIGN, DATA_READ);
        return;
    }

    uint32_t res = m_lsu->MemRead (addr, SIZE_32BIT, Lsu::SPCLD_ON);
    if (valid0 == 1) {
        v2.Set32 (0, res);
    }
    if (valid1 == 1) {
        v2.Set32 (1, res);
    }
    if (valid2 == 1) {
        v2.Set32 (2, res);
    }
    if (valid3 == 1) {
        v2.Set32 (3, res);
    }

    WRegWrite128DW (vreg2, v2.Get64(1), v2.Get64(0));
}


/*!
 * @brief Executes the instruction of STVQW. : STVQW命令
 * @param opcode Operation Code. : 命令コード
 */
void InstV850::Inst_STVQW_D16 (uint64_t opcode)
{
    uint32_t reg1  = OperandExtract (opcode,  4,  0);
    uint32_t vreg2 = OperandExtract (opcode, 31, 27);
    uint32_t disp  = OperandExtract (opcode, 47, 36) << 4;

    uint32_t v1     = GRegRead (reg1);
    uint32_t s_disp = SignExtend (disp, 16);
    PhysAddr addr   = v1 + s_disp;

    uint64_t v2_dw0, v2_dw1;
    WRegRead128DW (vreg2, &v2_dw1, &v2_dw0);

#ifdef ENABLE_DBG_BREAK
    if (unlikely (m_lsu->IsHitWriteBrk (addr, SIZE_128BIT) == true)) {
        return;
    }
#endif // ENABLE_DBG_BREAK

    bool is_mdp_suppressed = false;
    if (unlikely (m_lsu->IsMemExceptionFXU<DATA_WRITE> (addr, SIZE_128BIT) == true)) {
        if (m_trace->GetExpCode () != EXP_NONE) {
            SetMemErrorInfo (addr, vreg2, SIZE_128BIT, MA_SIGN, DATA_WRITE);
            return;
        }
        is_mdp_suppressed = true;
        m_lsu->DeleteOwnLink (addr, SIZE_128BIT);
    }

    if (is_mdp_suppressed == false) {
        m_lsu->MemWrite128 (addr, v2_dw1, v2_dw0);
    }
#if defined (ENABLE_TRACE_MDP_SUPPRESS)
    else {
        m_lsu->MemWrite128Mdp (addr, v2_dw1, v2_dw0);
    }
#endif // #if defined (ENABLE_TRACE_MDP_SUPPRESS)
}


/*!
 * @brief Executes the instruction of STVDW. : STVDW命令
 * @param opcode Operation Code. : 命令コード
 */
void InstV850::Inst_STVDW_D16 (uint64_t opcode)
{
    uint32_t reg1  = OperandExtract (opcode,  4,  0);
    uint32_t vreg2 = OperandExtract (opcode, 31, 27);
    uint32_t imm1  = OperandExtract (opcode, 21, 21);
    uint32_t disp  = OperandExtract (opcode, 47, 35) << 3;

    uint32_t v1     = GRegRead (reg1);
    uint32_t s_disp = SignExtend (disp, 16);
    PhysAddr addr   = v1 + s_disp;
    uint32_t pos    = imm1 * 2;
    uint64_t vr2;
    if (pos == 0) {
        vr2 = WRegRead64L(vreg2);
    } else {
        vr2 = WRegRead64H(vreg2);
    }

#ifdef ENABLE_DBG_BREAK
    if (unlikely (m_lsu->IsHitWriteBrk (addr, SIZE_64BIT) == true)) {
        return;
    }
#endif // ENABLE_DBG_BREAK

    bool is_mdp_suppressed = false;
    if (unlikely (m_lsu->IsMemExceptionFXU<DATA_WRITE> (addr, SIZE_64BIT) == true)) {
        if (m_trace->GetExpCode () != EXP_NONE) {
            SetMemErrorInfo (addr, vreg2, SIZE_64BIT, MA_SIGN, DATA_WRITE);
            return;
        }
        is_mdp_suppressed = true;
        m_lsu->DeleteOwnLink (addr, SIZE_64BIT);
    }

    if (is_mdp_suppressed == false) {
        m_lsu->MemWrite64 (addr, vr2);
    }
#if defined (ENABLE_TRACE_MDP_SUPPRESS)
    else {
        m_lsu->MemWrite64Mdp (addr, vr2);
    }
#endif // #if defined (ENABLE_TRACE_MDP_SUPPRESS)
}


/*!
 * @brief Executes the instruction of STVW. : STVW命令
 * @param opcode Operation Code. : 命令コード
 */
void InstV850::Inst_STVW_D16 (uint64_t opcode)
{
    uint32_t reg1  = OperandExtract (opcode,  4,  0);
    uint32_t vreg2 = OperandExtract (opcode, 31, 27);
    uint32_t imm2  = OperandExtract (opcode, 22, 21);
    uint32_t disp  = OperandExtract (opcode, 47, 34) << 2;

    uint32_t v1     = GRegRead (reg1);
    uint32_t s_disp = SignExtend (disp, 16);
    PhysAddr addr   = v1 + s_disp;
    uint32_t vr2    = WRegRead32 (vreg2, imm2);

#ifdef ENABLE_DBG_BREAK
    if (unlikely (m_lsu->IsHitWriteBrk (addr, SIZE_32BIT) == true)) {
        return;
    }
#endif // ENABLE_DBG_BREAK

    bool is_mdp_suppressed = false;
    if (unlikely (m_lsu->IsMemExceptionFXU<DATA_WRITE> (addr, SIZE_32BIT) == true)) {
        if (m_trace->GetExpCode () != EXP_NONE) {
            SetMemErrorInfo (addr, vreg2, SIZE_32BIT, MA_SIGN, DATA_WRITE);
            return;
        }
        is_mdp_suppressed = true;
        m_lsu->DeleteOwnLink (addr, SIZE_32BIT);
    }

    if (is_mdp_suppressed == false) {
        m_lsu->MemWrite (addr, vr2, SIZE_32BIT);
    }
#if defined (ENABLE_TRACE_MDP_SUPPRESS)
    else {
        m_lsu->MemWriteMdp (addr, vr2, SIZE_32BIT);
    }
#endif // #if defined (ENABLE_TRACE_MDP_SUPPRESS)
}


/*!
 * @brief Executes the instruction of LDVZH4. : LDVZH4命令
 * @param opcode Operation Code. : 命令コード
 */
void InstV850::Inst_LDVZH4_D16 (uint64_t opcode)
{
    uint32_t reg1  = OperandExtract (opcode,  4,  0);
    uint32_t disp  = OperandExtract (opcode, 47, 35) << 3;
    uint32_t vreg2 = OperandExtract (opcode, 31, 27);

    uint32_t v1     = GRegRead (reg1);
    uint32_t s_disp = SignExtend (disp, 16);
    PhysAddr addr = v1 + s_disp;

#ifdef ENABLE_DBG_BREAK
    if (unlikely (m_lsu->IsHitReadBrk (addr, SIZE_64BIT) == true)) {
        return;
    }
#endif // ENABLE_DBG_BREAK

    if (unlikely (m_lsu->IsMemExceptionFXU<DATA_READ> (addr, SIZE_64BIT) == true)) {
        SetMemErrorInfo (addr, vreg2, SIZE_64BIT, MA_SIGN, DATA_READ);
        return;
    }

    MemData64 res = m_lsu->MemRead64 (addr);
    uint32_t res_l0 = static_cast<uint32_t> ((res >>  0) & 0x0000FFFFU);
    uint32_t res_l1 = static_cast<uint32_t> ((res >> 16) & 0x0000FFFFU);
    uint32_t res_h0 = static_cast<uint32_t> ((res >> 32) & 0x0000FFFFU);
    uint32_t res_h1 = static_cast<uint32_t> ((res >> 48) & 0x0000FFFFU);

    WRegData128 v2;
    v2.Set32 (0, res_l0);
    v2.Set32 (1, res_l1);
    v2.Set32 (2, res_h0);
    v2.Set32 (3, res_h1);

    WRegWrite128DW (vreg2, v2.Get64(1), v2.Get64(0));
}


/*!
 * @brief Executes the instruction of STVZH4. : STVZH4命令
 * @param opcode Operation Code. : 命令コード
 */
void InstV850::Inst_STVZH4_D16 (uint64_t opcode)
{
    uint32_t reg1  = OperandExtract (opcode,  4,  0);
    uint32_t disp  = OperandExtract (opcode, 47, 35) << 3;
    uint32_t vreg2 = OperandExtract (opcode, 31, 27);

    uint32_t v1     = GRegRead (reg1);
    uint32_t s_disp = SignExtend (disp, 16);
    PhysAddr addr   = v1 + s_disp;
    uint32_t vr2_l0;
    uint32_t vr2_l1;
    uint32_t vr2_h0;
    uint32_t vr2_h1;
    WRegRead128W (vreg2, &vr2_h1, &vr2_h0, &vr2_l1, &vr2_l0);

#ifdef ENABLE_DBG_BREAK
    if (unlikely (m_lsu->IsHitWriteBrk (addr, SIZE_64BIT) == true)) {
        return;
    }
#endif // ENABLE_DBG_BREAK

    bool is_mdp_suppressed = false;
    if (unlikely (m_lsu->IsMemExceptionFXU<DATA_WRITE> (addr, SIZE_64BIT) == true)) {
        if (m_trace->GetExpCode () != EXP_NONE) {
            SetMemErrorInfo (addr, vreg2, SIZE_64BIT, MA_SIGN, DATA_WRITE);
            return;
        }
        is_mdp_suppressed = true;
        m_lsu->DeleteOwnLink (addr, SIZE_64BIT);
    }

    if (is_mdp_suppressed == false) {
        uint64_t wdata = ((static_cast<uint64_t> (vr2_h1) & 0x0000FFFF) << 48) |
                         ((static_cast<uint64_t> (vr2_h0) & 0x0000FFFF) << 32) |
                         ((static_cast<uint64_t> (vr2_l1) & 0x0000FFFF) << 16) |
                         ((static_cast<uint64_t> (vr2_l0) & 0x0000FFFF) <<  0);
        m_lsu->MemWrite64 (addr, wdata);
    }
#if defined (ENABLE_TRACE_MDP_SUPPRESS)
    else {
        uint64_t wdata = ((static_cast<uint64_t> (vr2_h1) & 0x0000FFFF) << 48) |
                         ((static_cast<uint64_t> (vr2_h0) & 0x0000FFFF) << 32) |
                         ((static_cast<uint64_t> (vr2_l1) & 0x0000FFFF) << 16) |
                         ((static_cast<uint64_t> (vr2_l0) & 0x0000FFFF) <<  0);
        m_lsu->MemWrite64Mdp (addr, wdata);
    }
#endif // #if defined (ENABLE_TRACE_MDP_SUPPRESS)
}


/*!
 * @brief Executes the instruction of CMOVFW4. : CMOVFW4命令
 * @param opcode Operation Code. : 命令コード
 */
void InstV850::Inst_CMOVFW4 (uint64_t opcode)
{
    uint32_t vreg1 = OperandExtract (opcode,  4,  0);
    uint32_t vreg2 = OperandExtract (opcode, 47, 43);
    uint32_t vreg3 = OperandExtract (opcode, 31, 27);
    uint32_t vreg4 = OperandExtract (opcode, 36, 32);

    uint64_t v1_dw0, v1_dw1;
    WRegRead128DW (vreg1, &v1_dw1, &v1_dw0);
    WRegData128 v1 (v1_dw1, v1_dw0);

    uint64_t v2_dw0, v2_dw1;
    WRegRead128DW (vreg2, &v2_dw1, &v2_dw0);
    WRegData128 v2 (v2_dw1, v2_dw0);

    uint64_t v4_dw0, v4_dw1;
    WRegRead128DW (vreg4, &v4_dw1, &v4_dw0);
    WRegData128 v4 (v4_dw1, v4_dw0);

    WRegData128 res = CMOVW (v4, v1, v2);

    WRegWrite128DW (vreg3, res.Get64(1), res.Get64(0));
}


/*!
 * @brief Executes the instruction of SHFLVW4. : SHFLVW4命令
 * @param opcode Operation Code. : 命令コード
 */
void InstV850::Inst_SHFLVW4 (uint64_t opcode)
{
    uint32_t vreg1 = OperandExtract (opcode,  4,  0);
    uint32_t vreg2 = OperandExtract (opcode, 47, 43);
    uint32_t vreg3 = OperandExtract (opcode, 31, 27);
    uint32_t imm12 = OperandExtract (opcode, 21, 21) << 11 |
                     OperandExtract (opcode, 42, 32);

    uint64_t v1_dw0, v1_dw1;
    WRegRead128DW (vreg1, &v1_dw1, &v1_dw0);
    WRegData128 v1 (v1_dw1, v1_dw0);

    uint64_t v2_dw0, v2_dw1;
    WRegRead128DW (vreg2, &v2_dw1, &v2_dw0);
    WRegData128 v2 (v2_dw1, v2_dw0);

    WRegData128 v3;

    for (uint32_t i = 0; i < 4; i++) {
        uint32_t sel = (imm12 >> (i*3)) & 0x7;
        uint32_t res = SHFLVW (sel, v1, v2);
        v3.Set32 (i, res);
    }
    WRegWrite128DW (vreg3, v3.Get64(1), v3.Get64(0));
}


/*!
 * @brief Executes the instruction of CMPFS. : CMPFS命令
 * @param a Operand Object. : オペランドオブジェクト
 * @param b Operand Object. : オペランドオブジェクト
 * @param fcond Condition to compare. : 比較条件
 * @param is_permitted The flag of permission for an Exception. : 例外許可フラグ
 * @return The object of the result of an arithmetic operation. : 演算結果オブジェクト
 */
template <class TVRegData>
TVRegData InstV850::CMPFS (const TVRegData &a, const TVRegData &b, uint32_t fcond,
                           bool *is_permitted)
{
    TVRegData res;
    uint32_t total_exp_code = FpuOpeBase::FPE_NONE;
    std::vector<uint32_t> exp_code_way;
    for (uint32_t way = 0; way < res.GetWayNum (); way++) {
        uint32_t exp_code = FpuOpeBase::FPE_NONE;
        FpuRegSingle v1 (a.Get32 (way));
        FpuRegSingle v2 (b.Get32 (way));
        res.Set32 (way, (m_fpsimd_ope->Cmp (v1, v2, fcond, &exp_code)) ? 0xFFFFFFFF : 0x0);
        exp_code_way.push_back (exp_code);
        total_exp_code |= exp_code;
    }
    *is_permitted = UpdateExpRegisterFXU (total_exp_code, &exp_code_way);
    return res;
}


/*!
 * @brief Executes the instruction of ROUNDFSW. : ROUNDFSW命令
 * @param a Operand Object. : オペランドオブジェクト
 * @param is_permitted The flag of permission for an Exception. : 例外許可フラグ
 * @return The object of the result of an arithmetic operation. : 演算結果オブジェクト
 */
template <class TVRegData>
TVRegData InstV850::ROUNDFSW (const TVRegData &a, bool *is_permitted)
{
    TVRegData res;
    uint32_t total_exp_code = FpuOpeBase::FPE_NONE;
    std::vector<uint32_t> exp_code_way;
    for (uint32_t way = 0; way < res.GetWayNum (); way++) {
        uint32_t exp_code = FpuOpeBase::FPE_NONE;
        FpuRegSingle v1 (a.Get32 (way));
        res.Set32 (way, (static_cast<uint32_t> (m_fpsimd_ope->Round_F32_I32 (v1, &exp_code))));
        exp_code_way.push_back (exp_code);
        total_exp_code |= exp_code;
    }
    *is_permitted = UpdateExpRegisterFXU (total_exp_code, &exp_code_way);
    return res;
}


/*!
 * @brief Executes the instruction of TRNCFSW. : TRNCFSW命令
 * @param a Operand Object. : オペランドオブジェクト
 * @param is_permitted The flag of permission for an Exception. : 例外許可フラグ
 * @return The object of the result of an arithmetic operation. : 演算結果オブジェクト
 */
template <class TVRegData>
TVRegData InstV850::TRNCFSW (const TVRegData &a, bool *is_permitted)
{
    TVRegData res;
    uint32_t total_exp_code = FpuOpeBase::FPE_NONE;
    std::vector<uint32_t> exp_code_way;
    for (uint32_t way = 0; way < res.GetWayNum (); way++) {
        uint32_t exp_code = FpuOpeBase::FPE_NONE;
        FpuRegSingle v1 (a.Get32 (way));
        res.Set32 (way, static_cast<uint32_t> (m_fpsimd_ope->Trnc_F32_I32 (v1, &exp_code)));
        exp_code_way.push_back (exp_code);
        total_exp_code |= exp_code;
    }
    *is_permitted = UpdateExpRegisterFXU (total_exp_code, &exp_code_way);
    return res;
}


/*!
 * @brief Executes the instruction of CEILFSW. : CEILFSW命令
 * @param a Operand Object. : オペランドオブジェクト
 * @param is_permitted The flag of permission for an Exception. : 例外許可フラグ
 * @return The object of the result of an arithmetic operation. : 演算結果オブジェクト
 */
template <class TVRegData>
TVRegData InstV850::CEILFSW (const TVRegData &a, bool *is_permitted)
{
    TVRegData res;
    uint32_t total_exp_code = FpuOpeBase::FPE_NONE;
    std::vector<uint32_t> exp_code_way;
    for (uint32_t way = 0; way < res.GetWayNum (); way++) {
        uint32_t exp_code = FpuOpeBase::FPE_NONE;
        FpuRegSingle v1 (a.Get32 (way));
        res.Set32 (way, static_cast<uint32_t> (m_fpsimd_ope->Ceil_F32_I32 (v1, &exp_code)));
        exp_code_way.push_back (exp_code);
        total_exp_code |= exp_code;
    }
    *is_permitted = UpdateExpRegisterFXU (total_exp_code, &exp_code_way);
    return res;
}


/*!
 * @brief Executes the instruction of FLOORFSW. : FLOORFSW命令
 * @param a Operand Object. : オペランドオブジェクト
 * @param is_permitted The flag of permission for an Exception. : 例外許可フラグ
 * @return The object of the result of an arithmetic operation. : 演算結果オブジェクト
 */
template <class TVRegData>
TVRegData InstV850::FLOORFSW (const TVRegData &a, bool *is_permitted)
{
    TVRegData res;
    uint32_t total_exp_code = FpuOpeBase::FPE_NONE;
    std::vector<uint32_t> exp_code_way;
    for (uint32_t way = 0; way < res.GetWayNum (); way++) {
        uint32_t exp_code = FpuOpeBase::FPE_NONE;
        FpuRegSingle v1 (a.Get32 (way));
        res.Set32 (way, static_cast<uint32_t> (m_fpsimd_ope->Floor_F32_I32 (v1, &exp_code)));
        exp_code_way.push_back (exp_code);
        total_exp_code |= exp_code;
    }
    *is_permitted = UpdateExpRegisterFXU (total_exp_code, &exp_code_way);
    return res;
}


/*!
 * @brief Executes the instruction of CVTFSW. : CVTFSW命令
 * @param a Operand Object. : オペランドオブジェクト
 * @param is_permitted The flag of permission for an Exception. : 例外許可フラグ
 * @return The object of the result of an arithmetic operation. : 演算結果オブジェクト
 */
template <class TVRegData>
TVRegData InstV850::CVTFSW (const TVRegData &a, bool *is_permitted)
{
    TVRegData res;
    uint32_t total_exp_code = FpuOpeBase::FPE_NONE;
    std::vector<uint32_t> exp_code_way;
    for (uint32_t way = 0; way < res.GetWayNum (); way++) {
        uint32_t exp_code = FpuOpeBase::FPE_NONE;
        FpuRegSingle v1 (a.Get32 (way));
        res.Set32 (way, static_cast<uint32_t> (m_fpsimd_ope->Cvt_F32_I32 (v1, &exp_code)));
        exp_code_way.push_back (exp_code);
        total_exp_code |= exp_code;
    }
    *is_permitted = UpdateExpRegisterFXU (total_exp_code, &exp_code_way);
    return res;
}


/*!
 * @brief Executes the instruction of ROUNDFSUW. : ROUNDFSUW命令
 * @param a Operand Object. : オペランドオブジェクト
 * @param is_permitted The flag of permission for an Exception. : 例外許可フラグ
 * @return The object of the result of an arithmetic operation. : 演算結果オブジェクト
 */
template <class TVRegData>
TVRegData InstV850::ROUNDFSUW (const TVRegData &a, bool *is_permitted)
{
    TVRegData res;
    uint32_t total_exp_code = FpuOpeBase::FPE_NONE;
    std::vector<uint32_t> exp_code_way;
    for (uint32_t way = 0; way < res.GetWayNum (); way++) {
        uint32_t exp_code = FpuOpeBase::FPE_NONE;
        FpuRegSingle v1 (a.Get32 (way));
        res.Set32 (way, m_fpsimd_ope->Round_F32_UI32 (v1, &exp_code));
        exp_code_way.push_back (exp_code);
        total_exp_code |= exp_code;
    }
    *is_permitted = UpdateExpRegisterFXU (total_exp_code, &exp_code_way);
    return res;
}


/*!
 * @brief Executes the instruction of TRNCFSUW. : TRNCFSUW命令
 * @param a Operand Object. : オペランドオブジェクト
 * @param is_permitted The flag of permission for an Exception. : 例外許可フラグ
 * @return The object of the result of an arithmetic operation. : 演算結果オブジェクト
 */
template <class TVRegData>
TVRegData InstV850::TRNCFSUW (const TVRegData &a, bool *is_permitted)
{
    TVRegData res;
    uint32_t total_exp_code = FpuOpeBase::FPE_NONE;
    std::vector<uint32_t> exp_code_way;
    for (uint32_t way = 0; way < res.GetWayNum (); way++) {
        uint32_t exp_code = FpuOpeBase::FPE_NONE;
        FpuRegSingle v1 (a.Get32 (way));
        res.Set32 (way, m_fpsimd_ope->Trnc_F32_UI32 (v1, &exp_code));
        exp_code_way.push_back (exp_code);
        total_exp_code |= exp_code;
    }
    *is_permitted = UpdateExpRegisterFXU (total_exp_code, &exp_code_way);
    return res;
}


/*!
 * @brief Executes the instruction of CEILFSUW. : CEILFSUW命令
 * @param a Operand Object. : オペランドオブジェクト
 * @param is_permitted The flag of permission for an Exception. : 例外許可フラグ
 * @return The object of the result of an arithmetic operation. : 演算結果オブジェクト
 */
template <class TVRegData>
TVRegData InstV850::CEILFSUW (const TVRegData &a, bool *is_permitted)
{
    TVRegData res;
    uint32_t total_exp_code = FpuOpeBase::FPE_NONE;
    std::vector<uint32_t> exp_code_way;
    for (uint32_t way = 0; way < res.GetWayNum (); way++) {
        uint32_t exp_code = FpuOpeBase::FPE_NONE;
        FpuRegSingle v1 (a.Get32 (way));
        res.Set32 (way, m_fpsimd_ope->Ceil_F32_UI32 (v1, &exp_code));
        exp_code_way.push_back (exp_code);
        total_exp_code |= exp_code;
    }
    *is_permitted = UpdateExpRegisterFXU (total_exp_code, &exp_code_way);
    return res;
}


/*!
 * @brief Executes the instruction of FLOORFSUW. : FLOORFSUW命令
 * @param a Operand Object. : オペランドオブジェクト
 * @param is_permitted The flag of permission for an Exception. : 例外許可フラグ
 * @return The object of the result of an arithmetic operation. : 演算結果オブジェクト
 */
template <class TVRegData>
TVRegData InstV850::FLOORFSUW (const TVRegData &a, bool *is_permitted)
{
    TVRegData res;
    uint32_t total_exp_code = FpuOpeBase::FPE_NONE;
    std::vector<uint32_t> exp_code_way;
    for (uint32_t way = 0; way < res.GetWayNum (); way++) {
        uint32_t exp_code = FpuOpeBase::FPE_NONE;
        FpuRegSingle v1 (a.Get32 (way));
        res.Set32 (way, m_fpsimd_ope->Floor_F32_UI32 (v1, &exp_code));
        exp_code_way.push_back (exp_code);
        total_exp_code |= exp_code;
    }
    *is_permitted = UpdateExpRegisterFXU (total_exp_code, &exp_code_way);
    return res;
}


/*!
 * @brief Executes the instruction of CVTFSUW. : CVTFSUW命令
 * @param a Operand Object. : オペランドオブジェクト
 * @param is_permitted The flag of permission for an Exception. : 例外許可フラグ
 * @return The object of the result of an arithmetic operation. : 演算結果オブジェクト
 */
template <class TVRegData>
TVRegData InstV850::CVTFSUW (const TVRegData &a, bool *is_permitted)
{
    TVRegData res;
    uint32_t total_exp_code = FpuOpeBase::FPE_NONE;
    std::vector<uint32_t> exp_code_way;
    for (uint32_t way = 0; way < res.GetWayNum (); way++) {
        uint32_t exp_code = FpuOpeBase::FPE_NONE;
        FpuRegSingle v1 (a.Get32 (way));
        res.Set32 (way, m_fpsimd_ope->Cvt_F32_UI32 (v1, &exp_code));
        exp_code_way.push_back (exp_code);
        total_exp_code |= exp_code;
    }
    *is_permitted = UpdateExpRegisterFXU (total_exp_code, &exp_code_way);
    return res;
}


/*!
 * @brief Executes the instruction of CVTFWS. : CVTFWS命令
 * @param a Operand Object. : オペランドオブジェクト
 * @param is_permitted The flag of permission for an Exception. : 例外許可フラグ
 * @return The object of the result of an arithmetic operation. : 演算結果オブジェクト
 */
template <class TVRegData>
TVRegData InstV850::CVTFWS (const TVRegData &a, bool *is_permitted)
{
    TVRegData res;
    uint32_t total_exp_code = FpuOpeBase::FPE_NONE;
    std::vector<uint32_t> exp_code_way;
    for (uint32_t way = 0; way < res.GetWayNum (); way++) {
        uint32_t exp_code = FpuOpeBase::FPE_NONE;
        FpuRegSingle v1 (a.Get32 (way));
        int32_t val = static_cast<int32_t> (v1.GetInteger ());
        res.SetFloat (way, (m_fpsimd_ope->Cvt_I32_F32 (val, &exp_code)).GetFloat());
        exp_code_way.push_back (exp_code);
        total_exp_code |= exp_code;
    }
    *is_permitted = UpdateExpRegisterFXU (total_exp_code, &exp_code_way);
    return res;
}


/*!
 * @brief Executes the instruction of CVTFUWS. : CVTFUWS命令
 * @param a Operand Object. : オペランドオブジェクト
 * @param is_permitted The flag of permission for an Exception. : 例外許可フラグ
 * @return The object of the result of an arithmetic operation. : 演算結果オブジェクト
 */
template <class TVRegData>
TVRegData InstV850::CVTFUWS (const TVRegData &a, bool *is_permitted)
{
    TVRegData res;
    uint32_t total_exp_code = FpuOpeBase::FPE_NONE;
    std::vector<uint32_t> exp_code_way;
    for (uint32_t way = 0; way < res.GetWayNum (); way++) {
        uint32_t exp_code = FpuOpeBase::FPE_NONE;
        FpuRegSingle v1 (a.Get32 (way));
        res.SetFloat (way, (m_fpsimd_ope->Cvt_UI32_F32 (v1.GetInteger (), &exp_code)).GetFloat());
        exp_code_way.push_back (exp_code);
        total_exp_code |= exp_code;
    }
    *is_permitted = UpdateExpRegisterFXU (total_exp_code, &exp_code_way);
    return res;
}


/*!
 * @brief Executes the instruction of CVTFHS. : CVTFHS命令
 * @param a Operand Object. : オペランドオブジェクト
 * @param is_permitted The flag of permission for an Exception. : 例外許可フラグ
 * @return The object of the result of an arithmetic operation. : 演算結果オブジェクト
 */
template <class TVRegData>
TVRegData InstV850::CVTFHS (const TVRegData &a, bool *is_permitted)
{
    TVRegData res;
    uint32_t total_exp_code = FpuOpeBase::FPE_NONE;
    std::vector<uint32_t> exp_code_way;
    for (uint32_t way = 0; way < res.GetWayNum (); way++) {
        uint32_t exp_code = FpuOpeBase::FPE_NONE;
        FpuRegHalf v1 (static_cast<uint16_t> (a.Get32 (way) & 0x0000FFFF));
        res.Set32 (way, m_fpsimd_ope->Cvt_HS (v1, &exp_code));
        exp_code_way.push_back (exp_code);
        total_exp_code |= exp_code;
    }
    *is_permitted = UpdateExpRegisterFXU (total_exp_code, &exp_code_way);
    return res;
}


/*!
 * @brief Executes the instruction of CVTFSH. : CVTFSH命令
 * @param a Operand Object. : オペランドオブジェクト
 * @param is_permitted The flag of permission for an Exception. : 例外許可フラグ
 * @return The object of the result of an arithmetic operation. : 演算結果オブジェクト
 */
template <class TVRegData>
TVRegData InstV850::CVTFSH (const TVRegData &a, bool *is_permitted)
{
    TVRegData res;
    uint32_t total_exp_code = FpuOpeBase::FPE_NONE;
    std::vector<uint32_t> exp_code_way;
    for (uint32_t way = 0; way < res.GetWayNum (); way++) {
        uint32_t exp_code = FpuOpeBase::FPE_NONE;
        FpuRegSingle v1 (a.Get32 (way));
        res.Set32 (way, m_fpsimd_ope->Cvt_SH (v1, &exp_code));
        exp_code_way.push_back (exp_code);
        total_exp_code |= exp_code;
    }
    *is_permitted = UpdateExpRegisterFXU (total_exp_code, &exp_code_way);
    return res;
}


/*!
 * @brief Executes the instruction of ABSFS. : ABSFS命令
 * @param a Operand Object. : オペランドオブジェクト
 * @return The object of the result of an arithmetic operation. : 演算結果オブジェクト
 */
template <class TVRegData>
TVRegData InstV850::ABSFS (const TVRegData &a)
{
    TVRegData res;
    for (uint32_t way = 0; way < res.GetWayNum (); way++) {
        uint32_t exp_code = FpuOpeBase::FPE_NONE;
        FpuRegSingle v1 (a.Get32 (way));
        FpuRegSingle v3 = m_fpsimd_ope->Abs (v1, &exp_code);;
        res.Set32 (way, v3.GetInteger ());
    }
    return res;
}


/*!
 * @brief Executes the instruction of NEGFS. : NEGFS命令
 * @param a Operand Object. : オペランドオブジェクト
 * @return The object of the result of an arithmetic operation. : 演算結果オブジェクト
 */
template <class TVRegData>
TVRegData InstV850::NEGFS (const TVRegData &a)
{
    TVRegData res;
    for (uint32_t way = 0; way < res.GetWayNum (); way++) {
        uint32_t exp_code = FpuOpeBase::FPE_NONE;
        FpuRegSingle v1 (a.Get32 (way));
        FpuRegSingle v3 = m_fpsimd_ope->Neg (v1, &exp_code);;
        res.Set32 (way, v3.GetInteger ());
    }
    return res;
}


/*!
 * @brief Executes the instruction of SQRTFS. : SQRTFS命令
 * @param a Operand Object. : オペランドオブジェクト
 * @param is_permitted The flag of permission for an Exception. : 例外許可フラグ
 * @return The object of the result of an arithmetic operation. : 演算結果オブジェクト
 */
template <class TVRegData>
TVRegData InstV850::SQRTFS (const TVRegData &a, bool *is_permitted)
{
    TVRegData res;
    uint32_t total_exp_code = FpuOpeBase::FPE_NONE;
    std::vector<uint32_t> exp_code_way;
    for (uint32_t way = 0; way < res.GetWayNum (); way++) {
        uint32_t exp_code = FpuOpeBase::FPE_NONE;
        FpuRegSingle v1 (a.Get32 (way));
        FpuRegSingle v3 = m_fpsimd_ope->Sqrt (v1, &exp_code);
        res.Set32 (way, v3.GetInteger ());
        exp_code_way.push_back (exp_code);
        total_exp_code |= exp_code;
    }
    *is_permitted = UpdateExpRegisterFXU (total_exp_code, &exp_code_way);
    return res;
}


/*!
 * @brief Executes the instruction of RECIPFS. : RECIPFS命令
 * @param a Operand Object. : オペランドオブジェクト
 * @param is_permitted The flag of permission for an Exception. : 例外許可フラグ
 * @return The object of the result of an arithmetic operation. : 演算結果オブジェクト
 */
template <class TVRegData>
TVRegData InstV850::RECIPFS (const TVRegData &a, bool *is_permitted)
{
    TVRegData res;
    uint32_t total_exp_code = FpuOpeBase::FPE_NONE;
    std::vector<uint32_t> exp_code_way;
    for (uint32_t way = 0; way < res.GetWayNum (); way++) {
        uint32_t exp_code = FpuOpeBase::FPE_NONE;
        FpuRegSingle v1 (a.Get32 (way));
        FpuRegSingle v3 = m_fpsimd_ope->Recip (v1, &exp_code);
        res.Set32 (way, v3.GetInteger ());
        exp_code_way.push_back (exp_code);
        total_exp_code |= exp_code;
    }
    *is_permitted = UpdateExpRegisterFXU (total_exp_code, &exp_code_way);
    return res;
}


/*!
 * @brief Executes the instruction of RSQRTFS. : RSQRTFS命令
 * @param a Operand Object. : オペランドオブジェクト
 * @param is_permitted The flag of permission for an Exception. : 例外許可フラグ
 * @return The object of the result of an arithmetic operation. : 演算結果オブジェクト
 */
template <class TVRegData>
TVRegData InstV850::RSQRTFS (const TVRegData &a, bool *is_permitted)
{
    TVRegData res;
    uint32_t total_exp_code = FpuOpeBase::FPE_NONE;
    std::vector<uint32_t> exp_code_way;
    for (uint32_t way = 0; way < res.GetWayNum (); way++) {
        uint32_t exp_code = FpuOpeBase::FPE_NONE;
        FpuRegSingle v1 (a.Get32 (way));
        FpuRegSingle v3 = m_fpsimd_ope->Rsqrt (v1, &exp_code);
        res.Set32 (way, v3.GetInteger ());
        exp_code_way.push_back (exp_code);
        total_exp_code |= exp_code;
    }
    *is_permitted = UpdateExpRegisterFXU (total_exp_code, &exp_code_way);
    return res;
}


/*!
 * @brief Executes the instruction of ADDFS. : ADDFS命令
 * @param a Operand Object. : オペランドオブジェクト
 * @param b Operand Object. : オペランドオブジェクト
 * @param is_permitted The flag of permission for an Exception. : 例外許可フラグ
 * @return The object of the result of an arithmetic operation. : 演算結果オブジェクト
 */
template <class TVRegData>
TVRegData InstV850::ADDFS (const TVRegData &a, const TVRegData &b, bool *is_permitted)
{
    TVRegData res;
    uint32_t total_exp_code = FpuOpeBase::FPE_NONE;
    std::vector<uint32_t> exp_code_way;
    for (uint32_t way = 0; way < res.GetWayNum (); way++) {
        uint32_t exp_code = FpuOpeBase::FPE_NONE;
        FpuRegSingle v1 (a.Get32 (way));
        FpuRegSingle v2 (b.Get32 (way));
        FpuRegSingle v3 = m_fpsimd_ope->Add (v2, v1, &exp_code);
        res.Set32 (way, v3.GetInteger ());
        exp_code_way.push_back (exp_code);
        total_exp_code |= exp_code;
    }
    *is_permitted = UpdateExpRegisterFXU (total_exp_code, &exp_code_way);

    return res;
}


/*!
 * @brief Executes the instruction of SUBFS. : SUBFS命令
 * @param a Operand Object. : オペランドオブジェクト
 * @param b Operand Object. : オペランドオブジェクト
 * @param is_permitted The flag of permission for an Exception. : 例外許可フラグ
 * @return The object of the result of an arithmetic operation. : 演算結果オブジェクト
 */
template <class TVRegData>
TVRegData InstV850::SUBFS (const TVRegData &a, const TVRegData &b, bool *is_permitted)
{
    TVRegData res;
    uint32_t total_exp_code = FpuOpeBase::FPE_NONE;
    std::vector<uint32_t> exp_code_way;
    for (uint32_t way = 0; way < res.GetWayNum (); way++) {
        uint32_t exp_code = FpuOpeBase::FPE_NONE;
        FpuRegSingle v1 (a.Get32 (way));
        FpuRegSingle v2 (b.Get32 (way));
        FpuRegSingle v3 = m_fpsimd_ope->Sub (v2, v1, &exp_code);
        res.Set32 (way, v3.GetInteger ());
        exp_code_way.push_back (exp_code);
        total_exp_code |= exp_code;
    }
    *is_permitted = UpdateExpRegisterFXU (total_exp_code, &exp_code_way);
    return res;
}


/*!
 * @brief Executes the instruction of MULFS. : MULFS命令
 * @param a Operand Object. : オペランドオブジェクト
 * @param b Operand Object. : オペランドオブジェクト
 * @param is_permitted The flag of permission for an Exception. : 例外許可フラグ
 * @return The object of the result of an arithmetic operation. : 演算結果オブジェクト
 */
template <class TVRegData>
TVRegData InstV850::MULFS (const TVRegData &a, const TVRegData &b, bool *is_permitted)
{
    TVRegData res;
    uint32_t total_exp_code = FpuOpeBase::FPE_NONE;
    std::vector<uint32_t> exp_code_way;
    for (uint32_t way = 0; way < res.GetWayNum (); way++) {
        uint32_t exp_code = FpuOpeBase::FPE_NONE;
        FpuRegSingle v1 (a.Get32 (way));
        FpuRegSingle v2 (b.Get32 (way));
        FpuRegSingle v3 = m_fpsimd_ope->Mul (v2, v1, &exp_code);
        res.Set32 (way, v3.GetInteger ());
        exp_code_way.push_back (exp_code);
        total_exp_code |= exp_code;
    }
    *is_permitted = UpdateExpRegisterFXU (total_exp_code, &exp_code_way);
    return res;
}


/*!
 * @brief Executes the instruction of MAXFS. : MAXFS命令
 * @param a Operand Object. : オペランドオブジェクト
 * @param b Operand Object. : オペランドオブジェクト
 * @param is_permitted The flag of permission for an Exception. : 例外許可フラグ
 * @return The object of the result of an arithmetic operation. : 演算結果オブジェクト
 */
template <class TVRegData>
TVRegData InstV850::MAXFS (const TVRegData &a, const TVRegData &b, bool *is_permitted)
{
    TVRegData res;
    uint32_t total_exp_code = FpuOpeBase::FPE_NONE;
    std::vector<uint32_t> exp_code_way;
    for (uint32_t way = 0; way < res.GetWayNum (); way++) {
        uint32_t exp_code = FpuOpeBase::FPE_NONE;
        FpuRegSingle v1 (a.Get32 (way));
        FpuRegSingle v2 (b.Get32 (way));
        FpuRegSingle v3 = m_fpsimd_ope->Max (v2, v1, &exp_code);
        res.Set32 (way, v3.GetInteger ());
        exp_code_way.push_back (exp_code);
        total_exp_code |= exp_code;
    }
    *is_permitted = UpdateExpRegisterFXU (total_exp_code, &exp_code_way);
    return res;
}


/*!
 * @brief Executes the instruction of MINFS. : MINFS命令
 * @param a Operand Object. : オペランドオブジェクト
 * @param b Operand Object. : オペランドオブジェクト
 * @param is_permitted The flag of permission for an Exception. : 例外許可フラグ
 * @return The object of the result of an arithmetic operation. : 演算結果オブジェクト
 */
template <class TVRegData>
TVRegData InstV850::MINFS (const TVRegData &a, const TVRegData &b, bool *is_permitted)
{
    TVRegData res;
    uint32_t total_exp_code = FpuOpeBase::FPE_NONE;
    std::vector<uint32_t> exp_code_way;
    for (uint32_t way = 0; way < res.GetWayNum (); way++) {
        uint32_t exp_code = FpuOpeBase::FPE_NONE;
        FpuRegSingle v1 (a.Get32 (way));
        FpuRegSingle v2 (b.Get32 (way));
        FpuRegSingle v3 = m_fpsimd_ope->Min (v2, v1, &exp_code);
        res.Set32 (way, v3.GetInteger ());
        exp_code_way.push_back (exp_code);
        total_exp_code |= exp_code;
    }
    *is_permitted = UpdateExpRegisterFXU (total_exp_code, &exp_code_way);
    return res;
}


/*!
 * @brief Executes the instruction of DIVFS. : DIVFS命令
 * @param a Operand Object. : オペランドオブジェクト
 * @param b Operand Object. : オペランドオブジェクト
 * @param is_permitted The flag of permission for an Exception. : 例外許可フラグ
 * @return The object of the result of an arithmetic operation. : 演算結果オブジェクト
 */
template <class TVRegData>
TVRegData InstV850::DIVFS (const TVRegData &a, const TVRegData &b, bool *is_permitted)
{
    TVRegData res;
    uint32_t total_exp_code = FpuOpeBase::FPE_NONE;
    std::vector<uint32_t> exp_code_way;
    for (uint32_t way = 0; way < res.GetWayNum (); way++) {
        uint32_t exp_code = FpuOpeBase::FPE_NONE;
        FpuRegSingle v1 (a.Get32 (way));
        FpuRegSingle v2 (b.Get32 (way));
        FpuRegSingle v3 = m_fpsimd_ope->Div (v2, v1, &exp_code);
        res.Set32 (way, v3.GetInteger ());
        exp_code_way.push_back (exp_code);
        total_exp_code |= exp_code;
    }
    *is_permitted = UpdateExpRegisterFXU (total_exp_code, &exp_code_way);
    return res;
}


/*!
 * @brief Executes the instruction of FMAFS. : FMAFS命令
 * @param a Operand Object. : オペランドオブジェクト
 * @param b Operand Object. : オペランドオブジェクト
 * @param c Operand Object. : オペランドオブジェクト
 * @param is_permitted The flag of permission for an Exception. : 例外許可フラグ
 * @return The object of the result of an arithmetic operation. : 演算結果オブジェクト
 */
template <class TVRegData>
TVRegData InstV850::FMAFS (const TVRegData &a, const TVRegData &b, const TVRegData &c,
                           bool *is_permitted)
{
    TVRegData res;
    uint32_t total_exp_code = FpuOpeBase::FPE_NONE;
    std::vector<uint32_t> exp_code_way;
    for (uint32_t way = 0; way < res.GetWayNum (); way++) {
        uint32_t exp_code = FpuOpeBase::FPE_NONE;
        FpuRegSingle v1 (a.Get32 (way));
        FpuRegSingle v2 (b.Get32 (way));
        FpuRegSingle v3 (c.Get32 (way));
        FpuRegSingle v4 = m_fpsimd_ope->Madd (v3, v2, v1, &exp_code);
        res.Set32 (way, v4.GetInteger ());
        exp_code_way.push_back (exp_code);
        total_exp_code |= exp_code;
    }
    *is_permitted = UpdateExpRegisterFXU (total_exp_code, &exp_code_way);
    return res;
}


/*!
 * @brief Executes the instruction of FMSFS. : FMSFS命令
 * @param a Operand Object. : オペランドオブジェクト
 * @param b Operand Object. : オペランドオブジェクト
 * @param c Operand Object. : オペランドオブジェクト
 * @param is_permitted The flag of permission for an Exception. : 例外許可フラグ
 * @return The object of the result of an arithmetic operation. : 演算結果オブジェクト
 */
template <class TVRegData>
TVRegData InstV850::FMSFS (const TVRegData &a, const TVRegData &b, const TVRegData &c,
                           bool *is_permitted)
{
    TVRegData res;
    uint32_t total_exp_code = FpuOpeBase::FPE_NONE;
    std::vector<uint32_t> exp_code_way;
    for (uint32_t way = 0; way < res.GetWayNum (); way++) {
        uint32_t exp_code = FpuOpeBase::FPE_NONE;
        FpuRegSingle v1 (a.Get32 (way));
        FpuRegSingle v2 (b.Get32 (way));
        FpuRegSingle v3 (c.Get32 (way));
        FpuRegSingle v4 = m_fpsimd_ope->Msub (v3, v2, v1, &exp_code);
        res.Set32 (way, v4.GetInteger ());
        exp_code_way.push_back (exp_code);
        total_exp_code |= exp_code;
    }
    *is_permitted = UpdateExpRegisterFXU (total_exp_code, &exp_code_way);
    return res;
}


/*!
 * @brief Executes the instruction of FNMAFS. : FNMAFS命令
 * @param a Operand Object. : オペランドオブジェクト
 * @param b Operand Object. : オペランドオブジェクト
 * @param c Operand Object. : オペランドオブジェクト
 * @param is_permitted The flag of permission for an Exception. : 例外許可フラグ
 * @return The object of the result of an arithmetic operation. : 演算結果オブジェクト
 */
template <class TVRegData>
TVRegData InstV850::FNMAFS (const TVRegData &a, const TVRegData &b, const TVRegData &c,
                            bool *is_permitted)
{
    TVRegData res;
    uint32_t total_exp_code = FpuOpeBase::FPE_NONE;
    std::vector<uint32_t> exp_code_way;
    for (uint32_t way = 0; way < res.GetWayNum (); way++) {
        uint32_t exp_code = FpuOpeBase::FPE_NONE;
        FpuRegSingle v1 (a.Get32 (way));
        FpuRegSingle v2 (b.Get32 (way));
        FpuRegSingle v3 (c.Get32 (way));
        FpuRegSingle v4 = m_fpsimd_ope->Madd (v3, v2, v1, &exp_code);
        FpuRegSingle v5;
        uint32_t tmp_exc_code = exp_code;
        // If any input is a SNAN, output will be QNAN[V] without do Mneg()
        if ((v4.GetInteger() == FpuRegSingle::FP_QNAN) && (exp_code == FpuOpeBase::FPE_V)) {
            v5 = v4;
        } else {
            v5 = m_fpsimd_ope->Mneg (v4, &exp_code);
            exp_code |= tmp_exc_code;
        }
        res.Set32 (way, v5.GetInteger ());
        exp_code_way.push_back (exp_code);
        total_exp_code |= exp_code;
    }
    *is_permitted = UpdateExpRegisterFXU (total_exp_code, &exp_code_way);
    return res;
}


/*!
 * @brief Executes the instruction of FNMSFS. : FNMSFS命令
 * @param a Operand Object. : オペランドオブジェクト
 * @param b Operand Object. : オペランドオブジェクト
 * @param c Operand Object. : オペランドオブジェクト
 * @param is_permitted The flag of permission for an Exception. : 例外許可フラグ
 * @return The object of the result of an arithmetic operation. : 演算結果オブジェクト
 */
template <class TVRegData>
TVRegData InstV850::FNMSFS (const TVRegData &a, const TVRegData &b, const TVRegData &c,
                            bool *is_permitted)
{
    TVRegData res;
    uint32_t total_exp_code = FpuOpeBase::FPE_NONE;
    std::vector<uint32_t> exp_code_way;
    for (uint32_t way = 0; way < res.GetWayNum (); way++) {
        uint32_t exp_code = FpuOpeBase::FPE_NONE;
        FpuRegSingle v1 (a.Get32 (way));
        FpuRegSingle v2 (b.Get32 (way));
        FpuRegSingle v3 (c.Get32 (way));
        FpuRegSingle v4 = m_fpsimd_ope->Msub (v3, v2, v1, &exp_code);
        FpuRegSingle v5;
        uint32_t tmp_exc_code = exp_code;
        // If any input is a SNAN, output will be QNAN[V] without do Mneg()
        if ((v4.GetInteger() == FpuRegSingle::FP_QNAN) && (exp_code == FpuOpeBase::FPE_V)) {
            v5 = v4;
        } else {
            v5 = m_fpsimd_ope->Mneg (v4, &exp_code);
            exp_code |= tmp_exc_code;
        }
        res.Set32 (way, v5.GetInteger ());
        exp_code_way.push_back (exp_code);
        total_exp_code |= exp_code;
    }
    *is_permitted = UpdateExpRegisterFXU (total_exp_code, &exp_code_way);
    return res;
}


/*!
 * @brief Executes the instruction of CMOVW. : CMOVW命令
 * @param a,b,c: オペランドオブジェクト
 * @return 結果オブジェクト
 */
template <class TVRegData>
TVRegData InstV850::CMOVW (const TVRegData &a, const TVRegData &b, const TVRegData &c)
{
    TVRegData res;
    for (uint32_t way = 0; way < res.GetWayNum (); way++) {
        uint32_t v3 = (a.Get32 (way) != 0) ? b.Get32 (way) : c.Get32 (way);
        res.Set32 (way, v3);
    }
    return res;
}


/*!
 * @brief Executes the instruction of FLPVS. : FLPVS命令
 * @param a Operand Object. : オペランドオブジェクト
 * @param imm Immidiate operand.
 * @return 結果オブジェクト
 */
template <class TVRegData>
TVRegData InstV850::FLPVS (const TVRegData &a, uint32_t imm)
{
    TVRegData res;
    uint32_t imm_l = imm & 0x1;
    uint32_t imm_h = (imm >> 1) & 0x1;

    for (uint32_t i = 0; i < res.GetWayNum () / 2 ; i++) {
        uint32_t way = 2 * i;
        if (imm_l == 1) {
            if (imm_h == 1) {
                res.Set32 (way, a.Get32 (way + 1));
                res.Set32 (way + 1, a.Get32 (way) ^ 0x80000000);
            } else {
                res.Set32 (way, a.Get32 (way + 1) ^ 0x80000000);
                res.Set32 (way + 1, a.Get32 (way));
            }
        } else {
            if (imm_h == 1) {
                res.Set32 (way, a.Get32 (way));
                res.Set32 (way + 1, a.Get32 (way + 1) ^ 0x80000000);
            } else {
                res.Set32 (way, a.Get32 (way) ^ 0x80000000);
                res.Set32 (way + 1, a.Get32 (way + 1));
            }
        }
    }
    return res;
}


/*!
 * @brief Executes the instruction of ADDRFS. : ADDRFS命令
 * @param a Operand Object. : オペランドオブジェクト
 * @param b Operand Object. : オペランドオブジェクト
 * @param is_permitted The flag of permission for an Exception. : 例外許可フラグ
 * @return The object of the result of an arithmetic operation. : 演算結果オブジェクト
 */
template <class TVRegData>
TVRegData InstV850::ADDRFS (const TVRegData &a, const TVRegData &b, bool *is_permitted)
{
    TVRegData res;
    uint32_t total_exp_code = FpuOpeBase::FPE_NONE;
    std::vector<uint32_t> exp_code_way;
    for (uint32_t way = 0; way < res.GetWayNum (); way++) {
        FpuRegSingle v1, v2;
        uint32_t pos = (way / 2) * 2;
        if (way % 2 == 0) {
            v1.SetInteger (a.Get32 (pos));
            v2.SetInteger (a.Get32 (pos + 1));
        } else {
            v1.SetInteger (b.Get32 (pos));
            v2.SetInteger (b.Get32 (pos + 1));
        }
        uint32_t exp_code = FpuOpeBase::FPE_NONE;
        FpuRegSingle v3 = m_fpsimd_ope->Add (v2, v1, &exp_code);
        res.Set32 (way, v3.GetInteger ());
        exp_code_way.push_back (exp_code);
        total_exp_code |= exp_code;
    }
    *is_permitted = UpdateExpRegisterFXU (total_exp_code, &exp_code_way);
    return res;
}


/*!
 * @brief Executes the instruction of SUBRFS. : SUBRFS命令
 * @param a Operand Object. : オペランドオブジェクト
 * @param b Operand Object. : オペランドオブジェクト
 * @param is_permitted The flag of permission for an Exception. : 例外許可フラグ
 * @return The object of the result of an arithmetic operation. : 演算結果オブジェクト
 */
template <class TVRegData>
TVRegData InstV850::SUBRFS (const TVRegData &a, const TVRegData &b, bool *is_permitted)
{
    TVRegData res;
    uint32_t total_exp_code = FpuOpeBase::FPE_NONE;
    std::vector<uint32_t> exp_code_way;
    for (uint32_t way = 0; way < res.GetWayNum (); way++) {
        FpuRegSingle v1, v2;
        uint32_t pos = (way / 2) * 2;
        if (way % 2 == 0) {
            v1.SetInteger (a.Get32 (pos));
            v2.SetInteger (a.Get32 (pos + 1));
        } else {
            v1.SetInteger (b.Get32 (pos));
            v2.SetInteger (b.Get32 (pos + 1));
        }
        uint32_t exp_code = FpuOpeBase::FPE_NONE;
        FpuRegSingle v3 = m_fpsimd_ope->Sub (v2, v1, &exp_code);
        res.Set32 (way, v3.GetInteger ());
        exp_code_way.push_back (exp_code);
        total_exp_code |= exp_code;
    }
    *is_permitted = UpdateExpRegisterFXU (total_exp_code, &exp_code_way);
    return res;
}


/*!
 * @brief Executes the instruction of MULRFS. : MULRFS命令
 * @param a Operand Object. : オペランドオブジェクト
 * @param b Operand Object. : オペランドオブジェクト
 * @param is_permitted The flag of permission for an Exception. : 例外許可フラグ
 * @return The object of the result of an arithmetic operation. : 演算結果オブジェクト
 */
template <class TVRegData>
TVRegData InstV850::MULRFS (const TVRegData &a, const TVRegData &b, bool *is_permitted)
{
    TVRegData res;
    uint32_t total_exp_code = FpuOpeBase::FPE_NONE;
    std::vector<uint32_t> exp_code_way;
    for (uint32_t way = 0; way < res.GetWayNum (); way++) {
        FpuRegSingle v1, v2;
        uint32_t pos = (way / 2) * 2;
        if (way % 2 == 0) {
            v1.SetInteger (a.Get32 (pos));
            v2.SetInteger (a.Get32 (pos + 1));
        } else {
            v1.SetInteger (b.Get32 (pos));
            v2.SetInteger (b.Get32 (pos + 1));
        }
        uint32_t exp_code = FpuOpeBase::FPE_NONE;
        FpuRegSingle v3 = m_fpsimd_ope->Mul (v2, v1, &exp_code);
        res.Set32 (way, v3.GetInteger ());
        exp_code_way.push_back (exp_code);
        total_exp_code |= exp_code;
    }
    *is_permitted = UpdateExpRegisterFXU (total_exp_code, &exp_code_way);
    return res;
}


/*!
 * @brief Executes the instruction of MAXRFS. : MAXRFS命令
 * @param a Operand Object. : オペランドオブジェクト
 * @param b Operand Object. : オペランドオブジェクト
 * @param is_permitted The flag of permission for an Exception. : 例外許可フラグ
 * @return The object of the result of an arithmetic operation. : 演算結果オブジェクト
 */
template <class TVRegData>
TVRegData InstV850::MAXRFS (const TVRegData &a, const TVRegData &b, bool *is_permitted)
{
    TVRegData res;
    uint32_t total_exp_code = FpuOpeBase::FPE_NONE;
    std::vector<uint32_t> exp_code_way;
    for (uint32_t way = 0; way < res.GetWayNum (); way++) {
        FpuRegSingle v1, v2;
        uint32_t pos = (way / 2) * 2;
        if (way % 2 == 0) {
            v1.SetInteger (a.Get32 (pos));
            v2.SetInteger (a.Get32 (pos + 1));
        } else {
            v1.SetInteger (b.Get32 (pos));
            v2.SetInteger (b.Get32 (pos + 1));
        }
        uint32_t exp_code = FpuOpeBase::FPE_NONE;
        FpuRegSingle v3 = m_fpsimd_ope->Max (v2, v1, &exp_code);
        res.Set32 (way, v3.GetInteger ());
        exp_code_way.push_back (exp_code);
        total_exp_code |= exp_code;
    }
    *is_permitted = UpdateExpRegisterFXU (total_exp_code, &exp_code_way);
    return res;
}


/*!
 * @brief Executes the instruction of MINRFS. : MINRFS命令
 * @param a Operand Object. : オペランドオブジェクト
 * @param b Operand Object. : オペランドオブジェクト
 * @param is_permitted The flag of permission for an Exception. : 例外許可フラグ
 * @return The object of the result of an arithmetic operation. : 演算結果オブジェクト
 */
template <class TVRegData>
TVRegData InstV850::MINRFS (const TVRegData &a, const TVRegData &b, bool *is_permitted)
{
    TVRegData res;
    uint32_t total_exp_code = FpuOpeBase::FPE_NONE;
    std::vector<uint32_t> exp_code_way;
    for (uint32_t way = 0; way < res.GetWayNum (); way++) {
        FpuRegSingle v1, v2;
        uint32_t pos = (way / 2) * 2;
        if (way % 2 == 0) {
            v1.SetInteger (a.Get32 (pos));
            v2.SetInteger (a.Get32 (pos + 1));
        } else {
            v1.SetInteger (b.Get32 (pos));
            v2.SetInteger (b.Get32 (pos + 1));
        }
        uint32_t exp_code = FpuOpeBase::FPE_NONE;
        FpuRegSingle v3 = m_fpsimd_ope->Min (v2, v1, &exp_code);
        res.Set32 (way, v3.GetInteger ());
        exp_code_way.push_back (exp_code);
        total_exp_code |= exp_code;
    }
    *is_permitted = UpdateExpRegisterFXU (total_exp_code, &exp_code_way);
    return res;
}


/*!
 * @brief Executes the instruction of ADDXFS. : ADDXFS命令
 * @param a Operand Object. : オペランドオブジェクト
 * @param b Operand Object. : オペランドオブジェクト
 * @param is_permitted The flag of permission for an Exception. : 例外許可フラグ
 * @return The object of the result of an arithmetic operation. : 演算結果オブジェクト
 */
template <class TVRegData>
TVRegData InstV850::ADDXFS (const TVRegData &a, const TVRegData &b, bool *is_permitted)
{
    TVRegData res;
    uint32_t total_exp_code = FpuOpeBase::FPE_NONE;
    std::vector<uint32_t> exp_code_way;
    for (uint32_t way = 0; way < res.GetWayNum (); way++) {
        FpuRegSingle v1, v2;
        uint32_t pos = (way / 2) * 2;
        if (way % 2 == 0) {
            v1.SetInteger (a.Get32 (pos + 1));
            v2.SetInteger (b.Get32 (pos));
        } else {
            v1.SetInteger (a.Get32 (pos));
            v2.SetInteger (b.Get32 (pos + 1));
        }
        uint32_t exp_code = FpuOpeBase::FPE_NONE;
        FpuRegSingle v3 = m_fpsimd_ope->Add (v2, v1, &exp_code);
        res.Set32 (way, v3.GetInteger ());
        exp_code_way.push_back (exp_code);
        total_exp_code |= exp_code;
    }
    *is_permitted = UpdateExpRegisterFXU (total_exp_code, &exp_code_way);
    return res;
}


/*!
 * @brief Executes the instruction of SUBXFS. : SUBXFS命令
 * @param a Operand Object. : オペランドオブジェクト
 * @param b Operand Object. : オペランドオブジェクト
 * @param is_permitted The flag of permission for an Exception. : 例外許可フラグ
 * @return The object of the result of an arithmetic operation. : 演算結果オブジェクト
 */
template <class TVRegData>
TVRegData InstV850::SUBXFS (const TVRegData &a, const TVRegData &b, bool *is_permitted)
{
    TVRegData res;
    uint32_t total_exp_code = FpuOpeBase::FPE_NONE;
    std::vector<uint32_t> exp_code_way;
    for (uint32_t way = 0; way < res.GetWayNum (); way++) {
        FpuRegSingle v1, v2;
        uint32_t pos = (way / 2) * 2;
        if (way % 2 == 0) {
            v1.SetInteger (a.Get32 (pos + 1));
            v2.SetInteger (b.Get32 (pos));
        } else {
            v1.SetInteger (a.Get32 (pos));
            v2.SetInteger (b.Get32 (pos + 1));
        }
        uint32_t exp_code = FpuOpeBase::FPE_NONE;
        FpuRegSingle v3 = m_fpsimd_ope->Sub (v2, v1, &exp_code);
        res.Set32 (way, v3.GetInteger ());
        exp_code_way.push_back (exp_code);
        total_exp_code |= exp_code;
    }
    *is_permitted = UpdateExpRegisterFXU (total_exp_code, &exp_code_way);
    return res;
}


/*!
 * @brief Executes the instruction of MULXFS. : MULXFS命令
 * @param a Operand Object. : オペランドオブジェクト
 * @param b Operand Object. : オペランドオブジェクト
 * @param is_permitted The flag of permission for an Exception. : 例外許可フラグ
 * @return The object of the result of an arithmetic operation. : 演算結果オブジェクト
 */
template <class TVRegData>
TVRegData InstV850::MULXFS (const TVRegData &a, const TVRegData &b, bool *is_permitted)
{
    TVRegData res;
    uint32_t total_exp_code = FpuOpeBase::FPE_NONE;
    std::vector<uint32_t> exp_code_way;
    for (uint32_t way = 0; way < res.GetWayNum (); way++) {
        FpuRegSingle v1, v2;
        uint32_t pos = (way / 2) * 2;
        if (way % 2 == 0) {
            v1.SetInteger (a.Get32 (pos + 1));
            v2.SetInteger (b.Get32 (pos));
        } else {
            v1.SetInteger (a.Get32 (pos));
            v2.SetInteger (b.Get32 (pos + 1));
        }
        uint32_t exp_code = FpuOpeBase::FPE_NONE;
        FpuRegSingle v3 = m_fpsimd_ope->Mul (v2, v1, &exp_code);
        res.Set32 (way, v3.GetInteger ());
        exp_code_way.push_back (exp_code);
        total_exp_code |= exp_code;
    }
    *is_permitted = UpdateExpRegisterFXU (total_exp_code, &exp_code_way);
    return res;
}


/*!
 * @brief Executes the instruction of ADDSUBFS. : ADDSUBFS命令
 * @param a Operand Object. : オペランドオブジェクト
 * @param b Operand Object. : オペランドオブジェクト
 * @param is_permitted The flag of permission for an Exception. : 例外許可フラグ
 * @return The object of the result of an arithmetic operation. : 演算結果オブジェクト
 */
template <class TVRegData>
TVRegData InstV850::ADDSUBFS (const TVRegData &a, const TVRegData &b, bool *is_permitted)
{
    TVRegData res;
    uint32_t total_exp_code = FpuOpeBase::FPE_NONE;
    std::vector<uint32_t> exp_code_way;
    for (uint32_t way = 0; way < res.GetWayNum (); way++) {
        FpuRegSingle v1 (a.Get32 (way));
        FpuRegSingle v2 (b.Get32 (way));
        FpuRegSingle v3;
        uint32_t exp_code = FpuOpeBase::FPE_NONE;
        if (way % 2 == 0) {
            v3 = m_fpsimd_ope->Sub (v2, v1, &exp_code);
        } else {
            v3 = m_fpsimd_ope->Add (v2, v1, &exp_code);
        }
        res.Set32 (way, v3.GetInteger ());
        exp_code_way.push_back (exp_code);
        total_exp_code |= exp_code;
    }
    *is_permitted = UpdateExpRegisterFXU (total_exp_code, &exp_code_way);
    return res;
}


/*!
 * @brief Executes the instruction of SUBADDFS. : SUBADDFS命令
 * @param a Operand Object. : オペランドオブジェクト
 * @param b Operand Object. : オペランドオブジェクト
 * @param is_permitted The flag of permission for an Exception. : 例外許可フラグ
 * @return The object of the result of an arithmetic operation. : 演算結果オブジェクト
 */
template <class TVRegData>
TVRegData InstV850::SUBADDFS (const TVRegData &a, const TVRegData &b, bool *is_permitted)
{
    TVRegData res;
    uint32_t total_exp_code = FpuOpeBase::FPE_NONE;
    std::vector<uint32_t> exp_code_way;
    for (uint32_t way = 0; way < res.GetWayNum (); way++) {
        FpuRegSingle v1 (a.Get32 (way));
        FpuRegSingle v2 (b.Get32 (way));
        FpuRegSingle v3;
        uint32_t exp_code = FpuOpeBase::FPE_NONE;
        if (way % 2 == 0) {
            v3 = m_fpsimd_ope->Add (v2, v1, &exp_code);
        } else {
            v3 = m_fpsimd_ope->Sub (v2, v1, &exp_code);
        }
        res.Set32 (way, v3.GetInteger ());
        exp_code_way.push_back (exp_code);
        total_exp_code |= exp_code;
    }
    *is_permitted = UpdateExpRegisterFXU (total_exp_code, &exp_code_way);
    return res;
}


/*!
 * @brief Executes the instruction of ADDSUBXFS. : ADDSUBXFS命令
 * @param a Operand Object. : オペランドオブジェクト
 * @param b Operand Object. : オペランドオブジェクト
 * @param is_permitted The flag of permission for an Exception. : 例外許可フラグ
 * @return The object of the result of an arithmetic operation. : 演算結果オブジェクト
 */
template <class TVRegData>
TVRegData InstV850::ADDSUBXFS (const TVRegData &a, const TVRegData &b, bool *is_permitted)
{
    TVRegData res;
    uint32_t total_exp_code = FpuOpeBase::FPE_NONE;
    std::vector<uint32_t> exp_code_way;
    for (uint32_t way = 0; way < res.GetWayNum (); way++) {
        FpuRegSingle v1, v2, v3;
        uint32_t pos = (way / 2) * 2;
        uint32_t exp_code = FpuOpeBase::FPE_NONE;
        if (way % 2 == 0) {
            v1.SetInteger (a.Get32 (pos + 1));
            v2.SetInteger (b.Get32 (pos));
            v3 = m_fpsimd_ope->Sub (v2, v1, &exp_code);
        } else {
            v1.SetInteger (a.Get32 (pos));
            v2.SetInteger (b.Get32 (pos + 1));
            v3 = m_fpsimd_ope->Add (v2, v1, &exp_code);
        }
        res.Set32 (way, v3.GetInteger ());
        exp_code_way.push_back (exp_code);
        total_exp_code |= exp_code;
    }
    *is_permitted = UpdateExpRegisterFXU (total_exp_code, &exp_code_way);
    return res;
}


/*!
 * @brief Executes the instruction of SUBADDXFS. : SUBADDXFS命令
 * @param a Operand Object. : オペランドオブジェクト
 * @param b Operand Object. : オペランドオブジェクト
 * @param is_permitted The flag of permission for an Exception. : 例外許可フラグ
 * @return The object of the result of an arithmetic operation. : 演算結果オブジェクト
 */
template <class TVRegData>
TVRegData InstV850::SUBADDXFS (const TVRegData &a, const TVRegData &b, bool *is_permitted)
{
    TVRegData res;
    uint32_t total_exp_code = FpuOpeBase::FPE_NONE;
    std::vector<uint32_t> exp_code_way;
    for (uint32_t way = 0; way < res.GetWayNum (); way++) {
        FpuRegSingle v1, v2, v3;
        uint32_t pos = (way / 2) * 2;
        uint32_t exp_code = FpuOpeBase::FPE_NONE;
        if (way % 2 == 0) {
            v1.SetInteger (a.Get32 (pos + 1));
            v2.SetInteger (b.Get32 (pos));
            v3 = m_fpsimd_ope->Add (v2, v1, &exp_code);
        } else {
            v1.SetInteger (a.Get32 (pos));
            v2.SetInteger (b.Get32 (pos + 1));
            v3 = m_fpsimd_ope->Sub (v2, v1, &exp_code);
        }
        res.Set32 (way, v3.GetInteger ());
        exp_code_way.push_back (exp_code);
        total_exp_code |= exp_code;
    }
    *is_permitted = UpdateExpRegisterFXU (total_exp_code, &exp_code_way);
    return res;
}


/*!
 * @brief Executes the instruction of ADDSUBNFS. : ADDSUBNFS命令
 * @param a Operand Object. : オペランドオブジェクト
 * @param b Operand Object. : オペランドオブジェクト
 * @param is_permitted The flag of permission for an Exception. : 例外許可フラグ
 * @return The object of the result of an arithmetic operation. : 演算結果オブジェクト
 */
template <class TVRegData>
TVRegData InstV850::ADDSUBNFS (const TVRegData &a, const TVRegData &b, bool *is_permitted)
{
    TVRegData res;
    uint32_t total_exp_code = FpuOpeBase::FPE_NONE;
    std::vector<uint32_t> exp_code_way;
    for (uint32_t way = 0; way < res.GetWayNum (); way++) {
        FpuRegSingle v1 (a.Get32 (way));
        FpuRegSingle v2 (b.Get32 (way));
        FpuRegSingle v3;
        FpuRegSingle v4;
        uint32_t exp_code = FpuOpeBase::FPE_NONE;
        if (way % 2 == 0) {
            v3 = m_fpsimd_ope->Sub (v2, v1, &exp_code);
        } else {
            v3 = m_fpsimd_ope->Add (v2, v1, &exp_code);
        }
        uint32_t tmp_exp_code = exp_code;
        // If any input is a SNAN, output will be QNAN[V] without do Mneg()
        if ((v3.GetInteger() == FpuRegSingle::FP_QNAN) && (exp_code  == FpuOpeBase::FPE_V)) {
            v4 = v3;
        } else {
            v4 = m_fpsimd_ope->Mneg (v3, &exp_code);
            exp_code |= tmp_exp_code;
        }
        res.Set32 (way, v4.GetInteger ());
        exp_code_way.push_back (exp_code);
        total_exp_code |= exp_code;
    }
    *is_permitted = UpdateExpRegisterFXU (total_exp_code, &exp_code_way);
    return res;
}


/*!
 * @brief Executes the instruction of SUBADDNFS. : SUBADDNFS命令
 * @param a Operand Object. : オペランドオブジェクト
 * @param b Operand Object. : オペランドオブジェクト
 * @param is_permitted The flag of permission for an Exception. : 例外許可フラグ
 * @return The object of the result of an arithmetic operation. : 演算結果オブジェクト
 */
template <class TVRegData>
TVRegData InstV850::SUBADDNFS (const TVRegData &a, const TVRegData &b, bool *is_permitted)
{
    TVRegData res;
    uint32_t total_exp_code = FpuOpeBase::FPE_NONE;
    std::vector<uint32_t> exp_code_way;
    for (uint32_t way = 0; way < res.GetWayNum (); way++) {
        FpuRegSingle v1 (a.Get32 (way));
        FpuRegSingle v2 (b.Get32 (way));
        FpuRegSingle v3;
        FpuRegSingle v4;
        uint32_t exp_code = FpuOpeBase::FPE_NONE;
        if (way % 2 == 0) {
            v3 = m_fpsimd_ope->Add (v2, v1, &exp_code);
        } else {
            v3 = m_fpsimd_ope->Sub (v2, v1, &exp_code);
        }
        uint32_t tmp_exp_code = exp_code;
        // If any input is a SNAN, output will be QNAN[V] without do Mneg()
        if ((v3.GetInteger() == FpuRegSingle::FP_QNAN) && (exp_code  == FpuOpeBase::FPE_V)) {
            v4 = v3;
        } else {
            v4 = m_fpsimd_ope->Mneg (v3, &exp_code);
            exp_code |= tmp_exp_code;
        }
        res.Set32 (way, v4.GetInteger ());
        exp_code_way.push_back (exp_code);
        total_exp_code |= exp_code;
    }
    *is_permitted = UpdateExpRegisterFXU (total_exp_code, &exp_code_way);
    return res;
}


/*!
 * @brief Executes the instruction of ADDSUBNXFS. : ADDSUBNXFS命令
 * @param a Operand Object. : オペランドオブジェクト
 * @param b Operand Object. : オペランドオブジェクト
 * @param is_permitted The flag of permission for an Exception. : 例外許可フラグ
 * @return The object of the result of an arithmetic operation. : 演算結果オブジェクト
 */
template <class TVRegData>
TVRegData InstV850::ADDSUBNXFS (const TVRegData &a, const TVRegData &b, bool *is_permitted)
{
    TVRegData res;
    uint32_t total_exp_code = FpuOpeBase::FPE_NONE;
    std::vector<uint32_t> exp_code_way;
    for (uint32_t way = 0; way < res.GetWayNum (); way++) {
        FpuRegSingle v1, v2, v3, v4;
        uint32_t pos = (way / 2) * 2;
        uint32_t exp_code = FpuOpeBase::FPE_NONE;
        if (way % 2 == 0) {
            v1.SetInteger (a.Get32 (pos + 1));
            v2.SetInteger (b.Get32 (pos));
            v3 = m_fpsimd_ope->Sub (v2, v1, &exp_code);
        } else {
            v1.SetInteger (a.Get32 (pos));
            v2.SetInteger (b.Get32 (pos + 1));
            v3 = m_fpsimd_ope->Add (v2, v1, &exp_code);
        }
        uint32_t tmp_exp_code = exp_code;
        // If any input is a SNAN, output will be QNAN[V] without do Mneg()
        if ((v3.GetInteger() == FpuRegSingle::FP_QNAN) && (exp_code  == FpuOpeBase::FPE_V)) {
            v4 = v3;
        } else {
            v4 = m_fpsimd_ope->Mneg (v3, &exp_code);
            exp_code |= tmp_exp_code;
        }
        res.Set32 (way, v4.GetInteger ());
        exp_code_way.push_back (exp_code);
        total_exp_code |= exp_code;
    }
    *is_permitted = UpdateExpRegisterFXU (total_exp_code, &exp_code_way);
    return res;
}


/*!
 * @brief Executes the instruction of SUBADDNXFS. : SUBADDNXFS命令
 * @param a Operand Object. : オペランドオブジェクト
 * @param b Operand Object. : オペランドオブジェクト
 * @param is_permitted The flag of permission for an Exception. : 例外許可フラグ
 * @return The object of the result of an arithmetic operation. : 演算結果オブジェクト
 */
template <class TVRegData>
TVRegData InstV850::SUBADDNXFS (const TVRegData &a, const TVRegData &b, bool *is_permitted)
{
    TVRegData res;
    uint32_t total_exp_code = FpuOpeBase::FPE_NONE;
    std::vector<uint32_t> exp_code_way;
    for (uint32_t way = 0; way < res.GetWayNum (); way++) {
        FpuRegSingle v1, v2, v3, v4;
        uint32_t pos = (way / 2) * 2;
        uint32_t exp_code = FpuOpeBase::FPE_NONE;
        if (way % 2 == 0) {
            v1.SetInteger (a.Get32 (pos + 1));
            v2.SetInteger (b.Get32 (pos));
            v3 = m_fpsimd_ope->Add (v2, v1, &exp_code);
        } else {
            v1.SetInteger (a.Get32 (pos));
            v2.SetInteger (b.Get32 (pos + 1));
            v3 = m_fpsimd_ope->Sub (v2, v1, &exp_code);
        }
        uint32_t tmp_exp_code = exp_code;
        // If any input is a SNAN, output will be QNAN[V] without do Mneg()
        if ((v3.GetInteger() == FpuRegSingle::FP_QNAN) && (exp_code  == FpuOpeBase::FPE_V)) {
            v4 = v3;
        } else {
            v4 = m_fpsimd_ope->Mneg (v3, &exp_code);
            exp_code |= tmp_exp_code;
        }
        res.Set32 (way, v4.GetInteger ());
        exp_code_way.push_back (exp_code);
        total_exp_code |= exp_code;
    }
    *is_permitted = UpdateExpRegisterFXU (total_exp_code, &exp_code_way);
    return res;
}


/*!
 * @brief Executes the instruction of SHFLVW. : SHFLVW命令
 * @param sel Selection number. : セレクト番号
 * @param a Operand Object. : オペランドオブジェクト
 * @param b Operand Object. : オペランドオブジェクト
 * @return Selected data of uint32_t. : セレクトデータ
 */
uint32_t InstV850::SHFLVW (uint32_t sel, const WRegData64 &a, const WRegData64 &b)
{
    uint32_t res = 0;
    switch (sel) {
    case 0: res = a.Get32(0); break;
    case 1: res = a.Get32(1); break;
    case 2: res = b.Get32(0); break;
    case 3: res = b.Get32(1); break;
    default: break;
    }
    return res;
}


/*!
 * @brief Executes the instruction of SHFLVW. : SHFLVW命令
 * @param sel Selection number. : セレクト番号
 * @param a Operand Object. : オペランドオブジェクト
 * @param b Operand Object. : オペランドオブジェクト
 * @return Selected data of uint32_t. : セレクトデータ
 */
uint32_t InstV850::SHFLVW (uint32_t sel, const WRegData128 &a, const WRegData128 &b)
{
    uint32_t res = 0;
    switch (sel) {
    case 0: res = a.Get32(0); break;
    case 1: res = a.Get32(1); break;
    case 2: res = a.Get32(2); break;
    case 3: res = a.Get32(3); break;
    case 4: res = b.Get32(0); break;
    case 5: res = b.Get32(1); break;
    case 6: res = b.Get32(2); break;
    case 7: res = b.Get32(3); break;
    default: break;
    }
    return res;
}


/*!
 * @brief Updates FXSR.XC, FXSR.XP, FXXC, FXXP based on exp code.
 * @param total_exp_code Total exception code.
 * @param exp_code_way The result of exception code for each way.
 * @return Exception occurence.
 * @retval : true="occurs permitted exception", false="no permitted exception".
 */
bool InstV850::UpdateExpRegisterFXU (uint32_t total_exp_code, std::vector<uint32_t> *exp_code_way)
{
    if (IsFpSimdExceptionPermitted (total_exp_code)) {
        CauseFpSimdException ();
        SetFxsrXC (total_exp_code);

        // Set FXXC for each way
        uint32_t way = 0;
        std::vector<uint32_t>::iterator it = exp_code_way->begin ();
        while (it != exp_code_way->end ()) {
            SetFxxc (way, *it);
            ++ way;
            ++ it;
        }
        // Don't record FXSR to trace here. Because FXSR.IF is updated other place.
        m_trace->RecordSReg<TraceCtrl::REG_WRITE> (CF_SR_FXXC, m_srf->SrFXXC()->ReadBody());
        return true;
    } else {
        if (total_exp_code != FpuOpeBase::FPE_NONE) {
            SetFxsrXP (total_exp_code);
            // Set FXXP for each way
            uint32_t way = 0;
            std::vector<uint32_t>::iterator it = exp_code_way->begin ();
            while (it != exp_code_way->end ()) {
                SetFxxp (way, *it);
                ++ way;
                ++ it;
            }
            // Don't record FXSR to trace here. Because FXSR.IF is updated other place.
            m_trace->RecordSReg<TraceCtrl::REG_WRITE> (CF_SR_FXXP, m_srf->SrFXXP()->ReadBody());
        }
        return false;
    }
}


/*!
 * @brief Checks FX exception. : FX例外判定
 * @param exp_code Exception code. : 例外コード
 * @return bool: [1]例外許可, [0]例外禁止
 */
bool InstV850::IsFpSimdExceptionPermitted (uint32_t exp_code) const
{
    uint32_t xe = FpuOpeBase::FPE_E | m_srf->SrFXSR()->GetXE();
    uint32_t exp_factor = exp_code & xe;
    return (exp_factor != 0) ? true : false;
}


/*!
 * @brief Causes FX exception. : FX例外発生
 * @param -
 */
void InstV850::CauseFpSimdException (void)
{
    m_trace->SetExpCode (EXP_FXE);
}


/*!
 * @brief Sets the souce bit. : 原因ビットを設定
 * @param exp_code Exception code. : 例外コード
 */
void InstV850::SetFxsrXC (uint32_t exp_code)
{
    SRegFXSR* p_fxsr = m_srf->SrFXSR();
    p_fxsr->SetXC (exp_code & 0x3F);
}


/*!
 * @brief Sets the storage bit. : 保存ビットを設定
 * @param exp_code Exception code. : 例外コード
 */
void InstV850::SetFxsrXP (uint32_t exp_code)
{
    SRegFXSR* p_fxsr = m_srf->SrFXSR();
    p_fxsr->SetXP (p_fxsr->GetXP() | (exp_code & 0x1F));
}


/*!
 * @brief Sets the source bit for each WAY. : WAY毎の原因ビットを設定
 * @param way Way. : ウェイ
 * @param exp_code Exception code. : 例外コード
 */
void InstV850::SetFxxc (uint32_t way, uint32_t exp_code)
{
    SRegFXXC* p_fxxc = m_srf->SrFXXC();
    switch (way) {
    case 0: p_fxxc->SetXC0 (exp_code & 0x3F); break;
    case 1: p_fxxc->SetXC1 (exp_code & 0x3F); break;
    case 2: p_fxxc->SetXC2 (exp_code & 0x3F); break;
    case 3: p_fxxc->SetXC3 (exp_code & 0x3F); break;
    default: break;
    }
}


/*!
 * @brief Sets the storage bit for each WAY. :WAY毎の保存ビットを設定
 * @param way Way. : ウェイ
 * @param exp_code Exception code. : 例外コード
 */
void InstV850::SetFxxp (uint32_t way, uint32_t exp_code)
{
    SRegFXXP* p_fxxp = m_srf->SrFXXP();
    switch (way) {
    case 0: p_fxxp->SetXP0 (p_fxxp->GetXP0() | (exp_code & 0x1F)); break;
    case 1: p_fxxp->SetXP1 (p_fxxp->GetXP1() | (exp_code & 0x1F)); break;
    case 2: p_fxxp->SetXP2 (p_fxxp->GetXP2() | (exp_code & 0x1F)); break;
    case 3: p_fxxp->SetXP3 (p_fxxp->GetXP3() | (exp_code & 0x1F)); break;
    default: break;
    }
}


/*!
 * @brief Writes 32 bit data to the vector register. : ベクトルレジスタ・32bitライト関数
 * @param regno Register number. : レジスタ番号
 * @param pos Way. : ウェイ
 * @param data Data to write. : ライトデータ
 */
void InstV850::WRegWriteBody (uint32_t regno, uint32_t pos, RegData data)
{
    m_grf->WriteWR (regno, pos, data);
}


/*!
 * @brief Writes 32 bit data to the vector register. : ベクトルレジスタ・32bitライト関数
 * @param regno Register number. : レジスタ番号
 * @param pos Way. : ウェイ
 * @param data Data to write. : ライトデータ
 */
void InstV850::WRegWrite32 (uint32_t regno, uint32_t pos, RegData data)
{
    WRegWriteBody (regno, pos, data);

    // Record trace
    RegData64 data_l64 = static_cast<uint64_t> (WRegReadBody (regno, 1)) << 32 |
                       static_cast<uint64_t> (WRegReadBody (regno, 0));
    RegData64 data_u64 = static_cast<uint64_t> (WRegReadBody (regno, 3)) << 32 |
                       static_cast<uint64_t> (WRegReadBody (regno, 2));
    m_trace->RecordWReg<TraceCtrl::REG_WRITE> (regno, data_u64, data_l64);
}


/*!
 * @brief Writes 64 bit data to the vector register. : ベクトルレジスタ・下位64bitライト関数
 * @param regno Register number. : レジスタ番号
 * @param data Data to write. : ライトデータ
 */
void InstV850::WRegWrite64L (uint32_t regno, RegData64 data)
{
    WRegWriteBody (regno, 0, static_cast<uint32_t> ((data >>  0) & 0xFFFFFFFFU));
    WRegWriteBody (regno, 1, static_cast<uint32_t> ((data >> 32) & 0xFFFFFFFFU));

    // Record trace
    RegData64 data_u64 = static_cast<uint64_t> (WRegReadBody (regno, 3)) << 32 |
                         static_cast<uint64_t> (WRegReadBody (regno, 2));
    m_trace->RecordWReg<TraceCtrl::REG_WRITE> (regno, data_u64, data);
}


/*!
 * @brief Writes 64 bit data to the vector register. : ベクトルレジスタ・上位64bitライト関数
 * @param regno Register number. : レジスタ番号
 * @param data Data to write. : ライトデータ
 */
void InstV850::WRegWrite64H (uint32_t regno, RegData64 data)
{
    WRegWriteBody (regno, 2, static_cast<uint32_t> ((data >>  0) & 0xFFFFFFFFU));
    WRegWriteBody (regno, 3, static_cast<uint32_t> ((data >> 32) & 0xFFFFFFFFU));

    // Record trace
    RegData64 data_l64 = static_cast<uint64_t> (WRegReadBody (regno, 1)) << 32 |
                         static_cast<uint64_t> (WRegReadBody (regno, 0));
    m_trace->RecordWReg<TraceCtrl::REG_WRITE> (regno, data, data_l64);
}


/*!
 * @brief Writes 128 bit data to the vector register.(for 2 DoubleWords) : ベクトルレジスタ・128bitライト関数 for 2 DoubleWord
 * @param regno Register number. : レジスタ番号
 * @param dw1 Data to write. : ライトデータ
 * @param dw0 Data to write. : ライトデータ
 */
void InstV850::WRegWrite128DW (uint32_t regno, RegData64 dw1, RegData64 dw0)
{
    WRegWriteBody (regno, 0, static_cast<uint32_t> ((dw0 >>  0) & 0xFFFFFFFFU));
    WRegWriteBody (regno, 1, static_cast<uint32_t> ((dw0 >> 32) & 0xFFFFFFFFU));
    WRegWriteBody (regno, 2, static_cast<uint32_t> ((dw1 >>  0) & 0xFFFFFFFFU));
    WRegWriteBody (regno, 3, static_cast<uint32_t> ((dw1 >> 32) & 0xFFFFFFFFU));

    m_trace->RecordWReg<TraceCtrl::REG_WRITE> (regno, dw1, dw0);
}


/*!
 * @brief Writes 128 bit data to the vector register.(for 4 Words) : ベクトルレジスタ・128bitライト関数 for 4 Word
 * @param regno Register number. : レジスタ番号
 * @param w3 Write data of w3.
 * @param w2 Write data of w2.
 * @param w1 Write data of w1.
 * @param w0 Write data of w0.
 */
void InstV850::WRegWrite128W (uint32_t regno, RegData w3, RegData w2, RegData w1, RegData w0)
{
    WRegWriteBody (regno, 0, w0);
    WRegWriteBody (regno, 1, w1);
    WRegWriteBody (regno, 2, w2);
    WRegWriteBody (regno, 3, w3);

    // Record trace
    RegData64 dw0 =  static_cast<uint64_t>(w0)
                  | (static_cast<uint64_t>(w1) << 32);
    RegData64 dw1 =  static_cast<uint64_t>(w2)
                  | (static_cast<uint64_t>(w3) << 32);

    m_trace->RecordWReg<TraceCtrl::REG_WRITE> (regno, dw1, dw0);
}


/*!
 * @brief ベクトルレジスタ・32bit リード関数 (No register access to trace info)
 * @param regno Register number. : レジスタ番号
 * @param pos Way. : ウェイ
 * @return RegData Data to read. : リードデータ
 */
RegData InstV850::WRegReadBody (uint32_t regno, uint32_t pos) const
{
    return m_grf->ReadWR (regno, pos);
}


/*!
 * @brief Reads 32 bit data from the vector register. : ベクトルレジスタ・32bitリード関数
 * @param regno Register number. : レジスタ番号
 * @param pos Way. : ウェイ
 * @return RegData Data to read. : リードデータ
 */
RegData InstV850::WRegRead32 (uint32_t regno, uint32_t pos) const
{
    RegData data = WRegReadBody (regno, pos);

    // Record trace
    RegData64 data_l64 = static_cast<uint64_t> (WRegReadBody (regno, 1)) << 32 |
                       static_cast<uint64_t> (WRegReadBody (regno, 0));
    RegData64 data_u64 = static_cast<uint64_t> (WRegReadBody (regno, 3)) << 32 |
                       static_cast<uint64_t> (WRegReadBody (regno, 2));
    m_trace->RecordWReg<TraceCtrl::REG_READ> (regno, data_u64, data_l64);
    return data;
}


/*!
 * @brief Reads lower 64 bit from the vector register by lower. : ベクトルレジスタ・下位64bitリード関数
 * @param regno Register number. : レジスタ番号
 * @return RegData64 Data to read. : リードデータ
 */
RegData64 InstV850::WRegRead64L (uint32_t regno) const
{
    RegData64 data64 = static_cast<uint64_t> (WRegReadBody (regno, 1)) << 32 |
                       static_cast<uint64_t> (WRegReadBody (regno, 0));

    // Record trace
    RegData64 data_u64 = static_cast<uint64_t> (WRegReadBody (regno, 3)) << 32 |
                       static_cast<uint64_t> (WRegReadBody (regno, 2));
    m_trace->RecordWReg<TraceCtrl::REG_READ> (regno, data_u64, data64);

    return data64;
}


/*!
 * @brief Reads lower 64 bit data from the vector register. : ベクトルレジスタ・上位64bitリード関数
 * @param regno Register number. : レジスタ番号
 * @return RegData64 Data to read. : リードデータ
 */
RegData64 InstV850::WRegRead64H (uint32_t regno) const
{
    RegData64 data64 = static_cast<uint64_t> (WRegReadBody (regno, 3)) << 32 |
                       static_cast<uint64_t> (WRegReadBody (regno, 2));

    // Record trace
    RegData64 data_l64 = static_cast<uint64_t> (WRegReadBody (regno, 1)) << 32 |
                       static_cast<uint64_t> (WRegReadBody (regno, 0));
    m_trace->RecordWReg<TraceCtrl::REG_READ> (regno, data64, data_l64);

    return data64;
}



/*!
 * @brief Reads 128 bit data from the vector register.(for 2 DoubleWords) : ベクトルレジスタ・128bitリード関数 for 2 DoubleWord
 * @param regno Register number. : レジスタ番号
 * @param dw1 Pointer to write read data of dw1.
 * @param dw0 Pointer to write read data of dw0.
 */
void InstV850::WRegRead128DW (uint32_t regno, RegData64* dw1, RegData64* dw0) const
{
    RegData64 data_l64 = static_cast<uint64_t> (WRegReadBody (regno, 1)) << 32 |
                       static_cast<uint64_t> (WRegReadBody (regno, 0));
    RegData64 data_u64 = static_cast<uint64_t> (WRegReadBody (regno, 3)) << 32 |
                       static_cast<uint64_t> (WRegReadBody (regno, 2));

    *dw1 = data_u64;
    *dw0 = data_l64;

    m_trace->RecordWReg<TraceCtrl::REG_READ> (regno, data_u64, data_l64);
}


/*!
 * @brief Reads 128 bit data from the vector register.(for 4 Words) : ベクトルレジスタ・128bitリード関数 for 4 Word
 * @param regno Register number. : レジスタ番号
 * @param w3 Pointer to write read data of w3.
 * @param w2 Pointer to write read data of w2.
 * @param w1 Pointer to write read data of w1.
 * @param w0 Pointer to write read data of w0.
 */
void InstV850::WRegRead128W (uint32_t regno, RegData* w3, RegData* w2, RegData* w1, RegData* w0) const
{
    RegData data0 = WRegReadBody (regno, 0);
    RegData data1 = WRegReadBody (regno, 1);
    RegData data2 = WRegReadBody (regno, 2);
    RegData data3 = WRegReadBody (regno, 3);

    *w0 = data0;
    *w1 = data1;
    *w2 = data2;
    *w3 = data3;

    RegData64 dw0 =  static_cast<uint64_t>(data0)
                  | (static_cast<uint64_t>(data1) << 32);
    RegData64 dw1 =  static_cast<uint64_t>(data2)
                  | (static_cast<uint64_t>(data3) << 32);
    m_trace->RecordWReg<TraceCtrl::REG_READ> (regno, dw1, dw0);
}
