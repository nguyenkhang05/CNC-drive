/*
 * (c) 2015 Renesas Electronics Corporation
 * RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY
 * This program must be used solely for the purpose for which
 * it was furnished by Renesas Electronics Corporation. No part of this
 * program may be reproduced or disclosed to others, in any
 * form, without the prior written permission of Renesas Electronics
 * Corporation.
 */

#include <stdio.h>
#include <limits>
#include "./config.h"
#include "./counter_base.h"
#include "./forest_internals.h"
#include "./forest_message.h"

template <typename TYPE>
TYPE BaseCounter<TYPE>::GetCount (void)
{
    Update ();
    return GetCountBody ();
}

template <typename TYPE>
void BaseCounter<TYPE>::SetCount (TYPE val)
{
    Update ();
    SetCountBody (val);
}


template <typename TYPE>
bool BaseCounter<TYPE>::GetOvf (void)
{
    Update ();
    return GetOvfBody ();
}


template <typename TYPE>
void BaseCounter<TYPE>::SetOvf (bool is_ovf)
{
    Update ();
    SetOvfBody (is_ovf);
}


template <typename TYPE>
void BaseCounter<TYPE>::Reset (void)
{
    m_start_val = 0;
    m_counter = 0;
    m_start = false;
    m_ovf = false;
    m_en = true; // Initial state is non-debug mode.
}


template <typename TYPE>
void BaseCounter<TYPE>::SetStart (bool is_start)
{
    Update ();
    SetStartBody (is_start);
}

template <typename TYPE>
bool BaseCounter<TYPE>::GetStart (void)
{
    Update ();
    return GetStartBody ();
}


template <typename TYPE>
void BaseCounter<TYPE>::SetDisable (void)
{
    Update ();
    SetEnableBody (false);
}

template <typename TYPE>
void BaseCounter<TYPE>::SetEnable (void)
{
    Update ();
    SetEnableBody (true);
}


template <typename TYPE>
void BaseCounter<TYPE>::Update (void)
{
    if (IsRunning ()) {
        // Update Counter
        INTER_SIZE start_val = GetStartVal ();
        INTER_SIZE curr_val = GetTargetVal ();
        INTER_SIZE elapsed_val = (curr_val - start_val);

        TYPE count = GetCountBody ();
        const TYPE max_val = std::numeric_limits<TYPE>::max();

        // Check overflow
        // - start_val > curr_val : GetTargetVal() returs smaller value than previous
        // - (max_val - count) < elapsed_val : count + elapsed_val exceeds max count
        if ((start_val > curr_val) || ((max_val - count) < elapsed_val)) {
            // OVF
            SetOvfBody (true);
        }

        TYPE updated_count = static_cast<TYPE>((count + elapsed_val) & max_val);
        SetCountBody (updated_count);

        // Update start val for next.
        SetStartVal (curr_val);
    } else {
        // Update start val for next.
        SetStartVal (GetTargetVal ());
    }
}

template <typename TYPE>
void BaseCounter<TYPE>::UpdateStartCount (void)
{
    SetStartVal (GetTargetVal ());
}


BaseCounter<uint64_t>::INTER_SIZE CycleCounter::GetTargetVal (void)
{
    return p_internals->GetCpuTime (m_peid);
}


BaseCounter<uint32_t>::INTER_SIZE PmCounter::GetTargetVal (void)
{
    // PM_CNT_NORMALCYC, PM_CNT_PSWID0CYC:
    // Counter value is same as PM_CNT_CYC.
    // But, Disable the count by SetDisable() for each condition. 

    INTER_SIZE val;
    switch (m_cond_type) {
    case PM_CNT_CYC:        val = p_internals->GetCpuTime (m_peid); break;
    case PM_CNT_STEP:       val = p_internals->GetInstCountPmc (m_peid); break;
    case PM_CNT_BRANCH:     val = p_internals->GetBranchCount (m_peid); break;
    case PM_CNT_CNDBCNT:    val = p_internals->GetBranchCondExecCount (m_peid); break;
    case PM_CNT_BPMISS:     val = p_internals->GetBranchCondMissCount (m_peid); break;
    case PM_CNT_EIINT:      val = p_internals->GetEiintCount (m_peid, m_id); break;
    case PM_CNT_FEINT:      val = p_internals->GetFeintCount (m_peid, m_id); break;
    case PM_CNT_SUSPEND:    val = p_internals->GetExpAbortCount (m_peid, m_id); break;
    case PM_CNT_REEXEC:     val = p_internals->GetExpNonAbortCount (m_peid, m_id); break;
    case PM_CNT_BGINT_SUSPENDED: val = p_internals->GetBgintAbortCount (m_peid, m_id); break;
    case PM_CNT_BGINT_ACCEPTED:  val = p_internals->GetBgintAccesptedCount (m_peid, m_id); break;
    case PM_CNT_NORMALCYC:  val = p_internals->GetCpuTime (m_peid); break;
    case PM_CNT_PSWID0CYC:  val = p_internals->GetCpuTime (m_peid); break;
    case PM_CNT_FETCH:      val = p_internals->GetTotalFetchCount(m_peid); break;
    case PM_CNT_ICHIT:      val = p_internals->GetIcacheHitCount (m_peid); break;
    case PM_CNT_ISSUE_STALL: val = p_internals->GetInstIssueStall (m_peid); break;
    case PM_CNT_FLASH_I:    val = p_internals->GetRomFetchCount(m_peid); break;
    case PM_CNT_FLASH_D:    val = p_internals->GetRomDataAccCount(m_peid); break;
    default:                val = 0; break;
    }

    return val;
}


void PmCounter::SetCondition (uint32_t cond)
{
    Update ();
    m_cond = cond;

    switch (cond) {
    case PM_CNT_CYC:        m_cond_type = PM_CNT_CYC;       break;
    case PM_CNT_STEP:       m_cond_type = PM_CNT_STEP;      break;
    case PM_CNT_BRANCH:     m_cond_type = PM_CNT_BRANCH;    break;
    case PM_CNT_CNDBCNT:    m_cond_type = PM_CNT_CNDBCNT;   break;
    case PM_CNT_BPMISS:     m_cond_type = PM_CNT_BPMISS;    break;
    case PM_CNT_EIINT:      m_cond_type = PM_CNT_EIINT;     break;
    case PM_CNT_FEINT:      m_cond_type = PM_CNT_FEINT;     break;
    case PM_CNT_SUSPEND:    m_cond_type = PM_CNT_SUSPEND;   break;
    case PM_CNT_REEXEC:     m_cond_type = PM_CNT_REEXEC;    break;
    case PM_CNT_BGINT_SUSPENDED: m_cond_type = PM_CNT_BGINT_SUSPENDED; break;
    case PM_CNT_BGINT_ACCEPTED:  m_cond_type = PM_CNT_BGINT_ACCEPTED; break;
    case PM_CNT_NORMALCYC:  m_cond_type = PM_CNT_NORMALCYC; break;
    case PM_CNT_PSWID0CYC:  m_cond_type = PM_CNT_PSWID0CYC; break;
    case PM_CNT_FETCH:      m_cond_type = PM_CNT_FETCH;     break;
    case PM_CNT_ICHIT:      m_cond_type = PM_CNT_ICHIT;     break;
    case PM_CNT_ISSUE_STALL: m_cond_type = PM_CNT_ISSUE_STALL; break;
    case PM_CNT_FLASH_I:    m_cond_type = PM_CNT_FLASH_I;   break;
    case PM_CNT_FLASH_D:    m_cond_type = PM_CNT_FLASH_D;   break;
    default:                m_cond_type = PM_CNT_OTHER;     break;
    }
    if (m_cond_type == PM_CNT_OTHER) {
        CfMsg::DPrint (MSG_INF, "<Invalid condition(0x%02x) in PMCTRL>\n", cond);
    }
    // Update start count by new condition.
    UpdateStartCount ();
}


void PmCounter::SetSubCondition (uint32_t subcond)
{
    Update ();
    m_subcond = subcond;

    switch (m_cond) {
    case PM_CNT_EIINT:
    case PM_CNT_FEINT:
        // Update start count by new condition.
        UpdateStartCount ();
        break;
    default:
        // No effect.
        break;
    }
}


void CycleCounter::SetRegisterByReservedInfo (void)
{
    if (m_reserved) {
        SetStart (m_reserved_cen);
        SetOvf (m_reserved_ovf);
        m_reserved = false;
    }
}


void CycleCounter::Reset (void)
{
    base::Reset ();

    m_reserved = false;
    m_reserved_ovf = false;
    m_reserved_cen = false;
}


void PmCounter::SetRegisterByReservedInfo (void)
{
    if (m_reserved) {
        SetCondition (m_reserved_cnd);
        SetStart (m_reserved_cen);
        SetOvf (m_reserved_ovf);
        SetHe (m_reserved_he);
        SetGe (m_reserved_ge);
        m_reserved = false;
    }
    if (m_reserved_set_subcnd) {
        SetSubCondition (m_reserved_subcnd);
        m_reserved_set_subcnd = false;
    }
}


uint32_t PmCounter::GetCondition (void)
{
    Update ();
    return m_cond;
}


uint32_t PmCounter::GetSubCondition (void)
{
    Update ();
    return m_subcond;
}


void PmCounter::SetHe (bool he)
{
    Update ();
    m_he = he;
}


bool PmCounter::GetHe (void)
{
    Update ();
    return m_he;
}


void PmCounter::SetGe (uint32_t ge)
{
    Update ();
    m_ge = ge;
}


uint32_t PmCounter::GetGe (void)
{
    Update ();
    return m_ge;
}


void PmCounter::Reset (void)
{
    base::Reset ();
    m_cond = 0;
    m_cond_type = PM_CNT_CYC;
    m_subcond = 0;

    m_reserved = false;
    m_reserved_set_subcnd = false;
    m_reserved_cnd = 0;
    m_reserved_subcnd = 0;
    m_reserved_ovf = false;
    m_reserved_cen = false;
    m_reserved_he = false;
    m_reserved_ge = 0;
}


template class BaseCounter<uint64_t>;
template class BaseCounter<uint32_t>;
