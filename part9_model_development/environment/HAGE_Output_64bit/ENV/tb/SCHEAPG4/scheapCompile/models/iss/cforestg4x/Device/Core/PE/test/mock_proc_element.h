/**
 * @file mock_proc_element.h
 *
 * @date Created on: 2017
 * @author Author: Hiroyuki Ono <hiroyuki.ono.jc@renesas.com>
 * @brief UNIT TEST for ProcElement.
 * @par Copyright: 2016 Renesas Electronics Corp.
 */
#pragma once

#include "proc_element_base.h"
#include "gmock/gmock.h"

// if m_core_type == CORE_G4MH && m_core_version >= CORE_VER_20
static const int N_MPU_REGION = 32;
// else
// static const int N_MPU_REGION = 24;

using ::testing::Invoke;
using ::testing::AnyNumber;

class ProcElementDummy : public ProcElementBase {
private:
  void* t = nullptr;
  const uint32_t  m_pe_id;
  const CoreType  m_core_type;
  const CoreVersion m_core_version;
  const ArchClass m_arch_class;

public:
  CoreType GetCoreType(void) const { return CORE_G4MH; };
  CoreVersion GetCoreVersion(void) const { return CORE_VER_20; };
  FsCtrl_Interface* GetHtPtr(void) const { return static_cast<FsCtrl_Interface*>(t); };
  SRegFile* GetSrfPtr(ContextId tc_id) const { return static_cast<SRegFile*>(t); };
  NcSRegFile* GetNcSrfPtr(void) const { return static_cast<NcSRegFile*>(t); };
  uint32_t GetExtPin(ExtPinType pin) const { return 0; };
  bool IsHvEnabled(void) const { return false; };
  bool IsHostMode(void) const { return false; };
  typedef std::vector<FsCtrl_Interface*> HtVec;
  HtVec* GetHtsPtr(void) {
    return static_cast<HtVec*>(t);
    ;
  };
  bool HasFpu(void) const { return false; };
  bool HasFxu(void) const { return false; };
  uint32_t GetNumMpuRegion(void) const { return 32; };
  bool HasMpu(void) const { return false; };
#ifdef ENABLE_PEID_POS_CTRL
  bool IsPeidPosG4x (void) const [ return true; }
#endif // #ifdef ENABLE_PEID_POS_CTRL
  Icache* GetIcPtr(void) const {
    return static_cast<Icache*>(t);
    ;
  };
  uint16_t GetSnoozePeriod(void) const { return 0; };
  ArchClass GetArchClass(void) const { return E3V5_F; };

  void UpdateHvStatus(void){};
  void ChangeToHostMode(void){};
  void ChangeToGuestMode(void){};
  void UpdateSideband (void) {};
  void UpdateHtActive(void) const {};
  // void UpdateRunnableByIntreq (void);
  void UpdatePrivLevel(void) const {};
  void ClearBtb(void) const {};
  void ReallocMpuRegion(void){};
  void ChangeMpuBank(uint32_t bank){};
  void NoticeCpuGuestModeToIntc (bool is_gm) {}
  void NoticeCpuGpidToIntc (uint32_t gpid) {}

  ProcElementDummy(uint32_t pe_id, CoreType core_type, CoreVersion core_version,
                           ArchClass arch_class)
      : m_pe_id(pe_id),
        m_core_type(core_type),
        m_core_version(core_version),
        m_arch_class(arch_class) {}

  ProcElementDummy()
      : m_pe_id(0), m_core_type(CORE_G4MH), m_core_version(CORE_VER_20), m_arch_class(E3V5_F)
  {}
  ~ProcElementDummy(){};
};

class IntReqInfoBase;
class CIntc2;
class MockProcElement : public ProcElementDummy {
public:
  MOCK_METHOD1(ReqIntByPeripheral, void(uint32_t));
  MOCK_METHOD1(CancelIntByPeripheral, void(uint32_t));

  MockProcElement(uint32_t pe_id, CoreType core_type, CoreVersion core_version,
                  ArchClass arch_class)
      : ProcElementDummy(pe_id, core_type, core_version, arch_class) {}

  MockProcElement() : ProcElementDummy() {}

  /*Add for test CIntc2*/
  MOCK_METHOD2(ReqIntByIntc2, void(uint32_t, IntReqInfoBase*));
  MOCK_METHOD1(SetIntc2Ptr, void(CIntc2*));
  MOCK_METHOD1(CancelIntByIntc2, void(uint32_t));
  MOCK_METHOD1(CancelIntByMask, void(uint32_t));
  MOCK_METHOD1(UpdateIntHighPriority, void(int32_t));
  MOCK_METHOD1(IsIntReqInQueue, bool(uint32_t));
  MOCK_METHOD0(GetPeId, void());

#if _INTC1_UNIT_TEST
  MOCK_METHOD0(GetTcLastId,ContextId(void));
  MOCK_METHOD0(GetHtPtr,MockFsCtrl* (void));

  MOCK_METHOD0(GetPeId,uint32_t(void));
  MOCK_METHOD1(SetIntc2Ptr,void (CIntc2*));
  MOCK_METHOD0(CancelIntByMask,uint32_t(void));
  MOCK_METHOD1(UpdateIntHighPriority,void(ContextId));
  MOCK_METHOD1(IsIntReqInQueue,bool(uint32_t));
  MOCK_METHOD2(ReqIntByIntc2,void(uint32_t,IntReqInfoBase*));
  MOCK_METHOD1(CancelIntByIntc2,void(uint32_t));
#endif
private:
  ProcElementDummy real_;
};
