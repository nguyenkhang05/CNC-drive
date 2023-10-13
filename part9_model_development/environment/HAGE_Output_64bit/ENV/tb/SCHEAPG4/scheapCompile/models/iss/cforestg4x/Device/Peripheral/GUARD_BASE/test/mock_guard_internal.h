/**
 * @file mock_guard_internal.h
 *
 * @date Created on: 2016
 * @author Author: Hiroyuki Ono <hiroyuki.ono.jc@renesas.com>
 * @brief UNIT TEST for gprot.
 * @par Copyright: 2016 Renesas Electronics Corp.
 */
#include "gmock/gmock.h"
#include "treg_guard.h"

class MockGuardInternal : public GuardInternal_Interface {
public:

  MOCK_METHOD1(IsChannelEn, bool (uint32_t));
  MOCK_METHOD0(IsWriteEn, bool ());
  MOCK_METHOD0(IsGuardEn, bool ());
  MOCK_METHOD1(IsCrgIvcVmEn, bool (uint32_t));
  MOCK_METHOD1(IsCrgIvcVmLock, bool (uint32_t));
  MOCK_METHOD1(IsCrgCsgLock, bool (uint32_t));
  MOCK_METHOD1(GetCrgSpidOwner, uint32_t (uint32_t));
  MOCK_METHOD1(GetCrgSpidListValid, uint32_t (uint32_t));
  MOCK_METHOD0(GetCrgMaddrw, uint32_t ());
  MOCK_METHOD0(GetMaxCtrlChannel, uint32_t ());
  MOCK_METHOD0(GetMaxCsgChannel, uint32_t ());
  MOCK_METHOD0(GetKeyCode, uint32_t ());
  MOCK_METHOD0(GetClid, uint32_t ());


  MOCK_METHOD1(SetWriteEn, void (bool guard_write));
  MOCK_METHOD1(SetGuardEn, void (bool guard_setting));
  MOCK_METHOD2(SetChannelEn, void (uint32_t chn, bool chn_en));
  MOCK_METHOD2(SetCrgIvcVmEn, void (uint32_t chn, bool ivc_vm_set));
  MOCK_METHOD2(SetCrgIvcVmLock, void (uint32_t chn, bool ivc_vm_lock));
  MOCK_METHOD2(SetCrgCsgLock, void (uint32_t chn, bool crg_csg_lock));
  MOCK_METHOD2(SetCrgSpidOwner, void (uint32_t chn, uint32_t spid_owner));
  MOCK_METHOD2(SetCrgSpidListValid, void (uint32_t chn, uint32_t spid_list_valid));
  MOCK_METHOD0(Reset, void ());
};
