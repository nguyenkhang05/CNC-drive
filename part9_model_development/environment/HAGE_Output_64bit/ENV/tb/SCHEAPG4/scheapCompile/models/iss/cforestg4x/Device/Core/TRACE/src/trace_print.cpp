/*
 * (c) 2011,2012,2013 Renesas Electronics Corporation
 * RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY
 * This program must be used solely for the purpose for which
 * it was furnished by Renesas Electronics Corporation. No part of this
 * program may be reproduced or disclosed to others, in any
 * form, without the prior written permission of Renesas Electronics
 * Corporation.
 */

#include <string.h>
#include <stdlib.h>
#include <string>
#include <sstream>
#include <iomanip>
#include <vector>
#include <bitset>
#include "./config.h"
#include "./forest_common.h"
#include "./forest_utility.h"
#include "./forest_message.h"
#include "./trace_info.h"
#include "./trace_print.h"
#include "./micro_arch_db.h"
#include "./regid.h"
#include "./sregfile.h"
#include "./btb_base.h"
#include "./inst_id_list.h"
#include "./exp_info.h"
#include "./inst_v850.h"

void TracePrintCForest::PrintCycleTrace (std::ostringstream* oss,
                                        SimulationMode sim_mode,
                                        const TraceCtrl* trace) const
{
#ifdef DEBUG_PRINT_STALL
    int64_t diff_cycle = trace->GetStageCycle(STAGE_EA) - trace->GetLastIssueCycle();
    *oss << std::setw(4) << diff_cycle << ' ';
#else // #ifdef DEBUG_PRINT_STALL
    if (m_core_type == CORE_G4MH) {
        *oss << std::setw(4) << trace->GetStageCycle(STAGE_EA) << ' ';
    } else {
        *oss << std::setw(4) << trace->GetStageCycle(STAGE_DP) << ' ';
    }
#endif // #ifdef DEBUG_PRINT_STALL
}


void TracePrintCForest::PrintOpcodeTrace (std::ostringstream* oss,
                                          SimulationMode sim_mode,
                                          const TraceCtrl* trace) const
{
    uint32_t inst_pc   = trace->GetInstAddr();
    ByteSize inst_size = trace->GetInstSize();
    uint64_t inst_code = trace->GetInstCode();
    *oss << std::setw(8) << inst_pc << ' ';
    if (inst_size == SIZE_16BIT) {
        *oss << "xxxx" << std::setw(4) << (inst_code & 0x00FFFFU) << ' ';
    } else {
        *oss << std::setw(8) << (inst_code & 0x00FFFFFFFFU);
        *oss << ((inst_size == SIZE_32BIT) ? ' ' : ((inst_size == SIZE_48BIT) ? 'x' : 'X'));
    }
    *oss << ' ';
}


void TracePrintCForest::PrintBasicTrace (std::ostringstream* oss,
                                         SimulationMode sim_mode,
                                         const TraceCtrl* trace) const
{
    // ステップ数
    *oss << std::setw(4) << trace->GetStepCount() << ' ';

    // サイクル数 (CSモードのみ)
    if (sim_mode == CS_MODE) {
        PrintCycleTrace (oss, sim_mode, trace);
    }

    // 命令アドレスとオペコード (48bit部はx、64bit部はXと表記)
    uint32_t inst_pc   = trace->GetInstAddr();
    uint32_t inst_id   = trace->GetInstId();
    uint64_t inst_code = trace->GetInstCode();
    *oss << std::hex << std::setfill('0');
    PrintOpcodeTrace (oss, sim_mode, trace);

    // ニーモニック
    std::string inst_mnem;
    m_print_operand.PrintOperand (inst_id, inst_code, inst_pc, &inst_mnem);
    *oss << std::left << std::setfill(' ');
    *oss << std::setw(40) << inst_mnem << ' ';
}


void TracePrintCForest::PrintExpTrace (std::ostringstream* oss,
                                       SimulationMode sim_mode,
                                       const TraceCtrl* trace) const
{
    // ステップ数
    *oss << std::setw(4) << trace->GetStepCount() << ' ';

    // サイクル数 (CSモードのみ)
    if (sim_mode == CS_MODE) {
        PrintCycleTrace (oss, sim_mode, trace);
    }

    // 命令アドレス
    uint32_t inst_pc   = trace->GetInstAddr();
    *oss << std::hex << std::setfill('0');
    *oss << std::setw(8) << inst_pc << ' ';

    // オペコード ( xxxxxxxx を表示)
    *oss << "xxxxxxxx" << ' ';
    *oss << ' ';

    // ニーモニック
    ExpCode exp_code = trace->GetExpCode ();
    std::string inst_mnem = ExpInfo::GetName(exp_code).c_str();
    inst_mnem += " [exp]";
    *oss << std::left << std::setfill(' ');
    *oss << std::setw(40) << inst_mnem << ' ';
}


#if defined(MAKE_FP_UNIT_TEST)
void TracePrintCForest::PrintUT32info (std::ostringstream* oss,
                                          const TraceCtrl* trace) const
{
    // Get value of read register
    uint32_t rreg_count = trace->GetRregCount ();
    if (rreg_count == 0x0) return;

    *oss << std::endl;
    // Add UT info
    *oss << std::left << std::setfill(' ') << std::setw(35) << "[UT_INF]: ";
    uint32_t inst_pc   = trace->GetInstAddr();
    uint32_t inst_id   = trace->GetInstId();
    uint64_t inst_code = trace->GetInstCode();

    // Add intruction name to UT info.
    std::string inst_mnem;
    m_print_operand.PrintOperand (inst_id, inst_code, inst_pc, &inst_mnem);
    *oss << std::left << std::setfill(' ');
    *oss << std::setw(40) << inst_mnem << ' ';

    *oss << std::right << std::setfill('0');

    // Get Fcond value.
    int32_t insn_id = trace->GetInstId();
    if ((insn_id == INST_ID_V_CMPFS) || ((insn_id == INST_ID_V_CMPFD))) {
        uint64_t insn_opcode = trace->GetInstCode();
        // Fcond field is in [27,30]
        uint32_t in_fcond = (insn_opcode >> 27) & 0xf;
        std::bitset<4> _fcond(in_fcond);
        *oss << "0b" << std::setw(4) << _fcond << ",";
    }

    for (uint32_t i = 0; i < rreg_count; ++i) {
        *oss << "0x";
        *oss << std::setw(8);
        *oss << trace->GetRregValue(i) << ',';
    }

    uint32_t peid_no = 0, fpsr_fn = 0, fpsr_rm = 0, fpsr_fs = 0, fpsr_xe_o = 0, fpsr_xe_u = 0;
    uint64_t cforest_sreg_data_0 = 0, cforest_sreg_data_1 = 0;
    uint32_t selid_fpsr = 0, regid_fpsr = 6;

    // Get FXSR value
    CReg sr_fpsr = 0x100 + (selid_fpsr << 5) + regid_fpsr;
    p_forest_util->RegReadDebug(peid_no, sr_fpsr, &cforest_sreg_data_0, &cforest_sreg_data_1);

    // Get FPSR.{FN, RM, FS} info
    fpsr_fn = (cforest_sreg_data_0 >> 23) & 0x1;
    fpsr_rm = (cforest_sreg_data_0 >> 18) & 0x3;
    fpsr_fs = (cforest_sreg_data_0 >> 17) & 0x1;
    fpsr_xe_o = (cforest_sreg_data_0 >> 7) & 0x1;
    fpsr_xe_u = (cforest_sreg_data_0 >> 6) & 0x1;
    std::bitset<2> _rm(fpsr_rm);
    std::bitset<1> _fn(fpsr_fn);
    std::bitset<1> _fs(fpsr_fs);
    std::bitset<1> _xe_o(fpsr_xe_o);
    std::bitset<1> _xe_u(fpsr_xe_u);

    // Print FPSR info: RM 2bit, FN 1bit, FS 1bit.
    *oss << "0b" << std::setw(2) << _rm << ",";
    *oss << "0b" << std::setw(1) << _fn << ",";
    *oss << "0b" << std::setw(1) << _fs << ",";
    *oss << "0b" << std::setw(1) << _xe_o << ",";
    *oss << "0b" << std::setw(1) << _xe_u << ",";

    // Get value of write register
    uint32_t wreg_count = trace->GetWregCount ();
    if ((insn_id == INST_ID_V_CMPFS) || ((insn_id == INST_ID_V_CMPFD))) {
        *oss << "0x00000000,";
    } else {
        for (uint32_t i = 0; i < wreg_count; ++i) {
            CReg wreg_index = trace->GetWriteReg(i);
            if (wreg_index >= R0 && wreg_index <= R31) {
                *oss << "0x00000000,";
            }
        }
    }

    *oss << "0b000000";
}


void TracePrintCForest::PrintUT128info (std::ostringstream* oss,
                                          const TraceCtrl* trace) const
{
    // Get values of read register from WRs.
    uint32_t rreg_count = trace->GetRregCount ();
    if (rreg_count == 0x0) return;

    *oss << std::endl;
    uint32_t reg[4][REC_RREG_NUM] = {0};
    for (uint32_t i = 0; i < rreg_count; ++i) {
        CReg regno = trace->GetReadReg (i);
        regno = RegID::GetRecRegId (regno);
        *oss << std::setw(8);
        if (RegID::GetRecRegId (regno) >= WREG_BASE && RegID::GetRecRegId (regno) < WREG_MAX) {
            // WREG (128bit)
            reg[3][i] = trace->GetRWregValue(i, 3);
            reg[2][i] = trace->GetRWregValue(i, 2);
            reg[1][i] = trace->GetRWregValue(i, 1);
            reg[0][i] = trace->GetRWregValue(i, 0);
        }
    }

    uint32_t peid_no = 0, fxsr_fn = 0, fxsr_rm = 0, fxsr_fs = 0, fxsr_xe_o = 0, fxsr_xe_u = 0;
    uint64_t cforest_sreg_data_0 = 0, cforest_sreg_data_1 = 0;
    uint32_t selid_fxsr = 10, regid_fxsr = 6;

    // Get FXSR value
    CReg sr_fxsr = 0x100 + (selid_fxsr << 5) + regid_fxsr;
    p_forest_util->RegReadDebug(peid_no, sr_fxsr, &cforest_sreg_data_0, &cforest_sreg_data_1);

    // Get FXSR.{FN, RM, FS} info
    fxsr_fn = (cforest_sreg_data_0 >> 23) & 0x1;
    fxsr_rm = (cforest_sreg_data_0 >> 18) & 0x3;
    fxsr_fs = (cforest_sreg_data_0 >> 17) & 0x1;
    fxsr_xe_o = (cforest_sreg_data_0 >> 7) & 0x1;
    fxsr_xe_u = (cforest_sreg_data_0 >> 6) & 0x1;
    std::bitset<2> _rm(fxsr_rm);
    std::bitset<1> _fn(fxsr_fn);
    std::bitset<1> _fs(fxsr_fs);
    std::bitset<1> _xe_o(fxsr_xe_o);
    std::bitset<1> _xe_u(fxsr_xe_u);

    for (uint32_t i = 0; i < 4; ++i) {
        // Add UT info
        *oss << std::left << std::setfill(' ') << std::setw(35) << "[UT_INF]: ";
        uint32_t inst_pc   = trace->GetInstAddr();
        uint32_t inst_id   = trace->GetInstId();
        uint64_t inst_code = trace->GetInstCode();

        // Add intruction name to UT info.
        std::string inst_mnem;
        m_print_operand.PrintOperand (inst_id, inst_code, inst_pc, &inst_mnem);
        *oss << std::left << std::setfill(' ');
        *oss << std::setw(40) << inst_mnem << ' ';

        // Get Fcond value.
        int32_t insn_id = trace->GetInstId();
        if (insn_id == INST_ID_V_CMPFS4) {
            uint64_t insn_opcode = trace->GetInstCode();
            // Fcond field is in [17,20]
            uint32_t in_fcond = (insn_opcode >> 17) & 0xf;
            std::bitset<4> _fcond(in_fcond);
            *oss << "0b" << std::setw(4) << _fcond << ",";
        }

        for (uint32_t j = 0; j < rreg_count; ++j) {
            // Get value of read register
            *oss << "0x";
            *oss << std::right << std::setfill('0') << std::setw(8) << reg[i][j] << ",";
        }

        // Print FXSR info
        *oss << "0b" << std::setw(2) << _rm << ",";
        *oss << "0b" << std::setw(1) << _fn << ",";
        *oss << "0b" << std::setw(1) << _fs << ",";
        *oss << "0b" << std::setw(1) << _xe_o << ",";
        *oss << "0b" << std::setw(1) << _xe_u << ",";
        *oss << "0x00000000," << "0b000000";
        *oss << std::endl;
    }
}
#endif


void TracePrintCForest::PrintRegMemTrace (std::ostringstream* oss,
                                          const TraceCtrl* trace) const
{
    *oss << std::right << std::setfill('0');

    // レジスタリード
    uint32_t rreg_count = trace->GetRregCount ();
    for (uint32_t i = 0; i < rreg_count; ++i) {
        CReg regno = trace->GetReadReg (i);
        *oss << RegID::GetName(regno) << "=>";
        if (regno < BIT_PRINT_MIN || regno > BIT_PRINT_MAX) {
            *oss << std::setw(8);
        }
        regno = RegID::GetRecRegId (regno);

        if (RegID::GetRecRegId (regno) >= WREG_BASE && RegID::GetRecRegId (regno) < WREG_MAX) {
            // WREG (128bit)
            RegData rdata32;
            rdata32 = trace->GetRWregValue(i, 3);
            *oss << std::setw(8) << rdata32 << '_';
            rdata32 = trace->GetRWregValue(i, 2);
            *oss << std::setw(8) << rdata32 << '_';
            rdata32 = trace->GetRWregValue(i, 1);
            *oss << std::setw(8) << rdata32 << '_';
            rdata32 = trace->GetRWregValue(i, 0);
            *oss << std::setw(8) << rdata32 << ' ';
            continue;
        }
        *oss << trace->GetRregValue(i) << ' ';
    }

    // メモリリード
#if defined (ENABLE_TRACE_MDP_SUPPRESS)
    uint32_t rmem_count = trace->GetRmemMdpCount ();
    for (uint32_t i = 0; i < rmem_count; ++i) {
        *oss << '('   << std::setw(8) << trace->GetRmemMdpAddr(i);
        if (trace->GetRmemMdpIssued(i)) {
			*oss << ")=>";
        } else {
			*oss << ")X=>";
        }
        if (trace->GetRmemMdpSize(i) == SIZE_128BIT) {
            *oss << std::setw(16) << trace->GetRmemMdpValueU64(i) << '_';
            *oss << std::setw(16) << trace->GetRmemMdpValueL64(i) << ' ';
        } else if (trace->GetRmemMdpSize(i) == SIZE_64BIT) {
            *oss << std::setw(16) << trace->GetRmemMdpValueL64(i) << ' ';
        } else {
            *oss << std::setw(8)  << trace->GetRmemMdpValueL64(i) << ' ';
        }
    }
#else // #if defined (ENABLE_TRACE_MDP_SUPPRESS)
    uint32_t rmem_count = trace->GetRmemCount ();
    for (uint32_t i = 0; i < rmem_count; ++i) {
        *oss << '('   << std::setw(8) << trace->GetRmemAddr(i) << ")=>";
        if (trace->GetRmemSize(i) == SIZE_128BIT) {
            *oss << std::setw(16) << trace->GetRmemValueU64(i) << '_';
            *oss << std::setw(16) << trace->GetRmemValueL64(i) << ' ';
        } else if (trace->GetRmemSize(i) == SIZE_64BIT) {
            *oss << std::setw(16) << trace->GetRmemValueL64(i) << ' ';
        } else {
            *oss << std::setw(8)  << trace->GetRmemValueL64(i) << ' ';
        }
    }
#endif // #if defined (ENABLE_TRACE_MDP_SUPPRESS)

    // レジスタライト
    uint32_t wreg_count = trace->GetWregCount ();
    for (uint32_t i = 0; i < wreg_count; ++i) {
        CReg regno = trace->GetWriteReg (i);
        *oss << RegID::GetName(regno) << "<=";
        if (regno < BIT_PRINT_MIN || regno > BIT_PRINT_MAX) {
            *oss << std::setw(8);
        }

        if (RegID::GetRecRegId (regno) >= WREG_BASE && RegID::GetRecRegId (regno) < WREG_MAX) {
            // WREG (128bit)
            RegData wdata32;

            wdata32 = trace->GetWWregValue(i, 3);
            *oss << std::setw(8) << wdata32 << '_';
            wdata32 = trace->GetWWregValue(i, 2);
            *oss << std::setw(8) << wdata32 << '_';
            wdata32 = trace->GetWWregValue(i, 1);
            *oss << std::setw(8) << wdata32 << '_';
            wdata32 = trace->GetWWregValue(i, 0);
            *oss << std::setw(8) << wdata32 << ' ';
            continue;
        }
        *oss << trace->GetWregValue(i) << ' ';
    }

    // メモリライト
#if defined (ENABLE_TRACE_MDP_SUPPRESS)
    uint32_t wmem_count = trace->GetWmemMdpCount();
    for (uint32_t i = 0; i < wmem_count; ++i) {
        *oss << '('   << std::setw(8) << trace->GetWmemMdpAddr(i);
        if (trace->GetWmemMdpIssued(i)) {
			*oss << ")<=";
        } else {
			*oss << ")X<=";
        }
        if (trace->GetWmemMdpSize(i) == SIZE_128BIT) {
            *oss << std::setw(16) << trace->GetWmemMdpValueU64(i) << '_';
            *oss << std::setw(16) << trace->GetWmemMdpValueL64(i) << ' ';
        } else if (trace->GetWmemMdpSize(i) == SIZE_64BIT) {
            *oss << std::setw(16) << trace->GetWmemMdpValueL64(i) << ' ';
        } else {
            *oss << std::setw(8) << trace->GetWmemMdpValueL64(i) << ' ';
        }
    }
#else // #if defined (ENABLE_TRACE_MDP_SUPPRESS)
    uint32_t wmem_count = trace->GetWmemCount();
    for (uint32_t i = 0; i < wmem_count; ++i) {
        *oss << '('   << std::setw(8) << trace->GetWmemAddr(i) << ")<=";
        if (trace->GetWmemSize(i) == SIZE_128BIT) {
            *oss << std::setw(16) << trace->GetWmemValueU64(i) << '_';
            *oss << std::setw(16) << trace->GetWmemValueL64(i) << ' ';
        } else if (trace->GetWmemSize(i) == SIZE_64BIT) {
            *oss << std::setw(16) << trace->GetWmemValueL64(i) << ' ';
        } else {
            *oss << std::setw(8) << trace->GetWmemValueL64(i) << ' ';
        }
    }
#endif // #if defined (ENABLE_TRACE_MDP_SUPPRESS)
}


void TracePrintCForest::PrintIcTrace (std::ostringstream* oss,
                                      const TraceCtrl* trace) const
{
    static const std::string ic_event_str[IC_EVT_NUM] = {
        "IC_MISS", "IC_HIT",    "IC_INV", "IC_FILL",
        "IC_LOCK", "IC_UNLOCK", "IC_ST",  "IC_LD", "IC_NOP"
    };

    *oss << std::right << std::setfill('0');

    // メモリリード
    int32_t ic_count = trace->GetIcCount ();
    for (int32_t i = 0; i < ic_count; ++i) {
        *oss << '[' << ic_event_str [trace->GetIcEvent(i)] << ':';
        *oss << 'e' << std::setw(4) << trace->GetIcEntry(i) << ':';
        *oss << 'w' << std::setw(1) << trace->GetIcWay(i) << "] ";
    }
}


void TracePrintCForest::PrintPipeTrace (std::ostringstream* oss,
                                        const TraceCtrl* trace) const
{
    // G3MHでは並列発行グループ無効なので一時的に無効化。G3Mとの切り替え対応時に変更する。
#if 0
    static const char* para_group_string[PGRP_NUM] = {
        "1_1", "1_2", "1_3", "1_4", "1_5",
        "2_1", "2_2", "2_3", "2_4", "2_5", "2_6", "2_7", "2_8",
        "3_1", "3_2", "3_3", "3_4",
        "4_1", "4_2", "4_3", "4_4",
        "5_1", "5_2", "5_3", "5_4", "5_5",
        "6_1",
        "7_1",
        "8_1", "8_2"
        "UND" // グループ未定義
    };

    // 並列発行グループ
    ParaGroupType para_group;
    if (trace->IsParaGroupSwitched () == true) {
        para_group = MicroArchDB::GetParaGroupAlt (trace->GetInstId ());
    } else {
        para_group = MicroArchDB::GetParaGroup (trace->GetInstId());
    }
    *oss << para_group_string[para_group] << ' ';
#endif

    // パイプトレースを表示
    std::string pipe (PIPE_DISP_LEN, ' ');
    pipe [ (trace->GetStageCycle(STAGE_IA) % PIPE_DISP_LEN) ] = 'I';
    pipe [ (trace->GetStageCycle(STAGE_IT) % PIPE_DISP_LEN) ] = 'T';
    pipe [ (trace->GetStageCycle(STAGE_DP) % PIPE_DISP_LEN) ] = 'D';
    pipe [ (trace->GetStageCycle(STAGE_EA) % PIPE_DISP_LEN) ] = 'E';
    pipe [ (trace->GetStageCycle(STAGE_WB) % PIPE_DISP_LEN) ] = 'C';

#ifdef PRINT_FXU_PIPE
    if (trace->GetStageCycle(STAGE_VD) != 0) {
        pipe [ (trace->GetStageCycle(STAGE_VD) % PIPE_DISP_LEN) ] = 'V';
    }
    if (trace->GetStageCycle(STAGE_VDSKID) != 0) {
        pipe [ (trace->GetStageCycle(STAGE_VDSKID) % PIPE_DISP_LEN) ] = 'S';
    }
#endif // PRINT_FXU_PIPE

    *oss << pipe << ' ';
}


void TracePrintCForest::PrintPipeBlank (std::ostringstream* oss,
                                        const TraceCtrl* trace) const
{
    *oss << std::setw(PIPE_DISP_LEN+1) << ' ';
}



void TracePrintCForest::PrintDependTrace (std::ostringstream* oss,
                                          const TraceCtrl* trace) const
{
    std::string dep;
    dep.reserve (14);

    // レジスタ依存情報 Read側 (R1-R31, CC)
    dep += '<';
    bool rcma_flag = false;
    uint32_t rreg_count = trace->GetRregCount ();
    for (uint32_t i = 0; i < rreg_count && i < REC_RREG_NUM; ++i) {
        CReg regno = trace->GetReadReg(i);
        if (regno >= R1 && regno <= R31) {
            if (rcma_flag == true) { dep += ','; }
            dep += RegID::GetName(regno);
            rcma_flag = true;
        }
    }

    // レジスタ依存情報 Write側 (R1-R31, CC)
    dep += '|';
    bool wcma_flag = false;
    uint32_t wreg_count = trace->GetWregCount ();
    for (uint32_t i = 0; i < wreg_count && i < REC_WREG_NUM; ++i) {
        CReg regno = trace->GetWriteReg(i);
        if (regno >= R1 && regno <= R31) {
            if (wcma_flag == true) { dep += ','; }
            dep += RegID::GetName(regno);
            wcma_flag = true;
        }
    }
    dep += "> ";

    *oss << std::left << std::setw(14) << dep;
}


void TracePrintCForest::PrintBranchTrace (std::ostringstream* oss,
                                          const TraceCtrl* trace) const
{
    BpResult bp_result = trace->GetBpResult ();
    if (bp_result == BP_HIT) {
        *oss << "[BP_HIT] ";
    } else if (bp_result == BP_MISS) {
        *oss << "[BP_MISS] ";
    }
}


void TracePrintCForest::Print (SimulationMode sim_mode, uint32_t dbg_mode,
                               const TraceCtrl* trace) const
{
    std::ostringstream oss;

    ExpCode exp_code = trace->GetExpCode ();
    if (ExpInfo::GetExpType(exp_code) != ExpInfo::EXP_T_ABORT) {
        PrintBasicTrace (&oss, sim_mode, trace);

        if ((dbg_mode & DBG_PIPE) != DBG_OFF) {
            PrintPipeTrace (&oss, trace);
        }
    } else {
        PrintExpTrace (&oss, sim_mode, trace);

        if ((dbg_mode & DBG_PIPE) != DBG_OFF) {
            PrintPipeBlank (&oss, trace);
        }
    }

    if ((dbg_mode & DBG_DEP) != DBG_OFF) {
        PrintDependTrace (&oss, trace);
    }

    if ((dbg_mode & DBG_RW) != DBG_OFF) {
        PrintRegMemTrace (&oss, trace);
    }

    if ((dbg_mode & DBG_CACHE) != DBG_OFF) {
        PrintIcTrace (&oss, trace);
    }

    if ((dbg_mode & DBG_BRA) != DBG_OFF) {
        PrintBranchTrace (&oss, trace);
    }

#if defined(MAKE_FP_UNIT_TEST)
    if ((sim_mode == CS_MODE) && (exp_code == ExpCode::EXP_NONE)) {
        int32_t insn_id = trace->GetInstId();
        if (insn_id >= INST_ID_V_ABSFD && insn_id <= INST_ID_V_CVTFSH) {
            PrintUT32info (&oss, trace);
        } else if (insn_id >= INST_ID_V_CMPFS4 && insn_id <= INST_ID_V_FNMSFS4) {
            PrintUT128info (&oss, trace);
        }
    }
#endif

    CfMsg::TPrint (MSG_ERR, trace, "%s\n", oss.str().c_str());
}


/*!
 * @brief Prints the trace log with the format to compare with RTL_sim. : RTL_simログとの比較用のフォーマットでトレースログを出力する関数
 * @param trace Trace informations for Instructions. : 命令トレース情報
 */
// @@ TODO : Need to support WR registers
void TracePrintRTL::Print (const TraceCtrl* trace) const
{
    // 高速化の必要はない

    // 例外ログ出力処理
    bool is_exp_t_dur = false;
    ExpCode exp_code = trace->GetExpCode ();
    if (unlikely (exp_code != EXP_NONE)) {
        ExpInfo::ExpType exp_type = ExpInfo::GetExpType (exp_code);

        if (exp_type == ExpInfo::EXP_T_ABORT) {
            // 中断型はここでログ出力
            PrintExp (trace);
            // 命令出力は無い為、ここでreturn
            return;
        } else if (exp_type == ExpInfo::EXP_T_REEXE) {
            // 再実行型例外
            // 再実行型のログは、例外発生命令のログ出力後に出力
            is_exp_t_dur = true;
        }
        // 完了型例外は特殊処理不要
    }

    char buff_head[MAX_LOG_LEN];
    memset (buff_head, ' ', sizeof(buff_head));
    buff_head[0] = '\0';

    uint64_t step = m_inst_step;
    if (is_exp_t_dur == false) { m_inst_step++; }

    // ステップ数
    int32_t  len = 0;
    len += sprintf (&buff_head[len], "%10" PRIu64 " ", step);

    // 命令アドレス
    uint32_t inst_pc = trace->GetInstAddr();
    len += sprintf (&buff_head[len], "%08x ", inst_pc);

    // オペコード (48/64bit部は後付け)
    uint32_t inst_id   = trace->GetInstId();
    ByteSize inst_size = trace->GetInstSize();
    uint64_t inst_code = trace->GetInstCode();
    if (inst_size == SIZE_16BIT) {
        len += sprintf (&buff_head[len], "xxxx%04" PRIx64, inst_code & 0x00FFFFU);
    } else {
        len += sprintf (&buff_head[len], "%08" PRIx64,     inst_code & 0x00FFFFFFFFU);
    }
    len += sprintf (&buff_head[len], " ");

    // レジスタライト
    bool     psw_acc_flag = false;
    uint32_t wreg_count = trace->GetWregCount();
    uint32_t w_greg_cnt = 0;
    std::vector <CReg> wreg_id;
    std::vector <RegData64> wreg_data;
    for (uint32_t i = 0; i < wreg_count; ++i) {
        CReg regno = trace->GetWriteReg(i);
        // PSWレジスタ ライトアクセス判定
        if ((regno >= HMPSW_Z && regno <= GMPSW_NP) || regno == m_psw_regno) {
            psw_acc_flag = true;
        }
        // 0-31 汎用レジスタ ライトアクセス判定
        if (regno <= R31 && regno >= R0) {
            wreg_id.push_back (regno);
            wreg_data.push_back (trace->GetWregValue(i));
            w_greg_cnt++;
        }
    }

    // PSWレジスタ ライトアクセス出力
    // 再実行型例外・DBTRAPでは、PSWは例外ログで表示するためここでは不要
    if ((psw_acc_flag == true) && (is_exp_t_dur == false)
        && (exp_code != EXP_DBTRAP)) {
        uint32_t psw_value = 0x0
            | m_srf->SrVPSW()->GetNP () << 7
            | m_srf->SrVPSW()->GetEP () << 6
            | m_srf->SrVPSW()->GetID () << 5
            | m_srf->SrVPSW()->GetSAT() << 4
            | m_srf->SrVPSW()->GetCY () << 3
            | m_srf->SrVPSW()->GetOV () << 2
            | m_srf->SrVPSW()->GetS  () << 1
            | m_srf->SrVPSW()->GetZ  () << 0;
        len += sprintf (&buff_head[len], "%02x ", psw_value);
    } else {
        len += sprintf (&buff_head[len], "-- ");
    }

    // メモリライトアクセス出力
    uint32_t w_mem_count = trace->GetWmemCount();
    uint32_t print_greg_count = 0;

    for (uint32_t i=0; i<w_mem_count; ++i) {
        char buff_mem[MAX_LOG_LEN];

        // ログの共通部をコピー
        memcpy  (buff_mem, buff_head, len);
        uint32_t len_tmp = len;

        if (unlikely ((w_greg_cnt == 1) && (   (inst_id == INST_ID_V_CAXI)
                                            || (inst_id == INST_ID_V_STCW)))) {
            // レジスタとメモリの両方にWriteする命令のうち
            // CAXIとSTC.WとVST.*は1行にレジスタ・メモリ両方のログを表示する
            len_tmp += sprintf (&buff_mem[len], "%2d %08x ", wreg_id[print_greg_count],
                                static_cast<RegData>(wreg_data[print_greg_count] & 0x0FFFFFFFFULL));
            ++print_greg_count;
        } else {
            // 他の命令はメモリWriteとレジスタWriteを同じ行にしない
            len_tmp += sprintf (&buff_mem[len], "            ");
        }

        // メモリライトアクセス
        if (trace->GetWmemSize(i) == SIZE_64BIT) {
            len_tmp += sprintf (&buff_mem[len_tmp], "%08x %016" PRIx64 " ",
                                                    trace->GetWmemAddr(i),
                                                    trace->GetWmemValueL64(i));
        } else {
            len_tmp += sprintf (&buff_mem[len_tmp], "%08x %08" PRIx64 "         ",
                                                    trace->GetWmemAddr(i),
                                                    trace->GetWmemValueL64(i));
        }

        // ニーモニック
        std::ostringstream cntext_str;
        PrintContextID (trace, &cntext_str);
        std::string inst_mnem;
        m_print_operand.PrintOperand (inst_id, inst_code, inst_pc, &inst_mnem);
        sprintf (&buff_mem[len_tmp], ":%s %s", cntext_str.str().c_str(), inst_mnem.c_str());

        // ファイル出力
        fprintf( m_trace_file, "%s\n", &buff_mem[0]);
    }

    // 0-31 汎用レジスタ ライトアクセス出力
    for (uint32_t i=print_greg_count; i<w_greg_cnt; ++i) {
        char buff_reg[MAX_LOG_LEN];

        // ログの共通部をコピー
        memcpy (buff_reg, buff_head, len);
        uint32_t len_tmp = len;

        // レジスタライトアクセス
        if (wreg_id[i] <= R31) {
            len_tmp += sprintf (&buff_reg[len], "%2d %08x ", wreg_id[i],
                                static_cast<RegData>(wreg_data[i] & 0x00FFFFFFFFULL));
            // メモリライトアクセスは空とする
            len_tmp += sprintf (&buff_reg[len_tmp], "                          ");
        }

        // ニーモニック
        std::ostringstream cntext_str;
        PrintContextID (trace, &cntext_str);
        std::string inst_mnem;
        m_print_operand.PrintOperand (inst_id, inst_code, inst_pc, &inst_mnem);
        sprintf (&buff_reg[len_tmp], ":%s %s", cntext_str.str().c_str(), inst_mnem.c_str());

        // ファイル出力
        fprintf( m_trace_file, "%s\n", &buff_reg[0]);
    }

    // レジスタ・メモリライトアクセスが無い命令の出力
    if ((w_greg_cnt == 0) && (w_mem_count == 0)) {
        len += sprintf (&buff_head[len], "            ");
        len += sprintf (&buff_head[len], "                          ");

        // ニーモニック
        std::ostringstream cntext_str;
        PrintContextID (trace, &cntext_str);
        std::string inst_mnem;
        m_print_operand.PrintOperand (inst_id, inst_code, inst_pc, &inst_mnem);
        sprintf (&buff_head[len], ":%s %s", cntext_str.str().c_str(), inst_mnem.c_str());

        // ファイル出力
        fprintf( m_trace_file, "%s\n", &buff_head[0]);
    }

    fflush(stdout);

    if (is_exp_t_dur || exp_code == EXP_DBTRAP) {
        // 再実行型例外のログは、発生命令のログ出力後に出力
        // DBTRAP のログは、実行したコンテキストで出力する
        PrintExp (trace);
    }
}


/*!
 * @brief Prints the strings of the Context ID. : コンテキストIDを示す文字列を生成する関数
 * @param trace Trace informations for Instructions. : 命令トレース情報
 * @param cntext_str Output stream for Context ID strings. : コンテキストID文字列格納用
 */
void TracePrintRTL::PrintContextID (const TraceCtrl* trace,
                                    std::ostringstream* cntext_str) const
{
    ContextId t_tcid = NC;
    ContextId t_vcid = NC;

    *cntext_str << std::right << std::setfill('0');
    *cntext_str << ((t_vcid == NC) ? 'N' : 'V');
    *cntext_str << ((t_vcid == NC) ? 'M' : static_cast<char>('0' + t_vcid));
    *cntext_str << '[' << std::setw(2) << ((t_tcid == NC) ? 0 : t_tcid) << ']';
}


/*!
 * @brief Prints the Exception log. : 例外発生ログを表示する関数
 * @param trace Trace informations for Instructions. : 命令トレース情報
 */
void TracePrintRTL::PrintExp (const TraceCtrl* trace) const
{
    char buff[MAX_LOG_LEN];
    memset (buff, ' ', sizeof(buff));
    buff[0] = '\0';

    uint64_t step = m_inst_step;

    int32_t len = 0;
    // ステップ数
    len += sprintf (&buff[len], "%10" PRIu64 " ", step);

    // 命令アドレス
    uint32_t inst_pc = trace->GetInstAddr ();
    if (trace->GetExpCode () == EXP_DBTRAP) {
        // DBTRAP受付けログのPCは復帰PCの値
        inst_pc = m_srf->GetSrPtr(CF_SR_DBPC)->ReadBody ();
    }
    len += sprintf (&buff[len], "%08x ", inst_pc);

    // オペコード (0x00000000を出力)
    len += sprintf (&buff[len], "00000000 ");

    // PSW (確実に更新されるためアクセス判定不要)
    uint32_t psw_value = 0x0
        | m_srf->SrVPSW()->GetNP () << 7
        | m_srf->SrVPSW()->GetEP () << 6
        | m_srf->SrVPSW()->GetID () << 5
        | m_srf->SrVPSW()->GetSAT() << 4
        | m_srf->SrVPSW()->GetCY () << 3
        | m_srf->SrVPSW()->GetOV () << 2
        | m_srf->SrVPSW()->GetS  () << 1
        | m_srf->SrVPSW()->GetZ  () << 0;
    len += sprintf (&buff[len], "%02x ", psw_value);

    // レジスタアクセスは空白    xx xxxxxxxx
    len += sprintf (&buff[len], "            ");

    // メモリアクセスも空白      xxxxxxxx xxxxxxxxxxxxxxxx
    len += sprintf (&buff[len], "                          ");

    // 例外名
    std::ostringstream cntext_str;
    PrintContextID (trace, &cntext_str);
    sprintf (&buff[len], ":%s %s", cntext_str.str().c_str()
                                 , PrintExpName(trace->GetExpCode()));

    // ファイル出力
    fprintf( m_trace_file, "%s\n", &buff[0]);

    fflush(stdout);
}


/*!
 * @brief Prints the strings of the name of the Exception. : 例外名の文字列を返す関数
 * @param exp_code Exception code(Defined in Utility/include/forest_if.h). : Exception Code.
 * @return The strings of the name of the exception. : 例外名の文字列
 */
const char* TracePrintRTL::PrintExpName (ExpCode exp_code) const
{
    return ExpInfo::GetName(exp_code).c_str();
}


/*!
 * @brief Adds more setting for initialization of TracePrintRTL class. : TracePrintRTLクラスの追加初期設定用関数
 * @param fp The file pointer to the output. : 出力ファイルのポインタ
 * @param nc_ptr The pointer to TracePrintRTL class for native. : ネイティブ用のTracePrintRTLクラスへのポインタ
 */
void TracePrintRTL::InitPrintRTL (FILE* fp, TracePrintRTL* nc_ptr)
{
    m_trace_file = fp;
    m_nc_ptr = nc_ptr;
}


/*!
 * @brief Constructs TracePrintRTL class. : TracePrintRTLクラスのコンストラクタ
 * @param srf The pointer to SRegFile in self thread. : 自スレッドの SRegFile へのポインタ
 */
TracePrintRTL::TracePrintRTL (SRegFile* srf)
    : m_srf (srf),
      m_psw_regno (RegID::GetSRegId (CF_SR_VPSW))
{
    // ステップ数の初期化
    // @@ TraceCtrlにあるステップ数情報とRTLsimのステップ数情報は一致しないため、
    // @@ TracePrintRTLクラスでは独自にステップ数情報を持つ。
    m_inst_step   = 0;
}


/*!
 * @brief Sets interface to the print-function of the trace information. : トレース情報出力関数へのインタフェース関数
 * @param sim_mode Simulation mode of CForest. : CForestのシミュレーションモード
 * @param dbg_mode Debug mode of CForest. : CForestのデバグモード
 * @param trace Trace informations for Instructions. : 命令トレース情報
 * @note
 *   この関数は１命令実行毎に呼び出されるため、
 *   ここへの処理の追加は極力避ける。
 */
void TracePrint::Print (SimulationMode sim_mode, uint32_t dbg_mode,
                        const TraceCtrl* trace) const
{
    // トレースの表示/非表示は、関数ポインタで切り替えるため判定不要
    (this->*m_print_func) (sim_mode, dbg_mode, trace);
}


/*!
 * @brief Detects the format to print the trace information. : 表示するトレース情報のフォーマットを判定する関数
 * @param sim_mode Simulation mode of CForest. : CForestのシミュレーションモード
 * @param dbg_mode Debug mode of CForest. : CForestのデバグモード
 * @param trace Trace informations for Instructions. : 命令トレース情報
 */
void TracePrint::PrintDebug (SimulationMode sim_mode, uint32_t dbg_mode,
                             const TraceCtrl* trace) const
{
    if ((dbg_mode & DBG_ON) == DBG_ON) {
        // CForestフォーマットのデバグログ表示
        m_print_cf->Print (sim_mode, dbg_mode, trace);
    }

    if (m_enable_print_rtl == true) {
        // RTLログ比較用フォーマットのデバグログ出力
        m_print_rtl->Print (trace);
    }
}


/*!
 * @brief Initializes the format to compare with RTL. : RTL比較用フォーマットの初期化を行う関数
 * @param fp The file pointer to the output. : 出力ファイルのポインタ
 * @param nc_ptr The pointer to TracePrintRTL class for native. : ネイティブ用のTracePrintRTLクラスへのポインタ
 */
void TracePrint::InitPrintRTL (FILE* fp, TracePrintRTL* nc_ptr)
{
    m_enable_print_rtl = true;  // RTL比較用フォーマット出力ON
    m_print_rtl->InitPrintRTL (fp, nc_ptr);
}


/*!
 * @brief Constructs TracePrint class. : TracePrintのコンストラクタ
 * @param srf The pointer to SRegFile in self thread. : 自スレッドの SRegFile へのポインタ
 * @param core_type G4MH(G4MH1.0) or G4MH11(G4MH1.1).
 */
TracePrint::TracePrint (SRegFile* srf, CoreType core_type)
{
    // 初期値ではトレース非表示に設定する
    m_print_func = &TracePrint::PrintDummy;
    m_enable_print_rtl = false;

    // 各フォーマット出力クラスのインスタンス
    m_print_cf  = new TracePrintCForest (core_type);
    m_print_rtl = new TracePrintRTL (srf);
}


/*!
 * @brief Destructs TracePrint class. : TracePrintのデストラクタ
 */
TracePrint::~TracePrint ()
{
    // オブジェクトの開放
    delete m_print_cf;
    delete m_print_rtl;
}
