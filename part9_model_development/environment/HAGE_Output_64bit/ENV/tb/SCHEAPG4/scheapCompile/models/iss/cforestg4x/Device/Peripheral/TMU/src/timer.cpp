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
#include "./config.h"
#include "./forest_common.h"
#include "./forest.h"
#include "./timer.h"
#include "./fsbus.h"
#include "./forest_message.h"
#include "./intc2.h"


// @@ 暫定実装
// @@ 仕様がFIXがしたら書きなおす

void CTimer::CyclicHandler (void)
{
    if (m_tmuch[0]->IsRunning() == true) { m_tmuch[0]->CountDown(); }
    if (m_tmuch[1]->IsRunning() == true) { m_tmuch[1]->CountDown(); }
    if (m_tmuch[2]->IsRunning() == true) { m_tmuch[2]->CountDown(); }
}


SysErrCause CTimer::TargetWriteBody (PhysAddr addr, uint8_t* data, ByteSize size)
{
    uint32_t val = 0;
    for (ByteSize i = 0; i < size; i++) {
        val |= (data[i] & 0xffU) << (i << 3);
    }

    switch (addr & TMU_CHANNEL_MASK) {
    case 0x04: SetTSTR (val); break;
    case 0x08: m_tmuch[0]->SetTCOR (val); break;
    case 0x0C: m_tmuch[0]->SetTCNT (val); break;
    case 0x10: m_tmuch[0]->SetTCR  (val); break;
    case 0x14: m_tmuch[1]->SetTCOR (val); break;
    case 0x18: m_tmuch[1]->SetTCNT (val); break;
    case 0x1C: m_tmuch[1]->SetTCR  (val); break;
    case 0x20: m_tmuch[2]->SetTCOR (val); break;
    case 0x24: m_tmuch[2]->SetTCNT (val); break;
    case 0x28: m_tmuch[2]->SetTCR  (val); break;
    default:
        CfMsg::DPrint (MSG_WAR, "<Warning: no registers for Timer\n");
        break;
    }

    return SYSERR_NONE;
}


SysErrCause CTimer::TargetReadBody (PhysAddr addr, uint8_t* data, ByteSize size)
{
    uint32_t val;

    switch (addr & TMU_CHANNEL_MASK) {
    case 0x04: val = GetTSTR (); break;
    case 0x08: val = m_tmuch[0]->GetTCOR (); break;
    case 0x0C: val = m_tmuch[0]->GetTCNT (); break;
    case 0x10: val = m_tmuch[0]->GetTCR ();  break;
    case 0x14: val = m_tmuch[1]->GetTCOR (); break;
    case 0x18: val = m_tmuch[1]->GetTCNT (); break;
    case 0x1C: val = m_tmuch[1]->GetTCR ();  break;
    case 0x20: val = m_tmuch[2]->GetTCOR (); break;
    case 0x24: val = m_tmuch[2]->GetTCNT (); break;
    case 0x28: val = m_tmuch[2]->GetTCR ();  break;
    default:
        CfMsg::DPrint (MSG_WAR, "<Warning: no registers for Timer\n");
        val = 0x0;
        break;
    }

    for (ByteSize i = 0; i < size; i++) {
        data[i] = (val >> (i << 3)) & 0xffU;
    }

    return SYSERR_NONE;
}


SysErrCause CTimer::TargetWrite (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size)
{
    return TargetWriteBody (addr, data, size);
}


SysErrCause CTimer::TargetRead (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size)
{
    return TargetReadBody (addr, data, size);
}


SysErrCause CTimer::TargetWriteDebug (PhysAddr addr, uint8_t* data, ByteSize size)
{
    return TargetWriteBody (addr, data, size);
}


SysErrCause CTimer::TargetReadDebug (PhysAddr addr, uint8_t* data, ByteSize size)
{
    return TargetReadBody (addr, data, size);
}


void CTimer::SetTSTR (uint32_t val)
{
    m_tmuch[0]->SetRunning ((val >> 0) & 0x01);
    m_tmuch[1]->SetRunning ((val >> 1) & 0x01);
    m_tmuch[2]->SetRunning ((val >> 2) & 0x01);

    if ((val & 0x7) == 0x0) {
        m_fsbus->RemoveCyclicHandler (this);
    } else {
        m_fsbus->AddCyclicHandler (this);
    }
}


uint32_t CTimer::GetTSTR (void) const
{
    return ((m_tmuch[0]->IsRunning () << 0)
            | (m_tmuch[1]->IsRunning () << 1)
            | (m_tmuch[2]->IsRunning () << 2));
}


void CTimer::HardReset (void)
{
    m_tmuch[0]->HardReset ();
    m_tmuch[1]->HardReset ();
    m_tmuch[2]->HardReset ();
}


CTimer::CTimer (uint32_t tmuid, FsBus* fsbus, CIntc2* intc2)
    : m_fsbus (fsbus)
{
    for (uint32_t id = 0; id < 3; id++) {
        m_tmuch[id] = new TmuChannel (tmuid * N_CH_PER_TMU + id, intc2);
    }
}


CTimer::~CTimer ()
{
    delete m_tmuch[0];
    delete m_tmuch[1];
    delete m_tmuch[2];
}


void TmuChannel::CountDown (void)
{
    if (m_counter != 0) {
        -- m_counter;
    } else {
        m_counter = m_constant;
        m_underflow_flag = true;
        if (m_underflow_enable == true) {
            m_intc2->ReqIntByPeripheral (TMU_BASE_CHANNEL + m_chid);
        }
    }
}


void TmuChannel::SetRunning (bool run) { m_is_running = run; }
bool TmuChannel::IsRunning (void) const { return m_is_running; }
void TmuChannel::SetTCOR (uint32_t val) { m_constant = val; }
void TmuChannel::SetTCNT (uint32_t val) { m_counter  = val; }
uint32_t TmuChannel::GetTCOR (void) const { return m_constant; }
uint32_t TmuChannel::GetTCNT (void) const { return m_counter; }


void TmuChannel::SetTCR  (uint32_t val)
{
    m_underflow_flag   = (val >> 8) & 0x01;
    m_underflow_enable = (val >> 5) & 0x01;
}


uint32_t TmuChannel::GetTCR  (void) const
{
    return ((m_underflow_flag << 8) | (m_underflow_enable << 5));
}


void TmuChannel::HardReset (void)
{
    m_is_running = false;
    m_counter    = 0x00FFFFFFFFU;
    m_constant   = 0x00FFFFFFFFU;
    m_underflow_flag   = false;
    m_underflow_enable = false;
}


TmuChannel::TmuChannel (uint32_t chid, CIntc2* intc2)
    : m_chid (chid),
      m_intc2 (intc2)
{
    HardReset ();
}
