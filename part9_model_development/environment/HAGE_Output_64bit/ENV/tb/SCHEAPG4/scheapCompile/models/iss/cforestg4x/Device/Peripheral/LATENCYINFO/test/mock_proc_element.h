/**
 * @file mock_proc_element.h
 *
 * @date Created on: 2016
 * @author Author: Hiroyuki Ono <hiroyuki.ono.jc@renesas.com>
 * @brief UNIT TEST for ProcElement.
 * @par Copyright: 2016 Renesas Electronics Corp.
 */
#pragma once

#include "gmock/gmock.h"
#include "forest_common.h"
//#include "proc_element.h"
#include "forest_common_api.h"
class MockFsCtrl;
class CIntc2;

class ProcElementDummy {
public:
    ProcElementDummy(){};
    ~ProcElementDummy(){};


    void ReqIntByPeripheral (uint32_t channel){}
    void CancelIntByPeripheral (uint32_t channel){}
    ContextId GetTcLastId (void) const {return -1;}
    MockFsCtrl* GetHtPtr (void) const {return (MockFsCtrl*)1;}
    uint32_t GetPeId (void) const {return 0;}
    void SetIntc2Ptr (CIntc2* intc2) {}
    void CancelIntByMask (uint32_t channel) {}
    void UpdateIntHighPriority (ContextId tcid) {}
    bool IsIntReqInQueue (uint32_t channel) {return true;}
//    void ReqIntByIntc2 (uint32_t channel, IntReqInfoBase* intreq_info) {}
//    void CancelIntByIntc2 (uint32_t channel) {}
};



class MockProcElement: public ProcElementDummy  {
//class MockProcElement  {
public:
  MockProcElement(uint32_t pe_id, CoreType core_type, CoreVersion core_version, ArchClass arch_class){}
  ~MockProcElement(){}
  //MOCK_METHOD1(ReqIntByPeripheral, void(uint32_t));
  //MOCK_METHOD1(CancelIntByPeripheral, void(uint32_t));
  //MOCK_METHOD0(GetTcLastId, ContextId(void));
  //MOCK_METHOD0(GetHtPtr, MockFsCtrl* (void));

  //MOCK_METHOD0(GetPeId, uint32_t(void));
  //MOCK_METHOD1(SetIntc2Ptr, void (CIntc2*));
  //MOCK_METHOD0(CancelIntByMask, uint32_t(void));
  //MOCK_METHOD1(UpdateIntHighPriority, void(ContextId));
  //MOCK_METHOD1(IsIntReqInQueue, bool(uint32_t));
  //MOCK_METHOD2(ReqIntByIntc2, void(uint32_t, IntReqInfoBase*));
  //MOCK_METHOD1(CancelIntByIntc2, void(uint32_t));
};
