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
 * @file os_timer.cpp
 * @brief OS Timer implementation
 * @note Support E2xFCC2 OS Timer. The spec is r01uh0770ej0010-e2x-fcc2(E2UH_E2H).pdf.
 */

#include <algorithm>
#include <iomanip>
#include "./config.h"
#include "./os_timer.h"
#include "./fsbus.h"
#if _UNIT_TEST
#include "./mock_proc_element.h"
#else
#include "./proc_element.h"
#endif
#include "./memory_body.h"
#if _UNIT_TEST
//class MockFsBus;
#else
#include "./intc1.h"
#endif
#include "./forest_internals.h"
#include "./forest_utility.h"


//#define USE_OSTM_RUNMODULE

//////////////////////////////////////////////////////////////////////////////
//
//  OstmIO - OSTimer Memory-mapped I/O
//
//////////////////////////////////////////////////////////////////////////////
#if _UNIT_TEST
OstmIO::OstmIO(uint32_t peid, MockCIntc1* intc, MockCIntc2* intc2,
                 PhysAddr baseAddr, uint32_t intch, uint32_t ostmch, MockFsBus* fsbus, uint32_t ratio)
: m_base_addr(baseAddr), m_ratio (ratio), m_wait_count (0),
  m_ostm_core(new OstmCore(peid, intc, intc2, intch, ostmch, fsbus, this, ratio))
{
    m_ostm_core->InitTRegMap(); // setup register
}
#else
OstmIO::OstmIO(uint32_t peid, CIntc1* intc, CIntc2* intc2,
                 PhysAddr baseAddr, uint32_t intch, uint32_t ostmch, FsBus* fsbus, uint32_t ratio)
: m_base_addr(baseAddr), m_ratio (ratio), m_wait_count (0),
  m_ostm_core(new OstmCore(peid, intc, intc2, intch, ostmch, fsbus, this, ratio))
{
    m_ostm_core->InitTRegMap(); // setup register
}
#endif

OstmIO::~OstmIO()
{
    delete m_ostm_core;
}


void OstmIO::SetClkRatio (uint32_t ratio)
{
    m_ratio = ratio;
    m_ostm_core->SetCpuRatio (ratio);
}


SysErrCause OstmIO::TargetWrite (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size)
{
    TRegBase* reg = m_ostm_core->GetTrPtr(addr - m_base_addr);
    reg->Write(sideband, addr - m_base_addr, data, size);
    uint32_t val = 0;
    memcpy (&val, data, size);
    CfMsg::TimerPrint("[OST@PE%d.CH%d.%-8s:Write_data=0x%08x]\n",
                      m_ostm_core->GetPeId(), m_ostm_core->GetOstmCh(), reg->GetName(), val);
    return SYSERR_NONE;
}


SysErrCause OstmIO::TargetRead (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size)
{
    TRegBase* reg = m_ostm_core->GetTrPtr(addr - m_base_addr);
    reg->Read(sideband, addr - m_base_addr, data, size);
    uint32_t val = 0;
    memcpy (&val, data, size);
    CfMsg::TimerPrint("[OST@PE%d.CH%d.%-8s:Read_data=0x%08x]\n",
                      m_ostm_core->GetPeId(), m_ostm_core->GetOstmCh(), reg->GetName(), val);
    return SYSERR_NONE;
}


SysErrCause OstmIO::TargetFetch (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size)
{
    TRegBase* reg = m_ostm_core->GetTrPtr(addr - m_base_addr);
    reg->Read(sideband, addr - m_base_addr, data, size);
    return SYSERR_NONE;
}


SysErrCause OstmIO::TargetWriteDebug (PhysAddr addr, uint8_t* data, ByteSize size)
{
    TargetWrite (SideBand::DebugAccess(), addr, data, size);
    return SYSERR_NONE;
}


SysErrCause OstmIO::TargetReadDebug (PhysAddr addr, uint8_t* data, ByteSize size)
{
    TargetRead (SideBand::DebugAccess(), addr, data, size);
    return SYSERR_NONE;
}


/**
 * @brief Reset device
 *
 * Do power-on reset.
 */
void OstmIO::HardReset (void)
{
    m_ostm_core->PowerOnReset();
    m_wait_count = 0;
}


/**
 * @brief cyclic event
 *
 * Execute OSTM counter.
 */
void OstmIO::CyclicHandler (void)
{
#ifdef USE_OSTM_RUNMODULE
#else // USE_OSTM_RUNMODULE
    if (m_wait_count == 0) {
        m_ostm_core->Drive();
    } else {
        -- m_wait_count;
    }
#endif // USE_OSTM_RUNMODULE
}


/**
 * @brief  Check access permission
 * @return Error info
 */
SysErrCause
OstmIO::IsAccPermited (SideBand sideband, PhysAddr addr, ByteSize size, MemAccessCommand cmd)
{
    return SYSERR_NONE;
}

#ifdef   OSTM_DEBUG
/**
 * @brief  Print debug message for OSTM
 *
 * Print registr value and internal status
 */
void OstmIO::Debug() const {
    m_ostm_core->PrintRegister(stderr);
    m_ostm_core->PrintInternal(stderr);
}
#endif //OSTM_DEBUG

const uint32_t   OstmIO::m_map_size = 0x1000;

#if (_UNIT_TEST)
void OstmIO::SetIntcPtr(MockCIntc1* intc1, MockCIntc2* intc2)
{
    m_ostm_core->SetIntcPtr(intc1, intc2);
}
#else
void OstmIO::SetIntcPtr(CIntc1* intc1, CIntc2* intc2)
{
    m_ostm_core->SetIntcPtr(intc1, intc2);
}
#endif

//////////////////////////////////////////////////////////////////////////////
//
//  OstmClock - Execution interface of OSTM.
//
//////////////////////////////////////////////////////////////////////////////

/**
 * @brief Run OSTM
 *
 * @attention This method doesn't call for each 1 clock.
 *            Calcurates cycle counts for next event.
 *            Sleep (Doesn't call this method) until the next event.
 */
IsElapsedTime OstmClock::RunModule (void)
{
#ifdef USE_OSTM_RUNMODULE
    Drive();
    ClockRequest(IsRunning());
#endif // USE_OSTM_RUNMODULE
    return TIME_ELAPSED;
}

/**
 * @brief  Register/Remove OSTM to the running queue of CForest (at FSBUS)
 *
 * Control event to call RunModule
 */
void OstmClock::ClockRequest(bool req)
{
    if (m_last_state != req) {
        if (req) {
#ifdef USE_OSTM_RUNMODULE
            p_internals->AddRunTarget(this);
#else // USE_OSTM_RUNMODULE
            m_fsbus->AddCyclicHandler (m_ostmio);
#endif // USE_OSTM_RUNMODULE
        } else {
#ifdef USE_OSTM_RUNMODULE
            p_internals->RemoveRunTarget(this);
#else // USE_OSTM_RUNMODULE
            m_fsbus->RemoveCyclicHandler (m_ostmio);
#endif // USE_OSTM_RUNMODULE
        }
        m_last_state = req;
    }

    if (req) {
#ifdef USE_OSTM_RUNMODULE
        ClockWait (GetProcessCycle());
#else // USE_OSTM_RUNMODULE
        m_ostmio->ClockWait (GetProcessCycle());
#endif // USE_OSTM_RUNMODULE
    }
}

uint64_t OstmClock::GetSystemTime(void) const
{
#if (_UNIT_TEST)
    return 1;
#else
    return p_internals->GetSystemTime();
#endif
}

const int32_t OstmClock::OSTM_SCID = 0x7ffe0000;


//////////////////////////////////////////////////////////////////////////////
//
//  OstmCore - OSTM functions
//
//////////////////////////////////////////////////////////////////////////////
#if (_UNIT_TEST)
OstmCore::OstmCore(uint32_t peid, MockCIntc1* intc, MockCIntc2* intc2,
                     uint32_t intch, uint32_t ostmch, MockFsBus* fsbus,
                     OstmIO* ostmio, uint32_t ratio)
 : OstmClock (ratio), m_peid(peid), m_status(OSTM_IDLE), m_cycle(0),
   m_ostmch(ostmch), m_irq(this, intc, intch, intc2)
{
    m_fsbus = fsbus;
    m_ostmio = ostmio;
    m_last_state = false;
}
#else
OstmCore::OstmCore(uint32_t peid, CIntc1* intc, CIntc2* intc2,
                     uint32_t intch, uint32_t ostmch, FsBus* fsbus,
                     OstmIO* ostmio, uint32_t ratio)
 : OstmClock (ratio), m_peid(peid), m_status(OSTM_IDLE), m_cycle(0),
   m_ostmch(ostmch), m_irq(this, intc, intch, intc2)
{
    m_fsbus = fsbus;
    m_ostmio = ostmio;
    m_last_state = false;
}
#endif

OstmCore::~OstmCore()
{
    for (TRegMap::iterator itr = m_trf.begin(); itr != m_trf.end(); itr++) {
        delete itr->second;
    }
}


/**
 * @brief Saves operating mode on start
 *
 * @note Keeps OSTMnMD1 when the time is started/restarted.
 */
void OstmCore::SaveMode(void)
{
    m_mode = (OSTMnCTL->GetOSTMnMD1()) ? OSTM_COMPARE : OSTM_INTERVAL;
}

/**
 * @brief Saves interruption of counter start setting on start
 *
 * @note Keeps OSTMnMD0 when the time is started/restarted.
 */
void OstmCore::SaveStartIntMode(void)
{
    m_start_int = (OSTMnCTL->GetOSTMnMD0()) ? true : false;
}

/**
 * @brief Saves interruption enable setting on start
 *
 * @note Keeps OSTMnIE when the time is started/restarted.
 */
void OstmCore::SaveIntEnabledMode(void)
{
    m_int_enabled = (OSTMnCTL->GetOSTMnIE()) ? true : false;
}

/**
 * @brief Saves start value of count down
 *
 * @note Used to calculate OSTMnCNT for interval mode in case of MD2 = 0
 */
void OstmCore::SaveCmpVal(void)
{
        m_compval = OSTMnCMP->GetVal();
}

/**
 * @brief Saves start value of count down
 *
 * @note Used to calculate OSTMnCNT for others
 */
void OstmCore::SaveCntVal(void)
{
        m_compval = OSTMnCNT->GetVal();
}

/**
 * @brief Instanciate registers
 * @note Register registers into TRegMap
 */
void OstmCore::InitTRegMap(void)
{
    /* Register OSTM registers into Register map. */
    /* Additonally, Keep pointer into m_REGNAME to access easy. */
    m_trf.insert ( TRegPair(TRegOSTMnCMP::m_addr , (OSTMnCMP = new TRegOSTMnCMP(this)  )) );
    m_trf.insert ( TRegPair(TRegOSTMnCNT::m_addr , (OSTMnCNT = new TRegOSTMnCNT (this) )) );
    m_trf.insert ( TRegPair(TRegOSTMnTO::m_addr  , (OSTMnTO  = new TRegOSTMnTO (this)  )) );
    m_trf.insert ( TRegPair(TRegOSTMnTOE::m_addr , (OSTMnTOE = new TRegOSTMnTOE (this) )) );
    m_trf.insert ( TRegPair(TRegOSTMnTE::m_addr  , (OSTMnTE  = new TRegOSTMnTE (this)  )) );
    m_trf.insert ( TRegPair(TRegOSTMnTS::m_addr  , (OSTMnTS  = new TRegOSTMnTS (this)  )) );
    m_trf.insert ( TRegPair(TRegOSTMnTT::m_addr  , (OSTMnTT  = new TRegOSTMnTT (this)  )) );
    m_trf.insert ( TRegPair(TRegOSTMnCTL::m_addr , (OSTMnCTL = new TRegOSTMnCTL (this) )) );
    m_trf.insert ( TRegPair(TRegOSTMnEMU::m_addr , (OSTMnEMU = new TRegOSTMnEMU (this) )) );
}


/**
 * @brief Reset
 *
 * @note Keep current value for the registers which initial value is UNDEFINED
 */
void OstmCore::PowerOnReset(void)
{
    // -> std::for_each (... lambda expression);
    for (TRegMap::iterator itr = m_trf.begin(); itr != m_trf.end(); itr->second->Reset(), itr++);

    m_status = OSTM_IDLE;
    m_cycle  = 0;
    m_md2_changed = false;
    // Clear scheduled event
    ClockRequest(false);
}


/**
 * @brief Start Scheduler
 */
void OstmCore::SetCyclicHandler(OstmEvent ev)
{
    m_event = ev;
    ClockRequest(true);
}


/**
 * @brief Update registers and event for elapsed time
 */
void OstmCore::Drive (void)
{
    std::ostringstream oss;
    oss << "[OST@PE" << GetPeId() << ".CH" << m_ostmch << ".";
    oss << std::left << std::setw(8) << "OSTM" << ":";

    OstmException intreq_type = OSTM_EXP_NA;
    if (m_event == OSTM_EVENT_ST) {
        if (IsIntervalMode()) {
            oss << "START_INTERVAL";
            // Update timer start time by current system time
            m_start_cycle = GetSystemTime();
            // Schedule the underflow event at the next cycle of count 0.
            if (!OSTMnCTL->GetOSTMnMD2()){
                uint64_t ostm_cmp = static_cast<uint64_t>(OSTMnCMP->GetVal());
                m_cycle = ostm_cmp + 1;
            } else {
                uint64_t ostm_cnt = static_cast<uint64_t>(OSTMnCNT->GetVal());
                m_cycle = ostm_cnt + 1;
            }
            m_event = OSTM_EVENT_UF;
        } else {
            oss << "START_COMPARE";
            // Update timer start time by current system time
            m_start_cycle = GetSystemTime();
            // Schedule the compare-match event at the next cycle of match.
            if (!OSTMnCTL->GetOSTMnMD2()){
                uint64_t ostm_cmp = static_cast<uint64_t>(OSTMnCMP->GetVal());
                m_cycle = ostm_cmp + 1;
            } else {
                uint64_t ostm_cnt = static_cast<uint64_t>(OSTMnCNT->GetVal());
                uint64_t ostm_cmp = static_cast<uint64_t>(OSTMnCMP->GetVal());
                uint64_t diff_cnt = (ostm_cmp >= ostm_cnt) ? (ostm_cmp - ostm_cnt + 1)
                                                           : (ostm_cmp + 0x100000000ULL - ostm_cnt);
                m_cycle = diff_cnt;
            }
            m_event = OSTM_EVENT_CM;
        }

        if (m_start_int == true) {
            if (OSTMnTOE->GetVal() & 1) {
                // Toggle OSTMnTO bit
                OSTMnTO->SetVal(OSTMnTO->GetVal() ^ 1);
                oss << ":TO=" << static_cast<uint32_t>(OSTMnTO->GetVal());
            }
            // Cause interruption
            if (m_int_enabled == 1) {
                intreq_type = OSTM_EXP_ST;
            }
        }
    } else if (m_event == OSTM_EVENT_UF) {
        oss << "EVENT_UF";
        // Update timer start time by current system time
        m_start_cycle = GetSystemTime();

        // Schedule the underflow event at the next cycle of count 0.
        uint64_t ostm_cmp = static_cast<uint64_t>(OSTMnCMP->GetVal());
        m_cycle = ostm_cmp + 1;

        // Save OSTMnCMP
        SaveCmpVal();

        if (OSTMnTOE->GetVal() & 1) {
              // Toggle OSTMnTO bit
              OSTMnTO->SetVal(OSTMnTO->GetVal() ^ 1);
              oss << ":TO=" << static_cast<uint32_t>(OSTMnTO->GetVal());
        }
        // Cause interruption
        if (m_int_enabled == 1) {
            intreq_type = OSTM_EXP_UF;
        }
    } else if (m_event == OSTM_EVENT_CM) {
        oss << "EVENT_CM";
        // Schedule next compare-match event
        m_cycle = 0x100000000ULL;
        if (OSTMnTOE->GetVal() & 1) {
              // Toggle OSTMnTO bit
              OSTMnTO->SetVal(OSTMnTO->GetVal() ^ 1);
              oss << ":TO=" << static_cast<uint32_t>(OSTMnTO->GetVal());
        }
        // Cause interruption
        if (m_int_enabled == 1) {
            intreq_type = OSTM_EXP_CM;
        }
    }
    oss << "]";
    CfMsg::TimerPrint("%s\n", oss.str().c_str());

    if (intreq_type != OSTM_EXP_NA) {
        m_irq.RequestInterrupt();
    }

#ifdef USE_OSTM_RUNMODULE
#else // USE_OSTM_RUNMODULE
    ClockRequest(IsRunning());
#endif // USE_OSTM_RUNMODULE
}

/**
 * @brief Processing timer start/re-start
 *
 * @note Process when writing 1 to OSTMnTS
 */
void OstmCore::Start(void)
{
    if (IsRunning()) {
        // Restart is available on interval mode
        if (IsIntervalMode()) {
            ClockRequest(false);
            m_time = GetSystemTime();
            m_cycle = GetCpuRatio() - (m_time % GetCpuRatio());
            SetCyclicHandler(OSTM_EVENT_ST);
            // Update timer start time by current system time
            m_start_cycle = m_time;
            // OSTMnEI
            SaveIntEnabledMode();
            // Save OSTMnMD0 on starting timer.
            // Although OSTMnMD0 can be modified anytime,
            // the setting on start is used on running state.
            SaveStartIntMode();
            // Save OSTMnCMP to update OSTMnCNT for count down.
            SaveCmpVal();
            CfMsg::TimerPrint(
                "[OST@PE%d.CH%d.%-8s:RESTART:Next_Wakeup=%d cycle]\n",
                GetPeId(), m_ostmch, "OSTM", m_time);
        }
    } else {
        // Schedule timer wakeup event on next clock cycle
        m_time = GetSystemTime();
        m_cycle = GetCpuRatio() - (m_time % GetCpuRatio());
        SetCyclicHandler(OSTM_EVENT_ST);
        // Update timer start time by current system time
        m_start_cycle = m_time;
        // OSTMnEI
        SaveIntEnabledMode();
        // Save OSTMnMD0 on starting timer.
        // Although OSTMnMD0 can be modified anytime,
        // the setting on start is used on running state.
        SaveStartIntMode();
        // Save OSTMnMD1
        SaveMode();

        // Save OSTMnCMP to update OSTMnCNT for count down.
        if (!OSTMnCTL->GetOSTMnMD2()){
            SaveCmpVal();
            m_comp_cnt_val = OSTMnCNT->GetVal();
        } else {
            SaveCntVal();
        }
        // Go running state
        SetRUN();
        CfMsg::TimerPrint(
               "[OST@PE%d.CH%d.%-8s:RUN]\n",
            GetPeId(), m_ostmch, "OSTM");
    }
}

/**
 * @brief Stop timer
 *
 * @note Process when write 1 to OSTMnTT while timer running
 */
void OstmCore::Stop(void)
{
    // Update OSTMnCNT for stop timing
    UpdateOSTMnCNT();
    m_time = GetSystemTime();
    // Cancel schduled event
    ClockRequest(false);
    // Go stop state
    SetIDLE();
    CfMsg::TimerPrint(
        "[OST@PE%d.CH%d.%-8s:IDLE:Updated %-8s=0x%08x]\n",
        GetPeId(), m_ostmch, "OSTM", OSTMnCNT->GetName(), OSTMnCNT->GetVal());
}

/**
 * @brief Restore CNT to compare value
 *
 * @note Process when write 1 to OSTMnTS while timer running in Free-Run Compare Mode
 */
void OstmCore::RestoreCNTtoCMP(void)
{
    m_compval = m_comp_cnt_val;
}

/**
 * @brief Notice that MD2 changed from 0 to 1
 */
void OstmCore::SetEventMD2Updated(void)
{
	m_md2_changed = true;
}

void OstmCore::CheckOSTMnCMP(void)
{
    if (IsRunning() && IsCompareMode()) {
        UpdateOSTMnCNT (); // Update OSTMnCNT
        ClockRequest(false);
        uint64_t ostm_cnt = static_cast<uint64_t>(OSTMnCNT->GetVal());
        uint64_t ostm_cmp = static_cast<uint64_t>(OSTMnCMP->GetVal());
        uint64_t diff_cnt = (ostm_cmp >= ostm_cnt) ? (ostm_cmp - ostm_cnt + 1)
                                                   : (ostm_cmp + 0x100000000ULL - ostm_cnt);

        m_time = GetSystemTime();
        m_cycle = diff_cnt;
        SetCyclicHandler(OSTM_EVENT_CM);
        CfMsg::TimerPrint(
            "[OST@PE%d.CH%d.%-8s:RESCHEDULE:NextWakeup=%lu cycle]\n",
            GetPeId(), m_ostmch, "OSTM", m_cycle + m_time);
    }
}

void OstmCore::UpdateOSTMnCNT(void)
{
    if (IsRunning()) {
        uint64_t elapsed = (GetSystemTime() - m_start_cycle) /  GetCpuRatio();
        if (!OSTMnCTL->GetOSTMnMD2()){ // MD2 = 0
            if (IsCompareMode()) { // Free-run compare mode
            // Counter increase from 0 to 0xFFFFFFFF
                if (elapsed != 0){
                    uint64_t ostm_cnt = elapsed & 0x0FFFFFFFFULL;
                    OSTMnCNT->SetVal(static_cast<uint32_t>(ostm_cnt));
                }else{
                    uint64_t ostm_cnt = 0x0;
                    OSTMnCNT->SetVal(static_cast<uint32_t>(ostm_cnt));
                }
            } else {
            // Counter decrease from CMP-value to 0
                if (elapsed != 0){
                    uint64_t ostm_cnt = (m_compval < elapsed) ? m_compval : (m_compval - elapsed);
                    OSTMnCNT->SetVal(static_cast<uint32_t>(ostm_cnt));
                }else{
                    uint64_t ostm_cnt = m_compval;
                    OSTMnCNT->SetVal(static_cast<uint32_t>(ostm_cnt));
                }
            }
        } else { // MD2 = 1
        // Behavior same as above two mode, but CNT at starting time was not initialized
            if (IsCompareMode()) {
                if (m_md2_changed){
                    SaveCntVal();
                }
                uint64_t ostm_cnt = (m_compval + elapsed) & 0x0FFFFFFFFULL;
                OSTMnCNT->SetVal(static_cast<uint32_t>(ostm_cnt));
                m_md2_changed = false;
            } else {
                if (m_compval < elapsed){
                    uint64_t ostm_cnt = OSTMnCMP->GetVal();
                    OSTMnCNT->SetVal(static_cast<uint32_t>(ostm_cnt));
                    m_start_cycle = GetSystemTime();//At the time of underflow, need to refresh counter
                } else {
                    uint64_t ostm_cnt = m_compval - elapsed;
                    OSTMnCNT->SetVal(static_cast<uint32_t>(ostm_cnt & 0x0FFFFFFFFULL));
                }
            }
        }
    }
}

void OstmCore::SetOSTMnCNT(uint32_t val)
{
    OSTMnCNT->SetVal(val);
}

uint32_t OstmCore::GetOSTMnTOE(void) const
{
    return OSTMnTOE->GetVal();
}

uint32_t OstmCore::GetOSTMnTE(void) const
{
    return OSTMnTE->GetVal();
}

void OstmCore::SetOSTMnTE (uint32_t val)
{
    OSTMnTE->SetVal(val);
}

uint32_t OstmCore::GetOSTMnCNT(void) const
{
    return OSTMnCNT->GetVal();
}

uint32_t OstmCore::GetOSTMnCMP(void) const
{
    return OSTMnCMP->GetVal();
}

void OstmCore::SetOSTMnCTL(uint32_t val)
{
    OSTMnCTL->SetVal(val);
}

uint32_t OstmCore::GetOSTMnCTL(void) const
{
    return OSTMnCTL->GetVal();
}

bool OstmCore::GetOSTMnCTLMD2(void) const
{
    return OSTMnCTL->GetOSTMnMD2();
}

bool OstmCore::GetOSTMnCTLMD1(void) const
{
    return OSTMnCTL->GetOSTMnMD1();
}

void OstmCore::SetOSTMnCMP(uint32_t val)
{
    OSTMnCMP->SetVal(val);
}


uint32_t OstmCore::GetPeId(void) const
{
    return m_peid;
}


#ifdef OSTM_DEBUG
/**
 * @brief  Output OSTM registers value to stream (For debug OSTM)
 * @param  os stream for output
 */
void OstmCore::PrintRegister(std::FILE* os) const {
    uint32_t buff;
    uint8_t* p = reinterpret_cast<uint8_t*>(&buff);

    std::fprintf(os, "==== OSTM REGISTER ====\n");

    int32_t cr = 0;

    for (TRegMap::const_iterator itr = m_trf.begin(); itr != m_trf.end(); itr++) {
        buff = 0;
        itr->second->ReadBody(0, p, 4);
        std::fprintf(os, "%08lx : %s = %08x", itr->first, itr->second->GetName(), buff);
        if (cr++ == 7) {
            std::fputc('\n', os);
        } else {
            std::fputs(", ", os);
        }
    }
    std::fputc('\n', os);
}


/**
 * @brief  Output OSTM registers value to stream with header info (For debug OSTM)
 * @param  os stream for output
 */
void OstmCore::PrintRegister(std::FILE* os, bool header) const {
    if (header) {
        for (TRegMap::const_iterator itr = m_trf.begin(); itr != m_trf.end(); itr++) {
            std::fprintf(os, "%s    ", itr->second->GetName());
        }
        std::fputc('\n', os);
    }

    uint32_t buff;
    uint8_t* p = reinterpret_cast<uint8_t*>(&buff);

    for (TRegMap::const_iterator itr = m_trf.begin(); itr != m_trf.end(); itr++) {
        buff = 0;
        itr->second->ReadBody(0, p, 4);
        std::fprintf(os, "%08x ", buff);
    }
    std::fputc('\n', os);
}


/**
 * @brief  Output OSTM internal value and status to stream (For debug OSTM)
 * @param  os stream for output
 */
void OstmCore::PrintInternal(std::FILE* os) const {
    std::fprintf(os, "==== OSTM INTERNAL ====\n");

    if (m_status == OSTM_IDLE) {
        std::fprintf(os, "Ostm is on OSTM_IDLE.\n\n");
    }else{
        std::fprintf(os, "Ostm is on RUNNING.\n");
        std::fprintf(os, "    To be counted down = [%ld]cycle\n\n", m_cycle);
    }
}

#endif // OSTM_DEBUG



//////////////////////////////////////////////////////////////////////////////
//  OSTM IRQ
//////////////////////////////////////////////////////////////////////////////
#if (_UNIT_TEST)
bool OstmCore::IntReq::RequestInterrupt (void) {
    return true;
}
# else
bool OstmCore::IntReq::RequestInterrupt (void) {
    if (m_channel == INTC_CH_FENMI) {
        if (m_intc != NULL) {
            return (m_intc->ReqIntByFenmi() == INT_E_OK);
        } else {
            // Distribute to all PE.
            uint32_t last_peid = p_forest_util->GetPeLastId ();
            for (uint32_t peid = 0; peid <= last_peid; ++ peid) {
                p_forest_util->ReqFenmiToIntc1 (peid);
            }
        }
    } else if ((m_channel <= INTC_CH_FEINT) && (m_channel > INTC2_CH_MAX)) {
        uint32_t feint_ch = m_channel - (INTC2_CH_MAX + 1);
        if (m_intc != NULL) {
            return (m_intc->ReqIntByFeint(feint_ch) == INT_E_OK);
        } else {
            // Distribute to all PE.
            uint32_t last_peid = p_forest_util->GetPeLastId ();
            for (uint32_t peid = 0; peid <= last_peid; ++ peid) {
                p_forest_util->ReqFeintToIntc1 (peid,feint_ch);
            }
        }
    } else if (m_channel <= INTC1_CH_MAX) {
        if (m_intc != NULL) {
            return (m_intc->ReqIntByPeripheral(m_channel) == INT_E_OK);
        } else {
            // Distribute to all PE.
            uint32_t last_peid = p_forest_util->GetPeLastId ();
            for (uint32_t peid = 0; peid <= last_peid; ++ peid) {
                p_forest_util->ReqInterruptToIntc1 (peid, m_channel);
            }
        }
    } else {
        if (m_intc2 != NULL) {
            return (m_intc2->ReqIntByPeripheral(m_channel) == INT_E_OK);
        }
    }
    return false;
}
#endif

//////////////////////////////////////////////////////////////////////////////
//  OSTM REGISTER
//////////////////////////////////////////////////////////////////////////////

// offset address
const PhysAddr   TRegOSTMnCMP::m_addr = 0x00;
const PhysAddr   TRegOSTMnCNT::m_addr = 0x04;
const PhysAddr   TRegOSTMnTO::m_addr  = 0x08;
const PhysAddr   TRegOSTMnTOE::m_addr = 0x0C;
const PhysAddr   TRegOSTMnTE::m_addr  = 0x10;
const PhysAddr   TRegOSTMnTS::m_addr  = 0x14;
const PhysAddr   TRegOSTMnTT::m_addr  = 0x18;
const PhysAddr   TRegOSTMnCTL::m_addr = 0x20;
const PhysAddr   TRegOSTMnEMU::m_addr = 0x24;

// register name
const char* const TRegOSTMnCMP::m_name = "OSTMnCMP";
const char* const TRegOSTMnCNT::m_name = "OSTMnCNT";
const char* const TRegOSTMnTO::m_name  = "OSTMnTO";
const char* const TRegOSTMnTOE::m_name = "OSTMnTOE";
const char* const TRegOSTMnTE::m_name  = "OSTMnTE ";
const char* const TRegOSTMnTS::m_name  = "OSTMnTS";
const char* const TRegOSTMnTT::m_name  = "OSTMnTT";
const char* const TRegOSTMnCTL::m_name = "OSTMnCTL";
const char* const TRegOSTMnEMU::m_name = "OSTMnEMU";


SysErrCause TRegOSTMnCMP::Write (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size)
{
    SysErrCause wr_err = SYSERR_NONE;

    if (IsRegAccessSizeValid (size) == false) {
        PrintAccessSizeWarning ();
        return wr_err;
    }

    wr_err = WriteBody (addr, data, size);

    // Reschedule next compare match timing.
    // Because when running on compare mode,
    // next compare-match timing is updated by the written value.
    m_core->CheckOSTMnCMP();
    return wr_err;
}


SysErrCause TRegOSTMnCNT::Read (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size) const
{
    SysErrCause wr_err = SYSERR_NONE;
    // Only on running state, update OSTMnCNT (running status is checked in UpdateOSTMnCNT())
    m_core->UpdateOSTMnCNT();

    if (IsRegAccessSizeValid (size)) {
        wr_err = ReadBody (addr, data, size);
    } else {
        PrintAccessSizeWarning ();
        std::fill_n (data, size, 0);
    }
    return wr_err;
}

SysErrCause TRegOSTMnCNT::Write (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size)
{
    SysErrCause wr_err = SYSERR_NONE;
    if (IsRegAccessSizeValid (size) == false) {
        PrintAccessSizeWarning ();
        return wr_err;
    }
    if (m_core->GetOSTMnTE() == 0) {
        wr_err= WriteBody (addr, data, size);
    }
    return wr_err;
}

SysErrCause TRegOSTMnTO::Write (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size)
{
    SysErrCause wr_err = SYSERR_NONE;
    if (IsRegAccessSizeValid (size) == false) {
        PrintAccessSizeWarning ();
        return wr_err;
    }
    if (m_core->GetOSTMnTOE() == 0) {
        wr_err= WriteBody (addr, data, size);
    }
    return wr_err;
}

SysErrCause TRegOSTMnTE::Read (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size) const
{
    SysErrCause wr_err = SYSERR_NONE;
    if (IsRegAccessSizeValid (size)) {
        data[1] = data[2] = data[3] = 0;
        data[0] = (m_core->IsRunning()) ? 1 : 0;
    } else {
        PrintAccessSizeWarning ();
        std::fill_n (data, size, 0);
    }
    return wr_err;
}


SysErrCause TRegOSTMnTS::Write (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size)
{
    SysErrCause wr_err = SYSERR_NONE;
    if (IsRegAccessSizeValid (size) == false) {
        PrintAccessSizeWarning ();
        return wr_err;
    }

    if (data[0] & 1) {
        wr_err= WriteBody (addr, data, size);
        m_core->SetOSTMnTE(0x1);
        if (m_core->IsRunning()) {
            if (m_core->IsIntervalMode()){
                // In the interval mode,
                // forced restart operation is performed regardless of the value of OSTMnMD2.
                m_core->SetOSTMnCNT(m_core->GetOSTMnCMP());
                // Restart timer
                m_core->Start();
            } else {
                // In the free-run compare mode,
                // forced restart operation is not performed.
                m_core->RestoreCNTtoCMP();
            }
        } else {
            if (!(m_core->GetOSTMnCTLMD2())){ //MD2=0
                if (!(m_core->GetOSTMnCTLMD1())){ //interval mode
                    m_core->SetOSTMnCNT(m_core->GetOSTMnCMP());
                } else { //free-run mode
                    m_core->SetOSTMnCNT(0x0);
                }
            } else {
                // No need to anything because counter is stopped.
            }
            // Start timer
            m_core->Start();
        }
    }
    return wr_err;
}

SysErrCause TRegOSTMnTS::Read (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size) const
{
    SysErrCause wr_err = SYSERR_NONE;
    std::fill_n (data, size, 0);
    return wr_err;
}


SysErrCause TRegOSTMnTT::Write (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size)
{
    SysErrCause wr_err = SYSERR_NONE;
    if (IsRegAccessSizeValid (size) == false) {
        PrintAccessSizeWarning ();
        return wr_err;
    }

    if (data[0] & 1) {
        WriteBody (addr, data, size);
    }

    if (m_core->IsRunning()) {
        if (data[0] & 1) {
            m_core->SetOSTMnTE(0x0);
            // Stop timer
            m_core->Stop();
        }
    }
    return wr_err;
}

SysErrCause TRegOSTMnTT::Read (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size) const
{
    SysErrCause wr_err = SYSERR_NONE;
    std::fill_n (data, size, 0);
    return wr_err;
}


SysErrCause TRegOSTMnCTL::Write (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size)
{
    SysErrCause wr_err = SYSERR_NONE;
    if (IsRegAccessSizeValid (size) == false) {
        PrintAccessSizeWarning ();
        return wr_err;
    }
    bool prev_md2 = GetOSTMnMD2();
    WriteBody (addr, data, size);
    bool pos_md2 = GetOSTMnMD2();
    if (!prev_md2 & pos_md2){
        m_core->SetEventMD2Updated();
    }
    return wr_err;
}
