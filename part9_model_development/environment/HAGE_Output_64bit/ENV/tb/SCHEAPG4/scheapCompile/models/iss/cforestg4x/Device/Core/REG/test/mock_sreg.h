/*
 * (c) 2011,2012,2013,2017 Renesas Electronics Corporation RENESAS
 * ELECTRONICS CONFIDENTIAL AND PROPRIETARY This program must be
 * used solely for the purpose for which it was furnished by
 * Renesas Electronics Corporation. No part of this program may be
 * reproduced or disclosed to others, in any form, without the;
 * prior written permission of Renesas Electronics Corporation.
 */

#pragma once

#include <string>
#include <sstream>

#include "sreg.h"
#include "sreg_vm_base.h"
#include "proc_element_base.h"
#include "gmock/gmock.h"

/*!
  @brief BMID register mock class
 */
class MockSRegBMID : public SRegBMID {

public:
  MOCK_METHOD1(SetBMID, void(RegData bmid));
  MOCK_CONST_METHOD0(GetBMID, RegData(void));

  MOCK_METHOD0(Reset, void(void));

  MOCK_METHOD3(Write, void(RegData data, PrivLevel priv, ContextId ctxt));
  MOCK_METHOD1(WriteBody, void(RegData data));
  MOCK_CONST_METHOD2(Read, RegData(PrivLevel priv, ContextId ctxt));
  MOCK_CONST_METHOD0(ReadBody, RegData(void));
  MOCK_CONST_METHOD0(GetName, std::string(void));

  MOCK_CONST_METHOD0(GetCfSRegId, CfSReg(void));

  MockSRegBMID(ContextId ctid, ProcElementBase* pe) : SRegBMID(ctid, pe) {};
};
void SRegBMID::Reset(void) {};
void SRegBMID::Write(RegData data, PrivLevel priv, ContextId ctxt) {};
RegData SRegBMID::Read(PrivLevel priv, ContextId ctxt) const { return static_cast<RegData>(0); };


/*!
  @brief BPAM register mock class
 */
class MockSRegBPAM : public SRegBPAM {

public:
  MOCK_METHOD1(SetBPAM, void(RegData bpam));
  MOCK_CONST_METHOD0(GetBPAM, RegData(void));

  MOCK_METHOD0(Reset, void(void));

  MOCK_METHOD3(Write, void(RegData data, PrivLevel priv, ContextId ctxt));
  MOCK_METHOD1(WriteBody, void(RegData data));
  MOCK_CONST_METHOD2(Read, RegData(PrivLevel priv, ContextId ctxt));
  MOCK_CONST_METHOD0(ReadBody, RegData(void));
  MOCK_CONST_METHOD0(GetName, std::string(void));

  MOCK_CONST_METHOD0(GetCfSRegId, CfSReg(void));

  MockSRegBPAM(ContextId ctid, ProcElementBase* pe) : SRegBPAM(ctid, pe) {};
};
void SRegBPAM::Write(RegData data, PrivLevel priv, ContextId ctxt) {};
RegData SRegBPAM::Read(PrivLevel priv, ContextId ctxt) const { return static_cast<RegData>(0); };


/*!
  @brief BPAM_N register mock class
 */
class MockSRegBPAM_N : public SRegBPAM_N {
  uint32_t m_id;

public:
  MOCK_METHOD1(SetBPAM, void(RegData bpam));
  MOCK_CONST_METHOD0(GetBPAM, RegData(void));

  MOCK_METHOD0(Reset, void(void));

  MOCK_METHOD3(Write, void(RegData data, PrivLevel priv, ContextId ctxt));
  MOCK_METHOD1(WriteBody, void(RegData data));
  MOCK_CONST_METHOD2(Read, RegData(PrivLevel priv, ContextId ctxt));
  MOCK_CONST_METHOD0(ReadBody, RegData(void));
  std::string GetName(void) const {
     std::ostringstream oss;
     oss << "BPAM" << m_id;
     return oss.str();
  }

  MOCK_CONST_METHOD0(GetCfSRegId, CfSReg(void));

  MockSRegBPAM_N(ContextId ctid, ProcElementBase* pe, uint32_t id) : SRegBPAM_N(ctid, pe, id) {};
};
RegData SRegBPAM_N::Read(PrivLevel priv, ContextId ctxt) const { return static_cast<RegData>(0); };


/*!
  @brief BPAV register mock class
 */
class MockSRegBPAV : public SRegBPAV {

public:
  MOCK_METHOD1(SetBPAV, void(RegData bpav));
  MOCK_CONST_METHOD0(GetBPAV, RegData(void));

  MOCK_METHOD0(Reset, void(void));

  MOCK_METHOD3(Write, void(RegData data, PrivLevel priv, ContextId ctxt));
  MOCK_METHOD1(WriteBody, void(RegData data));
  MOCK_CONST_METHOD2(Read, RegData(PrivLevel priv, ContextId ctxt));
  MOCK_CONST_METHOD0(ReadBody, RegData(void));
  MOCK_CONST_METHOD0(GetName, std::string(void));

  MOCK_CONST_METHOD0(GetCfSRegId, CfSReg(void));

  MockSRegBPAV(ContextId ctid, ProcElementBase* pe) : SRegBPAV(ctid, pe) {};
};
void SRegBPAV::Write(RegData data, PrivLevel priv, ContextId ctxt) {};
RegData SRegBPAV::Read(PrivLevel priv, ContextId ctxt) const { return static_cast<RegData>(0); };


/*!
  @brief BPAV_N register mock class
 */
class MockSRegBPAV_N : public SRegBPAV_N {
  uint32_t m_id;

public:
  MOCK_METHOD1(SetBPAV, void(RegData bpav));
  MOCK_CONST_METHOD0(GetBPAV, RegData(void));

  MOCK_METHOD0(Reset, void(void));

  MOCK_METHOD3(Write, void(RegData data, PrivLevel priv, ContextId ctxt));
  MOCK_METHOD1(WriteBody, void(RegData data));
  MOCK_CONST_METHOD2(Read, RegData(PrivLevel priv, ContextId ctxt));
  MOCK_CONST_METHOD0(ReadBody, RegData(void));
  std::string GetName(void) const {
     std::ostringstream oss;
     oss << "BPAV" << m_id;
     return oss.str();
  }

  MOCK_CONST_METHOD0(GetCfSRegId, CfSReg(void));

  MockSRegBPAV_N(ContextId ctid, ProcElementBase* pe, uint32_t id) : SRegBPAV_N(ctid, pe, id) {};
};
RegData SRegBPAV_N::Read(PrivLevel priv, ContextId ctxt) const { return static_cast<RegData>(0); };


/*!
  @brief BPC register mock class
 */
class MockSRegBPC : public SRegBPC {

public:
  MOCK_METHOD1(SetBPC, void(RegData bpc));
  MOCK_CONST_METHOD0(GetBPC, RegData(void));

  MOCK_METHOD0(Reset, void(void));

  MOCK_METHOD3(Write, void(RegData data, PrivLevel priv, ContextId ctxt));
  MOCK_METHOD1(WriteBody, void(RegData data));
  MOCK_CONST_METHOD2(Read, RegData(PrivLevel priv, ContextId ctxt));
  MOCK_CONST_METHOD0(ReadBody, RegData(void));
  MOCK_CONST_METHOD0(GetName, std::string(void));

  MOCK_CONST_METHOD0(GetCfSRegId, CfSReg(void));

  MockSRegBPC(ContextId ctid, ProcElementBase* pe) : SRegBPC(ctid, pe) {};
};
void SRegBPC::Write(RegData data, PrivLevel priv, ContextId ctxt) {};
RegData SRegBPC::Read(PrivLevel priv, ContextId ctxt) const { return static_cast<RegData>(0); };


/*!
  @brief BPC_N register mock class
 */
class MockSRegBPC_N : public SRegBPC_N {
  uint32_t m_id;

public:
  MOCK_METHOD1(SetHE, void(bool he));
  MOCK_METHOD1(SetGE, void(RegData ge));
  MOCK_METHOD1(SetTY, void(RegData ty));
  MOCK_METHOD1(SetVA, void(bool va));
  MOCK_METHOD1(SetEO, void(bool eo));
  MOCK_METHOD1(SetTE, void(bool te));
  MOCK_METHOD1(SetBE, void(bool be));
  MOCK_METHOD1(SetFE, void(bool fe));
  MOCK_METHOD1(SetWE, void(bool we));
  MOCK_METHOD1(SetRE, void(bool re));
  MOCK_CONST_METHOD0(GetHE, bool(void));
  MOCK_CONST_METHOD0(GetGE, RegData(void));
  MOCK_CONST_METHOD0(GetTY, RegData(void));
  MOCK_CONST_METHOD0(GetVA, bool(void));
  MOCK_CONST_METHOD0(GetEO, bool(void));
  MOCK_CONST_METHOD0(GetTE, bool(void));
  MOCK_CONST_METHOD0(GetBE, bool(void));
  MOCK_CONST_METHOD0(GetFE, bool(void));
  MOCK_CONST_METHOD0(GetWE, bool(void));
  MOCK_CONST_METHOD0(GetRE, bool(void));

  MOCK_METHOD0(Reset, void(void));

  MOCK_METHOD3(Write, void(RegData data, PrivLevel priv, ContextId ctxt));
  MOCK_METHOD1(WriteBody, void(RegData data));
  MOCK_CONST_METHOD2(Read, RegData(PrivLevel priv, ContextId ctxt));
  MOCK_CONST_METHOD0(ReadBody, RegData(void));
  std::string GetName(void) const {
     std::ostringstream oss;
     oss << "BPC" << m_id;
     return oss.str();
  }

  MOCK_CONST_METHOD0(GetCfSRegId, CfSReg(void));

  MockSRegBPC_N(ContextId ctid, ProcElementBase* pe, uint32_t id) : SRegBPC_N(ctid, pe, id) {};
};
void SRegBPC_N::SetTY(RegData ty) {};
void SRegBPC_N::SetWE(bool we) {};
void SRegBPC_N::SetRE(bool re) {};
void SRegBPC_N::Write(RegData data, PrivLevel priv, ContextId ctxt) {};
RegData SRegBPC_N::Read(PrivLevel priv, ContextId ctxt) const { return static_cast<RegData>(0); };


/*!
  @brief BRPACTRL register mock class
 */
class MockSRegBRPACTRL : public SRegBRPACTRL {

public:
  MOCK_METHOD1(SetSR, void(bool sr));
  MOCK_METHOD1(SetRW, void(bool rw));
  MOCK_METHOD1(SetSEL, void(RegData sel));
  MOCK_METHOD1(SetENTRY, void(RegData entry));
  MOCK_CONST_METHOD0(GetSR, bool(void));
  MOCK_CONST_METHOD0(GetRW, bool(void));
  MOCK_CONST_METHOD0(GetSEL, RegData(void));
  MOCK_CONST_METHOD0(GetENTRY, RegData(void));

  MOCK_METHOD0(Reset, void(void));

  MOCK_METHOD3(Write, void(RegData data, PrivLevel priv, ContextId ctxt));
  MOCK_METHOD1(WriteBody, void(RegData data));
  MOCK_CONST_METHOD2(Read, RegData(PrivLevel priv, ContextId ctxt));
  MOCK_CONST_METHOD0(ReadBody, RegData(void));
  MOCK_CONST_METHOD0(GetName, std::string(void));

  MOCK_CONST_METHOD0(GetCfSRegId, CfSReg(void));

  MockSRegBRPACTRL(ContextId ctid, ProcElementBase* pe) : SRegBRPACTRL(ctid, pe) {};
};
void SRegBRPACTRL::SetSR(bool sr) {};
void SRegBRPACTRL::Write(RegData data, PrivLevel priv, ContextId ctxt) {};
RegData SRegBRPACTRL::Read(PrivLevel priv, ContextId ctxt) const { return static_cast<RegData>(0); };


/*!
  @brief BRPCFG register mock class
 */
class MockSRegBRPCFG : public SRegBRPCFG {

public:
  MOCK_METHOD1(SetRASSIZE, void(RegData rassize));
  MOCK_METHOD1(SetBTACSIZE, void(RegData btacsize));
  MOCK_METHOD1(SetGBHTSIZE, void(RegData gbhtsize));
  MOCK_METHOD1(SetBHTSIZE, void(RegData bhtsize));
  MOCK_CONST_METHOD0(GetRASSIZE, RegData(void));
  MOCK_CONST_METHOD0(GetBTACSIZE, RegData(void));
  MOCK_CONST_METHOD0(GetGBHTSIZE, RegData(void));
  MOCK_CONST_METHOD0(GetBHTSIZE, RegData(void));

  MOCK_METHOD0(Reset, void(void));

  MOCK_METHOD3(Write, void(RegData data, PrivLevel priv, ContextId ctxt));
  MOCK_METHOD1(WriteBody, void(RegData data));
  MOCK_CONST_METHOD2(Read, RegData(PrivLevel priv, ContextId ctxt));
  MOCK_CONST_METHOD0(ReadBody, RegData(void));
  MOCK_CONST_METHOD0(GetName, std::string(void));

  MOCK_CONST_METHOD0(GetCfSRegId, CfSReg(void));

  MockSRegBRPCFG(ContextId ctid, ProcElementBase* pe) : SRegBRPCFG(ctid, pe) {};
};
void SRegBRPCFG::Reset(void) {};
void SRegBRPCFG::Write(RegData data, PrivLevel priv, ContextId ctxt) {};
RegData SRegBRPCFG::Read(PrivLevel priv, ContextId ctxt) const { return static_cast<RegData>(0); };


/*!
  @brief BRPCTRL0 register mock class
 */
class MockSRegBRPCTRL0 : public SRegBRPCTRL0 {

public:
  MOCK_METHOD1(SetBRPCLR, void(bool brpclr));
  MOCK_METHOD1(SetBRPEN, void(bool brpen));
  MOCK_CONST_METHOD0(GetBRPCLR, bool(void));
  MOCK_CONST_METHOD0(GetBRPEN, bool(void));

  MOCK_METHOD0(Reset, void(void));

  MOCK_METHOD3(Write, void(RegData data, PrivLevel priv, ContextId ctxt));
  MOCK_METHOD1(WriteBody, void(RegData data));
  MOCK_CONST_METHOD2(Read, RegData(PrivLevel priv, ContextId ctxt));
  MOCK_CONST_METHOD0(ReadBody, RegData(void));
  MOCK_CONST_METHOD0(GetName, std::string(void));

  MOCK_CONST_METHOD0(GetCfSRegId, CfSReg(void));

  MockSRegBRPCTRL0(ContextId ctid, ProcElementBase* pe) : SRegBRPCTRL0(ctid, pe) {};
};
void SRegBRPCTRL0::Write(RegData data, PrivLevel priv, ContextId ctxt) {};
RegData SRegBRPCTRL0::Read(PrivLevel priv, ContextId ctxt) const { return static_cast<RegData>(0); };


/*!
  @brief BRPCTRL1 register mock class
 */
class MockSRegBRPCTRL1 : public SRegBRPCTRL1 {

public:
  MOCK_METHOD1(SetBHTST, void(bool bhtst));
  MOCK_METHOD1(SetRASD, void(bool rasd));
  MOCK_METHOD1(SetBTACD, void(bool btacd));
  MOCK_METHOD1(SetGBHTD, void(bool gbhtd));
  MOCK_METHOD1(SetBHTD, void(bool bhtd));
  MOCK_CONST_METHOD0(GetBHTST, bool(void));
  MOCK_CONST_METHOD0(GetRASD, bool(void));
  MOCK_CONST_METHOD0(GetBTACD, bool(void));
  MOCK_CONST_METHOD0(GetGBHTD, bool(void));
  MOCK_CONST_METHOD0(GetBHTD, bool(void));

  MOCK_METHOD0(Reset, void(void));

  MOCK_METHOD3(Write, void(RegData data, PrivLevel priv, ContextId ctxt));
  MOCK_METHOD1(WriteBody, void(RegData data));
  MOCK_CONST_METHOD2(Read, RegData(PrivLevel priv, ContextId ctxt));
  MOCK_CONST_METHOD0(ReadBody, RegData(void));
  MOCK_CONST_METHOD0(GetName, std::string(void));

  MOCK_CONST_METHOD0(GetCfSRegId, CfSReg(void));

  MockSRegBRPCTRL1(ContextId ctid, ProcElementBase* pe) : SRegBRPCTRL1(ctid, pe) {};
};
void SRegBRPCTRL1::Write(RegData data, PrivLevel priv, ContextId ctxt) {};
RegData SRegBRPCTRL1::Read(PrivLevel priv, ContextId ctxt) const { return static_cast<RegData>(0); };


/*!
  @brief BRPDATA register mock class
 */
class MockSRegBRPDATA : public SRegBRPDATA {

public:
  MOCK_METHOD1(SetDATA, void(RegData data));
  MOCK_CONST_METHOD0(GetDATA, RegData(void));

  MOCK_METHOD0(Reset, void(void));

  MOCK_METHOD3(Write, void(RegData data, PrivLevel priv, ContextId ctxt));
  MOCK_METHOD1(WriteBody, void(RegData data));
  MOCK_CONST_METHOD2(Read, RegData(PrivLevel priv, ContextId ctxt));
  MOCK_CONST_METHOD0(ReadBody, RegData(void));
  MOCK_CONST_METHOD0(GetName, std::string(void));

  MOCK_CONST_METHOD0(GetCfSRegId, CfSReg(void));

  MockSRegBRPDATA(ContextId ctid, ProcElementBase* pe) : SRegBRPDATA(ctid, pe) {};
};
void SRegBRPDATA::Write(RegData data, PrivLevel priv, ContextId ctxt) {};
RegData SRegBRPDATA::Read(PrivLevel priv, ContextId ctxt) const { return static_cast<RegData>(0); };


/*!
  @brief CTBP register mock class
 */
class MockSRegCTBP : public SRegCTBP {

public:
  MOCK_METHOD1(SetCTBP, void(RegData ctbp));
  MOCK_CONST_METHOD0(GetCTBP, RegData(void));

  MOCK_METHOD0(Reset, void(void));

  MOCK_METHOD3(Write, void(RegData data, PrivLevel priv, ContextId ctxt));
  MOCK_METHOD1(WriteBody, void(RegData data));
  MOCK_CONST_METHOD2(Read, RegData(PrivLevel priv, ContextId ctxt));
  MOCK_CONST_METHOD0(ReadBody, RegData(void));
  MOCK_CONST_METHOD0(GetName, std::string(void));

  MOCK_CONST_METHOD0(GetCfSRegId, CfSReg(void));

  MockSRegCTBP(ContextId ctid, ProcElementBase* pe) : SRegCTBP(ctid, pe) {};
};


/*!
  @brief CTPC register mock class
 */
class MockSRegCTPC : public SRegCTPC {

public:
  MOCK_METHOD1(SetCTPC, void(RegData ctpc));
  MOCK_CONST_METHOD0(GetCTPC, RegData(void));

  MOCK_METHOD0(Reset, void(void));

  MOCK_METHOD3(Write, void(RegData data, PrivLevel priv, ContextId ctxt));
  MOCK_METHOD1(WriteBody, void(RegData data));
  MOCK_CONST_METHOD2(Read, RegData(PrivLevel priv, ContextId ctxt));
  MOCK_CONST_METHOD0(ReadBody, RegData(void));
  MOCK_CONST_METHOD0(GetName, std::string(void));

  MOCK_CONST_METHOD0(GetCfSRegId, CfSReg(void));

  MockSRegCTPC(ContextId ctid, ProcElementBase* pe) : SRegCTPC(ctid, pe) {};
};


/*!
  @brief CTPSW register mock class
 */
class MockSRegCTPSW : public SRegCTPSW {

public:
  MOCK_METHOD1(SetCTPSW, void(RegData ctpsw));
  MOCK_CONST_METHOD0(GetCTPSW, RegData(void));

  MOCK_METHOD0(Reset, void(void));

  MOCK_METHOD3(Write, void(RegData data, PrivLevel priv, ContextId ctxt));
  MOCK_METHOD1(WriteBody, void(RegData data));
  MOCK_CONST_METHOD2(Read, RegData(PrivLevel priv, ContextId ctxt));
  MOCK_CONST_METHOD0(ReadBody, RegData(void));
  MOCK_CONST_METHOD0(GetName, std::string(void));

  MOCK_CONST_METHOD0(GetCfSRegId, CfSReg(void));

  MockSRegCTPSW(ContextId ctid, ProcElementBase* pe) : SRegCTPSW(ctid, pe) {};
};


/*!
  @brief DBCFG register mock class
 */
class MockSRegDBCFG : public SRegDBCFG {

public:
  MOCK_METHOD1(SetNBP, void(RegData nbp));
  MOCK_CONST_METHOD0(GetNBP, RegData(void));

  MOCK_METHOD0(Reset, void(void));

  MOCK_METHOD3(Write, void(RegData data, PrivLevel priv, ContextId ctxt));
  MOCK_METHOD1(WriteBody, void(RegData data));
  MOCK_CONST_METHOD2(Read, RegData(PrivLevel priv, ContextId ctxt));
  MOCK_CONST_METHOD0(ReadBody, RegData(void));
  MOCK_CONST_METHOD0(GetName, std::string(void));

  MOCK_CONST_METHOD0(GetCfSRegId, CfSReg(void));

  MockSRegDBCFG(ContextId ctid, ProcElementBase* pe) : SRegDBCFG(ctid, pe) {};
};
RegData SRegDBCFG::GetNBP(void) const { return static_cast<RegData>(0); };
RegData SRegDBCFG::Read(PrivLevel priv, ContextId ctxt) const { return static_cast<RegData>(0); };


/*!
  @brief DBGEN register mock class
 */
class MockSRegDBGEN : public SRegDBGEN {

public:
  MOCK_METHOD1(SetHE, void(bool he));
  MOCK_METHOD1(SetGE, void(RegData ge));
  MOCK_CONST_METHOD0(GetHE, bool(void));
  MOCK_CONST_METHOD0(GetGE, RegData(void));

  MOCK_METHOD0(Reset, void(void));

  MOCK_METHOD3(Write, void(RegData data, PrivLevel priv, ContextId ctxt));
  MOCK_METHOD1(WriteBody, void(RegData data));
  MOCK_CONST_METHOD2(Read, RegData(PrivLevel priv, ContextId ctxt));
  MOCK_CONST_METHOD0(ReadBody, RegData(void));
  MOCK_CONST_METHOD0(GetName, std::string(void));

  MOCK_CONST_METHOD0(GetCfSRegId, CfSReg(void));

  MockSRegDBGEN(ContextId ctid, ProcElementBase* pe) : SRegDBGEN(ctid, pe) {};
};
void SRegDBGEN::Reset(void) {};


/*!
  @brief DBIC register mock class
 */
class MockSRegDBIC : public SRegDBIC {

public:
  MOCK_METHOD1(SetDBIC, void(RegData dbic));
  MOCK_CONST_METHOD0(GetDBIC, RegData(void));

  MOCK_METHOD0(Reset, void(void));

  MOCK_METHOD3(Write, void(RegData data, PrivLevel priv, ContextId ctxt));
  MOCK_METHOD1(WriteBody, void(RegData data));
  MOCK_CONST_METHOD2(Read, RegData(PrivLevel priv, ContextId ctxt));
  MOCK_CONST_METHOD0(ReadBody, RegData(void));
  MOCK_CONST_METHOD0(GetName, std::string(void));

  MOCK_CONST_METHOD0(GetCfSRegId, CfSReg(void));

  MockSRegDBIC(ContextId ctid, ProcElementBase* pe) : SRegDBIC(ctid, pe) {};
};
void SRegDBIC::Write(RegData data, PrivLevel priv, ContextId ctxt) {};
RegData SRegDBIC::Read(PrivLevel priv, ContextId ctxt) const { return static_cast<RegData>(0); };


/*!
  @brief DBPC register mock class
 */
class MockSRegDBPC : public SRegDBPC {

public:
  MOCK_METHOD1(SetDBPC, void(RegData dbpc));
  MOCK_CONST_METHOD0(GetDBPC, RegData(void));

  MOCK_METHOD0(Reset, void(void));

  MOCK_METHOD3(Write, void(RegData data, PrivLevel priv, ContextId ctxt));
  MOCK_METHOD1(WriteBody, void(RegData data));
  MOCK_CONST_METHOD2(Read, RegData(PrivLevel priv, ContextId ctxt));
  MOCK_CONST_METHOD0(ReadBody, RegData(void));
  MOCK_CONST_METHOD0(GetName, std::string(void));

  MOCK_CONST_METHOD0(GetCfSRegId, CfSReg(void));

  MockSRegDBPC(ContextId ctid, ProcElementBase* pe) : SRegDBPC(ctid, pe) {};
};
void SRegDBPC::Write(RegData data, PrivLevel priv, ContextId ctxt) {};
RegData SRegDBPC::Read(PrivLevel priv, ContextId ctxt) const { return static_cast<RegData>(0); };


/*!
  @brief DBPSW register mock class
 */
class MockSRegDBPSW : public SRegDBPSW {

public:
  MOCK_METHOD1(SetUM, void(bool um));
  MOCK_METHOD1(SetEIMASK, void(RegData eimask));
  MOCK_METHOD1(SetCU, void(RegData cu));
  MOCK_METHOD1(SetEBV, void(bool ebv));
  MOCK_METHOD1(SetNP, void(bool np));
  MOCK_METHOD1(SetEP, void(bool ep));
  MOCK_METHOD1(SetID, void(bool id));
  MOCK_METHOD1(SetSAT, void(bool sat));
  MOCK_METHOD1(SetCY, void(bool cy));
  MOCK_METHOD1(SetOV, void(bool ov));
  MOCK_METHOD1(SetS, void(bool s));
  MOCK_METHOD1(SetZ, void(bool z));
  MOCK_CONST_METHOD0(GetUM, bool(void));
  MOCK_CONST_METHOD0(GetEIMASK, RegData(void));
  MOCK_CONST_METHOD0(GetCU, RegData(void));
  MOCK_CONST_METHOD0(GetEBV, bool(void));
  MOCK_CONST_METHOD0(GetNP, bool(void));
  MOCK_CONST_METHOD0(GetEP, bool(void));
  MOCK_CONST_METHOD0(GetID, bool(void));
  MOCK_CONST_METHOD0(GetSAT, bool(void));
  MOCK_CONST_METHOD0(GetCY, bool(void));
  MOCK_CONST_METHOD0(GetOV, bool(void));
  MOCK_CONST_METHOD0(GetS, bool(void));
  MOCK_CONST_METHOD0(GetZ, bool(void));

  MOCK_METHOD0(Reset, void(void));

  MOCK_METHOD3(Write, void(RegData data, PrivLevel priv, ContextId ctxt));
  MOCK_METHOD1(WriteBody, void(RegData data));
  MOCK_CONST_METHOD2(Read, RegData(PrivLevel priv, ContextId ctxt));
  MOCK_CONST_METHOD0(ReadBody, RegData(void));
  MOCK_CONST_METHOD0(GetName, std::string(void));

  MOCK_CONST_METHOD0(GetCfSRegId, CfSReg(void));

  MockSRegDBPSW(ContextId ctid, ProcElementBase* pe) : SRegDBPSW(ctid, pe) {};
};
void SRegDBPSW::SetEIMASK(RegData eimask) {};
void SRegDBPSW::SetCU(RegData cu) {};
void SRegDBPSW::Write(RegData data, PrivLevel priv, ContextId ctxt) {};
RegData SRegDBPSW::Read(PrivLevel priv, ContextId ctxt) const { return static_cast<RegData>(0); };


/*!
  @brief DBPSWH register mock class
 */
class MockSRegDBPSWH : public SRegDBPSWH {

public:
  MOCK_METHOD1(SetGM, void(bool gm));
  MOCK_METHOD1(SetGPID, void(RegData gpid));
  MOCK_CONST_METHOD0(GetGM, bool(void));
  MOCK_CONST_METHOD0(GetGPID, RegData(void));

  MOCK_METHOD0(Reset, void(void));

  MOCK_METHOD3(Write, void(RegData data, PrivLevel priv, ContextId ctxt));
  MOCK_METHOD1(WriteBody, void(RegData data));
  MOCK_CONST_METHOD2(Read, RegData(PrivLevel priv, ContextId ctxt));
  MOCK_CONST_METHOD0(ReadBody, RegData(void));
  MOCK_CONST_METHOD0(GetName, std::string(void));

  MOCK_CONST_METHOD0(GetCfSRegId, CfSReg(void));

  MockSRegDBPSWH(ContextId ctid, ProcElementBase* pe) : SRegDBPSWH(ctid, pe) {};
};
void SRegDBPSWH::Write(RegData data, PrivLevel priv, ContextId ctxt) {};
RegData SRegDBPSWH::Read(PrivLevel priv, ContextId ctxt) const { return static_cast<RegData>(0); };


/*!
  @brief DBWR register mock class
 */
class MockSRegDBWR : public SRegDBWR {

public:
  MOCK_METHOD1(SetDBWR, void(RegData dbwr));
  MOCK_CONST_METHOD0(GetDBWR, RegData(void));

  MOCK_METHOD0(Reset, void(void));

  MOCK_METHOD3(Write, void(RegData data, PrivLevel priv, ContextId ctxt));
  MOCK_METHOD1(WriteBody, void(RegData data));
  MOCK_CONST_METHOD2(Read, RegData(PrivLevel priv, ContextId ctxt));
  MOCK_CONST_METHOD0(ReadBody, RegData(void));
  MOCK_CONST_METHOD0(GetName, std::string(void));

  MOCK_CONST_METHOD0(GetCfSRegId, CfSReg(void));

  MockSRegDBWR(ContextId ctid, ProcElementBase* pe) : SRegDBWR(ctid, pe) {};
};
void SRegDBWR::Write(RegData data, PrivLevel priv, ContextId ctxt) {};
RegData SRegDBWR::Read(PrivLevel priv, ContextId ctxt) const { return static_cast<RegData>(0); };


/*!
  @brief DCBKEY register mock class
 */
class MockSRegDCBKEY : public SRegDCBKEY {

public:
  MOCK_METHOD1(SetCBAKEY, void(RegData cbakey));
  MOCK_METHOD1(SetKEYCODE, void(RegData keycode));
  MOCK_CONST_METHOD0(GetCBAKEY, RegData(void));
  MOCK_CONST_METHOD0(GetKEYCODE, RegData(void));

  MOCK_METHOD0(Reset, void(void));

  MOCK_METHOD3(Write, void(RegData data, PrivLevel priv, ContextId ctxt));
  MOCK_METHOD1(WriteBody, void(RegData data));
  MOCK_CONST_METHOD2(Read, RegData(PrivLevel priv, ContextId ctxt));
  MOCK_CONST_METHOD0(ReadBody, RegData(void));
  MOCK_CONST_METHOD0(GetName, std::string(void));

  MOCK_CONST_METHOD0(GetCfSRegId, CfSReg(void));

  MockSRegDCBKEY(ContextId ctid, ProcElementBase* pe) : SRegDCBKEY(ctid, pe) {};
};
void SRegDCBKEY::Write(RegData data, PrivLevel priv, ContextId ctxt) {};
RegData SRegDCBKEY::Read(PrivLevel priv, ContextId ctxt) const { return static_cast<RegData>(0); };


/*!
  @brief DECFG register mock class
 */
class MockSRegDECFG : public SRegDECFG {

public:
  MOCK_METHOD1(SetEHE, void(bool ehe));
  MOCK_METHOD1(SetEGE, void(bool ege));
  MOCK_METHOD1(SetESE, void(bool ese));
  MOCK_CONST_METHOD0(GetEHE, bool(void));
  MOCK_CONST_METHOD0(GetEGE, bool(void));
  MOCK_CONST_METHOD0(GetESE, bool(void));

  MOCK_METHOD0(Reset, void(void));

  MOCK_METHOD3(Write, void(RegData data, PrivLevel priv, ContextId ctxt));
  MOCK_METHOD1(WriteBody, void(RegData data));
  MOCK_CONST_METHOD2(Read, RegData(PrivLevel priv, ContextId ctxt));
  MOCK_CONST_METHOD0(ReadBody, RegData(void));
  MOCK_CONST_METHOD0(GetName, std::string(void));

  MOCK_CONST_METHOD0(GetCfSRegId, CfSReg(void));

  MockSRegDECFG(ContextId ctid, ProcElementBase* pe) : SRegDECFG(ctid, pe) {};
};
void SRegDECFG::Write(RegData data, PrivLevel priv, ContextId ctxt) {};
RegData SRegDECFG::Read(PrivLevel priv, ContextId ctxt) const { return static_cast<RegData>(0); };


/*!
  @brief DECTRL register mock class
 */
class MockSRegDECTRL : public SRegDECTRL {

public:
  MOCK_METHOD1(SetBEVD, void(bool bevd));
  MOCK_CONST_METHOD0(GetBEVD, bool(void));

  MOCK_METHOD0(Reset, void(void));

  MOCK_METHOD3(Write, void(RegData data, PrivLevel priv, ContextId ctxt));
  MOCK_METHOD1(WriteBody, void(RegData data));
  MOCK_CONST_METHOD2(Read, RegData(PrivLevel priv, ContextId ctxt));
  MOCK_CONST_METHOD0(ReadBody, RegData(void));
  MOCK_CONST_METHOD0(GetName, std::string(void));

  MOCK_CONST_METHOD0(GetCfSRegId, CfSReg(void));

  MockSRegDECTRL(ContextId ctid, ProcElementBase* pe) : SRegDECTRL(ctid, pe) {};
};
void SRegDECTRL::SetBEVD(bool bevd) {};
bool SRegDECTRL::GetBEVD(void) const { return static_cast<bool>(0); };
void SRegDECTRL::Write(RegData data, PrivLevel priv, ContextId ctxt) {};
RegData SRegDECTRL::Read(PrivLevel priv, ContextId ctxt) const { return static_cast<RegData>(0); };


/*!
  @brief DEVDS register mock class
 */
class MockSRegDEVDS : public SRegDEVDS {

public:
  MOCK_METHOD1(SetBEVDHM, void(bool bevdhm));
  MOCK_METHOD1(SetBEVDGM7, void(bool bevdgm7));
  MOCK_METHOD1(SetBEVDGM6, void(bool bevdgm6));
  MOCK_METHOD1(SetBEVDGM5, void(bool bevdgm5));
  MOCK_METHOD1(SetBEVDGM4, void(bool bevdgm4));
  MOCK_METHOD1(SetBEVDGM3, void(bool bevdgm3));
  MOCK_METHOD1(SetBEVDGM2, void(bool bevdgm2));
  MOCK_METHOD1(SetBEVDGM1, void(bool bevdgm1));
  MOCK_METHOD1(SetBEVDGM0, void(bool bevdgm0));
  MOCK_CONST_METHOD0(GetBEVDHM, bool(void));
  MOCK_CONST_METHOD0(GetBEVDGM7, bool(void));
  MOCK_CONST_METHOD0(GetBEVDGM6, bool(void));
  MOCK_CONST_METHOD0(GetBEVDGM5, bool(void));
  MOCK_CONST_METHOD0(GetBEVDGM4, bool(void));
  MOCK_CONST_METHOD0(GetBEVDGM3, bool(void));
  MOCK_CONST_METHOD0(GetBEVDGM2, bool(void));
  MOCK_CONST_METHOD0(GetBEVDGM1, bool(void));
  MOCK_CONST_METHOD0(GetBEVDGM0, bool(void));

  MOCK_METHOD0(Reset, void(void));

  MOCK_METHOD3(Write, void(RegData data, PrivLevel priv, ContextId ctxt));
  MOCK_METHOD1(WriteBody, void(RegData data));
  MOCK_CONST_METHOD2(Read, RegData(PrivLevel priv, ContextId ctxt));
  MOCK_CONST_METHOD0(ReadBody, RegData(void));
  MOCK_CONST_METHOD0(GetName, std::string(void));

  MOCK_CONST_METHOD0(GetCfSRegId, CfSReg(void));

  MockSRegDEVDS(ContextId ctid, ProcElementBase* pe) : SRegDEVDS(ctid, pe) {};
};
void SRegDEVDS::SetBEVDHM(bool bevdhm) {};
void SRegDEVDS::SetBEVDGM7(bool bevdgm7) {};
void SRegDEVDS::SetBEVDGM6(bool bevdgm6) {};
void SRegDEVDS::SetBEVDGM5(bool bevdgm5) {};
void SRegDEVDS::SetBEVDGM4(bool bevdgm4) {};
void SRegDEVDS::SetBEVDGM3(bool bevdgm3) {};
void SRegDEVDS::SetBEVDGM2(bool bevdgm2) {};
void SRegDEVDS::SetBEVDGM1(bool bevdgm1) {};
void SRegDEVDS::SetBEVDGM0(bool bevdgm0) {};
bool SRegDEVDS::GetBEVDHM(void) const { return static_cast<bool>(0); };
bool SRegDEVDS::GetBEVDGM7(void) const { return static_cast<bool>(0); };
bool SRegDEVDS::GetBEVDGM6(void) const { return static_cast<bool>(0); };
bool SRegDEVDS::GetBEVDGM5(void) const { return static_cast<bool>(0); };
bool SRegDEVDS::GetBEVDGM4(void) const { return static_cast<bool>(0); };
bool SRegDEVDS::GetBEVDGM3(void) const { return static_cast<bool>(0); };
bool SRegDEVDS::GetBEVDGM2(void) const { return static_cast<bool>(0); };
bool SRegDEVDS::GetBEVDGM1(void) const { return static_cast<bool>(0); };
bool SRegDEVDS::GetBEVDGM0(void) const { return static_cast<bool>(0); };
void SRegDEVDS::Write(RegData data, PrivLevel priv, ContextId ctxt) {};
RegData SRegDEVDS::Read(PrivLevel priv, ContextId ctxt) const { return static_cast<RegData>(0); };


/*!
  @brief DIR0 register mock class
 */
class MockSRegDIR0 : public SRegDIR0 {

public:
  MOCK_METHOD1(SetAEE, void(bool aee));
  MOCK_METHOD1(SetAT, void(bool at));
  MOCK_METHOD1(SetDN, void(bool dn));
  MOCK_METHOD1(SetDM, void(bool dm));
  MOCK_CONST_METHOD0(GetAEE, bool(void));
  MOCK_CONST_METHOD0(GetAT, bool(void));
  MOCK_CONST_METHOD0(GetDN, bool(void));
  MOCK_CONST_METHOD0(GetDM, bool(void));

  MOCK_METHOD0(Reset, void(void));

  MOCK_METHOD3(Write, void(RegData data, PrivLevel priv, ContextId ctxt));
  MOCK_METHOD1(WriteBody, void(RegData data));
  MOCK_CONST_METHOD2(Read, RegData(PrivLevel priv, ContextId ctxt));
  MOCK_CONST_METHOD0(ReadBody, RegData(void));
  MOCK_CONST_METHOD0(GetName, std::string(void));

  MOCK_CONST_METHOD0(GetCfSRegId, CfSReg(void));

  MockSRegDIR0(ContextId ctid, ProcElementBase* pe) : SRegDIR0(ctid, pe) {};
};
void SRegDIR0::SetDN(bool dn) {};
void SRegDIR0::SetDM(bool dm) {};
void SRegDIR0::Write(RegData data, PrivLevel priv, ContextId ctxt) {};
RegData SRegDIR0::Read(PrivLevel priv, ContextId ctxt) const { return static_cast<RegData>(0); };


/*!
  @brief DIR1 register mock class
 */
class MockSRegDIR1 : public SRegDIR1 {

public:
  MOCK_METHOD1(SetBT, void(RegData bt));
  MOCK_METHOD1(SetCSL, void(RegData csl));
  MOCK_METHOD1(SetSQ1, void(bool sq1));
  MOCK_METHOD1(SetSQ0, void(bool sq0));
  MOCK_METHOD1(SetBEN, void(bool ben));
  MOCK_CONST_METHOD0(GetBT, RegData(void));
  MOCK_CONST_METHOD0(GetCSL, RegData(void));
  MOCK_CONST_METHOD0(GetSQ1, bool(void));
  MOCK_CONST_METHOD0(GetSQ0, bool(void));
  MOCK_CONST_METHOD0(GetBEN, bool(void));

  MOCK_METHOD0(Reset, void(void));

  MOCK_METHOD3(Write, void(RegData data, PrivLevel priv, ContextId ctxt));
  MOCK_METHOD1(WriteBody, void(RegData data));
  MOCK_CONST_METHOD2(Read, RegData(PrivLevel priv, ContextId ctxt));
  MOCK_CONST_METHOD0(ReadBody, RegData(void));
  MOCK_CONST_METHOD0(GetName, std::string(void));

  MOCK_CONST_METHOD0(GetCfSRegId, CfSReg(void));

  MockSRegDIR1(ContextId ctid, ProcElementBase* pe) : SRegDIR1(ctid, pe) {};
};
void SRegDIR1::Write(RegData data, PrivLevel priv, ContextId ctxt) {};
RegData SRegDIR1::Read(PrivLevel priv, ContextId ctxt) const { return static_cast<RegData>(0); };


/*!
  @brief EBASE register mock class
 */
class MockSRegHMEBASE : public SRegHMEBASE {

public:
  MOCK_METHOD1(SetEBASE, void(RegData ebase));
  MOCK_METHOD1(SetDV, void(bool dv));
  MOCK_METHOD1(SetRINT, void(bool rint));
  MOCK_CONST_METHOD0(GetEBASE, RegData(void));
  MOCK_CONST_METHOD0(GetDV, bool(void));
  MOCK_CONST_METHOD0(GetRINT, bool(void));

  MOCK_METHOD0(Reset, void(void));

  MOCK_METHOD3(Write, void(RegData data, PrivLevel priv, ContextId ctxt));
  MOCK_METHOD1(WriteBody, void(RegData data));
  MOCK_CONST_METHOD2(Read, RegData(PrivLevel priv, ContextId ctxt));
  MOCK_CONST_METHOD0(ReadBody, RegData(void));
  MOCK_CONST_METHOD0(GetName, std::string(void));

  MOCK_CONST_METHOD0(GetCfSRegId, CfSReg(void));

  MockSRegHMEBASE(ContextId ctid, ProcElementBase* pe) : SRegHMEBASE(ctid, pe) {};
};


/*!
  @brief EIIC register mock class
 */
class MockSRegHMEIIC : public SRegHMEIIC {

public:
  MOCK_METHOD1(SetEIIC, void(RegData eiic));
  MOCK_CONST_METHOD0(GetEIIC, RegData(void));

  MOCK_METHOD0(Reset, void(void));

  MOCK_METHOD3(Write, void(RegData data, PrivLevel priv, ContextId ctxt));
  MOCK_METHOD1(WriteBody, void(RegData data));
  MOCK_CONST_METHOD2(Read, RegData(PrivLevel priv, ContextId ctxt));
  MOCK_CONST_METHOD0(ReadBody, RegData(void));
  MOCK_CONST_METHOD0(GetName, std::string(void));

  MOCK_CONST_METHOD0(GetCfSRegId, CfSReg(void));

  MockSRegHMEIIC(ContextId ctid, ProcElementBase* pe) : SRegHMEIIC(ctid, pe) {};
};


/*!
  @brief EIPC register mock class
 */
class MockSRegHMEIPC : public SRegHMEIPC {

public:
  MOCK_METHOD1(SetEIPC, void(RegData eipc));
  MOCK_CONST_METHOD0(GetEIPC, RegData(void));

  MOCK_METHOD0(Reset, void(void));

  MOCK_METHOD3(Write, void(RegData data, PrivLevel priv, ContextId ctxt));
  MOCK_METHOD1(WriteBody, void(RegData data));
  MOCK_CONST_METHOD2(Read, RegData(PrivLevel priv, ContextId ctxt));
  MOCK_CONST_METHOD0(ReadBody, RegData(void));
  MOCK_CONST_METHOD0(GetName, std::string(void));

  MOCK_CONST_METHOD0(GetCfSRegId, CfSReg(void));

  MockSRegHMEIPC(ContextId ctid, ProcElementBase* pe) : SRegHMEIPC(ctid, pe) {};
};


/*!
  @brief EIPSW register mock class
 */
class MockSRegHMEIPSW : public SRegHMEIPSW {

public:
  MOCK_METHOD1(SetUM, void(bool um));
  MOCK_METHOD1(SetEIMASK, void(RegData eimask));
  MOCK_METHOD1(SetCU, void(RegData cu));
  MOCK_METHOD1(SetEBV, void(bool ebv));
  MOCK_METHOD1(SetNP, void(bool np));
  MOCK_METHOD1(SetEP, void(bool ep));
  MOCK_METHOD1(SetID, void(bool id));
  MOCK_METHOD1(SetSAT, void(bool sat));
  MOCK_METHOD1(SetCY, void(bool cy));
  MOCK_METHOD1(SetOV, void(bool ov));
  MOCK_METHOD1(SetS, void(bool s));
  MOCK_METHOD1(SetZ, void(bool z));
  MOCK_CONST_METHOD0(GetUM, bool(void));
  MOCK_CONST_METHOD0(GetEIMASK, RegData(void));
  MOCK_CONST_METHOD0(GetCU, RegData(void));
  MOCK_CONST_METHOD0(GetEBV, bool(void));
  MOCK_CONST_METHOD0(GetNP, bool(void));
  MOCK_CONST_METHOD0(GetEP, bool(void));
  MOCK_CONST_METHOD0(GetID, bool(void));
  MOCK_CONST_METHOD0(GetSAT, bool(void));
  MOCK_CONST_METHOD0(GetCY, bool(void));
  MOCK_CONST_METHOD0(GetOV, bool(void));
  MOCK_CONST_METHOD0(GetS, bool(void));
  MOCK_CONST_METHOD0(GetZ, bool(void));

  MOCK_METHOD0(Reset, void(void));

  MOCK_METHOD3(Write, void(RegData data, PrivLevel priv, ContextId ctxt));
  MOCK_METHOD1(WriteBody, void(RegData data));
  MOCK_CONST_METHOD2(Read, RegData(PrivLevel priv, ContextId ctxt));
  MOCK_CONST_METHOD0(ReadBody, RegData(void));
  MOCK_CONST_METHOD0(GetName, std::string(void));

  MOCK_CONST_METHOD0(GetCfSRegId, CfSReg(void));

  MockSRegHMEIPSW(ContextId ctid, ProcElementBase* pe) : SRegHMEIPSW(ctid, pe) {};
};
void SRegHMEIPSW::SetEIMASK(RegData eimask) {};
void SRegHMEIPSW::SetCU(RegData cu) {};
void SRegHMEIPSW::Write(RegData data, PrivLevel priv, ContextId ctxt) {};
RegData SRegHMEIPSW::Read(PrivLevel priv, ContextId ctxt) const { return static_cast<RegData>(0); };


/*!
  @brief EIPSWH register mock class
 */
class MockSRegEIPSWH : public SRegEIPSWH {

public:
  MOCK_METHOD1(SetGM, void(bool gm));
  MOCK_METHOD1(SetGPID, void(RegData gpid));
  MOCK_CONST_METHOD0(GetGM, bool(void));
  MOCK_CONST_METHOD0(GetGPID, RegData(void));

  MOCK_METHOD0(Reset, void(void));

  MOCK_METHOD3(Write, void(RegData data, PrivLevel priv, ContextId ctxt));
  MOCK_METHOD1(WriteBody, void(RegData data));
  MOCK_CONST_METHOD2(Read, RegData(PrivLevel priv, ContextId ctxt));
  MOCK_CONST_METHOD0(ReadBody, RegData(void));
  MOCK_CONST_METHOD0(GetName, std::string(void));

  MOCK_CONST_METHOD0(GetCfSRegId, CfSReg(void));

  MockSRegEIPSWH(ContextId ctid, ProcElementBase* pe) : SRegEIPSWH(ctid, pe) {};
};


/*!
  @brief EIWR register mock class
 */
class MockSRegHMEIWR : public SRegHMEIWR {

public:
  MOCK_METHOD1(SetEIWR, void(RegData eiwr));
  MOCK_CONST_METHOD0(GetEIWR, RegData(void));

  MOCK_METHOD0(Reset, void(void));

  MOCK_METHOD3(Write, void(RegData data, PrivLevel priv, ContextId ctxt));
  MOCK_METHOD1(WriteBody, void(RegData data));
  MOCK_CONST_METHOD2(Read, RegData(PrivLevel priv, ContextId ctxt));
  MOCK_CONST_METHOD0(ReadBody, RegData(void));
  MOCK_CONST_METHOD0(GetName, std::string(void));

  MOCK_CONST_METHOD0(GetCfSRegId, CfSReg(void));

  MockSRegHMEIWR(ContextId ctid, ProcElementBase* pe) : SRegHMEIWR(ctid, pe) {};
};


/*!
  @brief FEIC register mock class
 */
class MockSRegHMFEIC : public SRegHMFEIC {

public:
  MOCK_METHOD1(SetFEIC, void(RegData feic));
  MOCK_CONST_METHOD0(GetFEIC, RegData(void));

  MOCK_METHOD0(Reset, void(void));

  MOCK_METHOD3(Write, void(RegData data, PrivLevel priv, ContextId ctxt));
  MOCK_METHOD1(WriteBody, void(RegData data));
  MOCK_CONST_METHOD2(Read, RegData(PrivLevel priv, ContextId ctxt));
  MOCK_CONST_METHOD0(ReadBody, RegData(void));
  MOCK_CONST_METHOD0(GetName, std::string(void));

  MOCK_CONST_METHOD0(GetCfSRegId, CfSReg(void));

  MockSRegHMFEIC(ContextId ctid, ProcElementBase* pe) : SRegHMFEIC(ctid, pe) {};
};


/*!
  @brief FEPC register mock class
 */
class MockSRegHMFEPC : public SRegHMFEPC {

public:
  MOCK_METHOD1(SetFEPC, void(RegData fepc));
  MOCK_CONST_METHOD0(GetFEPC, RegData(void));

  MOCK_METHOD0(Reset, void(void));

  MOCK_METHOD3(Write, void(RegData data, PrivLevel priv, ContextId ctxt));
  MOCK_METHOD1(WriteBody, void(RegData data));
  MOCK_CONST_METHOD2(Read, RegData(PrivLevel priv, ContextId ctxt));
  MOCK_CONST_METHOD0(ReadBody, RegData(void));
  MOCK_CONST_METHOD0(GetName, std::string(void));

  MOCK_CONST_METHOD0(GetCfSRegId, CfSReg(void));

  MockSRegHMFEPC(ContextId ctid, ProcElementBase* pe) : SRegHMFEPC(ctid, pe) {};
};
void SRegHMFEPC::Reset(void) {};


/*!
  @brief FEPSW register mock class
 */
class MockSRegHMFEPSW : public SRegHMFEPSW {

public:
  MOCK_METHOD1(SetUM, void(bool um));
  MOCK_METHOD1(SetEIMASK, void(RegData eimask));
  MOCK_METHOD1(SetCU, void(RegData cu));
  MOCK_METHOD1(SetEBV, void(bool ebv));
  MOCK_METHOD1(SetNP, void(bool np));
  MOCK_METHOD1(SetEP, void(bool ep));
  MOCK_METHOD1(SetID, void(bool id));
  MOCK_METHOD1(SetSAT, void(bool sat));
  MOCK_METHOD1(SetCY, void(bool cy));
  MOCK_METHOD1(SetOV, void(bool ov));
  MOCK_METHOD1(SetS, void(bool s));
  MOCK_METHOD1(SetZ, void(bool z));
  MOCK_CONST_METHOD0(GetUM, bool(void));
  MOCK_CONST_METHOD0(GetEIMASK, RegData(void));
  MOCK_CONST_METHOD0(GetCU, RegData(void));
  MOCK_CONST_METHOD0(GetEBV, bool(void));
  MOCK_CONST_METHOD0(GetNP, bool(void));
  MOCK_CONST_METHOD0(GetEP, bool(void));
  MOCK_CONST_METHOD0(GetID, bool(void));
  MOCK_CONST_METHOD0(GetSAT, bool(void));
  MOCK_CONST_METHOD0(GetCY, bool(void));
  MOCK_CONST_METHOD0(GetOV, bool(void));
  MOCK_CONST_METHOD0(GetS, bool(void));
  MOCK_CONST_METHOD0(GetZ, bool(void));

  MOCK_METHOD0(Reset, void(void));

  MOCK_METHOD3(Write, void(RegData data, PrivLevel priv, ContextId ctxt));
  MOCK_METHOD1(WriteBody, void(RegData data));
  MOCK_CONST_METHOD2(Read, RegData(PrivLevel priv, ContextId ctxt));
  MOCK_CONST_METHOD0(ReadBody, RegData(void));
  MOCK_CONST_METHOD0(GetName, std::string(void));

  MOCK_CONST_METHOD0(GetCfSRegId, CfSReg(void));

  MockSRegHMFEPSW(ContextId ctid, ProcElementBase* pe) : SRegHMFEPSW(ctid, pe) {};
};
void SRegHMFEPSW::SetEIMASK(RegData eimask) {};
void SRegHMFEPSW::SetCU(RegData cu) {};
void SRegHMFEPSW::Write(RegData data, PrivLevel priv, ContextId ctxt) {};
RegData SRegHMFEPSW::Read(PrivLevel priv, ContextId ctxt) const { return static_cast<RegData>(0); };


/*!
  @brief FEPSWH register mock class
 */
class MockSRegFEPSWH : public SRegFEPSWH {

public:
  MOCK_METHOD1(SetGM, void(bool gm));
  MOCK_METHOD1(SetGPID, void(RegData gpid));
  MOCK_CONST_METHOD0(GetGM, bool(void));
  MOCK_CONST_METHOD0(GetGPID, RegData(void));

  MOCK_METHOD0(Reset, void(void));

  MOCK_METHOD3(Write, void(RegData data, PrivLevel priv, ContextId ctxt));
  MOCK_METHOD1(WriteBody, void(RegData data));
  MOCK_CONST_METHOD2(Read, RegData(PrivLevel priv, ContextId ctxt));
  MOCK_CONST_METHOD0(ReadBody, RegData(void));
  MOCK_CONST_METHOD0(GetName, std::string(void));

  MOCK_CONST_METHOD0(GetCfSRegId, CfSReg(void));

  MockSRegFEPSWH(ContextId ctid, ProcElementBase* pe) : SRegFEPSWH(ctid, pe) {};
};


/*!
  @brief FEWR register mock class
 */
class MockSRegHMFEWR : public SRegHMFEWR {

public:
  MOCK_METHOD1(SetFEWR, void(RegData fewr));
  MOCK_CONST_METHOD0(GetFEWR, RegData(void));

  MOCK_METHOD0(Reset, void(void));

  MOCK_METHOD3(Write, void(RegData data, PrivLevel priv, ContextId ctxt));
  MOCK_METHOD1(WriteBody, void(RegData data));
  MOCK_CONST_METHOD2(Read, RegData(PrivLevel priv, ContextId ctxt));
  MOCK_CONST_METHOD0(ReadBody, RegData(void));
  MOCK_CONST_METHOD0(GetName, std::string(void));

  MOCK_CONST_METHOD0(GetCfSRegId, CfSReg(void));

  MockSRegHMFEWR(ContextId ctid, ProcElementBase* pe) : SRegHMFEWR(ctid, pe) {};
};


/*!
  @brief FPCC register mock class
 */
class MockSRegFPCC : public SRegFPCC {

public:
  MOCK_METHOD1(SetCC, void(RegData cc));
  MOCK_CONST_METHOD0(GetCC, RegData(void));

  MOCK_METHOD0(Reset, void(void));

  MOCK_METHOD3(Write, void(RegData data, PrivLevel priv, ContextId ctxt));
  MOCK_METHOD1(WriteBody, void(RegData data));
  MOCK_CONST_METHOD2(Read, RegData(PrivLevel priv, ContextId ctxt));
  MOCK_CONST_METHOD0(ReadBody, RegData(void));
  MOCK_METHOD0(GetCoproType, CoproType(void));
  MOCK_CONST_METHOD0(GetName, std::string(void));

  MOCK_CONST_METHOD0(GetCfSRegId, CfSReg(void));

  MockSRegFPCC(ContextId ctid, ProcElementBase* pe) : SRegFPCC(ctid, pe) {};
};
void SRegFPCC::SetCC(RegData cc) {};
RegData SRegFPCC::GetCC(void) const { return static_cast<RegData>(0); };


/*!
  @brief FPCFG register mock class
 */
class MockSRegFPCFG : public SRegFPCFG {

public:
  MOCK_METHOD1(SetRM, void(RegData rm));
  MOCK_METHOD1(SetXE, void(RegData xe));
  MOCK_CONST_METHOD0(GetRM, RegData(void));
  MOCK_CONST_METHOD0(GetXE, RegData(void));

  MOCK_METHOD0(Reset, void(void));

  MOCK_METHOD3(Write, void(RegData data, PrivLevel priv, ContextId ctxt));
  MOCK_METHOD1(WriteBody, void(RegData data));
  MOCK_CONST_METHOD2(Read, RegData(PrivLevel priv, ContextId ctxt));
  MOCK_CONST_METHOD0(ReadBody, RegData(void));
  MOCK_METHOD0(GetCoproType, CoproType(void));
  MOCK_CONST_METHOD0(GetName, std::string(void));

  MOCK_CONST_METHOD0(GetCfSRegId, CfSReg(void));

  MockSRegFPCFG(ContextId ctid, ProcElementBase* pe) : SRegFPCFG(ctid, pe) {};
};
void SRegFPCFG::SetRM(RegData rm) {};
void SRegFPCFG::SetXE(RegData xe) {};
RegData SRegFPCFG::GetRM(void) const { return static_cast<RegData>(0); };
RegData SRegFPCFG::GetXE(void) const { return static_cast<RegData>(0); };


/*!
  @brief FPEC register mock class
 */
class MockSRegFPEC : public SRegFPEC {

public:
  MOCK_METHOD1(SetFPIVD, void(bool fpivd));
  MOCK_CONST_METHOD0(GetFPIVD, bool(void));

  MOCK_METHOD0(Reset, void(void));

  MOCK_METHOD3(Write, void(RegData data, PrivLevel priv, ContextId ctxt));
  MOCK_METHOD1(WriteBody, void(RegData data));
  MOCK_CONST_METHOD2(Read, RegData(PrivLevel priv, ContextId ctxt));
  MOCK_CONST_METHOD0(ReadBody, RegData(void));
  MOCK_METHOD0(GetCoproType, CoproType(void));
  MOCK_CONST_METHOD0(GetName, std::string(void));

  MOCK_CONST_METHOD0(GetCfSRegId, CfSReg(void));

  MockSRegFPEC(ContextId ctid, ProcElementBase* pe) : SRegFPEC(ctid, pe) {};
};
void SRegFPEC::SetFPIVD(bool fpivd) {};
void SRegFPEC::Write(RegData data, PrivLevel priv, ContextId ctxt) {};


/*!
  @brief FPEPC register mock class
 */
class MockSRegFPEPC : public SRegFPEPC {

public:
  MOCK_METHOD1(SetFPEPC, void(RegData fpepc));
  MOCK_CONST_METHOD0(GetFPEPC, RegData(void));

  MOCK_METHOD0(Reset, void(void));

  MOCK_METHOD3(Write, void(RegData data, PrivLevel priv, ContextId ctxt));
  MOCK_METHOD1(WriteBody, void(RegData data));
  MOCK_CONST_METHOD2(Read, RegData(PrivLevel priv, ContextId ctxt));
  MOCK_CONST_METHOD0(ReadBody, RegData(void));
  MOCK_METHOD0(GetCoproType, CoproType(void));
  MOCK_CONST_METHOD0(GetName, std::string(void));

  MOCK_CONST_METHOD0(GetCfSRegId, CfSReg(void));

  MockSRegFPEPC(ContextId ctid, ProcElementBase* pe) : SRegFPEPC(ctid, pe) {};
};


/*!
  @brief FPIPR register mock class
 */
class MockSRegFPIPR : public SRegFPIPR {

public:
  MOCK_METHOD1(SetFPIPR, void(RegData fpipr));
  MOCK_CONST_METHOD0(GetFPIPR, RegData(void));

  MOCK_METHOD0(Reset, void(void));

  MOCK_METHOD3(Write, void(RegData data, PrivLevel priv, ContextId ctxt));
  MOCK_METHOD1(WriteBody, void(RegData data));
  MOCK_CONST_METHOD2(Read, RegData(PrivLevel priv, ContextId ctxt));
  MOCK_CONST_METHOD0(ReadBody, RegData(void));
  MOCK_CONST_METHOD0(GetName, std::string(void));

  MOCK_CONST_METHOD0(GetCfSRegId, CfSReg(void));

  MockSRegFPIPR(ContextId ctid, ProcElementBase* pe) : SRegFPIPR(ctid, pe) {};
};
void SRegFPIPR::SetFPIPR(RegData fpipr) {};


/*!
  @brief FPSR register mock class
 */
class MockSRegFPSR : public SRegFPSR {

public:
  MOCK_METHOD1(SetCC, void(RegData cc));
  MOCK_METHOD1(SetFN, void(bool fn));
  MOCK_METHOD1(SetIF_, void(bool if_));
  MOCK_METHOD1(SetPEM, void(bool pem));
  MOCK_METHOD1(SetRM, void(RegData rm));
  MOCK_METHOD1(SetFS, void(bool fs));
  MOCK_METHOD1(SetXC, void(RegData xc));
  MOCK_METHOD1(SetXE, void(RegData xe));
  MOCK_METHOD1(SetXP, void(RegData xp));
  MOCK_CONST_METHOD0(GetCC, RegData(void));
  MOCK_CONST_METHOD0(GetFN, bool(void));
  MOCK_CONST_METHOD0(GetIF_, bool(void));
  MOCK_CONST_METHOD0(GetPEM, bool(void));
  MOCK_CONST_METHOD0(GetRM, RegData(void));
  MOCK_CONST_METHOD0(GetFS, bool(void));
  MOCK_CONST_METHOD0(GetXC, RegData(void));
  MOCK_CONST_METHOD0(GetXE, RegData(void));
  MOCK_CONST_METHOD0(GetXP, RegData(void));

  MOCK_METHOD0(Reset, void(void));

  MOCK_METHOD3(Write, void(RegData data, PrivLevel priv, ContextId ctxt));
  MOCK_METHOD1(WriteBody, void(RegData data));
  MOCK_CONST_METHOD2(Read, RegData(PrivLevel priv, ContextId ctxt));
  MOCK_CONST_METHOD0(ReadBody, RegData(void));
  MOCK_METHOD0(GetCoproType, CoproType(void));
  MOCK_CONST_METHOD0(GetName, std::string(void));

  MOCK_CONST_METHOD0(GetCfSRegId, CfSReg(void));

  MockSRegFPSR(ContextId ctid, ProcElementBase* pe) : SRegFPSR(ctid, pe) {};
};
void SRegFPSR::SetPEM(bool pem) {};
void SRegFPSR::Reset(void) {};


/*!
  @brief FPST register mock class
 */
class MockSRegFPST : public SRegFPST {

public:
  MOCK_METHOD1(SetXC, void(RegData xc));
  MOCK_METHOD1(SetIF_, void(bool if_));
  MOCK_METHOD1(SetXP, void(RegData xp));
  MOCK_CONST_METHOD0(GetXC, RegData(void));
  MOCK_CONST_METHOD0(GetIF_, bool(void));
  MOCK_CONST_METHOD0(GetXP, RegData(void));

  MOCK_METHOD0(Reset, void(void));

  MOCK_METHOD3(Write, void(RegData data, PrivLevel priv, ContextId ctxt));
  MOCK_METHOD1(WriteBody, void(RegData data));
  MOCK_CONST_METHOD2(Read, RegData(PrivLevel priv, ContextId ctxt));
  MOCK_CONST_METHOD0(ReadBody, RegData(void));
  MOCK_METHOD0(GetCoproType, CoproType(void));
  MOCK_CONST_METHOD0(GetName, std::string(void));

  MOCK_CONST_METHOD0(GetCfSRegId, CfSReg(void));

  MockSRegFPST(ContextId ctid, ProcElementBase* pe) : SRegFPST(ctid, pe) {};
};
void SRegFPST::SetXC(RegData xc) {};
void SRegFPST::SetIF_(bool if_) {};
void SRegFPST::SetXP(RegData xp) {};
RegData SRegFPST::GetXC(void) const { return static_cast<RegData>(0); };
bool SRegFPST::GetIF_(void) const { return static_cast<bool>(0); };
RegData SRegFPST::GetXP(void) const { return static_cast<RegData>(0); };


/*!
  @brief FXCFG register mock class
 */
class MockSRegFXCFG : public SRegFXCFG {

public:
  MOCK_METHOD1(SetRM, void(RegData rm));
  MOCK_METHOD1(SetXE, void(RegData xe));
  MOCK_CONST_METHOD0(GetRM, RegData(void));
  MOCK_CONST_METHOD0(GetXE, RegData(void));

  MOCK_METHOD0(Reset, void(void));

  MOCK_METHOD3(Write, void(RegData data, PrivLevel priv, ContextId ctxt));
  MOCK_METHOD1(WriteBody, void(RegData data));
  MOCK_CONST_METHOD2(Read, RegData(PrivLevel priv, ContextId ctxt));
  MOCK_CONST_METHOD0(ReadBody, RegData(void));
  MOCK_METHOD0(GetCoproType, CoproType(void));
  MOCK_CONST_METHOD0(GetName, std::string(void));

  MOCK_CONST_METHOD0(GetCfSRegId, CfSReg(void));

  MockSRegFXCFG(ContextId ctid, ProcElementBase* pe) : SRegFXCFG(ctid, pe) {};
};
void SRegFXCFG::SetRM(RegData rm) {};
void SRegFXCFG::SetXE(RegData xe) {};
RegData SRegFXCFG::GetRM(void) const { return static_cast<RegData>(0); };
RegData SRegFXCFG::GetXE(void) const { return static_cast<RegData>(0); };


/*!
  @brief FXINFO register mock class
 */
class MockSRegFXINFO : public SRegFXINFO {

public:
  MOCK_METHOD1(SetNFPU, void(bool nfpu));
  MOCK_METHOD1(SetRSIZE, void(bool rsize));
  MOCK_CONST_METHOD0(GetNFPU, bool(void));
  MOCK_CONST_METHOD0(GetRSIZE, bool(void));

  MOCK_METHOD0(Reset, void(void));

  MOCK_METHOD3(Write, void(RegData data, PrivLevel priv, ContextId ctxt));
  MOCK_METHOD1(WriteBody, void(RegData data));
  MOCK_CONST_METHOD2(Read, RegData(PrivLevel priv, ContextId ctxt));
  MOCK_CONST_METHOD0(ReadBody, RegData(void));
  MOCK_METHOD0(GetCoproType, CoproType(void));
  MOCK_CONST_METHOD0(GetName, std::string(void));

  MOCK_CONST_METHOD0(GetCfSRegId, CfSReg(void));

  MockSRegFXINFO(ContextId ctid, ProcElementBase* pe) : SRegFXINFO(ctid, pe) {};
};


/*!
  @brief FXSR register mock class
 */
class MockSRegFXSR : public SRegFXSR {

public:
  MOCK_METHOD1(SetFN, void(bool fn));
  MOCK_METHOD1(SetIF_, void(bool if_));
  MOCK_METHOD1(SetPEM, void(bool pem));
  MOCK_METHOD1(SetRM, void(RegData rm));
  MOCK_METHOD1(SetFS, void(bool fs));
  MOCK_METHOD1(SetXC, void(RegData xc));
  MOCK_METHOD1(SetXE, void(RegData xe));
  MOCK_METHOD1(SetXP, void(RegData xp));
  MOCK_CONST_METHOD0(GetFN, bool(void));
  MOCK_CONST_METHOD0(GetIF_, bool(void));
  MOCK_CONST_METHOD0(GetPEM, bool(void));
  MOCK_CONST_METHOD0(GetRM, RegData(void));
  MOCK_CONST_METHOD0(GetFS, bool(void));
  MOCK_CONST_METHOD0(GetXC, RegData(void));
  MOCK_CONST_METHOD0(GetXE, RegData(void));
  MOCK_CONST_METHOD0(GetXP, RegData(void));

  MOCK_METHOD0(Reset, void(void));

  MOCK_METHOD3(Write, void(RegData data, PrivLevel priv, ContextId ctxt));
  MOCK_METHOD1(WriteBody, void(RegData data));
  MOCK_CONST_METHOD2(Read, RegData(PrivLevel priv, ContextId ctxt));
  MOCK_CONST_METHOD0(ReadBody, RegData(void));
  MOCK_METHOD0(GetCoproType, CoproType(void));
  MOCK_CONST_METHOD0(GetName, std::string(void));

  MOCK_CONST_METHOD0(GetCfSRegId, CfSReg(void));

  MockSRegFXSR(ContextId ctid, ProcElementBase* pe) : SRegFXSR(ctid, pe) {};
};


/*!
  @brief FXST register mock class
 */
class MockSRegFXST : public SRegFXST {

public:
  MOCK_METHOD1(SetXC, void(RegData xc));
  MOCK_METHOD1(SetIF_, void(bool if_));
  MOCK_METHOD1(SetXP, void(RegData xp));
  MOCK_CONST_METHOD0(GetXC, RegData(void));
  MOCK_CONST_METHOD0(GetIF_, bool(void));
  MOCK_CONST_METHOD0(GetXP, RegData(void));

  MOCK_METHOD0(Reset, void(void));

  MOCK_METHOD3(Write, void(RegData data, PrivLevel priv, ContextId ctxt));
  MOCK_METHOD1(WriteBody, void(RegData data));
  MOCK_CONST_METHOD2(Read, RegData(PrivLevel priv, ContextId ctxt));
  MOCK_CONST_METHOD0(ReadBody, RegData(void));
  MOCK_METHOD0(GetCoproType, CoproType(void));
  MOCK_CONST_METHOD0(GetName, std::string(void));

  MOCK_CONST_METHOD0(GetCfSRegId, CfSReg(void));

  MockSRegFXST(ContextId ctid, ProcElementBase* pe) : SRegFXST(ctid, pe) {};
};
void SRegFXST::SetXC(RegData xc) {};
void SRegFXST::SetIF_(bool if_) {};
void SRegFXST::SetXP(RegData xp) {};
RegData SRegFXST::GetXC(void) const { return static_cast<RegData>(0); };
bool SRegFXST::GetIF_(void) const { return static_cast<bool>(0); };
RegData SRegFXST::GetXP(void) const { return static_cast<RegData>(0); };


/*!
  @brief FXXC register mock class
 */
class MockSRegFXXC : public SRegFXXC {

public:
  MOCK_METHOD1(SetXC3, void(RegData xc3));
  MOCK_METHOD1(SetXC2, void(RegData xc2));
  MOCK_METHOD1(SetXC1, void(RegData xc1));
  MOCK_METHOD1(SetXC0, void(RegData xc0));
  MOCK_CONST_METHOD0(GetXC3, RegData(void));
  MOCK_CONST_METHOD0(GetXC2, RegData(void));
  MOCK_CONST_METHOD0(GetXC1, RegData(void));
  MOCK_CONST_METHOD0(GetXC0, RegData(void));

  MOCK_METHOD0(Reset, void(void));

  MOCK_METHOD3(Write, void(RegData data, PrivLevel priv, ContextId ctxt));
  MOCK_METHOD1(WriteBody, void(RegData data));
  MOCK_CONST_METHOD2(Read, RegData(PrivLevel priv, ContextId ctxt));
  MOCK_CONST_METHOD0(ReadBody, RegData(void));
  MOCK_METHOD0(GetCoproType, CoproType(void));
  MOCK_CONST_METHOD0(GetName, std::string(void));

  MOCK_CONST_METHOD0(GetCfSRegId, CfSReg(void));

  MockSRegFXXC(ContextId ctid, ProcElementBase* pe) : SRegFXXC(ctid, pe) {};
};


/*!
  @brief FXXP register mock class
 */
class MockSRegFXXP : public SRegFXXP {

public:
  MOCK_METHOD1(SetXP3, void(RegData xp3));
  MOCK_METHOD1(SetXP2, void(RegData xp2));
  MOCK_METHOD1(SetXP1, void(RegData xp1));
  MOCK_METHOD1(SetXP0, void(RegData xp0));
  MOCK_CONST_METHOD0(GetXP3, RegData(void));
  MOCK_CONST_METHOD0(GetXP2, RegData(void));
  MOCK_CONST_METHOD0(GetXP1, RegData(void));
  MOCK_CONST_METHOD0(GetXP0, RegData(void));

  MOCK_METHOD0(Reset, void(void));

  MOCK_METHOD3(Write, void(RegData data, PrivLevel priv, ContextId ctxt));
  MOCK_METHOD1(WriteBody, void(RegData data));
  MOCK_CONST_METHOD2(Read, RegData(PrivLevel priv, ContextId ctxt));
  MOCK_CONST_METHOD0(ReadBody, RegData(void));
  MOCK_METHOD0(GetCoproType, CoproType(void));
  MOCK_CONST_METHOD0(GetName, std::string(void));

  MOCK_CONST_METHOD0(GetCfSRegId, CfSReg(void));

  MockSRegFXXP(ContextId ctid, ProcElementBase* pe) : SRegFXXP(ctid, pe) {};
};


/*!
  @brief GMCFG register mock class
 */
class MockSRegGMCFG : public SRegGMCFG {

public:
  MOCK_METHOD1(SetGCU2, void(bool gcu2));
  MOCK_METHOD1(SetGCU1, void(bool gcu1));
  MOCK_METHOD1(SetGCU0, void(bool gcu0));
  MOCK_METHOD1(SetGSYSE, void(bool gsyse));
  MOCK_METHOD1(SetHMP, void(bool hmp));
  MOCK_METHOD1(SetGMP, void(bool gmp));
  MOCK_CONST_METHOD0(GetGCU2, bool(void));
  MOCK_CONST_METHOD0(GetGCU1, bool(void));
  MOCK_CONST_METHOD0(GetGCU0, bool(void));
  MOCK_CONST_METHOD0(GetGSYSE, bool(void));
  MOCK_CONST_METHOD0(GetHMP, bool(void));
  MOCK_CONST_METHOD0(GetGMP, bool(void));

  MOCK_METHOD0(Reset, void(void));

  MOCK_METHOD3(Write, void(RegData data, PrivLevel priv, ContextId ctxt));
  MOCK_METHOD1(WriteBody, void(RegData data));
  MOCK_CONST_METHOD2(Read, RegData(PrivLevel priv, ContextId ctxt));
  MOCK_CONST_METHOD0(ReadBody, RegData(void));
  MOCK_CONST_METHOD0(GetName, std::string(void));

  MOCK_CONST_METHOD0(GetCfSRegId, CfSReg(void));

  MockSRegGMCFG(ContextId ctid, ProcElementBase* pe) : SRegGMCFG(ctid, pe) {};
};
void SRegGMCFG::SetGCU1(bool gcu1) {};
void SRegGMCFG::SetGCU0(bool gcu0) {};


/*!
  @brief GMEBASE register mock class
 */
class MockSRegGMEBASE : public SRegGMEBASE {

public:
  MOCK_METHOD1(SetEBASE, void(RegData ebase));
  MOCK_METHOD1(SetDV, void(bool dv));
  MOCK_METHOD1(SetRINT, void(bool rint));
  MOCK_CONST_METHOD0(GetEBASE, RegData(void));
  MOCK_CONST_METHOD0(GetDV, bool(void));
  MOCK_CONST_METHOD0(GetRINT, bool(void));

  MOCK_METHOD0(Reset, void(void));

  MOCK_METHOD3(Write, void(RegData data, PrivLevel priv, ContextId ctxt));
  MOCK_METHOD1(WriteBody, void(RegData data));
  MOCK_CONST_METHOD2(Read, RegData(PrivLevel priv, ContextId ctxt));
  MOCK_CONST_METHOD0(ReadBody, RegData(void));
  MOCK_CONST_METHOD0(GetName, std::string(void));

  MOCK_CONST_METHOD0(GetCfSRegId, CfSReg(void));

  MockSRegGMEBASE(ContextId ctid, ProcElementBase* pe) : SRegGMEBASE(ctid, pe) {};
};


/*!
  @brief GMEIIC register mock class
 */
class MockSRegGMEIIC : public SRegGMEIIC {

public:
  MOCK_METHOD1(SetEIIC, void(RegData eiic));
  MOCK_CONST_METHOD0(GetEIIC, RegData(void));

  MOCK_METHOD0(Reset, void(void));

  MOCK_METHOD3(Write, void(RegData data, PrivLevel priv, ContextId ctxt));
  MOCK_METHOD1(WriteBody, void(RegData data));
  MOCK_CONST_METHOD2(Read, RegData(PrivLevel priv, ContextId ctxt));
  MOCK_CONST_METHOD0(ReadBody, RegData(void));
  MOCK_CONST_METHOD0(GetName, std::string(void));

  MOCK_CONST_METHOD0(GetCfSRegId, CfSReg(void));

  MockSRegGMEIIC(ContextId ctid, ProcElementBase* pe) : SRegGMEIIC(ctid, pe) {};
};


/*!
  @brief GMEIPC register mock class
 */
class MockSRegGMEIPC : public SRegGMEIPC {

public:
  MOCK_METHOD1(SetEIPC, void(RegData eipc));
  MOCK_CONST_METHOD0(GetEIPC, RegData(void));

  MOCK_METHOD0(Reset, void(void));

  MOCK_METHOD3(Write, void(RegData data, PrivLevel priv, ContextId ctxt));
  MOCK_METHOD1(WriteBody, void(RegData data));
  MOCK_CONST_METHOD2(Read, RegData(PrivLevel priv, ContextId ctxt));
  MOCK_CONST_METHOD0(ReadBody, RegData(void));
  MOCK_CONST_METHOD0(GetName, std::string(void));

  MOCK_CONST_METHOD0(GetCfSRegId, CfSReg(void));

  MockSRegGMEIPC(ContextId ctid, ProcElementBase* pe) : SRegGMEIPC(ctid, pe) {};
};


/*!
  @brief GMEIPSW register mock class
 */
class MockSRegGMEIPSW : public SRegGMEIPSW {

public:
  MOCK_METHOD1(SetUM, void(bool um));
  MOCK_METHOD1(SetEIMASK, void(RegData eimask));
  MOCK_METHOD1(SetCU, void(RegData cu));
  MOCK_METHOD1(SetEBV, void(bool ebv));
  MOCK_METHOD1(SetNP, void(bool np));
  MOCK_METHOD1(SetEP, void(bool ep));
  MOCK_METHOD1(SetID, void(bool id));
  MOCK_METHOD1(SetSAT, void(bool sat));
  MOCK_METHOD1(SetCY, void(bool cy));
  MOCK_METHOD1(SetOV, void(bool ov));
  MOCK_METHOD1(SetS, void(bool s));
  MOCK_METHOD1(SetZ, void(bool z));
  MOCK_CONST_METHOD0(GetUM, bool(void));
  MOCK_CONST_METHOD0(GetEIMASK, RegData(void));
  MOCK_CONST_METHOD0(GetCU, RegData(void));
  MOCK_CONST_METHOD0(GetEBV, bool(void));
  MOCK_CONST_METHOD0(GetNP, bool(void));
  MOCK_CONST_METHOD0(GetEP, bool(void));
  MOCK_CONST_METHOD0(GetID, bool(void));
  MOCK_CONST_METHOD0(GetSAT, bool(void));
  MOCK_CONST_METHOD0(GetCY, bool(void));
  MOCK_CONST_METHOD0(GetOV, bool(void));
  MOCK_CONST_METHOD0(GetS, bool(void));
  MOCK_CONST_METHOD0(GetZ, bool(void));

  MOCK_METHOD0(Reset, void(void));

  MOCK_METHOD3(Write, void(RegData data, PrivLevel priv, ContextId ctxt));
  MOCK_METHOD1(WriteBody, void(RegData data));
  MOCK_CONST_METHOD2(Read, RegData(PrivLevel priv, ContextId ctxt));
  MOCK_CONST_METHOD0(ReadBody, RegData(void));
  MOCK_CONST_METHOD0(GetName, std::string(void));

  MOCK_CONST_METHOD0(GetCfSRegId, CfSReg(void));

  MockSRegGMEIPSW(ContextId ctid, ProcElementBase* pe) : SRegGMEIPSW(ctid, pe) {};
};
void SRegGMEIPSW::SetEIMASK(RegData eimask) {};
void SRegGMEIPSW::SetCU(RegData cu) {};
void SRegGMEIPSW::Write(RegData data, PrivLevel priv, ContextId ctxt) {};
RegData SRegGMEIPSW::Read(PrivLevel priv, ContextId ctxt) const { return static_cast<RegData>(0); };


/*!
  @brief GMEIWR register mock class
 */
class MockSRegGMEIWR : public SRegGMEIWR {

public:
  MOCK_METHOD1(SetEIWR, void(RegData eiwr));
  MOCK_CONST_METHOD0(GetEIWR, RegData(void));

  MOCK_METHOD0(Reset, void(void));

  MOCK_METHOD3(Write, void(RegData data, PrivLevel priv, ContextId ctxt));
  MOCK_METHOD1(WriteBody, void(RegData data));
  MOCK_CONST_METHOD2(Read, RegData(PrivLevel priv, ContextId ctxt));
  MOCK_CONST_METHOD0(ReadBody, RegData(void));
  MOCK_CONST_METHOD0(GetName, std::string(void));

  MOCK_CONST_METHOD0(GetCfSRegId, CfSReg(void));

  MockSRegGMEIWR(ContextId ctid, ProcElementBase* pe) : SRegGMEIWR(ctid, pe) {};
};


/*!
  @brief GMFEIC register mock class
 */
class MockSRegGMFEIC : public SRegGMFEIC {

public:
  MOCK_METHOD1(SetFEIC, void(RegData feic));
  MOCK_CONST_METHOD0(GetFEIC, RegData(void));

  MOCK_METHOD0(Reset, void(void));

  MOCK_METHOD3(Write, void(RegData data, PrivLevel priv, ContextId ctxt));
  MOCK_METHOD1(WriteBody, void(RegData data));
  MOCK_CONST_METHOD2(Read, RegData(PrivLevel priv, ContextId ctxt));
  MOCK_CONST_METHOD0(ReadBody, RegData(void));
  MOCK_CONST_METHOD0(GetName, std::string(void));

  MOCK_CONST_METHOD0(GetCfSRegId, CfSReg(void));

  MockSRegGMFEIC(ContextId ctid, ProcElementBase* pe) : SRegGMFEIC(ctid, pe) {};
};


/*!
  @brief GMFEPC register mock class
 */
class MockSRegGMFEPC : public SRegGMFEPC {

public:
  MOCK_METHOD1(SetFEPC, void(RegData fepc));
  MOCK_CONST_METHOD0(GetFEPC, RegData(void));

  MOCK_METHOD0(Reset, void(void));

  MOCK_METHOD3(Write, void(RegData data, PrivLevel priv, ContextId ctxt));
  MOCK_METHOD1(WriteBody, void(RegData data));
  MOCK_CONST_METHOD2(Read, RegData(PrivLevel priv, ContextId ctxt));
  MOCK_CONST_METHOD0(ReadBody, RegData(void));
  MOCK_CONST_METHOD0(GetName, std::string(void));

  MOCK_CONST_METHOD0(GetCfSRegId, CfSReg(void));

  MockSRegGMFEPC(ContextId ctid, ProcElementBase* pe) : SRegGMFEPC(ctid, pe) {};
};


/*!
  @brief GMFEPSW register mock class
 */
class MockSRegGMFEPSW : public SRegGMFEPSW {

public:
  MOCK_METHOD1(SetUM, void(bool um));
  MOCK_METHOD1(SetEIMASK, void(RegData eimask));
  MOCK_METHOD1(SetCU, void(RegData cu));
  MOCK_METHOD1(SetEBV, void(bool ebv));
  MOCK_METHOD1(SetNP, void(bool np));
  MOCK_METHOD1(SetEP, void(bool ep));
  MOCK_METHOD1(SetID, void(bool id));
  MOCK_METHOD1(SetSAT, void(bool sat));
  MOCK_METHOD1(SetCY, void(bool cy));
  MOCK_METHOD1(SetOV, void(bool ov));
  MOCK_METHOD1(SetS, void(bool s));
  MOCK_METHOD1(SetZ, void(bool z));
  MOCK_CONST_METHOD0(GetUM, bool(void));
  MOCK_CONST_METHOD0(GetEIMASK, RegData(void));
  MOCK_CONST_METHOD0(GetCU, RegData(void));
  MOCK_CONST_METHOD0(GetEBV, bool(void));
  MOCK_CONST_METHOD0(GetNP, bool(void));
  MOCK_CONST_METHOD0(GetEP, bool(void));
  MOCK_CONST_METHOD0(GetID, bool(void));
  MOCK_CONST_METHOD0(GetSAT, bool(void));
  MOCK_CONST_METHOD0(GetCY, bool(void));
  MOCK_CONST_METHOD0(GetOV, bool(void));
  MOCK_CONST_METHOD0(GetS, bool(void));
  MOCK_CONST_METHOD0(GetZ, bool(void));

  MOCK_METHOD0(Reset, void(void));

  MOCK_METHOD3(Write, void(RegData data, PrivLevel priv, ContextId ctxt));
  MOCK_METHOD1(WriteBody, void(RegData data));
  MOCK_CONST_METHOD2(Read, RegData(PrivLevel priv, ContextId ctxt));
  MOCK_CONST_METHOD0(ReadBody, RegData(void));
  MOCK_CONST_METHOD0(GetName, std::string(void));

  MOCK_CONST_METHOD0(GetCfSRegId, CfSReg(void));

  MockSRegGMFEPSW(ContextId ctid, ProcElementBase* pe) : SRegGMFEPSW(ctid, pe) {};
};
void SRegGMFEPSW::SetEIMASK(RegData eimask) {};
void SRegGMFEPSW::SetCU(RegData cu) {};
void SRegGMFEPSW::Write(RegData data, PrivLevel priv, ContextId ctxt) {};
RegData SRegGMFEPSW::Read(PrivLevel priv, ContextId ctxt) const { return static_cast<RegData>(0); };


/*!
  @brief GMFEWR register mock class
 */
class MockSRegGMFEWR : public SRegGMFEWR {

public:
  MOCK_METHOD1(SetFEWR, void(RegData fewr));
  MOCK_CONST_METHOD0(GetFEWR, RegData(void));

  MOCK_METHOD0(Reset, void(void));

  MOCK_METHOD3(Write, void(RegData data, PrivLevel priv, ContextId ctxt));
  MOCK_METHOD1(WriteBody, void(RegData data));
  MOCK_CONST_METHOD2(Read, RegData(PrivLevel priv, ContextId ctxt));
  MOCK_CONST_METHOD0(ReadBody, RegData(void));
  MOCK_CONST_METHOD0(GetName, std::string(void));

  MOCK_CONST_METHOD0(GetCfSRegId, CfSReg(void));

  MockSRegGMFEWR(ContextId ctid, ProcElementBase* pe) : SRegGMFEWR(ctid, pe) {};
};


/*!
  @brief GMINTBP register mock class
 */
class MockSRegGMINTBP : public SRegGMINTBP {

public:
  MOCK_METHOD1(SetINTBP, void(RegData intbp));
  MOCK_CONST_METHOD0(GetINTBP, RegData(void));

  MOCK_METHOD0(Reset, void(void));

  MOCK_METHOD3(Write, void(RegData data, PrivLevel priv, ContextId ctxt));
  MOCK_METHOD1(WriteBody, void(RegData data));
  MOCK_CONST_METHOD2(Read, RegData(PrivLevel priv, ContextId ctxt));
  MOCK_CONST_METHOD0(ReadBody, RegData(void));
  MOCK_CONST_METHOD0(GetName, std::string(void));

  MOCK_CONST_METHOD0(GetCfSRegId, CfSReg(void));

  MockSRegGMINTBP(ContextId ctid, ProcElementBase* pe) : SRegGMINTBP(ctid, pe) {};
};


/*!
  @brief GMINTCFG register mock class
 */
class MockSRegGMINTCFG : public SRegGMINTCFG {

public:
  MOCK_METHOD1(SetULNR, void(RegData ulnr));
  MOCK_METHOD1(SetEPL, void(bool epl));
  MOCK_METHOD1(SetISPC, void(bool ispc));
  MOCK_CONST_METHOD0(GetULNR, RegData(void));
  MOCK_CONST_METHOD0(GetEPL, bool(void));
  MOCK_CONST_METHOD0(GetISPC, bool(void));

  MOCK_METHOD0(Reset, void(void));

  MOCK_METHOD3(Write, void(RegData data, PrivLevel priv, ContextId ctxt));
  MOCK_METHOD1(WriteBody, void(RegData data));
  MOCK_CONST_METHOD2(Read, RegData(PrivLevel priv, ContextId ctxt));
  MOCK_CONST_METHOD0(ReadBody, RegData(void));
  MOCK_CONST_METHOD0(GetName, std::string(void));

  MOCK_CONST_METHOD0(GetCfSRegId, CfSReg(void));

  MockSRegGMINTCFG(ContextId ctid, ProcElementBase* pe) : SRegGMINTCFG(ctid, pe) {};
};
void SRegGMINTCFG::SetULNR(RegData ulnr) {};


/*!
  @brief GMMEA register mock class
 */
class MockSRegGMMEA : public SRegGMMEA {

public:
  MOCK_METHOD1(SetMEA, void(RegData mea));
  MOCK_CONST_METHOD0(GetMEA, RegData(void));

  MOCK_METHOD0(Reset, void(void));

  MOCK_METHOD3(Write, void(RegData data, PrivLevel priv, ContextId ctxt));
  MOCK_METHOD1(WriteBody, void(RegData data));
  MOCK_CONST_METHOD2(Read, RegData(PrivLevel priv, ContextId ctxt));
  MOCK_CONST_METHOD0(ReadBody, RegData(void));
  MOCK_CONST_METHOD0(GetName, std::string(void));

  MOCK_CONST_METHOD0(GetCfSRegId, CfSReg(void));

  MockSRegGMMEA(ContextId ctid, ProcElementBase* pe) : SRegGMMEA(ctid, pe) {};
};


/*!
  @brief GMMEI register mock class
 */
class MockSRegGMMEI : public SRegGMMEI {

public:
  MOCK_METHOD1(SetLEN, void(RegData len));
  MOCK_METHOD1(SetREG, void(RegData reg));
  MOCK_METHOD1(SetDS, void(RegData ds));
  MOCK_METHOD1(SetU, void(bool u));
  MOCK_METHOD1(SetITYPE, void(RegData itype));
  MOCK_METHOD1(SetRW, void(bool rw));
  MOCK_CONST_METHOD0(GetLEN, RegData(void));
  MOCK_CONST_METHOD0(GetREG, RegData(void));
  MOCK_CONST_METHOD0(GetDS, RegData(void));
  MOCK_CONST_METHOD0(GetU, bool(void));
  MOCK_CONST_METHOD0(GetITYPE, RegData(void));
  MOCK_CONST_METHOD0(GetRW, bool(void));

  MOCK_METHOD0(Reset, void(void));

  MOCK_METHOD3(Write, void(RegData data, PrivLevel priv, ContextId ctxt));
  MOCK_METHOD1(WriteBody, void(RegData data));
  MOCK_CONST_METHOD2(Read, RegData(PrivLevel priv, ContextId ctxt));
  MOCK_CONST_METHOD0(ReadBody, RegData(void));
  MOCK_CONST_METHOD0(GetName, std::string(void));

  MOCK_CONST_METHOD0(GetCfSRegId, CfSReg(void));

  MockSRegGMMEI(ContextId ctid, ProcElementBase* pe) : SRegGMMEI(ctid, pe) {};
};


/*!
  @brief GMMPM register mock class
 */
class MockSRegGMMPM : public SRegGMMPM {

public:
  MOCK_METHOD1(SetGMPE, void(bool gmpe));
  MOCK_METHOD1(SetSVP, void(bool svp));
  MOCK_METHOD1(SetMPE, void(bool mpe));
  MOCK_CONST_METHOD0(GetGMPE, bool(void));
  MOCK_CONST_METHOD0(GetSVP, bool(void));
  MOCK_CONST_METHOD0(GetMPE, bool(void));

  MOCK_METHOD0(Reset, void(void));

  MOCK_METHOD3(Write, void(RegData data, PrivLevel priv, ContextId ctxt));
  MOCK_METHOD1(WriteBody, void(RegData data));
  MOCK_CONST_METHOD2(Read, RegData(PrivLevel priv, ContextId ctxt));
  MOCK_CONST_METHOD0(ReadBody, RegData(void));
  MOCK_CONST_METHOD0(GetName, std::string(void));

  MOCK_CONST_METHOD0(GetCfSRegId, CfSReg(void));

  MockSRegGMMPM(ContextId ctid, ProcElementBase* pe) : SRegGMMPM(ctid, pe) {};
};
void SRegGMMPM::SetMPE(bool mpe) {};
void SRegGMMPM::Write(RegData data, PrivLevel priv, ContextId ctxt) {};
RegData SRegGMMPM::Read(PrivLevel priv, ContextId ctxt) const { return static_cast<RegData>(0); };


/*!
  @brief GMPEID register mock class
 */
class MockSRegGMPEID : public SRegGMPEID {

public:
  MOCK_METHOD1(SetPEID, void(RegData peid));
  MOCK_CONST_METHOD0(GetPEID, RegData(void));

  MOCK_METHOD0(Reset, void(void));

  MOCK_METHOD3(Write, void(RegData data, PrivLevel priv, ContextId ctxt));
  MOCK_METHOD1(WriteBody, void(RegData data));
  MOCK_CONST_METHOD2(Read, RegData(PrivLevel priv, ContextId ctxt));
  MOCK_CONST_METHOD0(ReadBody, RegData(void));
  MOCK_CONST_METHOD0(GetName, std::string(void));

  MOCK_CONST_METHOD0(GetCfSRegId, CfSReg(void));

  MockSRegGMPEID(ContextId ctid, ProcElementBase* pe) : SRegGMPEID(ctid, pe) {};
};
void SRegGMPEID::Reset(void) {};
void SRegGMPEID::Write(RegData data, PrivLevel priv, ContextId ctxt) {};
RegData SRegGMPEID::Read(PrivLevel priv, ContextId ctxt) const { return static_cast<RegData>(0); };


/*!
  @brief GMPLMR register mock class
 */
class MockSRegGMPLMR : public SRegGMPLMR {

public:
  MOCK_METHOD1(SetPLM, void(RegData plm));
  MOCK_CONST_METHOD0(GetPLM, RegData(void));

  MOCK_METHOD0(Reset, void(void));

  MOCK_METHOD3(Write, void(RegData data, PrivLevel priv, ContextId ctxt));
  MOCK_METHOD1(WriteBody, void(RegData data));
  MOCK_CONST_METHOD2(Read, RegData(PrivLevel priv, ContextId ctxt));
  MOCK_CONST_METHOD0(ReadBody, RegData(void));
  MOCK_CONST_METHOD0(GetName, std::string(void));

  MOCK_CONST_METHOD0(GetCfSRegId, CfSReg(void));

  MockSRegGMPLMR(ContextId ctid, ProcElementBase* pe) : SRegGMPLMR(ctid, pe) {};
};
void SRegGMPLMR::SetPLM(RegData plm) {};
void SRegGMPLMR::Reset(void) {};


/*!
  @brief GMPSW register mock class
 */
class MockSRegGMPSW : public SRegGMPSW {

public:
  MOCK_METHOD1(SetUM, void(bool um));
  MOCK_METHOD1(SetEIMASK, void(RegData eimask));
  MOCK_METHOD1(SetCU, void(RegData cu));
  MOCK_METHOD1(SetEBV, void(bool ebv));
  MOCK_METHOD1(SetNP, void(bool np));
  MOCK_METHOD1(SetEP, void(bool ep));
  MOCK_METHOD1(SetID, void(bool id));
  MOCK_METHOD1(SetSAT, void(bool sat));
  MOCK_METHOD1(SetCY, void(bool cy));
  MOCK_METHOD1(SetOV, void(bool ov));
  MOCK_METHOD1(SetS, void(bool s));
  MOCK_METHOD1(SetZ, void(bool z));
  MOCK_CONST_METHOD0(GetUM, bool(void));
  MOCK_CONST_METHOD0(GetEIMASK, RegData(void));
  MOCK_CONST_METHOD0(GetCU, RegData(void));
  MOCK_CONST_METHOD0(GetEBV, bool(void));
  MOCK_CONST_METHOD0(GetNP, bool(void));
  MOCK_CONST_METHOD0(GetEP, bool(void));
  MOCK_CONST_METHOD0(GetID, bool(void));
  MOCK_CONST_METHOD0(GetSAT, bool(void));
  MOCK_CONST_METHOD0(GetCY, bool(void));
  MOCK_CONST_METHOD0(GetOV, bool(void));
  MOCK_CONST_METHOD0(GetS, bool(void));
  MOCK_CONST_METHOD0(GetZ, bool(void));

  MOCK_METHOD0(Reset, void(void));

  MOCK_METHOD3(Write, void(RegData data, PrivLevel priv, ContextId ctxt));
  MOCK_METHOD1(WriteBody, void(RegData data));
  MOCK_CONST_METHOD2(Read, RegData(PrivLevel priv, ContextId ctxt));
  MOCK_CONST_METHOD0(ReadBody, RegData(void));
  MOCK_CONST_METHOD0(GetName, std::string(void));

  MOCK_CONST_METHOD0(GetCfSRegId, CfSReg(void));

  MockSRegGMPSW(ContextId ctid, ProcElementBase* pe) : SRegGMPSW(ctid, pe) {};
};
void SRegGMPSW::SetUM(bool um) {};
void SRegGMPSW::SetEIMASK(RegData eimask) {};
void SRegGMPSW::SetCU(RegData cu) {};
void SRegGMPSW::SetEBV(bool ebv) {};
void SRegGMPSW::SetNP(bool np) {};
void SRegGMPSW::SetID(bool id) {};
void SRegGMPSW::Write(RegData data, PrivLevel priv, ContextId ctxt) {};
RegData SRegGMPSW::Read(PrivLevel priv, ContextId ctxt) const { return static_cast<RegData>(0); };


/*!
  @brief GMSPID register mock class
 */
class MockSRegGMSPID : public SRegGMSPID {

public:
  MOCK_METHOD1(SetSPID, void(RegData spid));
  MOCK_CONST_METHOD0(GetSPID, RegData(void));

  MOCK_METHOD0(Reset, void(void));

  MOCK_METHOD3(Write, void(RegData data, PrivLevel priv, ContextId ctxt));
  MOCK_METHOD1(WriteBody, void(RegData data));
  MOCK_CONST_METHOD2(Read, RegData(PrivLevel priv, ContextId ctxt));
  MOCK_CONST_METHOD0(ReadBody, RegData(void));
  MOCK_CONST_METHOD0(GetName, std::string(void));

  MOCK_CONST_METHOD0(GetCfSRegId, CfSReg(void));

  MockSRegGMSPID(ContextId ctid, ProcElementBase* pe) : SRegGMSPID(ctid, pe) {};
};
void SRegGMSPID::SetSPID(RegData spid) {};
void SRegGMSPID::Reset(void) {};
void SRegGMSPID::Write(RegData data, PrivLevel priv, ContextId ctxt) {};


/*!
  @brief GMSPIDLIST register mock class
 */
class MockSRegGMSPIDLIST : public SRegGMSPIDLIST {

public:
  MOCK_METHOD1(SetSPIDLIST, void(RegData spidlist));
  MOCK_CONST_METHOD0(GetSPIDLIST, RegData(void));

  MOCK_METHOD0(Reset, void(void));

  MOCK_METHOD3(Write, void(RegData data, PrivLevel priv, ContextId ctxt));
  MOCK_METHOD1(WriteBody, void(RegData data));
  MOCK_CONST_METHOD2(Read, RegData(PrivLevel priv, ContextId ctxt));
  MOCK_CONST_METHOD0(ReadBody, RegData(void));
  MOCK_CONST_METHOD0(GetName, std::string(void));

  MOCK_CONST_METHOD0(GetCfSRegId, CfSReg(void));

  MockSRegGMSPIDLIST(ContextId ctid, ProcElementBase* pe) : SRegGMSPIDLIST(ctid, pe) {};
};
void SRegGMSPIDLIST::SetSPIDLIST(RegData spidlist) {};
void SRegGMSPIDLIST::Reset(void) {};
void SRegGMSPIDLIST::Write(RegData data, PrivLevel priv, ContextId ctxt) {};


/*!
  @brief GMSVLOCK register mock class
 */
class MockSRegGMSVLOCK : public SRegGMSVLOCK {

public:
  MOCK_METHOD1(SetSVL, void(bool svl));
  MOCK_CONST_METHOD0(GetSVL, bool(void));

  MOCK_METHOD0(Reset, void(void));

  MOCK_METHOD3(Write, void(RegData data, PrivLevel priv, ContextId ctxt));
  MOCK_METHOD1(WriteBody, void(RegData data));
  MOCK_CONST_METHOD2(Read, RegData(PrivLevel priv, ContextId ctxt));
  MOCK_CONST_METHOD0(ReadBody, RegData(void));
  MOCK_CONST_METHOD0(GetName, std::string(void));

  MOCK_CONST_METHOD0(GetCfSRegId, CfSReg(void));

  MockSRegGMSVLOCK(ContextId ctid, ProcElementBase* pe) : SRegGMSVLOCK(ctid, pe) {};
};


/*!
  @brief HVCFG register mock class
 */
class MockSRegHVCFG : public SRegHVCFG {

public:
  MOCK_METHOD1(SetHVE, void(bool hve));
  MOCK_CONST_METHOD0(GetHVE, bool(void));

  MOCK_METHOD0(Reset, void(void));

  MOCK_METHOD3(Write, void(RegData data, PrivLevel priv, ContextId ctxt));
  MOCK_METHOD1(WriteBody, void(RegData data));
  MOCK_CONST_METHOD2(Read, RegData(PrivLevel priv, ContextId ctxt));
  MOCK_CONST_METHOD0(ReadBody, RegData(void));
  MOCK_CONST_METHOD0(GetName, std::string(void));

  MOCK_CONST_METHOD0(GetCfSRegId, CfSReg(void));

  MockSRegHVCFG(ContextId ctid, ProcElementBase* pe) : SRegHVCFG(ctid, pe) {};
};
void SRegHVCFG::SetHVE(bool hve) {};
void SRegHVCFG::Reset(void) {};
void SRegHVCFG::Write(RegData data, PrivLevel priv, ContextId ctxt) {};
RegData SRegHVCFG::Read(PrivLevel priv, ContextId ctxt) const { return static_cast<RegData>(0); };


/*!
  @brief HVSB register mock class
 */
class MockSRegHVSB : public SRegHVSB {

public:
  MOCK_METHOD1(SetHVSB, void(RegData hvsb));
  MOCK_CONST_METHOD0(GetHVSB, RegData(void));

  MOCK_METHOD0(Reset, void(void));

  MOCK_METHOD3(Write, void(RegData data, PrivLevel priv, ContextId ctxt));
  MOCK_METHOD1(WriteBody, void(RegData data));
  MOCK_CONST_METHOD2(Read, RegData(PrivLevel priv, ContextId ctxt));
  MOCK_CONST_METHOD0(ReadBody, RegData(void));
  MOCK_CONST_METHOD0(GetName, std::string(void));

  MOCK_CONST_METHOD0(GetCfSRegId, CfSReg(void));

  MockSRegHVSB(ContextId ctid, ProcElementBase* pe) : SRegHVSB(ctid, pe) {};
};
void SRegHVSB::Write(RegData data, PrivLevel priv, ContextId ctxt) {};
RegData SRegHVSB::Read(PrivLevel priv, ContextId ctxt) const { return static_cast<RegData>(0); };


/*!
  @brief ICBKEY register mock class
 */
class MockSRegICBKEY : public SRegICBKEY {

public:
  MOCK_METHOD1(SetCBAKEY, void(RegData cbakey));
  MOCK_METHOD1(SetKEYCODE, void(RegData keycode));
  MOCK_CONST_METHOD0(GetCBAKEY, RegData(void));
  MOCK_CONST_METHOD0(GetKEYCODE, RegData(void));

  MOCK_METHOD0(Reset, void(void));

  MOCK_METHOD3(Write, void(RegData data, PrivLevel priv, ContextId ctxt));
  MOCK_METHOD1(WriteBody, void(RegData data));
  MOCK_CONST_METHOD2(Read, RegData(PrivLevel priv, ContextId ctxt));
  MOCK_CONST_METHOD0(ReadBody, RegData(void));
  MOCK_CONST_METHOD0(GetName, std::string(void));

  MOCK_CONST_METHOD0(GetCfSRegId, CfSReg(void));

  MockSRegICBKEY(ContextId ctid, ProcElementBase* pe) : SRegICBKEY(ctid, pe) {};
};
void SRegICBKEY::Write(RegData data, PrivLevel priv, ContextId ctxt) {};
RegData SRegICBKEY::Read(PrivLevel priv, ContextId ctxt) const { return static_cast<RegData>(0); };


/*!
  @brief ICCFG register mock class
 */
class MockSRegICCFG : public SRegICCFG {

public:
  MOCK_METHOD1(SetICHSIZE, void(RegData ichsize));
  MOCK_METHOD1(SetICHLINE, void(RegData ichline));
  MOCK_METHOD1(SetICHWAY, void(RegData ichway));
  MOCK_CONST_METHOD0(GetICHSIZE, RegData(void));
  MOCK_CONST_METHOD0(GetICHLINE, RegData(void));
  MOCK_CONST_METHOD0(GetICHWAY, RegData(void));

  MOCK_METHOD0(Reset, void(void));

  MOCK_METHOD3(Write, void(RegData data, PrivLevel priv, ContextId ctxt));
  MOCK_METHOD1(WriteBody, void(RegData data));
  MOCK_CONST_METHOD2(Read, RegData(PrivLevel priv, ContextId ctxt));
  MOCK_CONST_METHOD0(ReadBody, RegData(void));
  MOCK_CONST_METHOD0(GetName, std::string(void));

  MOCK_CONST_METHOD0(GetCfSRegId, CfSReg(void));

  MockSRegICCFG(ContextId ctid, ProcElementBase* pe) : SRegICCFG(ctid, pe) {};
};
RegData SRegICCFG::Read(PrivLevel priv, ContextId ctxt) const { return static_cast<RegData>(0); };


/*!
  @brief ICCTRL register mock class
 */
class MockSRegICCTRL : public SRegICCTRL {

public:
  MOCK_METHOD1(SetPBS, void(bool pbs));
  MOCK_METHOD1(SetICHCLR, void(bool ichclr));
  MOCK_METHOD1(SetICHEIV, void(bool icheiv));
  MOCK_METHOD1(SetICHEMK, void(bool ichemk));
  MOCK_METHOD1(SetICHEN, void(bool ichen));
  MOCK_CONST_METHOD0(GetPBS, bool(void));
  MOCK_CONST_METHOD0(GetICHCLR, bool(void));
  MOCK_CONST_METHOD0(GetICHEIV, bool(void));
  MOCK_CONST_METHOD0(GetICHEMK, bool(void));
  MOCK_CONST_METHOD0(GetICHEN, bool(void));

  MOCK_METHOD0(Reset, void(void));

  MOCK_METHOD3(Write, void(RegData data, PrivLevel priv, ContextId ctxt));
  MOCK_METHOD1(WriteBody, void(RegData data));
  MOCK_CONST_METHOD2(Read, RegData(PrivLevel priv, ContextId ctxt));
  MOCK_CONST_METHOD0(ReadBody, RegData(void));
  MOCK_CONST_METHOD0(GetName, std::string(void));

  MOCK_CONST_METHOD0(GetCfSRegId, CfSReg(void));

  MockSRegICCTRL(ContextId ctid, ProcElementBase* pe) : SRegICCTRL(ctid, pe) {};
};
void SRegICCTRL::SetICHCLR(bool ichclr) {};
void SRegICCTRL::SetICHEN(bool ichen) {};
void SRegICCTRL::Reset(void) {};
void SRegICCTRL::Write(RegData data, PrivLevel priv, ContextId ctxt) {};
RegData SRegICCTRL::Read(PrivLevel priv, ContextId ctxt) const { return static_cast<RegData>(0); };


/*!
  @brief ICDATH register mock class
 */
class MockSRegICDATH : public SRegICDATH {

public:
  MOCK_METHOD1(SetDATAH, void(RegData datah));
  MOCK_CONST_METHOD0(GetDATAH, RegData(void));

  MOCK_METHOD0(Reset, void(void));

  MOCK_METHOD3(Write, void(RegData data, PrivLevel priv, ContextId ctxt));
  MOCK_METHOD1(WriteBody, void(RegData data));
  MOCK_CONST_METHOD2(Read, RegData(PrivLevel priv, ContextId ctxt));
  MOCK_CONST_METHOD0(ReadBody, RegData(void));
  MOCK_CONST_METHOD0(GetName, std::string(void));

  MOCK_CONST_METHOD0(GetCfSRegId, CfSReg(void));

  MockSRegICDATH(ContextId ctid, ProcElementBase* pe) : SRegICDATH(ctid, pe) {};
};
void SRegICDATH::Write(RegData data, PrivLevel priv, ContextId ctxt) {};
RegData SRegICDATH::Read(PrivLevel priv, ContextId ctxt) const { return static_cast<RegData>(0); };


/*!
  @brief ICDATL register mock class
 */
class MockSRegICDATL : public SRegICDATL {

public:
  MOCK_METHOD1(SetDATAL, void(RegData datal));
  MOCK_CONST_METHOD0(GetDATAL, RegData(void));

  MOCK_METHOD0(Reset, void(void));

  MOCK_METHOD3(Write, void(RegData data, PrivLevel priv, ContextId ctxt));
  MOCK_METHOD1(WriteBody, void(RegData data));
  MOCK_CONST_METHOD2(Read, RegData(PrivLevel priv, ContextId ctxt));
  MOCK_CONST_METHOD0(ReadBody, RegData(void));
  MOCK_CONST_METHOD0(GetName, std::string(void));

  MOCK_CONST_METHOD0(GetCfSRegId, CfSReg(void));

  MockSRegICDATL(ContextId ctid, ProcElementBase* pe) : SRegICDATL(ctid, pe) {};
};
void SRegICDATL::Write(RegData data, PrivLevel priv, ContextId ctxt) {};
RegData SRegICDATL::Read(PrivLevel priv, ContextId ctxt) const { return static_cast<RegData>(0); };


/*!
  @brief ICERR register mock class
 */
class MockSRegICERR : public SRegICERR {

public:
  MOCK_METHOD1(SetCISTW, void(bool cistw));
  MOCK_METHOD1(SetESAFE, void(bool esafe));
  MOCK_METHOD1(SetESMH, void(bool esmh));
  MOCK_METHOD1(SetESPBSE, void(bool espbse));
  MOCK_METHOD1(SetESTE1, void(bool este1));
  MOCK_METHOD1(SetESTE2, void(bool este2));
  MOCK_METHOD1(SetESDC, void(bool esdc));
  MOCK_METHOD1(SetESDE, void(bool esde));
  MOCK_METHOD1(SetERMAFE, void(bool ermafe));
  MOCK_METHOD1(SetERMMH, void(bool ermmh));
  MOCK_METHOD1(SetERMPBSE, void(bool ermpbse));
  MOCK_METHOD1(SetERMTE1, void(bool ermte1));
  MOCK_METHOD1(SetERMTE2, void(bool ermte2));
  MOCK_METHOD1(SetERMDC, void(bool ermdc));
  MOCK_METHOD1(SetERMDE, void(bool ermde));
  MOCK_METHOD1(SetICHEWY, void(RegData ichewy));
  MOCK_METHOD1(SetICHEIX, void(RegData icheix));
  MOCK_METHOD1(SetICHERQ, void(bool icherq));
  MOCK_METHOD1(SetICHED, void(bool iched));
  MOCK_METHOD1(SetICHET, void(bool ichet));
  MOCK_METHOD1(SetICHERR, void(bool icherr));
  MOCK_CONST_METHOD0(GetCISTW, bool(void));
  MOCK_CONST_METHOD0(GetESAFE, bool(void));
  MOCK_CONST_METHOD0(GetESMH, bool(void));
  MOCK_CONST_METHOD0(GetESPBSE, bool(void));
  MOCK_CONST_METHOD0(GetESTE1, bool(void));
  MOCK_CONST_METHOD0(GetESTE2, bool(void));
  MOCK_CONST_METHOD0(GetESDC, bool(void));
  MOCK_CONST_METHOD0(GetESDE, bool(void));
  MOCK_CONST_METHOD0(GetERMAFE, bool(void));
  MOCK_CONST_METHOD0(GetERMMH, bool(void));
  MOCK_CONST_METHOD0(GetERMPBSE, bool(void));
  MOCK_CONST_METHOD0(GetERMTE1, bool(void));
  MOCK_CONST_METHOD0(GetERMTE2, bool(void));
  MOCK_CONST_METHOD0(GetERMDC, bool(void));
  MOCK_CONST_METHOD0(GetERMDE, bool(void));
  MOCK_CONST_METHOD0(GetICHEWY, RegData(void));
  MOCK_CONST_METHOD0(GetICHEIX, RegData(void));
  MOCK_CONST_METHOD0(GetICHERQ, bool(void));
  MOCK_CONST_METHOD0(GetICHED, bool(void));
  MOCK_CONST_METHOD0(GetICHET, bool(void));
  MOCK_CONST_METHOD0(GetICHERR, bool(void));

  MOCK_METHOD0(Reset, void(void));

  MOCK_METHOD3(Write, void(RegData data, PrivLevel priv, ContextId ctxt));
  MOCK_METHOD1(WriteBody, void(RegData data));
  MOCK_CONST_METHOD2(Read, RegData(PrivLevel priv, ContextId ctxt));
  MOCK_CONST_METHOD0(ReadBody, RegData(void));
  MOCK_CONST_METHOD0(GetName, std::string(void));

  MOCK_CONST_METHOD0(GetCfSRegId, CfSReg(void));

  MockSRegICERR(ContextId ctid, ProcElementBase* pe) : SRegICERR(ctid, pe) {};
};
void SRegICERR::SetICHEIX(RegData icheix) {};
void SRegICERR::Write(RegData data, PrivLevel priv, ContextId ctxt) {};
RegData SRegICERR::Read(PrivLevel priv, ContextId ctxt) const { return static_cast<RegData>(0); };


/*!
  @brief ICSR register mock class
 */
class MockSRegICSR : public SRegICSR {

public:
  MOCK_METHOD1(SetPMFP, void(bool pmfp));
  MOCK_METHOD1(SetPMEI, void(bool pmei));
  MOCK_CONST_METHOD0(GetPMFP, bool(void));
  MOCK_CONST_METHOD0(GetPMEI, bool(void));

  MOCK_METHOD0(Reset, void(void));

  MOCK_METHOD3(Write, void(RegData data, PrivLevel priv, ContextId ctxt));
  MOCK_METHOD1(WriteBody, void(RegData data));
  MOCK_CONST_METHOD2(Read, RegData(PrivLevel priv, ContextId ctxt));
  MOCK_CONST_METHOD0(ReadBody, RegData(void));
  MOCK_CONST_METHOD0(GetName, std::string(void));

  MOCK_CONST_METHOD0(GetCfSRegId, CfSReg(void));

  MockSRegICSR(ContextId ctid, ProcElementBase* pe) : SRegICSR(ctid, pe) {};
};


/*!
  @brief ICTAGH register mock class
 */
class MockSRegICTAGH : public SRegICTAGH {

public:
  MOCK_METHOD1(SetWD, void(bool wd));
  MOCK_METHOD1(SetPD, void(bool pd));
  MOCK_METHOD1(SetWT, void(bool wt));
  MOCK_METHOD1(SetPT, void(bool pt));
  MOCK_METHOD1(SetDATAECC, void(RegData dataecc));
  MOCK_METHOD1(SetTAGECC, void(RegData tagecc));
  MOCK_CONST_METHOD0(GetWD, bool(void));
  MOCK_CONST_METHOD0(GetPD, bool(void));
  MOCK_CONST_METHOD0(GetWT, bool(void));
  MOCK_CONST_METHOD0(GetPT, bool(void));
  MOCK_CONST_METHOD0(GetDATAECC, RegData(void));
  MOCK_CONST_METHOD0(GetTAGECC, RegData(void));

  MOCK_METHOD0(Reset, void(void));

  MOCK_METHOD3(Write, void(RegData data, PrivLevel priv, ContextId ctxt));
  MOCK_METHOD1(WriteBody, void(RegData data));
  MOCK_CONST_METHOD2(Read, RegData(PrivLevel priv, ContextId ctxt));
  MOCK_CONST_METHOD0(ReadBody, RegData(void));
  MOCK_CONST_METHOD0(GetName, std::string(void));

  MOCK_CONST_METHOD0(GetCfSRegId, CfSReg(void));

  MockSRegICTAGH(ContextId ctid, ProcElementBase* pe) : SRegICTAGH(ctid, pe) {};
};
void SRegICTAGH::Write(RegData data, PrivLevel priv, ContextId ctxt) {};
RegData SRegICTAGH::Read(PrivLevel priv, ContextId ctxt) const { return static_cast<RegData>(0); };


/*!
  @brief ICTAGL register mock class
 */
class MockSRegICTAGL : public SRegICTAGL {

public:
  MOCK_METHOD1(SetLPN, void(RegData lpn));
  MOCK_METHOD1(SetLRU, void(RegData lru));
  MOCK_METHOD1(SetL, void(bool l));
  MOCK_METHOD1(SetV, void(bool v));
  MOCK_CONST_METHOD0(GetLPN, RegData(void));
  MOCK_CONST_METHOD0(GetLRU, RegData(void));
  MOCK_CONST_METHOD0(GetL, bool(void));
  MOCK_CONST_METHOD0(GetV, bool(void));

  MOCK_METHOD0(Reset, void(void));

  MOCK_METHOD3(Write, void(RegData data, PrivLevel priv, ContextId ctxt));
  MOCK_METHOD1(WriteBody, void(RegData data));
  MOCK_CONST_METHOD2(Read, RegData(PrivLevel priv, ContextId ctxt));
  MOCK_CONST_METHOD0(ReadBody, RegData(void));
  MOCK_CONST_METHOD0(GetName, std::string(void));

  MOCK_CONST_METHOD0(GetCfSRegId, CfSReg(void));

  MockSRegICTAGL(ContextId ctid, ProcElementBase* pe) : SRegICTAGL(ctid, pe) {};
};
void SRegICTAGL::Write(RegData data, PrivLevel priv, ContextId ctxt) {};
RegData SRegICTAGL::Read(PrivLevel priv, ContextId ctxt) const { return static_cast<RegData>(0); };


/*!
  @brief IFCR register mock class
 */
class MockSRegIFCR : public SRegIFCR {

public:
  MOCK_METHOD1(SetPLEN, void(bool plen));
  MOCK_CONST_METHOD0(GetPLEN, bool(void));

  MOCK_METHOD0(Reset, void(void));

  MOCK_METHOD3(Write, void(RegData data, PrivLevel priv, ContextId ctxt));
  MOCK_METHOD1(WriteBody, void(RegData data));
  MOCK_CONST_METHOD2(Read, RegData(PrivLevel priv, ContextId ctxt));
  MOCK_CONST_METHOD0(ReadBody, RegData(void));
  MOCK_CONST_METHOD0(GetName, std::string(void));

  MOCK_CONST_METHOD0(GetCfSRegId, CfSReg(void));

  MockSRegIFCR(ContextId ctid, ProcElementBase* pe) : SRegIFCR(ctid, pe) {};
};
void SRegIFCR::Write(RegData data, PrivLevel priv, ContextId ctxt) {};
RegData SRegIFCR::Read(PrivLevel priv, ContextId ctxt) const { return static_cast<RegData>(0); };


/*!
  @brief IFCR1 register mock class
 */
class MockSRegIFCR1 : public SRegIFCR1 {

public:
  MOCK_METHOD1(SetFD2, void(bool fd2));
  MOCK_METHOD1(SetPLINTD, void(bool plintd));
  MOCK_METHOD1(SetPLNLND, void(bool plnlnd));
  MOCK_CONST_METHOD0(GetFD2, bool(void));
  MOCK_CONST_METHOD0(GetPLINTD, bool(void));
  MOCK_CONST_METHOD0(GetPLNLND, bool(void));

  MOCK_METHOD0(Reset, void(void));

  MOCK_METHOD3(Write, void(RegData data, PrivLevel priv, ContextId ctxt));
  MOCK_METHOD1(WriteBody, void(RegData data));
  MOCK_CONST_METHOD2(Read, RegData(PrivLevel priv, ContextId ctxt));
  MOCK_CONST_METHOD0(ReadBody, RegData(void));
  MOCK_CONST_METHOD0(GetName, std::string(void));

  MOCK_CONST_METHOD0(GetCfSRegId, CfSReg(void));

  MockSRegIFCR1(ContextId ctid, ProcElementBase* pe) : SRegIFCR1(ctid, pe) {};
};
void SRegIFCR1::Write(RegData data, PrivLevel priv, ContextId ctxt) {};
RegData SRegIFCR1::Read(PrivLevel priv, ContextId ctxt) const { return static_cast<RegData>(0); };


/*!
  @brief IMSR register mock class
 */
class MockSRegIMSR : public SRegIMSR {

public:
  MOCK_METHOD1(SetHFNP, void(bool hfnp));
  MOCK_METHOD1(SetHENP, void(bool henp));
  MOCK_METHOD1(SetHEID, void(bool heid));
  MOCK_METHOD1(SetHEPLM, void(bool heplm));
  MOCK_METHOD1(SetHEEIM, void(bool heeim));
  MOCK_METHOD1(SetFNP, void(bool fnp));
  MOCK_METHOD1(SetENP, void(bool enp));
  MOCK_METHOD1(SetEID, void(bool eid));
  MOCK_METHOD1(SetEPLM, void(bool eplm));
  MOCK_METHOD1(SetEEIM, void(bool eeim));
  MOCK_CONST_METHOD0(GetHFNP, bool(void));
  MOCK_CONST_METHOD0(GetHENP, bool(void));
  MOCK_CONST_METHOD0(GetHEID, bool(void));
  MOCK_CONST_METHOD0(GetHEPLM, bool(void));
  MOCK_CONST_METHOD0(GetHEEIM, bool(void));
  MOCK_CONST_METHOD0(GetFNP, bool(void));
  MOCK_CONST_METHOD0(GetENP, bool(void));
  MOCK_CONST_METHOD0(GetEID, bool(void));
  MOCK_CONST_METHOD0(GetEPLM, bool(void));
  MOCK_CONST_METHOD0(GetEEIM, bool(void));

  MOCK_METHOD0(Reset, void(void));

  MOCK_METHOD3(Write, void(RegData data, PrivLevel priv, ContextId ctxt));
  MOCK_METHOD1(WriteBody, void(RegData data));
  MOCK_CONST_METHOD2(Read, RegData(PrivLevel priv, ContextId ctxt));
  MOCK_CONST_METHOD0(ReadBody, RegData(void));
  MOCK_CONST_METHOD0(GetName, std::string(void));

  MOCK_CONST_METHOD0(GetCfSRegId, CfSReg(void));

  MockSRegIMSR(ContextId ctid, ProcElementBase* pe) : SRegIMSR(ctid, pe) {};
};
void SRegIMSR::SetHENP(bool henp) {};
void SRegIMSR::SetHEID(bool heid) {};
void SRegIMSR::SetHEPLM(bool heplm) {};
void SRegIMSR::SetENP(bool enp) {};
void SRegIMSR::SetEID(bool eid) {};
void SRegIMSR::SetEPLM(bool eplm) {};
bool SRegIMSR::GetHFNP(void) const { return static_cast<bool>(0); };
bool SRegIMSR::GetHENP(void) const { return static_cast<bool>(0); };
bool SRegIMSR::GetHEID(void) const { return static_cast<bool>(0); };
bool SRegIMSR::GetHEPLM(void) const { return static_cast<bool>(0); };
bool SRegIMSR::GetHEEIM(void) const { return static_cast<bool>(0); };


/*!
  @brief INTBP register mock class
 */
class MockSRegHMINTBP : public SRegHMINTBP {

public:
  MOCK_METHOD1(SetINTBP, void(RegData intbp));
  MOCK_CONST_METHOD0(GetINTBP, RegData(void));

  MOCK_METHOD0(Reset, void(void));

  MOCK_METHOD3(Write, void(RegData data, PrivLevel priv, ContextId ctxt));
  MOCK_METHOD1(WriteBody, void(RegData data));
  MOCK_CONST_METHOD2(Read, RegData(PrivLevel priv, ContextId ctxt));
  MOCK_CONST_METHOD0(ReadBody, RegData(void));
  MOCK_CONST_METHOD0(GetName, std::string(void));

  MOCK_CONST_METHOD0(GetCfSRegId, CfSReg(void));

  MockSRegHMINTBP(ContextId ctid, ProcElementBase* pe) : SRegHMINTBP(ctid, pe) {};
};


/*!
  @brief INTCFG register mock class
 */
class MockSRegHMINTCFG : public SRegHMINTCFG {

public:
  MOCK_METHOD1(SetULNR, void(RegData ulnr));
  MOCK_METHOD1(SetEPL, void(bool epl));
  MOCK_METHOD1(SetISPC, void(bool ispc));
  MOCK_CONST_METHOD0(GetULNR, RegData(void));
  MOCK_CONST_METHOD0(GetEPL, bool(void));
  MOCK_CONST_METHOD0(GetISPC, bool(void));

  MOCK_METHOD0(Reset, void(void));

  MOCK_METHOD3(Write, void(RegData data, PrivLevel priv, ContextId ctxt));
  MOCK_METHOD1(WriteBody, void(RegData data));
  MOCK_CONST_METHOD2(Read, RegData(PrivLevel priv, ContextId ctxt));
  MOCK_CONST_METHOD0(ReadBody, RegData(void));
  MOCK_CONST_METHOD0(GetName, std::string(void));

  MOCK_CONST_METHOD0(GetCfSRegId, CfSReg(void));

  MockSRegHMINTCFG(ContextId ctid, ProcElementBase* pe) : SRegHMINTCFG(ctid, pe) {};
};
void SRegHMINTCFG::SetULNR(RegData ulnr) {};
void SRegHMINTCFG::SetEPL(bool epl) {};
void SRegHMINTCFG::SetISPC(bool ispc) {};
void SRegHMINTCFG::Reset(void) {};


/*!
  @brief ISPR register mock class
 */
class MockSRegISPR : public SRegISPR {

public:
  MOCK_METHOD1(SetISP, void(RegData isp));
  MOCK_CONST_METHOD0(GetISP, RegData(void));

  MOCK_METHOD0(Reset, void(void));

  MOCK_METHOD3(Write, void(RegData data, PrivLevel priv, ContextId ctxt));
  MOCK_METHOD1(WriteBody, void(RegData data));
  MOCK_CONST_METHOD2(Read, RegData(PrivLevel priv, ContextId ctxt));
  MOCK_CONST_METHOD0(ReadBody, RegData(void));
  MOCK_CONST_METHOD0(GetName, std::string(void));

  MOCK_CONST_METHOD0(GetCfSRegId, CfSReg(void));

  MockSRegISPR(ContextId ctid, ProcElementBase* pe) : SRegISPR(ctid, pe) {};
};
void SRegISPR::SetISP(RegData isp) {};
void SRegISPR::Write(RegData data, PrivLevel priv, ContextId ctxt) {};


/*!
  @brief L1RCFG register mock class
 */
class MockSRegL1RCFG : public SRegL1RCFG {

public:
  MOCK_METHOD1(SetL1RSIZE, void(RegData l1rsize));
  MOCK_METHOD1(SetL1RLNKE, void(RegData l1rlnke));
  MOCK_CONST_METHOD0(GetL1RSIZE, RegData(void));
  MOCK_CONST_METHOD0(GetL1RLNKE, RegData(void));

  MOCK_METHOD0(Reset, void(void));

  MOCK_METHOD3(Write, void(RegData data, PrivLevel priv, ContextId ctxt));
  MOCK_METHOD1(WriteBody, void(RegData data));
  MOCK_CONST_METHOD2(Read, RegData(PrivLevel priv, ContextId ctxt));
  MOCK_CONST_METHOD0(ReadBody, RegData(void));
  MOCK_CONST_METHOD0(GetName, std::string(void));

  MOCK_CONST_METHOD0(GetCfSRegId, CfSReg(void));

  MockSRegL1RCFG(ContextId ctid, ProcElementBase* pe) : SRegL1RCFG(ctid, pe) {};
};
RegData SRegL1RCFG::GetL1RSIZE(void) const { return static_cast<RegData>(0); };
RegData SRegL1RCFG::GetL1RLNKE(void) const { return static_cast<RegData>(0); };
void SRegL1RCFG::Write(RegData data, PrivLevel priv, ContextId ctxt) {};
RegData SRegL1RCFG::Read(PrivLevel priv, ContextId ctxt) const { return static_cast<RegData>(0); };


/*!
  @brief L1RLCR register mock class
 */
class MockSRegL1RLCR : public SRegL1RLCR {

public:
  MOCK_METHOD1(SetASEL, void(RegData asel));
  MOCK_METHOD1(SetCLR, void(bool clr));
  MOCK_CONST_METHOD0(GetASEL, RegData(void));
  MOCK_CONST_METHOD0(GetCLR, bool(void));

  MOCK_METHOD0(Reset, void(void));

  MOCK_METHOD3(Write, void(RegData data, PrivLevel priv, ContextId ctxt));
  MOCK_METHOD1(WriteBody, void(RegData data));
  MOCK_CONST_METHOD2(Read, RegData(PrivLevel priv, ContextId ctxt));
  MOCK_CONST_METHOD0(ReadBody, RegData(void));
  MOCK_CONST_METHOD0(GetName, std::string(void));

  MOCK_CONST_METHOD0(GetCfSRegId, CfSReg(void));

  MockSRegL1RLCR(ContextId ctid, ProcElementBase* pe) : SRegL1RLCR(ctid, pe) {};
};
void SRegL1RLCR::SetCLR(bool clr) {};
void SRegL1RLCR::Write(RegData data, PrivLevel priv, ContextId ctxt) {};
RegData SRegL1RLCR::Read(PrivLevel priv, ContextId ctxt) const { return static_cast<RegData>(0); };


/*!
  @brief L1RLNK0 register mock class
 */
class MockSRegL1RLNK0 : public SRegL1RLNK0 {

public:
  MOCK_METHOD1(SetTID, void(RegData tid));
  MOCK_METHOD1(SetBMID, void(RegData bmid));
  MOCK_METHOD1(SetSIZE, void(RegData size));
  MOCK_METHOD1(SetV, void(bool v));
  MOCK_CONST_METHOD0(GetTID, RegData(void));
  MOCK_CONST_METHOD0(GetBMID, RegData(void));
  MOCK_CONST_METHOD0(GetSIZE, RegData(void));
  MOCK_CONST_METHOD0(GetV, bool(void));

  MOCK_METHOD0(Reset, void(void));

  MOCK_METHOD3(Write, void(RegData data, PrivLevel priv, ContextId ctxt));
  MOCK_METHOD1(WriteBody, void(RegData data));
  MOCK_CONST_METHOD2(Read, RegData(PrivLevel priv, ContextId ctxt));
  MOCK_CONST_METHOD0(ReadBody, RegData(void));
  MOCK_CONST_METHOD0(GetName, std::string(void));

  MOCK_CONST_METHOD0(GetCfSRegId, CfSReg(void));

  MockSRegL1RLNK0(ContextId ctid, ProcElementBase* pe) : SRegL1RLNK0(ctid, pe) {};
};
void SRegL1RLNK0::Write(RegData data, PrivLevel priv, ContextId ctxt) {};
RegData SRegL1RLNK0::Read(PrivLevel priv, ContextId ctxt) const { return static_cast<RegData>(0); };


/*!
  @brief L1RLNK1 register mock class
 */
class MockSRegL1RLNK1 : public SRegL1RLNK1 {

public:
  MOCK_METHOD1(SetADDR, void(RegData addr));
  MOCK_CONST_METHOD0(GetADDR, RegData(void));

  MOCK_METHOD0(Reset, void(void));

  MOCK_METHOD3(Write, void(RegData data, PrivLevel priv, ContextId ctxt));
  MOCK_METHOD1(WriteBody, void(RegData data));
  MOCK_CONST_METHOD2(Read, RegData(PrivLevel priv, ContextId ctxt));
  MOCK_CONST_METHOD0(ReadBody, RegData(void));
  MOCK_CONST_METHOD0(GetName, std::string(void));

  MOCK_CONST_METHOD0(GetCfSRegId, CfSReg(void));

  MockSRegL1RLNK1(ContextId ctid, ProcElementBase* pe) : SRegL1RLNK1(ctid, pe) {};
};
void SRegL1RLNK1::Write(RegData data, PrivLevel priv, ContextId ctxt) {};
RegData SRegL1RLNK1::Read(PrivLevel priv, ContextId ctxt) const { return static_cast<RegData>(0); };


/*!
  @brief LSCFG register mock class
 */
class MockSRegLSCFG : public SRegLSCFG {

public:
  MOCK_METHOD1(SetLSE, void(bool lse));
  MOCK_CONST_METHOD0(GetLSE, bool(void));

  MOCK_METHOD0(Reset, void(void));

  MOCK_METHOD3(Write, void(RegData data, PrivLevel priv, ContextId ctxt));
  MOCK_METHOD1(WriteBody, void(RegData data));
  MOCK_CONST_METHOD2(Read, RegData(PrivLevel priv, ContextId ctxt));
  MOCK_CONST_METHOD0(ReadBody, RegData(void));
  MOCK_CONST_METHOD0(GetName, std::string(void));

  MOCK_CONST_METHOD0(GetCfSRegId, CfSReg(void));

  MockSRegLSCFG(ContextId ctid, ProcElementBase* pe) : SRegLSCFG(ctid, pe) {};
};
void SRegLSCFG::Reset(void) {};


/*!
  @brief LSTEST0 register mock class
 */
class MockSRegLSTEST0 : public SRegLSTEST0 {

public:
  MOCK_METHOD1(SetLSTEST0, void(RegData lstest0));
  MOCK_CONST_METHOD0(GetLSTEST0, RegData(void));

  MOCK_METHOD0(Reset, void(void));

  MOCK_METHOD3(Write, void(RegData data, PrivLevel priv, ContextId ctxt));
  MOCK_METHOD1(WriteBody, void(RegData data));
  MOCK_CONST_METHOD2(Read, RegData(PrivLevel priv, ContextId ctxt));
  MOCK_CONST_METHOD0(ReadBody, RegData(void));
  MOCK_CONST_METHOD0(GetName, std::string(void));

  MOCK_CONST_METHOD0(GetCfSRegId, CfSReg(void));

  MockSRegLSTEST0(ContextId ctid, ProcElementBase* pe) : SRegLSTEST0(ctid, pe) {};
};
void SRegLSTEST0::Write(RegData data, PrivLevel priv, ContextId ctxt) {};
RegData SRegLSTEST0::Read(PrivLevel priv, ContextId ctxt) const { return static_cast<RegData>(0); };


/*!
  @brief LSTEST1 register mock class
 */
class MockSRegLSTEST1 : public SRegLSTEST1 {

public:
  MOCK_METHOD1(SetLSTEST1, void(RegData lstest1));
  MOCK_CONST_METHOD0(GetLSTEST1, RegData(void));

  MOCK_METHOD0(Reset, void(void));

  MOCK_METHOD3(Write, void(RegData data, PrivLevel priv, ContextId ctxt));
  MOCK_METHOD1(WriteBody, void(RegData data));
  MOCK_CONST_METHOD2(Read, RegData(PrivLevel priv, ContextId ctxt));
  MOCK_CONST_METHOD0(ReadBody, RegData(void));
  MOCK_CONST_METHOD0(GetName, std::string(void));

  MOCK_CONST_METHOD0(GetCfSRegId, CfSReg(void));

  MockSRegLSTEST1(ContextId ctid, ProcElementBase* pe) : SRegLSTEST1(ctid, pe) {};
};
void SRegLSTEST1::Write(RegData data, PrivLevel priv, ContextId ctxt) {};
RegData SRegLSTEST1::Read(PrivLevel priv, ContextId ctxt) const { return static_cast<RegData>(0); };


/*!
  @brief LSUCR register mock class
 */
class MockSRegLSUCR : public SRegLSUCR {

public:
  MOCK_METHOD1(SetBBOPEN, void(bool bbopen));
  MOCK_CONST_METHOD0(GetBBOPEN, bool(void));

  MOCK_METHOD0(Reset, void(void));

  MOCK_METHOD3(Write, void(RegData data, PrivLevel priv, ContextId ctxt));
  MOCK_METHOD1(WriteBody, void(RegData data));
  MOCK_CONST_METHOD2(Read, RegData(PrivLevel priv, ContextId ctxt));
  MOCK_CONST_METHOD0(ReadBody, RegData(void));
  MOCK_CONST_METHOD0(GetName, std::string(void));

  MOCK_CONST_METHOD0(GetCfSRegId, CfSReg(void));

  MockSRegLSUCR(ContextId ctid, ProcElementBase* pe) : SRegLSUCR(ctid, pe) {};
};
void SRegLSUCR::Write(RegData data, PrivLevel priv, ContextId ctxt) {};
RegData SRegLSUCR::Read(PrivLevel priv, ContextId ctxt) const { return static_cast<RegData>(0); };


/*!
  @brief LSULNK0 register mock class
 */
class MockSRegLSULNK0 : public SRegLSULNK0 {

public:
  MOCK_METHOD1(SetSIZE, void(RegData size));
  MOCK_METHOD1(SetV, void(bool v));
  MOCK_CONST_METHOD0(GetSIZE, RegData(void));
  MOCK_CONST_METHOD0(GetV, bool(void));

  MOCK_METHOD0(Reset, void(void));

  MOCK_METHOD3(Write, void(RegData data, PrivLevel priv, ContextId ctxt));
  MOCK_METHOD1(WriteBody, void(RegData data));
  MOCK_CONST_METHOD2(Read, RegData(PrivLevel priv, ContextId ctxt));
  MOCK_CONST_METHOD0(ReadBody, RegData(void));
  MOCK_CONST_METHOD0(GetName, std::string(void));

  MOCK_CONST_METHOD0(GetCfSRegId, CfSReg(void));

  MockSRegLSULNK0(ContextId ctid, ProcElementBase* pe) : SRegLSULNK0(ctid, pe) {};
};
void SRegLSULNK0::Write(RegData data, PrivLevel priv, ContextId ctxt) {};
RegData SRegLSULNK0::Read(PrivLevel priv, ContextId ctxt) const { return static_cast<RegData>(0); };


/*!
  @brief LSULNK1 register mock class
 */
class MockSRegLSULNK1 : public SRegLSULNK1 {

public:
  MOCK_METHOD1(SetADDR, void(RegData addr));
  MOCK_CONST_METHOD0(GetADDR, RegData(void));

  MOCK_METHOD0(Reset, void(void));

  MOCK_METHOD3(Write, void(RegData data, PrivLevel priv, ContextId ctxt));
  MOCK_METHOD1(WriteBody, void(RegData data));
  MOCK_CONST_METHOD2(Read, RegData(PrivLevel priv, ContextId ctxt));
  MOCK_CONST_METHOD0(ReadBody, RegData(void));
  MOCK_CONST_METHOD0(GetName, std::string(void));

  MOCK_CONST_METHOD0(GetCfSRegId, CfSReg(void));

  MockSRegLSULNK1(ContextId ctid, ProcElementBase* pe) : SRegLSULNK1(ctid, pe) {};
};
void SRegLSULNK1::Write(RegData data, PrivLevel priv, ContextId ctxt) {};
RegData SRegLSULNK1::Read(PrivLevel priv, ContextId ctxt) const { return static_cast<RegData>(0); };


/*!
  @brief MCA register mock class
 */
class MockSRegMCA : public SRegMCA {

public:
  MOCK_METHOD1(SetMCA, void(RegData mca));
  MOCK_CONST_METHOD0(GetMCA, RegData(void));

  MOCK_METHOD0(Reset, void(void));

  MOCK_METHOD3(Write, void(RegData data, PrivLevel priv, ContextId ctxt));
  MOCK_METHOD1(WriteBody, void(RegData data));
  MOCK_CONST_METHOD2(Read, RegData(PrivLevel priv, ContextId ctxt));
  MOCK_CONST_METHOD0(ReadBody, RegData(void));
  MOCK_CONST_METHOD0(GetName, std::string(void));

  MOCK_CONST_METHOD0(GetCfSRegId, CfSReg(void));

  MockSRegMCA(ContextId ctid, ProcElementBase* pe) : SRegMCA(ctid, pe) {};
};


/*!
  @brief MCC register mock class
 */
class MockSRegMCC : public SRegMCC {

public:
  MOCK_METHOD1(SetMCC, void(RegData mcc));
  MOCK_CONST_METHOD0(GetMCC, RegData(void));

  MOCK_METHOD0(Reset, void(void));

  MOCK_METHOD3(Write, void(RegData data, PrivLevel priv, ContextId ctxt));
  MOCK_METHOD1(WriteBody, void(RegData data));
  MOCK_CONST_METHOD2(Read, RegData(PrivLevel priv, ContextId ctxt));
  MOCK_CONST_METHOD0(ReadBody, RegData(void));
  MOCK_CONST_METHOD0(GetName, std::string(void));

  MOCK_CONST_METHOD0(GetCfSRegId, CfSReg(void));

  MockSRegMCC(ContextId ctid, ProcElementBase* pe) : SRegMCC(ctid, pe) {};
};
void SRegMCC::Write(RegData data, PrivLevel priv, ContextId ctxt) {};


/*!
  @brief MCI register mock class
 */
class MockSRegMCI : public SRegMCI {

public:
  MOCK_METHOD1(SetSPID, void(RegData spid));
  MOCK_CONST_METHOD0(GetSPID, RegData(void));

  MOCK_METHOD0(Reset, void(void));

  MOCK_METHOD3(Write, void(RegData data, PrivLevel priv, ContextId ctxt));
  MOCK_METHOD1(WriteBody, void(RegData data));
  MOCK_CONST_METHOD2(Read, RegData(PrivLevel priv, ContextId ctxt));
  MOCK_CONST_METHOD0(ReadBody, RegData(void));
  MOCK_CONST_METHOD0(GetName, std::string(void));

  MOCK_CONST_METHOD0(GetCfSRegId, CfSReg(void));

  MockSRegMCI(ContextId ctid, ProcElementBase* pe) : SRegMCI(ctid, pe) {};
};


/*!
  @brief MCR register mock class
 */
class MockSRegMCR : public SRegMCR {

public:
  MOCK_METHOD1(SetHSXE, void(bool hsxe));
  MOCK_METHOD1(SetHSWE, void(bool hswe));
  MOCK_METHOD1(SetHSRE, void(bool hsre));
  MOCK_METHOD1(SetHUXE, void(bool huxe));
  MOCK_METHOD1(SetHUWE, void(bool huwe));
  MOCK_METHOD1(SetHURE, void(bool hure));
  MOCK_METHOD1(SetGSXE, void(bool gsxe));
  MOCK_METHOD1(SetGSWE, void(bool gswe));
  MOCK_METHOD1(SetGSRE, void(bool gsre));
  MOCK_METHOD1(SetGUXE, void(bool guxe));
  MOCK_METHOD1(SetGUWE, void(bool guwe));
  MOCK_METHOD1(SetGURE, void(bool gure));
  MOCK_METHOD1(SetOV, void(bool ov));
  MOCK_METHOD1(SetSXE, void(bool sxe));
  MOCK_METHOD1(SetSWE, void(bool swe));
  MOCK_METHOD1(SetSRE, void(bool sre));
  MOCK_METHOD1(SetUXE, void(bool uxe));
  MOCK_METHOD1(SetUWE, void(bool uwe));
  MOCK_METHOD1(SetURE, void(bool ure));
  MOCK_CONST_METHOD0(GetHSXE, bool(void));
  MOCK_CONST_METHOD0(GetHSWE, bool(void));
  MOCK_CONST_METHOD0(GetHSRE, bool(void));
  MOCK_CONST_METHOD0(GetHUXE, bool(void));
  MOCK_CONST_METHOD0(GetHUWE, bool(void));
  MOCK_CONST_METHOD0(GetHURE, bool(void));
  MOCK_CONST_METHOD0(GetGSXE, bool(void));
  MOCK_CONST_METHOD0(GetGSWE, bool(void));
  MOCK_CONST_METHOD0(GetGSRE, bool(void));
  MOCK_CONST_METHOD0(GetGUXE, bool(void));
  MOCK_CONST_METHOD0(GetGUWE, bool(void));
  MOCK_CONST_METHOD0(GetGURE, bool(void));
  MOCK_CONST_METHOD0(GetOV, bool(void));
  MOCK_CONST_METHOD0(GetSXE, bool(void));
  MOCK_CONST_METHOD0(GetSWE, bool(void));
  MOCK_CONST_METHOD0(GetSRE, bool(void));
  MOCK_CONST_METHOD0(GetUXE, bool(void));
  MOCK_CONST_METHOD0(GetUWE, bool(void));
  MOCK_CONST_METHOD0(GetURE, bool(void));

  MOCK_METHOD0(Reset, void(void));

  MOCK_METHOD3(Write, void(RegData data, PrivLevel priv, ContextId ctxt));
  MOCK_METHOD1(WriteBody, void(RegData data));
  MOCK_CONST_METHOD2(Read, RegData(PrivLevel priv, ContextId ctxt));
  MOCK_CONST_METHOD0(ReadBody, RegData(void));
  MOCK_CONST_METHOD0(GetName, std::string(void));

  MOCK_CONST_METHOD0(GetCfSRegId, CfSReg(void));

  MockSRegMCR(ContextId ctid, ProcElementBase* pe) : SRegMCR(ctid, pe) {};
};
void SRegMCR::Write(RegData data, PrivLevel priv, ContextId ctxt) {};
RegData SRegMCR::Read(PrivLevel priv, ContextId ctxt) const { return static_cast<RegData>(0); };


/*!
  @brief MCS register mock class
 */
class MockSRegMCS : public SRegMCS {

public:
  MOCK_METHOD1(SetMCS, void(RegData mcs));
  MOCK_CONST_METHOD0(GetMCS, RegData(void));

  MOCK_METHOD0(Reset, void(void));

  MOCK_METHOD3(Write, void(RegData data, PrivLevel priv, ContextId ctxt));
  MOCK_METHOD1(WriteBody, void(RegData data));
  MOCK_CONST_METHOD2(Read, RegData(PrivLevel priv, ContextId ctxt));
  MOCK_CONST_METHOD0(ReadBody, RegData(void));
  MOCK_CONST_METHOD0(GetName, std::string(void));

  MOCK_CONST_METHOD0(GetCfSRegId, CfSReg(void));

  MockSRegMCS(ContextId ctid, ProcElementBase* pe) : SRegMCS(ctid, pe) {};
};


/*!
  @brief MCTL register mock class
 */
class MockSRegMCTL : public SRegMCTL {

public:
  MOCK_METHOD1(SetUIC, void(bool uic));
  MOCK_CONST_METHOD0(GetUIC, bool(void));

  MOCK_METHOD0(Reset, void(void));

  MOCK_METHOD3(Write, void(RegData data, PrivLevel priv, ContextId ctxt));
  MOCK_METHOD1(WriteBody, void(RegData data));
  MOCK_CONST_METHOD2(Read, RegData(PrivLevel priv, ContextId ctxt));
  MOCK_CONST_METHOD0(ReadBody, RegData(void));
  MOCK_CONST_METHOD0(GetName, std::string(void));

  MOCK_CONST_METHOD0(GetCfSRegId, CfSReg(void));

  MockSRegMCTL(ContextId ctid, ProcElementBase* pe) : SRegMCTL(ctid, pe) {};
};


/*!
  @brief MEA register mock class
 */
class MockSRegHMMEA : public SRegHMMEA {

public:
  MOCK_METHOD1(SetMEA, void(RegData mea));
  MOCK_CONST_METHOD0(GetMEA, RegData(void));

  MOCK_METHOD0(Reset, void(void));

  MOCK_METHOD3(Write, void(RegData data, PrivLevel priv, ContextId ctxt));
  MOCK_METHOD1(WriteBody, void(RegData data));
  MOCK_CONST_METHOD2(Read, RegData(PrivLevel priv, ContextId ctxt));
  MOCK_CONST_METHOD0(ReadBody, RegData(void));
  MOCK_CONST_METHOD0(GetName, std::string(void));

  MOCK_CONST_METHOD0(GetCfSRegId, CfSReg(void));

  MockSRegHMMEA(ContextId ctid, ProcElementBase* pe) : SRegHMMEA(ctid, pe) {};
};


/*!
  @brief MEI register mock class
 */
class MockSRegHMMEI : public SRegHMMEI {

public:
  MOCK_METHOD1(SetLEN, void(RegData len));
  MOCK_METHOD1(SetREG, void(RegData reg));
  MOCK_METHOD1(SetDS, void(RegData ds));
  MOCK_METHOD1(SetU, void(bool u));
  MOCK_METHOD1(SetITYPE, void(RegData itype));
  MOCK_METHOD1(SetRW, void(bool rw));
  MOCK_CONST_METHOD0(GetLEN, RegData(void));
  MOCK_CONST_METHOD0(GetREG, RegData(void));
  MOCK_CONST_METHOD0(GetDS, RegData(void));
  MOCK_CONST_METHOD0(GetU, bool(void));
  MOCK_CONST_METHOD0(GetITYPE, RegData(void));
  MOCK_CONST_METHOD0(GetRW, bool(void));

  MOCK_METHOD0(Reset, void(void));

  MOCK_METHOD3(Write, void(RegData data, PrivLevel priv, ContextId ctxt));
  MOCK_METHOD1(WriteBody, void(RegData data));
  MOCK_CONST_METHOD2(Read, RegData(PrivLevel priv, ContextId ctxt));
  MOCK_CONST_METHOD0(ReadBody, RegData(void));
  MOCK_CONST_METHOD0(GetName, std::string(void));

  MOCK_CONST_METHOD0(GetCfSRegId, CfSReg(void));

  MockSRegHMMEI(ContextId ctid, ProcElementBase* pe) : SRegHMMEI(ctid, pe) {};
};
void SRegHMMEI::SetDS(RegData ds) {};


/*!
  @brief MPAT register mock class
 */
class MockSRegMPAT : public SRegMPAT {

public:
  MOCK_METHOD1(SetMPAT, void(RegData mpat));
  MOCK_CONST_METHOD0(GetMPAT, RegData(void));

  MOCK_METHOD0(Reset, void(void));

  MOCK_METHOD3(Write, void(RegData data, PrivLevel priv, ContextId ctxt));
  MOCK_METHOD1(WriteBody, void(RegData data));
  MOCK_CONST_METHOD2(Read, RegData(PrivLevel priv, ContextId ctxt));
  MOCK_CONST_METHOD0(ReadBody, RegData(void));
  MOCK_CONST_METHOD0(GetName, std::string(void));

  MOCK_CONST_METHOD0(GetCfSRegId, CfSReg(void));

  MockSRegMPAT(ContextId ctid, ProcElementBase* pe) : SRegMPAT(ctid, pe) {};
};
void SRegMPAT::Write(RegData data, PrivLevel priv, ContextId ctxt) {};
RegData SRegMPAT::Read(PrivLevel priv, ContextId ctxt) const { return static_cast<RegData>(0); };


/*!
  @brief MPAT_N register mock class
 */
class MockSRegMPAT_N : public SRegMPAT_N {
  uint32_t m_id;
  uint32_t m_bank_id;

public:
  MOCK_METHOD1(SetWMPID, void(RegData wmpid));
  MOCK_METHOD1(SetRMPID, void(RegData rmpid));
  MOCK_METHOD1(SetWG, void(bool wg));
  MOCK_METHOD1(SetRG, void(bool rg));
  MOCK_METHOD1(SetE, void(bool e));
  MOCK_METHOD1(SetSX, void(bool sx));
  MOCK_METHOD1(SetSW, void(bool sw));
  MOCK_METHOD1(SetSR, void(bool sr));
  MOCK_METHOD1(SetUX, void(bool ux));
  MOCK_METHOD1(SetUW, void(bool uw));
  MOCK_METHOD1(SetUR, void(bool ur));
  MOCK_CONST_METHOD0(GetWMPID, RegData(void));
  MOCK_CONST_METHOD0(GetRMPID, RegData(void));
  MOCK_CONST_METHOD0(GetWG, bool(void));
  MOCK_CONST_METHOD0(GetRG, bool(void));
  MOCK_CONST_METHOD0(GetE, bool(void));
  MOCK_CONST_METHOD0(GetSX, bool(void));
  MOCK_CONST_METHOD0(GetSW, bool(void));
  MOCK_CONST_METHOD0(GetSR, bool(void));
  MOCK_CONST_METHOD0(GetUX, bool(void));
  MOCK_CONST_METHOD0(GetUW, bool(void));
  MOCK_CONST_METHOD0(GetUR, bool(void));

  MOCK_METHOD0(Reset, void(void));

  MOCK_METHOD3(Write, void(RegData data, PrivLevel priv, ContextId ctxt));
  MOCK_METHOD1(WriteBody, void(RegData data));
  MOCK_CONST_METHOD2(Read, RegData(PrivLevel priv, ContextId ctxt));
  MOCK_CONST_METHOD0(ReadBody, RegData(void));
  std::string GetName(void) const {
     std::ostringstream oss;
     oss << "MPAT" << m_id;
     if (m_bank_id != 0) {
         oss << "_B" << m_bank_id;
     }
     return oss.str();
  }

  MOCK_CONST_METHOD0(GetCfSRegId, CfSReg(void));

  MockSRegMPAT_N(ContextId ctid, ProcElementBase* pe, uint32_t id, uint32_t bank_id) : SRegMPAT_N(ctid, pe, id, bank_id) {};
};
void SRegMPAT_N::SetE(bool e) {};
void SRegMPAT_N::Reset(void) {};
void SRegMPAT_N::Write(RegData data, PrivLevel priv, ContextId ctxt) {};
RegData SRegMPAT_N::Read(PrivLevel priv, ContextId ctxt) const { return static_cast<RegData>(0); };


/*!
  @brief MPBK register mock class
 */
class MockSRegMPBK : public SRegMPBK {

public:
  MOCK_METHOD1(SetBK, void(bool bk));
  MOCK_CONST_METHOD0(GetBK, bool(void));

  MOCK_METHOD0(Reset, void(void));

  MOCK_METHOD3(Write, void(RegData data, PrivLevel priv, ContextId ctxt));
  MOCK_METHOD1(WriteBody, void(RegData data));
  MOCK_CONST_METHOD2(Read, RegData(PrivLevel priv, ContextId ctxt));
  MOCK_CONST_METHOD0(ReadBody, RegData(void));
  MOCK_CONST_METHOD0(GetName, std::string(void));

  MOCK_CONST_METHOD0(GetCfSRegId, CfSReg(void));

  MockSRegMPBK(ContextId ctid, ProcElementBase* pe) : SRegMPBK(ctid, pe) {};
};
void SRegMPBK::SetBK(bool bk) {};
void SRegMPBK::Reset(void) {};
void SRegMPBK::Write(RegData data, PrivLevel priv, ContextId ctxt) {};
RegData SRegMPBK::Read(PrivLevel priv, ContextId ctxt) const { return static_cast<RegData>(0); };


/*!
  @brief MPCFG register mock class
 */
class MockSRegMPCFG : public SRegMPCFG {

public:
  MOCK_METHOD1(SetDMDP, void(bool dmdp));
  MOCK_METHOD1(SetNBK, void(RegData nbk));
  MOCK_METHOD1(SetARCH, void(RegData arch));
  MOCK_METHOD1(SetHBE, void(RegData hbe));
  MOCK_METHOD1(SetNMPUE, void(RegData nmpue));
  MOCK_CONST_METHOD0(GetDMDP, bool(void));
  MOCK_CONST_METHOD0(GetNBK, RegData(void));
  MOCK_CONST_METHOD0(GetARCH, RegData(void));
  MOCK_CONST_METHOD0(GetHBE, RegData(void));
  MOCK_CONST_METHOD0(GetNMPUE, RegData(void));

  MOCK_METHOD0(Reset, void(void));

  MOCK_METHOD3(Write, void(RegData data, PrivLevel priv, ContextId ctxt));
  MOCK_METHOD1(WriteBody, void(RegData data));
  MOCK_CONST_METHOD2(Read, RegData(PrivLevel priv, ContextId ctxt));
  MOCK_CONST_METHOD0(ReadBody, RegData(void));
  MOCK_CONST_METHOD0(GetName, std::string(void));

  MOCK_CONST_METHOD0(GetCfSRegId, CfSReg(void));

  MockSRegMPCFG(ContextId ctid, ProcElementBase* pe) : SRegMPCFG(ctid, pe) {};
};
void SRegMPCFG::SetDMDP(bool dmdp) {};
bool SRegMPCFG::GetDMDP(void) const { return static_cast<bool>(0); };
void SRegMPCFG::Reset(void) {};
void SRegMPCFG::Write(RegData data, PrivLevel priv, ContextId ctxt) {};
RegData SRegMPCFG::Read(PrivLevel priv, ContextId ctxt) const { return static_cast<RegData>(0); };


/*!
  @brief MPID register mock class
 */
class MockSRegMPID : public SRegMPID {
  uint32_t m_id;

public:
  MOCK_METHOD1(SetSPID, void(RegData spid));
  MOCK_CONST_METHOD0(GetSPID, RegData(void));

  MOCK_METHOD0(Reset, void(void));

  MOCK_METHOD3(Write, void(RegData data, PrivLevel priv, ContextId ctxt));
  MOCK_METHOD1(WriteBody, void(RegData data));
  MOCK_CONST_METHOD2(Read, RegData(PrivLevel priv, ContextId ctxt));
  MOCK_CONST_METHOD0(ReadBody, RegData(void));
  std::string GetName(void) const {
     std::ostringstream oss;
     oss << "MPID" << m_id;
     return oss.str();
  }

  MOCK_CONST_METHOD0(GetCfSRegId, CfSReg(void));

  MockSRegMPID(ContextId ctid, ProcElementBase* pe, uint32_t id) : SRegMPID(ctid, pe, id) {};
};
void SRegMPID::SetSPID(RegData spid) {};
void SRegMPID::Write(RegData data, PrivLevel priv, ContextId ctxt) {};


/*!
  @brief MPIDX register mock class
 */
class MockSRegMPIDX : public SRegMPIDX {

public:
  MOCK_METHOD1(SetMPIDX, void(RegData mpidx));
  MOCK_CONST_METHOD0(GetMPIDX, RegData(void));

  MOCK_METHOD0(Reset, void(void));

  MOCK_METHOD3(Write, void(RegData data, PrivLevel priv, ContextId ctxt));
  MOCK_METHOD1(WriteBody, void(RegData data));
  MOCK_CONST_METHOD2(Read, RegData(PrivLevel priv, ContextId ctxt));
  MOCK_CONST_METHOD0(ReadBody, RegData(void));
  MOCK_CONST_METHOD0(GetName, std::string(void));

  MOCK_CONST_METHOD0(GetCfSRegId, CfSReg(void));

  MockSRegMPIDX(ContextId ctid, ProcElementBase* pe) : SRegMPIDX(ctid, pe) {};
};


/*!
  @brief MPLA register mock class
 */
class MockSRegMPLA : public SRegMPLA {

public:
  MOCK_METHOD1(SetMPLA, void(RegData mpla));
  MOCK_CONST_METHOD0(GetMPLA, RegData(void));

  MOCK_METHOD0(Reset, void(void));

  MOCK_METHOD3(Write, void(RegData data, PrivLevel priv, ContextId ctxt));
  MOCK_METHOD1(WriteBody, void(RegData data));
  MOCK_CONST_METHOD2(Read, RegData(PrivLevel priv, ContextId ctxt));
  MOCK_CONST_METHOD0(ReadBody, RegData(void));
  MOCK_CONST_METHOD0(GetName, std::string(void));

  MOCK_CONST_METHOD0(GetCfSRegId, CfSReg(void));

  MockSRegMPLA(ContextId ctid, ProcElementBase* pe) : SRegMPLA(ctid, pe) {};
};
void SRegMPLA::Write(RegData data, PrivLevel priv, ContextId ctxt) {};
RegData SRegMPLA::Read(PrivLevel priv, ContextId ctxt) const { return static_cast<RegData>(0); };


/*!
  @brief MPLA_N register mock class
 */
class MockSRegMPLA_N : public SRegMPLA_N {
  uint32_t m_id;
  uint32_t m_bank_id;

public:
  MOCK_METHOD1(SetMPLA, void(RegData mpla));
  MOCK_CONST_METHOD0(GetMPLA, RegData(void));

  MOCK_METHOD0(Reset, void(void));

  MOCK_METHOD3(Write, void(RegData data, PrivLevel priv, ContextId ctxt));
  MOCK_METHOD1(WriteBody, void(RegData data));
  MOCK_CONST_METHOD2(Read, RegData(PrivLevel priv, ContextId ctxt));
  MOCK_CONST_METHOD0(ReadBody, RegData(void));
  std::string GetName(void) const {
     std::ostringstream oss;
     oss << "MPLA" << m_id;
     if (m_bank_id != 0) {
         oss << "_B" << m_bank_id;
     }
     return oss.str();
  }

  MOCK_CONST_METHOD0(GetCfSRegId, CfSReg(void));

  MockSRegMPLA_N(ContextId ctid, ProcElementBase* pe, uint32_t id, uint32_t bank_id) : SRegMPLA_N(ctid, pe, id, bank_id) {};
};
void SRegMPLA_N::Write(RegData data, PrivLevel priv, ContextId ctxt) {};
RegData SRegMPLA_N::Read(PrivLevel priv, ContextId ctxt) const { return static_cast<RegData>(0); };


/*!
  @brief MPM register mock class
 */
class MockSRegHMMPM : public SRegHMMPM {

public:
  MOCK_METHOD1(SetGMPE, void(bool gmpe));
  MOCK_METHOD1(SetSVP, void(bool svp));
  MOCK_METHOD1(SetMPE, void(bool mpe));
  MOCK_CONST_METHOD0(GetGMPE, bool(void));
  MOCK_CONST_METHOD0(GetSVP, bool(void));
  MOCK_CONST_METHOD0(GetMPE, bool(void));

  MOCK_METHOD0(Reset, void(void));

  MOCK_METHOD3(Write, void(RegData data, PrivLevel priv, ContextId ctxt));
  MOCK_METHOD1(WriteBody, void(RegData data));
  MOCK_CONST_METHOD2(Read, RegData(PrivLevel priv, ContextId ctxt));
  MOCK_CONST_METHOD0(ReadBody, RegData(void));
  MOCK_CONST_METHOD0(GetName, std::string(void));

  MOCK_CONST_METHOD0(GetCfSRegId, CfSReg(void));

  MockSRegHMMPM(ContextId ctid, ProcElementBase* pe) : SRegHMMPM(ctid, pe) {};
};
void SRegHMMPM::SetMPE(bool mpe) {};
bool SRegHMMPM::GetSVP(void) const { return static_cast<bool>(0); };
bool SRegHMMPM::GetMPE(void) const { return static_cast<bool>(0); };
void SRegHMMPM::Write(RegData data, PrivLevel priv, ContextId ctxt) {};


/*!
  @brief MPUA register mock class
 */
class MockSRegMPUA : public SRegMPUA {

public:
  MOCK_METHOD1(SetMPUA, void(RegData mpua));
  MOCK_CONST_METHOD0(GetMPUA, RegData(void));

  MOCK_METHOD0(Reset, void(void));

  MOCK_METHOD3(Write, void(RegData data, PrivLevel priv, ContextId ctxt));
  MOCK_METHOD1(WriteBody, void(RegData data));
  MOCK_CONST_METHOD2(Read, RegData(PrivLevel priv, ContextId ctxt));
  MOCK_CONST_METHOD0(ReadBody, RegData(void));
  MOCK_CONST_METHOD0(GetName, std::string(void));

  MOCK_CONST_METHOD0(GetCfSRegId, CfSReg(void));

  MockSRegMPUA(ContextId ctid, ProcElementBase* pe) : SRegMPUA(ctid, pe) {};
};
void SRegMPUA::Write(RegData data, PrivLevel priv, ContextId ctxt) {};
RegData SRegMPUA::Read(PrivLevel priv, ContextId ctxt) const { return static_cast<RegData>(0); };


/*!
  @brief MPUA_N register mock class
 */
class MockSRegMPUA_N : public SRegMPUA_N {
  uint32_t m_id;
  uint32_t m_bank_id;

public:
  MOCK_METHOD1(SetMPUA, void(RegData mpua));
  MOCK_CONST_METHOD0(GetMPUA, RegData(void));

  MOCK_METHOD0(Reset, void(void));

  MOCK_METHOD3(Write, void(RegData data, PrivLevel priv, ContextId ctxt));
  MOCK_METHOD1(WriteBody, void(RegData data));
  MOCK_CONST_METHOD2(Read, RegData(PrivLevel priv, ContextId ctxt));
  MOCK_CONST_METHOD0(ReadBody, RegData(void));
  std::string GetName(void) const {
     std::ostringstream oss;
     oss << "MPUA" << m_id;
     if (m_bank_id != 0) {
         oss << "_B" << m_bank_id;
     }
     return oss.str();
  }

  MOCK_CONST_METHOD0(GetCfSRegId, CfSReg(void));

  MockSRegMPUA_N(ContextId ctid, ProcElementBase* pe, uint32_t id, uint32_t bank_id) : SRegMPUA_N(ctid, pe, id, bank_id) {};
};
void SRegMPUA_N::Write(RegData data, PrivLevel priv, ContextId ctxt) {};
RegData SRegMPUA_N::Read(PrivLevel priv, ContextId ctxt) const { return static_cast<RegData>(0); };


/*!
  @brief PEID register mock class
 */
class MockSRegHMPEID : public SRegHMPEID {

public:
  MOCK_METHOD1(SetPEID, void(RegData peid));
  MOCK_CONST_METHOD0(GetPEID, RegData(void));

  MOCK_METHOD0(Reset, void(void));

  MOCK_METHOD3(Write, void(RegData data, PrivLevel priv, ContextId ctxt));
  MOCK_METHOD1(WriteBody, void(RegData data));
  MOCK_CONST_METHOD2(Read, RegData(PrivLevel priv, ContextId ctxt));
  MOCK_CONST_METHOD0(ReadBody, RegData(void));
  MOCK_CONST_METHOD0(GetName, std::string(void));

  MOCK_CONST_METHOD0(GetCfSRegId, CfSReg(void));

  MockSRegHMPEID(ContextId ctid, ProcElementBase* pe) : SRegHMPEID(ctid, pe) {};
};
void SRegHMPEID::Reset(void) {};
void SRegHMPEID::Write(RegData data, PrivLevel priv, ContextId ctxt) {};
RegData SRegHMPEID::Read(PrivLevel priv, ContextId ctxt) const { return static_cast<RegData>(0); };


/*!
  @brief PID register mock class
 */
class MockSRegPID : public SRegPID {

public:
  MOCK_METHOD1(SetPID, void(RegData pid));
  MOCK_METHOD1(SetFID, void(RegData fid));
  MOCK_METHOD1(SetVID, void(RegData vid));
  MOCK_CONST_METHOD0(GetPID, RegData(void));
  MOCK_CONST_METHOD0(GetFID, RegData(void));
  MOCK_CONST_METHOD0(GetVID, RegData(void));

  MOCK_METHOD0(Reset, void(void));

  MOCK_METHOD3(Write, void(RegData data, PrivLevel priv, ContextId ctxt));
  MOCK_METHOD1(WriteBody, void(RegData data));
  MOCK_CONST_METHOD2(Read, RegData(PrivLevel priv, ContextId ctxt));
  MOCK_CONST_METHOD0(ReadBody, RegData(void));
  MOCK_CONST_METHOD0(GetName, std::string(void));

  MOCK_CONST_METHOD0(GetCfSRegId, CfSReg(void));

  MockSRegPID(ContextId ctid, ProcElementBase* pe) : SRegPID(ctid, pe) {};
};
void SRegPID::Reset(void) {};


/*!
  @brief PLMR register mock class
 */
class MockSRegHMPLMR : public SRegHMPLMR {

public:
  MOCK_METHOD1(SetPLM, void(RegData plm));
  MOCK_CONST_METHOD0(GetPLM, RegData(void));

  MOCK_METHOD0(Reset, void(void));

  MOCK_METHOD3(Write, void(RegData data, PrivLevel priv, ContextId ctxt));
  MOCK_METHOD1(WriteBody, void(RegData data));
  MOCK_CONST_METHOD2(Read, RegData(PrivLevel priv, ContextId ctxt));
  MOCK_CONST_METHOD0(ReadBody, RegData(void));
  MOCK_CONST_METHOD0(GetName, std::string(void));

  MOCK_CONST_METHOD0(GetCfSRegId, CfSReg(void));

  MockSRegHMPLMR(ContextId ctid, ProcElementBase* pe) : SRegHMPLMR(ctid, pe) {};
};
void SRegHMPLMR::SetPLM(RegData plm) {};


/*!
  @brief PMCOUNT register mock class
 */
class MockSRegPMCOUNT : public SRegPMCOUNT {
  uint32_t m_id;

public:
  MOCK_METHOD1(SetPMCNT, void(RegData pmcnt));
  MOCK_CONST_METHOD0(GetPMCNT, RegData(void));

  MOCK_METHOD0(Reset, void(void));

  MOCK_METHOD3(Write, void(RegData data, PrivLevel priv, ContextId ctxt));
  MOCK_METHOD1(WriteBody, void(RegData data));
  MOCK_CONST_METHOD2(Read, RegData(PrivLevel priv, ContextId ctxt));
  MOCK_CONST_METHOD0(ReadBody, RegData(void));
  std::string GetName(void) const {
     std::ostringstream oss;
     oss << "PMCOUNT" << m_id;
     return oss.str();
  }

  MOCK_CONST_METHOD0(GetCfSRegId, CfSReg(void));

  MockSRegPMCOUNT(ContextId ctid, ProcElementBase* pe, uint32_t id) : SRegPMCOUNT(ctid, pe, id) {};
};
void SRegPMCOUNT::Write(RegData data, PrivLevel priv, ContextId ctxt) {};
RegData SRegPMCOUNT::Read(PrivLevel priv, ContextId ctxt) const { return static_cast<RegData>(0); };


/*!
  @brief PMCTRL register mock class
 */
class MockSRegPMCTRL : public SRegPMCTRL {
  uint32_t m_id;

public:
  MOCK_METHOD1(SetHE, void(bool he));
  MOCK_METHOD1(SetGE, void(RegData ge));
  MOCK_METHOD1(SetCND, void(RegData cnd));
  MOCK_METHOD1(SetOVF, void(bool ovf));
  MOCK_METHOD1(SetCEN, void(bool cen));
  MOCK_CONST_METHOD0(GetHE, bool(void));
  MOCK_CONST_METHOD0(GetGE, RegData(void));
  MOCK_CONST_METHOD0(GetCND, RegData(void));
  MOCK_CONST_METHOD0(GetOVF, bool(void));
  MOCK_CONST_METHOD0(GetCEN, bool(void));

  MOCK_METHOD0(Reset, void(void));

  MOCK_METHOD3(Write, void(RegData data, PrivLevel priv, ContextId ctxt));
  MOCK_METHOD1(WriteBody, void(RegData data));
  MOCK_CONST_METHOD2(Read, RegData(PrivLevel priv, ContextId ctxt));
  MOCK_CONST_METHOD0(ReadBody, RegData(void));
  std::string GetName(void) const {
     std::ostringstream oss;
     oss << "PMCTRL" << m_id;
     return oss.str();
  }

  MOCK_CONST_METHOD0(GetCfSRegId, CfSReg(void));

  MockSRegPMCTRL(ContextId ctid, ProcElementBase* pe, uint32_t id) : SRegPMCTRL(ctid, pe, id) {};
};
void SRegPMCTRL::Write(RegData data, PrivLevel priv, ContextId ctxt) {};
RegData SRegPMCTRL::Read(PrivLevel priv, ContextId ctxt) const { return static_cast<RegData>(0); };


/*!
  @brief PMGMCTRL register mock class
 */
class MockSRegPMGMCTRL : public SRegPMGMCTRL {

public:
  MOCK_METHOD1(SetPMGE, void(RegData pmge));
  MOCK_CONST_METHOD0(GetPMGE, RegData(void));

  MOCK_METHOD0(Reset, void(void));

  MOCK_METHOD3(Write, void(RegData data, PrivLevel priv, ContextId ctxt));
  MOCK_METHOD1(WriteBody, void(RegData data));
  MOCK_CONST_METHOD2(Read, RegData(PrivLevel priv, ContextId ctxt));
  MOCK_CONST_METHOD0(ReadBody, RegData(void));
  MOCK_CONST_METHOD0(GetName, std::string(void));

  MOCK_CONST_METHOD0(GetCfSRegId, CfSReg(void));

  MockSRegPMGMCTRL(ContextId ctid, ProcElementBase* pe) : SRegPMGMCTRL(ctid, pe) {};
};


/*!
  @brief PMSUBCND register mock class
 */
class MockSRegPMSUBCND : public SRegPMSUBCND {
  uint32_t m_id;

public:
  MOCK_METHOD1(SetSUBCND, void(RegData subcnd));
  MOCK_CONST_METHOD0(GetSUBCND, RegData(void));

  MOCK_METHOD0(Reset, void(void));

  MOCK_METHOD3(Write, void(RegData data, PrivLevel priv, ContextId ctxt));
  MOCK_METHOD1(WriteBody, void(RegData data));
  MOCK_CONST_METHOD2(Read, RegData(PrivLevel priv, ContextId ctxt));
  MOCK_CONST_METHOD0(ReadBody, RegData(void));
  std::string GetName(void) const {
     std::ostringstream oss;
     oss << "PMSUBCND" << m_id;
     return oss.str();
  }

  MOCK_CONST_METHOD0(GetCfSRegId, CfSReg(void));

  MockSRegPMSUBCND(ContextId ctid, ProcElementBase* pe, uint32_t id) : SRegPMSUBCND(ctid, pe, id) {};
};
void SRegPMSUBCND::Write(RegData data, PrivLevel priv, ContextId ctxt) {};
RegData SRegPMSUBCND::Read(PrivLevel priv, ContextId ctxt) const { return static_cast<RegData>(0); };


/*!
  @brief PMUMCTRL register mock class
 */
class MockSRegPMUMCTRL : public SRegPMUMCTRL {

public:
  MOCK_METHOD1(SetPMUE, void(RegData pmue));
  MOCK_CONST_METHOD0(GetPMUE, RegData(void));

  MOCK_METHOD0(Reset, void(void));

  MOCK_METHOD3(Write, void(RegData data, PrivLevel priv, ContextId ctxt));
  MOCK_METHOD1(WriteBody, void(RegData data));
  MOCK_CONST_METHOD2(Read, RegData(PrivLevel priv, ContextId ctxt));
  MOCK_CONST_METHOD0(ReadBody, RegData(void));
  MOCK_CONST_METHOD0(GetName, std::string(void));

  MOCK_CONST_METHOD0(GetCfSRegId, CfSReg(void));

  MockSRegPMUMCTRL(ContextId ctid, ProcElementBase* pe) : SRegPMUMCTRL(ctid, pe) {};
};
void SRegPMUMCTRL::Write(RegData data, PrivLevel priv, ContextId ctxt) {};


/*!
  @brief PSW register mock class
 */
class MockSRegHMPSW : public SRegHMPSW {

public:
  MOCK_METHOD1(SetUM, void(bool um));
  MOCK_METHOD1(SetEIMASK, void(RegData eimask));
  MOCK_METHOD1(SetCU, void(RegData cu));
  MOCK_METHOD1(SetEBV, void(bool ebv));
  MOCK_METHOD1(SetNP, void(bool np));
  MOCK_METHOD1(SetEP, void(bool ep));
  MOCK_METHOD1(SetID, void(bool id));
  MOCK_METHOD1(SetSAT, void(bool sat));
  MOCK_METHOD1(SetCY, void(bool cy));
  MOCK_METHOD1(SetOV, void(bool ov));
  MOCK_METHOD1(SetS, void(bool s));
  MOCK_METHOD1(SetZ, void(bool z));
  MOCK_CONST_METHOD0(GetUM, bool(void));
  MOCK_CONST_METHOD0(GetEIMASK, RegData(void));
  MOCK_CONST_METHOD0(GetCU, RegData(void));
  MOCK_CONST_METHOD0(GetEBV, bool(void));
  MOCK_CONST_METHOD0(GetNP, bool(void));
  MOCK_CONST_METHOD0(GetEP, bool(void));
  MOCK_CONST_METHOD0(GetID, bool(void));
  MOCK_CONST_METHOD0(GetSAT, bool(void));
  MOCK_CONST_METHOD0(GetCY, bool(void));
  MOCK_CONST_METHOD0(GetOV, bool(void));
  MOCK_CONST_METHOD0(GetS, bool(void));
  MOCK_CONST_METHOD0(GetZ, bool(void));

  MOCK_METHOD0(Reset, void(void));

  MOCK_METHOD3(Write, void(RegData data, PrivLevel priv, ContextId ctxt));
  MOCK_METHOD1(WriteBody, void(RegData data));
  MOCK_CONST_METHOD2(Read, RegData(PrivLevel priv, ContextId ctxt));
  MOCK_CONST_METHOD0(ReadBody, RegData(void));
  MOCK_CONST_METHOD0(GetName, std::string(void));

  MOCK_CONST_METHOD0(GetCfSRegId, CfSReg(void));

  MockSRegHMPSW(ContextId ctid, ProcElementBase* pe) : SRegHMPSW(ctid, pe) {};
};
void SRegHMPSW::SetUM(bool um) {};
void SRegHMPSW::SetEIMASK(RegData eimask) {};
void SRegHMPSW::SetCU(RegData cu) {};
void SRegHMPSW::SetNP(bool np) {};
void SRegHMPSW::SetID(bool id) {};
RegData SRegHMPSW::GetEIMASK(void) const { return static_cast<RegData>(0); };
void SRegHMPSW::Write(RegData data, PrivLevel priv, ContextId ctxt) {};
RegData SRegHMPSW::Read(PrivLevel priv, ContextId ctxt) const { return static_cast<RegData>(0); };


/*!
  @brief PSWH register mock class
 */
class MockSRegPSWH : public SRegPSWH {

public:
  MOCK_METHOD1(SetGM, void(bool gm));
  MOCK_METHOD1(SetGPID, void(RegData gpid));
  MOCK_CONST_METHOD0(GetGM, bool(void));
  MOCK_CONST_METHOD0(GetGPID, RegData(void));

  MOCK_METHOD0(Reset, void(void));

  MOCK_METHOD3(Write, void(RegData data, PrivLevel priv, ContextId ctxt));
  MOCK_METHOD1(WriteBody, void(RegData data));
  MOCK_CONST_METHOD2(Read, RegData(PrivLevel priv, ContextId ctxt));
  MOCK_CONST_METHOD0(ReadBody, RegData(void));
  MOCK_CONST_METHOD0(GetName, std::string(void));

  MOCK_CONST_METHOD0(GetCfSRegId, CfSReg(void));

  MockSRegPSWH(ContextId ctid, ProcElementBase* pe) : SRegPSWH(ctid, pe) {};
};
void SRegPSWH::SetGM(bool gm) {};
void SRegPSWH::SetGPID(RegData gpid) {};
void SRegPSWH::Reset(void) {};
void SRegPSWH::Write(RegData data, PrivLevel priv, ContextId ctxt) {};


/*!
  @brief RBASE register mock class
 */
class MockSRegRBASE : public SRegRBASE {

public:
  MOCK_METHOD1(SetRBASE, void(RegData rbase));
  MOCK_METHOD1(SetDV, void(bool dv));
  MOCK_METHOD1(SetRINT, void(bool rint));
  MOCK_CONST_METHOD0(GetRBASE, RegData(void));
  MOCK_CONST_METHOD0(GetDV, bool(void));
  MOCK_CONST_METHOD0(GetRINT, bool(void));

  MOCK_METHOD0(Reset, void(void));

  MOCK_METHOD3(Write, void(RegData data, PrivLevel priv, ContextId ctxt));
  MOCK_METHOD1(WriteBody, void(RegData data));
  MOCK_CONST_METHOD2(Read, RegData(PrivLevel priv, ContextId ctxt));
  MOCK_CONST_METHOD0(ReadBody, RegData(void));
  MOCK_CONST_METHOD0(GetName, std::string(void));

  MOCK_CONST_METHOD0(GetCfSRegId, CfSReg(void));

  MockSRegRBASE(ContextId ctid, ProcElementBase* pe) : SRegRBASE(ctid, pe) {};
};
void SRegRBASE::Reset(void) {};


/*!
  @brief RBCR0 register mock class
 */
class MockSRegRBCR0 : public SRegRBCR0 {

public:
  MOCK_METHOD1(SetMD, void(bool md));
  MOCK_METHOD1(SetBE, void(RegData be));
  MOCK_CONST_METHOD0(GetMD, bool(void));
  MOCK_CONST_METHOD0(GetBE, RegData(void));

  MOCK_METHOD0(Reset, void(void));

  MOCK_METHOD3(Write, void(RegData data, PrivLevel priv, ContextId ctxt));
  MOCK_METHOD1(WriteBody, void(RegData data));
  MOCK_CONST_METHOD2(Read, RegData(PrivLevel priv, ContextId ctxt));
  MOCK_CONST_METHOD0(ReadBody, RegData(void));
  MOCK_CONST_METHOD0(GetName, std::string(void));

  MOCK_CONST_METHOD0(GetCfSRegId, CfSReg(void));

  MockSRegRBCR0(ContextId ctid, ProcElementBase* pe) : SRegRBCR0(ctid, pe) {};
};


/*!
  @brief RBCR1 register mock class
 */
class MockSRegRBCR1 : public SRegRBCR1 {

public:
  MOCK_METHOD1(SetNC, void(RegData nc));
  MOCK_CONST_METHOD0(GetNC, RegData(void));

  MOCK_METHOD0(Reset, void(void));

  MOCK_METHOD3(Write, void(RegData data, PrivLevel priv, ContextId ctxt));
  MOCK_METHOD1(WriteBody, void(RegData data));
  MOCK_CONST_METHOD2(Read, RegData(PrivLevel priv, ContextId ctxt));
  MOCK_CONST_METHOD0(ReadBody, RegData(void));
  MOCK_CONST_METHOD0(GetName, std::string(void));

  MOCK_CONST_METHOD0(GetCfSRegId, CfSReg(void));

  MockSRegRBCR1(ContextId ctid, ProcElementBase* pe) : SRegRBCR1(ctid, pe) {};
};


/*!
  @brief RBIP register mock class
 */
class MockSRegRBIP : public SRegRBIP {

public:
  MOCK_METHOD1(SetRBIP, void(RegData rbip));
  MOCK_CONST_METHOD0(GetRBIP, RegData(void));

  MOCK_METHOD0(Reset, void(void));

  MOCK_METHOD3(Write, void(RegData data, PrivLevel priv, ContextId ctxt));
  MOCK_METHOD1(WriteBody, void(RegData data));
  MOCK_CONST_METHOD2(Read, RegData(PrivLevel priv, ContextId ctxt));
  MOCK_CONST_METHOD0(ReadBody, RegData(void));
  MOCK_CONST_METHOD0(GetName, std::string(void));

  MOCK_CONST_METHOD0(GetCfSRegId, CfSReg(void));

  MockSRegRBIP(ContextId ctid, ProcElementBase* pe) : SRegRBIP(ctid, pe) {};
};


/*!
  @brief RBNR register mock class
 */
class MockSRegRBNR : public SRegRBNR {

public:
  MOCK_METHOD1(SetBN, void(RegData bn));
  MOCK_CONST_METHOD0(GetBN, RegData(void));

  MOCK_METHOD0(Reset, void(void));

  MOCK_METHOD3(Write, void(RegData data, PrivLevel priv, ContextId ctxt));
  MOCK_METHOD1(WriteBody, void(RegData data));
  MOCK_CONST_METHOD2(Read, RegData(PrivLevel priv, ContextId ctxt));
  MOCK_CONST_METHOD0(ReadBody, RegData(void));
  MOCK_CONST_METHOD0(GetName, std::string(void));

  MOCK_CONST_METHOD0(GetCfSRegId, CfSReg(void));

  MockSRegRBNR(ContextId ctid, ProcElementBase* pe) : SRegRBNR(ctid, pe) {};
};
void SRegRBNR::SetBN(RegData bn) {};


/*!
  @brief RDBACR register mock class
 */
class MockSRegRDBACR : public SRegRDBACR {

public:
  MOCK_METHOD1(SetENTRY, void(RegData entry));
  MOCK_METHOD1(SetULSEL, void(bool ulsel));
  MOCK_CONST_METHOD0(GetENTRY, RegData(void));
  MOCK_CONST_METHOD0(GetULSEL, bool(void));

  MOCK_METHOD0(Reset, void(void));

  MOCK_METHOD3(Write, void(RegData data, PrivLevel priv, ContextId ctxt));
  MOCK_METHOD1(WriteBody, void(RegData data));
  MOCK_CONST_METHOD2(Read, RegData(PrivLevel priv, ContextId ctxt));
  MOCK_CONST_METHOD0(ReadBody, RegData(void));
  MOCK_CONST_METHOD0(GetName, std::string(void));

  MOCK_CONST_METHOD0(GetCfSRegId, CfSReg(void));

  MockSRegRDBACR(ContextId ctid, ProcElementBase* pe) : SRegRDBACR(ctid, pe) {};
};
void SRegRDBACR::Write(RegData data, PrivLevel priv, ContextId ctxt) {};
RegData SRegRDBACR::Read(PrivLevel priv, ContextId ctxt) const { return static_cast<RegData>(0); };


/*!
  @brief RDBADAT0 register mock class
 */
class MockSRegRDBADAT0 : public SRegRDBADAT0 {

public:
  MOCK_METHOD1(SetDATA, void(RegData data));
  MOCK_CONST_METHOD0(GetDATA, RegData(void));

  MOCK_METHOD0(Reset, void(void));

  MOCK_METHOD3(Write, void(RegData data, PrivLevel priv, ContextId ctxt));
  MOCK_METHOD1(WriteBody, void(RegData data));
  MOCK_CONST_METHOD2(Read, RegData(PrivLevel priv, ContextId ctxt));
  MOCK_CONST_METHOD0(ReadBody, RegData(void));
  MOCK_CONST_METHOD0(GetName, std::string(void));

  MOCK_CONST_METHOD0(GetCfSRegId, CfSReg(void));

  MockSRegRDBADAT0(ContextId ctid, ProcElementBase* pe) : SRegRDBADAT0(ctid, pe) {};
};
void SRegRDBADAT0::Write(RegData data, PrivLevel priv, ContextId ctxt) {};
RegData SRegRDBADAT0::Read(PrivLevel priv, ContextId ctxt) const { return static_cast<RegData>(0); };


/*!
  @brief RDBADAT1 register mock class
 */
class MockSRegRDBADAT1 : public SRegRDBADAT1 {

public:
  MOCK_METHOD1(SetDATA, void(RegData data));
  MOCK_CONST_METHOD0(GetDATA, RegData(void));

  MOCK_METHOD0(Reset, void(void));

  MOCK_METHOD3(Write, void(RegData data, PrivLevel priv, ContextId ctxt));
  MOCK_METHOD1(WriteBody, void(RegData data));
  MOCK_CONST_METHOD2(Read, RegData(PrivLevel priv, ContextId ctxt));
  MOCK_CONST_METHOD0(ReadBody, RegData(void));
  MOCK_CONST_METHOD0(GetName, std::string(void));

  MOCK_CONST_METHOD0(GetCfSRegId, CfSReg(void));

  MockSRegRDBADAT1(ContextId ctid, ProcElementBase* pe) : SRegRDBADAT1(ctid, pe) {};
};
void SRegRDBADAT1::Write(RegData data, PrivLevel priv, ContextId ctxt) {};
RegData SRegRDBADAT1::Read(PrivLevel priv, ContextId ctxt) const { return static_cast<RegData>(0); };


/*!
  @brief RDBADAT2 register mock class
 */
class MockSRegRDBADAT2 : public SRegRDBADAT2 {

public:
  MOCK_METHOD1(SetDATA, void(RegData data));
  MOCK_CONST_METHOD0(GetDATA, RegData(void));

  MOCK_METHOD0(Reset, void(void));

  MOCK_METHOD3(Write, void(RegData data, PrivLevel priv, ContextId ctxt));
  MOCK_METHOD1(WriteBody, void(RegData data));
  MOCK_CONST_METHOD2(Read, RegData(PrivLevel priv, ContextId ctxt));
  MOCK_CONST_METHOD0(ReadBody, RegData(void));
  MOCK_CONST_METHOD0(GetName, std::string(void));

  MOCK_CONST_METHOD0(GetCfSRegId, CfSReg(void));

  MockSRegRDBADAT2(ContextId ctid, ProcElementBase* pe) : SRegRDBADAT2(ctid, pe) {};
};
void SRegRDBADAT2::Write(RegData data, PrivLevel priv, ContextId ctxt) {};
RegData SRegRDBADAT2::Read(PrivLevel priv, ContextId ctxt) const { return static_cast<RegData>(0); };


/*!
  @brief RDBADAT3 register mock class
 */
class MockSRegRDBADAT3 : public SRegRDBADAT3 {

public:
  MOCK_METHOD1(SetDATA, void(RegData data));
  MOCK_CONST_METHOD0(GetDATA, RegData(void));

  MOCK_METHOD0(Reset, void(void));

  MOCK_METHOD3(Write, void(RegData data, PrivLevel priv, ContextId ctxt));
  MOCK_METHOD1(WriteBody, void(RegData data));
  MOCK_CONST_METHOD2(Read, RegData(PrivLevel priv, ContextId ctxt));
  MOCK_CONST_METHOD0(ReadBody, RegData(void));
  MOCK_CONST_METHOD0(GetName, std::string(void));

  MOCK_CONST_METHOD0(GetCfSRegId, CfSReg(void));

  MockSRegRDBADAT3(ContextId ctid, ProcElementBase* pe) : SRegRDBADAT3(ctid, pe) {};
};
void SRegRDBADAT3::Write(RegData data, PrivLevel priv, ContextId ctxt) {};
RegData SRegRDBADAT3::Read(PrivLevel priv, ContextId ctxt) const { return static_cast<RegData>(0); };


/*!
  @brief RDBATAG register mock class
 */
class MockSRegRDBATAG : public SRegRDBATAG {

public:
  MOCK_METHOD1(SetPPN, void(RegData ppn));
  MOCK_METHOD1(SetV, void(bool v));
  MOCK_CONST_METHOD0(GetPPN, RegData(void));
  MOCK_CONST_METHOD0(GetV, bool(void));

  MOCK_METHOD0(Reset, void(void));

  MOCK_METHOD3(Write, void(RegData data, PrivLevel priv, ContextId ctxt));
  MOCK_METHOD1(WriteBody, void(RegData data));
  MOCK_CONST_METHOD2(Read, RegData(PrivLevel priv, ContextId ctxt));
  MOCK_CONST_METHOD0(ReadBody, RegData(void));
  MOCK_CONST_METHOD0(GetName, std::string(void));

  MOCK_CONST_METHOD0(GetCfSRegId, CfSReg(void));

  MockSRegRDBATAG(ContextId ctid, ProcElementBase* pe) : SRegRDBATAG(ctid, pe) {};
};
void SRegRDBATAG::Write(RegData data, PrivLevel priv, ContextId ctxt) {};
RegData SRegRDBATAG::Read(PrivLevel priv, ContextId ctxt) const { return static_cast<RegData>(0); };


/*!
  @brief RDBCR register mock class
 */
class MockSRegRDBCR : public SRegRDBCR {

public:
  MOCK_METHOD1(SetRDBDBE, void(bool rdbdbe));
  MOCK_METHOD1(SetTPEN, void(bool tpen));
  MOCK_METHOD1(SetAPEN, void(bool apen));
  MOCK_METHOD1(SetRDBCLR, void(bool rdbclr));
  MOCK_METHOD1(SetRDBEN, void(bool rdben));
  MOCK_CONST_METHOD0(GetRDBDBE, bool(void));
  MOCK_CONST_METHOD0(GetTPEN, bool(void));
  MOCK_CONST_METHOD0(GetAPEN, bool(void));
  MOCK_CONST_METHOD0(GetRDBCLR, bool(void));
  MOCK_CONST_METHOD0(GetRDBEN, bool(void));

  MOCK_METHOD0(Reset, void(void));

  MOCK_METHOD3(Write, void(RegData data, PrivLevel priv, ContextId ctxt));
  MOCK_METHOD1(WriteBody, void(RegData data));
  MOCK_CONST_METHOD2(Read, RegData(PrivLevel priv, ContextId ctxt));
  MOCK_CONST_METHOD0(ReadBody, RegData(void));
  MOCK_CONST_METHOD0(GetName, std::string(void));

  MOCK_CONST_METHOD0(GetCfSRegId, CfSReg(void));

  MockSRegRDBCR(ContextId ctid, ProcElementBase* pe) : SRegRDBCR(ctid, pe) {};
};
void SRegRDBCR::SetRDBCLR(bool rdbclr) {};
void SRegRDBCR::Write(RegData data, PrivLevel priv, ContextId ctxt) {};
RegData SRegRDBCR::Read(PrivLevel priv, ContextId ctxt) const { return static_cast<RegData>(0); };


/*!
  @brief RDBSTAT register mock class
 */
class MockSRegRDBSTAT : public SRegRDBSTAT {

public:
  MOCK_METHOD1(SetLRU, void(RegData lru));
  MOCK_CONST_METHOD0(GetLRU, RegData(void));

  MOCK_METHOD0(Reset, void(void));

  MOCK_METHOD3(Write, void(RegData data, PrivLevel priv, ContextId ctxt));
  MOCK_METHOD1(WriteBody, void(RegData data));
  MOCK_CONST_METHOD2(Read, RegData(PrivLevel priv, ContextId ctxt));
  MOCK_CONST_METHOD0(ReadBody, RegData(void));
  MOCK_CONST_METHOD0(GetName, std::string(void));

  MOCK_CONST_METHOD0(GetCfSRegId, CfSReg(void));

  MockSRegRDBSTAT(ContextId ctid, ProcElementBase* pe) : SRegRDBSTAT(ctid, pe) {};
};
void SRegRDBSTAT::Write(RegData data, PrivLevel priv, ContextId ctxt) {};
RegData SRegRDBSTAT::Read(PrivLevel priv, ContextId ctxt) const { return static_cast<RegData>(0); };


/*!
  @brief SCBP register mock class
 */
class MockSRegSCBP : public SRegSCBP {

public:
  MOCK_METHOD1(SetSCBP, void(RegData scbp));
  MOCK_CONST_METHOD0(GetSCBP, RegData(void));

  MOCK_METHOD0(Reset, void(void));

  MOCK_METHOD3(Write, void(RegData data, PrivLevel priv, ContextId ctxt));
  MOCK_METHOD1(WriteBody, void(RegData data));
  MOCK_CONST_METHOD2(Read, RegData(PrivLevel priv, ContextId ctxt));
  MOCK_CONST_METHOD0(ReadBody, RegData(void));
  MOCK_CONST_METHOD0(GetName, std::string(void));

  MOCK_CONST_METHOD0(GetCfSRegId, CfSReg(void));

  MockSRegSCBP(ContextId ctid, ProcElementBase* pe) : SRegSCBP(ctid, pe) {};
};


/*!
  @brief SCCFG register mock class
 */
class MockSRegSCCFG : public SRegSCCFG {

public:
  MOCK_METHOD1(SetSIZE, void(RegData size));
  MOCK_CONST_METHOD0(GetSIZE, RegData(void));

  MOCK_METHOD0(Reset, void(void));

  MOCK_METHOD3(Write, void(RegData data, PrivLevel priv, ContextId ctxt));
  MOCK_METHOD1(WriteBody, void(RegData data));
  MOCK_CONST_METHOD2(Read, RegData(PrivLevel priv, ContextId ctxt));
  MOCK_CONST_METHOD0(ReadBody, RegData(void));
  MOCK_CONST_METHOD0(GetName, std::string(void));

  MOCK_CONST_METHOD0(GetCfSRegId, CfSReg(void));

  MockSRegSCCFG(ContextId ctid, ProcElementBase* pe) : SRegSCCFG(ctid, pe) {};
};


/*!
  @brief SNZCFG register mock class
 */
class MockSRegSNZCFG : public SRegSNZCFG {

public:
  MOCK_METHOD1(SetSNZCNT, void(RegData snzcnt));
  MOCK_CONST_METHOD0(GetSNZCNT, RegData(void));

  MOCK_METHOD0(Reset, void(void));

  MOCK_METHOD3(Write, void(RegData data, PrivLevel priv, ContextId ctxt));
  MOCK_METHOD1(WriteBody, void(RegData data));
  MOCK_CONST_METHOD2(Read, RegData(PrivLevel priv, ContextId ctxt));
  MOCK_CONST_METHOD0(ReadBody, RegData(void));
  MOCK_CONST_METHOD0(GetName, std::string(void));

  MOCK_CONST_METHOD0(GetCfSRegId, CfSReg(void));

  MockSRegSNZCFG(ContextId ctid, ProcElementBase* pe) : SRegSNZCFG(ctid, pe) {};
};
void SRegSNZCFG::Reset(void) {};
void SRegSNZCFG::Write(RegData data, PrivLevel priv, ContextId ctxt) {};
RegData SRegSNZCFG::Read(PrivLevel priv, ContextId ctxt) const { return static_cast<RegData>(0); };


/*!
  @brief SPID register mock class
 */
class MockSRegHMSPID : public SRegHMSPID {

public:
  MOCK_METHOD1(SetSPID, void(RegData spid));
  MOCK_CONST_METHOD0(GetSPID, RegData(void));

  MOCK_METHOD0(Reset, void(void));

  MOCK_METHOD3(Write, void(RegData data, PrivLevel priv, ContextId ctxt));
  MOCK_METHOD1(WriteBody, void(RegData data));
  MOCK_CONST_METHOD2(Read, RegData(PrivLevel priv, ContextId ctxt));
  MOCK_CONST_METHOD0(ReadBody, RegData(void));
  MOCK_CONST_METHOD0(GetName, std::string(void));

  MOCK_CONST_METHOD0(GetCfSRegId, CfSReg(void));

  MockSRegHMSPID(ContextId ctid, ProcElementBase* pe) : SRegHMSPID(ctid, pe) {};
};
void SRegHMSPID::SetSPID(RegData spid) {};
void SRegHMSPID::Reset(void) {};
void SRegHMSPID::Write(RegData data, PrivLevel priv, ContextId ctxt) {};


/*!
  @brief SPIDLIST register mock class
 */
class MockSRegHMSPIDLIST : public SRegHMSPIDLIST {

public:
  MOCK_METHOD1(SetSPIDLIST, void(RegData spidlist));
  MOCK_CONST_METHOD0(GetSPIDLIST, RegData(void));

  MOCK_METHOD0(Reset, void(void));

  MOCK_METHOD3(Write, void(RegData data, PrivLevel priv, ContextId ctxt));
  MOCK_METHOD1(WriteBody, void(RegData data));
  MOCK_CONST_METHOD2(Read, RegData(PrivLevel priv, ContextId ctxt));
  MOCK_CONST_METHOD0(ReadBody, RegData(void));
  MOCK_CONST_METHOD0(GetName, std::string(void));

  MOCK_CONST_METHOD0(GetCfSRegId, CfSReg(void));

  MockSRegHMSPIDLIST(ContextId ctid, ProcElementBase* pe) : SRegHMSPIDLIST(ctid, pe) {};
};
void SRegHMSPIDLIST::Reset(void) {};


/*!
  @brief SVLOCK register mock class
 */
class MockSRegHMSVLOCK : public SRegHMSVLOCK {

public:
  MOCK_METHOD1(SetSVL, void(bool svl));
  MOCK_CONST_METHOD0(GetSVL, bool(void));

  MOCK_METHOD0(Reset, void(void));

  MOCK_METHOD3(Write, void(RegData data, PrivLevel priv, ContextId ctxt));
  MOCK_METHOD1(WriteBody, void(RegData data));
  MOCK_CONST_METHOD2(Read, RegData(PrivLevel priv, ContextId ctxt));
  MOCK_CONST_METHOD0(ReadBody, RegData(void));
  MOCK_CONST_METHOD0(GetName, std::string(void));

  MOCK_CONST_METHOD0(GetCfSRegId, CfSReg(void));

  MockSRegHMSVLOCK(ContextId ctid, ProcElementBase* pe) : SRegHMSVLOCK(ctid, pe) {};
};


/*!
  @brief TSCOUNTH register mock class
 */
class MockSRegTSCOUNTH : public SRegTSCOUNTH {

public:
  MOCK_METHOD1(SetTSCNTH, void(RegData tscnth));
  MOCK_CONST_METHOD0(GetTSCNTH, RegData(void));

  MOCK_METHOD0(Reset, void(void));

  MOCK_METHOD3(Write, void(RegData data, PrivLevel priv, ContextId ctxt));
  MOCK_METHOD1(WriteBody, void(RegData data));
  MOCK_CONST_METHOD2(Read, RegData(PrivLevel priv, ContextId ctxt));
  MOCK_CONST_METHOD0(ReadBody, RegData(void));
  MOCK_CONST_METHOD0(GetName, std::string(void));

  MOCK_CONST_METHOD0(GetCfSRegId, CfSReg(void));

  MockSRegTSCOUNTH(ContextId ctid, ProcElementBase* pe) : SRegTSCOUNTH(ctid, pe) {};
};
void SRegTSCOUNTH::Write(RegData data, PrivLevel priv, ContextId ctxt) {};
RegData SRegTSCOUNTH::Read(PrivLevel priv, ContextId ctxt) const { return static_cast<RegData>(0); };


/*!
  @brief TSCOUNTL register mock class
 */
class MockSRegTSCOUNTL : public SRegTSCOUNTL {

public:
  MOCK_METHOD1(SetTSCNTL, void(RegData tscntl));
  MOCK_CONST_METHOD0(GetTSCNTL, RegData(void));

  MOCK_METHOD0(Reset, void(void));

  MOCK_METHOD3(Write, void(RegData data, PrivLevel priv, ContextId ctxt));
  MOCK_METHOD1(WriteBody, void(RegData data));
  MOCK_CONST_METHOD2(Read, RegData(PrivLevel priv, ContextId ctxt));
  MOCK_CONST_METHOD0(ReadBody, RegData(void));
  MOCK_CONST_METHOD0(GetName, std::string(void));

  MOCK_CONST_METHOD0(GetCfSRegId, CfSReg(void));

  MockSRegTSCOUNTL(ContextId ctid, ProcElementBase* pe) : SRegTSCOUNTL(ctid, pe) {};
};
void SRegTSCOUNTL::Write(RegData data, PrivLevel priv, ContextId ctxt) {};
RegData SRegTSCOUNTL::Read(PrivLevel priv, ContextId ctxt) const { return static_cast<RegData>(0); };


/*!
  @brief TSCTRL register mock class
 */
class MockSRegTSCTRL : public SRegTSCTRL {

public:
  MOCK_METHOD1(SetOVF, void(bool ovf));
  MOCK_METHOD1(SetCEN, void(bool cen));
  MOCK_CONST_METHOD0(GetOVF, bool(void));
  MOCK_CONST_METHOD0(GetCEN, bool(void));

  MOCK_METHOD0(Reset, void(void));

  MOCK_METHOD3(Write, void(RegData data, PrivLevel priv, ContextId ctxt));
  MOCK_METHOD1(WriteBody, void(RegData data));
  MOCK_CONST_METHOD2(Read, RegData(PrivLevel priv, ContextId ctxt));
  MOCK_CONST_METHOD0(ReadBody, RegData(void));
  MOCK_CONST_METHOD0(GetName, std::string(void));

  MOCK_CONST_METHOD0(GetCfSRegId, CfSReg(void));

  MockSRegTSCTRL(ContextId ctid, ProcElementBase* pe) : SRegTSCTRL(ctid, pe) {};
};
void SRegTSCTRL::Write(RegData data, PrivLevel priv, ContextId ctxt) {};
RegData SRegTSCTRL::Read(PrivLevel priv, ContextId ctxt) const { return static_cast<RegData>(0); };

