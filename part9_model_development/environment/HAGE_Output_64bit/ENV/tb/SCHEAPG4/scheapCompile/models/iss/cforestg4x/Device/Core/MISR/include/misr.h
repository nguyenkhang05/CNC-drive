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

#include <vector>
#include "./forest_common.h"
#include "./module_vc.h"
#include "./module_guard.h"
#include "./treg.h"

class MisrL;
class MisrH;
class MisrCdrl;
class MisrCr;
class MisrBaseAdr;
class MisrAdrMsk;
class MisrDcntCtl;
class MisrDcnt;
class TRegMisr;
class Misr;


/*!
  @brief The class to be the members MISR1L and MISR2L of MISR Register.
*/
class MisrL : public TRegDataBase<TRegData32> {
private:
    union {
        TRegData32 m_data;
        RegData    m_bit;
    };

public:
    static const uint32_t MISRL_MASK = 0xFFFFFFFF;
    void Reset (void);

    const char* GetName (void) const { return "MisrL"; }
    void SetData (RegData data) { m_bit = data; }
    RegData GetData (void) const { return m_bit; }

    explicit MisrL (PhysAddr reg_addr);
};


/*!
  @brief The class to be the members MISR1H and MISR2H of MISR Register.
*/
class MisrH : public TRegDataBase<TRegData32> {
private:
    union {
        TRegData32 m_data;
        RegData    m_bit;
    };

public:
    static const uint32_t MISRH_MASK = 0xFFFFFFFF;

    void Reset (void);

    const char* GetName (void) const { return "MisrH"; }
    void SetData (RegData data) { m_bit = data; }
    RegData GetData (void) const { return m_bit; }

    explicit MisrH (PhysAddr reg_addr);
};


/*!
  @brief The class to the member MISRCDRL of MISR Register.
*/
class MisrCdrl : public TRegDataBase<TRegData32> {
private:
    union {
        TRegData32 m_data;
        RegData    m_bit;
    };

public:
    void Reset (void);
    const char* GetName (void) const { return "MisrCdrl"; }
    MisrCdrl (PhysAddr reg_addr, MisrCr *misrCr,
              MisrDcntCtl *misrDcntCtl, MisrDcnt *misrDcnt,
              Misr *misr);
    SysErrCause Write (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size);
    SysErrCause Read (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size);
    void SetData (RegData data) { m_bit = data; }
    RegData GetData (void) const { return m_bit; }

private:
    static const uint32_t MISRCDRL_MASK = 0xFFFFFFFF;
    MisrCr *m_misrCr;
    Misr   *m_misr;
    MisrDcntCtl *m_misrDcntCtl;
    MisrDcnt    *m_misrDcnt;
};


/*!
  @brief The class to be the member MISRCR of MISR Register.
*/
class MisrCr : public TRegDataBase<TRegData8> {
private:
    union {
        TRegData8 m_data;
        struct {
            uint8_t m_misr1en  :1;
            uint8_t m_misr2en  :1;
            uint8_t m_misr1cnd :1;
            uint8_t m_misr2cnd :1;
            uint8_t            :4;
        } m_bit;
    };

public:
    static const uint8_t MISRCR_MASK = 0x0F;

    void Reset (void);

    const char* GetName (void) const { return "MisrCr"; }

    void SetMisr1En  (bool en) { m_bit.m_misr1en  = en; }
    void SetMisr2En  (bool en) { m_bit.m_misr2en  = en; }
    void SetMisr1Cnd (bool en) { m_bit.m_misr1cnd = en; }
    void SetMisr2Cnd (bool en) { m_bit.m_misr2cnd = en; }

    bool GetMisr1En  (void) const { return m_bit.m_misr1en; }
    bool GetMisr2En  (void) const { return m_bit.m_misr2en; }
    bool GetMisr1Cnd (void) const { return m_bit.m_misr1cnd; }
    bool GetMisr2Cnd (void) const { return m_bit.m_misr2cnd; }

    explicit MisrCr (PhysAddr reg_addr);

private:
    uint32_t m_id;
    CoreType m_core_type;
    uint32_t m_peid;
};


/*!
  @brief The class of the member MISRBASEADR of MISR Register.
*/
class MisrBaseAdr : public TRegDataBase<TRegData32> {
private:
    union {
        TRegData32 m_data;
        RegData    m_bit;
    };

public:
    static const uint32_t MISRBASEADR_MASK = 0xFFFFFFFF;

    void Reset (void);

    const char* GetName (void) const { return "MisrBaseAdr"; }
    void SetData (RegData data) { m_bit = data; }
    RegData GetData (void) const { return m_bit; }

    explicit MisrBaseAdr (PhysAddr reg_addr);

private:
    uint32_t m_id;
    CoreType m_core_type;
    uint32_t m_peid;
};


/*!
  @brief The class of the member MISRADRMSK of MISR Register.
*/
class MisrAdrMsk : public TRegDataBase<TRegData32> {
private:
    union {
        TRegData32 m_data;
        RegData    m_bit;
    };

public:
    static const uint32_t MISRADRMSK_MASK = 0xFFFFFFFF;

    void Reset (void);

    const char* GetName (void) const { return "MisrAdrMsk"; }
    void SetData (RegData data) { m_bit = data; }
    RegData GetData (void) const { return m_bit; }

    explicit MisrAdrMsk (PhysAddr reg_addr);

private:
    uint32_t m_id;
    CoreType m_core_type;
    uint32_t m_peid;
};


/*!
  @brief The class of the member MISRDCNTCTL of MISR Register.
*/
class MisrDcntCtl : public TRegDataBase<TRegData8> {
private:
    union {
        TRegData8 m_data;
        struct {
            uint8_t m_cntsta : 1;
            uint8_t m_cnttrg : 1;
            uint8_t          : 6;
        } m_bit;
    };

public:
    static const uint32_t MISRDCNTCTL_MASK = 0x03;

    void Reset (void);

    const char* GetName (void) const { return "MisrDcntCtl"; }
    void SetCntSta  (bool en) { m_bit.m_cntsta  = en; }
    void SetCntTrg  (bool en) { m_bit.m_cnttrg  = en; }

    bool GetCntSta  (void) const { return m_bit.m_cntsta; }
    bool GetCntTrg  (void) const { return m_bit.m_cnttrg; }

    explicit MisrDcntCtl (PhysAddr reg_addr);

private:
    uint32_t m_id;
    CoreType m_core_type;
    uint32_t m_peid;
};


/*!
  @brief The class of the member MISRDCNT of MISR Register.
*/
class MisrDcnt : public TRegDataBase<TRegData16> {
private:
    union {
        TRegData16 m_data;
        RegData    m_bit;
    };

public:
    static const uint32_t MISRDCNT_MASK = 0xFFFFFFFF;

    void Reset (void);

    const char* GetName (void) const { return "MisrDcnt"; }
    void SetData (RegData data) { m_bit = data; }
    RegData GetData (void) const { return m_bit; }
    void CountUp () { m_bit = m_bit + 1; }

    explicit MisrDcnt (PhysAddr reg_addr);

private:
    uint32_t m_id;
    CoreType m_core_type;
    uint32_t m_peid;
};


/*!
  @brief The class to be the member MISR Register in MISR.
 */
class TRegMisr : public TRegModuleVc {
public:
    static const PhysAddr MISR1L_OFFSET = 0x00;
    static const PhysAddr MISR1H_OFFSET = 0x04;
    static const PhysAddr MISR2L_OFFSET = 0x08;
    static const PhysAddr MISR2H_OFFSET = 0x0C;
    static const PhysAddr MISRCDRL_OFFSET = 0x10;
    static const PhysAddr MISRCR_OFFSET = 0x18;
    static const PhysAddr MISRBASEADR_OFFSET = 0x1C;
    static const PhysAddr MISRADRMSK_OFFSET = 0x20;
    static const PhysAddr MISRDCNTCTL_OFFSET = 0x24;
    static const PhysAddr MISRDCNT_OFFSET = 0x28;

    explicit TRegMisr (PhysAddr reg_addr_base, uint32_t pe_id, Misr *misr);
    ~TRegMisr () { DelTRegMap (); }

    MisrL  *GetMisr1LPtr (void) { return m_misr1L_ptr; }
    MisrH  *GetMisr1HPtr (void) { return m_misr1H_ptr; }
    MisrL  *GetMisr2LPtr (void) { return m_misr2L_ptr; }
    MisrH  *GetMisr2HPtr (void) { return m_misr2H_ptr; }
    MisrCr *GetMisrCrPtr (void) { return m_misrCr_ptr; }
    MisrBaseAdr *GetMisrBaseAdrPtr (void) { return m_misrBaseAdr_ptr; }
    MisrAdrMsk  *GetMisrAdrMskPtr (void) { return m_misrAdrMsk_ptr; }
    MisrDcnt    *GetMisrDcnt (void) { return m_misrDcnt_ptr; }
    MisrDcntCtl *GetMisrDcntCtl (void) { return m_misrDcntCtl_ptr; }

private:
    PhysAddr m_reg_addr_base;
    Misr     *m_misr;

    MisrL*       m_misr1L_ptr;
    MisrH*       m_misr1H_ptr;
    MisrL*       m_misr2L_ptr;
    MisrH*       m_misr2H_ptr;
    MisrCdrl*    m_misrCdrl_ptr;
    MisrCr*      m_misrCr_ptr;
    MisrBaseAdr* m_misrBaseAdr_ptr;
    MisrAdrMsk*  m_misrAdrMsk_ptr;
    MisrDcntCtl* m_misrDcntCtl_ptr;
    MisrDcnt*    m_misrDcnt_ptr;

    // Not use of this function, because parameter is not match.
    void InitTRegMap (void) {}
};


/*!
  @brief MISR(Functional Safety related IP)
 */
class Misr : public CModuleVc {
private:
    PhysAddr  m_misr_base_addr;
    TRegMisr* m_trfs;   // MISR Register

    static const uint32_t MISR1_FEDBCK_BIT = 0x04c11db7UL;
    static const uint32_t MISR2_FEDBCK_BIT = 0x00400007UL;

public:
    static const PhysAddr MISR_REG_SIZE = 0x000001000ULL;
    PhysAddr GetMisrAddr (void) const { return m_misr_base_addr; }

    void CyclicHandler (void) {}
    SysErrCause TargetWrite (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size);
    SysErrCause TargetRead  (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size);
    SysErrCause TargetFetch (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size) {
        return TargetRead (sideband, addr, data, size);
    }

    bool IsMisrRegion (PhysAddr addr, ByteSize size);

    void CalcSignMonitor (PhysAddr addr, MemData8 *wdata, ByteSize size);
    void CalcSignatureMisr1 (uint32_t data, ByteSize size);
    void CalcSignatureMisr2 (uint32_t data, ByteSize size);

    uint32_t BitRevert32 (uint32_t data);

    SysErrCause TargetWriteDebug (PhysAddr addr, uint8_t* data, ByteSize size);
    SysErrCause TargetReadDebug  (PhysAddr addr, uint8_t* data, ByteSize size);
    void HardReset (void);

    Misr (PhysAddr base_addr, uint32_t pe_id);
    virtual ~Misr ();
};
