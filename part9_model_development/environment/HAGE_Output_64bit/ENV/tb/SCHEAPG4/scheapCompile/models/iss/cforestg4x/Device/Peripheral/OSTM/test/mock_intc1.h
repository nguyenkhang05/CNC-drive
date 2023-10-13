/**
 * @file mock_fsbus.h
 *
 * @date Created on: Oct 21, 2016
 * @author Author: Can Vy <can.vy.wh@rvc.renesas.com>
 * @brief UNIT TEST for intc1.
 * @par Copyright: 2016 Renesas Electronics Corp.
 */
#include "gmock/gmock.h"
#include "./module_vc.h"
#include "proc_element.h"
#include "fsbus.h"
#include "./intc2.h"
#include "mock_fsbus.h"

class MockCIntc1 : public CModuleVc, public CIntcBase {
public:
    MockCIntc1 (MockProcElement* pe, uint32_t pe_id, MockFsBus* fsbus, CoreType core_type) { }
    ~MockCIntc1 () { }
#if _INTC1_20
    CoreVersion m_core_version = CORE_VER_20;
#else
    CoreVersion m_core_version = CORE_VER_10;
#endif
    MOCK_METHOD1 (UpdateHighPriority, void(int32_t));
    MOCK_METHOD1 (UpdateInterruptPort, void (uint32_t));
    MOCK_METHOD1 (ReqIntByEIC, void (uint32_t));
    MOCK_METHOD1 (CancelInt, void (uint32_t));
    MOCK_METHOD1 (CancelIntByMask, void (uint32_t));

    MOCK_METHOD4 (TargetWrite, SysErrCause (SideBand, PhysAddr, uint8_t*, ByteSize));
    MOCK_METHOD4 (TargetRead, SysErrCause (SideBand, PhysAddr, uint8_t*, ByteSize));
    MOCK_METHOD4 (TargetFetch, SysErrCause (SideBand, PhysAddr, uint8_t*, ByteSize));
    MOCK_METHOD3 (TargetWriteDebug, SysErrCause (PhysAddr, uint8_t*, ByteSize size));
    MOCK_METHOD3 (TargetReadDebug, SysErrCause (PhysAddr, uint8_t*, ByteSize size));
    MOCK_METHOD0 (HardReset, void (void));
    MOCK_METHOD0 (CyclicHandler, void (void));

    MOCK_CONST_METHOD1 (GetEICPtr, TRegEIC* (uint32_t));
    MOCK_CONST_METHOD1 (GetEIBDtr, TRegEIBD* (uint32_t));
    MOCK_CONST_METHOD0 (GetFNCPtr, TRegFNC* (void));
    MOCK_CONST_METHOD1 (GetFICPtr, TRegFIC* (uint32_t));
    MOCK_CONST_METHOD0 (GetCoreVersion, CoreVersion (void));
    MOCK_METHOD1 (UpdateOverwriteBit,void (uint32_t));
    MOCK_CONST_METHOD0 (GetExternalEIINTreq, bool (void));
    MOCK_CONST_METHOD1 (SetExternalEIINTreq, void (bool));
    uint32_t m_eic_eeic;
};


class MockCIntc2 : public CModuleVc{
public:
    MockCIntc2 (PeVec* pes, MockFsBus* mock_fsbus, Intc2ImplType type) { }
    ~MockCIntc2 () { }
    CoreVersion m_core_version = CORE_VER_20;

};

