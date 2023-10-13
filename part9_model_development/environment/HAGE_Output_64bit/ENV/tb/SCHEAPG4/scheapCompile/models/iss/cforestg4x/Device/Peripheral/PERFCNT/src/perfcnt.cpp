/*
 * (c) 2011,2012,2013,2014 Renesas Electronics Corporation
 * RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY
 * This program must be used solely for the purpose for which
 * it was furnished by Renesas Electronics Corporation. No part of this
 * program may be reproduced or disclosed to others, in any
 * form, without the prior written permission of Renesas Electronics
 * Corporation.
 */

#include <vector>
#include "./config.h"
#include "./perfcnt.h"
#include "./forest.h"
#include "./forest_utility.h"
#include "./forest_internals.h"
#include "./forest_message.h"
#include "./fs_controler.h"
#include "./proc_element.h"


SysErrCause Cperfcnt::TargetWrite (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size)
{
    PhysAddr local_addr = addr & PERFCNT_ADDR_MASK;

    switch (local_addr) {
    case PERF_CTRL_ADDR: WriteCNTCTRL (data, size); break;
    case PERF_CNT_ADDR:  WriteCNTDAT  (data, size); break;
    default:
        CfMsg::DPrint (MSG_ERR, "<Error: Unsupported register is accessed>\n");
        break;
    }

    return SYSERR_NONE;
}


SysErrCause Cperfcnt::TargetRead (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size)
{
    PhysAddr local_addr = addr & PERFCNT_ADDR_MASK;

    switch (local_addr) {
    case PERF_CTRL_ADDR: ReadCNTCTRL (data, size); break;
    case PERF_CNT_ADDR: ReadCNTDAT  (data, size); break;
    default:
        CfMsg::DPrint (MSG_ERR, "<Error: Unsupported register is accessed>\n");
        break;
    }

    return SYSERR_NONE;
}


SysErrCause Cperfcnt::TargetWriteDebug (PhysAddr addr, uint8_t* data, ByteSize size)
{
    return TargetWrite (SideBand::DebugAccess (), addr, data, size);
}


SysErrCause Cperfcnt::TargetReadDebug (PhysAddr addr, uint8_t* data, ByteSize size)
{
    return TargetRead (SideBand::DebugAccess (), addr, data, size);
}


void Cperfcnt::WriteCNTCTRL (uint8_t* data, ByteSize size)
{
    if ((data[0]&0x02) == 0x02) {
        CountWrite (0);
        if (p_forest_util->IsPerfInfoWorkWithPerfcnt()) {
            p_forest_util->ResetPerfInfo (m_peid);
        }
    }
    if ((data[0]&0x01) == 0x01) {
        CountStart ();
        if (p_forest_util->IsPerfInfoWorkWithPerfcnt()) {
            p_forest_util->ResetPerfInfo (m_peid);
        }
    } else {
        CountStop ();
    }
#ifdef CLUSTER
    CfMsg::PerfcntPrint ("C%d:", m_clid);
#endif // CLUSTER
    CfMsg::PerfcntPrint ("P%d:<<<<< APBDMY >>>>> CNTCTRL Register Write [CNTEN=%d], [CNTDAT=%d]\n",
                   m_peid, m_cnten, m_counter);
}


void Cperfcnt::ReadCNTCTRL (uint8_t* data, ByteSize size)
{
    for (ByteSize i = 0; i < size; i++) {
        if(i == 0){
            data[i] = m_cnten & 0x1;
        } else {
            data[i] = 0;
        }
    }
    CountUpdate ();

#ifdef CLUSTER
    CfMsg::PerfcntPrint ("C%d:", m_clid);
#endif // CLUSTER
    CfMsg::PerfcntPrint ("P%d:<<<<< APBDMY >>>>> CNTCTRL Register Read [CNTCTRL=%d]\n", m_peid, m_cnten);
}


void Cperfcnt::WriteCNTDAT (uint8_t* data, ByteSize size)
{
    uint32_t val = m_counter;
    for (ByteSize i = 0; i < size; i++) {
        val = (val & ~(0xFFU <<(i*8))) | (data[i]<<(i*8));
    }
    uint32_t m_counter_before = CountRead ();
    CountWrite (val);
#ifdef CLUSTER
    CfMsg::PerfcntPrint ("C%d:", m_clid);
#endif // CLUSTER
    CfMsg::PerfcntPrint ("P%d:<<<<< APBDMY >>>>> CNTDAT Register Write (%d -> %d)\n", m_peid,
                   m_counter_before, m_counter);
}


void Cperfcnt::ReadCNTDAT(uint8_t* data, ByteSize size)
{
    uint32_t val = CountRead ();
    for (ByteSize i = 0; i < size; i++) {
        data[i] = (val>>(i*8)) & 0xFF;
    }
#ifdef CLUSTER
    CfMsg::PerfcntPrint ("C%d:", m_clid);
#endif // CLUSTER
    CfMsg::PerfcntPrint ("P%d:<<<<< APBDMY >>>>> CNTDAT Register Read [CNTDAT=%d]\n", m_peid, m_counter);

    if (p_forest_util->IsPerfInfoWorkWithPerfcnt()) {
        p_forest_util->ShowPerfInfo (m_peid);
    }
}


void Cperfcnt::CountWrite (uint32_t cycle)
{
    CountUpdate ();
    m_counter = cycle;
}


uint32_t Cperfcnt::CountRead (void)
{
    CountUpdate ();
    return m_counter;
}


void Cperfcnt::CountStart (void)
{
    CountUpdate ();
    m_cnten = 1;
}


void Cperfcnt::CountStop (void)
{
    CountUpdate ();
    m_cnten = 0;
}


void Cperfcnt::CountUpdate (void)
{
    uint32_t now = (p_internals->GetSystemTime ()) & 0xFFFFFFFF;
    if(m_cnten == 1) {
        m_counter += now - m_count_start_time;
    }
    m_count_start_time = now;
}


void Cperfcnt::HardReset (void)
{
    m_cnten = 0;
    CountWrite (0);
    m_count_start_time = 0;
}


Cperfcnt::Cperfcnt (uint32_t clid, uint32_t peid)
    : m_clid (clid), m_peid (peid)
{
    HardReset();
}


Cperfcnt::~Cperfcnt ()
{
}
