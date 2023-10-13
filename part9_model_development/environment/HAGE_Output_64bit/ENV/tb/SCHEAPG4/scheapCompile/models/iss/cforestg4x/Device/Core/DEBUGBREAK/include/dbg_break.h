/*
 * (c) 2011 Renesas Electronics Corporation
 * RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY
 * This program must be used solely for the purpose for which
 * it was furnished by Renesas Electronics Corporation. No part of this
 * program may be reproduced or disclosed to others, in any
 * form, without the prior written permission of Renesas Electronics
 * Corporation.
 */

#ifndef __DEBUG_BREAK_H__
#define __DEBUG_BREAK_H__

#include <vector>

class SRegFile;
class TraceCtrl;

#ifdef _UNIT_TEST
#include "gtest/gtest.h"
#endif

class DbgBreak {
public:
    static const uint32_t N_BPA = 12;
    static const uint32_t N_BPD =  4;

#ifdef ENABLE_DBG_BREAK
    ExpCode UpdateBreakPcbExcp (PhysAddr addr, TraceCtrl* trace);
    template <MemAccessCommand cmd>
    ExpCode UpdateBreakLsabExcp (PhysAddr addr, ByteSize size,
                                TraceCtrl* trace, LsabType is_eiint);
    bool IsDbSyncExcp (ExpCode exp_code) const
    { return (exp_code == EXP_LSAB_DB || exp_code == EXP_LSAB_EIINT); }

    DbgBreak (SRegFile* srf, CoreType core_type, CoreVersion core_version);
    ~DbgBreak () {}

    void ClearPcbEoReserved (TraceCtrl* trace);
    void ApplyPcbEoReserved (TraceCtrl* trace);
    void ClearLsabEoReserved (TraceCtrl* trace);
    void ApplyLsabEoReserved (TraceCtrl* trace);
    void InitEoReserved (void);

private:
#ifdef _UNIT_TEST
    FRIEND_TEST(DbgBreakMethodCheck, CheckInitEoReserved);
    FRIEND_TEST(DbgBreakMethodCheck, CheckClearPcbEoReserved);
    FRIEND_TEST(DbgBreakMethodCheck, CheckApplyPcbEoReserved);
    FRIEND_TEST(DbgBreakMethodCheck, CheckClearLsabEoReserved);
    FRIEND_TEST(DbgBreakMethodCheck, CheckApplyLsabEoReserved);
    FRIEND_TEST(DbgBreakMethodCheck, CheckUpdateBreakPcbExcp);
    FRIEND_TEST(DbgBreakMethodCheck, CheckIsBrkAddrHit);
    FRIEND_TEST(DbgBreakMethodCheck, CheckIsBrkSizeHit);
    FRIEND_TEST(DbgBreakMethodCheck_BREFORE_G4MH20, CheckIsDebuggableMode_a);
    FRIEND_TEST(DbgBreakMethodCheck, CheckIsDebuggableMode_b);
    FRIEND_TEST(DbgBreakMethodCheck, CheckIsDebuggableMode_c);
    FRIEND_TEST(DbgBreakMethodCheck, CheckIsDebuggableMode_d);
    FRIEND_TEST(DbgBreakMethodCheck, CheckIsDebuggableMode_e);
    FRIEND_TEST(DbgBreakMethodCheck, CheckIsDebuggableMode_f);
#endif
    std::vector<uint32_t> m_pcb_eo_reserve;
    std::vector<uint32_t> m_lsab_eo_reserve;

    SRegFile* m_srf;
    const CoreType m_core_type;
    const CoreVersion m_core_version;
    bool m_vm_supported; // Virtualization support
    bool IsBrkAddrHit (uint32_t ch, PhysAddr addr) const;
    bool IsBrkSizeHit (uint32_t ch, ByteSize size, TraceCtrl* trace) const;
    bool IsBrkAddrSizeHit (uint32_t ch, PhysAddr addr, ByteSize size, TraceCtrl* trace) const;
    bool IsSeqBrkHit (uint32_t ch) const;
    bool IsSeqBrkValidComb (uint32_t ch) const;
    void UpdateExpCodeAndSR (uint32_t ch, ExpCode exp_code, TraceCtrl* trace);
    bool IsBrkExcpAcceptable (void) const;
    bool IsDebuggableMode (uint32_t ch) const;
#endif // #ifdef ENABLE_DBG_BREAK
};
#endif // __DEBUG_BREAK_H__
