/*
 * (c) 2011,2012,2013,2017 Renesas Electronics Corporation
 * RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY
 * This program must be used solely for the purpose for which
 * it was furnished by Renesas Electronics Corporation. No part of this
 * program may be reproduced or disclosed to others, in any
 * form, without the prior written permission of Renesas Electronics
 * Corporation.
 */

#pragma once

#include "trace_info.h"
#include "gmock/gmock.h"

TraceCtrl::TraceCtrl(uint32_t pe_id) {
  // Variables to be initialized are here
  m_tr.pe_id = pe_id;
  m_tr.step_count = 0x0;
  m_tr.sim_state = SIMULATE_STOP;
  m_tr.exp_code = EXP_NONE;
  m_tr.exp_cause = EXP_CAUSE_NONE;
};
void TraceCtrl::SetExpCode(ExpCode exp_code){};
template <TraceCtrl::RegCommand cmd> void TraceCtrl::RecordSReg(CfSReg cfsrid, RegData data){};
template void TraceCtrl::RecordSReg<TraceCtrl::REG_WRITE>(CfSReg cfsrid, RegData data);
template void TraceCtrl::RecordSReg<TraceCtrl::REG_READ>(CfSReg cfsrid, RegData data);

// Mock
class MockTraceCtrl : public TraceCtrl {
public:
  MOCK_METHOD2_T(RecordSReg, void(CfSReg, RegData));
  MOCK_CONST_METHOD0(GetInstId, int32_t());
  MOCK_METHOD1(SetExpCode, void(ExpCode));
  MOCK_CONST_METHOD0(GetPeId, uint32_t(void));
  MockTraceCtrl(uint32_t pe_id) : TraceCtrl(pe_id){};
};
