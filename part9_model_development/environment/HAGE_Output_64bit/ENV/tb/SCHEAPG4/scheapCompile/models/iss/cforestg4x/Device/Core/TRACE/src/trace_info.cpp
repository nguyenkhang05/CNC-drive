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
#include "./config.h"
#include "./forest_common.h"
#include "./trace_info.h"
#include "./micro_arch_db.h"
#include "./regid.h"
#include "./forest_utility.h"
#include "./forest_message.h"


template <>
void TraceCtrl::RecordCReg<TraceCtrl::REG_WRITE> (CReg creg, RegData data)
{
    int32_t cnt = m_tr.wreg_count;
    if (cnt < REC_WREG_NUM) {
        m_tr.write_reg[cnt] = creg;
        m_tr.wreg_value [cnt] = data;
        ++ m_tr.wreg_count;
    } else {
        CfMsg::TPrint (MSG_ERR, this, "<Internal Error: Exceed max num of write register in trace>\n");
    }
}


template <>
void TraceCtrl::RecordCReg<TraceCtrl::REG_READ> (CReg creg, RegData data)
{
    int32_t cnt = m_tr.rreg_count;
    if (cnt < REC_RREG_NUM) {
        m_tr.read_reg[cnt] = creg;
        m_tr.rreg_value[cnt] = data;
        ++ m_tr.rreg_count;
    } else {
        CfMsg::TPrint (MSG_ERR, this, "<Internal Error: Exceed max num of read register in trace>\n");
    }
}


template <TraceCtrl::RegCommand cmd>
void TraceCtrl::RecordPC (PhysAddr addr)
{
    RecordCReg<cmd> (PC, addr);
}


template <TraceCtrl::RegCommand cmd>
void TraceCtrl::RecordPsw (CReg regno, RegData data)
{
    RecordCReg<cmd> (regno, data);
}


template <TraceCtrl::RegCommand cmd>
void TraceCtrl::RecordSesr (CReg regno, bool data)
{
    RecordCReg<cmd> (regno, data);
}


template <TraceCtrl::RegCommand cmd>
void TraceCtrl::RecordGReg (uint32_t regno, RegData data)
{
    RecordCReg<cmd> (R0 + regno, data);
    if (cmd == REG_WRITE) {
        m_tr.wdep_regs.push_back (R0 + regno);
    } else {
        m_tr.rdep_regs.push_back (R0 + regno);
    }
}


template <TraceCtrl::RegCommand cmd>
void TraceCtrl::RecordWReg (uint32_t regno, RegData64 data_u, RegData64 data_l)
{
    if (cmd == REG_WRITE) {
        int32_t cnt = m_tr.wreg_count;
        if (cnt < REC_WREG_NUM) {
            m_tr.write_reg[cnt] = WR0 + regno;
            m_tr.wreg_wr_value [cnt][0] = static_cast<uint32_t> ((data_l >>  0) & 0xFFFFFFFFU);
            m_tr.wreg_wr_value [cnt][1] = static_cast<uint32_t> ((data_l >> 32) & 0xFFFFFFFFU);
            m_tr.wreg_wr_value [cnt][2] = static_cast<uint32_t> ((data_u >>  0) & 0xFFFFFFFFU);
            m_tr.wreg_wr_value [cnt][3] = static_cast<uint32_t> ((data_u >> 32) & 0xFFFFFFFFU);
            ++ m_tr.wreg_count;
        } else {
            CfMsg::TPrint (MSG_ERR, this, "<Internal Error: Exceed max num of write wreg in trace>\n");
        }
    } else {
        int32_t cnt = m_tr.rreg_count;
        if (cnt < REC_RREG_NUM) {
            m_tr.read_reg[cnt] = WR0 + regno;
            m_tr.rreg_wr_value [cnt][0] = static_cast<uint32_t> ((data_l >>  0) & 0xFFFFFFFFU);
            m_tr.rreg_wr_value [cnt][1] = static_cast<uint32_t> ((data_l >> 32) & 0xFFFFFFFFU);
            m_tr.rreg_wr_value [cnt][2] = static_cast<uint32_t> ((data_u >>  0) & 0xFFFFFFFFU);
            m_tr.rreg_wr_value [cnt][3] = static_cast<uint32_t> ((data_u >> 32) & 0xFFFFFFFFU);
            ++ m_tr.rreg_count;
        } else {
            CfMsg::TPrint (MSG_ERR, this, "<Internal Error: Exceed max num of read wreg in trace>\n");
        }
    }

    if (cmd == REG_WRITE) {
        m_tr.wdep_regs.push_back (WR0 + regno);
    } else {
        m_tr.rdep_regs.push_back (WR0 + regno);
    }
}


template <TraceCtrl::RegCommand cmd>
void TraceCtrl::RecordSReg (CfSReg cfsrid, RegData data)
{
    CReg sregid = RegID::GetSRegId (cfsrid);
    RecordCReg<cmd> (sregid, data);
}

#if defined(COMP_RUNNER)
template <TraceCtrl::RegCommand cmd>
void TraceCtrl::RecordSReg (CReg sregid, RegData data)
{
    RecordCReg<cmd> (sregid, data);
}
#endif //#if defined(COMP_RUNNER)

void TraceCtrl::RecordMemFetch (PhysAddr addr, MemAccessRoute route)
{
    int32_t cnt = m_tr.fetch_count;
    if (cnt < REC_FETCH_NUM) {
        m_tr.fetch_addr[cnt]  = addr;
        m_tr.fetch_route[cnt] = route;
        ++ m_tr.fetch_count;
    } else {
        CfMsg::TPrint (MSG_ERR, this, "<Internal Error: Exceed max num of fetch in trace>\n");
    }
}


void TraceCtrl::RecordIcAccess (uint32_t entry, uint32_t way, IcEventType event, PhysAddr addr)
{
    int32_t cnt = m_tr.ic_count;
    if (cnt < REC_IC_NUM) {
        m_tr.ic_entry[cnt] = entry;
        m_tr.ic_way[cnt]   = way;
        m_tr.ic_event[cnt] = event;
        m_tr.ic_addr[cnt]  = addr;
        ++ m_tr.ic_count;
    } else {
        CfMsg::TPrint (MSG_ERR, this, "<Internal Error: Exceed max num of ic in trace>\n");
    }
}

#if defined (ENABLE_TRACE_MDP_SUPPRESS)
template <>
void TraceCtrl::RecordMemAccessMdp<DATA_WRITE>
(PhysAddr addr, MemData64 data_u, MemData64 data_l, ByteSize size, MemAccessRoute route, bool is_issued)
{
    int32_t cnt = m_tr.wmem_mdp_count;
    if (cnt < REC_WMEM_NUM) {
        m_tr.wmem_mdp_addr [cnt] = addr;
        m_tr.wmem_mdp_value_l64[cnt] = data_l;
        m_tr.wmem_mdp_value_u64[cnt] = data_u;
        m_tr.wmem_mdp_size [cnt] = size;
        m_tr.wmem_mdp_route[cnt] = route;
        m_tr.wmem_mdp_issued[cnt] = is_issued;
        ++ m_tr.wmem_mdp_count;
    } else {
        CfMsg::TPrint (MSG_ERR, this, "<Internal Error: Exceed max num of store in trace>\n");
    }
}


template <>
void TraceCtrl::RecordMemAccessMdp<DATA_READ>
(PhysAddr addr, MemData64 data_u, MemData64 data_l, ByteSize size, MemAccessRoute route, bool is_issued)
{
    int32_t cnt = m_tr.rmem_mdp_count;
    if (cnt < REC_RMEM_NUM) {
        m_tr.rmem_mdp_addr [cnt] = addr;
        m_tr.rmem_mdp_value_l64[cnt] = data_l;
        m_tr.rmem_mdp_value_u64[cnt] = data_u;
        m_tr.rmem_mdp_size [cnt] = size;
        m_tr.rmem_mdp_route[cnt] = route;
        m_tr.rmem_mdp_issued[cnt] = is_issued;
        ++ m_tr.rmem_mdp_count;
    } else {
        CfMsg::TPrint (MSG_ERR, this, "<Internal Error: Exceed max num of load in trace>\n");
    }
}
#endif // #if defined (ENABLE_TRACE_MDP_SUPPRESS)


template <>
void TraceCtrl::RecordMemAccess<DATA_WRITE>
(PhysAddr addr, MemData64 data_u, MemData64 data_l, ByteSize size, MemAccessRoute route)
{
    int32_t cnt = m_tr.wmem_count;
    if (cnt < REC_WMEM_NUM) {
        m_tr.wmem_addr [cnt] = addr;
        m_tr.wmem_value_l64[cnt] = data_l;
        m_tr.wmem_value_u64[cnt] = data_u;
        m_tr.wmem_size [cnt] = size;
        m_tr.wmem_route[cnt] = route;
        ++ m_tr.wmem_count;
#if defined (ENABLE_TRACE_MDP_SUPPRESS)
        RecordMemAccessMdp<DATA_WRITE> (addr, data_u, data_l, size, route, true);
#endif // #if defined (ENABLE_TRACE_MDP_SUPPRESS)
    } else {
        CfMsg::TPrint (MSG_ERR, this, "<Internal Error: Exceed max num of store in trace>\n");
    }
}


template <>
void TraceCtrl::RecordMemAccess<DATA_READ>
(PhysAddr addr, MemData64 data_u, MemData64 data_l, ByteSize size, MemAccessRoute route)
{
    int32_t cnt = m_tr.rmem_count;
    if (cnt < REC_RMEM_NUM) {
        m_tr.rmem_addr [cnt] = addr;
        m_tr.rmem_value_l64[cnt] = data_l;
        m_tr.rmem_value_u64[cnt] = data_u;
        m_tr.rmem_size [cnt] = size;
        m_tr.rmem_route[cnt] = route;
        ++ m_tr.rmem_count;
#if defined (ENABLE_TRACE_MDP_SUPPRESS)
        RecordMemAccessMdp<DATA_READ> (addr, data_u, data_l, size, route, true);
#endif // #if defined (ENABLE_TRACE_MDP_SUPPRESS)
    } else {
        CfMsg::TPrint (MSG_ERR, this, "<Internal Error: Exceed max num of load in trace>\n");
    }
}

#ifdef FROG_API_IF
void TraceCtrl::RecordDeleteLLBit  (int32_t pe_id, PhysAddr paddr)
{
    int32_t cnt = m_tr.del_llbit_count;
    if (cnt < REC_LLBIT_NUM) {
        m_tr.del_llbit_paddr[cnt].first = pe_id;
        m_tr.del_llbit_paddr[cnt].second = paddr;
        ++ m_tr.del_llbit_count;
    }
}


void TraceCtrl::SetLLBitAdd (PhysAddr llbit_paddr, ByteSize llbit_size)
{
    m_tr.add_llbit.first  = llbit_paddr;
    m_tr.add_llbit.second = llbit_size;
}
#endif //FROG_API_IF


void TraceCtrl::RecordRregDepVec  (uint32_t regno)
{
    m_tr.rdep_regs.push_back (R0 + regno);
}


void TraceCtrl::SetPeId (uint32_t pe_id)
{ m_tr.pe_id = pe_id; }

void TraceCtrl::SetInstAddr (PhysAddr inst_addr)
{ m_tr.inst_addr = inst_addr; }

void TraceCtrl::SetFetchAddr (PhysAddr fetch_addr, int32_t i)
{ m_tr.fetch_addr[i] = fetch_addr; }

void TraceCtrl::SetNextInstAddr (PhysAddr next_inst_addr)
{ m_tr.next_inst_addr = next_inst_addr; }

void TraceCtrl::SetInstCode (uint64_t inst_code)
{ m_tr.inst_code = inst_code; }

void TraceCtrl::SetInstSize (ByteSize inst_size)
{ m_tr.inst_size = inst_size; }

void TraceCtrl::SetInstId (int32_t inst_id)
{ m_tr.inst_id = inst_id; }

void TraceCtrl::SetPrivLevel (PrivLevel priv_level)
{ m_tr.priv_level = priv_level; }

void TraceCtrl::SetExpCode (ExpCode exp_code)
{ m_tr.exp_code = exp_code; }

void TraceCtrl::SetExpCause (ExpCause exp_cause)
{ m_tr.exp_cause = exp_cause; }

void TraceCtrl::SetBranchInfo (BranchInfo branch_info)
{ m_tr.branch_info = branch_info; }

void TraceCtrl::SetBraTargetInfo (bool is_branch_target)
{ m_tr.m_is_branch_target = is_branch_target; }

void TraceCtrl::SetBpResult (BpResult bp_result)
{ m_tr.bp_result = bp_result; }

void TraceCtrl::SetParallel (bool is_parallel)
{ m_tr.is_parallel = is_parallel; }

void TraceCtrl::SetStageCycle (PipeStage stage, uint64_t cycle)
{ m_tr.stage_cycle[stage] = cycle; }

void TraceCtrl::SetParaGroupSwitched (bool is_switched)
{ m_tr.is_para_grp_switched = is_switched; }

#ifdef SUPPORT_CUBESUITE
void TraceCtrl::SetNextPeId (uint32_t next_pe_id)
{ m_tr.next_pe_id = next_pe_id; }
#endif //#ifdef SUPPORT_CUBESUITE

void TraceCtrl::SetSimState (NotifySimState state)
{ m_tr.sim_state = state; }

void TraceCtrl::SetStepCount (uint64_t step)
{ m_tr.step_count = step; }

void TraceCtrl::IncStepCount (void)
{ ++ m_tr.step_count; }

void TraceCtrl::SetCancel (bool is_cancel)
{ m_tr.is_cancel = is_cancel; }

#if defined(COMP_RUNNER)
void TraceCtrl::SetCancelMcyc (bool is_cancel)
{ m_tr.is_cancel_mcyc = is_cancel; }
#endif // #if defined(COMP_RUNNER)

#ifdef SUPPORT_SC_HEAP
void TraceCtrl::SetSuspendState (SuspendState state)
{ m_tr.suspend_state = state; }

void TraceCtrl::SetGuardError (GuardErrType err)
{ m_tr.guard_err |= err; } // 累積する
#endif // SUPPORT_SC_HEAP

#ifdef CLUSTER
uint32_t TraceCtrl::GetClId (void) const
{ return ForestUtil::ExtractClId (m_tr.pe_id); }
#endif // CLUSTER

#if defined (ENABLE_TRACE_MDP_SUPPRESS)
MemAccessRoute TraceCtrl::GetRmemMdpRoute (int32_t i) const
{ return m_tr.rmem_mdp_route[i]; }

MemAccessRoute TraceCtrl::GetWmemMdpRoute (int32_t i) const
{ return m_tr.wmem_mdp_route[i]; }

int32_t TraceCtrl::GetRmemMdpCount (void) const
{ return m_tr.rmem_mdp_count; }

int32_t TraceCtrl::GetWmemMdpCount (void) const
{ return m_tr.wmem_mdp_count; }

bool TraceCtrl::GetRmemMdpIssued (int32_t i) const
{ return m_tr.rmem_mdp_issued[i]; }

bool TraceCtrl::GetWmemMdpIssued (int32_t i) const
{ return m_tr.wmem_mdp_issued[i]; }

MemData64 TraceCtrl::GetRmemMdpValueL64 (int32_t i) const
{ return m_tr.rmem_mdp_value_l64[i];}

MemData64 TraceCtrl::GetRmemMdpValueU64 (int32_t i) const
{ return m_tr.rmem_mdp_value_u64[i];}

MemData64 TraceCtrl::GetWmemMdpValueL64 (int32_t i) const
{ return m_tr.wmem_mdp_value_l64[i];}

MemData64 TraceCtrl::GetWmemMdpValueU64 (int32_t i) const
{ return m_tr.wmem_mdp_value_u64[i];}

PhysAddr TraceCtrl::GetRmemMdpAddr (int32_t i) const
{ return m_tr.rmem_mdp_addr[i]; }

PhysAddr TraceCtrl::GetWmemMdpAddr (int32_t i) const
{ return m_tr.wmem_mdp_addr[i]; }

ByteSize TraceCtrl::GetRmemMdpSize (int32_t i) const
{ return m_tr.rmem_mdp_size[i]; }

ByteSize TraceCtrl::GetWmemMdpSize (int32_t i) const
{ return m_tr.wmem_mdp_size[i]; }

MemMappedArea TraceCtrl::GetRmemMdpTarget (int32_t i) const
{ return m_tr.rmem_mdp_target[i]; }

MemMappedArea TraceCtrl::GetWmemMdpTarget (int32_t i) const
{ return m_tr.wmem_mdp_target[i]; }
#endif // #if defined (ENABLE_TRACE_MDP_SUPPRESS)

#ifdef FROG_API_IF
int32_t TraceCtrl::GetDeletedLLBitCount (void) const
{ return m_tr.del_llbit_count; }

std::pair <PhysAddr, ByteSize> TraceCtrl::GetLLBitAdd (void) const
{ return m_tr.add_llbit; }

std::pair<int32_t, PhysAddr> TraceCtrl::GetDeletedLLBit (int32_t i) const
{ return m_tr.del_llbit_paddr[i];}

bool TraceCtrl::DelLLBit (int32_t peid, PhysAddr paddr) {
    int32_t cnt = m_tr.del_llbit_count;
    for (int i = 0; i < cnt; i++) {
        if ((m_tr.del_llbit_paddr[i].first == peid) && (m_tr.del_llbit_paddr[i].second == paddr)) {
            --cnt;
            m_tr.del_llbit_paddr[i].first  = m_tr.del_llbit_paddr[cnt].first;
            m_tr.del_llbit_paddr[i].second = m_tr.del_llbit_paddr[cnt].second;
            m_tr.del_llbit_count = cnt;
        }
    }
    return true;
}
#endif //FROG_API_IF

#ifdef SUPPORT_SC_HEAP
SuspendState TraceCtrl::GetSuspendState (void) const
{ return m_tr.suspend_state; }

GuardErrType TraceCtrl::GetGuardError (void) const
{ return m_tr.guard_err; }
#endif // SUPPORT_SC_HEAP

void TraceCtrl::Clean (void)
{
    m_tr.fetch_count = 0;
    m_tr.ic_count = 0;
    m_tr.rmem_count = 0;
    m_tr.wmem_count = 0;
#if defined (ENABLE_TRACE_MDP_SUPPRESS)
    m_tr.rmem_mdp_count = 0;
    m_tr.wmem_mdp_count = 0;
#endif // #if defined (ENABLE_TRACE_MDP_SUPPRESS)
    m_tr.rreg_count = 0;
    m_tr.wreg_count = 0;
#ifdef FROG_API_IF
    m_tr.add_llbit.first       = 0;
    m_tr.del_llbit_count  = 0;
#endif //FROG_API_IF
    m_tr.branch_info = NO_BRANCH;
    m_tr.bp_result = BP_NONE;
    m_tr.is_para_grp_switched = false;
    m_tr.exp_code = EXP_NONE;
    m_tr.exp_cause = EXP_CAUSE_NONE;
    m_tr.rdep_regs.clear();
    m_tr.wdep_regs.clear();
    m_tr.is_cancel = false;
#if defined(COMP_RUNNER)
    m_tr.is_cancel_mcyc = false;
#endif // #if defined(COMP_RUNNER)

#ifdef SUPPORT_SC_HEAP
    m_tr.suspend_state = NO_SUSPEND;
    m_tr.guard_err = GD_NONE_ERR;
    m_tr.is_pswh_changed = false;
#endif // SUPPORT_SC_HEAP

    m_tr.is_inst_ic_hit = false;
}


void TraceCtrl::InitTime (void)
{
    m_tr.step_count = 0x0;
    m_tr.stage_cycle[STAGE_IA]  = 0x0;
    m_tr.stage_cycle[STAGE_IT]  = 0x0;
    m_tr.stage_cycle[STAGE_DP]  = 0x0;
    m_tr.stage_cycle[STAGE_EA]  = 0x0;
    m_tr.stage_cycle[STAGE_MEM] = 0x0;
    m_tr.stage_cycle[STAGE_AL]  = 0x0;
    m_tr.stage_cycle[STAGE_WB]  = 0x0;
}


TraceCtrl::TraceCtrl (uint32_t pe_id)
{
    memset ( static_cast<void*>(&m_tr), 0x00, sizeof(m_tr) );

    m_tr.pe_id = pe_id;
    m_tr.step_count = 0x0;
    m_tr.sim_state = SIMULATE_STOP;
    Clean ();
}


TraceCtrl::TraceCtrl (const TraceInfo *tr)
{
    memcpy ( static_cast<void*>(&m_tr), tr, sizeof (m_tr));
}


// template explicit instantiation
template void TraceCtrl::RecordPC<TraceCtrl::REG_WRITE> (PhysAddr addr);
template void TraceCtrl::RecordPC<TraceCtrl::REG_READ> (PhysAddr addr);
template void TraceCtrl::RecordPsw<TraceCtrl::REG_WRITE> (CReg regno, RegData data);
template void TraceCtrl::RecordPsw<TraceCtrl::REG_READ> (CReg regno, RegData data);
template void TraceCtrl::RecordSesr<TraceCtrl::REG_WRITE> (CReg regno, bool data);
template void TraceCtrl::RecordSesr<TraceCtrl::REG_READ> (CReg regno, bool data);
template void TraceCtrl::RecordGReg<TraceCtrl::REG_WRITE> (uint32_t regno, RegData data);
template void TraceCtrl::RecordGReg<TraceCtrl::REG_READ> (uint32_t regno, RegData data);
template void TraceCtrl::RecordWReg<TraceCtrl::REG_WRITE> (uint32_t regno, RegData64 dw1, RegData64 dw0);
template void TraceCtrl::RecordWReg<TraceCtrl::REG_READ> (uint32_t regno, RegData64 dw1, RegData64 dw0);
template void TraceCtrl::RecordSReg<TraceCtrl::REG_WRITE> (CfSReg cfsrid, RegData data);
template void TraceCtrl::RecordSReg<TraceCtrl::REG_READ> (CfSReg cfsrid, RegData data);

#if defined(COMP_RUNNER)
template void TraceCtrl::RecordSReg<TraceCtrl::REG_WRITE> (CReg sregid, RegData data);
template void TraceCtrl::RecordSReg<TraceCtrl::REG_READ> (CReg sregid, RegData data);
#endif //#if defined(COMP_RUNNER)
