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
#include "./treg.h"
#include <functional>

class CBar;
class FsBus;
#if _UNIT_TEST
#include "mock_fsbus.h"
#endif
// class ProcElement;
// typedef std::vector<ProcElement*> PeVec;
#include "proc_element_vector.h"

// BAR type
enum BarImplType {
    BAR_STANDARD,
};

template <class TR>
class TRegBarBase : public TRegDataBase<TR> {
private:
    virtual bool IsRegWriteEnable (SideBand sideband) const = 0;
public:
    SysErrCause Write (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size);

    TRegBarBase (ByteSize reg_size, ByteSize min_access_size, PhysAddr reg_addr, TRegMask reg_mask)
        : TRegDataBase<TR> (reg_size, min_access_size, reg_addr, reg_mask) {}
    virtual ~TRegBarBase() {}
};


class TRegCNTBarBase : public TRegBarBase<TRegData8> {
protected:
    bool IsRegWriteEnable (SideBand sideband) const;
    void Reset (void) {
    }

public:
    TRegCNTBarBase (ByteSize reg_size, ByteSize min_access_size, PhysAddr reg_addr, TRegMask reg_mask)
        : TRegBarBase<TRegData8> (reg_size, min_access_size, reg_addr, reg_mask){}
};



class TRegBRINIT : public TRegCNTBarBase{
private:
    union {
        TRegData8 m_data;
        struct {
            RegData m_brinit:1;
            RegData         :7;
        } m_bit;
    };

    uint32_t m_channel;

public:
    static const uint32_t BRINIT_MASK = 0x00000001U;
    void Reset (void) {
            m_data.m_data8 = 0x00U;
            TRegCNTBarBase::Reset();
    }
    void WriteBodyByte (uint32_t pos, uint8_t data){}
    SysErrCause WriteBody (PhysAddr addr, uint8_t* data, ByteSize size);
    SysErrCause Write (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size);

    RegData GetData (void) const { return m_data.m_data8; }
    void SetData (RegData data) { m_data.m_data8 = data & 0x01U; }

    const char* GetName (void) const { return "BRINIT"; }

    //TRegBRINIT (uint32_t reg_size, uint32_t min_access_size, PhysAddr reg_addr, uint32_t channel, CModuleVc* tgtvc);
    TRegBRINIT (uint32_t reg_size, uint32_t min_access_size, PhysAddr reg_addr, uint32_t channel);
    ~TRegBRINIT (void){}

    std::function<void(uint32_t, uint32_t, RegData)> Set_BRCHK;
    std::function<RegData(uint32_t, uint32_t)> Get_BRCHK;
    std::function<void(uint32_t, uint32_t, RegData)> Set_BRSYN;
    std::function<RegData(uint32_t, uint32_t)> Get_BRSYN;
    std::function<void(uint32_t, RegData)> Set_BREN;
    std::function<RegData(uint32_t)> Get_BREN;
    std::function<void(uint32_t, RegData)> Set_BRINIT;
    std::function<RegData(uint32_t)> Get_BRINIT;
};

class TRegBREN : public TRegCNTBarBase {
private:
    union {
        TRegData8 m_data;
        struct {
            RegData m_peid0:1;
            RegData m_peid1:1;
            RegData m_peid2:1;
            RegData m_peid3:1;
            RegData m_peid4:1;
            RegData m_peid5:1;
            RegData m_peid6:1;
            RegData m_peid7:1;
        } m_bit;
    };

    uint32_t m_channel;
public:
    static const uint32_t BREN_MASK = 0x000000FFU;
    void Reset (void) {
            m_data.m_data8 = 0x00U;
            TRegCNTBarBase::Reset();
    }
    void WriteBodyByte (uint32_t pos, uint8_t data){}
    SysErrCause WriteBody (PhysAddr addr, uint8_t* data, ByteSize size);
    SysErrCause Write (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size);

    RegData GetData (void) const { return m_data.m_data8; }
    void SetData (RegData data) { m_data.m_data8 = data & 0xFFU; }
    const char* GetName (void) const { return "BREN"; }

    TRegBREN (uint32_t reg_size, uint32_t min_access_size, PhysAddr reg_addr,
                    uint32_t channel);
    ~TRegBREN (void){}

    std::function<void(uint32_t, uint32_t, RegData)> Set_BRCHK;
    std::function<RegData(uint32_t, uint32_t)> Get_BRCHK;
    std::function<void(uint32_t, uint32_t, RegData)> Set_BRSYN;
    std::function<RegData(uint32_t, uint32_t)> Get_BRSYN;
    std::function<void(uint32_t, RegData)> Set_BREN;
    std::function<RegData(uint32_t)> Get_BREN;
    std::function<void(uint32_t, RegData)> Set_BRINIT;
    std::function<RegData(uint32_t)> Get_BRINIT;
};

class TRegBRCHK : public TRegCNTBarBase {
private:
    union {
        TRegData8 m_data;
        struct {
            RegData m_brchk:1;
            RegData        :7;
        } m_bit;
    };

    uint32_t m_channel;
    uint32_t m_bar_id;
#if _UNIT_TEST
    MockFsBus* m_fsbus;
#else
    FsBus* m_fsbus;
#endif

public:
    static const uint32_t BRCHK_MASK = 0x00000001U;
    void Reset (void) {
            m_data.m_data8 = 0x00U;
            TRegCNTBarBase::Reset();
    }
    void WriteBodyByte (uint32_t pos, uint8_t data){}
    SysErrCause WriteBody (PhysAddr addr, uint8_t* data, ByteSize size);
    SysErrCause Write (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size);

    RegData GetData (void) const { return m_data.m_data8; }
    void SetData (RegData data) { m_data.m_data8 = data & 0x01U; }

    const char* GetName (void) const { return "BRCHK"; }

#if _UNIT_TEST
    TRegBRCHK (uint32_t reg_size, uint32_t min_access_size, PhysAddr reg_addr,
                uint32_t channel, uint32_t bar_id, MockFsBus* fsbus);
#else
    TRegBRCHK (uint32_t reg_size, uint32_t min_access_size, PhysAddr reg_addr,
                uint32_t channel, uint32_t bar_id, FsBus* fsbus);
#endif
    ~TRegBRCHK (void){}

    std::function<void(uint32_t, uint32_t, RegData)> Set_BRCHK;
    std::function<RegData(uint32_t, uint32_t)> Get_BRCHK;
    std::function<void(uint32_t, uint32_t, RegData)> Set_BRSYN;
    std::function<RegData(uint32_t, uint32_t)> Get_BRSYN;
    std::function<void(uint32_t, RegData)> Set_BREN;
    std::function<RegData(uint32_t)> Get_BREN;
    std::function<void(uint32_t, RegData)> Set_BRINIT;
    std::function<RegData(uint32_t)> Get_BRINIT;
};


class TRegBRSYN : public TRegCNTBarBase {
private:
    union {
        TRegData8 m_data;
        struct {
            RegData m_brsyn:1;
            RegData        :7;
        } m_bit;
    };

    uint32_t m_channel;
    uint32_t m_bar_id;
#if _UNIT_TEST
    MockFsBus* m_fsbus;
#else
    FsBus* m_fsbus;
#endif

public:
    static const uint32_t BRSYN_MASK = 0x00000001U;
    void Reset (void) {
            m_data.m_data8 = 0x00U;
            TRegCNTBarBase::Reset();
    }
    void WriteBodyByte (uint32_t pos, uint8_t data){}
    SysErrCause WriteBody (PhysAddr addr, uint8_t* data, ByteSize size);
    SysErrCause Write (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size);

    RegData GetData (void) const { return m_data.m_data8; }
    void SetData (RegData data) { m_data.m_data8 = data & 0x01U; }

    const char* GetName (void) const { return "BRSYN"; }

#if _UNIT_TEST
    TRegBRSYN (uint32_t reg_size, uint32_t min_access_size, PhysAddr reg_addr,
                uint32_t channel,uint32_t bar_id, MockFsBus* fsbus);
#else
    TRegBRSYN (uint32_t reg_size, uint32_t min_access_size, PhysAddr reg_addr,
                uint32_t channel,uint32_t bar_id, FsBus* fsbus);
#endif
    ~TRegBRSYN (void){}

    std::function<void(uint32_t, uint32_t, RegData)> Set_BRCHK;
    std::function<RegData(uint32_t, uint32_t)> Get_BRCHK;
    std::function<void(uint32_t, uint32_t, RegData)> Set_BRSYN;
    std::function<RegData(uint32_t, uint32_t)> Get_BRSYN;
    std::function<void(uint32_t, RegData)> Set_BREN;
    std::function<RegData(uint32_t)> Get_BREN;
    std::function<void(uint32_t, RegData)> Set_BRINIT;
    std::function<RegData(uint32_t)> Get_BRINIT;

};


class TRegBar : public TRegModuleVc {
public:
#if _UNIT_TEST
    explicit TRegBar (CBar* bar, PeVec* pes,  MockFsBus* fsbus) : m_bar (bar), m_pes (pes), m_fsbus(fsbus) { InitTRegMap (); }
#else
    explicit TRegBar (CBar* bar, PeVec* pes,  FsBus* fsbus) : m_bar (bar), m_pes (pes), m_fsbus(fsbus) { InitTRegMap (); }
#endif
    ~TRegBar () { DelTRegMap (); }

    static const uint32_t N_G4MH_CH        = 16;
    static const uint32_t N_G4MH_BAR_PE    = 8;
    static const uint32_t BAR_FUNC_DISABLE = 0x0;
    static const uint32_t BAR_CHK_CLR      = 0x0;
    static const uint32_t BAR_CHK_SET      = 0x1;
    static const uint32_t BRINIT_OFFSET    = 0x0;
    static const uint32_t BREN_OFFSET      = 0x4;
    static const uint32_t BRCHK_OFFSET     = 0x800;
    static const uint32_t BRSYN_OFFSET     = 0x804;

    TRegBRINIT*  TrBRINIT  (PhysAddr addr) const { return static_cast<TRegBRINIT*>  (GetTrPtr (addr)); }
    TRegBREN* TrBREN (PhysAddr addr) const { return static_cast<TRegBREN*>  (GetTrPtr (addr)); }
    TRegBRCHK* TrBRCHK (PhysAddr addr)const { return static_cast<TRegBRCHK*>  (GetTrPtr (addr)); }
    TRegBRSYN* TrBRSYN (PhysAddr addr) const { return static_cast<TRegBRSYN*>  (GetTrPtr (addr)); }

    PhysAddr TrBRINITAddr  (uint32_t channel) const;
    PhysAddr TrBRENAddr (uint32_t channel) const;
    PhysAddr TrBRCHKAddr(uint32_t channel, uint32_t bar_id) const;
    PhysAddr TrBRSYNAddr (uint32_t channel, uint32_t bar_id) const;

private:
    CBar* m_bar;
    PeVec* m_pes;
    uint32_t m_peid;
#if _UNIT_TEST
    MockFsBus* m_fsbus;
#else
    FsBus* m_fsbus;
#endif

    void InitTRegMap (void);
};

class CBar : public CModuleVc {
private:
    BarImplType m_impl_type;
    TRegBar* m_trf;
    PeVec* m_pes;
#if _UNIT_TEST
    MockFsBus* m_fsbus;
#else
    FsBus* m_fsbus;
#endif
#if _UNIT_TEST
    FRIEND_TEST (ConvertSelfAddr, ConvertSelfAddr_Regs);
#endif
    PhysAddr ConvertSelfAddr(uint32_t peid, PhysAddr addr);
public:

    uint32_t m_channel;
    static const PhysAddr BAR_REG_SIZE = 0x00001000;
    static const PhysAddr BAR_REG_ADDR_MASK = BAR_REG_SIZE - 1;
    static const uint32_t INIT_CHANNEL = 17;
    static const uint32_t BAR_S_OFFSET_START = 0x100;
    static const uint32_t BAR_S_OFFSET_END = 0x800;
    static const uint32_t BAR_PE_OFFSET = 0x100;

    BarImplType GetImplType (void) const { return m_impl_type; }
    TRegModuleVc* GetTRegFilePtr (void) { return m_trf; }
    void CyclicHandler (void) {};

    SysErrCause TargetWrite (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size);
    SysErrCause TargetRead  (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size);
    SysErrCause TargetFetch (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size) {
        return TargetRead (sideband, addr, data, size);
    }
    SysErrCause TargetWriteDebug (PhysAddr addr, uint8_t* data, ByteSize size);
    SysErrCause TargetReadDebug  (PhysAddr addr, uint8_t* data, ByteSize size);
    void HardReset (void);

    TRegBRCHK* GetBRCHKPtr (uint32_t channel, uint32_t bar_id) const;
    TRegBRSYN* GetBRSYNPtr (uint32_t channel, uint32_t bar_id) const;
    TRegBREN* GetBRENPtr (uint32_t channel) const;
    TRegBRINIT* GetBRINITPtr (uint32_t channel) const;

    void Set_BRCHK (uint32_t channel, uint32_t bar_id, RegData data);
    RegData Get_BRCHK (uint32_t channel, uint32_t bar_id);

    void Set_BRSYN(uint32_t channel, uint32_t bar_id, RegData data);
    RegData Get_BRSYN (uint32_t channel, uint32_t bar_id);

    void Set_BREN(uint32_t channel, RegData data);
    RegData Get_BREN (uint32_t channel);

    void Set_BRINIT(uint32_t channel, RegData data);
    RegData Get_BRINIT (uint32_t channel);

    bool IsValidPeId (uint32_t peid) const;
#if _UNIT_TEST
    CBar (PeVec* pes, MockFsBus* fsbus, BarImplType type);
#else
    CBar (PeVec* pes, FsBus* fsbus, BarImplType type);
#endif
    virtual ~CBar (void) { delete m_trf; }
};
