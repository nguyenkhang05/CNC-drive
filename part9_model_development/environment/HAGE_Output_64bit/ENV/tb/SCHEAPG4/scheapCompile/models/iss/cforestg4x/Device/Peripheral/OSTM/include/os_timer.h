/*
 * (c) 2011,2012 Renesas Electronics Corporation
 * RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY
 * This program must be used solely for the purpose for which
 * it was furnished by Renesas Electronics Corporation. No part of this
 * program may be reproduced or disclosed to others, in any
 * form, without the prior written permission of Renesas Electronics
 * Corporation.
 */
/**
 * @file os_timer.h
 * @brief Header of OS Timer
 */

#pragma  once

#ifdef   OSTM_DEBUG
#include <cstdio>
#include <iomanip>
#include <sstream>
#endif

#include <string>
#include <exception>
#include <utility>
#include "./forest_message.h"
#include "./treg.h"
#include "./module_vc.h"
#include "./module_run.h"
#if _UNIT_TEST
#include "./mock_intc1.h"
#else
#include "./intc1.h"
#endif

class FsBus;
class OstmCore;
class OstmReg8;     // Base class of 8bit OSTM register.
class OstmReg32;    // Base class of 32bit OSTM register.
class TRegOSTMnCMP;  // +00 OSTM compare register
class TRegOSTMnCNT;  // +04 OSTM counter register
class TRegOSTMnTO;   // +08 OSTM output register
class TRegOSTMnTOE;  // +0C OSTM output enable register
class TRegOSTMnTE;   // +10 OSTM count enable status register
class TRegOSTMnTS;   // +14 OSTM count start trigger register
class TRegOSTMnTT;   // +18 OSTM count stop trigger register
class TRegOSTMnCTL;  // +20 OSTM control register
class TRegOSTMnEMU;  // +24 OSTM emulation register


/*!
  @brief Interface to control I/O for memory map in OstmIO.
*/
class OstmIO : public CModuleVc
{
public:
#if _UNIT_TEST
    OstmIO(uint32_t peid, MockCIntc1* intc, MockCIntc2* intc2,
            PhysAddr base, uint32_t intch, uint32_t ostmch, MockFsBus* fsbus, uint32_t ratio);
#else
    OstmIO(uint32_t peid, CIntc1* intc, CIntc2* intc2,
            PhysAddr base, uint32_t intch, uint32_t ostmch, FsBus* fsbus, uint32_t ratio);
#endif
    virtual ~OstmIO();

    // CModuleVc interface methods
    SysErrCause TargetWrite (SideBand, PhysAddr, uint8_t*, ByteSize);
    SysErrCause TargetRead (SideBand, PhysAddr, uint8_t*, ByteSize);
    SysErrCause TargetFetch (SideBand, PhysAddr, uint8_t*, ByteSize);
    SysErrCause TargetWriteDebug (PhysAddr addr, uint8_t* data, ByteSize);
    SysErrCause TargetReadDebug (PhysAddr, uint8_t*, ByteSize);
    void HardReset (void);
    void CyclicHandler (void);
    SysErrCause IsAccPermited (SideBand, PhysAddr, ByteSize, MemAccessCommand);
    OstmCore* GetOstmCore(void) {return m_ostm_core;}
#if (_UNIT_TEST)
    void SetIntcPtr(MockCIntc1* intc1, MockCIntc2* intc2);
#else
    void SetIntcPtr(CIntc1* intc1, CIntc2* intc2);
#endif
    void ClockWait (uint64_t wait_cycle) { m_wait_count = wait_cycle * m_ratio; }
    void SetClkRatio (uint32_t ratio);

#ifdef   OSTM_DEBUG
    void Debug(void) const;
#endif

public:
    PhysAddr       m_base_addr;        //!< @brief Base address of memory mapped area
    static const uint32_t m_map_size;  //!< @brief Size of memory-mapped area

    uint32_t m_ratio;
    uint64_t m_wait_count;

protected:
    OstmCore* const m_ostm_core;     //!< @brief  OSTM timer function core
};



/*!
  @brief  OSTM running interface

  @note   OSTM's timer function is drived by CyclicHandler
*/
class OstmClock : public ModuleRun
{
public:
    OstmClock(uint32_t ratio) : m_time(0), m_ratio(ratio), m_fsbus(NULL), m_ostmio(NULL) {}
    virtual ~OstmClock(){}

    // Implement ModuleRun
    IsElapsedTime RunModule (void);
    void ClockWait (uint64_t wait_cycle) { m_time += (m_ratio * wait_cycle);}
    void PreConsumeClock (void) {}
    void AddHaltCycle (uint64_t halt_cycle) {}
    void UpdateSnoozeState (void) {}
    bool IsBreakHitPreExec (void) const {return false;}
    bool IsBreakHitPostExec (void) const {return false;}
    bool IsHtRunable (void) const {return false;}
    bool IsSnooze (void) const {return false;}
    bool IsHtActive (void) const {return false;}
    bool IsIntRequested (void) const {return false;}
    bool IsInHalt (void) const {return false;}
    bool IsCpu (void) const {return false;}
    FILE* GetFilePointer (void) const {return NULL; }
    int32_t GetScId (void) const {return OSTM_SCID;}
    uint64_t GetCpuTime (void) const  { return m_time;}
    uint64_t GetBusTime (void) const  { return m_time;}
    uint32_t GetCpuRatio (void) const {return m_ratio;}
    void SetCpuRatio (uint32_t ratio) { m_ratio = ratio; }
    OstmIO* GetOstmIO (void) {return m_ostmio;}

    #ifdef SUPPORT_SC_HEAP
    bool IsSuspendState (void) const { return false; }
    #endif // SUPPORT_SC_HEAP

protected:
    virtual void Drive(){}
    virtual uint64_t GetProcessCycle() const {return 1;}
    virtual bool IsRunning() const { return false;}
    void ClockRequest(bool req);
    uint64_t GetSystemTime(void) const;
    uint64_t m_time;
    uint32_t m_ratio;
    bool m_last_state;
#if (_UNIT_TEST)
    MockFsBus* m_fsbus;
#else
    FsBus* m_fsbus;
#endif
    //!@brief  OstmIO
    OstmIO* m_ostmio;
    static const int32_t  OSTM_SCID;
};



/*!
  @brief  OSTM functions core

  @note   OSTM's timer function is drived by CyclicHandler
*/
class OstmCore
    : public TRegModuleVc,      // Supports Register access
      public OstmClock         // Run by ModuleRun Clock
{
public:
#if (_UNIT_TEST)
    OstmCore(uint32_t peid, MockCIntc1* intc1, MockCIntc2* intc2,
              uint32_t intch, uint32_t ostmch, MockFsBus* fsbus, OstmIO* ostmio, uint32_t ratio);
#else
    OstmCore(uint32_t peid, CIntc1* intc1, CIntc2* intc2,
              uint32_t intch, uint32_t ostmch, FsBus* fsbus, OstmIO* ostmio, uint32_t ratio);
#endif
    virtual ~OstmCore();

    void   InitTRegMap(void);
    void   PowerOnReset(void);
    void   ManualReset(void) { PowerOnReset(); }
    void   Drive(void);
    /**
     * @brief Returns true when the timer is running
     */
    bool   IsRunning (void) const { return m_status == OSTM_RUN; }

    void   SetIntChannel(uint32_t ch) { m_irq.SetChannel(ch); }
#if (_UNIT_TEST)
    void   SetIntcPtr(MockCIntc1* intc1, MockCIntc2* intc2) { m_irq.SetIntcPtr(intc1, intc2); }
#else
    void   SetIntcPtr(CIntc1* intc1, CIntc2* intc2) { m_irq.SetIntcPtr(intc1, intc2); }
#endif
    uint64_t GetProcessCycle() const {return m_cycle;}
    uint64_t GetSystemTime(void) { return OstmClock::GetSystemTime(); }
    bool   IsIntervalMode() const { return m_mode == OSTM_INTERVAL; }
    bool   IsCompareMode() const { return m_mode == OSTM_COMPARE; }
    void   SetRUN() { m_status = OSTM_RUN; }
    void   SetIDLE() { m_status = OSTM_IDLE; }
    uint32_t  GetStatus() { return m_status; }

    /**
     * @brief Returns PEID of binded PE (for PE binded OSTM)
     */
    uint32_t GetPeId(void) const;

    /**
     * @brief Gets channel number of OSTM
     */
    uint32_t GetOstmCh(void) const { return m_ostmch; }

    /**
     * @brief Reschedules event when running on compare mode
     */
    void CheckOSTMnCMP(void);

    /**
     * @brief Gets OSTMnTOE
     */
    uint32_t GetOSTMnTOE(void) const;

    /**
     * @brief Gets OSTMnTE
     */
    uint32_t GetOSTMnTE(void) const;

    /**
     * @brief Sets SetOSTMnTE
     */
    void SetOSTMnTE(uint32_t val);

    /**
     * @brief Sets OSTMnCNT
     */
    void SetOSTMnCNT(uint32_t val);

    /**
     * @brief Gets OSTMnCNT
     */
    uint32_t GetOSTMnCNT(void) const;

    /**
     * @brief Gets OSTMnCMP
     */
    uint32_t GetOSTMnCMP(void) const;

    /**
     * @brief Sets OSTMnCMP
     */
    void SetOSTMnCMP(uint32_t val);

    /**
     * @brief Gets OSTMnCTL
     */
    uint32_t GetOSTMnCTL(void) const;

    /**
     * @brief Gets OSTMnCTLMD2
     */
    bool GetOSTMnCTLMD2(void) const;

    /**
     * @brief Gets OSTMnCTLMD1
     */
    bool GetOSTMnCTLMD1(void) const;

    /**
     * @brief Sets OSTMnCTL
     */
    void SetOSTMnCTL(uint32_t val);

    /**
     * @brief Updates OSTMnCNT when timer is running
     */
    void UpdateOSTMnCNT(void);

    /**
     * @brief Starts timer
     */
    void Start(void);

    /**
     * @brief Stops timer
     */
    void Stop(void);

    /**
     * @brief Restore CNT to compare value
     */
    void RestoreCNTtoCMP(void);

    /**
     * @brief Set m_md2_changed when MD2 from 0 to 1
     */
    void SetEventMD2Updated(void);

private:
    /**
     * @brief Save OSTMnMD1
     */
    void SaveMode(void);

    /**
     * @brief Save OSTMnMD0
     */
    void SaveStartIntMode(void);

    /**
     * @brief Save OSTMnIE
     */
    void SaveIntEnabledMode(void);


    /**
     * @brief Save OSTMnCMP
     */
    void SaveCmpVal(void);

    /**
     * @brief Save OSTMnCNT
     */
    void SaveCntVal(void);




#ifdef OSTM_DEBUG
    void   PrintRegister(std::FILE* os) const;
    void   PrintRegister(std::FILE* os, bool) const;
    void   PrintInternal(std::FILE* os) const;
#endif

    enum OstmEvent {
        OSTM_EVENT_NA,     //!< @brief Not Assert
        OSTM_EVENT_ST,     //!< @brief TimerStart
        OSTM_EVENT_CM,     //!< @brief CompareMatch
        OSTM_EVENT_UF      //!< @brief UnderFlow
    };
    void   SetCyclicHandler(OstmEvent ev);

private:
    typedef std::pair<const PhysAddr, TRegBase*> TRegPair;

    enum OstmStatus {
        OSTM_IDLE,        //!< @brief  ostm is on idle
        OSTM_RUN          //!< @brief  ostm is on running
    };

    enum OstmMode {
        OSTM_INTERVAL,    //!< @brief  ostm is interval timer mode
        OSTM_COMPARE      //!< @brief  ostm is free running compare mode
    };

    enum OstmException {
        OSTM_EXP_NA,      //!< @brief Not Assert
        OSTM_EXP_ST,      //!< @brief Timer Start
        OSTM_EXP_UF,      //!< @brief Under Run
        OSTM_EXP_CM       //!< @brief Compare
    };


private:
    //!@brief  PEID (for PE binded OSTM)
    const uint32_t m_peid;
    //!@brief  Status is on idle or on running (OSTM_IDLE/OSTM_RUN)
    OstmStatus         m_status;
    //!@brief  OSTM is in interval timer mode or free running compare mode (OSTM_INTERVAL/OSTM_COMPARE)
    OstmMode           m_mode;
    //!@brief  remaining time for next event
    uint64_t            m_cycle;
    //!@brief  Start value of count down
    uint32_t            m_compval;
    //!@brief  MD2 change from 0 to 1 effect to next start count
    bool                m_md2_changed;
    //!@brief  Start value of count down when MD2 change from 0 to 1
    uint32_t            m_comp_cnt_val;
    //!@brief  Value of OSTMnMD0 when timer is started/restarted
    bool                m_start_int;
    //!@brief  Value of OSTMnIE when timer is started/restarted
    bool                m_int_enabled;
    //!@brief  Next event type when CyclicHandler is called next time
    OstmEvent          m_event;
    //!@brief  System cycle on started (updated every event)
    uint64_t            m_start_cycle;
    //!@brief  The channel of OSTM
    uint32_t            m_ostmch;
    /*!
      @brief  OSTM registers
     */
    struct {
        TRegOSTMnCMP*   OSTMnCMP;
        TRegOSTMnCNT*   OSTMnCNT;
        TRegOSTMnTO*    OSTMnTO;
        TRegOSTMnTOE*   OSTMnTOE;
        TRegOSTMnTE*    OSTMnTE;
        TRegOSTMnTS*    OSTMnTS;
        TRegOSTMnTT*    OSTMnTT;
        TRegOSTMnCTL*   OSTMnCTL;
        TRegOSTMnEMU*   OSTMnEMU;
    };

    /*!
      @brief  Process interrupt request
     */
    class IntReq {
    public:
#if (_UNIT_TEST)
        IntReq(OstmCore* p, MockCIntc1* intc, uint32_t intch, MockCIntc2* intc2)
              : m_owner(p), m_intc(intc), m_channel(intch), m_intc2(intc2) {}
#else
        IntReq(OstmCore* p, CIntc1* intc, uint32_t intch, CIntc2* intc2)
              : m_owner(p), m_intc(intc), m_channel(intch), m_intc2(intc2) {}
#endif

        virtual ~IntReq() {}
        void SetChannel (uint32_t ch) { m_channel = ch; }
        bool RequestInterrupt(void);
#if (_UNIT_TEST)
        void SetIntcPtr (MockCIntc1* intc1, MockCIntc2* intc2)
        {
            m_intc = intc1;
            m_intc2 = intc2;
        }
#else
        void SetIntcPtr (CIntc1* intc1, CIntc2* intc2)
        {
            m_intc = intc1;
            m_intc2 = intc2;
        }
#endif
    private:
        OstmCore*  const   m_owner;    //!< @brief Parent instance
#if (_UNIT_TEST)
        MockCIntc1*         m_intc;
#else
        CIntc1*             m_intc;     //!< @brief INTC1
#endif
        uint32_t            m_channel;  //!< @brief channel
#if (_UNIT_TEST)
        MockCIntc2*         m_intc2;
#else
        CIntc2*             m_intc2;    //!< @brief INTC2
#endif
    } m_irq;

public:
    bool GetMD0Status (void) {return m_start_int;}
    OstmMode GetMD1Status (void) {return m_mode;}
    bool GetIntEnabledStatus (void) {return m_int_enabled;}

};



/*!
  @brief Base class for representation of Register 8 bit in OSTM
 */
class OstmReg8 : public TRegDataBase<TRegData8>
{
public:
    /**
     * @brief generation of objects
     * @param addr register address
     * @param msk  mask value. When accessing it, the logical add is taken by this value.
     */
    OstmReg8 (PhysAddr addr, TRegMask msk)
        : TRegDataBase<TRegData8>(1, 1, addr, msk)
    {
        m_treg = new TRegData8();
    }

    /**
     * @brief  object destruction
     */
    virtual ~OstmReg8 () {
        delete m_treg, m_treg = NULL;
    }

    /**
     * @brief Reset
     * @note  Override when register has initial value
     */
    void Reset() {}

    /**
     * @brief  Gets register name
     * @return char string of register name
     */
    const char* GetName (void) const {
        return ""; // NULL;
    }

    /**
     * @brief  Gets value of register
     * @return The value of register
     */
    virtual uint8_t GetVal() const {
        return m_treg->m_data8;
    }

    /**
     * @brief Set value to register
     * @param val value is set
     */
    virtual void SetVal(uint8_t val) {
        m_treg->m_data8 = val;
    }

    /**
     * @brief  Add value to register value
     * @param v value is added
     */
    template <typename TAdd>
    void AddVal(TAdd v) {
        m_treg->m_data8 += v;
    }

    /**
     * @brief  Subtract value to register value
     * @param v value is subtracted
     */
    template <typename TSub>
    void SubVal(TSub v) {
        m_treg->m_data8 -= v;
    }
};



/*!
  @brief Base class for 32bit OSTM register
 */
class OstmReg32 : public TRegDataBase<TRegData32>
{
public:
    /**
     * @brief Constructor
     * @param addr  address of register
     * @param msk   mask value. OR-mask for write value.
     */
    OstmReg32 (PhysAddr addr, TRegMask msk)
        : TRegDataBase<TRegData32>(4, 4, addr, msk)
    {
        m_treg = new TRegData32();
    }

    /**
     * @brief  Destructor
     */
    virtual ~OstmReg32 () {
        delete m_treg, m_treg = NULL;
    }

    /**
     * @brief Reset
     * @note  Override when register has initial value
     */
    void Reset() {}

    /**
     * @brief  Gets register name
     * @return char string of register name
     */
    const char* GetName (void) const {
        return ""; // NULL;
    }

    /**
     * @brief  Gets value of register
     * @return The value of register
     */
    virtual uint32_t GetVal() const {
        return m_treg->m_data32;
    }

    /**
     * @brief Set value to register
     * @param val value is set
     */
    virtual void SetVal(uint32_t val) {
        m_treg->m_data32 = val;
    }

    /**
     * @brief  Add value to register value
     * @param v value is added
     */
    template <typename TAdd>
    void AddVal(TAdd v) {
        m_treg->m_data32 += v;
    }

    /**
     * @brief  Subtract value to register value
     * @param v value is subtracted
     */
    template <typename TSub>
    void SubVal(TSub v) {
        m_treg->m_data32 -= v;
    }
};


/*!
  @brief OSTMOCMP implementation
 */
class TRegOSTMnCMP : public OstmReg32
{
public:
    explicit TRegOSTMnCMP(OstmCore* core)
        : OstmReg32(m_addr, TRegMask(0xffffffff)), m_core(core) {}
    virtual ~TRegOSTMnCMP(){}
    virtual const char* GetName(void) const { return m_name; }
    void Reset() { m_treg->m_data32 = 0x0; }
    SysErrCause Write (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size);

    static const char* const m_name;    //!< @brief register name
    static const PhysAddr    m_addr;    //!< @brief offset from base
    OstmCore*                m_core;    //!< @brief pointer to OSTM core
};


/*!
  @brief  OSTMnCNT implementation
 */
class TRegOSTMnCNT : public OstmReg32
{
public:
    explicit TRegOSTMnCNT(OstmCore* core)
        : OstmReg32(m_addr, TRegMask(0xFFFFFFFF)), m_core(core) {}
    virtual ~TRegOSTMnCNT(){}
    virtual const char* GetName(void) const { return m_name; }
    void Reset() { m_treg->m_data32 = 0xFFFFFFFF; }
    SysErrCause Write (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size);
    SysErrCause Read (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size) const;

    static const char* const m_name;    //!< @brief register name
    static const PhysAddr    m_addr;    //!< @brief offset from base
    OstmCore*                m_core;    //!< @brief pointer to OSTM core
};


/*!
  @brief  OSTMnTO implementation
 */
class TRegOSTMnTO : public OstmReg8
{
public:
    explicit TRegOSTMnTO(OstmCore*  core)
        : OstmReg8(m_addr, TRegMask(0x01)), m_core(core) {}
    virtual ~TRegOSTMnTO(){}
    virtual const char* GetName(void) const { return m_name; }
    void Reset() { m_treg->m_data8 = 0x0; }
    SysErrCause Write (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size);

    static const char* const m_name;    //!< @brief register name
    static const PhysAddr    m_addr;    //!< @brief offset from base
    OstmCore*                m_core;    //!< @brief pointer to OSTM core
};


/*!
  @brief  OSTMnTOE implementation
 */
class TRegOSTMnTOE : public OstmReg8
{
public:
    explicit TRegOSTMnTOE(OstmCore* core)
        : OstmReg8(m_addr, TRegMask(0x01)), m_core(core) {}
    virtual ~TRegOSTMnTOE(){}
    virtual const char* GetName(void) const { return m_name; }
    void Reset() { m_treg->m_data8 = 0x0; }

    static const char* const m_name;    //!< @brief register name
    static const PhysAddr    m_addr;    //!< @brief offset from base
    OstmCore*                m_core;    //!< @brief pointer to OSTM core
};


/*!
  @brief  OSTMnTE implementation
 */
class TRegOSTMnTE : public OstmReg8
{
public:
    explicit TRegOSTMnTE(OstmCore* core)
        : OstmReg8(m_addr, TRegMask(0x00)), m_core(core) {}
    virtual ~TRegOSTMnTE(){}
    virtual const char* GetName(void) const { return m_name; }
    void Reset() { m_treg->m_data8 = 0x0; }
    SysErrCause Read (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size) const;
    SysErrCause Write (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size) { return SYSERR_NONE; }
    static const char* const m_name;    //!< @brief register name
    static const PhysAddr    m_addr;    //!< @brief offset from base
    OstmCore*                m_core;    //!< @brief pointer to OSTM core
};


/*!
  @brief  OSTMnTS implementation
 */
class TRegOSTMnTS : public OstmReg8
{
public:
    explicit TRegOSTMnTS(OstmCore* core)
        : OstmReg8(m_addr, TRegMask(0x01)), m_core(core)  {}
    virtual ~TRegOSTMnTS(){}
    virtual const char* GetName(void) const { return m_name; }
    void Reset() { m_treg->m_data8 = 0x0; }
    SysErrCause Write (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size);
    SysErrCause Read (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size) const;

    static const char* const m_name;    //!< @brief register name
    static const PhysAddr    m_addr;    //!< @brief offset from base
    OstmCore*                m_core;    //!< @brief pointer to OSTM core
};


/*!
  @brief  OSTMnTT implementation
 */
class TRegOSTMnTT : public OstmReg8
{
public:
    explicit TRegOSTMnTT(OstmCore* core)
        : OstmReg8(m_addr, TRegMask(0x01)), m_core(core) {}
    virtual ~TRegOSTMnTT(){}
    virtual const char* GetName(void) const { return m_name; }
    void Reset() { m_treg->m_data8 = 0x0; }
    SysErrCause Write (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size);
    SysErrCause Read (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size) const;

    static const char* const m_name;    //!< @brief register name
    static const PhysAddr    m_addr;    //!< @brief offset from base
    OstmCore*                m_core;    //!< @brief pointer to OSTM core
};


/*!
  @brief  OSTMnCTL implementation
 */
class TRegOSTMnCTL : public OstmReg8
{
public:
    explicit TRegOSTMnCTL(OstmCore* core)
        : OstmReg8(m_addr, TRegMask(0x87)), m_core(core) {}
    virtual ~TRegOSTMnCTL(){}
    virtual const char* GetName(void) const { return m_name; }
    void Reset() { m_treg->m_data8 = 0x0; }
    bool GetOSTMnIE()  const { return ((m_treg->m_data8 & 0x80) != 0); }
    bool GetOSTMnMD2() const { return ((m_treg->m_data8 & 0x04) != 0); }
    bool GetOSTMnMD1() const { return ((m_treg->m_data8 & 0x02) != 0); }
    bool GetOSTMnMD0() const { return ((m_treg->m_data8 & 0x01) != 0); }
    SysErrCause Write (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size);

    static const char* const m_name;    //!< @brief register name
    static const PhysAddr    m_addr;    //!< @brief offset from base
    OstmCore*                m_core;    //!< @brief pointer to OSTM core
};


/*!
  @brief  Emulation register of OSTM
 */
class TRegOSTMnEMU : public OstmReg8
{
public:
    explicit TRegOSTMnEMU(OstmCore* core)
        : OstmReg8(m_addr, TRegMask(0x80)), m_core(core) {}
    virtual ~TRegOSTMnEMU(){}
    virtual const char* GetName(void) const { return m_name; }
    void Reset() { m_treg->m_data8 = 0x0; }
    bool GetOSTMnSVSDIS() const { return ((m_treg->m_data8 & 0x80) != 0); }

    static const char* const m_name;    //!< @brief register name
    static const PhysAddr    m_addr;    //!< @brief offset from base
    OstmCore*                m_core;    //!< @brief pointer to OSTM core
};
