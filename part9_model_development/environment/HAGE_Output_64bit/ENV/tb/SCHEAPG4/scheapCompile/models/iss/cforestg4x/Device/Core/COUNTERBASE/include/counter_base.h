/*
 * (c) 2015 Renesas Electronics Corporation
 * RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY
 * This program must be used solely for the purpose for which
 * it was furnished by Renesas Electronics Corporation. No part of this
 * program may be reproduced or disclosed to others, in any
 * form, without the prior written permission of Renesas Electronics
 * Corporation.
 */

#pragma once

#include <inttypes.h>
#include "forest_common.h"

template <typename TYPE>
class BaseCounter
{
public:
    TYPE GetCount (void);
    void SetCount (TYPE val);
    bool GetOvf (void);
    void SetOvf (bool is_ovf);
    virtual void Reset (void);
    void SetStart (bool is_start);
    bool GetStart (void);
    void SetEnable (void);
    void SetDisable (void);

    BaseCounter () : m_start_val (0), m_counter (0), m_start (false), m_ovf (false), m_en (true)
                     { Reset (); }
    virtual ~BaseCounter () {}

protected:
    // To prevent overflow in base counter, 64bit value (doesn't overflow) should be returned.
    typedef uint64_t INTER_SIZE;

    void Update (void);
    void UpdateStartCount (void);

private:
    virtual INTER_SIZE GetTargetVal (void) = 0;

    bool GetStartBody (void) const { return m_start; }
    void SetStartBody (bool start) { m_start = start; }

    INTER_SIZE GetStartVal (void) const { return m_start_val; }
    void SetStartVal (INTER_SIZE val) { m_start_val = val; }

    TYPE GetCountBody (void) const { return m_counter; }
    void SetCountBody (TYPE val) { m_counter = val; }

    bool GetOvfBody (void) const { return m_ovf; }
    void SetOvfBody (bool is_ovf) { m_ovf = is_ovf; }

    bool GetEnableBody (void) const { return m_en; }
    void SetEnableBody (bool is_enable) { m_en = is_enable; }

    bool IsRunning (void) const { return m_start && m_en; }

    INTER_SIZE m_start_val;
    TYPE m_counter;
    bool m_start;
    bool m_ovf;
    bool m_en;
};


class CycleCounter : public BaseCounter <uint64_t>
{
public:
    virtual uint64_t GetTargetVal (void);
    virtual void Reset (void);
    CycleCounter (uint32_t peid, ContextId tcid) : m_peid (peid), m_tcid (tcid) {}
    virtual ~CycleCounter () {}

    void ReserveSetting (bool cen, bool ovf) {
        m_reserved_ovf = ovf;
        m_reserved_cen = cen;
        m_reserved = true;
    }
    void SetRegisterByReservedInfo (void);

private:
    uint32_t m_peid;
    ContextId m_tcid;

    bool m_reserved;
    bool m_reserved_ovf;
    bool m_reserved_cen;

    typedef BaseCounter<uint64_t> base;
};


class PmCounter : public BaseCounter <uint32_t>
{
public:
    enum PmCondType {
        PM_CNT_CYC       = 0x00,
        PM_CNT_STEP      = 0x10,
        PM_CNT_BRANCH    = 0x18,
        PM_CNT_CNDBCNT   = 0x19,
        PM_CNT_BPMISS    = 0x1A,
        PM_CNT_EIINT     = 0x20,
        PM_CNT_FEINT     = 0x21,
        PM_CNT_SUSPEND   = 0x22,
        PM_CNT_REEXEC    = 0x23,
        PM_CNT_BGINT_SUSPENDED = 0x24, // for G4MH2.0 and later
        PM_CNT_BGINT_ACCEPTED = 0x25, // for G4MH2.0 and later
        PM_CNT_NORMALCYC = 0x28,
        PM_CNT_PSWID0CYC = 0x29,
        PM_CNT_FETCH     = 0x30,
        PM_CNT_ICHIT     = 0x31,
        PM_CNT_ISSUE_STALL = 0x40,
        PM_CNT_FLASH_I   = 0x50,
        PM_CNT_FLASH_D   = 0x51,
        PM_CNT_OTHER
    };

    virtual uint64_t GetTargetVal (void);
    virtual void Reset (void);

    void SetCondition (uint32_t cond);
    uint32_t GetCondition (void);
    PmCondType GetConditionBody (void) const { return m_cond_type; }

    void SetSubCondition (uint32_t subcond);
    uint32_t GetSubCondition (void);
    uint32_t GetSubConditionBody (void) const { return m_subcond; }

    void SetHe (bool he);
    bool GetHe (void);
    bool GetHeBody (void) const { return m_he; }

    void SetGe (uint32_t ge);
    uint32_t GetGe (void);
    uint32_t GetGeBody (void) const { return m_ge; }
    bool IsGpidMatch (uint32_t gpid) { Update (); return IsGpidMatchBody (gpid); }
    bool IsGpidMatchBody (uint32_t gpid) const { return (((m_ge >> gpid) & 0x1) == 0x1); }

    bool m_reserved;
    bool m_reserved_set_subcnd;
    uint32_t m_reserved_cnd;
    uint32_t m_reserved_subcnd;
    bool m_reserved_ovf;
    bool m_reserved_cen;
    bool m_reserved_he;
    uint32_t m_reserved_ge;
    void ReserveSetting (uint32_t cnd, bool cen, bool ovf, bool he, uint32_t ge) {
        m_reserved_cnd = cnd;
        m_reserved_ovf = ovf;
        m_reserved_cen = cen;
        m_reserved = true;
        m_reserved_he = he;
        m_reserved_ge = ge;
    }

    void ReserveSettingSubcond (uint32_t subcnd) {
        m_reserved_subcnd = subcnd;
        m_reserved_set_subcnd = true;
    }

    void SetRegisterByReservedInfo (void);

    PmCounter (uint32_t peid, ContextId tcid, uint32_t id)
    : m_peid (peid), m_tcid (tcid), m_id (id), m_cond (0), m_subcond (0), m_cond_type (PM_CNT_CYC), m_he (0), m_ge (0) {}
    virtual ~PmCounter () {}

private:
    uint32_t m_peid;
    ContextId m_tcid;
    uint32_t m_id;
    uint32_t m_cond;
    uint32_t m_subcond;
    PmCondType m_cond_type;
    bool m_he;
    uint32_t m_ge;

    typedef BaseCounter<uint32_t> base;

    void SetCondType (PmCondType cond)  { m_cond_type = cond; }
    PmCondType GetCondType (void) const { return m_cond_type; }
};
