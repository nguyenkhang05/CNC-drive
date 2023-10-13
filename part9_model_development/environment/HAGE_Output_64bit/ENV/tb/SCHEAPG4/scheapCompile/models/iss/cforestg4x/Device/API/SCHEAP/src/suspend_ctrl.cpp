/*
 * (c) 2011,2012,2013 Renesas Electronics Corporation
 * RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY
 * This program must be used solely for the purpose for which
 * it was furnished by Renesas Electronics Corporation. No part of this
 * program may be reproduced or disclosed to others, in any
 * form, without the prior written permission of Renesas Electronics
 * Corporation.
 */

#include <vector>
#include <algorithm>
#include "./config.h"
#include "./suspend_ctrl.h"
#include "./trace_info.h"
#include "./fs_controler.h"
#include "./cs_controler.h"
#include "./inst_id_list.h"
#include "./forest_internals.h"
#include "./forest_utility.h"
#include "./outstand_base.h"
#include "./inst_v850.h"
#include "./gregfile.h"
#include "./lsu.h"


void SuspendCtrl::CalcDepRegs (TraceCtrl* trace)
{
    int32_t inst_id = trace->GetInstId ();
    ArgFormat arg_format = MicroArchDB::GetArgFormat (inst_id);

    m_dep_regs.clear ();
    m_has_self_dep = false;
    m_sync_type = SYNC_NONE;

    // ベース値を計算
    // VCALC、VCMOV、VCONCAT、DoubleFPU、reg4については追加計算必要
    uint64_t inst_code = trace->GetInstCode();
    CReg reg1 = OperandExtract (inst_code,  4,  0);
    CReg reg2 = OperandExtract (inst_code, 15, 11);
    CReg reg3 = OperandExtract (inst_code, 31, 27);
    CReg reg4;
    CReg rh;
    CReg rt;

    // @@ swtichを使わず、inst_idをindexとするvectorに変換関数を格納すべき
    switch (arg_format) {
    case ARG_DR1_DR2:
        // DR1
        reg1 &= BIT0_MASK;
        AddDependReg (R0 + reg1);
        AddDependReg (R0 + reg1 + 1);

        // DR2
        reg2 &= BIT0_MASK;
        AddDependReg (R0 + reg2);
        AddDependReg (R0 + reg2 + 1);
        break;
    case ARG_DR1_DR2_DR3:
        // DR1
        reg1 &= BIT0_MASK;
        AddDependReg (R0 + reg1);
        AddDependReg (R0 + reg1 + 1);

        // DR2
        reg2 &= BIT0_MASK;
        AddDependReg (R0 + reg2);
        AddDependReg (R0 + reg2 + 1);

        // DR3
        reg3 &= BIT0_MASK;
        AddDependReg (R0 + reg3);
        AddDependReg (R0 + reg3 + 1);
        break;
    case ARG_DR2_DR3:
        // DR2
        reg2 &= BIT0_MASK;
        AddDependReg (R0 + reg2);
        AddDependReg (R0 + reg2 + 1);

        // DR3
        reg3 &= BIT0_MASK;
        AddDependReg (R0 + reg3);
        AddDependReg (R0 + reg3 + 1);
        break;
    case ARG_DR2_R3:
        // DR2
        reg2 &= BIT0_MASK;
        AddDependReg (R0 + reg2);
        AddDependReg (R0 + reg2 + 1);

        // R3
        AddDependReg (R0 + reg3);
        break;
    case ARG_NONE:
        break;
    case ARG_R1:
        // R1
        AddDependReg (R0 + reg1);
        break;
    case ARG_R1_SYNC:
        m_sync_type = SYNC_READ;
        // R1
        AddDependReg (R0 + reg1);
        break;
    case ARG_R1_DR3:
        // R1
        AddDependReg (R0 + reg1);

        // DR3
        reg3 &= BIT0_MASK;
        AddDependReg (R0 + reg3);
        AddDependReg (R0 + reg3 + 1);
        break;
    case ARG_R1_R2:
        // R1
        AddDependReg (R0 + reg1);

        // R2
        AddDependReg (R0 + reg2);
        break;
    case ARG_R1_R2_R3:
        // R1
        AddDependReg (R0 + reg1);

        // R2
        AddDependReg (R0 + reg2);

        // R3
        AddDependReg (R0 + reg3);
        break;
    case ARG_R1_R2_DR3_DR4:
        // R1
        AddDependReg (R0 + reg1);

        // R2
        AddDependReg (R0 + reg2);

        // DR3
        reg3 &= BIT0_MASK;
        AddDependReg (R0 + reg3);
        AddDependReg (R0 + reg3 + 1);

        // DR4
        reg4 = OperandExtract (inst_code, 20, 17) << 1;
        AddDependReg (R0 + reg4);
        AddDependReg (R0 + reg4 + 1);
        break;
    case ARG_R1_R2_R3_SELF:
        // CAXI
        // R1
        AddDependReg (R0 + reg1);

        // R2
        AddDependReg (R0 + reg2);

        // R3
        AddDependReg (R0 + reg3);

        // SELF
        m_has_self_dep = true;
        break;
    case ARG_R1_R2_SELF:
        // CLR1/TST/SET1/NOT1
        // R1
        AddDependReg (R0 + reg1);

        // R2
        AddDependReg (R0 + reg2);

        // SELF
        m_has_self_dep = true;
        break;
    case ARG_R1_R3:
        // R1
        AddDependReg (R0 + reg1);

        // R3
        AddDependReg (R0 + reg3);
        break;
    case ARG_R1_RS_SP_SELF:
        // DISPOSE_R命令
        // REGS
        AddDependRegsList12 (trace);

        // SELF
        // もしもR1がREGS(List12)に含まれていたら自己依存あり
        if (std::find (m_dep_regs.begin(), m_dep_regs.end(), R0 + reg1) != m_dep_regs.end()) {
            m_has_self_dep = true;
        }

        // R1
        AddDependReg (R0 + reg1);

        // SP
        AddDependReg (R3);
        break;
    case ARG_R1_SELF:
        // SWITCH/CLR1/TST/SET1/NOT1
        // R1
        AddDependReg (R0 + reg1);

        // SELF
        m_has_self_dep = true;
        break;
    case ARG_R2:
        // R2
        AddDependReg (R0 + reg2);
        break;
    case ARG_R2_DR3:
        // R2
        AddDependReg (R0 + reg2);

        // DR3
        reg3 &= BIT0_MASK;
        AddDependReg (R0 + reg3);
        AddDependReg (R0 + reg3 + 1);
        break;
    case ARG_R2_EP:
        // R2
        AddDependReg (R0 + reg2);

        // EP
        AddDependReg (R30);
        break;
    case ARG_R2_R3:
        // R2
        AddDependReg (R0 + reg2);

        // R3
        AddDependReg (R0 + reg3);
        break;
    case ARG_RH_RT_SP:
        // RH-RT @ PUSHSP/POPSP
        rh = OperandExtract (inst_code,  4,  0) + R0;
        rt = OperandExtract (inst_code, 31, 27) + R0;
        while (rh <= rt) {
            AddDependReg (rh);
            rh += 1;
        }

        // SP
        AddDependReg (R3);
        break;

    case ARG_RS_SP:
        // DISPOSE/PREPARE
        // REGS
        AddDependRegsList12 (trace);

        // SP
        AddDependReg (R3);
        break;
    case ARG_RS_SP_EP:
        // sp/imm付きのPREPARE
        // REGS
        AddDependRegsList12 (trace);

        // SP
        AddDependReg (R3);

        // EP
        AddDependReg (R30);
        break;
    case ARG_SELF:
        // SYSCALL/CALLT/HVCALL
        // SELF
        m_has_self_dep = true;
        break;
    case ARG_VR1_VR2_VR3:
        // VR1
        AddDependReg (WR0 + reg1);

        // VR2
        AddDependReg (WR0 + reg2);

        // VR3
        AddDependReg (WR0 + reg3);
        break;
    case ARG_VR2_VR3:
        // VR2
        AddDependReg (WR0 + reg2);

        // VR3
        AddDependReg (WR0 + reg3);
        break;
    case ARG_R1_VR2:
        // R1
        AddDependReg (R0 + reg1);

        // VR2
        AddDependReg (WR0 + reg2);
        break;
    case ARG_VR1_VR2X_VR3:
        // VR1
        AddDependReg (WR0 + reg1);

        // VR2
        reg2 = OperandExtract (inst_code, 47, 43);
        AddDependReg (WR0 + reg2);

        // VR3
        AddDependReg (WR0 + reg3);
        break;
    case ARG_VR1_VR2X_VR3_VR4:
        // VR1
        AddDependReg (WR0 + reg1);

        // VR2
        reg2 = OperandExtract (inst_code, 47, 43);
        AddDependReg (WR0 + reg2);

        // VR3
        AddDependReg (WR0 + reg3);

        // VR4
        reg4 = OperandExtract (inst_code, 36, 32);
        AddDependReg (WR0 + reg4);
        break;
    case ARG_VR2:
        // VR2
        AddDependReg (WR0 + reg2);
        break;
    case ARG_SYNC_R:
        m_sync_type = SYNC_READ;
        break;
    case ARG_SYNC_RW:
        m_sync_type = SYNC_READ_WRITE;
        break;
    case ARG_ERROR:
        break;
    }
}


void SuspendCtrl::AddDependRegsList12 (TraceCtrl* trace)
{
    uint64_t inst_code = trace->GetInstCode();

    // list12を r20-r31に並び替え
    uint32_t l20    = OperandExtract (inst_code, 27, 24) << 8;
    uint32_t l24    = OperandExtract (inst_code, 31, 28) << 4;
    uint32_t l28    = OperandExtract (inst_code, 23, 22) << 2;
    uint32_t l30    = OperandExtract (inst_code,  0,  0) << 1;
    uint32_t l31    = OperandExtract (inst_code, 21, 21);
    uint32_t list12 = l20 | l24 | l28 | l30 | l31;

    // list12は降順にソート済み
    // list12[11:0]がレジスタ r24,r25,...,r29,r30,r31に対応
    for (CReg regno = R31; regno >= R20; --regno) {
        // スタックロードは降順 (list12の下位からロード)
        if (list12 & 0x001) {
            AddDependReg (regno);
        }
        list12 >>= 1;
    }
}


void SuspendCtrl::AddDependReg (CReg reg)
{
    if (reg != R0) {
        // @@ TODO : Currently, WR0-31 aren't support to wait SC-HEAP response.
        if ((reg < WR0) || (reg > WR31)) {
            m_dep_regs.push_back (reg);
        }
    }
}


void SuspendCtrl::WaitForRegDepend (TraceCtrl* trace)
{
    CalcDepRegs (trace);
    if (IsReadLatencyReceivedFs ()) {
        SetSuspendState (RELEASED_FS_EXEC);
    } else {
        SetSuspendState (SUSPEND_FS_EXEC);
        SetIsRunnable (false);
    }
}


void SuspendCtrl::WaitForRegDependSelf (TraceCtrl* trace)
{
    if ((trace->GetExpCode () != EXP_NONE) || (trace->IsCancel ())) {
        // @@ 例外発生時は実行スキップされているので待たない
        SetSuspendState (RELEASED_FS);
        return;
    }

    int32_t inst_id = trace->GetInstId ();
    switch (inst_id) {
    case INST_ID_V_DISPOSE_R:
    case INST_ID_V_CALLT:
    case INST_ID_V_SYSCALL:
    case INST_ID_V_SWITCH:
        // @@ PCに対する書き戻しは未対応。従来ルーチンに任せて、ここでは何もしない。
        SetSuspendState (RELEASED_FS);
        break;
    case INST_ID_V_CLR1:
    case INST_ID_V_TST1:
    case INST_ID_V_SET1:
    case INST_ID_V_NOT1:
    case INST_ID_V_CLR1_B3:
    case INST_ID_V_TST1_B3:
    case INST_ID_V_SET1_B3:
    case INST_ID_V_NOT1_B3:
    case INST_ID_V_CAXI:
        // RMWのR結果待ち
        if (trace->GetRmemCount () > 0) {
            PhysAddr acc_addr = trace->GetRmemAddr (0);
#ifdef CLUSTER
            MemLatencyInfoBase* mem_info = p_internals->GetMemResourceInfo (m_pe_id, acc_addr, DATA_RMW);
#else // CLUSTER
            MemLatencyInfoBase* mem_info = p_internals->GetMemResourceInfo (m_pe_id, acc_addr);
#endif // CLUSTER
            if (mem_info->IsScheap ()) {
                // ロード対象がSC-HEAPなので、ロード結果を待つ
                TransactionId trid = RegisterTransaction (acc_addr, LDTYPE_RMW);
                RegisterLoadTarget (trid, CF_RMW_BUF); // ロード結果を格納するテンポラリ

                SetSuspendState (SUSPEND_FS_EXEC_SELF);
                SetIsRunnable (false);
                return;
            } else {
                m_rmw_read_buf = m_insts->GetReadValRMW ();
            }
        }
        SetSuspendState (RELEASED_FS);
        break;
    default:
        SetSuspendState (RELEASED_FS);
    }
}


void SuspendCtrl::ResumeExec (TraceCtrl* trace)
{
    if ((trace->GetExpCode () != EXP_NONE) || (trace->IsCancel ())) {
        // @@ 例外発生時は実行スキップされているのでRMWの再開動作をスキップ
        return;
    }

    int32_t inst_id = trace->GetInstId ();
    uint64_t opcode = trace->GetInstCode ();
    switch (inst_id) {
    case INST_ID_V_CLR1:    m_insts->Inst_CLR1_MW (opcode, m_rmw_read_buf);    break;
    case INST_ID_V_TST1:    m_insts->Inst_TST1_MW (opcode, m_rmw_read_buf);    break;
    case INST_ID_V_SET1:    m_insts->Inst_SET1_MW (opcode, m_rmw_read_buf);    break;
    case INST_ID_V_NOT1:    m_insts->Inst_NOT1_MW (opcode, m_rmw_read_buf);    break;
    case INST_ID_V_CLR1_B3: m_insts->Inst_CLR1_B3_MW (opcode, m_rmw_read_buf); break;
    case INST_ID_V_TST1_B3: m_insts->Inst_TST1_B3_MW (opcode, m_rmw_read_buf); break;
    case INST_ID_V_SET1_B3: m_insts->Inst_SET1_B3_MW (opcode, m_rmw_read_buf); break;
    case INST_ID_V_NOT1_B3: m_insts->Inst_NOT1_B3_MW (opcode, m_rmw_read_buf); break;
    case INST_ID_V_CAXI:    m_insts->Inst_CAXI_MW (opcode, m_rmw_read_buf);    break;
    default: break;
    }
}


bool SuspendCtrl::IsReadLatencyReceivedRMW (void) const
{
    return ! (IsInLoadTarget (CF_RMW_BUF));
}


bool SuspendCtrl::IsReadLatencyReceivedFs (void) const
{
    CRegVec::const_iterator it = m_dep_regs.begin ();
    while (it != m_dep_regs.end ()) {
        if (IsInLoadTarget (*it) == true) {
            return false;
        }
        ++ it;
    }

    switch (m_sync_type) {
    case SYNC_READ:
        // SYNCE/SYNCP命令では全SCHEAPアクセスが終わるまで待ち合わせる
        return m_trid_infos.empty ();
        break;
    case SYNC_READ_WRITE:
        // SYNCM命令では全SCHEAPアクセスが終わるまで待ち合わせる
        return m_trid_infos.empty () && m_write_trid_infos.empty ();
        break;
    default:
        break;
    }

    return true;
}


bool SuspendCtrl::IsReadLatencyReceivedCs (TraceCtrl* trace) const
{
    CRegVec::const_iterator it_rreg = trace->GetRregDepVec()->begin ();
    CRegVec::const_iterator it_rend = trace->GetRregDepVec()->end ();
    while (it_rreg != it_rend) {
        if (IsInLoadTarget (*it_rreg) == true) {
            return false;
        }
        ++ it_rreg;
    }

    // レイテンシ未通知のレジスタがDSTレジスタの場合もアクセス完了まで待たせる
    CRegVec::const_iterator it_wreg = trace->GetWregDepVec()->begin();
    CRegVec::const_iterator it_wend = trace->GetWregDepVec()->end();
    while (it_wreg != it_wend) {
        if (IsInLoadTarget (*it_wreg) == true) {
            return false;
        }
        ++ it_wreg;
    }

    // 同時発行でなければストールするので、IssueRateに対する依存がないかチェック
    if (m_csctrl->IsParallelIssued () == false) {
        if (IsInLoadTarget (CF_ISSUE) == true) {
            return false;
        }
    }

    // メモリアクセスを生じる命令では、LSUバッファに空きができるまでストール
    if ((trace->GetRmemCount () > 0) || (trace->GetWmemCount () > 0)) {
        if (m_csctrl->IsLsuBufEmpty (trace) == false) {
            return false;
        }
    }

    return true;
}


void SuspendCtrl::ApplyLatencyToLsuBuf (TransactionId trid, uint32_t latency)
{
    MemIssueCycleVec::iterator it = m_mem_issue_cycle_infos.begin ();
    while ( it != m_mem_issue_cycle_infos.end ()) {
        if (it->first == trid) {
            uint64_t start_cycle = it->second;
            m_csctrl->SetNotifiedLatencyScheap (start_cycle + latency);
            m_mem_issue_cycle_infos.erase (it);
            break;
        }
        ++ it;
    }
}


bool SuspendCtrl::SetLoadResult (PhysAddr addr, uint32_t latency, uint64_t read_data)
{
    // アドレス値を元にレジスタを検索する
    //   同じアドレスなら、先のアクセスに対するレイテンシが先に返されるはず
    //   異なるアドレスなら、順番が入れ替わって通知されるかもしれない
    //   1個のメモリアクセスが複数レジスタに関連する
    //   1個のレジスタが複数メモリアクセスに関係していたら？
    //   1命令で複数メモリアクセスを発行する命令が厄介。
    bool is_found_acc = false;
    TrIdInfoVec::iterator it = m_trid_infos.begin ();
    while ( it != m_trid_infos.end() ) {
        if (it->first == addr) {
            TransactionId trid = it->second;
            ApplyLatencyToLsuBuf (trid, latency);

            LoadType load_type = LDTYPE_UNDEF;
            LoadTypeInfoVec::iterator it_ldtype = m_loadtype_infos.begin ();
            while ( it_ldtype != m_loadtype_infos.end() ) {
                if (it_ldtype->first == trid) {
                    load_type = it_ldtype->second;
                    m_loadtype_infos.erase (it_ldtype);
                    break;
                }
                ++ it_ldtype;
            }

            ReadRegInfoVec::iterator it_reg = m_rreg_infos.begin ();
            while ( it_reg != m_rreg_infos.end() ) {
                if (it_reg->first == trid) {
                    CReg reg = it_reg->second;
                    if (reg == CF_ISSUE) {
                        m_csctrl->AddIssueRate (latency);
                    } else if (reg == CF_RMW_BUF) {
                        // RMWのR結果を格納
                        m_rmw_read_latency = latency;
                        m_rmw_read_buf = read_data & 0xFFFFFFFF;
                    } else {
                        m_csctrl->AddRegActiveCycle (reg, latency);
                        ApplyLoadResult (load_type, reg, read_data);
                    }
                    it_reg = m_rreg_infos.erase (it_reg);
                } else {
                    ++ it_reg;
                }
            }
            m_trid_infos.erase (it);
            is_found_acc = true;
            break;
        }
        ++ it;
    }

    return is_found_acc;
}


bool SuspendCtrl::SetStoreResult (PhysAddr addr, uint32_t latency)
{
    // @@ latency反映には未対応 （LSU情報の更新が必要)
    bool is_found_acc = false;
    TrIdInfoVec::iterator it = m_write_trid_infos.begin ();
    while ( it != m_write_trid_infos.end() ) {
        if (it->first == addr) {
            TransactionId trid = it->second;
            ApplyLatencyToLsuBuf (trid, latency);
            m_write_trid_infos.erase (it);
            is_found_acc = true;
            break;
        }
        ++ it;
    }

    return is_found_acc;
}


SuspendCtrl::TransactionId SuspendCtrl::RegisterWriteTransaction (PhysAddr addr)
{
    // SC-HEAPから指定されるアドレスは、バス上のアドレス値。
    // CPU内物理アドレス値をSELF/MIRROR変換して、FsBusに出た後のアドレス値に変換して記録。
    addr = m_fsctrl->m_lsu->GetBusAddress (addr);

    // LSUの情報更新に対応するときのために、tridはLoadとStoreで共有しておく。
    // - LSUのレイテンシ情報を更新するときに、R/Wの判定をせずにtridだけで検索したいので。
    TransactionId trid = ++ m_transaction_id;
    m_write_trid_infos.push_back (TrIdInfo(addr, trid));
    return trid;
}


SuspendCtrl::TransactionId SuspendCtrl::RegisterTransaction (PhysAddr addr, LoadType load_type)
{
    // SC-HEAPから指定されるアドレスは、バス上のアドレス値。
    // CPU内物理アドレス値をSELF/MIRROR変換して、FsBusに出た後のアドレス値に変換して記録。
    addr = m_fsctrl->m_lsu->GetBusAddress (addr);

    TransactionId trid = ++ m_transaction_id;
    m_trid_infos.push_back (TrIdInfo(addr, trid));
    m_loadtype_infos.push_back (LoadTypeInfo(trid, load_type));
    return trid;
}


void SuspendCtrl::RegisterMemIssueCycle (TransactionId trid, uint64_t start_cycle)
{
    m_mem_issue_cycle_infos.push_back (MemIssueCycleInfo(trid, start_cycle));
}


void SuspendCtrl::RegisterLoadTarget (TransactionId trid, CReg reg)
{
    m_rreg_infos.push_back (ReadRegInfo(trid, reg));
}


bool SuspendCtrl::IsInLoadTarget (CReg reg) const
{
    ReadRegInfoVec::const_iterator it = m_rreg_infos.begin ();
    while ( it != m_rreg_infos.end() ) {
        if (it->second == reg) {
            return true;
        }
        ++ it;
    }
    return false;
}


void SuspendCtrl::NotifyFetchLatency (TraceCtrl* trace, PhysAddr addr, uint32_t latency)
{
    // no support
}


void SuspendCtrl::NotifyReadLatency  (TraceCtrl* trace, PhysAddr addr,
                                      uint32_t latency, uint64_t data)
{
    // NotifyされるレイテンシはSystimeベース(clkratio=1相当)なので、PEクロックベースに換算
    uint32_t pe_clk_base_latency = m_csctrl->GetPeBaseCycleFromSystime (latency);

    if (SetLoadResult (addr, pe_clk_base_latency, data) == true) {
        UpdateSuspendState (trace);
    } else {
        // 対応するアクセス情報が見つからなかった
        // 自命令のアクセス情報は未登録。自命令の情報が登録されるまでNotify内容を保持
        RegisterReadLatencyOwn (addr, pe_clk_base_latency, data);
    }
}


void SuspendCtrl::NotifyWriteLatency (TraceCtrl* trace, PhysAddr addr, uint32_t latency)
{
    // NotifyされるレイテンシはSystimeベース(clkratio=1相当)なので、PEクロックベースに換算
    uint32_t pe_clk_base_latency = m_csctrl->GetPeBaseCycleFromSystime (latency);

    if (SetStoreResult (addr, pe_clk_base_latency) == true) {
        UpdateSuspendState (trace); // syncによる待ちを解除判定
    } else {
        // 対応するアクセス情報が見つからなかった
        // 自命令のアクセス情報は未登録。自命令の情報が登録されるまでNotify内容を保持
        RegisterWriteLatencyOwn (addr, pe_clk_base_latency);
    }
}


void SuspendCtrl::RegisterReadLatencyOwn (PhysAddr addr, uint32_t latency, uint64_t data)
{
    m_notify_read_info.push_back (NotifyReadType(addr, latency, data));
    m_has_notify_own = true;
}


void SuspendCtrl::RegisterWriteLatencyOwn (PhysAddr addr, uint32_t latency)
{
    m_notify_write_info.push_back (NotifyWriteType(addr, latency));
    m_has_notify_own = true;
}


void SuspendCtrl::ApplyNotifyOwn (TraceCtrl* trace)
{
    if (m_has_notify_own) {
        NotifyReadVec::iterator it_r = m_notify_read_info.begin ();
        while (it_r != m_notify_read_info.end ()) {
            if (SetLoadResult ((*it_r).m_addr, (*it_r).m_latency, (*it_r).m_data)) {
                it_r = m_notify_read_info.erase (it_r);
            } else {
                ++ it_r;
            }
        }

        NotifyWriteVec::iterator it_w = m_notify_write_info.begin ();
        while (it_w != m_notify_write_info.end ()) {
            if (SetStoreResult ((*it_w).m_addr, (*it_w).m_latency)) {
                it_w = m_notify_write_info.erase (it_w);
            } else {
                ++ it_w;
            }
        }
    }
}


void SuspendCtrl::InitNotifyOwn (void)
{
    m_notify_read_info.clear ();
    m_notify_write_info.clear ();
    m_has_notify_own = false;
}


void SuspendCtrl::UpdateSuspendState (TraceCtrl* trace)
{
    SuspendState state = GetSuspendState ();
    switch (state) {
    case SUSPEND_FS_EXEC:
        if (IsReadLatencyReceivedFs () == true) {
            SetSuspendState (RELEASED_FS_EXEC);
            SetIsRunnable (true);
        }
        break;
    case SUSPEND_FS_EXEC_SELF:
        if ( (IsReadLatencyReceivedFs () == true) && (IsReadLatencyReceivedRMW ()) ) {
            SetSuspendState (RELEASED_FS);
            SetIsRunnable (true);
        }
        break;
    case SUSPEND_CS_FETCH:
        // NotifyFetchでだけ解除可能
        break;
    case SUSPEND_CS_EXEC:
        if (IsReadLatencyReceivedCs (trace) == true) {
            SetSuspendState (RELEASED_CS_EXEC);
            SetIsRunnable (true);
        }
        break;
    case SUSPEND_CS_LSU:
        if (m_csctrl->IsLsuBufEmpty (trace) == true) {
            SetSuspendState (RELEASED_CS_LSU);
            SetIsRunnable (true);
        }
        break;
    case RELEASED_FS_EXEC:
    case RELEASED_FS:
    case RELEASED_CS_FETCH:
    case RELEASED_CS_EXEC:
    case RELEASED_CS_LSU:
    case RELEASED_CS:
    case NO_SUSPEND:
        // Suspendしていないのでステート更新不要
        break;
    }
}


void SuspendCtrl::RegisterStoreDependency (TraceCtrl* trace, int32_t pos, uint64_t start_cycle)
{
    TransactionId trid = RegisterWriteTransaction (trace->GetWmemAddr (pos));
    RegisterMemIssueCycle (trid, start_cycle);
}


void SuspendCtrl::RegisterLoadDependency (TraceCtrl* trace, int32_t pos,
                                          CReg dest_reg, DependIssueType has_dep_issue,
                                          uint64_t start_cycle)
{
    // 現在のところ、ロード結果に対するRegWriteはinst_xxxの最後に設置
    // (RMW命令、特殊命令ではSetPswXXがさらに後にあるが、ここには来ないので除外)
    // traceの最後のRegエントリを探せばロード先レジスタがわかる(Disposeなど複数回アクセス除く)
    // DWの時には最後ではないけど、ビット0をマスクすれば良い
    LoadType load_type = MicroArchDB::GetLoadType (trace->GetInstId ());
    TransactionId trid = RegisterTransaction (trace->GetRmemAddr (pos), load_type);
    RegisterMemIssueCycle (trid, start_cycle);
    if (has_dep_issue == HAS_DEP_ISSUE) {
        RegisterLoadTarget (trid, CF_ISSUE);
    }

    // R0/CF_ISSUEでなければ、依存関係を設定
    if ((IsRegDepAnaNecessity (dest_reg) == true) && (dest_reg != R0)) {
         RegisterLoadTarget (trid, dest_reg);

        // DWの時には2個目のload対象レジスタを登録(ビット0をマスクすれば良い)
        if (load_type == LDTYPE_U64) {
            CReg dest_reg_lo = dest_reg & BIT0_MASK;
            if ((IsRegDepAnaNecessity (dest_reg_lo) == true) && (dest_reg_lo != R0)) {
                RegisterLoadTarget (trid, dest_reg_lo);
            }
        }
    }
}


void SuspendCtrl::ApplyLoadResult (LoadType load_type, CReg reg, uint64_t data)
{
    // @@ 配列に関数ポインタを格納して、LoadTypeをindexとして引けばよい。
    switch (load_type) {
    case LDTYPE_S8:
        ApplyLoadResult_S8 (reg, data);
        break;
    case LDTYPE_S16:
        ApplyLoadResult_S16 (reg, data);
        break;
    case LDTYPE_U8:
        ApplyLoadResult_U8 (reg, data);
        break;
    case LDTYPE_U16:
        ApplyLoadResult_U16 (reg, data);
        break;
    case LDTYPE_U32:
        ApplyLoadResult_U32 (reg, data);
        break;
    case LDTYPE_U64:
        ApplyLoadResult_U64 (reg, data);
        break;
    case LDTYPE_CACHE:
    case LDTYPE_CALLT:
    case LDTYPE_PREF:
    case LDTYPE_SWITCH:
    case LDTYPE_SYSCALL:
    case LDTYPE_LDM:
        // @@ 書き戻しに未対応
        // cache/prefではキャッシュ内容まで書き換えないといけないので複雑。要求ない限り非対応。
        // その他の命令については、必要性が低そう＆実装が面倒だったので未対応。
        // - callt/hvcall/switch/syscall/ジャンプ付きdisposeのジャンプはRMW命令と同様、
        //   SELF依存で止めればよい
        ApplyLoadResult_Ignore (reg, data);
        break;
    case LDTYPE_DISPOSE:
    case LDTYPE_POP:
    case LDTYPE_RESBANK:
        // @@ DISPOSE_RのPCについては書き戻し未対応
        // - ジャンプ付きdisposeのジャンプはRMW命令と同様、
        //   SELF依存で止めればよい
        // @@ system registers of RESBANK can't be updated.
        ApplyLoadResult_U32 (reg, data);
        break;
    case LDTYPE_RMW:
        // R結果を待ってからWしているので書き戻し不要
        break;
    case LDTYPE_UNDEF:
        // @@ ここに来たらエラー
        break;
    }
}


void SuspendCtrl::ApplyLoadResult_Ignore (CReg reg, uint64_t data)
{}


void SuspendCtrl::ApplyLoadResult_S8 (CReg reg, uint64_t data)
{
    uint32_t s_data = SignExtend (data & 0xFF, 8);
    UpdateGReg (reg, m_grf->Read (reg - R0), s_data);
}


void SuspendCtrl::ApplyLoadResult_S16 (CReg reg, uint64_t data)
{
    uint32_t s_data = SignExtend (data & 0xFFFF, 16);
    UpdateGReg (reg, m_grf->Read (reg - R0), s_data);
}


void SuspendCtrl::ApplyLoadResult_U8 (CReg reg, uint64_t data)
{
    UpdateGReg (reg, m_grf->Read (reg - R0), data & 0xFF);
}


void SuspendCtrl::ApplyLoadResult_U16 (CReg reg, uint64_t data)
{
    UpdateGReg (reg, m_grf->Read (reg - R0), data & 0xFFFF);
}


void SuspendCtrl::ApplyLoadResult_U32 (CReg reg, uint64_t data)
{
    UpdateGReg (reg, m_grf->Read (reg - R0), data & 0xFFFFFFFF);
}


void SuspendCtrl::ApplyLoadResult_U64 (CReg reg, uint64_t data)
{
    // LD.DW用：reg[0:0]=0なら下位32bit、=1なら上位32bitをレジスタに格納
    uint32_t data32;
    if ((reg & 0x1) == 0x0) {
        data32 = data & 0xFFFFFFFF;
    } else {
        data32 = static_cast <uint32_t> ((data >> 32) & 0xFFFFFFFF);
    }
    UpdateGReg (reg, m_fsctrl->m_grf->Read (reg - R0), data32);
}


void SuspendCtrl::UpdateGReg (CReg reg, uint32_t old_data, uint32_t new_data)
{
    if (old_data != new_data) {
        reg -= R0;
        m_grf->Write (reg, new_data);
        if ((p_forest_util->GetDebugMode() & DBG_ON) != DBG_OFF) {
            // ログ出力が有効な場合、出力したログ内容が間違っていることを通知
            CfMsg::TPrint (MSG_ERR, m_fsctrl->m_trace,
                           "<Load result is changed : r%d<=%08lx >\n", reg, new_data);
        }
    }
}


void SuspendCtrl::SetSuspendState (SuspendState state)
{
    m_suspend_state = state;
    m_fsctrl->m_trace->SetSuspendState (state);
}


void SuspendCtrl::Init (void)
{
    m_transaction_id = 0;
    m_trid_infos.clear ();
    m_rreg_infos.clear ();
    m_loadtype_infos.clear ();
    m_dep_regs.clear ();
    m_mem_issue_cycle_infos.clear ();

    m_write_trid_infos.clear ();

    m_suspend_state = NO_SUSPEND;
    m_is_runnable = true;
    m_has_self_dep = false;

    m_rmw_read_buf = 0xDEADBEEF;
    m_rmw_read_latency = 0;

    m_sync_type = SYNC_NONE;
    m_is_lsu_repeated = false;
    m_repeat_count = 0;

    InitNotifyOwn ();
}


SuspendCtrl::SuspendCtrl (FsCtrl* fsctrl, CsCtrl* csctrl,
                          InstV850* insts, GRegFile* grf, uint32_t pe_id)
    : m_fsctrl (fsctrl), m_csctrl (csctrl),
      m_insts (insts), m_grf (grf), m_pe_id (pe_id)
{
    Init ();
}


SuspendCtrl::~SuspendCtrl ()
{}
