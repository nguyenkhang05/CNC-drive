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
#include "forest_message.h"
#include "sreg_vm_base.h"
#include "proc_element_base.h"

/*!
  @brief BMID register interface class
 */
class SRegBMID_IF {
public:
  virtual void SetBMID(RegData bmid) = 0;
  virtual RegData GetBMID(void) const = 0;
  virtual void Reset(void) = 0;
  virtual void Write(RegData data, PrivLevel priv, ContextId ctxt) = 0;
  virtual RegData Read(PrivLevel priv, ContextId ctxt) const = 0;
  virtual std::string GetName(void) const = 0;
  virtual CfSReg GetCfSRegId(void) const = 0;
  virtual ~SRegBMID_IF() {}
};

/*!
  @brief BPAM register interface class
 */
class SRegBPAM_IF {
public:
  virtual void SetBPAM(RegData bpam) = 0;
  virtual RegData GetBPAM(void) const = 0;
  virtual void Reset(void) = 0;
  virtual void Write(RegData data, PrivLevel priv, ContextId ctxt) = 0;
  virtual RegData Read(PrivLevel priv, ContextId ctxt) const = 0;
  virtual std::string GetName(void) const = 0;
  virtual CfSReg GetCfSRegId(void) const = 0;
  virtual ~SRegBPAM_IF() {}
};

/*!
  @brief BPAM_N register interface class
 */
class SRegBPAM_N_IF {
public:
  virtual void SetBPAM(RegData bpam) = 0;
  virtual RegData GetBPAM(void) const = 0;
  virtual void Reset(void) = 0;
  virtual void Write(RegData data, PrivLevel priv, ContextId ctxt) = 0;
  virtual RegData Read(PrivLevel priv, ContextId ctxt) const = 0;
  virtual std::string GetName(void) const = 0;
  virtual CfSReg GetCfSRegId(void) const = 0;
  virtual ~SRegBPAM_N_IF() {}
};

/*!
  @brief BPAV register interface class
 */
class SRegBPAV_IF {
public:
  virtual void SetBPAV(RegData bpav) = 0;
  virtual RegData GetBPAV(void) const = 0;
  virtual void Reset(void) = 0;
  virtual void Write(RegData data, PrivLevel priv, ContextId ctxt) = 0;
  virtual RegData Read(PrivLevel priv, ContextId ctxt) const = 0;
  virtual std::string GetName(void) const = 0;
  virtual CfSReg GetCfSRegId(void) const = 0;
  virtual ~SRegBPAV_IF() {}
};

/*!
  @brief BPAV_N register interface class
 */
class SRegBPAV_N_IF {
public:
  virtual void SetBPAV(RegData bpav) = 0;
  virtual RegData GetBPAV(void) const = 0;
  virtual void Reset(void) = 0;
  virtual void Write(RegData data, PrivLevel priv, ContextId ctxt) = 0;
  virtual RegData Read(PrivLevel priv, ContextId ctxt) const = 0;
  virtual std::string GetName(void) const = 0;
  virtual CfSReg GetCfSRegId(void) const = 0;
  virtual ~SRegBPAV_N_IF() {}
};

/*!
  @brief BPC register interface class
 */
class SRegBPC_IF {
public:
  virtual void SetBPC(RegData bpc) = 0;
  virtual RegData GetBPC(void) const = 0;
  virtual void Reset(void) = 0;
  virtual void Write(RegData data, PrivLevel priv, ContextId ctxt) = 0;
  virtual RegData Read(PrivLevel priv, ContextId ctxt) const = 0;
  virtual std::string GetName(void) const = 0;
  virtual CfSReg GetCfSRegId(void) const = 0;
  virtual ~SRegBPC_IF() {}
};

/*!
  @brief BPC_N register interface class
 */
class SRegBPC_N_IF {
public:
  virtual void SetHE(bool he) = 0;
  virtual void SetGE(RegData ge) = 0;
  virtual void SetTY(RegData ty) = 0;
  virtual void SetVA(bool va) = 0;
  virtual void SetEO(bool eo) = 0;
  virtual void SetTE(bool te) = 0;
  virtual void SetBE(bool be) = 0;
  virtual void SetFE(bool fe) = 0;
  virtual void SetWE(bool we) = 0;
  virtual void SetRE(bool re) = 0;
  virtual bool GetHE(void) const = 0;
  virtual RegData GetGE(void) const = 0;
  virtual RegData GetTY(void) const = 0;
  virtual bool GetVA(void) const = 0;
  virtual bool GetEO(void) const = 0;
  virtual bool GetTE(void) const = 0;
  virtual bool GetBE(void) const = 0;
  virtual bool GetFE(void) const = 0;
  virtual bool GetWE(void) const = 0;
  virtual bool GetRE(void) const = 0;
  virtual void Reset(void) = 0;
  virtual void Write(RegData data, PrivLevel priv, ContextId ctxt) = 0;
  virtual RegData Read(PrivLevel priv, ContextId ctxt) const = 0;
  virtual std::string GetName(void) const = 0;
  virtual CfSReg GetCfSRegId(void) const = 0;
  virtual ~SRegBPC_N_IF() {}
};

/*!
  @brief BRPACTRL register interface class
 */
class SRegBRPACTRL_IF {
public:
  virtual void SetSR(bool sr) = 0;
  virtual void SetRW(bool rw) = 0;
  virtual void SetSEL(RegData sel) = 0;
  virtual void SetENTRY(RegData entry) = 0;
  virtual bool GetSR(void) const = 0;
  virtual bool GetRW(void) const = 0;
  virtual RegData GetSEL(void) const = 0;
  virtual RegData GetENTRY(void) const = 0;
  virtual void Reset(void) = 0;
  virtual void Write(RegData data, PrivLevel priv, ContextId ctxt) = 0;
  virtual RegData Read(PrivLevel priv, ContextId ctxt) const = 0;
  virtual std::string GetName(void) const = 0;
  virtual CfSReg GetCfSRegId(void) const = 0;
  virtual ~SRegBRPACTRL_IF() {}
};

/*!
  @brief BRPCFG register interface class
 */
class SRegBRPCFG_IF {
public:
  virtual void SetRASSIZE(RegData rassize) = 0;
  virtual void SetBTACSIZE(RegData btacsize) = 0;
  virtual void SetGBHTSIZE(RegData gbhtsize) = 0;
  virtual void SetBHTSIZE(RegData bhtsize) = 0;
  virtual RegData GetRASSIZE(void) const = 0;
  virtual RegData GetBTACSIZE(void) const = 0;
  virtual RegData GetGBHTSIZE(void) const = 0;
  virtual RegData GetBHTSIZE(void) const = 0;
  virtual void Reset(void) = 0;
  virtual void Write(RegData data, PrivLevel priv, ContextId ctxt) = 0;
  virtual RegData Read(PrivLevel priv, ContextId ctxt) const = 0;
  virtual std::string GetName(void) const = 0;
  virtual CfSReg GetCfSRegId(void) const = 0;
  virtual ~SRegBRPCFG_IF() {}
};

/*!
  @brief BRPCTRL0 register interface class
 */
class SRegBRPCTRL0_IF {
public:
  virtual void SetBRPCLR(bool brpclr) = 0;
  virtual void SetBRPEN(bool brpen) = 0;
  virtual bool GetBRPCLR(void) const = 0;
  virtual bool GetBRPEN(void) const = 0;
  virtual void Reset(void) = 0;
  virtual void Write(RegData data, PrivLevel priv, ContextId ctxt) = 0;
  virtual RegData Read(PrivLevel priv, ContextId ctxt) const = 0;
  virtual std::string GetName(void) const = 0;
  virtual CfSReg GetCfSRegId(void) const = 0;
  virtual ~SRegBRPCTRL0_IF() {}
};

/*!
  @brief BRPCTRL1 register interface class
 */
class SRegBRPCTRL1_IF {
public:
  virtual void SetBHTST(bool bhtst) = 0;
  virtual void SetRASD(bool rasd) = 0;
  virtual void SetBTACD(bool btacd) = 0;
  virtual void SetGBHTD(bool gbhtd) = 0;
  virtual void SetBHTD(bool bhtd) = 0;
  virtual bool GetBHTST(void) const = 0;
  virtual bool GetRASD(void) const = 0;
  virtual bool GetBTACD(void) const = 0;
  virtual bool GetGBHTD(void) const = 0;
  virtual bool GetBHTD(void) const = 0;
  virtual void Reset(void) = 0;
  virtual void Write(RegData data, PrivLevel priv, ContextId ctxt) = 0;
  virtual RegData Read(PrivLevel priv, ContextId ctxt) const = 0;
  virtual std::string GetName(void) const = 0;
  virtual CfSReg GetCfSRegId(void) const = 0;
  virtual ~SRegBRPCTRL1_IF() {}
};

/*!
  @brief BRPDATA register interface class
 */
class SRegBRPDATA_IF {
public:
  virtual void SetDATA(RegData data) = 0;
  virtual RegData GetDATA(void) const = 0;
  virtual void Reset(void) = 0;
  virtual void Write(RegData data, PrivLevel priv, ContextId ctxt) = 0;
  virtual RegData Read(PrivLevel priv, ContextId ctxt) const = 0;
  virtual std::string GetName(void) const = 0;
  virtual CfSReg GetCfSRegId(void) const = 0;
  virtual ~SRegBRPDATA_IF() {}
};

/*!
  @brief CTBP register interface class
 */
class SRegCTBP_IF {
public:
  virtual void SetCTBP(RegData ctbp) = 0;
  virtual RegData GetCTBP(void) const = 0;
  virtual void Reset(void) = 0;
  virtual void Write(RegData data, PrivLevel priv, ContextId ctxt) = 0;
  virtual RegData Read(PrivLevel priv, ContextId ctxt) const = 0;
  virtual std::string GetName(void) const = 0;
  virtual CfSReg GetCfSRegId(void) const = 0;
  virtual ~SRegCTBP_IF() {}
};

/*!
  @brief CTPC register interface class
 */
class SRegCTPC_IF {
public:
  virtual void SetCTPC(RegData ctpc) = 0;
  virtual RegData GetCTPC(void) const = 0;
  virtual void Reset(void) = 0;
  virtual void Write(RegData data, PrivLevel priv, ContextId ctxt) = 0;
  virtual RegData Read(PrivLevel priv, ContextId ctxt) const = 0;
  virtual std::string GetName(void) const = 0;
  virtual CfSReg GetCfSRegId(void) const = 0;
  virtual ~SRegCTPC_IF() {}
};

/*!
  @brief CTPSW register interface class
 */
class SRegCTPSW_IF {
public:
  virtual void SetCTPSW(RegData ctpsw) = 0;
  virtual RegData GetCTPSW(void) const = 0;
  virtual void Reset(void) = 0;
  virtual void Write(RegData data, PrivLevel priv, ContextId ctxt) = 0;
  virtual RegData Read(PrivLevel priv, ContextId ctxt) const = 0;
  virtual std::string GetName(void) const = 0;
  virtual CfSReg GetCfSRegId(void) const = 0;
  virtual ~SRegCTPSW_IF() {}
};

/*!
  @brief DBCFG register interface class
 */
class SRegDBCFG_IF {
public:
  virtual void SetNBP(RegData nbp) = 0;
  virtual RegData GetNBP(void) const = 0;
  virtual void Reset(void) = 0;
  virtual void Write(RegData data, PrivLevel priv, ContextId ctxt) = 0;
  virtual RegData Read(PrivLevel priv, ContextId ctxt) const = 0;
  virtual std::string GetName(void) const = 0;
  virtual CfSReg GetCfSRegId(void) const = 0;
  virtual ~SRegDBCFG_IF() {}
};

/*!
  @brief DBGEN register interface class
 */
class SRegDBGEN_IF {
public:
  virtual void SetHE(bool he) = 0;
  virtual void SetGE(RegData ge) = 0;
  virtual bool GetHE(void) const = 0;
  virtual RegData GetGE(void) const = 0;
  virtual void Reset(void) = 0;
  virtual void Write(RegData data, PrivLevel priv, ContextId ctxt) = 0;
  virtual RegData Read(PrivLevel priv, ContextId ctxt) const = 0;
  virtual std::string GetName(void) const = 0;
  virtual CfSReg GetCfSRegId(void) const = 0;
  virtual ~SRegDBGEN_IF() {}
};

/*!
  @brief DBIC register interface class
 */
class SRegDBIC_IF {
public:
  virtual void SetDBIC(RegData dbic) = 0;
  virtual RegData GetDBIC(void) const = 0;
  virtual void Reset(void) = 0;
  virtual void Write(RegData data, PrivLevel priv, ContextId ctxt) = 0;
  virtual RegData Read(PrivLevel priv, ContextId ctxt) const = 0;
  virtual std::string GetName(void) const = 0;
  virtual CfSReg GetCfSRegId(void) const = 0;
  virtual ~SRegDBIC_IF() {}
};

/*!
  @brief DBPC register interface class
 */
class SRegDBPC_IF {
public:
  virtual void SetDBPC(RegData dbpc) = 0;
  virtual RegData GetDBPC(void) const = 0;
  virtual void Reset(void) = 0;
  virtual void Write(RegData data, PrivLevel priv, ContextId ctxt) = 0;
  virtual RegData Read(PrivLevel priv, ContextId ctxt) const = 0;
  virtual std::string GetName(void) const = 0;
  virtual CfSReg GetCfSRegId(void) const = 0;
  virtual ~SRegDBPC_IF() {}
};

/*!
  @brief DBPSW register interface class
 */
class SRegDBPSW_IF {
public:
  virtual void SetUM(bool um) = 0;
  virtual void SetEIMASK(RegData eimask) = 0;
  virtual void SetCU(RegData cu) = 0;
  virtual void SetEBV(bool ebv) = 0;
  virtual void SetNP(bool np) = 0;
  virtual void SetEP(bool ep) = 0;
  virtual void SetID(bool id) = 0;
  virtual void SetSAT(bool sat) = 0;
  virtual void SetCY(bool cy) = 0;
  virtual void SetOV(bool ov) = 0;
  virtual void SetS(bool s) = 0;
  virtual void SetZ(bool z) = 0;
  virtual bool GetUM(void) const = 0;
  virtual RegData GetEIMASK(void) const = 0;
  virtual RegData GetCU(void) const = 0;
  virtual bool GetEBV(void) const = 0;
  virtual bool GetNP(void) const = 0;
  virtual bool GetEP(void) const = 0;
  virtual bool GetID(void) const = 0;
  virtual bool GetSAT(void) const = 0;
  virtual bool GetCY(void) const = 0;
  virtual bool GetOV(void) const = 0;
  virtual bool GetS(void) const = 0;
  virtual bool GetZ(void) const = 0;
  virtual void Reset(void) = 0;
  virtual void Write(RegData data, PrivLevel priv, ContextId ctxt) = 0;
  virtual RegData Read(PrivLevel priv, ContextId ctxt) const = 0;
  virtual std::string GetName(void) const = 0;
  virtual CfSReg GetCfSRegId(void) const = 0;
  virtual ~SRegDBPSW_IF() {}
};

/*!
  @brief DBPSWH register interface class
 */
class SRegDBPSWH_IF {
public:
  virtual void SetGM(bool gm) = 0;
  virtual void SetGPID(RegData gpid) = 0;
  virtual bool GetGM(void) const = 0;
  virtual RegData GetGPID(void) const = 0;
  virtual void Reset(void) = 0;
  virtual void Write(RegData data, PrivLevel priv, ContextId ctxt) = 0;
  virtual RegData Read(PrivLevel priv, ContextId ctxt) const = 0;
  virtual std::string GetName(void) const = 0;
  virtual CfSReg GetCfSRegId(void) const = 0;
  virtual ~SRegDBPSWH_IF() {}
};

/*!
  @brief DBWR register interface class
 */
class SRegDBWR_IF {
public:
  virtual void SetDBWR(RegData dbwr) = 0;
  virtual RegData GetDBWR(void) const = 0;
  virtual void Reset(void) = 0;
  virtual void Write(RegData data, PrivLevel priv, ContextId ctxt) = 0;
  virtual RegData Read(PrivLevel priv, ContextId ctxt) const = 0;
  virtual std::string GetName(void) const = 0;
  virtual CfSReg GetCfSRegId(void) const = 0;
  virtual ~SRegDBWR_IF() {}
};

/*!
  @brief DCBKEY register interface class
 */
class SRegDCBKEY_IF {
public:
  virtual void SetCBAKEY(RegData cbakey) = 0;
  virtual void SetKEYCODE(RegData keycode) = 0;
  virtual RegData GetCBAKEY(void) const = 0;
  virtual RegData GetKEYCODE(void) const = 0;
  virtual void Reset(void) = 0;
  virtual void Write(RegData data, PrivLevel priv, ContextId ctxt) = 0;
  virtual RegData Read(PrivLevel priv, ContextId ctxt) const = 0;
  virtual std::string GetName(void) const = 0;
  virtual CfSReg GetCfSRegId(void) const = 0;
  virtual ~SRegDCBKEY_IF() {}
};

/*!
  @brief DECFG register interface class
 */
class SRegDECFG_IF {
public:
  virtual void SetEHE(bool ehe) = 0;
  virtual void SetEGE(bool ege) = 0;
  virtual void SetESE(bool ese) = 0;
  virtual bool GetEHE(void) const = 0;
  virtual bool GetEGE(void) const = 0;
  virtual bool GetESE(void) const = 0;
  virtual void Reset(void) = 0;
  virtual void Write(RegData data, PrivLevel priv, ContextId ctxt) = 0;
  virtual RegData Read(PrivLevel priv, ContextId ctxt) const = 0;
  virtual std::string GetName(void) const = 0;
  virtual CfSReg GetCfSRegId(void) const = 0;
  virtual ~SRegDECFG_IF() {}
};

/*!
  @brief DECTRL register interface class
 */
class SRegDECTRL_IF {
public:
  virtual void SetBEVD(bool bevd) = 0;
  virtual bool GetBEVD(void) const = 0;
  virtual void Reset(void) = 0;
  virtual void Write(RegData data, PrivLevel priv, ContextId ctxt) = 0;
  virtual RegData Read(PrivLevel priv, ContextId ctxt) const = 0;
  virtual std::string GetName(void) const = 0;
  virtual CfSReg GetCfSRegId(void) const = 0;
  virtual ~SRegDECTRL_IF() {}
};

/*!
  @brief DEVDS register interface class
 */
class SRegDEVDS_IF {
public:
  virtual void SetBEVDHM(bool bevdhm) = 0;
  virtual void SetBEVDGM7(bool bevdgm7) = 0;
  virtual void SetBEVDGM6(bool bevdgm6) = 0;
  virtual void SetBEVDGM5(bool bevdgm5) = 0;
  virtual void SetBEVDGM4(bool bevdgm4) = 0;
  virtual void SetBEVDGM3(bool bevdgm3) = 0;
  virtual void SetBEVDGM2(bool bevdgm2) = 0;
  virtual void SetBEVDGM1(bool bevdgm1) = 0;
  virtual void SetBEVDGM0(bool bevdgm0) = 0;
  virtual bool GetBEVDHM(void) const = 0;
  virtual bool GetBEVDGM7(void) const = 0;
  virtual bool GetBEVDGM6(void) const = 0;
  virtual bool GetBEVDGM5(void) const = 0;
  virtual bool GetBEVDGM4(void) const = 0;
  virtual bool GetBEVDGM3(void) const = 0;
  virtual bool GetBEVDGM2(void) const = 0;
  virtual bool GetBEVDGM1(void) const = 0;
  virtual bool GetBEVDGM0(void) const = 0;
  virtual void Reset(void) = 0;
  virtual void Write(RegData data, PrivLevel priv, ContextId ctxt) = 0;
  virtual RegData Read(PrivLevel priv, ContextId ctxt) const = 0;
  virtual std::string GetName(void) const = 0;
  virtual CfSReg GetCfSRegId(void) const = 0;
  virtual ~SRegDEVDS_IF() {}
};

/*!
  @brief DIR0 register interface class
 */
class SRegDIR0_IF {
public:
  virtual void SetAEE(bool aee) = 0;
  virtual void SetAT(bool at) = 0;
  virtual void SetDN(bool dn) = 0;
  virtual void SetDM(bool dm) = 0;
  virtual bool GetAEE(void) const = 0;
  virtual bool GetAT(void) const = 0;
  virtual bool GetDN(void) const = 0;
  virtual bool GetDM(void) const = 0;
  virtual void Reset(void) = 0;
  virtual void Write(RegData data, PrivLevel priv, ContextId ctxt) = 0;
  virtual RegData Read(PrivLevel priv, ContextId ctxt) const = 0;
  virtual std::string GetName(void) const = 0;
  virtual CfSReg GetCfSRegId(void) const = 0;
  virtual ~SRegDIR0_IF() {}
};

/*!
  @brief DIR1 register interface class
 */
class SRegDIR1_IF {
public:
  virtual void SetBT(RegData bt) = 0;
  virtual void SetCSL(RegData csl) = 0;
  virtual void SetSQ1(bool sq1) = 0;
  virtual void SetSQ0(bool sq0) = 0;
  virtual void SetBEN(bool ben) = 0;
  virtual RegData GetBT(void) const = 0;
  virtual RegData GetCSL(void) const = 0;
  virtual bool GetSQ1(void) const = 0;
  virtual bool GetSQ0(void) const = 0;
  virtual bool GetBEN(void) const = 0;
  virtual void Reset(void) = 0;
  virtual void Write(RegData data, PrivLevel priv, ContextId ctxt) = 0;
  virtual RegData Read(PrivLevel priv, ContextId ctxt) const = 0;
  virtual std::string GetName(void) const = 0;
  virtual CfSReg GetCfSRegId(void) const = 0;
  virtual ~SRegDIR1_IF() {}
};

/*!
  @brief EBASE register interface class
 */
class SRegHMEBASE_IF {
public:
  virtual void SetEBASE(RegData ebase) = 0;
  virtual void SetDV(bool dv) = 0;
  virtual void SetRINT(bool rint) = 0;
  virtual RegData GetEBASE(void) const = 0;
  virtual bool GetDV(void) const = 0;
  virtual bool GetRINT(void) const = 0;
  virtual void Reset(void) = 0;
  virtual void Write(RegData data, PrivLevel priv, ContextId ctxt) = 0;
  virtual RegData Read(PrivLevel priv, ContextId ctxt) const = 0;
  virtual std::string GetName(void) const = 0;
  virtual CfSReg GetCfSRegId(void) const = 0;
  virtual ~SRegHMEBASE_IF() {}
};

/*!
  @brief EIIC register interface class
 */
class SRegHMEIIC_IF {
public:
  virtual void SetEIIC(RegData eiic) = 0;
  virtual RegData GetEIIC(void) const = 0;
  virtual void Reset(void) = 0;
  virtual void Write(RegData data, PrivLevel priv, ContextId ctxt) = 0;
  virtual RegData Read(PrivLevel priv, ContextId ctxt) const = 0;
  virtual std::string GetName(void) const = 0;
  virtual CfSReg GetCfSRegId(void) const = 0;
  virtual ~SRegHMEIIC_IF() {}
};

/*!
  @brief EIPC register interface class
 */
class SRegHMEIPC_IF {
public:
  virtual void SetEIPC(RegData eipc) = 0;
  virtual RegData GetEIPC(void) const = 0;
  virtual void Reset(void) = 0;
  virtual void Write(RegData data, PrivLevel priv, ContextId ctxt) = 0;
  virtual RegData Read(PrivLevel priv, ContextId ctxt) const = 0;
  virtual std::string GetName(void) const = 0;
  virtual CfSReg GetCfSRegId(void) const = 0;
  virtual ~SRegHMEIPC_IF() {}
};

/*!
  @brief EIPSW register interface class
 */
class SRegHMEIPSW_IF {
public:
  virtual void SetUM(bool um) = 0;
  virtual void SetEIMASK(RegData eimask) = 0;
  virtual void SetCU(RegData cu) = 0;
  virtual void SetEBV(bool ebv) = 0;
  virtual void SetNP(bool np) = 0;
  virtual void SetEP(bool ep) = 0;
  virtual void SetID(bool id) = 0;
  virtual void SetSAT(bool sat) = 0;
  virtual void SetCY(bool cy) = 0;
  virtual void SetOV(bool ov) = 0;
  virtual void SetS(bool s) = 0;
  virtual void SetZ(bool z) = 0;
  virtual bool GetUM(void) const = 0;
  virtual RegData GetEIMASK(void) const = 0;
  virtual RegData GetCU(void) const = 0;
  virtual bool GetEBV(void) const = 0;
  virtual bool GetNP(void) const = 0;
  virtual bool GetEP(void) const = 0;
  virtual bool GetID(void) const = 0;
  virtual bool GetSAT(void) const = 0;
  virtual bool GetCY(void) const = 0;
  virtual bool GetOV(void) const = 0;
  virtual bool GetS(void) const = 0;
  virtual bool GetZ(void) const = 0;
  virtual void Reset(void) = 0;
  virtual void Write(RegData data, PrivLevel priv, ContextId ctxt) = 0;
  virtual RegData Read(PrivLevel priv, ContextId ctxt) const = 0;
  virtual std::string GetName(void) const = 0;
  virtual CfSReg GetCfSRegId(void) const = 0;
  virtual ~SRegHMEIPSW_IF() {}
};

/*!
  @brief EIPSWH register interface class
 */
class SRegEIPSWH_IF {
public:
  virtual void SetGM(bool gm) = 0;
  virtual void SetGPID(RegData gpid) = 0;
  virtual bool GetGM(void) const = 0;
  virtual RegData GetGPID(void) const = 0;
  virtual void Reset(void) = 0;
  virtual void Write(RegData data, PrivLevel priv, ContextId ctxt) = 0;
  virtual RegData Read(PrivLevel priv, ContextId ctxt) const = 0;
  virtual std::string GetName(void) const = 0;
  virtual CfSReg GetCfSRegId(void) const = 0;
  virtual ~SRegEIPSWH_IF() {}
};

/*!
  @brief EIWR register interface class
 */
class SRegHMEIWR_IF {
public:
  virtual void SetEIWR(RegData eiwr) = 0;
  virtual RegData GetEIWR(void) const = 0;
  virtual void Reset(void) = 0;
  virtual void Write(RegData data, PrivLevel priv, ContextId ctxt) = 0;
  virtual RegData Read(PrivLevel priv, ContextId ctxt) const = 0;
  virtual std::string GetName(void) const = 0;
  virtual CfSReg GetCfSRegId(void) const = 0;
  virtual ~SRegHMEIWR_IF() {}
};

/*!
  @brief FEIC register interface class
 */
class SRegHMFEIC_IF {
public:
  virtual void SetFEIC(RegData feic) = 0;
  virtual RegData GetFEIC(void) const = 0;
  virtual void Reset(void) = 0;
  virtual void Write(RegData data, PrivLevel priv, ContextId ctxt) = 0;
  virtual RegData Read(PrivLevel priv, ContextId ctxt) const = 0;
  virtual std::string GetName(void) const = 0;
  virtual CfSReg GetCfSRegId(void) const = 0;
  virtual ~SRegHMFEIC_IF() {}
};

/*!
  @brief FEPC register interface class
 */
class SRegHMFEPC_IF {
public:
  virtual void SetFEPC(RegData fepc) = 0;
  virtual RegData GetFEPC(void) const = 0;
  virtual void Reset(void) = 0;
  virtual void Write(RegData data, PrivLevel priv, ContextId ctxt) = 0;
  virtual RegData Read(PrivLevel priv, ContextId ctxt) const = 0;
  virtual std::string GetName(void) const = 0;
  virtual CfSReg GetCfSRegId(void) const = 0;
  virtual ~SRegHMFEPC_IF() {}
};

/*!
  @brief FEPSW register interface class
 */
class SRegHMFEPSW_IF {
public:
  virtual void SetUM(bool um) = 0;
  virtual void SetEIMASK(RegData eimask) = 0;
  virtual void SetCU(RegData cu) = 0;
  virtual void SetEBV(bool ebv) = 0;
  virtual void SetNP(bool np) = 0;
  virtual void SetEP(bool ep) = 0;
  virtual void SetID(bool id) = 0;
  virtual void SetSAT(bool sat) = 0;
  virtual void SetCY(bool cy) = 0;
  virtual void SetOV(bool ov) = 0;
  virtual void SetS(bool s) = 0;
  virtual void SetZ(bool z) = 0;
  virtual bool GetUM(void) const = 0;
  virtual RegData GetEIMASK(void) const = 0;
  virtual RegData GetCU(void) const = 0;
  virtual bool GetEBV(void) const = 0;
  virtual bool GetNP(void) const = 0;
  virtual bool GetEP(void) const = 0;
  virtual bool GetID(void) const = 0;
  virtual bool GetSAT(void) const = 0;
  virtual bool GetCY(void) const = 0;
  virtual bool GetOV(void) const = 0;
  virtual bool GetS(void) const = 0;
  virtual bool GetZ(void) const = 0;
  virtual void Reset(void) = 0;
  virtual void Write(RegData data, PrivLevel priv, ContextId ctxt) = 0;
  virtual RegData Read(PrivLevel priv, ContextId ctxt) const = 0;
  virtual std::string GetName(void) const = 0;
  virtual CfSReg GetCfSRegId(void) const = 0;
  virtual ~SRegHMFEPSW_IF() {}
};

/*!
  @brief FEPSWH register interface class
 */
class SRegFEPSWH_IF {
public:
  virtual void SetGM(bool gm) = 0;
  virtual void SetGPID(RegData gpid) = 0;
  virtual bool GetGM(void) const = 0;
  virtual RegData GetGPID(void) const = 0;
  virtual void Reset(void) = 0;
  virtual void Write(RegData data, PrivLevel priv, ContextId ctxt) = 0;
  virtual RegData Read(PrivLevel priv, ContextId ctxt) const = 0;
  virtual std::string GetName(void) const = 0;
  virtual CfSReg GetCfSRegId(void) const = 0;
  virtual ~SRegFEPSWH_IF() {}
};

/*!
  @brief FEWR register interface class
 */
class SRegHMFEWR_IF {
public:
  virtual void SetFEWR(RegData fewr) = 0;
  virtual RegData GetFEWR(void) const = 0;
  virtual void Reset(void) = 0;
  virtual void Write(RegData data, PrivLevel priv, ContextId ctxt) = 0;
  virtual RegData Read(PrivLevel priv, ContextId ctxt) const = 0;
  virtual std::string GetName(void) const = 0;
  virtual CfSReg GetCfSRegId(void) const = 0;
  virtual ~SRegHMFEWR_IF() {}
};

/*!
  @brief FPCC register interface class
 */
class SRegFPCC_IF {
public:
  virtual void SetCC(RegData cc) = 0;
  virtual RegData GetCC(void) const = 0;
  virtual void Reset(void) = 0;
  virtual void Write(RegData data, PrivLevel priv, ContextId ctxt) = 0;
  virtual RegData Read(PrivLevel priv, ContextId ctxt) const = 0;
  virtual CoproType GetCoproType(void) const = 0;
  virtual std::string GetName(void) const = 0;
  virtual CfSReg GetCfSRegId(void) const = 0;
  virtual ~SRegFPCC_IF() {}
};

/*!
  @brief FPCFG register interface class
 */
class SRegFPCFG_IF {
public:
  virtual void SetRM(RegData rm) = 0;
  virtual void SetXE(RegData xe) = 0;
  virtual RegData GetRM(void) const = 0;
  virtual RegData GetXE(void) const = 0;
  virtual void Reset(void) = 0;
  virtual void Write(RegData data, PrivLevel priv, ContextId ctxt) = 0;
  virtual RegData Read(PrivLevel priv, ContextId ctxt) const = 0;
  virtual CoproType GetCoproType(void) const = 0;
  virtual std::string GetName(void) const = 0;
  virtual CfSReg GetCfSRegId(void) const = 0;
  virtual ~SRegFPCFG_IF() {}
};

/*!
  @brief FPEC register interface class
 */
class SRegFPEC_IF {
public:
  virtual void SetFPIVD(bool fpivd) = 0;
  virtual bool GetFPIVD(void) const = 0;
  virtual void Reset(void) = 0;
  virtual void Write(RegData data, PrivLevel priv, ContextId ctxt) = 0;
  virtual RegData Read(PrivLevel priv, ContextId ctxt) const = 0;
  virtual CoproType GetCoproType(void) const = 0;
  virtual std::string GetName(void) const = 0;
  virtual CfSReg GetCfSRegId(void) const = 0;
  virtual ~SRegFPEC_IF() {}
};

/*!
  @brief FPEPC register interface class
 */
class SRegFPEPC_IF {
public:
  virtual void SetFPEPC(RegData fpepc) = 0;
  virtual RegData GetFPEPC(void) const = 0;
  virtual void Reset(void) = 0;
  virtual void Write(RegData data, PrivLevel priv, ContextId ctxt) = 0;
  virtual RegData Read(PrivLevel priv, ContextId ctxt) const = 0;
  virtual CoproType GetCoproType(void) const = 0;
  virtual std::string GetName(void) const = 0;
  virtual CfSReg GetCfSRegId(void) const = 0;
  virtual ~SRegFPEPC_IF() {}
};

/*!
  @brief FPIPR register interface class
 */
class SRegFPIPR_IF {
public:
  virtual void SetFPIPR(RegData fpipr) = 0;
  virtual RegData GetFPIPR(void) const = 0;
  virtual void Reset(void) = 0;
  virtual void Write(RegData data, PrivLevel priv, ContextId ctxt) = 0;
  virtual RegData Read(PrivLevel priv, ContextId ctxt) const = 0;
  virtual std::string GetName(void) const = 0;
  virtual CfSReg GetCfSRegId(void) const = 0;
  virtual ~SRegFPIPR_IF() {}
};

/*!
  @brief FPSR register interface class
 */
class SRegFPSR_IF {
public:
  virtual void SetCC(RegData cc) = 0;
  virtual void SetFN(bool fn) = 0;
  virtual void SetIF_(bool if_) = 0;
  virtual void SetPEM(bool pem) = 0;
  virtual void SetRM(RegData rm) = 0;
  virtual void SetFS(bool fs) = 0;
  virtual void SetXC(RegData xc) = 0;
  virtual void SetXE(RegData xe) = 0;
  virtual void SetXP(RegData xp) = 0;
  virtual RegData GetCC(void) const = 0;
  virtual bool GetFN(void) const = 0;
  virtual bool GetIF_(void) const = 0;
  virtual bool GetPEM(void) const = 0;
  virtual RegData GetRM(void) const = 0;
  virtual bool GetFS(void) const = 0;
  virtual RegData GetXC(void) const = 0;
  virtual RegData GetXE(void) const = 0;
  virtual RegData GetXP(void) const = 0;
  virtual void Reset(void) = 0;
  virtual void Write(RegData data, PrivLevel priv, ContextId ctxt) = 0;
  virtual RegData Read(PrivLevel priv, ContextId ctxt) const = 0;
  virtual CoproType GetCoproType(void) const = 0;
  virtual std::string GetName(void) const = 0;
  virtual CfSReg GetCfSRegId(void) const = 0;
  virtual ~SRegFPSR_IF() {}
};

/*!
  @brief FPST register interface class
 */
class SRegFPST_IF {
public:
  virtual void SetXC(RegData xc) = 0;
  virtual void SetIF_(bool if_) = 0;
  virtual void SetXP(RegData xp) = 0;
  virtual RegData GetXC(void) const = 0;
  virtual bool GetIF_(void) const = 0;
  virtual RegData GetXP(void) const = 0;
  virtual void Reset(void) = 0;
  virtual void Write(RegData data, PrivLevel priv, ContextId ctxt) = 0;
  virtual RegData Read(PrivLevel priv, ContextId ctxt) const = 0;
  virtual CoproType GetCoproType(void) const = 0;
  virtual std::string GetName(void) const = 0;
  virtual CfSReg GetCfSRegId(void) const = 0;
  virtual ~SRegFPST_IF() {}
};

/*!
  @brief FXCFG register interface class
 */
class SRegFXCFG_IF {
public:
  virtual void SetRM(RegData rm) = 0;
  virtual void SetXE(RegData xe) = 0;
  virtual RegData GetRM(void) const = 0;
  virtual RegData GetXE(void) const = 0;
  virtual void Reset(void) = 0;
  virtual void Write(RegData data, PrivLevel priv, ContextId ctxt) = 0;
  virtual RegData Read(PrivLevel priv, ContextId ctxt) const = 0;
  virtual CoproType GetCoproType(void) const = 0;
  virtual std::string GetName(void) const = 0;
  virtual CfSReg GetCfSRegId(void) const = 0;
  virtual ~SRegFXCFG_IF() {}
};

/*!
  @brief FXINFO register interface class
 */
class SRegFXINFO_IF {
public:
  virtual void SetNFPU(bool nfpu) = 0;
  virtual void SetRSIZE(bool rsize) = 0;
  virtual bool GetNFPU(void) const = 0;
  virtual bool GetRSIZE(void) const = 0;
  virtual void Reset(void) = 0;
  virtual void Write(RegData data, PrivLevel priv, ContextId ctxt) = 0;
  virtual RegData Read(PrivLevel priv, ContextId ctxt) const = 0;
  virtual CoproType GetCoproType(void) const = 0;
  virtual std::string GetName(void) const = 0;
  virtual CfSReg GetCfSRegId(void) const = 0;
  virtual ~SRegFXINFO_IF() {}
};

/*!
  @brief FXSR register interface class
 */
class SRegFXSR_IF {
public:
  virtual void SetFN(bool fn) = 0;
  virtual void SetIF_(bool if_) = 0;
  virtual void SetPEM(bool pem) = 0;
  virtual void SetRM(RegData rm) = 0;
  virtual void SetFS(bool fs) = 0;
  virtual void SetXC(RegData xc) = 0;
  virtual void SetXE(RegData xe) = 0;
  virtual void SetXP(RegData xp) = 0;
  virtual bool GetFN(void) const = 0;
  virtual bool GetIF_(void) const = 0;
  virtual bool GetPEM(void) const = 0;
  virtual RegData GetRM(void) const = 0;
  virtual bool GetFS(void) const = 0;
  virtual RegData GetXC(void) const = 0;
  virtual RegData GetXE(void) const = 0;
  virtual RegData GetXP(void) const = 0;
  virtual void Reset(void) = 0;
  virtual void Write(RegData data, PrivLevel priv, ContextId ctxt) = 0;
  virtual RegData Read(PrivLevel priv, ContextId ctxt) const = 0;
  virtual CoproType GetCoproType(void) const = 0;
  virtual std::string GetName(void) const = 0;
  virtual CfSReg GetCfSRegId(void) const = 0;
  virtual ~SRegFXSR_IF() {}
};

/*!
  @brief FXST register interface class
 */
class SRegFXST_IF {
public:
  virtual void SetXC(RegData xc) = 0;
  virtual void SetIF_(bool if_) = 0;
  virtual void SetXP(RegData xp) = 0;
  virtual RegData GetXC(void) const = 0;
  virtual bool GetIF_(void) const = 0;
  virtual RegData GetXP(void) const = 0;
  virtual void Reset(void) = 0;
  virtual void Write(RegData data, PrivLevel priv, ContextId ctxt) = 0;
  virtual RegData Read(PrivLevel priv, ContextId ctxt) const = 0;
  virtual CoproType GetCoproType(void) const = 0;
  virtual std::string GetName(void) const = 0;
  virtual CfSReg GetCfSRegId(void) const = 0;
  virtual ~SRegFXST_IF() {}
};

/*!
  @brief FXXC register interface class
 */
class SRegFXXC_IF {
public:
  virtual void SetXC3(RegData xc3) = 0;
  virtual void SetXC2(RegData xc2) = 0;
  virtual void SetXC1(RegData xc1) = 0;
  virtual void SetXC0(RegData xc0) = 0;
  virtual RegData GetXC3(void) const = 0;
  virtual RegData GetXC2(void) const = 0;
  virtual RegData GetXC1(void) const = 0;
  virtual RegData GetXC0(void) const = 0;
  virtual void Reset(void) = 0;
  virtual void Write(RegData data, PrivLevel priv, ContextId ctxt) = 0;
  virtual RegData Read(PrivLevel priv, ContextId ctxt) const = 0;
  virtual CoproType GetCoproType(void) const = 0;
  virtual std::string GetName(void) const = 0;
  virtual CfSReg GetCfSRegId(void) const = 0;
  virtual ~SRegFXXC_IF() {}
};

/*!
  @brief FXXP register interface class
 */
class SRegFXXP_IF {
public:
  virtual void SetXP3(RegData xp3) = 0;
  virtual void SetXP2(RegData xp2) = 0;
  virtual void SetXP1(RegData xp1) = 0;
  virtual void SetXP0(RegData xp0) = 0;
  virtual RegData GetXP3(void) const = 0;
  virtual RegData GetXP2(void) const = 0;
  virtual RegData GetXP1(void) const = 0;
  virtual RegData GetXP0(void) const = 0;
  virtual void Reset(void) = 0;
  virtual void Write(RegData data, PrivLevel priv, ContextId ctxt) = 0;
  virtual RegData Read(PrivLevel priv, ContextId ctxt) const = 0;
  virtual CoproType GetCoproType(void) const = 0;
  virtual std::string GetName(void) const = 0;
  virtual CfSReg GetCfSRegId(void) const = 0;
  virtual ~SRegFXXP_IF() {}
};

/*!
  @brief GMCFG register interface class
 */
class SRegGMCFG_IF {
public:
  virtual void SetGCU2(bool gcu2) = 0;
  virtual void SetGCU1(bool gcu1) = 0;
  virtual void SetGCU0(bool gcu0) = 0;
  virtual void SetGSYSE(bool gsyse) = 0;
  virtual void SetHMP(bool hmp) = 0;
  virtual void SetGMP(bool gmp) = 0;
  virtual bool GetGCU2(void) const = 0;
  virtual bool GetGCU1(void) const = 0;
  virtual bool GetGCU0(void) const = 0;
  virtual bool GetGSYSE(void) const = 0;
  virtual bool GetHMP(void) const = 0;
  virtual bool GetGMP(void) const = 0;
  virtual void Reset(void) = 0;
  virtual void Write(RegData data, PrivLevel priv, ContextId ctxt) = 0;
  virtual RegData Read(PrivLevel priv, ContextId ctxt) const = 0;
  virtual std::string GetName(void) const = 0;
  virtual CfSReg GetCfSRegId(void) const = 0;
  virtual ~SRegGMCFG_IF() {}
};

/*!
  @brief GMEBASE register interface class
 */
class SRegGMEBASE_IF {
public:
  virtual void SetEBASE(RegData ebase) = 0;
  virtual void SetDV(bool dv) = 0;
  virtual void SetRINT(bool rint) = 0;
  virtual RegData GetEBASE(void) const = 0;
  virtual bool GetDV(void) const = 0;
  virtual bool GetRINT(void) const = 0;
  virtual void Reset(void) = 0;
  virtual void Write(RegData data, PrivLevel priv, ContextId ctxt) = 0;
  virtual RegData Read(PrivLevel priv, ContextId ctxt) const = 0;
  virtual std::string GetName(void) const = 0;
  virtual CfSReg GetCfSRegId(void) const = 0;
  virtual ~SRegGMEBASE_IF() {}
};

/*!
  @brief GMEIIC register interface class
 */
class SRegGMEIIC_IF {
public:
  virtual void SetEIIC(RegData eiic) = 0;
  virtual RegData GetEIIC(void) const = 0;
  virtual void Reset(void) = 0;
  virtual void Write(RegData data, PrivLevel priv, ContextId ctxt) = 0;
  virtual RegData Read(PrivLevel priv, ContextId ctxt) const = 0;
  virtual std::string GetName(void) const = 0;
  virtual CfSReg GetCfSRegId(void) const = 0;
  virtual ~SRegGMEIIC_IF() {}
};

/*!
  @brief GMEIPC register interface class
 */
class SRegGMEIPC_IF {
public:
  virtual void SetEIPC(RegData eipc) = 0;
  virtual RegData GetEIPC(void) const = 0;
  virtual void Reset(void) = 0;
  virtual void Write(RegData data, PrivLevel priv, ContextId ctxt) = 0;
  virtual RegData Read(PrivLevel priv, ContextId ctxt) const = 0;
  virtual std::string GetName(void) const = 0;
  virtual CfSReg GetCfSRegId(void) const = 0;
  virtual ~SRegGMEIPC_IF() {}
};

/*!
  @brief GMEIPSW register interface class
 */
class SRegGMEIPSW_IF {
public:
  virtual void SetUM(bool um) = 0;
  virtual void SetEIMASK(RegData eimask) = 0;
  virtual void SetCU(RegData cu) = 0;
  virtual void SetEBV(bool ebv) = 0;
  virtual void SetNP(bool np) = 0;
  virtual void SetEP(bool ep) = 0;
  virtual void SetID(bool id) = 0;
  virtual void SetSAT(bool sat) = 0;
  virtual void SetCY(bool cy) = 0;
  virtual void SetOV(bool ov) = 0;
  virtual void SetS(bool s) = 0;
  virtual void SetZ(bool z) = 0;
  virtual bool GetUM(void) const = 0;
  virtual RegData GetEIMASK(void) const = 0;
  virtual RegData GetCU(void) const = 0;
  virtual bool GetEBV(void) const = 0;
  virtual bool GetNP(void) const = 0;
  virtual bool GetEP(void) const = 0;
  virtual bool GetID(void) const = 0;
  virtual bool GetSAT(void) const = 0;
  virtual bool GetCY(void) const = 0;
  virtual bool GetOV(void) const = 0;
  virtual bool GetS(void) const = 0;
  virtual bool GetZ(void) const = 0;
  virtual void Reset(void) = 0;
  virtual void Write(RegData data, PrivLevel priv, ContextId ctxt) = 0;
  virtual RegData Read(PrivLevel priv, ContextId ctxt) const = 0;
  virtual std::string GetName(void) const = 0;
  virtual CfSReg GetCfSRegId(void) const = 0;
  virtual ~SRegGMEIPSW_IF() {}
};

/*!
  @brief GMEIWR register interface class
 */
class SRegGMEIWR_IF {
public:
  virtual void SetEIWR(RegData eiwr) = 0;
  virtual RegData GetEIWR(void) const = 0;
  virtual void Reset(void) = 0;
  virtual void Write(RegData data, PrivLevel priv, ContextId ctxt) = 0;
  virtual RegData Read(PrivLevel priv, ContextId ctxt) const = 0;
  virtual std::string GetName(void) const = 0;
  virtual CfSReg GetCfSRegId(void) const = 0;
  virtual ~SRegGMEIWR_IF() {}
};

/*!
  @brief GMFEIC register interface class
 */
class SRegGMFEIC_IF {
public:
  virtual void SetFEIC(RegData feic) = 0;
  virtual RegData GetFEIC(void) const = 0;
  virtual void Reset(void) = 0;
  virtual void Write(RegData data, PrivLevel priv, ContextId ctxt) = 0;
  virtual RegData Read(PrivLevel priv, ContextId ctxt) const = 0;
  virtual std::string GetName(void) const = 0;
  virtual CfSReg GetCfSRegId(void) const = 0;
  virtual ~SRegGMFEIC_IF() {}
};

/*!
  @brief GMFEPC register interface class
 */
class SRegGMFEPC_IF {
public:
  virtual void SetFEPC(RegData fepc) = 0;
  virtual RegData GetFEPC(void) const = 0;
  virtual void Reset(void) = 0;
  virtual void Write(RegData data, PrivLevel priv, ContextId ctxt) = 0;
  virtual RegData Read(PrivLevel priv, ContextId ctxt) const = 0;
  virtual std::string GetName(void) const = 0;
  virtual CfSReg GetCfSRegId(void) const = 0;
  virtual ~SRegGMFEPC_IF() {}
};

/*!
  @brief GMFEPSW register interface class
 */
class SRegGMFEPSW_IF {
public:
  virtual void SetUM(bool um) = 0;
  virtual void SetEIMASK(RegData eimask) = 0;
  virtual void SetCU(RegData cu) = 0;
  virtual void SetEBV(bool ebv) = 0;
  virtual void SetNP(bool np) = 0;
  virtual void SetEP(bool ep) = 0;
  virtual void SetID(bool id) = 0;
  virtual void SetSAT(bool sat) = 0;
  virtual void SetCY(bool cy) = 0;
  virtual void SetOV(bool ov) = 0;
  virtual void SetS(bool s) = 0;
  virtual void SetZ(bool z) = 0;
  virtual bool GetUM(void) const = 0;
  virtual RegData GetEIMASK(void) const = 0;
  virtual RegData GetCU(void) const = 0;
  virtual bool GetEBV(void) const = 0;
  virtual bool GetNP(void) const = 0;
  virtual bool GetEP(void) const = 0;
  virtual bool GetID(void) const = 0;
  virtual bool GetSAT(void) const = 0;
  virtual bool GetCY(void) const = 0;
  virtual bool GetOV(void) const = 0;
  virtual bool GetS(void) const = 0;
  virtual bool GetZ(void) const = 0;
  virtual void Reset(void) = 0;
  virtual void Write(RegData data, PrivLevel priv, ContextId ctxt) = 0;
  virtual RegData Read(PrivLevel priv, ContextId ctxt) const = 0;
  virtual std::string GetName(void) const = 0;
  virtual CfSReg GetCfSRegId(void) const = 0;
  virtual ~SRegGMFEPSW_IF() {}
};

/*!
  @brief GMFEWR register interface class
 */
class SRegGMFEWR_IF {
public:
  virtual void SetFEWR(RegData fewr) = 0;
  virtual RegData GetFEWR(void) const = 0;
  virtual void Reset(void) = 0;
  virtual void Write(RegData data, PrivLevel priv, ContextId ctxt) = 0;
  virtual RegData Read(PrivLevel priv, ContextId ctxt) const = 0;
  virtual std::string GetName(void) const = 0;
  virtual CfSReg GetCfSRegId(void) const = 0;
  virtual ~SRegGMFEWR_IF() {}
};

/*!
  @brief GMINTBP register interface class
 */
class SRegGMINTBP_IF {
public:
  virtual void SetINTBP(RegData intbp) = 0;
  virtual RegData GetINTBP(void) const = 0;
  virtual void Reset(void) = 0;
  virtual void Write(RegData data, PrivLevel priv, ContextId ctxt) = 0;
  virtual RegData Read(PrivLevel priv, ContextId ctxt) const = 0;
  virtual std::string GetName(void) const = 0;
  virtual CfSReg GetCfSRegId(void) const = 0;
  virtual ~SRegGMINTBP_IF() {}
};

/*!
  @brief GMINTCFG register interface class
 */
class SRegGMINTCFG_IF {
public:
  virtual void SetULNR(RegData ulnr) = 0;
  virtual void SetEPL(bool epl) = 0;
  virtual void SetISPC(bool ispc) = 0;
  virtual RegData GetULNR(void) const = 0;
  virtual bool GetEPL(void) const = 0;
  virtual bool GetISPC(void) const = 0;
  virtual void Reset(void) = 0;
  virtual void Write(RegData data, PrivLevel priv, ContextId ctxt) = 0;
  virtual RegData Read(PrivLevel priv, ContextId ctxt) const = 0;
  virtual std::string GetName(void) const = 0;
  virtual CfSReg GetCfSRegId(void) const = 0;
  virtual ~SRegGMINTCFG_IF() {}
};

/*!
  @brief GMMEA register interface class
 */
class SRegGMMEA_IF {
public:
  virtual void SetMEA(RegData mea) = 0;
  virtual RegData GetMEA(void) const = 0;
  virtual void Reset(void) = 0;
  virtual void Write(RegData data, PrivLevel priv, ContextId ctxt) = 0;
  virtual RegData Read(PrivLevel priv, ContextId ctxt) const = 0;
  virtual std::string GetName(void) const = 0;
  virtual CfSReg GetCfSRegId(void) const = 0;
  virtual ~SRegGMMEA_IF() {}
};

/*!
  @brief GMMEI register interface class
 */
class SRegGMMEI_IF {
public:
  virtual void SetLEN(RegData len) = 0;
  virtual void SetREG(RegData reg) = 0;
  virtual void SetDS(RegData ds) = 0;
  virtual void SetU(bool u) = 0;
  virtual void SetITYPE(RegData itype) = 0;
  virtual void SetRW(bool rw) = 0;
  virtual RegData GetLEN(void) const = 0;
  virtual RegData GetREG(void) const = 0;
  virtual RegData GetDS(void) const = 0;
  virtual bool GetU(void) const = 0;
  virtual RegData GetITYPE(void) const = 0;
  virtual bool GetRW(void) const = 0;
  virtual void Reset(void) = 0;
  virtual void Write(RegData data, PrivLevel priv, ContextId ctxt) = 0;
  virtual RegData Read(PrivLevel priv, ContextId ctxt) const = 0;
  virtual std::string GetName(void) const = 0;
  virtual CfSReg GetCfSRegId(void) const = 0;
  virtual ~SRegGMMEI_IF() {}
};

/*!
  @brief GMMPM register interface class
 */
class SRegGMMPM_IF {
public:
  virtual void SetGMPE(bool gmpe) = 0;
  virtual void SetSVP(bool svp) = 0;
  virtual void SetMPE(bool mpe) = 0;
  virtual bool GetGMPE(void) const = 0;
  virtual bool GetSVP(void) const = 0;
  virtual bool GetMPE(void) const = 0;
  virtual void Reset(void) = 0;
  virtual void Write(RegData data, PrivLevel priv, ContextId ctxt) = 0;
  virtual RegData Read(PrivLevel priv, ContextId ctxt) const = 0;
  virtual std::string GetName(void) const = 0;
  virtual CfSReg GetCfSRegId(void) const = 0;
  virtual ~SRegGMMPM_IF() {}
};

/*!
  @brief GMPEID register interface class
 */
class SRegGMPEID_IF {
public:
  virtual void SetPEID(RegData peid) = 0;
  virtual RegData GetPEID(void) const = 0;
  virtual void Reset(void) = 0;
  virtual void Write(RegData data, PrivLevel priv, ContextId ctxt) = 0;
  virtual RegData Read(PrivLevel priv, ContextId ctxt) const = 0;
  virtual std::string GetName(void) const = 0;
  virtual CfSReg GetCfSRegId(void) const = 0;
  virtual ~SRegGMPEID_IF() {}
};

/*!
  @brief GMPLMR register interface class
 */
class SRegGMPLMR_IF {
public:
  virtual void SetPLM(RegData plm) = 0;
  virtual RegData GetPLM(void) const = 0;
  virtual void Reset(void) = 0;
  virtual void Write(RegData data, PrivLevel priv, ContextId ctxt) = 0;
  virtual RegData Read(PrivLevel priv, ContextId ctxt) const = 0;
  virtual std::string GetName(void) const = 0;
  virtual CfSReg GetCfSRegId(void) const = 0;
  virtual ~SRegGMPLMR_IF() {}
};

/*!
  @brief GMPSW register interface class
 */
class SRegGMPSW_IF {
public:
  virtual void SetUM(bool um) = 0;
  virtual void SetEIMASK(RegData eimask) = 0;
  virtual void SetCU(RegData cu) = 0;
  virtual void SetEBV(bool ebv) = 0;
  virtual void SetNP(bool np) = 0;
  virtual void SetEP(bool ep) = 0;
  virtual void SetID(bool id) = 0;
  virtual void SetSAT(bool sat) = 0;
  virtual void SetCY(bool cy) = 0;
  virtual void SetOV(bool ov) = 0;
  virtual void SetS(bool s) = 0;
  virtual void SetZ(bool z) = 0;
  virtual bool GetUM(void) const = 0;
  virtual RegData GetEIMASK(void) const = 0;
  virtual RegData GetCU(void) const = 0;
  virtual bool GetEBV(void) const = 0;
  virtual bool GetNP(void) const = 0;
  virtual bool GetEP(void) const = 0;
  virtual bool GetID(void) const = 0;
  virtual bool GetSAT(void) const = 0;
  virtual bool GetCY(void) const = 0;
  virtual bool GetOV(void) const = 0;
  virtual bool GetS(void) const = 0;
  virtual bool GetZ(void) const = 0;
  virtual void Reset(void) = 0;
  virtual void Write(RegData data, PrivLevel priv, ContextId ctxt) = 0;
  virtual RegData Read(PrivLevel priv, ContextId ctxt) const = 0;
  virtual std::string GetName(void) const = 0;
  virtual CfSReg GetCfSRegId(void) const = 0;
  virtual ~SRegGMPSW_IF() {}
};

/*!
  @brief GMSPID register interface class
 */
class SRegGMSPID_IF {
public:
  virtual void SetSPID(RegData spid) = 0;
  virtual RegData GetSPID(void) const = 0;
  virtual void Reset(void) = 0;
  virtual void Write(RegData data, PrivLevel priv, ContextId ctxt) = 0;
  virtual RegData Read(PrivLevel priv, ContextId ctxt) const = 0;
  virtual std::string GetName(void) const = 0;
  virtual CfSReg GetCfSRegId(void) const = 0;
  virtual ~SRegGMSPID_IF() {}
};

/*!
  @brief GMSPIDLIST register interface class
 */
class SRegGMSPIDLIST_IF {
public:
  virtual void SetSPIDLIST(RegData spidlist) = 0;
  virtual RegData GetSPIDLIST(void) const = 0;
  virtual void Reset(void) = 0;
  virtual void Write(RegData data, PrivLevel priv, ContextId ctxt) = 0;
  virtual RegData Read(PrivLevel priv, ContextId ctxt) const = 0;
  virtual std::string GetName(void) const = 0;
  virtual CfSReg GetCfSRegId(void) const = 0;
  virtual ~SRegGMSPIDLIST_IF() {}
};

/*!
  @brief GMSVLOCK register interface class
 */
class SRegGMSVLOCK_IF {
public:
  virtual void SetSVL(bool svl) = 0;
  virtual bool GetSVL(void) const = 0;
  virtual void Reset(void) = 0;
  virtual void Write(RegData data, PrivLevel priv, ContextId ctxt) = 0;
  virtual RegData Read(PrivLevel priv, ContextId ctxt) const = 0;
  virtual std::string GetName(void) const = 0;
  virtual CfSReg GetCfSRegId(void) const = 0;
  virtual ~SRegGMSVLOCK_IF() {}
};

/*!
  @brief HVCFG register interface class
 */
class SRegHVCFG_IF {
public:
  virtual void SetHVE(bool hve) = 0;
  virtual bool GetHVE(void) const = 0;
  virtual void Reset(void) = 0;
  virtual void Write(RegData data, PrivLevel priv, ContextId ctxt) = 0;
  virtual RegData Read(PrivLevel priv, ContextId ctxt) const = 0;
  virtual std::string GetName(void) const = 0;
  virtual CfSReg GetCfSRegId(void) const = 0;
  virtual ~SRegHVCFG_IF() {}
};

/*!
  @brief HVSB register interface class
 */
class SRegHVSB_IF {
public:
  virtual void SetHVSB(RegData hvsb) = 0;
  virtual RegData GetHVSB(void) const = 0;
  virtual void Reset(void) = 0;
  virtual void Write(RegData data, PrivLevel priv, ContextId ctxt) = 0;
  virtual RegData Read(PrivLevel priv, ContextId ctxt) const = 0;
  virtual std::string GetName(void) const = 0;
  virtual CfSReg GetCfSRegId(void) const = 0;
  virtual ~SRegHVSB_IF() {}
};

/*!
  @brief ICBKEY register interface class
 */
class SRegICBKEY_IF {
public:
  virtual void SetCBAKEY(RegData cbakey) = 0;
  virtual void SetKEYCODE(RegData keycode) = 0;
  virtual RegData GetCBAKEY(void) const = 0;
  virtual RegData GetKEYCODE(void) const = 0;
  virtual void Reset(void) = 0;
  virtual void Write(RegData data, PrivLevel priv, ContextId ctxt) = 0;
  virtual RegData Read(PrivLevel priv, ContextId ctxt) const = 0;
  virtual std::string GetName(void) const = 0;
  virtual CfSReg GetCfSRegId(void) const = 0;
  virtual ~SRegICBKEY_IF() {}
};

/*!
  @brief ICCFG register interface class
 */
class SRegICCFG_IF {
public:
  virtual void SetICHSIZE(RegData ichsize) = 0;
  virtual void SetICHLINE(RegData ichline) = 0;
  virtual void SetICHWAY(RegData ichway) = 0;
  virtual RegData GetICHSIZE(void) const = 0;
  virtual RegData GetICHLINE(void) const = 0;
  virtual RegData GetICHWAY(void) const = 0;
  virtual void Reset(void) = 0;
  virtual void Write(RegData data, PrivLevel priv, ContextId ctxt) = 0;
  virtual RegData Read(PrivLevel priv, ContextId ctxt) const = 0;
  virtual std::string GetName(void) const = 0;
  virtual CfSReg GetCfSRegId(void) const = 0;
  virtual ~SRegICCFG_IF() {}
};

/*!
  @brief ICCTRL register interface class
 */
class SRegICCTRL_IF {
public:
  virtual void SetPBS(bool pbs) = 0;
  virtual void SetICHCLR(bool ichclr) = 0;
  virtual void SetICHEIV(bool icheiv) = 0;
  virtual void SetICHEMK(bool ichemk) = 0;
  virtual void SetICHEN(bool ichen) = 0;
  virtual bool GetPBS(void) const = 0;
  virtual bool GetICHCLR(void) const = 0;
  virtual bool GetICHEIV(void) const = 0;
  virtual bool GetICHEMK(void) const = 0;
  virtual bool GetICHEN(void) const = 0;
  virtual void Reset(void) = 0;
  virtual void Write(RegData data, PrivLevel priv, ContextId ctxt) = 0;
  virtual RegData Read(PrivLevel priv, ContextId ctxt) const = 0;
  virtual std::string GetName(void) const = 0;
  virtual CfSReg GetCfSRegId(void) const = 0;
  virtual ~SRegICCTRL_IF() {}
};

/*!
  @brief ICDATH register interface class
 */
class SRegICDATH_IF {
public:
  virtual void SetDATAH(RegData datah) = 0;
  virtual RegData GetDATAH(void) const = 0;
  virtual void Reset(void) = 0;
  virtual void Write(RegData data, PrivLevel priv, ContextId ctxt) = 0;
  virtual RegData Read(PrivLevel priv, ContextId ctxt) const = 0;
  virtual std::string GetName(void) const = 0;
  virtual CfSReg GetCfSRegId(void) const = 0;
  virtual ~SRegICDATH_IF() {}
};

/*!
  @brief ICDATL register interface class
 */
class SRegICDATL_IF {
public:
  virtual void SetDATAL(RegData datal) = 0;
  virtual RegData GetDATAL(void) const = 0;
  virtual void Reset(void) = 0;
  virtual void Write(RegData data, PrivLevel priv, ContextId ctxt) = 0;
  virtual RegData Read(PrivLevel priv, ContextId ctxt) const = 0;
  virtual std::string GetName(void) const = 0;
  virtual CfSReg GetCfSRegId(void) const = 0;
  virtual ~SRegICDATL_IF() {}
};

/*!
  @brief ICERR register interface class
 */
class SRegICERR_IF {
public:
  virtual void SetCISTW(bool cistw) = 0;
  virtual void SetESAFE(bool esafe) = 0;
  virtual void SetESMH(bool esmh) = 0;
  virtual void SetESPBSE(bool espbse) = 0;
  virtual void SetESTE1(bool este1) = 0;
  virtual void SetESTE2(bool este2) = 0;
  virtual void SetESDC(bool esdc) = 0;
  virtual void SetESDE(bool esde) = 0;
  virtual void SetERMAFE(bool ermafe) = 0;
  virtual void SetERMMH(bool ermmh) = 0;
  virtual void SetERMPBSE(bool ermpbse) = 0;
  virtual void SetERMTE1(bool ermte1) = 0;
  virtual void SetERMTE2(bool ermte2) = 0;
  virtual void SetERMDC(bool ermdc) = 0;
  virtual void SetERMDE(bool ermde) = 0;
  virtual void SetICHEWY(RegData ichewy) = 0;
  virtual void SetICHEIX(RegData icheix) = 0;
  virtual void SetICHERQ(bool icherq) = 0;
  virtual void SetICHED(bool iched) = 0;
  virtual void SetICHET(bool ichet) = 0;
  virtual void SetICHERR(bool icherr) = 0;
  virtual bool GetCISTW(void) const = 0;
  virtual bool GetESAFE(void) const = 0;
  virtual bool GetESMH(void) const = 0;
  virtual bool GetESPBSE(void) const = 0;
  virtual bool GetESTE1(void) const = 0;
  virtual bool GetESTE2(void) const = 0;
  virtual bool GetESDC(void) const = 0;
  virtual bool GetESDE(void) const = 0;
  virtual bool GetERMAFE(void) const = 0;
  virtual bool GetERMMH(void) const = 0;
  virtual bool GetERMPBSE(void) const = 0;
  virtual bool GetERMTE1(void) const = 0;
  virtual bool GetERMTE2(void) const = 0;
  virtual bool GetERMDC(void) const = 0;
  virtual bool GetERMDE(void) const = 0;
  virtual RegData GetICHEWY(void) const = 0;
  virtual RegData GetICHEIX(void) const = 0;
  virtual bool GetICHERQ(void) const = 0;
  virtual bool GetICHED(void) const = 0;
  virtual bool GetICHET(void) const = 0;
  virtual bool GetICHERR(void) const = 0;
  virtual void Reset(void) = 0;
  virtual void Write(RegData data, PrivLevel priv, ContextId ctxt) = 0;
  virtual RegData Read(PrivLevel priv, ContextId ctxt) const = 0;
  virtual std::string GetName(void) const = 0;
  virtual CfSReg GetCfSRegId(void) const = 0;
  virtual ~SRegICERR_IF() {}
};

/*!
  @brief ICSR register interface class
 */
class SRegICSR_IF {
public:
  virtual void SetPMFP(bool pmfp) = 0;
  virtual void SetPMEI(bool pmei) = 0;
  virtual bool GetPMFP(void) const = 0;
  virtual bool GetPMEI(void) const = 0;
  virtual void Reset(void) = 0;
  virtual void Write(RegData data, PrivLevel priv, ContextId ctxt) = 0;
  virtual RegData Read(PrivLevel priv, ContextId ctxt) const = 0;
  virtual std::string GetName(void) const = 0;
  virtual CfSReg GetCfSRegId(void) const = 0;
  virtual ~SRegICSR_IF() {}
};

/*!
  @brief ICTAGH register interface class
 */
class SRegICTAGH_IF {
public:
  virtual void SetWD(bool wd) = 0;
  virtual void SetPD(bool pd) = 0;
  virtual void SetWT(bool wt) = 0;
  virtual void SetPT(bool pt) = 0;
  virtual void SetDATAECC(RegData dataecc) = 0;
  virtual void SetTAGECC(RegData tagecc) = 0;
  virtual bool GetWD(void) const = 0;
  virtual bool GetPD(void) const = 0;
  virtual bool GetWT(void) const = 0;
  virtual bool GetPT(void) const = 0;
  virtual RegData GetDATAECC(void) const = 0;
  virtual RegData GetTAGECC(void) const = 0;
  virtual void Reset(void) = 0;
  virtual void Write(RegData data, PrivLevel priv, ContextId ctxt) = 0;
  virtual RegData Read(PrivLevel priv, ContextId ctxt) const = 0;
  virtual std::string GetName(void) const = 0;
  virtual CfSReg GetCfSRegId(void) const = 0;
  virtual ~SRegICTAGH_IF() {}
};

/*!
  @brief ICTAGL register interface class
 */
class SRegICTAGL_IF {
public:
  virtual void SetLPN(RegData lpn) = 0;
  virtual void SetLRU(RegData lru) = 0;
  virtual void SetL(bool l) = 0;
  virtual void SetV(bool v) = 0;
  virtual RegData GetLPN(void) const = 0;
  virtual RegData GetLRU(void) const = 0;
  virtual bool GetL(void) const = 0;
  virtual bool GetV(void) const = 0;
  virtual void Reset(void) = 0;
  virtual void Write(RegData data, PrivLevel priv, ContextId ctxt) = 0;
  virtual RegData Read(PrivLevel priv, ContextId ctxt) const = 0;
  virtual std::string GetName(void) const = 0;
  virtual CfSReg GetCfSRegId(void) const = 0;
  virtual ~SRegICTAGL_IF() {}
};

/*!
  @brief IFCR register interface class
 */
class SRegIFCR_IF {
public:
  virtual void SetPLEN(bool plen) = 0;
  virtual bool GetPLEN(void) const = 0;
  virtual void Reset(void) = 0;
  virtual void Write(RegData data, PrivLevel priv, ContextId ctxt) = 0;
  virtual RegData Read(PrivLevel priv, ContextId ctxt) const = 0;
  virtual std::string GetName(void) const = 0;
  virtual CfSReg GetCfSRegId(void) const = 0;
  virtual ~SRegIFCR_IF() {}
};

/*!
  @brief IFCR1 register interface class
 */
class SRegIFCR1_IF {
public:
  virtual void SetFD2(bool fd2) = 0;
  virtual void SetPLINTD(bool plintd) = 0;
  virtual void SetPLNLND(bool plnlnd) = 0;
  virtual bool GetFD2(void) const = 0;
  virtual bool GetPLINTD(void) const = 0;
  virtual bool GetPLNLND(void) const = 0;
  virtual void Reset(void) = 0;
  virtual void Write(RegData data, PrivLevel priv, ContextId ctxt) = 0;
  virtual RegData Read(PrivLevel priv, ContextId ctxt) const = 0;
  virtual std::string GetName(void) const = 0;
  virtual CfSReg GetCfSRegId(void) const = 0;
  virtual ~SRegIFCR1_IF() {}
};

/*!
  @brief IMSR register interface class
 */
class SRegIMSR_IF {
public:
  virtual void SetHFNP(bool hfnp) = 0;
  virtual void SetHENP(bool henp) = 0;
  virtual void SetHEID(bool heid) = 0;
  virtual void SetHEPLM(bool heplm) = 0;
  virtual void SetHEEIM(bool heeim) = 0;
  virtual void SetFNP(bool fnp) = 0;
  virtual void SetENP(bool enp) = 0;
  virtual void SetEID(bool eid) = 0;
  virtual void SetEPLM(bool eplm) = 0;
  virtual void SetEEIM(bool eeim) = 0;
  virtual bool GetHFNP(void) const = 0;
  virtual bool GetHENP(void) const = 0;
  virtual bool GetHEID(void) const = 0;
  virtual bool GetHEPLM(void) const = 0;
  virtual bool GetHEEIM(void) const = 0;
  virtual bool GetFNP(void) const = 0;
  virtual bool GetENP(void) const = 0;
  virtual bool GetEID(void) const = 0;
  virtual bool GetEPLM(void) const = 0;
  virtual bool GetEEIM(void) const = 0;
  virtual void Reset(void) = 0;
  virtual void Write(RegData data, PrivLevel priv, ContextId ctxt) = 0;
  virtual RegData Read(PrivLevel priv, ContextId ctxt) const = 0;
  virtual std::string GetName(void) const = 0;
  virtual CfSReg GetCfSRegId(void) const = 0;
  virtual ~SRegIMSR_IF() {}
};

/*!
  @brief INTBP register interface class
 */
class SRegHMINTBP_IF {
public:
  virtual void SetINTBP(RegData intbp) = 0;
  virtual RegData GetINTBP(void) const = 0;
  virtual void Reset(void) = 0;
  virtual void Write(RegData data, PrivLevel priv, ContextId ctxt) = 0;
  virtual RegData Read(PrivLevel priv, ContextId ctxt) const = 0;
  virtual std::string GetName(void) const = 0;
  virtual CfSReg GetCfSRegId(void) const = 0;
  virtual ~SRegHMINTBP_IF() {}
};

/*!
  @brief INTCFG register interface class
 */
class SRegHMINTCFG_IF {
public:
  virtual void SetULNR(RegData ulnr) = 0;
  virtual void SetEPL(bool epl) = 0;
  virtual void SetISPC(bool ispc) = 0;
  virtual RegData GetULNR(void) const = 0;
  virtual bool GetEPL(void) const = 0;
  virtual bool GetISPC(void) const = 0;
  virtual void Reset(void) = 0;
  virtual void Write(RegData data, PrivLevel priv, ContextId ctxt) = 0;
  virtual RegData Read(PrivLevel priv, ContextId ctxt) const = 0;
  virtual std::string GetName(void) const = 0;
  virtual CfSReg GetCfSRegId(void) const = 0;
  virtual ~SRegHMINTCFG_IF() {}
};

/*!
  @brief ISPR register interface class
 */
class SRegISPR_IF {
public:
  virtual void SetISP(RegData isp) = 0;
  virtual RegData GetISP(void) const = 0;
  virtual void Reset(void) = 0;
  virtual void Write(RegData data, PrivLevel priv, ContextId ctxt) = 0;
  virtual RegData Read(PrivLevel priv, ContextId ctxt) const = 0;
  virtual std::string GetName(void) const = 0;
  virtual CfSReg GetCfSRegId(void) const = 0;
  virtual ~SRegISPR_IF() {}
};

/*!
  @brief L1RCFG register interface class
 */
class SRegL1RCFG_IF {
public:
  virtual void SetL1RSIZE(RegData l1rsize) = 0;
  virtual void SetL1RLNKE(RegData l1rlnke) = 0;
  virtual RegData GetL1RSIZE(void) const = 0;
  virtual RegData GetL1RLNKE(void) const = 0;
  virtual void Reset(void) = 0;
  virtual void Write(RegData data, PrivLevel priv, ContextId ctxt) = 0;
  virtual RegData Read(PrivLevel priv, ContextId ctxt) const = 0;
  virtual std::string GetName(void) const = 0;
  virtual CfSReg GetCfSRegId(void) const = 0;
  virtual ~SRegL1RCFG_IF() {}
};

/*!
  @brief L1RLCR register interface class
 */
class SRegL1RLCR_IF {
public:
  virtual void SetASEL(RegData asel) = 0;
  virtual void SetCLR(bool clr) = 0;
  virtual RegData GetASEL(void) const = 0;
  virtual bool GetCLR(void) const = 0;
  virtual void Reset(void) = 0;
  virtual void Write(RegData data, PrivLevel priv, ContextId ctxt) = 0;
  virtual RegData Read(PrivLevel priv, ContextId ctxt) const = 0;
  virtual std::string GetName(void) const = 0;
  virtual CfSReg GetCfSRegId(void) const = 0;
  virtual ~SRegL1RLCR_IF() {}
};

/*!
  @brief L1RLNK0 register interface class
 */
class SRegL1RLNK0_IF {
public:
  virtual void SetTID(RegData tid) = 0;
  virtual void SetBMID(RegData bmid) = 0;
  virtual void SetSIZE(RegData size) = 0;
  virtual void SetV(bool v) = 0;
  virtual RegData GetTID(void) const = 0;
  virtual RegData GetBMID(void) const = 0;
  virtual RegData GetSIZE(void) const = 0;
  virtual bool GetV(void) const = 0;
  virtual void Reset(void) = 0;
  virtual void Write(RegData data, PrivLevel priv, ContextId ctxt) = 0;
  virtual RegData Read(PrivLevel priv, ContextId ctxt) const = 0;
  virtual std::string GetName(void) const = 0;
  virtual CfSReg GetCfSRegId(void) const = 0;
  virtual ~SRegL1RLNK0_IF() {}
};

/*!
  @brief L1RLNK1 register interface class
 */
class SRegL1RLNK1_IF {
public:
  virtual void SetADDR(RegData addr) = 0;
  virtual RegData GetADDR(void) const = 0;
  virtual void Reset(void) = 0;
  virtual void Write(RegData data, PrivLevel priv, ContextId ctxt) = 0;
  virtual RegData Read(PrivLevel priv, ContextId ctxt) const = 0;
  virtual std::string GetName(void) const = 0;
  virtual CfSReg GetCfSRegId(void) const = 0;
  virtual ~SRegL1RLNK1_IF() {}
};

/*!
  @brief LSCFG register interface class
 */
class SRegLSCFG_IF {
public:
  virtual void SetLSE(bool lse) = 0;
  virtual bool GetLSE(void) const = 0;
  virtual void Reset(void) = 0;
  virtual void Write(RegData data, PrivLevel priv, ContextId ctxt) = 0;
  virtual RegData Read(PrivLevel priv, ContextId ctxt) const = 0;
  virtual std::string GetName(void) const = 0;
  virtual CfSReg GetCfSRegId(void) const = 0;
  virtual ~SRegLSCFG_IF() {}
};

/*!
  @brief LSTEST0 register interface class
 */
class SRegLSTEST0_IF {
public:
  virtual void SetLSTEST0(RegData lstest0) = 0;
  virtual RegData GetLSTEST0(void) const = 0;
  virtual void Reset(void) = 0;
  virtual void Write(RegData data, PrivLevel priv, ContextId ctxt) = 0;
  virtual RegData Read(PrivLevel priv, ContextId ctxt) const = 0;
  virtual std::string GetName(void) const = 0;
  virtual CfSReg GetCfSRegId(void) const = 0;
  virtual ~SRegLSTEST0_IF() {}
};

/*!
  @brief LSTEST1 register interface class
 */
class SRegLSTEST1_IF {
public:
  virtual void SetLSTEST1(RegData lstest1) = 0;
  virtual RegData GetLSTEST1(void) const = 0;
  virtual void Reset(void) = 0;
  virtual void Write(RegData data, PrivLevel priv, ContextId ctxt) = 0;
  virtual RegData Read(PrivLevel priv, ContextId ctxt) const = 0;
  virtual std::string GetName(void) const = 0;
  virtual CfSReg GetCfSRegId(void) const = 0;
  virtual ~SRegLSTEST1_IF() {}
};

/*!
  @brief LSUCR register interface class
 */
class SRegLSUCR_IF {
public:
  virtual void SetBBOPEN(bool bbopen) = 0;
  virtual bool GetBBOPEN(void) const = 0;
  virtual void Reset(void) = 0;
  virtual void Write(RegData data, PrivLevel priv, ContextId ctxt) = 0;
  virtual RegData Read(PrivLevel priv, ContextId ctxt) const = 0;
  virtual std::string GetName(void) const = 0;
  virtual CfSReg GetCfSRegId(void) const = 0;
  virtual ~SRegLSUCR_IF() {}
};

/*!
  @brief LSULNK0 register interface class
 */
class SRegLSULNK0_IF {
public:
  virtual void SetSIZE(RegData size) = 0;
  virtual void SetV(bool v) = 0;
  virtual RegData GetSIZE(void) const = 0;
  virtual bool GetV(void) const = 0;
  virtual void Reset(void) = 0;
  virtual void Write(RegData data, PrivLevel priv, ContextId ctxt) = 0;
  virtual RegData Read(PrivLevel priv, ContextId ctxt) const = 0;
  virtual std::string GetName(void) const = 0;
  virtual CfSReg GetCfSRegId(void) const = 0;
  virtual ~SRegLSULNK0_IF() {}
};

/*!
  @brief LSULNK1 register interface class
 */
class SRegLSULNK1_IF {
public:
  virtual void SetADDR(RegData addr) = 0;
  virtual RegData GetADDR(void) const = 0;
  virtual void Reset(void) = 0;
  virtual void Write(RegData data, PrivLevel priv, ContextId ctxt) = 0;
  virtual RegData Read(PrivLevel priv, ContextId ctxt) const = 0;
  virtual std::string GetName(void) const = 0;
  virtual CfSReg GetCfSRegId(void) const = 0;
  virtual ~SRegLSULNK1_IF() {}
};

/*!
  @brief MCA register interface class
 */
class SRegMCA_IF {
public:
  virtual void SetMCA(RegData mca) = 0;
  virtual RegData GetMCA(void) const = 0;
  virtual void Reset(void) = 0;
  virtual void Write(RegData data, PrivLevel priv, ContextId ctxt) = 0;
  virtual RegData Read(PrivLevel priv, ContextId ctxt) const = 0;
  virtual std::string GetName(void) const = 0;
  virtual CfSReg GetCfSRegId(void) const = 0;
  virtual ~SRegMCA_IF() {}
};

/*!
  @brief MCC register interface class
 */
class SRegMCC_IF {
public:
  virtual void SetMCC(RegData mcc) = 0;
  virtual RegData GetMCC(void) const = 0;
  virtual void Reset(void) = 0;
  virtual void Write(RegData data, PrivLevel priv, ContextId ctxt) = 0;
  virtual RegData Read(PrivLevel priv, ContextId ctxt) const = 0;
  virtual std::string GetName(void) const = 0;
  virtual CfSReg GetCfSRegId(void) const = 0;
  virtual ~SRegMCC_IF() {}
};

/*!
  @brief MCI register interface class
 */
class SRegMCI_IF {
public:
  virtual void SetSPID(RegData spid) = 0;
  virtual RegData GetSPID(void) const = 0;
  virtual void Reset(void) = 0;
  virtual void Write(RegData data, PrivLevel priv, ContextId ctxt) = 0;
  virtual RegData Read(PrivLevel priv, ContextId ctxt) const = 0;
  virtual std::string GetName(void) const = 0;
  virtual CfSReg GetCfSRegId(void) const = 0;
  virtual ~SRegMCI_IF() {}
};

/*!
  @brief MCR register interface class
 */
class SRegMCR_IF {
public:
  virtual void SetHSXE(bool hsxe) = 0;
  virtual void SetHSWE(bool hswe) = 0;
  virtual void SetHSRE(bool hsre) = 0;
  virtual void SetHUXE(bool huxe) = 0;
  virtual void SetHUWE(bool huwe) = 0;
  virtual void SetHURE(bool hure) = 0;
  virtual void SetGSXE(bool gsxe) = 0;
  virtual void SetGSWE(bool gswe) = 0;
  virtual void SetGSRE(bool gsre) = 0;
  virtual void SetGUXE(bool guxe) = 0;
  virtual void SetGUWE(bool guwe) = 0;
  virtual void SetGURE(bool gure) = 0;
  virtual void SetOV(bool ov) = 0;
  virtual void SetSXE(bool sxe) = 0;
  virtual void SetSWE(bool swe) = 0;
  virtual void SetSRE(bool sre) = 0;
  virtual void SetUXE(bool uxe) = 0;
  virtual void SetUWE(bool uwe) = 0;
  virtual void SetURE(bool ure) = 0;
  virtual bool GetHSXE(void) const = 0;
  virtual bool GetHSWE(void) const = 0;
  virtual bool GetHSRE(void) const = 0;
  virtual bool GetHUXE(void) const = 0;
  virtual bool GetHUWE(void) const = 0;
  virtual bool GetHURE(void) const = 0;
  virtual bool GetGSXE(void) const = 0;
  virtual bool GetGSWE(void) const = 0;
  virtual bool GetGSRE(void) const = 0;
  virtual bool GetGUXE(void) const = 0;
  virtual bool GetGUWE(void) const = 0;
  virtual bool GetGURE(void) const = 0;
  virtual bool GetOV(void) const = 0;
  virtual bool GetSXE(void) const = 0;
  virtual bool GetSWE(void) const = 0;
  virtual bool GetSRE(void) const = 0;
  virtual bool GetUXE(void) const = 0;
  virtual bool GetUWE(void) const = 0;
  virtual bool GetURE(void) const = 0;
  virtual void Reset(void) = 0;
  virtual void Write(RegData data, PrivLevel priv, ContextId ctxt) = 0;
  virtual RegData Read(PrivLevel priv, ContextId ctxt) const = 0;
  virtual std::string GetName(void) const = 0;
  virtual CfSReg GetCfSRegId(void) const = 0;
  virtual ~SRegMCR_IF() {}
};

/*!
  @brief MCS register interface class
 */
class SRegMCS_IF {
public:
  virtual void SetMCS(RegData mcs) = 0;
  virtual RegData GetMCS(void) const = 0;
  virtual void Reset(void) = 0;
  virtual void Write(RegData data, PrivLevel priv, ContextId ctxt) = 0;
  virtual RegData Read(PrivLevel priv, ContextId ctxt) const = 0;
  virtual std::string GetName(void) const = 0;
  virtual CfSReg GetCfSRegId(void) const = 0;
  virtual ~SRegMCS_IF() {}
};

/*!
  @brief MCTL register interface class
 */
class SRegMCTL_IF {
public:
  virtual void SetUIC(bool uic) = 0;
  virtual bool GetUIC(void) const = 0;
  virtual void Reset(void) = 0;
  virtual void Write(RegData data, PrivLevel priv, ContextId ctxt) = 0;
  virtual RegData Read(PrivLevel priv, ContextId ctxt) const = 0;
  virtual std::string GetName(void) const = 0;
  virtual CfSReg GetCfSRegId(void) const = 0;
  virtual ~SRegMCTL_IF() {}
};

/*!
  @brief MEA register interface class
 */
class SRegHMMEA_IF {
public:
  virtual void SetMEA(RegData mea) = 0;
  virtual RegData GetMEA(void) const = 0;
  virtual void Reset(void) = 0;
  virtual void Write(RegData data, PrivLevel priv, ContextId ctxt) = 0;
  virtual RegData Read(PrivLevel priv, ContextId ctxt) const = 0;
  virtual std::string GetName(void) const = 0;
  virtual CfSReg GetCfSRegId(void) const = 0;
  virtual ~SRegHMMEA_IF() {}
};

/*!
  @brief MEI register interface class
 */
class SRegHMMEI_IF {
public:
  virtual void SetLEN(RegData len) = 0;
  virtual void SetREG(RegData reg) = 0;
  virtual void SetDS(RegData ds) = 0;
  virtual void SetU(bool u) = 0;
  virtual void SetITYPE(RegData itype) = 0;
  virtual void SetRW(bool rw) = 0;
  virtual RegData GetLEN(void) const = 0;
  virtual RegData GetREG(void) const = 0;
  virtual RegData GetDS(void) const = 0;
  virtual bool GetU(void) const = 0;
  virtual RegData GetITYPE(void) const = 0;
  virtual bool GetRW(void) const = 0;
  virtual void Reset(void) = 0;
  virtual void Write(RegData data, PrivLevel priv, ContextId ctxt) = 0;
  virtual RegData Read(PrivLevel priv, ContextId ctxt) const = 0;
  virtual std::string GetName(void) const = 0;
  virtual CfSReg GetCfSRegId(void) const = 0;
  virtual ~SRegHMMEI_IF() {}
};

/*!
  @brief MPAT register interface class
 */
class SRegMPAT_IF {
public:
  virtual void SetMPAT(RegData mpat) = 0;
  virtual RegData GetMPAT(void) const = 0;
  virtual void Reset(void) = 0;
  virtual void Write(RegData data, PrivLevel priv, ContextId ctxt) = 0;
  virtual RegData Read(PrivLevel priv, ContextId ctxt) const = 0;
  virtual std::string GetName(void) const = 0;
  virtual CfSReg GetCfSRegId(void) const = 0;
  virtual ~SRegMPAT_IF() {}
};

/*!
  @brief MPAT_N register interface class
 */
class SRegMPAT_N_IF {
public:
  virtual void SetWMPID(RegData wmpid) = 0;
  virtual void SetRMPID(RegData rmpid) = 0;
  virtual void SetWG(bool wg) = 0;
  virtual void SetRG(bool rg) = 0;
  virtual void SetE(bool e) = 0;
  virtual void SetSX(bool sx) = 0;
  virtual void SetSW(bool sw) = 0;
  virtual void SetSR(bool sr) = 0;
  virtual void SetUX(bool ux) = 0;
  virtual void SetUW(bool uw) = 0;
  virtual void SetUR(bool ur) = 0;
  virtual RegData GetWMPID(void) const = 0;
  virtual RegData GetRMPID(void) const = 0;
  virtual bool GetWG(void) const = 0;
  virtual bool GetRG(void) const = 0;
  virtual bool GetE(void) const = 0;
  virtual bool GetSX(void) const = 0;
  virtual bool GetSW(void) const = 0;
  virtual bool GetSR(void) const = 0;
  virtual bool GetUX(void) const = 0;
  virtual bool GetUW(void) const = 0;
  virtual bool GetUR(void) const = 0;
  virtual void Reset(void) = 0;
  virtual void Write(RegData data, PrivLevel priv, ContextId ctxt) = 0;
  virtual RegData Read(PrivLevel priv, ContextId ctxt) const = 0;
  virtual std::string GetName(void) const = 0;
  virtual CfSReg GetCfSRegId(void) const = 0;
  virtual ~SRegMPAT_N_IF() {}
};

/*!
  @brief MPBK register interface class
 */
class SRegMPBK_IF {
public:
  virtual void SetBK(bool bk) = 0;
  virtual bool GetBK(void) const = 0;
  virtual void Reset(void) = 0;
  virtual void Write(RegData data, PrivLevel priv, ContextId ctxt) = 0;
  virtual RegData Read(PrivLevel priv, ContextId ctxt) const = 0;
  virtual std::string GetName(void) const = 0;
  virtual CfSReg GetCfSRegId(void) const = 0;
  virtual ~SRegMPBK_IF() {}
};

/*!
  @brief MPCFG register interface class
 */
class SRegMPCFG_IF {
public:
  virtual void SetDMDP(bool dmdp) = 0;
  virtual void SetNBK(RegData nbk) = 0;
  virtual void SetARCH(RegData arch) = 0;
  virtual void SetHBE(RegData hbe) = 0;
  virtual void SetNMPUE(RegData nmpue) = 0;
  virtual bool GetDMDP(void) const = 0;
  virtual RegData GetNBK(void) const = 0;
  virtual RegData GetARCH(void) const = 0;
  virtual RegData GetHBE(void) const = 0;
  virtual RegData GetNMPUE(void) const = 0;
  virtual void Reset(void) = 0;
  virtual void Write(RegData data, PrivLevel priv, ContextId ctxt) = 0;
  virtual RegData Read(PrivLevel priv, ContextId ctxt) const = 0;
  virtual std::string GetName(void) const = 0;
  virtual CfSReg GetCfSRegId(void) const = 0;
  virtual ~SRegMPCFG_IF() {}
};

/*!
  @brief MPID register interface class
 */
class SRegMPID_IF {
public:
  virtual void SetSPID(RegData spid) = 0;
  virtual RegData GetSPID(void) const = 0;
  virtual void Reset(void) = 0;
  virtual void Write(RegData data, PrivLevel priv, ContextId ctxt) = 0;
  virtual RegData Read(PrivLevel priv, ContextId ctxt) const = 0;
  virtual std::string GetName(void) const = 0;
  virtual CfSReg GetCfSRegId(void) const = 0;
  virtual ~SRegMPID_IF() {}
};

/*!
  @brief MPIDX register interface class
 */
class SRegMPIDX_IF {
public:
  virtual void SetMPIDX(RegData mpidx) = 0;
  virtual RegData GetMPIDX(void) const = 0;
  virtual void Reset(void) = 0;
  virtual void Write(RegData data, PrivLevel priv, ContextId ctxt) = 0;
  virtual RegData Read(PrivLevel priv, ContextId ctxt) const = 0;
  virtual std::string GetName(void) const = 0;
  virtual CfSReg GetCfSRegId(void) const = 0;
  virtual ~SRegMPIDX_IF() {}
};

/*!
  @brief MPLA register interface class
 */
class SRegMPLA_IF {
public:
  virtual void SetMPLA(RegData mpla) = 0;
  virtual RegData GetMPLA(void) const = 0;
  virtual void Reset(void) = 0;
  virtual void Write(RegData data, PrivLevel priv, ContextId ctxt) = 0;
  virtual RegData Read(PrivLevel priv, ContextId ctxt) const = 0;
  virtual std::string GetName(void) const = 0;
  virtual CfSReg GetCfSRegId(void) const = 0;
  virtual ~SRegMPLA_IF() {}
};

/*!
  @brief MPLA_N register interface class
 */
class SRegMPLA_N_IF {
public:
  virtual void SetMPLA(RegData mpla) = 0;
  virtual RegData GetMPLA(void) const = 0;
  virtual void Reset(void) = 0;
  virtual void Write(RegData data, PrivLevel priv, ContextId ctxt) = 0;
  virtual RegData Read(PrivLevel priv, ContextId ctxt) const = 0;
  virtual std::string GetName(void) const = 0;
  virtual CfSReg GetCfSRegId(void) const = 0;
  virtual ~SRegMPLA_N_IF() {}
};

/*!
  @brief MPM register interface class
 */
class SRegHMMPM_IF {
public:
  virtual void SetGMPE(bool gmpe) = 0;
  virtual void SetSVP(bool svp) = 0;
  virtual void SetMPE(bool mpe) = 0;
  virtual bool GetGMPE(void) const = 0;
  virtual bool GetSVP(void) const = 0;
  virtual bool GetMPE(void) const = 0;
  virtual void Reset(void) = 0;
  virtual void Write(RegData data, PrivLevel priv, ContextId ctxt) = 0;
  virtual RegData Read(PrivLevel priv, ContextId ctxt) const = 0;
  virtual std::string GetName(void) const = 0;
  virtual CfSReg GetCfSRegId(void) const = 0;
  virtual ~SRegHMMPM_IF() {}
};

/*!
  @brief MPUA register interface class
 */
class SRegMPUA_IF {
public:
  virtual void SetMPUA(RegData mpua) = 0;
  virtual RegData GetMPUA(void) const = 0;
  virtual void Reset(void) = 0;
  virtual void Write(RegData data, PrivLevel priv, ContextId ctxt) = 0;
  virtual RegData Read(PrivLevel priv, ContextId ctxt) const = 0;
  virtual std::string GetName(void) const = 0;
  virtual CfSReg GetCfSRegId(void) const = 0;
  virtual ~SRegMPUA_IF() {}
};

/*!
  @brief MPUA_N register interface class
 */
class SRegMPUA_N_IF {
public:
  virtual void SetMPUA(RegData mpua) = 0;
  virtual RegData GetMPUA(void) const = 0;
  virtual void Reset(void) = 0;
  virtual void Write(RegData data, PrivLevel priv, ContextId ctxt) = 0;
  virtual RegData Read(PrivLevel priv, ContextId ctxt) const = 0;
  virtual std::string GetName(void) const = 0;
  virtual CfSReg GetCfSRegId(void) const = 0;
  virtual ~SRegMPUA_N_IF() {}
};

/*!
  @brief PEID register interface class
 */
class SRegHMPEID_IF {
public:
  virtual void SetPEID(RegData peid) = 0;
  virtual RegData GetPEID(void) const = 0;
  virtual void Reset(void) = 0;
  virtual void Write(RegData data, PrivLevel priv, ContextId ctxt) = 0;
  virtual RegData Read(PrivLevel priv, ContextId ctxt) const = 0;
  virtual std::string GetName(void) const = 0;
  virtual CfSReg GetCfSRegId(void) const = 0;
  virtual ~SRegHMPEID_IF() {}
};

/*!
  @brief PID register interface class
 */
class SRegPID_IF {
public:
  virtual void SetPID(RegData pid) = 0;
  virtual void SetFID(RegData fid) = 0;
  virtual void SetVID(RegData vid) = 0;
  virtual RegData GetPID(void) const = 0;
  virtual RegData GetFID(void) const = 0;
  virtual RegData GetVID(void) const = 0;
  virtual void Reset(void) = 0;
  virtual void Write(RegData data, PrivLevel priv, ContextId ctxt) = 0;
  virtual RegData Read(PrivLevel priv, ContextId ctxt) const = 0;
  virtual std::string GetName(void) const = 0;
  virtual CfSReg GetCfSRegId(void) const = 0;
  virtual ~SRegPID_IF() {}
};

/*!
  @brief PLMR register interface class
 */
class SRegHMPLMR_IF {
public:
  virtual void SetPLM(RegData plm) = 0;
  virtual RegData GetPLM(void) const = 0;
  virtual void Reset(void) = 0;
  virtual void Write(RegData data, PrivLevel priv, ContextId ctxt) = 0;
  virtual RegData Read(PrivLevel priv, ContextId ctxt) const = 0;
  virtual std::string GetName(void) const = 0;
  virtual CfSReg GetCfSRegId(void) const = 0;
  virtual ~SRegHMPLMR_IF() {}
};

/*!
  @brief PMCOUNT register interface class
 */
class SRegPMCOUNT_IF {
public:
  virtual void SetPMCNT(RegData pmcnt) = 0;
  virtual RegData GetPMCNT(void) const = 0;
  virtual void Reset(void) = 0;
  virtual void Write(RegData data, PrivLevel priv, ContextId ctxt) = 0;
  virtual RegData Read(PrivLevel priv, ContextId ctxt) const = 0;
  virtual std::string GetName(void) const = 0;
  virtual CfSReg GetCfSRegId(void) const = 0;
  virtual ~SRegPMCOUNT_IF() {}
};

/*!
  @brief PMCTRL register interface class
 */
class SRegPMCTRL_IF {
public:
  virtual void SetHE(bool he) = 0;
  virtual void SetGE(RegData ge) = 0;
  virtual void SetCND(RegData cnd) = 0;
  virtual void SetOVF(bool ovf) = 0;
  virtual void SetCEN(bool cen) = 0;
  virtual bool GetHE(void) const = 0;
  virtual RegData GetGE(void) const = 0;
  virtual RegData GetCND(void) const = 0;
  virtual bool GetOVF(void) const = 0;
  virtual bool GetCEN(void) const = 0;
  virtual void Reset(void) = 0;
  virtual void Write(RegData data, PrivLevel priv, ContextId ctxt) = 0;
  virtual RegData Read(PrivLevel priv, ContextId ctxt) const = 0;
  virtual std::string GetName(void) const = 0;
  virtual CfSReg GetCfSRegId(void) const = 0;
  virtual ~SRegPMCTRL_IF() {}
};

/*!
  @brief PMGMCTRL register interface class
 */
class SRegPMGMCTRL_IF {
public:
  virtual void SetPMGE(RegData pmge) = 0;
  virtual RegData GetPMGE(void) const = 0;
  virtual void Reset(void) = 0;
  virtual void Write(RegData data, PrivLevel priv, ContextId ctxt) = 0;
  virtual RegData Read(PrivLevel priv, ContextId ctxt) const = 0;
  virtual std::string GetName(void) const = 0;
  virtual CfSReg GetCfSRegId(void) const = 0;
  virtual ~SRegPMGMCTRL_IF() {}
};

/*!
  @brief PMSUBCND register interface class
 */
class SRegPMSUBCND_IF {
public:
  virtual void SetSUBCND(RegData subcnd) = 0;
  virtual RegData GetSUBCND(void) const = 0;
  virtual void Reset(void) = 0;
  virtual void Write(RegData data, PrivLevel priv, ContextId ctxt) = 0;
  virtual RegData Read(PrivLevel priv, ContextId ctxt) const = 0;
  virtual std::string GetName(void) const = 0;
  virtual CfSReg GetCfSRegId(void) const = 0;
  virtual ~SRegPMSUBCND_IF() {}
};

/*!
  @brief PMUMCTRL register interface class
 */
class SRegPMUMCTRL_IF {
public:
  virtual void SetPMUE(RegData pmue) = 0;
  virtual RegData GetPMUE(void) const = 0;
  virtual void Reset(void) = 0;
  virtual void Write(RegData data, PrivLevel priv, ContextId ctxt) = 0;
  virtual RegData Read(PrivLevel priv, ContextId ctxt) const = 0;
  virtual std::string GetName(void) const = 0;
  virtual CfSReg GetCfSRegId(void) const = 0;
  virtual ~SRegPMUMCTRL_IF() {}
};

/*!
  @brief PSW register interface class
 */
class SRegHMPSW_IF {
public:
  virtual void SetUM(bool um) = 0;
  virtual void SetEIMASK(RegData eimask) = 0;
  virtual void SetCU(RegData cu) = 0;
  virtual void SetEBV(bool ebv) = 0;
  virtual void SetNP(bool np) = 0;
  virtual void SetEP(bool ep) = 0;
  virtual void SetID(bool id) = 0;
  virtual void SetSAT(bool sat) = 0;
  virtual void SetCY(bool cy) = 0;
  virtual void SetOV(bool ov) = 0;
  virtual void SetS(bool s) = 0;
  virtual void SetZ(bool z) = 0;
  virtual bool GetUM(void) const = 0;
  virtual RegData GetEIMASK(void) const = 0;
  virtual RegData GetCU(void) const = 0;
  virtual bool GetEBV(void) const = 0;
  virtual bool GetNP(void) const = 0;
  virtual bool GetEP(void) const = 0;
  virtual bool GetID(void) const = 0;
  virtual bool GetSAT(void) const = 0;
  virtual bool GetCY(void) const = 0;
  virtual bool GetOV(void) const = 0;
  virtual bool GetS(void) const = 0;
  virtual bool GetZ(void) const = 0;
  virtual void Reset(void) = 0;
  virtual void Write(RegData data, PrivLevel priv, ContextId ctxt) = 0;
  virtual RegData Read(PrivLevel priv, ContextId ctxt) const = 0;
  virtual std::string GetName(void) const = 0;
  virtual CfSReg GetCfSRegId(void) const = 0;
  virtual ~SRegHMPSW_IF() {}
};

/*!
  @brief PSWH register interface class
 */
class SRegPSWH_IF {
public:
  virtual void SetGM(bool gm) = 0;
  virtual void SetGPID(RegData gpid) = 0;
  virtual bool GetGM(void) const = 0;
  virtual RegData GetGPID(void) const = 0;
  virtual void Reset(void) = 0;
  virtual void Write(RegData data, PrivLevel priv, ContextId ctxt) = 0;
  virtual RegData Read(PrivLevel priv, ContextId ctxt) const = 0;
  virtual std::string GetName(void) const = 0;
  virtual CfSReg GetCfSRegId(void) const = 0;
  virtual ~SRegPSWH_IF() {}
};

/*!
  @brief RBASE register interface class
 */
class SRegRBASE_IF {
public:
  virtual void SetRBASE(RegData rbase) = 0;
  virtual void SetDV(bool dv) = 0;
  virtual void SetRINT(bool rint) = 0;
  virtual RegData GetRBASE(void) const = 0;
  virtual bool GetDV(void) const = 0;
  virtual bool GetRINT(void) const = 0;
  virtual void Reset(void) = 0;
  virtual void Write(RegData data, PrivLevel priv, ContextId ctxt) = 0;
  virtual RegData Read(PrivLevel priv, ContextId ctxt) const = 0;
  virtual std::string GetName(void) const = 0;
  virtual CfSReg GetCfSRegId(void) const = 0;
  virtual ~SRegRBASE_IF() {}
};

/*!
  @brief RBCR0 register interface class
 */
class SRegRBCR0_IF {
public:
  virtual void SetMD(bool md) = 0;
  virtual void SetBE(RegData be) = 0;
  virtual bool GetMD(void) const = 0;
  virtual RegData GetBE(void) const = 0;
  virtual void Reset(void) = 0;
  virtual void Write(RegData data, PrivLevel priv, ContextId ctxt) = 0;
  virtual RegData Read(PrivLevel priv, ContextId ctxt) const = 0;
  virtual std::string GetName(void) const = 0;
  virtual CfSReg GetCfSRegId(void) const = 0;
  virtual ~SRegRBCR0_IF() {}
};

/*!
  @brief RBCR1 register interface class
 */
class SRegRBCR1_IF {
public:
  virtual void SetNC(RegData nc) = 0;
  virtual RegData GetNC(void) const = 0;
  virtual void Reset(void) = 0;
  virtual void Write(RegData data, PrivLevel priv, ContextId ctxt) = 0;
  virtual RegData Read(PrivLevel priv, ContextId ctxt) const = 0;
  virtual std::string GetName(void) const = 0;
  virtual CfSReg GetCfSRegId(void) const = 0;
  virtual ~SRegRBCR1_IF() {}
};

/*!
  @brief RBIP register interface class
 */
class SRegRBIP_IF {
public:
  virtual void SetRBIP(RegData rbip) = 0;
  virtual RegData GetRBIP(void) const = 0;
  virtual void Reset(void) = 0;
  virtual void Write(RegData data, PrivLevel priv, ContextId ctxt) = 0;
  virtual RegData Read(PrivLevel priv, ContextId ctxt) const = 0;
  virtual std::string GetName(void) const = 0;
  virtual CfSReg GetCfSRegId(void) const = 0;
  virtual ~SRegRBIP_IF() {}
};

/*!
  @brief RBNR register interface class
 */
class SRegRBNR_IF {
public:
  virtual void SetBN(RegData bn) = 0;
  virtual RegData GetBN(void) const = 0;
  virtual void Reset(void) = 0;
  virtual void Write(RegData data, PrivLevel priv, ContextId ctxt) = 0;
  virtual RegData Read(PrivLevel priv, ContextId ctxt) const = 0;
  virtual std::string GetName(void) const = 0;
  virtual CfSReg GetCfSRegId(void) const = 0;
  virtual ~SRegRBNR_IF() {}
};

/*!
  @brief RDBACR register interface class
 */
class SRegRDBACR_IF {
public:
  virtual void SetENTRY(RegData entry) = 0;
  virtual void SetULSEL(bool ulsel) = 0;
  virtual RegData GetENTRY(void) const = 0;
  virtual bool GetULSEL(void) const = 0;
  virtual void Reset(void) = 0;
  virtual void Write(RegData data, PrivLevel priv, ContextId ctxt) = 0;
  virtual RegData Read(PrivLevel priv, ContextId ctxt) const = 0;
  virtual std::string GetName(void) const = 0;
  virtual CfSReg GetCfSRegId(void) const = 0;
  virtual ~SRegRDBACR_IF() {}
};

/*!
  @brief RDBADAT0 register interface class
 */
class SRegRDBADAT0_IF {
public:
  virtual void SetDATA(RegData data) = 0;
  virtual RegData GetDATA(void) const = 0;
  virtual void Reset(void) = 0;
  virtual void Write(RegData data, PrivLevel priv, ContextId ctxt) = 0;
  virtual RegData Read(PrivLevel priv, ContextId ctxt) const = 0;
  virtual std::string GetName(void) const = 0;
  virtual CfSReg GetCfSRegId(void) const = 0;
  virtual ~SRegRDBADAT0_IF() {}
};

/*!
  @brief RDBADAT1 register interface class
 */
class SRegRDBADAT1_IF {
public:
  virtual void SetDATA(RegData data) = 0;
  virtual RegData GetDATA(void) const = 0;
  virtual void Reset(void) = 0;
  virtual void Write(RegData data, PrivLevel priv, ContextId ctxt) = 0;
  virtual RegData Read(PrivLevel priv, ContextId ctxt) const = 0;
  virtual std::string GetName(void) const = 0;
  virtual CfSReg GetCfSRegId(void) const = 0;
  virtual ~SRegRDBADAT1_IF() {}
};

/*!
  @brief RDBADAT2 register interface class
 */
class SRegRDBADAT2_IF {
public:
  virtual void SetDATA(RegData data) = 0;
  virtual RegData GetDATA(void) const = 0;
  virtual void Reset(void) = 0;
  virtual void Write(RegData data, PrivLevel priv, ContextId ctxt) = 0;
  virtual RegData Read(PrivLevel priv, ContextId ctxt) const = 0;
  virtual std::string GetName(void) const = 0;
  virtual CfSReg GetCfSRegId(void) const = 0;
  virtual ~SRegRDBADAT2_IF() {}
};

/*!
  @brief RDBADAT3 register interface class
 */
class SRegRDBADAT3_IF {
public:
  virtual void SetDATA(RegData data) = 0;
  virtual RegData GetDATA(void) const = 0;
  virtual void Reset(void) = 0;
  virtual void Write(RegData data, PrivLevel priv, ContextId ctxt) = 0;
  virtual RegData Read(PrivLevel priv, ContextId ctxt) const = 0;
  virtual std::string GetName(void) const = 0;
  virtual CfSReg GetCfSRegId(void) const = 0;
  virtual ~SRegRDBADAT3_IF() {}
};

/*!
  @brief RDBATAG register interface class
 */
class SRegRDBATAG_IF {
public:
  virtual void SetPPN(RegData ppn) = 0;
  virtual void SetV(bool v) = 0;
  virtual RegData GetPPN(void) const = 0;
  virtual bool GetV(void) const = 0;
  virtual void Reset(void) = 0;
  virtual void Write(RegData data, PrivLevel priv, ContextId ctxt) = 0;
  virtual RegData Read(PrivLevel priv, ContextId ctxt) const = 0;
  virtual std::string GetName(void) const = 0;
  virtual CfSReg GetCfSRegId(void) const = 0;
  virtual ~SRegRDBATAG_IF() {}
};

/*!
  @brief RDBCR register interface class
 */
class SRegRDBCR_IF {
public:
  virtual void SetRDBDBE(bool rdbdbe) = 0;
  virtual void SetTPEN(bool tpen) = 0;
  virtual void SetAPEN(bool apen) = 0;
  virtual void SetRDBCLR(bool rdbclr) = 0;
  virtual void SetRDBEN(bool rdben) = 0;
  virtual bool GetRDBDBE(void) const = 0;
  virtual bool GetTPEN(void) const = 0;
  virtual bool GetAPEN(void) const = 0;
  virtual bool GetRDBCLR(void) const = 0;
  virtual bool GetRDBEN(void) const = 0;
  virtual void Reset(void) = 0;
  virtual void Write(RegData data, PrivLevel priv, ContextId ctxt) = 0;
  virtual RegData Read(PrivLevel priv, ContextId ctxt) const = 0;
  virtual std::string GetName(void) const = 0;
  virtual CfSReg GetCfSRegId(void) const = 0;
  virtual ~SRegRDBCR_IF() {}
};

/*!
  @brief RDBSTAT register interface class
 */
class SRegRDBSTAT_IF {
public:
  virtual void SetLRU(RegData lru) = 0;
  virtual RegData GetLRU(void) const = 0;
  virtual void Reset(void) = 0;
  virtual void Write(RegData data, PrivLevel priv, ContextId ctxt) = 0;
  virtual RegData Read(PrivLevel priv, ContextId ctxt) const = 0;
  virtual std::string GetName(void) const = 0;
  virtual CfSReg GetCfSRegId(void) const = 0;
  virtual ~SRegRDBSTAT_IF() {}
};

/*!
  @brief SCBP register interface class
 */
class SRegSCBP_IF {
public:
  virtual void SetSCBP(RegData scbp) = 0;
  virtual RegData GetSCBP(void) const = 0;
  virtual void Reset(void) = 0;
  virtual void Write(RegData data, PrivLevel priv, ContextId ctxt) = 0;
  virtual RegData Read(PrivLevel priv, ContextId ctxt) const = 0;
  virtual std::string GetName(void) const = 0;
  virtual CfSReg GetCfSRegId(void) const = 0;
  virtual ~SRegSCBP_IF() {}
};

/*!
  @brief SCCFG register interface class
 */
class SRegSCCFG_IF {
public:
  virtual void SetSIZE(RegData size) = 0;
  virtual RegData GetSIZE(void) const = 0;
  virtual void Reset(void) = 0;
  virtual void Write(RegData data, PrivLevel priv, ContextId ctxt) = 0;
  virtual RegData Read(PrivLevel priv, ContextId ctxt) const = 0;
  virtual std::string GetName(void) const = 0;
  virtual CfSReg GetCfSRegId(void) const = 0;
  virtual ~SRegSCCFG_IF() {}
};

/*!
  @brief SNZCFG register interface class
 */
class SRegSNZCFG_IF {
public:
  virtual void SetSNZCNT(RegData snzcnt) = 0;
  virtual RegData GetSNZCNT(void) const = 0;
  virtual void Reset(void) = 0;
  virtual void Write(RegData data, PrivLevel priv, ContextId ctxt) = 0;
  virtual RegData Read(PrivLevel priv, ContextId ctxt) const = 0;
  virtual std::string GetName(void) const = 0;
  virtual CfSReg GetCfSRegId(void) const = 0;
  virtual ~SRegSNZCFG_IF() {}
};

/*!
  @brief SPID register interface class
 */
class SRegHMSPID_IF {
public:
  virtual void SetSPID(RegData spid) = 0;
  virtual RegData GetSPID(void) const = 0;
  virtual void Reset(void) = 0;
  virtual void Write(RegData data, PrivLevel priv, ContextId ctxt) = 0;
  virtual RegData Read(PrivLevel priv, ContextId ctxt) const = 0;
  virtual std::string GetName(void) const = 0;
  virtual CfSReg GetCfSRegId(void) const = 0;
  virtual ~SRegHMSPID_IF() {}
};

/*!
  @brief SPIDLIST register interface class
 */
class SRegHMSPIDLIST_IF {
public:
  virtual void SetSPIDLIST(RegData spidlist) = 0;
  virtual RegData GetSPIDLIST(void) const = 0;
  virtual void Reset(void) = 0;
  virtual void Write(RegData data, PrivLevel priv, ContextId ctxt) = 0;
  virtual RegData Read(PrivLevel priv, ContextId ctxt) const = 0;
  virtual std::string GetName(void) const = 0;
  virtual CfSReg GetCfSRegId(void) const = 0;
  virtual ~SRegHMSPIDLIST_IF() {}
};

/*!
  @brief SVLOCK register interface class
 */
class SRegHMSVLOCK_IF {
public:
  virtual void SetSVL(bool svl) = 0;
  virtual bool GetSVL(void) const = 0;
  virtual void Reset(void) = 0;
  virtual void Write(RegData data, PrivLevel priv, ContextId ctxt) = 0;
  virtual RegData Read(PrivLevel priv, ContextId ctxt) const = 0;
  virtual std::string GetName(void) const = 0;
  virtual CfSReg GetCfSRegId(void) const = 0;
  virtual ~SRegHMSVLOCK_IF() {}
};

/*!
  @brief TSCOUNTH register interface class
 */
class SRegTSCOUNTH_IF {
public:
  virtual void SetTSCNTH(RegData tscnth) = 0;
  virtual RegData GetTSCNTH(void) const = 0;
  virtual void Reset(void) = 0;
  virtual void Write(RegData data, PrivLevel priv, ContextId ctxt) = 0;
  virtual RegData Read(PrivLevel priv, ContextId ctxt) const = 0;
  virtual std::string GetName(void) const = 0;
  virtual CfSReg GetCfSRegId(void) const = 0;
  virtual ~SRegTSCOUNTH_IF() {}
};

/*!
  @brief TSCOUNTL register interface class
 */
class SRegTSCOUNTL_IF {
public:
  virtual void SetTSCNTL(RegData tscntl) = 0;
  virtual RegData GetTSCNTL(void) const = 0;
  virtual void Reset(void) = 0;
  virtual void Write(RegData data, PrivLevel priv, ContextId ctxt) = 0;
  virtual RegData Read(PrivLevel priv, ContextId ctxt) const = 0;
  virtual std::string GetName(void) const = 0;
  virtual CfSReg GetCfSRegId(void) const = 0;
  virtual ~SRegTSCOUNTL_IF() {}
};

/*!
  @brief TSCTRL register interface class
 */
class SRegTSCTRL_IF {
public:
  virtual void SetOVF(bool ovf) = 0;
  virtual void SetCEN(bool cen) = 0;
  virtual bool GetOVF(void) const = 0;
  virtual bool GetCEN(void) const = 0;
  virtual void Reset(void) = 0;
  virtual void Write(RegData data, PrivLevel priv, ContextId ctxt) = 0;
  virtual RegData Read(PrivLevel priv, ContextId ctxt) const = 0;
  virtual std::string GetName(void) const = 0;
  virtual CfSReg GetCfSRegId(void) const = 0;
  virtual ~SRegTSCTRL_IF() {}
};

/*!
  @brief BMID register class
 */
class SRegBMID : public SRegBasicImpl, SRegBMID_IF {
private:
  RegData m_bmid;

public:
  void SetBMID(RegData bmid) { m_bmid = bmid; }
  RegData GetBMID(void) const { return m_bmid; }

  void Reset(void);

  void Write(RegData data, PrivLevel priv, ContextId ctxt);

  RegData Read(PrivLevel priv, ContextId ctxt) const;

  std::string GetName(void) const { return "BMID"; }

  CfSReg GetCfSRegId(void) const { return CF_SR_BMID; }

  SRegBMID(ContextId ctid, ProcElementBase* pe) :
     SRegBasicImpl (ctid, PRIV_PERMIT_UM, PRIV_PIE_UM, pe),
        m_bmid (0) {}
};

/*!
  @brief BPAM register class
 */
class SRegBPAM : public SRegBasicImpl, SRegBPAM_IF {
private:
  RegData m_bpam;

public:
  void SetBPAM(RegData bpam) { m_bpam = bpam; }
  RegData GetBPAM(void) const { return m_bpam; }

  void Reset(void) {
  }

  void Write(RegData data, PrivLevel priv, ContextId ctxt);

  RegData Read(PrivLevel priv, ContextId ctxt) const;

  std::string GetName(void) const { return "BPAM"; }

  CfSReg GetCfSRegId(void) const { return CF_SR_BPAM; }

  SRegBPAM(ContextId ctid, ProcElementBase* pe) :
     SRegBasicImpl (ctid, PRIV_PERMIT_DBSV, PRIV_PIE_DBSV, pe),
        m_bpam (0) {}
};

/*!
  @brief BPAM_N register class
 */
class SRegBPAM_N : public SRegBasicImpl, SRegBPAM_N_IF {
private:
  RegData m_bpam;
  uint32_t m_id;

public:
  void SetBPAM(RegData bpam) { m_bpam = bpam; }
  RegData GetBPAM(void) const { return m_bpam; }

  void Reset(void) {
  }

  void Write(RegData data, PrivLevel priv, ContextId ctxt) {
        if (IsPrivileged(priv) == false) return;
     SetBPAM ((data >> 0) & RegData (0xffffffffU));
  }

  RegData Read(PrivLevel priv, ContextId ctxt) const;

  std::string GetName(void) const {
     std::ostringstream oss;
     oss << "BPAM" << m_id;
     return oss.str();
  }

  CfSReg GetCfSRegId(void) const { return static_cast<CfSReg>(static_cast<uint32_t>(CF_SR_BPAM0) + m_id); }

  SRegBPAM_N(ContextId ctid, ProcElementBase* pe, uint32_t id) :
     SRegBasicImpl (ctid, PRIV_PERMIT_DBSV, PRIV_PIE_DBSV, pe),
        m_bpam (0),
        m_id (id) {}
};

/*!
  @brief BPAV register class
 */
class SRegBPAV : public SRegBasicImpl, SRegBPAV_IF {
private:
  RegData m_bpav;

public:
  void SetBPAV(RegData bpav) { m_bpav = bpav; }
  RegData GetBPAV(void) const { return m_bpav; }

  void Reset(void) {
  }

  void Write(RegData data, PrivLevel priv, ContextId ctxt);

  RegData Read(PrivLevel priv, ContextId ctxt) const;

  std::string GetName(void) const { return "BPAV"; }

  CfSReg GetCfSRegId(void) const { return CF_SR_BPAV; }

  SRegBPAV(ContextId ctid, ProcElementBase* pe) :
     SRegBasicImpl (ctid, PRIV_PERMIT_DBSV, PRIV_PIE_DBSV, pe),
        m_bpav (0) {}
};

/*!
  @brief BPAV_N register class
 */
class SRegBPAV_N : public SRegBasicImpl, SRegBPAV_N_IF {
private:
  RegData m_bpav;
  uint32_t m_id;

public:
  void SetBPAV(RegData bpav) { m_bpav = bpav; }
  RegData GetBPAV(void) const { return m_bpav; }

  void Reset(void) {
  }

  void Write(RegData data, PrivLevel priv, ContextId ctxt) {
        if (IsPrivileged(priv) == false) return;
     SetBPAV ((data >> 0) & RegData (0xffffffffU));
  }

  RegData Read(PrivLevel priv, ContextId ctxt) const;

  std::string GetName(void) const {
     std::ostringstream oss;
     oss << "BPAV" << m_id;
     return oss.str();
  }

  CfSReg GetCfSRegId(void) const { return static_cast<CfSReg>(static_cast<uint32_t>(CF_SR_BPAV0) + m_id); }

  SRegBPAV_N(ContextId ctid, ProcElementBase* pe, uint32_t id) :
     SRegBasicImpl (ctid, PRIV_PERMIT_DBSV, PRIV_PIE_DBSV, pe),
        m_bpav (0),
        m_id (id) {}
};

/*!
  @brief BPC register class
 */
class SRegBPC : public SRegBasicImpl, SRegBPC_IF {
private:
  RegData m_bpc;

public:
  void SetBPC(RegData bpc) { m_bpc = bpc; }
  RegData GetBPC(void) const { return m_bpc; }

  void Reset(void) {
  }

  void Write(RegData data, PrivLevel priv, ContextId ctxt);

  RegData Read(PrivLevel priv, ContextId ctxt) const;

  std::string GetName(void) const { return "BPC"; }

  CfSReg GetCfSRegId(void) const { return CF_SR_BPC; }

  SRegBPC(ContextId ctid, ProcElementBase* pe) :
     SRegBasicImpl (ctid, PRIV_PERMIT_DBSV, PRIV_PIE_DBSV, pe),
        m_bpc (0) {}
};

/*!
  @brief BPC_N register class
 */
class SRegBPC_N : public SRegBasicImpl, SRegBPC_N_IF {
private:
  bool    m_he;
  RegData m_ge;
  RegData m_ty;
  bool    m_va;
  bool    m_eo;
  bool    m_te;
  bool    m_be;
  bool    m_fe;
  bool    m_we;
  bool    m_re;
  uint32_t m_id;

public:
  void SetHE(bool he) { m_he = he; }
  void SetGE(RegData ge) { m_ge = ge; }
  void SetTY(RegData ty);
  void SetVA(bool va) { m_va = va; }
  void SetEO(bool eo) { m_eo = eo; }
  void SetTE(bool te) { m_te = te; }
  void SetBE(bool be) { m_be = be; }
  void SetFE(bool fe) { m_fe = fe; }
  void SetWE(bool we);
  void SetRE(bool re);
  bool GetHE(void) const { return m_he; }
  RegData GetGE(void) const { return m_ge; }
  RegData GetTY(void) const { return m_ty; }
  bool GetVA(void) const { return m_va; }
  bool GetEO(void) const { return m_eo; }
  bool GetTE(void) const { return m_te; }
  bool GetBE(void) const { return m_be; }
  bool GetFE(void) const { return m_fe; }
  bool GetWE(void) const { return m_we; }
  bool GetRE(void) const { return m_re; }

  void Reset(void) {
     m_he = 0x0U;
     m_ge = 0x0U;
     m_ty = 0x0U;
     m_va = 0x0U;
     m_eo = 0x0U;
     m_te = 0x0U;
     m_be = 0x0U;
     m_fe = 0x0U;
     m_we = 0x0U;
     m_re = 0x0U;
  }

  void Write(RegData data, PrivLevel priv, ContextId ctxt);

  RegData Read(PrivLevel priv, ContextId ctxt) const;

  std::string GetName(void) const {
     std::ostringstream oss;
     oss << "BPC" << m_id;
     return oss.str();
  }

  CfSReg GetCfSRegId(void) const { return static_cast<CfSReg>(static_cast<uint32_t>(CF_SR_BPC0) + m_id); }

  SRegBPC_N(ContextId ctid, ProcElementBase* pe, uint32_t id) :
     SRegBasicImpl (ctid, PRIV_PERMIT_DBSV, PRIV_PIE_DBSV, pe),
        m_he (0),
        m_ge (0),
        m_ty (0),
        m_va (0),
        m_eo (0),
        m_te (0),
        m_be (0),
        m_fe (0),
        m_we (0),
        m_re (0),
        m_id (id) {}
};

/*!
  @brief BRPACTRL register class
 */
class SRegBRPACTRL : public SRegBasicImpl, SRegBRPACTRL_IF {
private:
  bool    m_sr;
  bool    m_rw;
  RegData m_sel;
  RegData m_entry;

public:
  void SetSR(bool sr);
  void SetRW(bool rw) { m_rw = rw; }
  void SetSEL(RegData sel) { m_sel = sel; }
  void SetENTRY(RegData entry) { m_entry = entry; }
  bool GetSR(void) const { return m_sr; }
  bool GetRW(void) const { return m_rw; }
  RegData GetSEL(void) const { return m_sel; }
  RegData GetENTRY(void) const { return m_entry; }

  void Reset(void) {
     m_sr = 0x0U;
     m_rw = 0x0U;
     m_sel = 0x0U;
     m_entry = 0x0U;
  }

  void Write(RegData data, PrivLevel priv, ContextId ctxt);

  RegData Read(PrivLevel priv, ContextId ctxt) const;

  std::string GetName(void) const { return "BRPACTRL"; }

  CfSReg GetCfSRegId(void) const { return CF_SR_BRPACTRL; }

  SRegBRPACTRL(ContextId ctid, ProcElementBase* pe) :
     SRegBasicImpl (ctid, PRIV_PERMIT_SV, PRIV_PIE_SV, pe),
        m_sr (0),
        m_rw (0),
        m_sel (0),
        m_entry (0) {}
};

/*!
  @brief BRPCFG register class
 */
class SRegBRPCFG : public SRegBasicImpl, SRegBRPCFG_IF {
private:
  RegData m_rassize;
  RegData m_btacsize;
  RegData m_gbhtsize;
  RegData m_bhtsize;

public:
  void SetRASSIZE(RegData rassize) { m_rassize = rassize; }
  void SetBTACSIZE(RegData btacsize) { m_btacsize = btacsize; }
  void SetGBHTSIZE(RegData gbhtsize) { m_gbhtsize = gbhtsize; }
  void SetBHTSIZE(RegData bhtsize) { m_bhtsize = bhtsize; }
  RegData GetRASSIZE(void) const { return m_rassize; }
  RegData GetBTACSIZE(void) const { return m_btacsize; }
  RegData GetGBHTSIZE(void) const { return m_gbhtsize; }
  RegData GetBHTSIZE(void) const { return m_bhtsize; }

  void Reset(void);

  void Write(RegData data, PrivLevel priv, ContextId ctxt);

  RegData Read(PrivLevel priv, ContextId ctxt) const;

  std::string GetName(void) const { return "BRPCFG"; }

  CfSReg GetCfSRegId(void) const { return CF_SR_BRPCFG; }

  SRegBRPCFG(ContextId ctid, ProcElementBase* pe) :
     SRegBasicImpl (ctid, PRIV_PERMIT_SV, PRIV_PIE_SV, pe),
        m_rassize (0),
        m_btacsize (0),
        m_gbhtsize (0),
        m_bhtsize (0) {}
};

/*!
  @brief BRPCTRL0 register class
 */
class SRegBRPCTRL0 : public SRegBasicImpl, SRegBRPCTRL0_IF {
private:
  bool    m_brpclr;
  bool    m_brpen;

public:
  void SetBRPCLR(bool brpclr) { m_brpclr = brpclr; }
  void SetBRPEN(bool brpen) { m_brpen = brpen; }
  bool GetBRPCLR(void) const { return m_brpclr; }
  bool GetBRPEN(void) const { return m_brpen; }

  void Reset(void) {
     m_brpclr = 0x0U;
     m_brpen = 0x1U;
  }

  void Write(RegData data, PrivLevel priv, ContextId ctxt);

  RegData Read(PrivLevel priv, ContextId ctxt) const;

  std::string GetName(void) const { return "BRPCTRL0"; }

  CfSReg GetCfSRegId(void) const { return CF_SR_BRPCTRL0; }

  SRegBRPCTRL0(ContextId ctid, ProcElementBase* pe) :
     SRegBasicImpl (ctid, PRIV_PERMIT_SV, PRIV_PIE_SV, pe),
        m_brpclr (0),
        m_brpen (0) {}
};

/*!
  @brief BRPCTRL1 register class
 */
class SRegBRPCTRL1 : public SRegBasicImpl, SRegBRPCTRL1_IF {
private:
  bool    m_bhtst;
  bool    m_rasd;
  bool    m_btacd;
  bool    m_gbhtd;
  bool    m_bhtd;

public:
  void SetBHTST(bool bhtst) { m_bhtst = bhtst; }
  void SetRASD(bool rasd) { m_rasd = rasd; }
  void SetBTACD(bool btacd) { m_btacd = btacd; }
  void SetGBHTD(bool gbhtd) { m_gbhtd = gbhtd; }
  void SetBHTD(bool bhtd) { m_bhtd = bhtd; }
  bool GetBHTST(void) const { return m_bhtst; }
  bool GetRASD(void) const { return m_rasd; }
  bool GetBTACD(void) const { return m_btacd; }
  bool GetGBHTD(void) const { return m_gbhtd; }
  bool GetBHTD(void) const { return m_bhtd; }

  void Reset(void) {
     m_bhtst = 0x0U;
     m_rasd = 0x0U;
     m_btacd = 0x0U;
     m_gbhtd = 0x0U;
     m_bhtd = 0x0U;
  }

  void Write(RegData data, PrivLevel priv, ContextId ctxt);

  RegData Read(PrivLevel priv, ContextId ctxt) const;

  std::string GetName(void) const { return "BRPCTRL1"; }

  CfSReg GetCfSRegId(void) const { return CF_SR_BRPCTRL1; }

  SRegBRPCTRL1(ContextId ctid, ProcElementBase* pe) :
     SRegBasicImpl (ctid, PRIV_PERMIT_SV, PRIV_PIE_SV, pe),
        m_bhtst (0),
        m_rasd (0),
        m_btacd (0),
        m_gbhtd (0),
        m_bhtd (0) {}
};

/*!
  @brief BRPDATA register class
 */
class SRegBRPDATA : public SRegBasicImpl, SRegBRPDATA_IF {
private:
  RegData m_data;

public:
  void SetDATA(RegData data) { m_data = data; }
  RegData GetDATA(void) const { return m_data; }

  void Reset(void) {
     m_data = 0x0U;
  }

  void Write(RegData data, PrivLevel priv, ContextId ctxt);

  RegData Read(PrivLevel priv, ContextId ctxt) const;

  std::string GetName(void) const { return "BRPDATA"; }

  CfSReg GetCfSRegId(void) const { return CF_SR_BRPDATA; }

  SRegBRPDATA(ContextId ctid, ProcElementBase* pe) :
     SRegBasicImpl (ctid, PRIV_PERMIT_SV, PRIV_PIE_SV, pe),
        m_data (0) {}
};

/*!
  @brief CTBP register class
 */
class SRegCTBP : public SRegBasicImpl, SRegCTBP_IF {
private:
  RegData m_ctbp;

public:
  void SetCTBP(RegData ctbp) { m_ctbp = ctbp; }
  RegData GetCTBP(void) const { return m_ctbp; }

  void Reset(void) {
     m_ctbp = 0x0U;
  }

  void Write(RegData data, PrivLevel priv, ContextId ctxt) {
        if (IsPrivileged(priv) == false) return;
     SetCTBP ((data >> 1) & RegData (0x7fffffffU));
  }

  RegData Read(PrivLevel priv, ContextId ctxt) const {
        if (IsPrivileged (priv) == false) return 0;
     RegData ret = 0x0
            | (GetCTBP () << 1);
     return ret;
  }

  std::string GetName(void) const { return "CTBP"; }

  CfSReg GetCfSRegId(void) const { return CF_SR_CTBP; }

  SRegCTBP(ContextId ctid, ProcElementBase* pe) :
     SRegBasicImpl (ctid, PRIV_PERMIT_UM, PRIV_PIE_UM, pe),
        m_ctbp (0) {}
};

/*!
  @brief CTPC register class
 */
class SRegCTPC : public SRegBasicImpl, SRegCTPC_IF {
private:
  RegData m_ctpc;

public:
  void SetCTPC(RegData ctpc) { m_ctpc = ctpc; }
  RegData GetCTPC(void) const { return m_ctpc; }

  void Reset(void) {
  }

  void Write(RegData data, PrivLevel priv, ContextId ctxt) {
        if (IsPrivileged(priv) == false) return;
     SetCTPC ((data >> 0) & RegData (0xffffffffU));
  }

  RegData Read(PrivLevel priv, ContextId ctxt) const {
        if (IsPrivileged (priv) == false) return 0;
     RegData ret = 0x0
            | (GetCTPC () << 0);
     return ret;
  }

  std::string GetName(void) const { return "CTPC"; }

  CfSReg GetCfSRegId(void) const { return CF_SR_CTPC; }

  SRegCTPC(ContextId ctid, ProcElementBase* pe) :
     SRegBasicImpl (ctid, PRIV_PERMIT_UM, PRIV_PIE_UM, pe),
        m_ctpc (0) {}
};

/*!
  @brief CTPSW register class
 */
class SRegCTPSW : public SRegBasicImpl, SRegCTPSW_IF {
private:
  RegData m_ctpsw;

public:
  void SetCTPSW(RegData ctpsw) { m_ctpsw = ctpsw; }
  RegData GetCTPSW(void) const { return m_ctpsw; }

  void Reset(void) {
     m_ctpsw = 0x0U;
  }

  void Write(RegData data, PrivLevel priv, ContextId ctxt) {
        if (IsPrivileged(priv) == false) return;
     SetCTPSW ((data >> 0) & RegData (0x0000001fU));
  }

  RegData Read(PrivLevel priv, ContextId ctxt) const {
        if (IsPrivileged (priv) == false) return 0;
     RegData ret = 0x0
            | (GetCTPSW () << 0);
     return ret;
  }

  std::string GetName(void) const { return "CTPSW"; }

  CfSReg GetCfSRegId(void) const { return CF_SR_CTPSW; }

  SRegCTPSW(ContextId ctid, ProcElementBase* pe) :
     SRegBasicImpl (ctid, PRIV_PERMIT_UM, PRIV_PIE_UM, pe),
        m_ctpsw (0) {}
};

/*!
  @brief DBCFG register class
 */
class SRegDBCFG : public SRegBasicImpl, SRegDBCFG_IF {
private:
  RegData m_nbp;

public:
  void SetNBP(RegData nbp) { m_nbp = nbp; }
  RegData GetNBP(void) const;

  void Reset(void) {
  }

  void Write(RegData data, PrivLevel priv, ContextId ctxt) {
        if (IsPrivileged(priv) == false) return;
  }

  RegData Read(PrivLevel priv, ContextId ctxt) const;

  std::string GetName(void) const { return "DBCFG"; }

  CfSReg GetCfSRegId(void) const { return CF_SR_DBCFG; }

  SRegDBCFG(ContextId ctid, ProcElementBase* pe) :
     SRegBasicImpl (ctid, PRIV_PERMIT_DBSV, PRIV_PIE_DBSV, pe),
        m_nbp (0) {}
};

/*!
  @brief DBGEN register class
 */
class SRegDBGEN : public SRegBasicImpl, SRegDBGEN_IF {
private:
  bool    m_he;
  RegData m_ge;

public:
  void SetHE(bool he) { m_he = he; }
  void SetGE(RegData ge) { m_ge = ge; }
  bool GetHE(void) const { return m_he; }
  RegData GetGE(void) const { return m_ge; }

  void Reset(void);

  void Write(RegData data, PrivLevel priv, ContextId ctxt) {
        if (m_pe->IsHvEnabled() == false) return;
     SetHE ((data >> 8) & RegData (0x00000001U));
     SetGE ((data >> 0) & RegData (0x000000ffU));
  }

  RegData Read(PrivLevel priv, ContextId ctxt) const {
        if (m_pe->IsHvEnabled () == false) return 0;
     RegData ret = 0x0
            | (GetHE () << 8)
            | (GetGE () << 0);
     return ret;
  }

  std::string GetName(void) const { return "DBGEN"; }

  CfSReg GetCfSRegId(void) const { return CF_SR_DBGEN; }

  SRegDBGEN(ContextId ctid, ProcElementBase* pe) :
     SRegBasicImpl (ctid, PRIV_PERMIT_HV, PRIV_PIE_HV, pe),
        m_he (0),
        m_ge (0) {}
};

/*!
  @brief DBIC register class
 */
class SRegDBIC : public SRegBasicImpl, SRegDBIC_IF {
private:
  RegData m_dbic;

public:
  void SetDBIC(RegData dbic) { m_dbic = dbic; }
  RegData GetDBIC(void) const { return m_dbic; }

  void Reset(void) {
     m_dbic = 0x0U;
  }

  void Write(RegData data, PrivLevel priv, ContextId ctxt);

  RegData Read(PrivLevel priv, ContextId ctxt) const;

  std::string GetName(void) const { return "DBIC"; }

  CfSReg GetCfSRegId(void) const { return CF_SR_DBIC; }

  SRegDBIC(ContextId ctid, ProcElementBase* pe) :
     SRegBasicImpl (ctid, PRIV_PERMIT_DBSV, PRIV_PIE_DBSV, pe),
        m_dbic (0) {}
};

/*!
  @brief DBPC register class
 */
class SRegDBPC : public SRegBasicImpl, SRegDBPC_IF {
private:
  RegData m_dbpc;

public:
  void SetDBPC(RegData dbpc) { m_dbpc = dbpc; }
  RegData GetDBPC(void) const { return m_dbpc; }

  void Reset(void) {
  }

  void Write(RegData data, PrivLevel priv, ContextId ctxt);

  RegData Read(PrivLevel priv, ContextId ctxt) const;

  std::string GetName(void) const { return "DBPC"; }

  CfSReg GetCfSRegId(void) const { return CF_SR_DBPC; }

  SRegDBPC(ContextId ctid, ProcElementBase* pe) :
     SRegBasicImpl (ctid, PRIV_PERMIT_DBSV, PRIV_PIE_DBSV, pe),
        m_dbpc (0) {}
};

/*!
  @brief DBPSW register class
 */
class SRegDBPSW : public SRegBasicImpl, SRegDBPSW_IF {
private:
  bool    m_um;
  RegData m_eimask;
  RegData m_cu;
  bool    m_ebv;
  bool    m_np;
  bool    m_ep;
  bool    m_id;
  bool    m_sat;
  bool    m_cy;
  bool    m_ov;
  bool    m_s;
  bool    m_z;

public:
  void SetUM(bool um) { m_um = um; }
  void SetEIMASK(RegData eimask);
  void SetCU(RegData cu);
  void SetEBV(bool ebv) { m_ebv = ebv; }
  void SetNP(bool np) { m_np = np; }
  void SetEP(bool ep) { m_ep = ep; }
  void SetID(bool id) { m_id = id; }
  void SetSAT(bool sat) { m_sat = sat; }
  void SetCY(bool cy) { m_cy = cy; }
  void SetOV(bool ov) { m_ov = ov; }
  void SetS(bool s) { m_s = s; }
  void SetZ(bool z) { m_z = z; }
  bool GetUM(void) const { return m_um; }
  RegData GetEIMASK(void) const { return m_eimask; }
  RegData GetCU(void) const { return m_cu; }
  bool GetEBV(void) const { return m_ebv; }
  bool GetNP(void) const { return m_np; }
  bool GetEP(void) const { return m_ep; }
  bool GetID(void) const { return m_id; }
  bool GetSAT(void) const { return m_sat; }
  bool GetCY(void) const { return m_cy; }
  bool GetOV(void) const { return m_ov; }
  bool GetS(void) const { return m_s; }
  bool GetZ(void) const { return m_z; }

  void Reset(void) {
     m_um = 0x0U;
     m_eimask = 0x0U;
     m_cu = 0x0U;
     m_ebv = 0x0U;
     m_np = 0x0U;
     m_ep = 0x0U;
     m_id = 0x1U;
     m_sat = 0x0U;
     m_cy = 0x0U;
     m_ov = 0x0U;
     m_s = 0x0U;
     m_z = 0x0U;
  }

  void Write(RegData data, PrivLevel priv, ContextId ctxt);

  RegData Read(PrivLevel priv, ContextId ctxt) const;

  std::string GetName(void) const { return "DBPSW"; }

  CfSReg GetCfSRegId(void) const { return CF_SR_DBPSW; }

  SRegDBPSW(ContextId ctid, ProcElementBase* pe) :
     SRegBasicImpl (ctid, PRIV_PERMIT_DBSV, PRIV_PIE_DBSV, pe),
        m_um (0),
        m_eimask (0),
        m_cu (0),
        m_ebv (0),
        m_np (0),
        m_ep (0),
        m_id (0),
        m_sat (0),
        m_cy (0),
        m_ov (0),
        m_s (0),
        m_z (0) {}
};

/*!
  @brief DBPSWH register class
 */
class SRegDBPSWH : public SRegBasicImpl, SRegDBPSWH_IF {
private:
  bool    m_gm;
  RegData m_gpid;

public:
  void SetGM(bool gm) { m_gm = gm; }
  void SetGPID(RegData gpid) { m_gpid = gpid; }
  bool GetGM(void) const { return m_gm; }
  RegData GetGPID(void) const { return m_gpid; }

  void Reset(void) {
     m_gm = 0x0U;
     m_gpid = 0x0U;
  }

  void Write(RegData data, PrivLevel priv, ContextId ctxt);

  RegData Read(PrivLevel priv, ContextId ctxt) const;

  std::string GetName(void) const { return "DBPSWH"; }

  CfSReg GetCfSRegId(void) const { return CF_SR_DBPSWH; }

  SRegDBPSWH(ContextId ctid, ProcElementBase* pe) :
     SRegBasicImpl (ctid, PRIV_PERMIT_DBHV, PRIV_PIE_DBHV, pe),
        m_gm (0),
        m_gpid (0) {}
};

/*!
  @brief DBWR register class
 */
class SRegDBWR : public SRegBasicImpl, SRegDBWR_IF {
private:
  RegData m_dbwr;

public:
  void SetDBWR(RegData dbwr) { m_dbwr = dbwr; }
  RegData GetDBWR(void) const { return m_dbwr; }

  void Reset(void) {
  }

  void Write(RegData data, PrivLevel priv, ContextId ctxt);

  RegData Read(PrivLevel priv, ContextId ctxt) const;

  std::string GetName(void) const { return "DBWR"; }

  CfSReg GetCfSRegId(void) const { return CF_SR_DBWR; }

  SRegDBWR(ContextId ctid, ProcElementBase* pe) :
     SRegBasicImpl (ctid, PRIV_PERMIT_DBSV, PRIV_PIE_DBSV, pe),
        m_dbwr (0) {}
};

/*!
  @brief DCBKEY register class
 */
class SRegDCBKEY : public SRegBasicImpl, SRegDCBKEY_IF {
private:
  RegData m_cbakey;
  RegData m_keycode;

public:
  void SetCBAKEY(RegData cbakey) { m_cbakey = cbakey; }
  void SetKEYCODE(RegData keycode) { m_keycode = keycode; }
  RegData GetCBAKEY(void) const { return m_cbakey; }
  RegData GetKEYCODE(void) const { return m_keycode; }

  void Reset(void) {
     m_cbakey = 0x0U;
     m_keycode = 0x0U;
  }

  void Write(RegData data, PrivLevel priv, ContextId ctxt);

  RegData Read(PrivLevel priv, ContextId ctxt) const;

  std::string GetName(void) const { return "DCBKEY"; }

  CfSReg GetCfSRegId(void) const { return CF_SR_DCBKEY; }

  SRegDCBKEY(ContextId ctid, ProcElementBase* pe) :
     SRegBasicImpl (ctid, PRIV_PERMIT_SV, PRIV_PIE_SV, pe),
        m_cbakey (0),
        m_keycode (0) {}
};

/*!
  @brief DECFG register class
 */
class SRegDECFG : public SRegBasicImpl, SRegDECFG_IF {
private:
  bool    m_ehe;
  bool    m_ege;
  bool    m_ese;

public:
  void SetEHE(bool ehe) { m_ehe = ehe; }
  void SetEGE(bool ege) { m_ege = ege; }
  void SetESE(bool ese) { m_ese = ese; }
  bool GetEHE(void) const { return m_ehe; }
  bool GetEGE(void) const { return m_ege; }
  bool GetESE(void) const { return m_ese; }

  void Reset(void) {
     m_ehe = 0x0U;
     m_ege = 0x0U;
     m_ese = 0x0U;
  }

  void Write(RegData data, PrivLevel priv, ContextId ctxt);

  RegData Read(PrivLevel priv, ContextId ctxt) const;

  std::string GetName(void) const { return "DECFG"; }

  CfSReg GetCfSRegId(void) const { return CF_SR_DECFG; }

  SRegDECFG(ContextId ctid, ProcElementBase* pe) :
     SRegBasicImpl (ctid, PRIV_PERMIT_HV, PRIV_PIE_HV, pe),
        m_ehe (0),
        m_ege (0),
        m_ese (0) {}
};

/*!
  @brief DECTRL register class
 */
class SRegDECTRL : public SRegBasicImpl, SRegDECTRL_IF {
private:
  bool    m_bevd;

public:
  void SetBEVD(bool bevd);
  bool GetBEVD(void) const;

  void Reset(void) {
     m_bevd = 0x0U;
  }

  void Write(RegData data, PrivLevel priv, ContextId ctxt);

  RegData Read(PrivLevel priv, ContextId ctxt) const;

  std::string GetName(void) const { return "DECTRL"; }

  CfSReg GetCfSRegId(void) const { return CF_SR_DECTRL; }

  SRegDECTRL(ContextId ctid, ProcElementBase* pe) :
     SRegBasicImpl (ctid, PRIV_PERMIT_SV, PRIV_PIE_SV, pe),
        m_bevd (0) {}
};

/*!
  @brief DEVDS register class
 */
class SRegDEVDS : public SRegBasicImpl, SRegDEVDS_IF {
private:
  bool    m_bevdhm;
  bool    m_bevdgm7;
  bool    m_bevdgm6;
  bool    m_bevdgm5;
  bool    m_bevdgm4;
  bool    m_bevdgm3;
  bool    m_bevdgm2;
  bool    m_bevdgm1;
  bool    m_bevdgm0;

public:
  void SetBEVDHM(bool bevdhm);
  void SetBEVDGM7(bool bevdgm7);
  void SetBEVDGM6(bool bevdgm6);
  void SetBEVDGM5(bool bevdgm5);
  void SetBEVDGM4(bool bevdgm4);
  void SetBEVDGM3(bool bevdgm3);
  void SetBEVDGM2(bool bevdgm2);
  void SetBEVDGM1(bool bevdgm1);
  void SetBEVDGM0(bool bevdgm0);
  bool GetBEVDHM(void) const;
  bool GetBEVDGM7(void) const;
  bool GetBEVDGM6(void) const;
  bool GetBEVDGM5(void) const;
  bool GetBEVDGM4(void) const;
  bool GetBEVDGM3(void) const;
  bool GetBEVDGM2(void) const;
  bool GetBEVDGM1(void) const;
  bool GetBEVDGM0(void) const;

  void Reset(void) {
     m_bevdhm = 0x0U;
     m_bevdgm7 = 0x0U;
     m_bevdgm6 = 0x0U;
     m_bevdgm5 = 0x0U;
     m_bevdgm4 = 0x0U;
     m_bevdgm3 = 0x0U;
     m_bevdgm2 = 0x0U;
     m_bevdgm1 = 0x0U;
     m_bevdgm0 = 0x0U;
  }

  void Write(RegData data, PrivLevel priv, ContextId ctxt);

  RegData Read(PrivLevel priv, ContextId ctxt) const;

  std::string GetName(void) const { return "DEVDS"; }

  CfSReg GetCfSRegId(void) const { return CF_SR_DEVDS; }

  SRegDEVDS(ContextId ctid, ProcElementBase* pe) :
     SRegBasicImpl (ctid, PRIV_PERMIT_HV, PRIV_PIE_HV, pe),
        m_bevdhm (0),
        m_bevdgm7 (0),
        m_bevdgm6 (0),
        m_bevdgm5 (0),
        m_bevdgm4 (0),
        m_bevdgm3 (0),
        m_bevdgm2 (0),
        m_bevdgm1 (0),
        m_bevdgm0 (0) {}
};

/*!
  @brief DIR0 register class
 */
class SRegDIR0 : public SRegBasicImpl, SRegDIR0_IF {
private:
  bool    m_aee;
  bool    m_at;
  bool    m_dn;
  bool    m_dm;

public:
  void SetAEE(bool aee) { m_aee = aee; }
  void SetAT(bool at) { m_at = at; }
  void SetDN(bool dn);
  void SetDM(bool dm);
  bool GetAEE(void) const { return m_aee; }
  bool GetAT(void) const { return m_at; }
  bool GetDN(void) const { return m_dn; }
  bool GetDM(void) const { return m_dm; }

  void Reset(void) {
     m_aee = 0x0U;
     m_at = 0x0U;
     m_dn = 0x0U;
     m_dm = 0x0U;
  }

  void Write(RegData data, PrivLevel priv, ContextId ctxt);

  RegData Read(PrivLevel priv, ContextId ctxt) const;

  std::string GetName(void) const { return "DIR0"; }

  CfSReg GetCfSRegId(void) const { return CF_SR_DIR0; }

  SRegDIR0(ContextId ctid, ProcElementBase* pe) :
     SRegBasicImpl (ctid, PRIV_PERMIT_DBSV, PRIV_PIE_DBSV, pe),
        m_aee (0),
        m_at (0),
        m_dn (0),
        m_dm (0) {}
};

/*!
  @brief DIR1 register class
 */
class SRegDIR1 : public SRegBasicImpl, SRegDIR1_IF {
private:
  RegData m_bt;
  RegData m_csl;
  bool    m_sq1;
  bool    m_sq0;
  bool    m_ben;

public:
  void SetBT(RegData bt) { m_bt = bt; }
  void SetCSL(RegData csl) { m_csl = csl; }
  void SetSQ1(bool sq1) { m_sq1 = sq1; }
  void SetSQ0(bool sq0) { m_sq0 = sq0; }
  void SetBEN(bool ben) { m_ben = ben; }
  RegData GetBT(void) const { return m_bt; }
  RegData GetCSL(void) const { return m_csl; }
  bool GetSQ1(void) const { return m_sq1; }
  bool GetSQ0(void) const { return m_sq0; }
  bool GetBEN(void) const { return m_ben; }

  void Reset(void) {
     m_bt = 0x0U;
     m_csl = 0x0U;
     m_sq1 = 0x0U;
     m_sq0 = 0x0U;
     m_ben = 0x0U;
  }

  void Write(RegData data, PrivLevel priv, ContextId ctxt);

  RegData Read(PrivLevel priv, ContextId ctxt) const;

  std::string GetName(void) const { return "DIR1"; }

  CfSReg GetCfSRegId(void) const { return CF_SR_DIR1; }

  SRegDIR1(ContextId ctid, ProcElementBase* pe) :
     SRegBasicImpl (ctid, PRIV_PERMIT_DBSV, PRIV_PIE_DBSV, pe),
        m_bt (0),
        m_csl (0),
        m_sq1 (0),
        m_sq0 (0),
        m_ben (0) {}
};

/*!
  @brief EBASE register class
 */
class SRegHMEBASE : public SRegBaseEBASE, SRegHMEBASE_IF {
private:
  RegData m_ebase;
  bool    m_dv;
  bool    m_rint;

public:
  void SetEBASE(RegData ebase) { m_ebase = ebase; }
  void SetDV(bool dv) { m_dv = dv; }
  void SetRINT(bool rint) { m_rint = rint; }
  RegData GetEBASE(void) const { return m_ebase; }
  bool GetDV(void) const { return m_dv; }
  bool GetRINT(void) const { return m_rint; }

  void Reset(void) {
  }

  void Write(RegData data, PrivLevel priv, ContextId ctxt) {
        if (IsPrivileged(priv) == false) return;
     SetEBASE ((data >> 9) & RegData (0x007fffffU));
     SetDV ((data >> 1) & RegData (0x00000001U));
     SetRINT ((data >> 0) & RegData (0x00000001U));
  }

  RegData Read(PrivLevel priv, ContextId ctxt) const {
        if (IsPrivileged (priv) == false) return 0;
     RegData ret = 0x0
            | (GetEBASE () << 9)
            | (GetDV () << 1)
            | (GetRINT () << 0);
     return ret;
  }

  std::string GetName(void) const { return "EBASE"; }

  CfSReg GetCfSRegId(void) const { return CF_SR_HMEBASE; }

  SRegHMEBASE(ContextId ctid, ProcElementBase* pe) :
     SRegBaseEBASE (ctid, PRIV_PERMIT_SV, PRIV_PIE_SV, pe),
        m_ebase (0),
        m_dv (0),
        m_rint (0) {}
};

/*!
  @brief EIIC register class
 */
class SRegHMEIIC : public SRegBaseEIIC, SRegHMEIIC_IF {
private:
  RegData m_eiic;

public:
  void SetEIIC(RegData eiic) { m_eiic = eiic; }
  RegData GetEIIC(void) const { return m_eiic; }

  void Reset(void) {
     m_eiic = 0x0U;
  }

  void Write(RegData data, PrivLevel priv, ContextId ctxt) {
        if (IsPrivileged(priv) == false) return;
     SetEIIC ((data >> 0) & RegData (0xffffffffU));
  }

  RegData Read(PrivLevel priv, ContextId ctxt) const {
        if (IsPrivileged (priv) == false) return 0;
     RegData ret = 0x0
            | (GetEIIC () << 0);
     return ret;
  }

  std::string GetName(void) const { return "EIIC"; }

  CfSReg GetCfSRegId(void) const { return CF_SR_HMEIIC; }

  SRegHMEIIC(ContextId ctid, ProcElementBase* pe) :
     SRegBaseEIIC (ctid, PRIV_PERMIT_SV, PRIV_PIE_SV, pe),
        m_eiic (0) {}
};

/*!
  @brief EIPC register class
 */
class SRegHMEIPC : public SRegBaseEIPC, SRegHMEIPC_IF {
private:
  RegData m_eipc;

public:
  void SetEIPC(RegData eipc) { m_eipc = eipc; }
  RegData GetEIPC(void) const { return m_eipc; }

  void Reset(void) {
  }

  void Write(RegData data, PrivLevel priv, ContextId ctxt) {
        if (IsPrivileged(priv) == false) return;
     SetEIPC ((data >> 0) & RegData (0xffffffffU));
  }

  RegData Read(PrivLevel priv, ContextId ctxt) const {
        if (IsPrivileged (priv) == false) return 0;
     RegData ret = 0x0
            | (GetEIPC () << 0);
     return ret;
  }

  std::string GetName(void) const { return "EIPC"; }

  CfSReg GetCfSRegId(void) const { return CF_SR_HMEIPC; }

  SRegHMEIPC(ContextId ctid, ProcElementBase* pe) :
     SRegBaseEIPC (ctid, PRIV_PERMIT_SV, PRIV_PIE_SV, pe),
        m_eipc (0) {}
};

/*!
  @brief EIPSW register class
 */
class SRegHMEIPSW : public SRegBasePSW, SRegHMEIPSW_IF {
private:
  bool    m_um;
  RegData m_eimask;
  RegData m_cu;
  bool    m_ebv;
  bool    m_np;
  bool    m_ep;
  bool    m_id;
  bool    m_sat;
  bool    m_cy;
  bool    m_ov;
  bool    m_s;
  bool    m_z;

public:
  void SetUM(bool um) { m_um = um; }
  void SetEIMASK(RegData eimask);
  void SetCU(RegData cu);
  void SetEBV(bool ebv) { m_ebv = ebv; }
  void SetNP(bool np) { m_np = np; }
  void SetEP(bool ep) { m_ep = ep; }
  void SetID(bool id) { m_id = id; }
  void SetSAT(bool sat) { m_sat = sat; }
  void SetCY(bool cy) { m_cy = cy; }
  void SetOV(bool ov) { m_ov = ov; }
  void SetS(bool s) { m_s = s; }
  void SetZ(bool z) { m_z = z; }
  bool GetUM(void) const { return m_um; }
  RegData GetEIMASK(void) const { return m_eimask; }
  RegData GetCU(void) const { return m_cu; }
  bool GetEBV(void) const { return m_ebv; }
  bool GetNP(void) const { return m_np; }
  bool GetEP(void) const { return m_ep; }
  bool GetID(void) const { return m_id; }
  bool GetSAT(void) const { return m_sat; }
  bool GetCY(void) const { return m_cy; }
  bool GetOV(void) const { return m_ov; }
  bool GetS(void) const { return m_s; }
  bool GetZ(void) const { return m_z; }

  void Reset(void) {
     m_um = 0x0U;
     m_eimask = 0x0U;
     m_cu = 0x0U;
     m_ebv = 0x0U;
     m_np = 0x0U;
     m_ep = 0x0U;
     m_id = 0x1U;
     m_sat = 0x0U;
     m_cy = 0x0U;
     m_ov = 0x0U;
     m_s = 0x0U;
     m_z = 0x0U;
  }

  void Write(RegData data, PrivLevel priv, ContextId ctxt);

  RegData Read(PrivLevel priv, ContextId ctxt) const;

  std::string GetName(void) const { return "EIPSW"; }

  CfSReg GetCfSRegId(void) const { return CF_SR_HMEIPSW; }

  SRegHMEIPSW(ContextId ctid, ProcElementBase* pe) :
     SRegBasePSW (ctid, PRIV_PERMIT_SV, PRIV_PIE_SV, pe),
        m_um (0),
        m_eimask (0),
        m_cu (0),
        m_ebv (0),
        m_np (0),
        m_ep (0),
        m_id (0),
        m_sat (0),
        m_cy (0),
        m_ov (0),
        m_s (0),
        m_z (0) {}
};

/*!
  @brief EIPSWH register class
 */
class SRegEIPSWH : public SRegBasicImpl, SRegEIPSWH_IF {
private:
  bool    m_gm;
  RegData m_gpid;

public:
  void SetGM(bool gm) { m_gm = gm; }
  void SetGPID(RegData gpid) { m_gpid = gpid; }
  bool GetGM(void) const { return m_gm; }
  RegData GetGPID(void) const { return m_gpid; }

  void Reset(void) {
     m_gm = 0x0U;
     m_gpid = 0x0U;
  }

  void Write(RegData data, PrivLevel priv, ContextId ctxt) {
        if (m_pe->IsHvEnabled() == false) return;
     SetGM ((data >> 31) & RegData (0x00000001U));
     SetGPID ((data >> 8) & RegData (0x00000007U));
  }

  RegData Read(PrivLevel priv, ContextId ctxt) const {
        if (m_pe->IsHvEnabled () == false) return 0;
     RegData ret = 0x0
            | (GetGM () << 31)
            | (GetGPID () << 8);
     return ret;
  }

  std::string GetName(void) const { return "EIPSWH"; }

  CfSReg GetCfSRegId(void) const { return CF_SR_EIPSWH; }

  SRegEIPSWH(ContextId ctid, ProcElementBase* pe) :
     SRegBasicImpl (ctid, PRIV_PERMIT_HV, PRIV_PIE_HV, pe),
        m_gm (0),
        m_gpid (0) {}
};

/*!
  @brief EIWR register class
 */
class SRegHMEIWR : public SRegBaseEIWR, SRegHMEIWR_IF {
private:
  RegData m_eiwr;

public:
  void SetEIWR(RegData eiwr) { m_eiwr = eiwr; }
  RegData GetEIWR(void) const { return m_eiwr; }

  void Reset(void) {
  }

  void Write(RegData data, PrivLevel priv, ContextId ctxt) {
        if (IsPrivileged(priv) == false) return;
     SetEIWR ((data >> 0) & RegData (0xffffffffU));
  }

  RegData Read(PrivLevel priv, ContextId ctxt) const {
        if (IsPrivileged (priv) == false) return 0;
     RegData ret = 0x0
            | (GetEIWR () << 0);
     return ret;
  }

  std::string GetName(void) const { return "EIWR"; }

  CfSReg GetCfSRegId(void) const { return CF_SR_HMEIWR; }

  SRegHMEIWR(ContextId ctid, ProcElementBase* pe) :
     SRegBaseEIWR (ctid, PRIV_PERMIT_SV, PRIV_PIE_SV, pe),
        m_eiwr (0) {}
};

/*!
  @brief FEIC register class
 */
class SRegHMFEIC : public SRegBaseFEIC, SRegHMFEIC_IF {
private:
  RegData m_feic;

public:
  void SetFEIC(RegData feic) { m_feic = feic; }
  RegData GetFEIC(void) const { return m_feic; }

  void Reset(void) {
     m_feic = 0x0U;
  }

  void Write(RegData data, PrivLevel priv, ContextId ctxt) {
        if (IsPrivileged(priv) == false) return;
     SetFEIC ((data >> 0) & RegData (0xffffffffU));
  }

  RegData Read(PrivLevel priv, ContextId ctxt) const {
        if (IsPrivileged (priv) == false) return 0;
     RegData ret = 0x0
            | (GetFEIC () << 0);
     return ret;
  }

  std::string GetName(void) const { return "FEIC"; }

  CfSReg GetCfSRegId(void) const { return CF_SR_HMFEIC; }

  SRegHMFEIC(ContextId ctid, ProcElementBase* pe) :
     SRegBaseFEIC (ctid, PRIV_PERMIT_SV, PRIV_PIE_SV, pe),
        m_feic (0) {}
};

/*!
  @brief FEPC register class
 */
class SRegHMFEPC : public SRegBaseFEPC, SRegHMFEPC_IF {
private:
  RegData m_fepc;

public:
  void SetFEPC(RegData fepc) { m_fepc = fepc; }
  RegData GetFEPC(void) const { return m_fepc; }

  void Reset(void);

  void Write(RegData data, PrivLevel priv, ContextId ctxt) {
        if (IsPrivileged(priv) == false) return;
     SetFEPC ((data >> 0) & RegData (0xffffffffU));
  }

  RegData Read(PrivLevel priv, ContextId ctxt) const {
        if (IsPrivileged (priv) == false) return 0;
     RegData ret = 0x0
            | (GetFEPC () << 0);
     return ret;
  }

  std::string GetName(void) const { return "FEPC"; }

  CfSReg GetCfSRegId(void) const { return CF_SR_HMFEPC; }

  SRegHMFEPC(ContextId ctid, ProcElementBase* pe) :
     SRegBaseFEPC (ctid, PRIV_PERMIT_SV, PRIV_PIE_SV, pe),
        m_fepc (0) {}
};

/*!
  @brief FEPSW register class
 */
class SRegHMFEPSW : public SRegBasePSW, SRegHMFEPSW_IF {
private:
  bool    m_um;
  RegData m_eimask;
  RegData m_cu;
  bool    m_ebv;
  bool    m_np;
  bool    m_ep;
  bool    m_id;
  bool    m_sat;
  bool    m_cy;
  bool    m_ov;
  bool    m_s;
  bool    m_z;

public:
  void SetUM(bool um) { m_um = um; }
  void SetEIMASK(RegData eimask);
  void SetCU(RegData cu);
  void SetEBV(bool ebv) { m_ebv = ebv; }
  void SetNP(bool np) { m_np = np; }
  void SetEP(bool ep) { m_ep = ep; }
  void SetID(bool id) { m_id = id; }
  void SetSAT(bool sat) { m_sat = sat; }
  void SetCY(bool cy) { m_cy = cy; }
  void SetOV(bool ov) { m_ov = ov; }
  void SetS(bool s) { m_s = s; }
  void SetZ(bool z) { m_z = z; }
  bool GetUM(void) const { return m_um; }
  RegData GetEIMASK(void) const { return m_eimask; }
  RegData GetCU(void) const { return m_cu; }
  bool GetEBV(void) const { return m_ebv; }
  bool GetNP(void) const { return m_np; }
  bool GetEP(void) const { return m_ep; }
  bool GetID(void) const { return m_id; }
  bool GetSAT(void) const { return m_sat; }
  bool GetCY(void) const { return m_cy; }
  bool GetOV(void) const { return m_ov; }
  bool GetS(void) const { return m_s; }
  bool GetZ(void) const { return m_z; }

  void Reset(void) {
     m_um = 0x0U;
     m_eimask = 0x0U;
     m_cu = 0x0U;
     m_ebv = 0x0U;
     m_np = 0x0U;
     m_ep = 0x0U;
     m_id = 0x1U;
     m_sat = 0x0U;
     m_cy = 0x0U;
     m_ov = 0x0U;
     m_s = 0x0U;
     m_z = 0x0U;
  }

  void Write(RegData data, PrivLevel priv, ContextId ctxt);

  RegData Read(PrivLevel priv, ContextId ctxt) const;

  std::string GetName(void) const { return "FEPSW"; }

  CfSReg GetCfSRegId(void) const { return CF_SR_HMFEPSW; }

  SRegHMFEPSW(ContextId ctid, ProcElementBase* pe) :
     SRegBasePSW (ctid, PRIV_PERMIT_SV, PRIV_PIE_SV, pe),
        m_um (0),
        m_eimask (0),
        m_cu (0),
        m_ebv (0),
        m_np (0),
        m_ep (0),
        m_id (0),
        m_sat (0),
        m_cy (0),
        m_ov (0),
        m_s (0),
        m_z (0) {}
};

/*!
  @brief FEPSWH register class
 */
class SRegFEPSWH : public SRegBasicImpl, SRegFEPSWH_IF {
private:
  bool    m_gm;
  RegData m_gpid;

public:
  void SetGM(bool gm) { m_gm = gm; }
  void SetGPID(RegData gpid) { m_gpid = gpid; }
  bool GetGM(void) const { return m_gm; }
  RegData GetGPID(void) const { return m_gpid; }

  void Reset(void) {
     m_gm = 0x0U;
     m_gpid = 0x0U;
  }

  void Write(RegData data, PrivLevel priv, ContextId ctxt) {
        if (m_pe->IsHvEnabled() == false) return;
     SetGM ((data >> 31) & RegData (0x00000001U));
     SetGPID ((data >> 8) & RegData (0x00000007U));
  }

  RegData Read(PrivLevel priv, ContextId ctxt) const {
        if (m_pe->IsHvEnabled () == false) return 0;
     RegData ret = 0x0
            | (GetGM () << 31)
            | (GetGPID () << 8);
     return ret;
  }

  std::string GetName(void) const { return "FEPSWH"; }

  CfSReg GetCfSRegId(void) const { return CF_SR_FEPSWH; }

  SRegFEPSWH(ContextId ctid, ProcElementBase* pe) :
     SRegBasicImpl (ctid, PRIV_PERMIT_HV, PRIV_PIE_HV, pe),
        m_gm (0),
        m_gpid (0) {}
};

/*!
  @brief FEWR register class
 */
class SRegHMFEWR : public SRegBaseFEWR, SRegHMFEWR_IF {
private:
  RegData m_fewr;

public:
  void SetFEWR(RegData fewr) { m_fewr = fewr; }
  RegData GetFEWR(void) const { return m_fewr; }

  void Reset(void) {
  }

  void Write(RegData data, PrivLevel priv, ContextId ctxt) {
        if (IsPrivileged(priv) == false) return;
     SetFEWR ((data >> 0) & RegData (0xffffffffU));
  }

  RegData Read(PrivLevel priv, ContextId ctxt) const {
        if (IsPrivileged (priv) == false) return 0;
     RegData ret = 0x0
            | (GetFEWR () << 0);
     return ret;
  }

  std::string GetName(void) const { return "FEWR"; }

  CfSReg GetCfSRegId(void) const { return CF_SR_HMFEWR; }

  SRegHMFEWR(ContextId ctid, ProcElementBase* pe) :
     SRegBaseFEWR (ctid, PRIV_PERMIT_SV, PRIV_PIE_SV, pe),
        m_fewr (0) {}
};

/*!
  @brief FPCC register class
 */
class SRegFPCC : public SRegBasicImpl, SRegFPCC_IF {
private:
  RegData m_cc;

public:
  void SetCC(RegData cc);
  RegData GetCC(void) const;

  void Reset(void) {
  }

  void Write(RegData data, PrivLevel priv, ContextId ctxt) {
        if (IsPrivileged(priv) == false) return;
     SetCC ((data >> 0) & RegData (0x000000ffU));
  }

  RegData Read(PrivLevel priv, ContextId ctxt) const {
        if (IsPrivileged (priv) == false) return 0;
     RegData ret = 0x0
            | (GetCC () << 0);
     return ret;
  }

  CoproType GetCoproType (void) const { return COPRO_FPU; }
  std::string GetName(void) const { return "FPCC"; }

  CfSReg GetCfSRegId(void) const { return CF_SR_FPCC; }

  SRegFPCC(ContextId ctid, ProcElementBase* pe) :
     SRegBasicImpl (ctid, PRIV_PERMIT_UM, PRIV_PIE_UM, pe),
        m_cc (0) {}
};

/*!
  @brief FPCFG register class
 */
class SRegFPCFG : public SRegBasicImpl, SRegFPCFG_IF {
private:
  RegData m_rm;
  RegData m_xe;

public:
  void SetRM(RegData rm);
  void SetXE(RegData xe);
  RegData GetRM(void) const;
  RegData GetXE(void) const;

  void Reset(void) {
     m_rm = 0x0U;
     m_xe = 0x0U;
  }

  void Write(RegData data, PrivLevel priv, ContextId ctxt) {
        if (IsPrivileged(priv) == false) return;
     SetRM ((data >> 8) & RegData (0x00000003U));
     SetXE ((data >> 0) & RegData (0x0000001fU));
  }

  RegData Read(PrivLevel priv, ContextId ctxt) const {
        if (IsPrivileged (priv) == false) return 0;
     RegData ret = 0x0
            | (GetRM () << 8)
            | (GetXE () << 0);
     return ret;
  }

  CoproType GetCoproType (void) const { return COPRO_FPU; }
  std::string GetName(void) const { return "FPCFG"; }

  CfSReg GetCfSRegId(void) const { return CF_SR_FPCFG; }

  SRegFPCFG(ContextId ctid, ProcElementBase* pe) :
     SRegBasicImpl (ctid, PRIV_PERMIT_UM, PRIV_PIE_UM, pe),
        m_rm (0),
        m_xe (0) {}
};

/*!
  @brief FPEC register class
 */
class SRegFPEC : public SRegBasicImpl, SRegFPEC_IF {
private:
  bool    m_fpivd;

public:
  void SetFPIVD(bool fpivd);
  bool GetFPIVD(void) const { return m_fpivd; }

  void Reset(void) {
     m_fpivd = 0x0U;
  }

  void Write(RegData data, PrivLevel priv, ContextId ctxt);

  RegData Read(PrivLevel priv, ContextId ctxt) const {
        if (IsPrivileged (priv) == false) return 0;
     RegData ret = 0x0
            | (GetFPIVD () << 0);
     return ret;
  }

  CoproType GetCoproType (void) const { return COPRO_FPU; }
  std::string GetName(void) const { return "FPEC"; }

  CfSReg GetCfSRegId(void) const { return CF_SR_FPEC; }

  SRegFPEC(ContextId ctid, ProcElementBase* pe) :
     SRegBasicImpl (ctid, PRIV_PERMIT_SV, PRIV_PIE_SV, pe),
        m_fpivd (0) {}
};

/*!
  @brief FPEPC register class
 */
class SRegFPEPC : public SRegBasicImpl, SRegFPEPC_IF {
private:
  RegData m_fpepc;

public:
  void SetFPEPC(RegData fpepc) { m_fpepc = fpepc; }
  RegData GetFPEPC(void) const { return m_fpepc; }

  void Reset(void) {
  }

  void Write(RegData data, PrivLevel priv, ContextId ctxt) {
        if (IsPrivileged(priv) == false) return;
     SetFPEPC ((data >> 1) & RegData (0x7fffffffU));
  }

  RegData Read(PrivLevel priv, ContextId ctxt) const {
        if (IsPrivileged (priv) == false) return 0;
     RegData ret = 0x0
            | (GetFPEPC () << 1);
     return ret;
  }

  CoproType GetCoproType (void) const { return COPRO_FPU; }
  std::string GetName(void) const { return "FPEPC"; }

  CfSReg GetCfSRegId(void) const { return CF_SR_FPEPC; }

  SRegFPEPC(ContextId ctid, ProcElementBase* pe) :
     SRegBasicImpl (ctid, PRIV_PERMIT_SV, PRIV_PIE_SV, pe),
        m_fpepc (0) {}
};

/*!
  @brief FPIPR register class
 */
class SRegFPIPR : public SRegBasicImpl, SRegFPIPR_IF {
private:
  RegData m_fpipr;

public:
  void SetFPIPR(RegData fpipr);
  RegData GetFPIPR(void) const { return m_fpipr; }

  void Reset(void) {
     m_fpipr = 0x0U;
  }

  void Write(RegData data, PrivLevel priv, ContextId ctxt) {
        if (IsPrivileged(priv) == false) return;
     SetFPIPR ((data >> 0) & RegData (0x0000001fU));
  }

  RegData Read(PrivLevel priv, ContextId ctxt) const {
        if (IsPrivileged (priv) == false) return 0;
     RegData ret = 0x0
            | (GetFPIPR () << 0);
     return ret;
  }

  std::string GetName(void) const { return "FPIPR"; }

  CfSReg GetCfSRegId(void) const { return CF_SR_FPIPR; }

  SRegFPIPR(ContextId ctid, ProcElementBase* pe) :
     SRegBasicImpl (ctid, PRIV_PERMIT_SV, PRIV_PIE_SV, pe),
        m_fpipr (0) {}
};

/*!
  @brief FPSR register class
 */
class SRegFPSR : public SRegBasicImpl, SRegFPSR_IF {
private:
  RegData m_cc;
  bool    m_fn;
  bool    m_if_;
  bool    m_pem;
  RegData m_rm;
  bool    m_fs;
  RegData m_xc;
  RegData m_xe;
  RegData m_xp;

public:
  void SetCC(RegData cc) { m_cc = cc; }
  void SetFN(bool fn) { m_fn = fn; }
  void SetIF_(bool if_) { m_if_ = if_; }
  void SetPEM(bool pem);
  void SetRM(RegData rm) { m_rm = rm; }
  void SetFS(bool fs) { m_fs = fs; }
  void SetXC(RegData xc) { m_xc = xc; }
  void SetXE(RegData xe) { m_xe = xe; }
  void SetXP(RegData xp) { m_xp = xp; }
  RegData GetCC(void) const { return m_cc; }
  bool GetFN(void) const { return m_fn; }
  bool GetIF_(void) const { return m_if_; }
  bool GetPEM(void) const { return m_pem; }
  RegData GetRM(void) const { return m_rm; }
  bool GetFS(void) const { return m_fs; }
  RegData GetXC(void) const { return m_xc; }
  RegData GetXE(void) const { return m_xe; }
  RegData GetXP(void) const { return m_xp; }

  void Reset(void);

  void Write(RegData data, PrivLevel priv, ContextId ctxt) {
        if (IsPrivileged(priv) == false) return;
     SetCC ((data >> 24) & RegData (0x000000ffU));
     SetFN ((data >> 23) & RegData (0x00000001U));
     SetIF_ ((data >> 22) & RegData (0x00000001U));
     SetPEM ((data >> 21) & RegData (0x00000001U));
     SetRM ((data >> 18) & RegData (0x00000003U));
     SetFS ((data >> 17) & RegData (0x00000001U));
     SetXC ((data >> 10) & RegData (0x0000003fU));
     SetXE ((data >> 5) & RegData (0x0000001fU));
     SetXP ((data >> 0) & RegData (0x0000001fU));
  }

  RegData Read(PrivLevel priv, ContextId ctxt) const {
        if (IsPrivileged (priv) == false) return 0;
     RegData ret = 0x0
            | (GetCC () << 24)
            | (GetFN () << 23)
            | (GetIF_ () << 22)
            | (GetPEM () << 21)
            | (GetRM () << 18)
            | (GetFS () << 17)
            | (GetXC () << 10)
            | (GetXE () << 5)
            | (GetXP () << 0);
     return ret;
  }

  CoproType GetCoproType (void) const { return COPRO_FPU; }
  std::string GetName(void) const { return "FPSR"; }

  CfSReg GetCfSRegId(void) const { return CF_SR_FPSR; }

  SRegFPSR(ContextId ctid, ProcElementBase* pe) :
     SRegBasicImpl (ctid, PRIV_PERMIT_SV, PRIV_PIE_SV, pe),
        m_cc (0),
        m_fn (0),
        m_if_ (0),
        m_pem (0),
        m_rm (0),
        m_fs (0),
        m_xc (0),
        m_xe (0),
        m_xp (0) {}
};

/*!
  @brief FPST register class
 */
class SRegFPST : public SRegBasicImpl, SRegFPST_IF {
private:
  RegData m_xc;
  bool    m_if_;
  RegData m_xp;

public:
  void SetXC(RegData xc);
  void SetIF_(bool if_);
  void SetXP(RegData xp);
  RegData GetXC(void) const;
  bool GetIF_(void) const;
  RegData GetXP(void) const;

  void Reset(void) {
  }

  void Write(RegData data, PrivLevel priv, ContextId ctxt) {
        if (IsPrivileged(priv) == false) return;
     SetXC ((data >> 8) & RegData (0x0000003fU));
     SetIF_ ((data >> 5) & RegData (0x00000001U));
     SetXP ((data >> 0) & RegData (0x0000001fU));
  }

  RegData Read(PrivLevel priv, ContextId ctxt) const {
        if (IsPrivileged (priv) == false) return 0;
     RegData ret = 0x0
            | (GetXC () << 8)
            | (GetIF_ () << 5)
            | (GetXP () << 0);
     return ret;
  }

  CoproType GetCoproType (void) const { return COPRO_FPU; }
  std::string GetName(void) const { return "FPST"; }

  CfSReg GetCfSRegId(void) const { return CF_SR_FPST; }

  SRegFPST(ContextId ctid, ProcElementBase* pe) :
     SRegBasicImpl (ctid, PRIV_PERMIT_UM, PRIV_PIE_UM, pe),
        m_xc (0),
        m_if_ (0),
        m_xp (0) {}
};

/*!
  @brief FXCFG register class
 */
class SRegFXCFG : public SRegBasicImpl, SRegFXCFG_IF {
private:
  RegData m_rm;
  RegData m_xe;

public:
  void SetRM(RegData rm);
  void SetXE(RegData xe);
  RegData GetRM(void) const;
  RegData GetXE(void) const;

  void Reset(void) {
     m_rm = 0x0U;
     m_xe = 0x0U;
  }

  void Write(RegData data, PrivLevel priv, ContextId ctxt) {
        if (IsPrivileged(priv) == false) return;
     SetRM ((data >> 8) & RegData (0x00000003U));
     SetXE ((data >> 0) & RegData (0x0000001fU));
  }

  RegData Read(PrivLevel priv, ContextId ctxt) const {
        if (IsPrivileged (priv) == false) return 0;
     RegData ret = 0x0
            | (GetRM () << 8)
            | (GetXE () << 0);
     return ret;
  }

  CoproType GetCoproType (void) const { return COPRO_SIMD; }
  std::string GetName(void) const { return "FXCFG"; }

  CfSReg GetCfSRegId(void) const { return CF_SR_FXCFG; }

  SRegFXCFG(ContextId ctid, ProcElementBase* pe) :
     SRegBasicImpl (ctid, PRIV_PERMIT_UM, PRIV_PIE_UM, pe),
        m_rm (0),
        m_xe (0) {}
};

/*!
  @brief FXINFO register class
 */
class SRegFXINFO : public SRegBasicImpl, SRegFXINFO_IF {
private:
  bool    m_nfpu;
  bool    m_rsize;

public:
  void SetNFPU(bool nfpu) { m_nfpu = nfpu; }
  void SetRSIZE(bool rsize) { m_rsize = rsize; }
  bool GetNFPU(void) const { return m_nfpu; }
  bool GetRSIZE(void) const { return m_rsize; }

  void Reset(void) {
     m_nfpu = 0x1U;
     m_rsize = 0x1U;
  }

  void Write(RegData data, PrivLevel priv, ContextId ctxt) {
        if (IsPrivileged(priv) == false) return;
  }

  RegData Read(PrivLevel priv, ContextId ctxt) const {
        if (IsPrivileged (priv) == false) return 0;
     RegData ret = 0x0
            | (GetNFPU () << 1)
            | (GetRSIZE () << 0);
     return ret;
  }

  CoproType GetCoproType (void) const { return COPRO_SIMD; }
  std::string GetName(void) const { return "FXINFO"; }

  CfSReg GetCfSRegId(void) const { return CF_SR_FXINFO; }

  SRegFXINFO(ContextId ctid, ProcElementBase* pe) :
     SRegBasicImpl (ctid, PRIV_PERMIT_UM, PRIV_PIE_UM, pe),
        m_nfpu (0),
        m_rsize (0) {}
};

/*!
  @brief FXSR register class
 */
class SRegFXSR : public SRegBasicImpl, SRegFXSR_IF {
private:
  bool    m_fn;
  bool    m_if_;
  bool    m_pem;
  RegData m_rm;
  bool    m_fs;
  RegData m_xc;
  RegData m_xe;
  RegData m_xp;

public:
  void SetFN(bool fn) { m_fn = fn; }
  void SetIF_(bool if_) { m_if_ = if_; }
  void SetPEM(bool pem) { m_pem = pem; }
  void SetRM(RegData rm) { m_rm = rm; }
  void SetFS(bool fs) { m_fs = fs; }
  void SetXC(RegData xc) { m_xc = xc; }
  void SetXE(RegData xe) { m_xe = xe; }
  void SetXP(RegData xp) { m_xp = xp; }
  bool GetFN(void) const { return m_fn; }
  bool GetIF_(void) const { return m_if_; }
  bool GetPEM(void) const { return m_pem; }
  RegData GetRM(void) const { return m_rm; }
  bool GetFS(void) const { return m_fs; }
  RegData GetXC(void) const { return m_xc; }
  RegData GetXE(void) const { return m_xe; }
  RegData GetXP(void) const { return m_xp; }

  void Reset(void) {
     m_fn = 0x0U;
     m_if_ = 0x0U;
     m_pem = 0x1U;
     m_rm = 0x0U;
     m_fs = 0x1U;
     m_xe = 0x0U;
  }

  void Write(RegData data, PrivLevel priv, ContextId ctxt) {
        if (IsPrivileged(priv) == false) return;
     SetFN ((data >> 23) & RegData (0x00000001U));
     SetIF_ ((data >> 22) & RegData (0x00000001U));
     SetRM ((data >> 18) & RegData (0x00000003U));
     SetFS ((data >> 17) & RegData (0x00000001U));
     SetXC ((data >> 10) & RegData (0x0000003fU));
     SetXE ((data >> 5) & RegData (0x0000001fU));
     SetXP ((data >> 0) & RegData (0x0000001fU));
  }

  RegData Read(PrivLevel priv, ContextId ctxt) const {
        if (IsPrivileged (priv) == false) return 0;
     RegData ret = 0x0
            | (GetFN () << 23)
            | (GetIF_ () << 22)
            | (GetPEM () << 21)
            | (GetRM () << 18)
            | (GetFS () << 17)
            | (GetXC () << 10)
            | (GetXE () << 5)
            | (GetXP () << 0);
     return ret;
  }

  CoproType GetCoproType (void) const { return COPRO_SIMD; }
  std::string GetName(void) const { return "FXSR"; }

  CfSReg GetCfSRegId(void) const { return CF_SR_FXSR; }

  SRegFXSR(ContextId ctid, ProcElementBase* pe) :
     SRegBasicImpl (ctid, PRIV_PERMIT_SV, PRIV_PIE_SV, pe),
        m_fn (0),
        m_if_ (0),
        m_pem (0),
        m_rm (0),
        m_fs (0),
        m_xc (0),
        m_xe (0),
        m_xp (0) {}
};

/*!
  @brief FXST register class
 */
class SRegFXST : public SRegBasicImpl, SRegFXST_IF {
private:
  RegData m_xc;
  bool    m_if_;
  RegData m_xp;

public:
  void SetXC(RegData xc);
  void SetIF_(bool if_);
  void SetXP(RegData xp);
  RegData GetXC(void) const;
  bool GetIF_(void) const;
  RegData GetXP(void) const;

  void Reset(void) {
     m_if_ = 0x0U;
  }

  void Write(RegData data, PrivLevel priv, ContextId ctxt) {
        if (IsPrivileged(priv) == false) return;
     SetXC ((data >> 8) & RegData (0x0000003fU));
     SetIF_ ((data >> 5) & RegData (0x00000001U));
     SetXP ((data >> 0) & RegData (0x0000001fU));
  }

  RegData Read(PrivLevel priv, ContextId ctxt) const {
        if (IsPrivileged (priv) == false) return 0;
     RegData ret = 0x0
            | (GetXC () << 8)
            | (GetIF_ () << 5)
            | (GetXP () << 0);
     return ret;
  }

  CoproType GetCoproType (void) const { return COPRO_SIMD; }
  std::string GetName(void) const { return "FXST"; }

  CfSReg GetCfSRegId(void) const { return CF_SR_FXST; }

  SRegFXST(ContextId ctid, ProcElementBase* pe) :
     SRegBasicImpl (ctid, PRIV_PERMIT_UM, PRIV_PIE_UM, pe),
        m_xc (0),
        m_if_ (0),
        m_xp (0) {}
};

/*!
  @brief FXXC register class
 */
class SRegFXXC : public SRegBasicImpl, SRegFXXC_IF {
private:
  RegData m_xc3;
  RegData m_xc2;
  RegData m_xc1;
  RegData m_xc0;

public:
  void SetXC3(RegData xc3) { m_xc3 = xc3; }
  void SetXC2(RegData xc2) { m_xc2 = xc2; }
  void SetXC1(RegData xc1) { m_xc1 = xc1; }
  void SetXC0(RegData xc0) { m_xc0 = xc0; }
  RegData GetXC3(void) const { return m_xc3; }
  RegData GetXC2(void) const { return m_xc2; }
  RegData GetXC1(void) const { return m_xc1; }
  RegData GetXC0(void) const { return m_xc0; }

  void Reset(void) {
  }

  void Write(RegData data, PrivLevel priv, ContextId ctxt) {
        if (IsPrivileged(priv) == false) return;
     SetXC3 ((data >> 24) & RegData (0x0000003fU));
     SetXC2 ((data >> 16) & RegData (0x0000003fU));
     SetXC1 ((data >> 8) & RegData (0x0000003fU));
     SetXC0 ((data >> 0) & RegData (0x0000003fU));
  }

  RegData Read(PrivLevel priv, ContextId ctxt) const {
        if (IsPrivileged (priv) == false) return 0;
     RegData ret = 0x0
            | (GetXC3 () << 24)
            | (GetXC2 () << 16)
            | (GetXC1 () << 8)
            | (GetXC0 () << 0);
     return ret;
  }

  CoproType GetCoproType (void) const { return COPRO_SIMD; }
  std::string GetName(void) const { return "FXXC"; }

  CfSReg GetCfSRegId(void) const { return CF_SR_FXXC; }

  SRegFXXC(ContextId ctid, ProcElementBase* pe) :
     SRegBasicImpl (ctid, PRIV_PERMIT_UM, PRIV_PIE_UM, pe),
        m_xc3 (0),
        m_xc2 (0),
        m_xc1 (0),
        m_xc0 (0) {}
};

/*!
  @brief FXXP register class
 */
class SRegFXXP : public SRegBasicImpl, SRegFXXP_IF {
private:
  RegData m_xp3;
  RegData m_xp2;
  RegData m_xp1;
  RegData m_xp0;

public:
  void SetXP3(RegData xp3) { m_xp3 = xp3; }
  void SetXP2(RegData xp2) { m_xp2 = xp2; }
  void SetXP1(RegData xp1) { m_xp1 = xp1; }
  void SetXP0(RegData xp0) { m_xp0 = xp0; }
  RegData GetXP3(void) const { return m_xp3; }
  RegData GetXP2(void) const { return m_xp2; }
  RegData GetXP1(void) const { return m_xp1; }
  RegData GetXP0(void) const { return m_xp0; }

  void Reset(void) {
  }

  void Write(RegData data, PrivLevel priv, ContextId ctxt) {
        if (IsPrivileged(priv) == false) return;
     SetXP3 ((data >> 24) & RegData (0x0000001fU));
     SetXP2 ((data >> 16) & RegData (0x0000001fU));
     SetXP1 ((data >> 8) & RegData (0x0000001fU));
     SetXP0 ((data >> 0) & RegData (0x0000001fU));
  }

  RegData Read(PrivLevel priv, ContextId ctxt) const {
        if (IsPrivileged (priv) == false) return 0;
     RegData ret = 0x0
            | (GetXP3 () << 24)
            | (GetXP2 () << 16)
            | (GetXP1 () << 8)
            | (GetXP0 () << 0);
     return ret;
  }

  CoproType GetCoproType (void) const { return COPRO_SIMD; }
  std::string GetName(void) const { return "FXXP"; }

  CfSReg GetCfSRegId(void) const { return CF_SR_FXXP; }

  SRegFXXP(ContextId ctid, ProcElementBase* pe) :
     SRegBasicImpl (ctid, PRIV_PERMIT_UM, PRIV_PIE_UM, pe),
        m_xp3 (0),
        m_xp2 (0),
        m_xp1 (0),
        m_xp0 (0) {}
};

/*!
  @brief GMCFG register class
 */
class SRegGMCFG : public SRegBasicImpl, SRegGMCFG_IF {
private:
  bool    m_gcu2;
  bool    m_gcu1;
  bool    m_gcu0;
  bool    m_gsyse;
  bool    m_hmp;
  bool    m_gmp;

public:
  void SetGCU2(bool gcu2) { m_gcu2 = gcu2; }
  void SetGCU1(bool gcu1);
  void SetGCU0(bool gcu0);
  void SetGSYSE(bool gsyse) { m_gsyse = gsyse; }
  void SetHMP(bool hmp) { m_hmp = hmp; }
  void SetGMP(bool gmp) { m_gmp = gmp; }
  bool GetGCU2(void) const { return m_gcu2; }
  bool GetGCU1(void) const { return m_gcu1; }
  bool GetGCU0(void) const { return m_gcu0; }
  bool GetGSYSE(void) const { return m_gsyse; }
  bool GetHMP(void) const { return m_hmp; }
  bool GetGMP(void) const { return m_gmp; }

  void Reset(void) {
     m_gcu2 = 0x0U;
     m_gcu1 = 0x0U;
     m_gcu0 = 0x0U;
     m_gsyse = 0x0U;
     m_hmp = 0x0U;
     m_gmp = 0x0U;
  }

  void Write(RegData data, PrivLevel priv, ContextId ctxt) {
        if (m_pe->IsHvEnabled() == false) return;
     SetGCU1 ((data >> 17) & RegData (0x00000001U));
     SetGCU0 ((data >> 16) & RegData (0x00000001U));
     SetGSYSE ((data >> 4) & RegData (0x00000001U));
     SetHMP ((data >> 1) & RegData (0x00000001U));
     SetGMP ((data >> 0) & RegData (0x00000001U));
  }

  RegData Read(PrivLevel priv, ContextId ctxt) const {
        if (m_pe->IsHvEnabled () == false) return 0;
     RegData ret = 0x0
            | (GetGCU2 () << 18)
            | (GetGCU1 () << 17)
            | (GetGCU0 () << 16)
            | (GetGSYSE () << 4)
            | (GetHMP () << 1)
            | (GetGMP () << 0);
     return ret;
  }

  std::string GetName(void) const { return "GMCFG"; }

  CfSReg GetCfSRegId(void) const { return CF_SR_GMCFG; }

  SRegGMCFG(ContextId ctid, ProcElementBase* pe) :
     SRegBasicImpl (ctid, PRIV_PERMIT_HV, PRIV_PIE_HV, pe),
        m_gcu2 (0),
        m_gcu1 (0),
        m_gcu0 (0),
        m_gsyse (0),
        m_hmp (0),
        m_gmp (0) {}
};

/*!
  @brief GMEBASE register class
 */
class SRegGMEBASE : public SRegBaseEBASE, SRegGMEBASE_IF {
private:
  RegData m_ebase;
  bool    m_dv;
  bool    m_rint;

public:
  void SetEBASE(RegData ebase) { m_ebase = ebase; }
  void SetDV(bool dv) { m_dv = dv; }
  void SetRINT(bool rint) { m_rint = rint; }
  RegData GetEBASE(void) const { return m_ebase; }
  bool GetDV(void) const { return m_dv; }
  bool GetRINT(void) const { return m_rint; }

  void Reset(void) {
  }

  void Write(RegData data, PrivLevel priv, ContextId ctxt) {
        if (m_pe->IsHvEnabled() == false) return;
     SetEBASE ((data >> 9) & RegData (0x007fffffU));
     SetDV ((data >> 1) & RegData (0x00000001U));
     SetRINT ((data >> 0) & RegData (0x00000001U));
  }

  RegData Read(PrivLevel priv, ContextId ctxt) const {
        if (m_pe->IsHvEnabled () == false) return 0;
     RegData ret = 0x0
            | (GetEBASE () << 9)
            | (GetDV () << 1)
            | (GetRINT () << 0);
     return ret;
  }

  std::string GetName(void) const { return "GMEBASE"; }

  CfSReg GetCfSRegId(void) const { return CF_SR_GMEBASE; }

  SRegGMEBASE(ContextId ctid, ProcElementBase* pe) :
     SRegBaseEBASE (ctid, PRIV_PERMIT_HV, PRIV_PIE_HV, pe),
        m_ebase (0),
        m_dv (0),
        m_rint (0) {}
};

/*!
  @brief GMEIIC register class
 */
class SRegGMEIIC : public SRegBaseEIIC, SRegGMEIIC_IF {
private:
  RegData m_eiic;

public:
  void SetEIIC(RegData eiic) { m_eiic = eiic; }
  RegData GetEIIC(void) const { return m_eiic; }

  void Reset(void) {
     m_eiic = 0x0U;
  }

  void Write(RegData data, PrivLevel priv, ContextId ctxt) {
        if (m_pe->IsHvEnabled() == false) return;
     SetEIIC ((data >> 0) & RegData (0xffffffffU));
  }

  RegData Read(PrivLevel priv, ContextId ctxt) const {
        if (m_pe->IsHvEnabled () == false) return 0;
     RegData ret = 0x0
            | (GetEIIC () << 0);
     return ret;
  }

  std::string GetName(void) const { return "GMEIIC"; }

  CfSReg GetCfSRegId(void) const { return CF_SR_GMEIIC; }

  SRegGMEIIC(ContextId ctid, ProcElementBase* pe) :
     SRegBaseEIIC (ctid, PRIV_PERMIT_HV, PRIV_PIE_HV, pe),
        m_eiic (0) {}
};

/*!
  @brief GMEIPC register class
 */
class SRegGMEIPC : public SRegBaseEIPC, SRegGMEIPC_IF {
private:
  RegData m_eipc;

public:
  void SetEIPC(RegData eipc) { m_eipc = eipc; }
  RegData GetEIPC(void) const { return m_eipc; }

  void Reset(void) {
  }

  void Write(RegData data, PrivLevel priv, ContextId ctxt) {
        if (m_pe->IsHvEnabled() == false) return;
     SetEIPC ((data >> 0) & RegData (0xffffffffU));
  }

  RegData Read(PrivLevel priv, ContextId ctxt) const {
        if (m_pe->IsHvEnabled () == false) return 0;
     RegData ret = 0x0
            | (GetEIPC () << 0);
     return ret;
  }

  std::string GetName(void) const { return "GMEIPC"; }

  CfSReg GetCfSRegId(void) const { return CF_SR_GMEIPC; }

  SRegGMEIPC(ContextId ctid, ProcElementBase* pe) :
     SRegBaseEIPC (ctid, PRIV_PERMIT_HV, PRIV_PIE_HV, pe),
        m_eipc (0) {}
};

/*!
  @brief GMEIPSW register class
 */
class SRegGMEIPSW : public SRegBasePSW, SRegGMEIPSW_IF {
private:
  bool    m_um;
  RegData m_eimask;
  RegData m_cu;
  bool    m_ebv;
  bool    m_np;
  bool    m_ep;
  bool    m_id;
  bool    m_sat;
  bool    m_cy;
  bool    m_ov;
  bool    m_s;
  bool    m_z;

public:
  void SetUM(bool um) { m_um = um; }
  void SetEIMASK(RegData eimask);
  void SetCU(RegData cu);
  void SetEBV(bool ebv) { m_ebv = ebv; }
  void SetNP(bool np) { m_np = np; }
  void SetEP(bool ep) { m_ep = ep; }
  void SetID(bool id) { m_id = id; }
  void SetSAT(bool sat) { m_sat = sat; }
  void SetCY(bool cy) { m_cy = cy; }
  void SetOV(bool ov) { m_ov = ov; }
  void SetS(bool s) { m_s = s; }
  void SetZ(bool z) { m_z = z; }
  bool GetUM(void) const { return m_um; }
  RegData GetEIMASK(void) const { return m_eimask; }
  RegData GetCU(void) const { return m_cu; }
  bool GetEBV(void) const { return m_ebv; }
  bool GetNP(void) const { return m_np; }
  bool GetEP(void) const { return m_ep; }
  bool GetID(void) const { return m_id; }
  bool GetSAT(void) const { return m_sat; }
  bool GetCY(void) const { return m_cy; }
  bool GetOV(void) const { return m_ov; }
  bool GetS(void) const { return m_s; }
  bool GetZ(void) const { return m_z; }

  void Reset(void) {
     m_um = 0x0U;
     m_eimask = 0x0U;
     m_cu = 0x0U;
     m_ebv = 0x0U;
     m_np = 0x0U;
     m_ep = 0x0U;
     m_id = 0x1U;
     m_sat = 0x0U;
     m_cy = 0x0U;
     m_ov = 0x0U;
     m_s = 0x0U;
     m_z = 0x0U;
  }

  void Write(RegData data, PrivLevel priv, ContextId ctxt);

  RegData Read(PrivLevel priv, ContextId ctxt) const;

  std::string GetName(void) const { return "GMEIPSW"; }

  CfSReg GetCfSRegId(void) const { return CF_SR_GMEIPSW; }

  SRegGMEIPSW(ContextId ctid, ProcElementBase* pe) :
     SRegBasePSW (ctid, PRIV_PERMIT_HV, PRIV_PIE_HV, pe),
        m_um (0),
        m_eimask (0),
        m_cu (0),
        m_ebv (0),
        m_np (0),
        m_ep (0),
        m_id (0),
        m_sat (0),
        m_cy (0),
        m_ov (0),
        m_s (0),
        m_z (0) {}
};

/*!
  @brief GMEIWR register class
 */
class SRegGMEIWR : public SRegBaseEIWR, SRegGMEIWR_IF {
private:
  RegData m_eiwr;

public:
  void SetEIWR(RegData eiwr) { m_eiwr = eiwr; }
  RegData GetEIWR(void) const { return m_eiwr; }

  void Reset(void) {
  }

  void Write(RegData data, PrivLevel priv, ContextId ctxt) {
        if (m_pe->IsHvEnabled() == false) return;
     SetEIWR ((data >> 0) & RegData (0xffffffffU));
  }

  RegData Read(PrivLevel priv, ContextId ctxt) const {
        if (m_pe->IsHvEnabled () == false) return 0;
     RegData ret = 0x0
            | (GetEIWR () << 0);
     return ret;
  }

  std::string GetName(void) const { return "GMEIWR"; }

  CfSReg GetCfSRegId(void) const { return CF_SR_GMEIWR; }

  SRegGMEIWR(ContextId ctid, ProcElementBase* pe) :
     SRegBaseEIWR (ctid, PRIV_PERMIT_HV, PRIV_PIE_HV, pe),
        m_eiwr (0) {}
};

/*!
  @brief GMFEIC register class
 */
class SRegGMFEIC : public SRegBaseFEIC, SRegGMFEIC_IF {
private:
  RegData m_feic;

public:
  void SetFEIC(RegData feic) { m_feic = feic; }
  RegData GetFEIC(void) const { return m_feic; }

  void Reset(void) {
     m_feic = 0x0U;
  }

  void Write(RegData data, PrivLevel priv, ContextId ctxt) {
        if (m_pe->IsHvEnabled() == false) return;
     SetFEIC ((data >> 0) & RegData (0xffffffffU));
  }

  RegData Read(PrivLevel priv, ContextId ctxt) const {
        if (m_pe->IsHvEnabled () == false) return 0;
     RegData ret = 0x0
            | (GetFEIC () << 0);
     return ret;
  }

  std::string GetName(void) const { return "GMFEIC"; }

  CfSReg GetCfSRegId(void) const { return CF_SR_GMFEIC; }

  SRegGMFEIC(ContextId ctid, ProcElementBase* pe) :
     SRegBaseFEIC (ctid, PRIV_PERMIT_HV, PRIV_PIE_HV, pe),
        m_feic (0) {}
};

/*!
  @brief GMFEPC register class
 */
class SRegGMFEPC : public SRegBaseFEPC, SRegGMFEPC_IF {
private:
  RegData m_fepc;

public:
  void SetFEPC(RegData fepc) { m_fepc = fepc; }
  RegData GetFEPC(void) const { return m_fepc; }

  void Reset(void) {
  }

  void Write(RegData data, PrivLevel priv, ContextId ctxt) {
        if (m_pe->IsHvEnabled() == false) return;
     SetFEPC ((data >> 0) & RegData (0xffffffffU));
  }

  RegData Read(PrivLevel priv, ContextId ctxt) const {
        if (m_pe->IsHvEnabled () == false) return 0;
     RegData ret = 0x0
            | (GetFEPC () << 0);
     return ret;
  }

  std::string GetName(void) const { return "GMFEPC"; }

  CfSReg GetCfSRegId(void) const { return CF_SR_GMFEPC; }

  SRegGMFEPC(ContextId ctid, ProcElementBase* pe) :
     SRegBaseFEPC (ctid, PRIV_PERMIT_HV, PRIV_PIE_HV, pe),
        m_fepc (0) {}
};

/*!
  @brief GMFEPSW register class
 */
class SRegGMFEPSW : public SRegBasePSW, SRegGMFEPSW_IF {
private:
  bool    m_um;
  RegData m_eimask;
  RegData m_cu;
  bool    m_ebv;
  bool    m_np;
  bool    m_ep;
  bool    m_id;
  bool    m_sat;
  bool    m_cy;
  bool    m_ov;
  bool    m_s;
  bool    m_z;

public:
  void SetUM(bool um) { m_um = um; }
  void SetEIMASK(RegData eimask);
  void SetCU(RegData cu);
  void SetEBV(bool ebv) { m_ebv = ebv; }
  void SetNP(bool np) { m_np = np; }
  void SetEP(bool ep) { m_ep = ep; }
  void SetID(bool id) { m_id = id; }
  void SetSAT(bool sat) { m_sat = sat; }
  void SetCY(bool cy) { m_cy = cy; }
  void SetOV(bool ov) { m_ov = ov; }
  void SetS(bool s) { m_s = s; }
  void SetZ(bool z) { m_z = z; }
  bool GetUM(void) const { return m_um; }
  RegData GetEIMASK(void) const { return m_eimask; }
  RegData GetCU(void) const { return m_cu; }
  bool GetEBV(void) const { return m_ebv; }
  bool GetNP(void) const { return m_np; }
  bool GetEP(void) const { return m_ep; }
  bool GetID(void) const { return m_id; }
  bool GetSAT(void) const { return m_sat; }
  bool GetCY(void) const { return m_cy; }
  bool GetOV(void) const { return m_ov; }
  bool GetS(void) const { return m_s; }
  bool GetZ(void) const { return m_z; }

  void Reset(void) {
     m_um = 0x0U;
     m_eimask = 0x0U;
     m_cu = 0x0U;
     m_ebv = 0x0U;
     m_np = 0x0U;
     m_ep = 0x0U;
     m_id = 0x1U;
     m_sat = 0x0U;
     m_cy = 0x0U;
     m_ov = 0x0U;
     m_s = 0x0U;
     m_z = 0x0U;
  }

  void Write(RegData data, PrivLevel priv, ContextId ctxt);

  RegData Read(PrivLevel priv, ContextId ctxt) const;

  std::string GetName(void) const { return "GMFEPSW"; }

  CfSReg GetCfSRegId(void) const { return CF_SR_GMFEPSW; }

  SRegGMFEPSW(ContextId ctid, ProcElementBase* pe) :
     SRegBasePSW (ctid, PRIV_PERMIT_HV, PRIV_PIE_HV, pe),
        m_um (0),
        m_eimask (0),
        m_cu (0),
        m_ebv (0),
        m_np (0),
        m_ep (0),
        m_id (0),
        m_sat (0),
        m_cy (0),
        m_ov (0),
        m_s (0),
        m_z (0) {}
};

/*!
  @brief GMFEWR register class
 */
class SRegGMFEWR : public SRegBaseFEWR, SRegGMFEWR_IF {
private:
  RegData m_fewr;

public:
  void SetFEWR(RegData fewr) { m_fewr = fewr; }
  RegData GetFEWR(void) const { return m_fewr; }

  void Reset(void) {
  }

  void Write(RegData data, PrivLevel priv, ContextId ctxt) {
        if (m_pe->IsHvEnabled() == false) return;
     SetFEWR ((data >> 0) & RegData (0xffffffffU));
  }

  RegData Read(PrivLevel priv, ContextId ctxt) const {
        if (m_pe->IsHvEnabled () == false) return 0;
     RegData ret = 0x0
            | (GetFEWR () << 0);
     return ret;
  }

  std::string GetName(void) const { return "GMFEWR"; }

  CfSReg GetCfSRegId(void) const { return CF_SR_GMFEWR; }

  SRegGMFEWR(ContextId ctid, ProcElementBase* pe) :
     SRegBaseFEWR (ctid, PRIV_PERMIT_HV, PRIV_PIE_HV, pe),
        m_fewr (0) {}
};

/*!
  @brief GMINTBP register class
 */
class SRegGMINTBP : public SRegBaseINTBP, SRegGMINTBP_IF {
private:
  RegData m_intbp;

public:
  void SetINTBP(RegData intbp) { m_intbp = intbp; }
  RegData GetINTBP(void) const { return m_intbp; }

  void Reset(void) {
  }

  void Write(RegData data, PrivLevel priv, ContextId ctxt) {
        if (m_pe->IsHvEnabled() == false) return;
     SetINTBP ((data >> 9) & RegData (0x007fffffU));
  }

  RegData Read(PrivLevel priv, ContextId ctxt) const {
        if (m_pe->IsHvEnabled () == false) return 0;
     RegData ret = 0x0
            | (GetINTBP () << 9);
     return ret;
  }

  std::string GetName(void) const { return "GMINTBP"; }

  CfSReg GetCfSRegId(void) const { return CF_SR_GMINTBP; }

  SRegGMINTBP(ContextId ctid, ProcElementBase* pe) :
     SRegBaseINTBP (ctid, PRIV_PERMIT_HV, PRIV_PIE_HV, pe),
        m_intbp (0) {}
};

/*!
  @brief GMINTCFG register class
 */
class SRegGMINTCFG : public SRegBaseINTCFG, SRegGMINTCFG_IF {
private:
  RegData m_ulnr;
  bool    m_epl;
  bool    m_ispc;

public:
  void SetULNR(RegData ulnr);
  void SetEPL(bool epl) { m_epl = epl; }
  void SetISPC(bool ispc) { m_ispc = ispc; }
  RegData GetULNR(void) const { return m_ulnr; }
  bool GetEPL(void) const { return m_epl; }
  bool GetISPC(void) const { return m_ispc; }

  void Reset(void) {
     m_ulnr = 0xFU;
     m_epl = 0x1U;
     m_ispc = 0x0U;
  }

  void Write(RegData data, PrivLevel priv, ContextId ctxt) {
        if (m_pe->IsHvEnabled() == false) return;
     SetULNR ((data >> 16) & RegData (0x000000ffU));
  }

  RegData Read(PrivLevel priv, ContextId ctxt) const {
        if (m_pe->IsHvEnabled () == false) return 0;
     RegData ret = 0x0
            | (GetULNR () << 16)
            | (GetEPL () << 1)
            | (GetISPC () << 0);
     return ret;
  }

  std::string GetName(void) const { return "GMINTCFG"; }

  CfSReg GetCfSRegId(void) const { return CF_SR_GMINTCFG; }

  SRegGMINTCFG(ContextId ctid, ProcElementBase* pe) :
     SRegBaseINTCFG (ctid, PRIV_PERMIT_HV, PRIV_PIE_HV, pe),
        m_ulnr (0),
        m_epl (0),
        m_ispc (0) {}
};

/*!
  @brief GMMEA register class
 */
class SRegGMMEA : public SRegBaseMEA, SRegGMMEA_IF {
private:
  RegData m_mea;

public:
  void SetMEA(RegData mea) { m_mea = mea; }
  RegData GetMEA(void) const { return m_mea; }

  void Reset(void) {
  }

  void Write(RegData data, PrivLevel priv, ContextId ctxt) {
        if (m_pe->IsHvEnabled() == false) return;
     SetMEA ((data >> 0) & RegData (0xffffffffU));
  }

  RegData Read(PrivLevel priv, ContextId ctxt) const {
        if (m_pe->IsHvEnabled () == false) return 0;
     RegData ret = 0x0
            | (GetMEA () << 0);
     return ret;
  }

  std::string GetName(void) const { return "GMMEA"; }

  CfSReg GetCfSRegId(void) const { return CF_SR_GMMEA; }

  SRegGMMEA(ContextId ctid, ProcElementBase* pe) :
     SRegBaseMEA (ctid, PRIV_PERMIT_HV, PRIV_PIE_HV, pe),
        m_mea (0) {}
};

/*!
  @brief GMMEI register class
 */
class SRegGMMEI : public SRegBaseMEI, SRegGMMEI_IF {
private:
  RegData m_len;
  RegData m_reg;
  RegData m_ds;
  bool    m_u;
  RegData m_itype;
  bool    m_rw;

public:
  void SetLEN(RegData len) { m_len = len; }
  void SetREG(RegData reg) { m_reg = reg; }
  void SetDS(RegData ds) { m_ds = ds; }
  void SetU(bool u) { m_u = u; }
  void SetITYPE(RegData itype) { m_itype = itype; }
  void SetRW(bool rw) { m_rw = rw; }
  RegData GetLEN(void) const { return m_len; }
  RegData GetREG(void) const { return m_reg; }
  RegData GetDS(void) const { return m_ds; }
  bool GetU(void) const { return m_u; }
  RegData GetITYPE(void) const { return m_itype; }
  bool GetRW(void) const { return m_rw; }

  void Reset(void) {
  }

  void Write(RegData data, PrivLevel priv, ContextId ctxt) {
        if (m_pe->IsHvEnabled() == false) return;
     SetLEN ((data >> 28) & RegData (0x0000000fU));
     SetREG ((data >> 16) & RegData (0x0000001fU));
     SetDS ((data >> 9) & RegData (0x00000007U));
     SetU ((data >> 8) & RegData (0x00000001U));
     SetITYPE ((data >> 1) & RegData (0x0000001fU));
     SetRW ((data >> 0) & RegData (0x00000001U));
  }

  RegData Read(PrivLevel priv, ContextId ctxt) const {
        if (m_pe->IsHvEnabled () == false) return 0;
     RegData ret = 0x0
            | (GetLEN () << 28)
            | (GetREG () << 16)
            | (GetDS () << 9)
            | (GetU () << 8)
            | (GetITYPE () << 1)
            | (GetRW () << 0);
     return ret;
  }

  std::string GetName(void) const { return "GMMEI"; }

  CfSReg GetCfSRegId(void) const { return CF_SR_GMMEI; }

  SRegGMMEI(ContextId ctid, ProcElementBase* pe) :
     SRegBaseMEI (ctid, PRIV_PERMIT_HV, PRIV_PIE_HV, pe),
        m_len (0),
        m_reg (0),
        m_ds (0),
        m_u (0),
        m_itype (0),
        m_rw (0) {}
};

/*!
  @brief GMMPM register class
 */
class SRegGMMPM : public SRegBaseMPM, SRegGMMPM_IF {
private:
  bool    m_gmpe;
  bool    m_svp;
  bool    m_mpe;

public:
  void SetGMPE(bool gmpe) { m_gmpe = gmpe; }
  void SetSVP(bool svp) { m_svp = svp; }
  void SetMPE(bool mpe);
  bool GetGMPE(void) const { return m_gmpe; }
  bool GetSVP(void) const { return m_svp; }
  bool GetMPE(void) const { return m_mpe; }

  void Reset(void) {
     m_gmpe = 0x0U;
     m_svp = 0x0U;
     m_mpe = 0x0U;
  }

  void Write(RegData data, PrivLevel priv, ContextId ctxt);

  RegData Read(PrivLevel priv, ContextId ctxt) const;

  std::string GetName(void) const { return "GMMPM"; }

  CfSReg GetCfSRegId(void) const { return CF_SR_GMMPM; }

  SRegGMMPM(ContextId ctid, ProcElementBase* pe) :
     SRegBaseMPM (ctid, PRIV_PERMIT_HV, PRIV_PIE_HV, pe),
        m_gmpe (0),
        m_svp (0),
        m_mpe (0) {}
};

/*!
  @brief GMPEID register class
 */
class SRegGMPEID : public SRegBasePEID, SRegGMPEID_IF {
private:
  RegData m_peid;

public:
  void SetPEID(RegData peid) { m_peid = peid; }
  RegData GetPEID(void) const { return m_peid; }

  void Reset(void);

  void Write(RegData data, PrivLevel priv, ContextId ctxt);

  RegData Read(PrivLevel priv, ContextId ctxt) const;

  std::string GetName(void) const { return "GMPEID"; }

  CfSReg GetCfSRegId(void) const { return CF_SR_GMPEID; }

  SRegGMPEID(ContextId ctid, ProcElementBase* pe) :
     SRegBasePEID (ctid, PRIV_PERMIT_UM, PRIV_PIE_UM, pe),
        m_peid (0) {}
};

/*!
  @brief GMPLMR register class
 */
class SRegGMPLMR : public SRegBasePLMR, SRegGMPLMR_IF {
private:
  RegData m_plm;

public:
  void SetPLM(RegData plm);
  RegData GetPLM(void) const { return m_plm; }

  void Reset(void);

  void Write(RegData data, PrivLevel priv, ContextId ctxt) {
        if (m_pe->IsHvEnabled() == false) return;
     SetPLM ((data >> 0) & RegData (0x000000ffU));
  }

  RegData Read(PrivLevel priv, ContextId ctxt) const {
        if (m_pe->IsHvEnabled () == false) return 0;
     RegData ret = 0x0
            | (GetPLM () << 0);
     return ret;
  }

  std::string GetName(void) const { return "GMPLMR"; }

  CfSReg GetCfSRegId(void) const { return CF_SR_GMPLMR; }

  SRegGMPLMR(ContextId ctid, ProcElementBase* pe) :
     SRegBasePLMR (ctid, PRIV_PERMIT_HV, PRIV_PIE_HV, pe),
        m_plm (0) {}
};

/*!
  @brief GMPSW register class
 */
class SRegGMPSW : public SRegBasePSW, SRegGMPSW_IF {
private:
  bool    m_um;
  RegData m_eimask;
  RegData m_cu;
  bool    m_ebv;
  bool    m_np;
  bool    m_ep;
  bool    m_id;
  bool    m_sat;
  bool    m_cy;
  bool    m_ov;
  bool    m_s;
  bool    m_z;

public:
  void SetUM(bool um);
  void SetEIMASK(RegData eimask);
  void SetCU(RegData cu);
  void SetEBV(bool ebv);
  void SetNP(bool np);
  void SetEP(bool ep) { m_ep = ep; }
  void SetID(bool id);
  void SetSAT(bool sat) { m_sat = sat; }
  void SetCY(bool cy) { m_cy = cy; }
  void SetOV(bool ov) { m_ov = ov; }
  void SetS(bool s) { m_s = s; }
  void SetZ(bool z) { m_z = z; }
  bool GetUM(void) const { return m_um; }
  RegData GetEIMASK(void) const { return m_eimask; }
  RegData GetCU(void) const { return m_cu; }
  bool GetEBV(void) const { return m_ebv; }
  bool GetNP(void) const { return m_np; }
  bool GetEP(void) const { return m_ep; }
  bool GetID(void) const { return m_id; }
  bool GetSAT(void) const { return m_sat; }
  bool GetCY(void) const { return m_cy; }
  bool GetOV(void) const { return m_ov; }
  bool GetS(void) const { return m_s; }
  bool GetZ(void) const { return m_z; }

  void Reset(void) {
     m_um = 0x0U;
     m_eimask = 0x0U;
     m_cu = 0x0U;
     m_ebv = 0x1U;
     m_np = 0x0U;
     m_ep = 0x0U;
     m_id = 0x1U;
     m_sat = 0x0U;
     m_cy = 0x0U;
     m_ov = 0x0U;
     m_s = 0x0U;
     m_z = 0x0U;
  }

  void Write(RegData data, PrivLevel priv, ContextId ctxt);

  RegData Read(PrivLevel priv, ContextId ctxt) const;

  std::string GetName(void) const { return "GMPSW"; }

  CfSReg GetCfSRegId(void) const { return CF_SR_GMPSW; }

  SRegGMPSW(ContextId ctid, ProcElementBase* pe) :
     SRegBasePSW (ctid, PRIV_PERMIT_UM, PRIV_PIE_UM, pe),
        m_um (0),
        m_eimask (0),
        m_cu (0),
        m_ebv (0),
        m_np (0),
        m_ep (0),
        m_id (0),
        m_sat (0),
        m_cy (0),
        m_ov (0),
        m_s (0),
        m_z (0) {}
};

/*!
  @brief GMSPID register class
 */
class SRegGMSPID : public SRegBaseSPID, SRegGMSPID_IF {
private:
  RegData m_spid;

public:
  void SetSPID(RegData spid);
  RegData GetSPID(void) const { return m_spid; }

  void Reset(void);

  void Write(RegData data, PrivLevel priv, ContextId ctxt);

  RegData Read(PrivLevel priv, ContextId ctxt) const {
        if (m_pe->IsHvEnabled () == false) return 0;
     RegData ret = 0x0
            | (GetSPID () << 0);
     return ret;
  }

  std::string GetName(void) const { return "GMSPID"; }

  CfSReg GetCfSRegId(void) const { return CF_SR_GMSPID; }

  SRegGMSPID(ContextId ctid, ProcElementBase* pe) :
     SRegBaseSPID (ctid, PRIV_PERMIT_HV, PRIV_PIE_HV, pe),
        m_spid (0) {}
};

/*!
  @brief GMSPIDLIST register class
 */
class SRegGMSPIDLIST : public SRegBaseSPIDLIST, SRegGMSPIDLIST_IF {
private:
  RegData m_spidlist;

public:
  void SetSPIDLIST(RegData spidlist);
  RegData GetSPIDLIST(void) const { return m_spidlist; }

  void Reset(void);

  void Write(RegData data, PrivLevel priv, ContextId ctxt);

  RegData Read(PrivLevel priv, ContextId ctxt) const {
        if (m_pe->IsHvEnabled () == false) return 0;
     RegData ret = 0x0
            | (GetSPIDLIST () << 0);
     return ret;
  }

  std::string GetName(void) const { return "GMSPIDLIST"; }

  CfSReg GetCfSRegId(void) const { return CF_SR_GMSPIDLIST; }

  SRegGMSPIDLIST(ContextId ctid, ProcElementBase* pe) :
     SRegBaseSPIDLIST (ctid, PRIV_PERMIT_HV, PRIV_PIE_HV, pe),
        m_spidlist (0) {}
};

/*!
  @brief GMSVLOCK register class
 */
class SRegGMSVLOCK : public SRegBaseSVLOCK, SRegGMSVLOCK_IF {
private:
  bool    m_svl;

public:
  void SetSVL(bool svl) { m_svl = svl; }
  bool GetSVL(void) const { return m_svl; }

  void Reset(void) {
     m_svl = 0x0U;
  }

  void Write(RegData data, PrivLevel priv, ContextId ctxt) {
        if (m_pe->IsHvEnabled() == false) return;
     SetSVL ((data >> 0) & RegData (0x00000001U));
  }

  RegData Read(PrivLevel priv, ContextId ctxt) const {
        if (m_pe->IsHvEnabled () == false) return 0;
     RegData ret = 0x0
            | (GetSVL () << 0);
     return ret;
  }

  std::string GetName(void) const { return "GMSVLOCK"; }

  CfSReg GetCfSRegId(void) const { return CF_SR_GMSVLOCK; }

  SRegGMSVLOCK(ContextId ctid, ProcElementBase* pe) :
     SRegBaseSVLOCK (ctid, PRIV_PERMIT_HV, PRIV_PIE_HV, pe),
        m_svl (0) {}
};

/*!
  @brief HVCFG register class
 */
class SRegHVCFG : public SRegBasicImpl, SRegHVCFG_IF {
private:
  bool    m_hve;

public:
  void SetHVE(bool hve);
  bool GetHVE(void) const { return m_hve; }

  void Reset(void);

  void Write(RegData data, PrivLevel priv, ContextId ctxt);

  RegData Read(PrivLevel priv, ContextId ctxt) const;

  std::string GetName(void) const { return "HVCFG"; }

  CfSReg GetCfSRegId(void) const { return CF_SR_HVCFG; }

  SRegHVCFG(ContextId ctid, ProcElementBase* pe) :
     SRegBasicImpl (ctid, PRIV_PERMIT_HV, PRIV_PIE_HV, pe),
        m_hve (0) {}
};

/*!
  @brief HVSB register class
 */
class SRegHVSB : public SRegBasicImpl, SRegHVSB_IF {
private:
  RegData m_hvsb;

public:
  void SetHVSB(RegData hvsb) { m_hvsb = hvsb; }
  RegData GetHVSB(void) const { return m_hvsb; }

  void Reset(void) {
  }

  void Write(RegData data, PrivLevel priv, ContextId ctxt);

  RegData Read(PrivLevel priv, ContextId ctxt) const;

  std::string GetName(void) const { return "HVSB"; }

  CfSReg GetCfSRegId(void) const { return CF_SR_HVSB; }

  SRegHVSB(ContextId ctid, ProcElementBase* pe) :
     SRegBasicImpl (ctid, PRIV_PERMIT_UM, PRIV_PIE_UM, pe),
        m_hvsb (0) {}
};

/*!
  @brief ICBKEY register class
 */
class SRegICBKEY : public SRegBasicImpl, SRegICBKEY_IF {
private:
  RegData m_cbakey;
  RegData m_keycode;

public:
  void SetCBAKEY(RegData cbakey) { m_cbakey = cbakey; }
  void SetKEYCODE(RegData keycode) { m_keycode = keycode; }
  RegData GetCBAKEY(void) const { return m_cbakey; }
  RegData GetKEYCODE(void) const { return m_keycode; }

  void Reset(void) {
     m_cbakey = 0x0U;
     m_keycode = 0x0U;
  }

  void Write(RegData data, PrivLevel priv, ContextId ctxt);

  RegData Read(PrivLevel priv, ContextId ctxt) const;

  std::string GetName(void) const { return "ICBKEY"; }

  CfSReg GetCfSRegId(void) const { return CF_SR_ICBKEY; }

  SRegICBKEY(ContextId ctid, ProcElementBase* pe) :
     SRegBasicImpl (ctid, PRIV_PERMIT_SV, PRIV_PIE_SV, pe),
        m_cbakey (0),
        m_keycode (0) {}
};

/*!
  @brief ICCFG register class
 */
class SRegICCFG : public SRegBasicImpl, SRegICCFG_IF {
private:
  RegData m_ichsize;
  RegData m_ichline;
  RegData m_ichway;

public:
  void SetICHSIZE(RegData ichsize) { m_ichsize = ichsize; }
  void SetICHLINE(RegData ichline) { m_ichline = ichline; }
  void SetICHWAY(RegData ichway) { m_ichway = ichway; }
  RegData GetICHSIZE(void) const { return m_ichsize; }
  RegData GetICHLINE(void) const { return m_ichline; }
  RegData GetICHWAY(void) const { return m_ichway; }

  void Reset(void) {
     m_ichway = 0x4U;
  }

  void Write(RegData data, PrivLevel priv, ContextId ctxt) {
        if (IsPrivileged(priv) == false) return;
  }

  RegData Read(PrivLevel priv, ContextId ctxt) const;

  std::string GetName(void) const { return "ICCFG"; }

  CfSReg GetCfSRegId(void) const { return CF_SR_ICCFG; }

  SRegICCFG(ContextId ctid, ProcElementBase* pe) :
     SRegBasicImpl (ctid, PRIV_PERMIT_SV, PRIV_PIE_SV, pe),
        m_ichsize (0),
        m_ichline (0),
        m_ichway (0) {}
};

/*!
  @brief ICCTRL register class
 */
class SRegICCTRL : public SRegBasicImpl, SRegICCTRL_IF {
private:
  bool    m_pbs;
  bool    m_ichclr;
  bool    m_icheiv;
  bool    m_ichemk;
  bool    m_ichen;

public:
  void SetPBS(bool pbs) { m_pbs = pbs; }
  void SetICHCLR(bool ichclr);
  void SetICHEIV(bool icheiv) { m_icheiv = icheiv; }
  void SetICHEMK(bool ichemk) { m_ichemk = ichemk; }
  void SetICHEN(bool ichen);
  bool GetPBS(void) const { return m_pbs; }
  bool GetICHCLR(void) const { return m_ichclr; }
  bool GetICHEIV(void) const { return m_icheiv; }
  bool GetICHEMK(void) const { return m_ichemk; }
  bool GetICHEN(void) const { return m_ichen; }

  void Reset(void);

  void Write(RegData data, PrivLevel priv, ContextId ctxt);

  RegData Read(PrivLevel priv, ContextId ctxt) const;

  std::string GetName(void) const { return "ICCTRL"; }

  CfSReg GetCfSRegId(void) const { return CF_SR_ICCTRL; }

  SRegICCTRL(ContextId ctid, ProcElementBase* pe) :
     SRegBasicImpl (ctid, PRIV_PERMIT_SV, PRIV_PIE_SV, pe),
        m_pbs (0),
        m_ichclr (0),
        m_icheiv (0),
        m_ichemk (0),
        m_ichen (0) {}
};

/*!
  @brief ICDATH register class
 */
class SRegICDATH : public SRegBasicImpl, SRegICDATH_IF {
private:
  RegData m_datah;

public:
  void SetDATAH(RegData datah) { m_datah = datah; }
  RegData GetDATAH(void) const { return m_datah; }

  void Reset(void) {
  }

  void Write(RegData data, PrivLevel priv, ContextId ctxt);

  RegData Read(PrivLevel priv, ContextId ctxt) const;

  std::string GetName(void) const { return "ICDATH"; }

  CfSReg GetCfSRegId(void) const { return CF_SR_ICDATH; }

  SRegICDATH(ContextId ctid, ProcElementBase* pe) :
     SRegBasicImpl (ctid, PRIV_PERMIT_HV, PRIV_PIE_HV, pe),
        m_datah (0) {}
};

/*!
  @brief ICDATL register class
 */
class SRegICDATL : public SRegBasicImpl, SRegICDATL_IF {
private:
  RegData m_datal;

public:
  void SetDATAL(RegData datal) { m_datal = datal; }
  RegData GetDATAL(void) const { return m_datal; }

  void Reset(void) {
  }

  void Write(RegData data, PrivLevel priv, ContextId ctxt);

  RegData Read(PrivLevel priv, ContextId ctxt) const;

  std::string GetName(void) const { return "ICDATL"; }

  CfSReg GetCfSRegId(void) const { return CF_SR_ICDATL; }

  SRegICDATL(ContextId ctid, ProcElementBase* pe) :
     SRegBasicImpl (ctid, PRIV_PERMIT_HV, PRIV_PIE_HV, pe),
        m_datal (0) {}
};

/*!
  @brief ICERR register class
 */
class SRegICERR : public SRegBasicImpl, SRegICERR_IF {
private:
  bool    m_cistw;
  bool    m_esafe;
  bool    m_esmh;
  bool    m_espbse;
  bool    m_este1;
  bool    m_este2;
  bool    m_esdc;
  bool    m_esde;
  bool    m_ermafe;
  bool    m_ermmh;
  bool    m_ermpbse;
  bool    m_ermte1;
  bool    m_ermte2;
  bool    m_ermdc;
  bool    m_ermde;
  RegData m_ichewy;
  RegData m_icheix;
  bool    m_icherq;
  bool    m_iched;
  bool    m_ichet;
  bool    m_icherr;

public:
  void SetCISTW(bool cistw) { m_cistw = cistw; }
  void SetESAFE(bool esafe) { m_esafe = esafe; }
  void SetESMH(bool esmh) { m_esmh = esmh; }
  void SetESPBSE(bool espbse) { m_espbse = espbse; }
  void SetESTE1(bool este1) { m_este1 = este1; }
  void SetESTE2(bool este2) { m_este2 = este2; }
  void SetESDC(bool esdc) { m_esdc = esdc; }
  void SetESDE(bool esde) { m_esde = esde; }
  void SetERMAFE(bool ermafe) { m_ermafe = ermafe; }
  void SetERMMH(bool ermmh) { m_ermmh = ermmh; }
  void SetERMPBSE(bool ermpbse) { m_ermpbse = ermpbse; }
  void SetERMTE1(bool ermte1) { m_ermte1 = ermte1; }
  void SetERMTE2(bool ermte2) { m_ermte2 = ermte2; }
  void SetERMDC(bool ermdc) { m_ermdc = ermdc; }
  void SetERMDE(bool ermde) { m_ermde = ermde; }
  void SetICHEWY(RegData ichewy) { m_ichewy = ichewy; }
  void SetICHEIX(RegData icheix);
  void SetICHERQ(bool icherq) { m_icherq = icherq; }
  void SetICHED(bool iched) { m_iched = iched; }
  void SetICHET(bool ichet) { m_ichet = ichet; }
  void SetICHERR(bool icherr) { m_icherr = icherr; }
  bool GetCISTW(void) const { return m_cistw; }
  bool GetESAFE(void) const { return m_esafe; }
  bool GetESMH(void) const { return m_esmh; }
  bool GetESPBSE(void) const { return m_espbse; }
  bool GetESTE1(void) const { return m_este1; }
  bool GetESTE2(void) const { return m_este2; }
  bool GetESDC(void) const { return m_esdc; }
  bool GetESDE(void) const { return m_esde; }
  bool GetERMAFE(void) const { return m_ermafe; }
  bool GetERMMH(void) const { return m_ermmh; }
  bool GetERMPBSE(void) const { return m_ermpbse; }
  bool GetERMTE1(void) const { return m_ermte1; }
  bool GetERMTE2(void) const { return m_ermte2; }
  bool GetERMDC(void) const { return m_ermdc; }
  bool GetERMDE(void) const { return m_ermde; }
  RegData GetICHEWY(void) const { return m_ichewy; }
  RegData GetICHEIX(void) const { return m_icheix; }
  bool GetICHERQ(void) const { return m_icherq; }
  bool GetICHED(void) const { return m_iched; }
  bool GetICHET(void) const { return m_ichet; }
  bool GetICHERR(void) const { return m_icherr; }

  void Reset(void) {
     m_cistw = 0x0U;
     m_esafe = 0x0U;
     m_ermafe = 0x0U;
     m_ermmh = 0x0U;
     m_ermpbse = 0x0U;
     m_ermte1 = 0x0U;
     m_ermte2 = 0x0U;
     m_ermdc = 0x0U;
     m_ermde = 0x0U;
     m_icherq = 0x0U;
     m_iched = 0x0U;
     m_ichet = 0x0U;
     m_icherr = 0x0U;
  }

  void Write(RegData data, PrivLevel priv, ContextId ctxt);

  RegData Read(PrivLevel priv, ContextId ctxt) const;

  std::string GetName(void) const { return "ICERR"; }

  CfSReg GetCfSRegId(void) const { return CF_SR_ICERR; }

  SRegICERR(ContextId ctid, ProcElementBase* pe) :
     SRegBasicImpl (ctid, PRIV_PERMIT_SV, PRIV_PIE_SV, pe),
        m_cistw (0),
        m_esafe (0),
        m_esmh (0),
        m_espbse (0),
        m_este1 (0),
        m_este2 (0),
        m_esdc (0),
        m_esde (0),
        m_ermafe (0),
        m_ermmh (0),
        m_ermpbse (0),
        m_ermte1 (0),
        m_ermte2 (0),
        m_ermdc (0),
        m_ermde (0),
        m_ichewy (0),
        m_icheix (0),
        m_icherq (0),
        m_iched (0),
        m_ichet (0),
        m_icherr (0) {}
};

/*!
  @brief ICSR register class
 */
class SRegICSR : public SRegBasicImpl, SRegICSR_IF {
private:
  bool    m_pmfp;
  bool    m_pmei;

public:
  void SetPMFP(bool pmfp) { m_pmfp = pmfp; }
  void SetPMEI(bool pmei) { m_pmei = pmei; }
  bool GetPMFP(void) const { return m_pmfp; }
  bool GetPMEI(void) const { return m_pmei; }

  void Reset(void) {
     m_pmfp = 0x0U;
     m_pmei = 0x0U;
  }

  void Write(RegData data, PrivLevel priv, ContextId ctxt) {
        if (IsPrivileged(priv) == false) return;
  }

  RegData Read(PrivLevel priv, ContextId ctxt) const {
        if (IsPrivileged (priv) == false) return 0;
     RegData ret = 0x0
            | (GetPMFP () << 1)
            | (GetPMEI () << 0);
     return ret;
  }

  std::string GetName(void) const { return "ICSR"; }

  CfSReg GetCfSRegId(void) const { return CF_SR_ICSR; }

  SRegICSR(ContextId ctid, ProcElementBase* pe) :
     SRegBasicImpl (ctid, PRIV_PERMIT_SV, PRIV_PIE_SV, pe),
        m_pmfp (0),
        m_pmei (0) {}
};

/*!
  @brief ICTAGH register class
 */
class SRegICTAGH : public SRegBasicImpl, SRegICTAGH_IF {
private:
  bool    m_wd;
  bool    m_pd;
  bool    m_wt;
  bool    m_pt;
  RegData m_dataecc;
  RegData m_tagecc;

public:
  void SetWD(bool wd) { m_wd = wd; }
  void SetPD(bool pd) { m_pd = pd; }
  void SetWT(bool wt) { m_wt = wt; }
  void SetPT(bool pt) { m_pt = pt; }
  void SetDATAECC(RegData dataecc) { m_dataecc = dataecc; }
  void SetTAGECC(RegData tagecc) { m_tagecc = tagecc; }
  bool GetWD(void) const { return m_wd; }
  bool GetPD(void) const { return m_pd; }
  bool GetWT(void) const { return m_wt; }
  bool GetPT(void) const { return m_pt; }
  RegData GetDATAECC(void) const { return m_dataecc; }
  RegData GetTAGECC(void) const { return m_tagecc; }

  void Reset(void) {
     m_wd = 0x0U;
     m_pd = 0x0U;
     m_wt = 0x0U;
     m_pt = 0x0U;
  }

  void Write(RegData data, PrivLevel priv, ContextId ctxt);

  RegData Read(PrivLevel priv, ContextId ctxt) const;

  std::string GetName(void) const { return "ICTAGH"; }

  CfSReg GetCfSRegId(void) const { return CF_SR_ICTAGH; }

  SRegICTAGH(ContextId ctid, ProcElementBase* pe) :
     SRegBasicImpl (ctid, PRIV_PERMIT_HV, PRIV_PIE_HV, pe),
        m_wd (0),
        m_pd (0),
        m_wt (0),
        m_pt (0),
        m_dataecc (0),
        m_tagecc (0) {}
};

/*!
  @brief ICTAGL register class
 */
class SRegICTAGL : public SRegBasicImpl, SRegICTAGL_IF {
private:
  RegData m_lpn;
  RegData m_lru;
  bool    m_l;
  bool    m_v;

public:
  void SetLPN(RegData lpn) { m_lpn = lpn; }
  void SetLRU(RegData lru) { m_lru = lru; }
  void SetL(bool l) { m_l = l; }
  void SetV(bool v) { m_v = v; }
  RegData GetLPN(void) const { return m_lpn; }
  RegData GetLRU(void) const { return m_lru; }
  bool GetL(void) const { return m_l; }
  bool GetV(void) const { return m_v; }

  void Reset(void) {
  }

  void Write(RegData data, PrivLevel priv, ContextId ctxt);

  RegData Read(PrivLevel priv, ContextId ctxt) const;

  std::string GetName(void) const { return "ICTAGL"; }

  CfSReg GetCfSRegId(void) const { return CF_SR_ICTAGL; }

  SRegICTAGL(ContextId ctid, ProcElementBase* pe) :
     SRegBasicImpl (ctid, PRIV_PERMIT_HV, PRIV_PIE_HV, pe),
        m_lpn (0),
        m_lru (0),
        m_l (0),
        m_v (0) {}
};

/*!
  @brief IFCR register class
 */
class SRegIFCR : public SRegBasicImpl, SRegIFCR_IF {
private:
  bool    m_plen;

public:
  void SetPLEN(bool plen) { m_plen = plen; }
  bool GetPLEN(void) const { return m_plen; }

  void Reset(void) {
     m_plen = 0x1U;
  }

  void Write(RegData data, PrivLevel priv, ContextId ctxt);

  RegData Read(PrivLevel priv, ContextId ctxt) const;

  std::string GetName(void) const { return "IFCR"; }

  CfSReg GetCfSRegId(void) const { return CF_SR_IFCR; }

  SRegIFCR(ContextId ctid, ProcElementBase* pe) :
     SRegBasicImpl (ctid, PRIV_PERMIT_SV, PRIV_PIE_SV, pe),
        m_plen (0) {}
};

/*!
  @brief IFCR1 register class
 */
class SRegIFCR1 : public SRegBasicImpl, SRegIFCR1_IF {
private:
  bool    m_fd2;
  bool    m_plintd;
  bool    m_plnlnd;

public:
  void SetFD2(bool fd2) { m_fd2 = fd2; }
  void SetPLINTD(bool plintd) { m_plintd = plintd; }
  void SetPLNLND(bool plnlnd) { m_plnlnd = plnlnd; }
  bool GetFD2(void) const { return m_fd2; }
  bool GetPLINTD(void) const { return m_plintd; }
  bool GetPLNLND(void) const { return m_plnlnd; }

  void Reset(void) {
     m_fd2 = 0x0U;
     m_plintd = 0x0U;
     m_plnlnd = 0x0U;
  }

  void Write(RegData data, PrivLevel priv, ContextId ctxt);

  RegData Read(PrivLevel priv, ContextId ctxt) const;

  std::string GetName(void) const { return "IFCR1"; }

  CfSReg GetCfSRegId(void) const { return CF_SR_IFCR1; }

  SRegIFCR1(ContextId ctid, ProcElementBase* pe) :
     SRegBasicImpl (ctid, PRIV_PERMIT_SV, PRIV_PIE_SV, pe),
        m_fd2 (0),
        m_plintd (0),
        m_plnlnd (0) {}
};

/*!
  @brief IMSR register class
 */
class SRegIMSR : public SRegBasicImpl, SRegIMSR_IF {
private:
  bool    m_hfnp;
  bool    m_henp;
  bool    m_heid;
  bool    m_heplm;
  bool    m_heeim;
  bool    m_fnp;
  bool    m_enp;
  bool    m_eid;
  bool    m_eplm;
  bool    m_eeim;

public:
  void SetHFNP(bool hfnp) { m_hfnp = hfnp; }
  void SetHENP(bool henp);
  void SetHEID(bool heid);
  void SetHEPLM(bool heplm);
  void SetHEEIM(bool heeim) { m_heeim = heeim; }
  void SetFNP(bool fnp) { m_fnp = fnp; }
  void SetENP(bool enp);
  void SetEID(bool eid);
  void SetEPLM(bool eplm);
  void SetEEIM(bool eeim) { m_eeim = eeim; }
  bool GetHFNP(void) const;
  bool GetHENP(void) const;
  bool GetHEID(void) const;
  bool GetHEPLM(void) const;
  bool GetHEEIM(void) const;
  bool GetFNP(void) const { return m_fnp; }
  bool GetENP(void) const { return m_enp; }
  bool GetEID(void) const { return m_eid; }
  bool GetEPLM(void) const { return m_eplm; }
  bool GetEEIM(void) const { return m_eeim; }

  void Reset(void) {
     m_hfnp = 0x0U;
     m_henp = 0x0U;
     m_heid = 0x0U;
     m_heplm = 0x0U;
     m_heeim = 0x0U;
     m_fnp = 0x0U;
     m_enp = 0x0U;
     m_eid = 0x0U;
     m_eplm = 0x0U;
     m_eeim = 0x0U;
  }

  void Write(RegData data, PrivLevel priv, ContextId ctxt) {
        if (IsPrivileged(priv) == false) return;
  }

  RegData Read(PrivLevel priv, ContextId ctxt) const {
        if (IsPrivileged (priv) == false) return 0;
     RegData ret = 0x0
            | (GetHFNP () << 12)
            | (GetHENP () << 11)
            | (GetHEID () << 10)
            | (GetHEPLM () << 9)
            | (GetHEEIM () << 8)
            | (GetFNP () << 4)
            | (GetENP () << 3)
            | (GetEID () << 2)
            | (GetEPLM () << 1)
            | (GetEEIM () << 0);
     return ret;
  }

  std::string GetName(void) const { return "IMSR"; }

  CfSReg GetCfSRegId(void) const { return CF_SR_IMSR; }

  SRegIMSR(ContextId ctid, ProcElementBase* pe) :
     SRegBasicImpl (ctid, PRIV_PERMIT_SV, PRIV_PIE_SV, pe),
        m_hfnp (0),
        m_henp (0),
        m_heid (0),
        m_heplm (0),
        m_heeim (0),
        m_fnp (0),
        m_enp (0),
        m_eid (0),
        m_eplm (0),
        m_eeim (0) {}
};

/*!
  @brief INTBP register class
 */
class SRegHMINTBP : public SRegBaseINTBP, SRegHMINTBP_IF {
private:
  RegData m_intbp;

public:
  void SetINTBP(RegData intbp) { m_intbp = intbp; }
  RegData GetINTBP(void) const { return m_intbp; }

  void Reset(void) {
  }

  void Write(RegData data, PrivLevel priv, ContextId ctxt) {
        if (IsPrivileged(priv) == false) return;
     SetINTBP ((data >> 9) & RegData (0x007fffffU));
  }

  RegData Read(PrivLevel priv, ContextId ctxt) const {
        if (IsPrivileged (priv) == false) return 0;
     RegData ret = 0x0
            | (GetINTBP () << 9);
     return ret;
  }

  std::string GetName(void) const { return "INTBP"; }

  CfSReg GetCfSRegId(void) const { return CF_SR_HMINTBP; }

  SRegHMINTBP(ContextId ctid, ProcElementBase* pe) :
     SRegBaseINTBP (ctid, PRIV_PERMIT_SV, PRIV_PIE_SV, pe),
        m_intbp (0) {}
};

/*!
  @brief INTCFG register class
 */
class SRegHMINTCFG : public SRegBaseINTCFG, SRegHMINTCFG_IF {
private:
  RegData m_ulnr;
  bool    m_epl;
  bool    m_ispc;

public:
  void SetULNR(RegData ulnr);
  void SetEPL(bool epl);
  void SetISPC(bool ispc);
  RegData GetULNR(void) const { return m_ulnr; }
  bool GetEPL(void) const { return m_epl; }
  bool GetISPC(void) const { return m_ispc; }

  void Reset(void);

  void Write(RegData data, PrivLevel priv, ContextId ctxt) {
        if (IsPrivileged(priv) == false) return;
     SetULNR ((data >> 16) & RegData (0x000000ffU));
     SetEPL ((data >> 1) & RegData (0x00000001U));
     SetISPC ((data >> 0) & RegData (0x00000001U));
  }

  RegData Read(PrivLevel priv, ContextId ctxt) const {
        if (IsPrivileged (priv) == false) return 0;
     RegData ret = 0x0
            | (GetULNR () << 16)
            | (GetEPL () << 1)
            | (GetISPC () << 0);
     return ret;
  }

  std::string GetName(void) const { return "INTCFG"; }

  CfSReg GetCfSRegId(void) const { return CF_SR_HMINTCFG; }

  SRegHMINTCFG(ContextId ctid, ProcElementBase* pe) :
     SRegBaseINTCFG (ctid, PRIV_PERMIT_SV, PRIV_PIE_SV, pe),
        m_ulnr (0),
        m_epl (0),
        m_ispc (0) {}
};

/*!
  @brief ISPR register class
 */
class SRegISPR : public SRegBasicImpl, SRegISPR_IF {
private:
  RegData m_isp;

public:
  void SetISP(RegData isp);
  RegData GetISP(void) const { return m_isp; }

  void Reset(void) {
     m_isp = 0x0U;
  }

  void Write(RegData data, PrivLevel priv, ContextId ctxt);

  RegData Read(PrivLevel priv, ContextId ctxt) const {
        if (IsPrivileged (priv) == false) return 0;
     RegData ret = 0x0
            | (GetISP () << 0);
     return ret;
  }

  std::string GetName(void) const { return "ISPR"; }

  CfSReg GetCfSRegId(void) const { return CF_SR_ISPR; }

  SRegISPR(ContextId ctid, ProcElementBase* pe) :
     SRegBasicImpl (ctid, PRIV_PERMIT_SV, PRIV_PIE_SV, pe),
        m_isp (0) {}
};

/*!
  @brief L1RCFG register class
 */
class SRegL1RCFG : public SRegBasicImpl, SRegL1RCFG_IF {
private:
  RegData m_l1rsize;
  RegData m_l1rlnke;

public:
  void SetL1RSIZE(RegData l1rsize) { m_l1rsize = l1rsize; }
  void SetL1RLNKE(RegData l1rlnke) { m_l1rlnke = l1rlnke; }
  RegData GetL1RSIZE(void) const;
  RegData GetL1RLNKE(void) const;

  void Reset(void) {
     m_l1rsize = 0x0U;
     m_l1rlnke = 0x0U;
  }

  void Write(RegData data, PrivLevel priv, ContextId ctxt);

  RegData Read(PrivLevel priv, ContextId ctxt) const;

  std::string GetName(void) const { return "L1RCFG"; }

  CfSReg GetCfSRegId(void) const { return CF_SR_L1RCFG; }

  SRegL1RCFG(ContextId ctid, ProcElementBase* pe) :
     SRegBasicImpl (ctid, PRIV_PERMIT_SV, PRIV_PIE_SV, pe),
        m_l1rsize (0),
        m_l1rlnke (0) {}
};

/*!
  @brief L1RLCR register class
 */
class SRegL1RLCR : public SRegBasicImpl, SRegL1RLCR_IF {
private:
  RegData m_asel;
  bool    m_clr;

public:
  void SetASEL(RegData asel) { m_asel = asel; }
  void SetCLR(bool clr);
  RegData GetASEL(void) const { return m_asel; }
  bool GetCLR(void) const { return m_clr; }

  void Reset(void) {
     m_asel = 0x0U;
     m_clr = 0x0U;
  }

  void Write(RegData data, PrivLevel priv, ContextId ctxt);

  RegData Read(PrivLevel priv, ContextId ctxt) const;

  std::string GetName(void) const { return "L1RLCR"; }

  CfSReg GetCfSRegId(void) const { return CF_SR_L1RLCR; }

  SRegL1RLCR(ContextId ctid, ProcElementBase* pe) :
     SRegBasicImpl (ctid, PRIV_PERMIT_SV, PRIV_PIE_SV, pe),
        m_asel (0),
        m_clr (0) {}
};

/*!
  @brief L1RLNK0 register class
 */
class SRegL1RLNK0 : public SRegBasicImpl, SRegL1RLNK0_IF {
private:
  RegData m_tid;
  RegData m_bmid;
  RegData m_size;
  bool    m_v;

public:
  void SetTID(RegData tid) { m_tid = tid; }
  void SetBMID(RegData bmid) { m_bmid = bmid; }
  void SetSIZE(RegData size) { m_size = size; }
  void SetV(bool v) { m_v = v; }
  RegData GetTID(void) const { return m_tid; }
  RegData GetBMID(void) const { return m_bmid; }
  RegData GetSIZE(void) const { return m_size; }
  bool GetV(void) const { return m_v; }

  void Reset(void) {
     m_tid = 0x0U;
     m_bmid = 0x0U;
     m_size = 0x0U;
     m_v = 0x0U;
  }

  void Write(RegData data, PrivLevel priv, ContextId ctxt);

  RegData Read(PrivLevel priv, ContextId ctxt) const;

  std::string GetName(void) const { return "L1RLNK0"; }

  CfSReg GetCfSRegId(void) const { return CF_SR_L1RLNK0; }

  SRegL1RLNK0(ContextId ctid, ProcElementBase* pe) :
     SRegBasicImpl (ctid, PRIV_PERMIT_SV, PRIV_PIE_SV, pe),
        m_tid (0),
        m_bmid (0),
        m_size (0),
        m_v (0) {}
};

/*!
  @brief L1RLNK1 register class
 */
class SRegL1RLNK1 : public SRegBasicImpl, SRegL1RLNK1_IF {
private:
  RegData m_addr;

public:
  void SetADDR(RegData addr) { m_addr = addr; }
  RegData GetADDR(void) const { return m_addr; }

  void Reset(void) {
     m_addr = 0x0U;
  }

  void Write(RegData data, PrivLevel priv, ContextId ctxt);

  RegData Read(PrivLevel priv, ContextId ctxt) const;

  std::string GetName(void) const { return "L1RLNK1"; }

  CfSReg GetCfSRegId(void) const { return CF_SR_L1RLNK1; }

  SRegL1RLNK1(ContextId ctid, ProcElementBase* pe) :
     SRegBasicImpl (ctid, PRIV_PERMIT_SV, PRIV_PIE_SV, pe),
        m_addr (0) {}
};

/*!
  @brief LSCFG register class
 */
class SRegLSCFG : public SRegBasicImpl, SRegLSCFG_IF {
private:
  bool    m_lse;

public:
  void SetLSE(bool lse) { m_lse = lse; }
  bool GetLSE(void) const { return m_lse; }

  void Reset(void);

  void Write(RegData data, PrivLevel priv, ContextId ctxt) {
        if (IsPrivileged(priv) == false) return;
  }

  RegData Read(PrivLevel priv, ContextId ctxt) const {
        if (IsPrivileged (priv) == false) return 0;
     RegData ret = 0x0
            | (GetLSE () << 0);
     return ret;
  }

  std::string GetName(void) const { return "LSCFG"; }

  CfSReg GetCfSRegId(void) const { return CF_SR_LSCFG; }

  SRegLSCFG(ContextId ctid, ProcElementBase* pe) :
     SRegBasicImpl (ctid, PRIV_PERMIT_SV, PRIV_PIE_SV, pe),
        m_lse (0) {}
};

/*!
  @brief LSTEST0 register class
 */
class SRegLSTEST0 : public SRegBasicImpl, SRegLSTEST0_IF {
private:
  RegData m_lstest0;

public:
  void SetLSTEST0(RegData lstest0) { m_lstest0 = lstest0; }
  RegData GetLSTEST0(void) const { return m_lstest0; }

  void Reset(void) {
     m_lstest0 = 0x0U;
  }

  void Write(RegData data, PrivLevel priv, ContextId ctxt);

  RegData Read(PrivLevel priv, ContextId ctxt) const;

  std::string GetName(void) const { return "LSTEST0"; }

  CfSReg GetCfSRegId(void) const { return CF_SR_LSTEST0; }

  SRegLSTEST0(ContextId ctid, ProcElementBase* pe) :
     SRegBasicImpl (ctid, PRIV_PERMIT_SV, PRIV_PIE_SV, pe),
        m_lstest0 (0) {}
};

/*!
  @brief LSTEST1 register class
 */
class SRegLSTEST1 : public SRegBasicImpl, SRegLSTEST1_IF {
private:
  RegData m_lstest1;

public:
  void SetLSTEST1(RegData lstest1) { m_lstest1 = lstest1; }
  RegData GetLSTEST1(void) const { return m_lstest1; }

  void Reset(void) {
     m_lstest1 = 0x0U;
  }

  void Write(RegData data, PrivLevel priv, ContextId ctxt);

  RegData Read(PrivLevel priv, ContextId ctxt) const;

  std::string GetName(void) const { return "LSTEST1"; }

  CfSReg GetCfSRegId(void) const { return CF_SR_LSTEST1; }

  SRegLSTEST1(ContextId ctid, ProcElementBase* pe) :
     SRegBasicImpl (ctid, PRIV_PERMIT_SV, PRIV_PIE_SV, pe),
        m_lstest1 (0) {}
};

/*!
  @brief LSUCR register class
 */
class SRegLSUCR : public SRegBasicImpl, SRegLSUCR_IF {
private:
  bool    m_bbopen;

public:
  void SetBBOPEN(bool bbopen) { m_bbopen = bbopen; }
  bool GetBBOPEN(void) const { return m_bbopen; }

  void Reset(void) {
     m_bbopen = 0x0U;
  }

  void Write(RegData data, PrivLevel priv, ContextId ctxt);

  RegData Read(PrivLevel priv, ContextId ctxt) const;

  std::string GetName(void) const { return "LSUCR"; }

  CfSReg GetCfSRegId(void) const { return CF_SR_LSUCR; }

  SRegLSUCR(ContextId ctid, ProcElementBase* pe) :
     SRegBasicImpl (ctid, PRIV_PERMIT_SV, PRIV_PIE_SV, pe),
        m_bbopen (0) {}
};

/*!
  @brief LSULNK0 register class
 */
class SRegLSULNK0 : public SRegBasicImpl, SRegLSULNK0_IF {
private:
  RegData m_size;
  bool    m_v;

public:
  void SetSIZE(RegData size) { m_size = size; }
  void SetV(bool v) { m_v = v; }
  RegData GetSIZE(void) const { return m_size; }
  bool GetV(void) const { return m_v; }

  void Reset(void) {
     m_size = 0x0U;
     m_v = 0x0U;
  }

  void Write(RegData data, PrivLevel priv, ContextId ctxt);

  RegData Read(PrivLevel priv, ContextId ctxt) const;

  std::string GetName(void) const { return "LSULNK0"; }

  CfSReg GetCfSRegId(void) const { return CF_SR_LSULNK0; }

  SRegLSULNK0(ContextId ctid, ProcElementBase* pe) :
     SRegBasicImpl (ctid, PRIV_PERMIT_SV, PRIV_PIE_SV, pe),
        m_size (0),
        m_v (0) {}
};

/*!
  @brief LSULNK1 register class
 */
class SRegLSULNK1 : public SRegBasicImpl, SRegLSULNK1_IF {
private:
  RegData m_addr;

public:
  void SetADDR(RegData addr) { m_addr = addr; }
  RegData GetADDR(void) const { return m_addr; }

  void Reset(void) {
     m_addr = 0x0U;
  }

  void Write(RegData data, PrivLevel priv, ContextId ctxt);

  RegData Read(PrivLevel priv, ContextId ctxt) const;

  std::string GetName(void) const { return "LSULNK1"; }

  CfSReg GetCfSRegId(void) const { return CF_SR_LSULNK1; }

  SRegLSULNK1(ContextId ctid, ProcElementBase* pe) :
     SRegBasicImpl (ctid, PRIV_PERMIT_SV, PRIV_PIE_SV, pe),
        m_addr (0) {}
};

/*!
  @brief MCA register class
 */
class SRegMCA : public SRegBasicImpl, SRegMCA_IF {
private:
  RegData m_mca;

public:
  void SetMCA(RegData mca) { m_mca = mca; }
  RegData GetMCA(void) const { return m_mca; }

  void Reset(void) {
  }

  void Write(RegData data, PrivLevel priv, ContextId ctxt) {
        if (IsPrivileged(priv) == false) return;
     SetMCA ((data >> 0) & RegData (0xffffffffU));
  }

  RegData Read(PrivLevel priv, ContextId ctxt) const {
        if (IsPrivileged (priv) == false) return 0;
     RegData ret = 0x0
            | (GetMCA () << 0);
     return ret;
  }

  std::string GetName(void) const { return "MCA"; }

  CfSReg GetCfSRegId(void) const { return CF_SR_MCA; }

  SRegMCA(ContextId ctid, ProcElementBase* pe) :
     SRegBasicImpl (ctid, PRIV_PERMIT_SV, PRIV_PIE_SV, pe),
        m_mca (0) {}
};

/*!
  @brief MCC register class
 */
class SRegMCC : public SRegBasicImpl, SRegMCC_IF {
private:
  RegData m_mcc;

public:
  void SetMCC(RegData mcc) { m_mcc = mcc; }
  RegData GetMCC(void) const { return m_mcc; }

  void Reset(void) {
     m_mcc = 0x0U;
  }

  void Write(RegData data, PrivLevel priv, ContextId ctxt);

  RegData Read(PrivLevel priv, ContextId ctxt) const {
        if (IsPrivileged (priv) == false) return 0;
     RegData ret = 0x0
            | (GetMCC () << 0);
     return ret;
  }

  std::string GetName(void) const { return "MCC"; }

  CfSReg GetCfSRegId(void) const { return CF_SR_MCC; }

  SRegMCC(ContextId ctid, ProcElementBase* pe) :
     SRegBasicImpl (ctid, PRIV_PERMIT_SV, PRIV_PIE_SV, pe),
        m_mcc (0) {}
};

/*!
  @brief MCI register class
 */
class SRegMCI : public SRegBasicImpl, SRegMCI_IF {
private:
  RegData m_spid;

public:
  void SetSPID(RegData spid) { m_spid = spid; }
  RegData GetSPID(void) const { return m_spid; }

  void Reset(void) {
  }

  void Write(RegData data, PrivLevel priv, ContextId ctxt) {
        if (IsPrivileged(priv) == false) return;
     SetSPID ((data >> 0) & RegData (0x0000001fU));
  }

  RegData Read(PrivLevel priv, ContextId ctxt) const {
        if (IsPrivileged (priv) == false) return 0;
     RegData ret = 0x0
            | (GetSPID () << 0);
     return ret;
  }

  std::string GetName(void) const { return "MCI"; }

  CfSReg GetCfSRegId(void) const { return CF_SR_MCI; }

  SRegMCI(ContextId ctid, ProcElementBase* pe) :
     SRegBasicImpl (ctid, PRIV_PERMIT_SV, PRIV_PIE_SV, pe),
        m_spid (0) {}
};

/*!
  @brief MCR register class
 */
class SRegMCR : public SRegBasicImpl, SRegMCR_IF {
private:
  bool    m_hsxe;
  bool    m_hswe;
  bool    m_hsre;
  bool    m_huxe;
  bool    m_huwe;
  bool    m_hure;
  bool    m_gsxe;
  bool    m_gswe;
  bool    m_gsre;
  bool    m_guxe;
  bool    m_guwe;
  bool    m_gure;
  bool    m_ov;
  bool    m_sxe;
  bool    m_swe;
  bool    m_sre;
  bool    m_uxe;
  bool    m_uwe;
  bool    m_ure;

public:
  void SetHSXE(bool hsxe) { m_hsxe = hsxe; }
  void SetHSWE(bool hswe) { m_hswe = hswe; }
  void SetHSRE(bool hsre) { m_hsre = hsre; }
  void SetHUXE(bool huxe) { m_huxe = huxe; }
  void SetHUWE(bool huwe) { m_huwe = huwe; }
  void SetHURE(bool hure) { m_hure = hure; }
  void SetGSXE(bool gsxe) { m_gsxe = gsxe; }
  void SetGSWE(bool gswe) { m_gswe = gswe; }
  void SetGSRE(bool gsre) { m_gsre = gsre; }
  void SetGUXE(bool guxe) { m_guxe = guxe; }
  void SetGUWE(bool guwe) { m_guwe = guwe; }
  void SetGURE(bool gure) { m_gure = gure; }
  void SetOV(bool ov) { m_ov = ov; }
  void SetSXE(bool sxe) { m_sxe = sxe; }
  void SetSWE(bool swe) { m_swe = swe; }
  void SetSRE(bool sre) { m_sre = sre; }
  void SetUXE(bool uxe) { m_uxe = uxe; }
  void SetUWE(bool uwe) { m_uwe = uwe; }
  void SetURE(bool ure) { m_ure = ure; }
  bool GetHSXE(void) const { return m_hsxe; }
  bool GetHSWE(void) const { return m_hswe; }
  bool GetHSRE(void) const { return m_hsre; }
  bool GetHUXE(void) const { return m_huxe; }
  bool GetHUWE(void) const { return m_huwe; }
  bool GetHURE(void) const { return m_hure; }
  bool GetGSXE(void) const { return m_gsxe; }
  bool GetGSWE(void) const { return m_gswe; }
  bool GetGSRE(void) const { return m_gsre; }
  bool GetGUXE(void) const { return m_guxe; }
  bool GetGUWE(void) const { return m_guwe; }
  bool GetGURE(void) const { return m_gure; }
  bool GetOV(void) const { return m_ov; }
  bool GetSXE(void) const { return m_sxe; }
  bool GetSWE(void) const { return m_swe; }
  bool GetSRE(void) const { return m_sre; }
  bool GetUXE(void) const { return m_uxe; }
  bool GetUWE(void) const { return m_uwe; }
  bool GetURE(void) const { return m_ure; }

  void Reset(void) {
  }

  void Write(RegData data, PrivLevel priv, ContextId ctxt);

  RegData Read(PrivLevel priv, ContextId ctxt) const;

  std::string GetName(void) const { return "MCR"; }

  CfSReg GetCfSRegId(void) const { return CF_SR_MCR; }

  SRegMCR(ContextId ctid, ProcElementBase* pe) :
     SRegBasicImpl (ctid, PRIV_PERMIT_SV, PRIV_PIE_SV, pe),
        m_hsxe (0),
        m_hswe (0),
        m_hsre (0),
        m_huxe (0),
        m_huwe (0),
        m_hure (0),
        m_gsxe (0),
        m_gswe (0),
        m_gsre (0),
        m_guxe (0),
        m_guwe (0),
        m_gure (0),
        m_ov (0),
        m_sxe (0),
        m_swe (0),
        m_sre (0),
        m_uxe (0),
        m_uwe (0),
        m_ure (0) {}
};

/*!
  @brief MCS register class
 */
class SRegMCS : public SRegBasicImpl, SRegMCS_IF {
private:
  RegData m_mcs;

public:
  void SetMCS(RegData mcs) { m_mcs = mcs; }
  RegData GetMCS(void) const { return m_mcs; }

  void Reset(void) {
  }

  void Write(RegData data, PrivLevel priv, ContextId ctxt) {
        if (IsPrivileged(priv) == false) return;
     SetMCS ((data >> 0) & RegData (0xffffffffU));
  }

  RegData Read(PrivLevel priv, ContextId ctxt) const {
        if (IsPrivileged (priv) == false) return 0;
     RegData ret = 0x0
            | (GetMCS () << 0);
     return ret;
  }

  std::string GetName(void) const { return "MCS"; }

  CfSReg GetCfSRegId(void) const { return CF_SR_MCS; }

  SRegMCS(ContextId ctid, ProcElementBase* pe) :
     SRegBasicImpl (ctid, PRIV_PERMIT_SV, PRIV_PIE_SV, pe),
        m_mcs (0) {}
};

/*!
  @brief MCTL register class
 */
class SRegMCTL : public SRegBasicImpl, SRegMCTL_IF {
private:
  bool    m_uic;

public:
  void SetUIC(bool uic) { m_uic = uic; }
  bool GetUIC(void) const { return m_uic; }

  void Reset(void) {
     m_uic = 0x0U;
  }

  void Write(RegData data, PrivLevel priv, ContextId ctxt) {
        if (IsPrivileged(priv) == false) return;
     SetUIC ((data >> 0) & RegData (0x00000001U));
  }

  RegData Read(PrivLevel priv, ContextId ctxt) const {
        if (IsPrivileged (priv) == false) return 0;
     RegData ret = 0x0
            | (GetUIC () << 0);
     return ret;
  }

  std::string GetName(void) const { return "MCTL"; }

  CfSReg GetCfSRegId(void) const { return CF_SR_MCTL; }

  SRegMCTL(ContextId ctid, ProcElementBase* pe) :
     SRegBasicImpl (ctid, PRIV_PERMIT_SV, PRIV_PIE_SV, pe),
        m_uic (0) {}
};

/*!
  @brief MEA register class
 */
class SRegHMMEA : public SRegBaseMEA, SRegHMMEA_IF {
private:
  RegData m_mea;

public:
  void SetMEA(RegData mea) { m_mea = mea; }
  RegData GetMEA(void) const { return m_mea; }

  void Reset(void) {
  }

  void Write(RegData data, PrivLevel priv, ContextId ctxt) {
        if (IsPrivileged(priv) == false) return;
     SetMEA ((data >> 0) & RegData (0xffffffffU));
  }

  RegData Read(PrivLevel priv, ContextId ctxt) const {
        if (IsPrivileged (priv) == false) return 0;
     RegData ret = 0x0
            | (GetMEA () << 0);
     return ret;
  }

  std::string GetName(void) const { return "MEA"; }

  CfSReg GetCfSRegId(void) const { return CF_SR_HMMEA; }

  SRegHMMEA(ContextId ctid, ProcElementBase* pe) :
     SRegBaseMEA (ctid, PRIV_PERMIT_SV, PRIV_PIE_SV, pe),
        m_mea (0) {}
};

/*!
  @brief MEI register class
 */
class SRegHMMEI : public SRegBaseMEI, SRegHMMEI_IF {
private:
  RegData m_len;
  RegData m_reg;
  RegData m_ds;
  bool    m_u;
  RegData m_itype;
  bool    m_rw;

public:
  void SetLEN(RegData len) { m_len = len; }
  void SetREG(RegData reg) { m_reg = reg; }
  void SetDS(RegData ds);
  void SetU(bool u) { m_u = u; }
  void SetITYPE(RegData itype) { m_itype = itype; }
  void SetRW(bool rw) { m_rw = rw; }
  RegData GetLEN(void) const { return m_len; }
  RegData GetREG(void) const { return m_reg; }
  RegData GetDS(void) const { return m_ds; }
  bool GetU(void) const { return m_u; }
  RegData GetITYPE(void) const { return m_itype; }
  bool GetRW(void) const { return m_rw; }

  void Reset(void) {
  }

  void Write(RegData data, PrivLevel priv, ContextId ctxt) {
        if (IsPrivileged(priv) == false) return;
     SetLEN ((data >> 28) & RegData (0x0000000fU));
     SetREG ((data >> 16) & RegData (0x0000001fU));
     SetDS ((data >> 9) & RegData (0x00000007U));
     SetU ((data >> 8) & RegData (0x00000001U));
     SetITYPE ((data >> 1) & RegData (0x0000001fU));
     SetRW ((data >> 0) & RegData (0x00000001U));
  }

  RegData Read(PrivLevel priv, ContextId ctxt) const {
        if (IsPrivileged (priv) == false) return 0;
     RegData ret = 0x0
            | (GetLEN () << 28)
            | (GetREG () << 16)
            | (GetDS () << 9)
            | (GetU () << 8)
            | (GetITYPE () << 1)
            | (GetRW () << 0);
     return ret;
  }

  std::string GetName(void) const { return "MEI"; }

  CfSReg GetCfSRegId(void) const { return CF_SR_HMMEI; }

  SRegHMMEI(ContextId ctid, ProcElementBase* pe) :
     SRegBaseMEI (ctid, PRIV_PERMIT_SV, PRIV_PIE_SV, pe),
        m_len (0),
        m_reg (0),
        m_ds (0),
        m_u (0),
        m_itype (0),
        m_rw (0) {}
};

/*!
  @brief MPAT register class
 */
class SRegMPAT : public SRegBasicImpl, SRegMPAT_IF {
private:
  RegData m_mpat;

public:
  void SetMPAT(RegData mpat) { m_mpat = mpat; }
  RegData GetMPAT(void) const { return m_mpat; }

  void Reset(void) {
  }

  void Write(RegData data, PrivLevel priv, ContextId ctxt);

  RegData Read(PrivLevel priv, ContextId ctxt) const;

  std::string GetName(void) const { return "MPAT"; }

  CfSReg GetCfSRegId(void) const { return CF_SR_MPAT; }

  SRegMPAT(ContextId ctid, ProcElementBase* pe) :
     SRegBasicImpl (ctid, PRIV_PERMIT_SV, PRIV_PIE_SV, pe),
        m_mpat (0) {}
};

/*!
  @brief MPAT_N register class
 */
class SRegMPAT_N : public SRegBasicImpl, SRegMPAT_N_IF {
private:
  RegData m_wmpid;
  RegData m_rmpid;
  bool    m_wg;
  bool    m_rg;
  bool    m_e;
  bool    m_sx;
  bool    m_sw;
  bool    m_sr;
  bool    m_ux;
  bool    m_uw;
  bool    m_ur;
  uint32_t m_id;
  uint32_t m_bank_id;

public:
  void SetWMPID(RegData wmpid) { m_wmpid = wmpid; }
  void SetRMPID(RegData rmpid) { m_rmpid = rmpid; }
  void SetWG(bool wg) { m_wg = wg; }
  void SetRG(bool rg) { m_rg = rg; }
  void SetE(bool e);
  void SetSX(bool sx) { m_sx = sx; }
  void SetSW(bool sw) { m_sw = sw; }
  void SetSR(bool sr) { m_sr = sr; }
  void SetUX(bool ux) { m_ux = ux; }
  void SetUW(bool uw) { m_uw = uw; }
  void SetUR(bool ur) { m_ur = ur; }
  RegData GetWMPID(void) const { return m_wmpid; }
  RegData GetRMPID(void) const { return m_rmpid; }
  bool GetWG(void) const { return m_wg; }
  bool GetRG(void) const { return m_rg; }
  bool GetE(void) const { return m_e; }
  bool GetSX(void) const { return m_sx; }
  bool GetSW(void) const { return m_sw; }
  bool GetSR(void) const { return m_sr; }
  bool GetUX(void) const { return m_ux; }
  bool GetUW(void) const { return m_uw; }
  bool GetUR(void) const { return m_ur; }

  void Reset(void);

  void Write(RegData data, PrivLevel priv, ContextId ctxt);

  RegData Read(PrivLevel priv, ContextId ctxt) const;

  std::string GetName(void) const {
     std::ostringstream oss;
     oss << "MPAT" << m_id;
     if (m_bank_id != 0) {
         oss << "_B" << m_bank_id;
     }
     return oss.str();
  }

  CfSReg GetCfSRegId(void) const {
      if (m_bank_id == 0) {
          return static_cast<CfSReg>(static_cast<uint32_t>(CF_SR_MPAT0) + m_id);
      } else if (m_bank_id == 1) {
          return static_cast<CfSReg>(static_cast<uint32_t>(CF_SR_MPAT0_B1) + m_id);
      } else {
          printf ("<Internal Error: Unsupported bankid in MPAT>");
          return CF_SR_DUMMY;
      }
  }

  SRegMPAT_N(ContextId ctid, ProcElementBase* pe, uint32_t id, uint32_t bank_id) :
     SRegBasicImpl (ctid, PRIV_PERMIT_SV, PRIV_PIE_SV, pe),
        m_wmpid (0),
        m_rmpid (0),
        m_wg (0),
        m_rg (0),
        m_e (0),
        m_sx (0),
        m_sw (0),
        m_sr (0),
        m_ux (0),
        m_uw (0),
        m_ur (0),
        m_id (id),
        m_bank_id (bank_id) {}
};

/*!
  @brief MPBK register class
 */
class SRegMPBK : public SRegBasicImpl, SRegMPBK_IF {
private:
  bool    m_bk;

public:
  void SetBK(bool bk);
  bool GetBK(void) const { return m_bk; }

  void Reset(void);

  void Write(RegData data, PrivLevel priv, ContextId ctxt);

  RegData Read(PrivLevel priv, ContextId ctxt) const;

  std::string GetName(void) const { return "MPBK"; }

  CfSReg GetCfSRegId(void) const { return CF_SR_MPBK; }

  SRegMPBK(ContextId ctid, ProcElementBase* pe) :
     SRegBasicImpl (ctid, PRIV_PERMIT_SV, PRIV_PIE_SV, pe),
        m_bk (0) {}
};

/*!
  @brief MPCFG register class
 */
class SRegMPCFG : public SRegBasicImpl, SRegMPCFG_IF {
private:
  bool    m_dmdp;
  RegData m_nbk;
  RegData m_arch;
  RegData m_hbe;
  RegData m_nmpue;

public:
  void SetDMDP(bool dmdp);
  void SetNBK(RegData nbk) { m_nbk = nbk; }
  void SetARCH(RegData arch) { m_arch = arch; }
  void SetHBE(RegData hbe) { m_hbe = hbe; }
  void SetNMPUE(RegData nmpue) { m_nmpue = nmpue; }
  bool GetDMDP(void) const;
  RegData GetNBK(void) const { return m_nbk; }
  RegData GetARCH(void) const { return m_arch; }
  RegData GetHBE(void) const { return m_hbe; }
  RegData GetNMPUE(void) const { return m_nmpue; }

  void Reset(void);

  void Write(RegData data, PrivLevel priv, ContextId ctxt);

  RegData Read(PrivLevel priv, ContextId ctxt) const;

  std::string GetName(void) const { return "MPCFG"; }

  CfSReg GetCfSRegId(void) const { return CF_SR_MPCFG; }

  SRegMPCFG(ContextId ctid, ProcElementBase* pe) :
     SRegBasicImpl (ctid, PRIV_PERMIT_SV, PRIV_PIE_SV, pe),
        m_dmdp (0),
        m_nbk (0),
        m_arch (0),
        m_hbe (0),
        m_nmpue (0) {}
};

/*!
  @brief MPID register class
 */
class SRegMPID : public SRegBasicImpl, SRegMPID_IF {
private:
  RegData m_spid;
  uint32_t m_id;

public:
  void SetSPID(RegData spid);
  RegData GetSPID(void) const { return m_spid; }

  void Reset(void) {
  }

  void Write(RegData data, PrivLevel priv, ContextId ctxt);

  RegData Read(PrivLevel priv, ContextId ctxt) const {
        if (IsPrivileged (priv) == false) return 0;
     RegData ret = 0x0
            | (GetSPID () << 0);
     return ret;
  }

  std::string GetName(void) const {
     std::ostringstream oss;
     oss << "MPID" << m_id;
     return oss.str();
  }

  CfSReg GetCfSRegId(void) const { return static_cast<CfSReg>(static_cast<uint32_t>(CF_SR_MPID0) + m_id); }

  SRegMPID(ContextId ctid, ProcElementBase* pe, uint32_t id) :
     SRegBasicImpl (ctid, PRIV_PERMIT_SV, PRIV_PIE_SV, pe),
        m_spid (0),
        m_id (id) {}
};

/*!
  @brief MPIDX register class
 */
class SRegMPIDX : public SRegBasicImpl, SRegMPIDX_IF {
private:
  RegData m_mpidx;

public:
  void SetMPIDX(RegData mpidx) { m_mpidx = mpidx; }
  RegData GetMPIDX(void) const { return m_mpidx; }

  void Reset(void) {
     m_mpidx = 0x0U;
  }

  void Write(RegData data, PrivLevel priv, ContextId ctxt) {
        if (IsPrivileged(priv) == false) return;
     SetMPIDX ((data >> 0) & RegData (0x0000001fU));
  }

  RegData Read(PrivLevel priv, ContextId ctxt) const {
        if (IsPrivileged (priv) == false) return 0;
     RegData ret = 0x0
            | (GetMPIDX () << 0);
     return ret;
  }

  std::string GetName(void) const { return "MPIDX"; }

  CfSReg GetCfSRegId(void) const { return CF_SR_MPIDX; }

  SRegMPIDX(ContextId ctid, ProcElementBase* pe) :
     SRegBasicImpl (ctid, PRIV_PERMIT_SV, PRIV_PIE_SV, pe),
        m_mpidx (0) {}
};

/*!
  @brief MPLA register class
 */
class SRegMPLA : public SRegBasicImpl, SRegMPLA_IF {
private:
  RegData m_mpla;

public:
  void SetMPLA(RegData mpla) { m_mpla = mpla; }
  RegData GetMPLA(void) const { return m_mpla; }

  void Reset(void) {
  }

  void Write(RegData data, PrivLevel priv, ContextId ctxt);

  RegData Read(PrivLevel priv, ContextId ctxt) const;

  std::string GetName(void) const { return "MPLA"; }

  CfSReg GetCfSRegId(void) const { return CF_SR_MPLA; }

  SRegMPLA(ContextId ctid, ProcElementBase* pe) :
     SRegBasicImpl (ctid, PRIV_PERMIT_SV, PRIV_PIE_SV, pe),
        m_mpla (0) {}
};

/*!
  @brief MPLA_N register class
 */
class SRegMPLA_N : public SRegBasicImpl, SRegMPLA_N_IF {
private:
  RegData m_mpla;
  uint32_t m_id;
  uint32_t m_bank_id;

public:
  void SetMPLA(RegData mpla) { m_mpla = mpla; }
  RegData GetMPLA(void) const { return m_mpla; }

  void Reset(void) {
  }

  void Write(RegData data, PrivLevel priv, ContextId ctxt);

  RegData Read(PrivLevel priv, ContextId ctxt) const;

  std::string GetName(void) const {
     std::ostringstream oss;
     oss << "MPLA" << m_id;
     if (m_bank_id != 0) {
         oss << "_B" << m_bank_id;
     }
     return oss.str();
  }

  CfSReg GetCfSRegId(void) const {
      if (m_bank_id == 0) {
          return static_cast<CfSReg>(static_cast<uint32_t>(CF_SR_MPLA0) + m_id);
      } else if (m_bank_id == 1) {
          return static_cast<CfSReg>(static_cast<uint32_t>(CF_SR_MPLA0_B1) + m_id);
      } else {
          printf ("<Internal Error: Unsupported bankid in MPLA>");
          return CF_SR_DUMMY;
      }
  }

  SRegMPLA_N(ContextId ctid, ProcElementBase* pe, uint32_t id, uint32_t bank_id) :
     SRegBasicImpl (ctid, PRIV_PERMIT_SV, PRIV_PIE_SV, pe),
        m_mpla (0),
        m_id (id),
        m_bank_id (bank_id) {}
};

/*!
  @brief MPM register class
 */
class SRegHMMPM : public SRegBaseMPM, SRegHMMPM_IF {
private:
  bool    m_gmpe;
  bool    m_svp;
  bool    m_mpe;

public:
  void SetGMPE(bool gmpe) { m_gmpe = gmpe; }
  void SetSVP(bool svp) { m_svp = svp; }
  void SetMPE(bool mpe);
  bool GetGMPE(void) const { return m_gmpe; }
  bool GetSVP(void) const;
  bool GetMPE(void) const;

  void Reset(void) {
     m_gmpe = 0x0U;
     m_svp = 0x0U;
     m_mpe = 0x0U;
  }

  void Write(RegData data, PrivLevel priv, ContextId ctxt);

  RegData Read(PrivLevel priv, ContextId ctxt) const {
        if (IsPrivileged (priv) == false) return 0;
     RegData ret = 0x0
            | (GetGMPE () << 2)
            | (GetSVP () << 1)
            | (GetMPE () << 0);
     return ret;
  }

  std::string GetName(void) const { return "MPM"; }

  CfSReg GetCfSRegId(void) const { return CF_SR_HMMPM; }

  SRegHMMPM(ContextId ctid, ProcElementBase* pe) :
     SRegBaseMPM (ctid, PRIV_PERMIT_SV, PRIV_PIE_SV, pe),
        m_gmpe (0),
        m_svp (0),
        m_mpe (0) {}
};

/*!
  @brief MPUA register class
 */
class SRegMPUA : public SRegBasicImpl, SRegMPUA_IF {
private:
  RegData m_mpua;

public:
  void SetMPUA(RegData mpua) { m_mpua = mpua; }
  RegData GetMPUA(void) const { return m_mpua; }

  void Reset(void) {
  }

  void Write(RegData data, PrivLevel priv, ContextId ctxt);

  RegData Read(PrivLevel priv, ContextId ctxt) const;

  std::string GetName(void) const { return "MPUA"; }

  CfSReg GetCfSRegId(void) const { return CF_SR_MPUA; }

  SRegMPUA(ContextId ctid, ProcElementBase* pe) :
     SRegBasicImpl (ctid, PRIV_PERMIT_SV, PRIV_PIE_SV, pe),
        m_mpua (0) {}
};

/*!
  @brief MPUA_N register class
 */
class SRegMPUA_N : public SRegBasicImpl, SRegMPUA_N_IF {
private:
  RegData m_mpua;
  uint32_t m_id;
  uint32_t m_bank_id;

public:
  void SetMPUA(RegData mpua) { m_mpua = mpua; }
  RegData GetMPUA(void) const { return m_mpua; }

  void Reset(void) {
  }

  void Write(RegData data, PrivLevel priv, ContextId ctxt);

  RegData Read(PrivLevel priv, ContextId ctxt) const;

  std::string GetName(void) const {
     std::ostringstream oss;
     oss << "MPUA" << m_id;
     if (m_bank_id != 0) {
         oss << "_B" << m_bank_id;
     }
     return oss.str();
  }

  CfSReg GetCfSRegId(void) const {
      if (m_bank_id == 0) {
          return static_cast<CfSReg>(static_cast<uint32_t>(CF_SR_MPUA0) + m_id);
      } else if (m_bank_id == 1) {
          return static_cast<CfSReg>(static_cast<uint32_t>(CF_SR_MPUA0_B1) + m_id);
      } else {
          printf ("<Internal Error: Unsupported bankid in MPUA>");
          return CF_SR_DUMMY;
      }
  }

  SRegMPUA_N(ContextId ctid, ProcElementBase* pe, uint32_t id, uint32_t bank_id) :
     SRegBasicImpl (ctid, PRIV_PERMIT_SV, PRIV_PIE_SV, pe),
        m_mpua (0),
        m_id (id),
        m_bank_id (bank_id) {}
};

/*!
  @brief PEID register class
 */
class SRegHMPEID : public SRegBasePEID, SRegHMPEID_IF {
private:
  RegData m_peid;

public:
  void SetPEID(RegData peid) { m_peid = peid; }
  RegData GetPEID(void) const { return m_peid; }

  void Reset(void);

  void Write(RegData data, PrivLevel priv, ContextId ctxt);

  RegData Read(PrivLevel priv, ContextId ctxt) const;

  std::string GetName(void) const { return "PEID"; }

  CfSReg GetCfSRegId(void) const { return CF_SR_HMPEID; }

  SRegHMPEID(ContextId ctid, ProcElementBase* pe) :
     SRegBasePEID (ctid, PRIV_PERMIT_UM, PRIV_PIE_UM, pe),
        m_peid (0) {}
};

/*!
  @brief PID register class
 */
class SRegPID : public SRegBasicImpl, SRegPID_IF {
private:
  RegData m_pid;
  RegData m_fid;
  RegData m_vid;

public:
  void SetPID(RegData pid) { m_pid = pid; }
  void SetFID(RegData fid) { m_fid = fid; }
  void SetVID(RegData vid) { m_vid = vid; }
  RegData GetPID(void) const { return m_pid; }
  RegData GetFID(void) const { return m_fid; }
  RegData GetVID(void) const { return m_vid; }

  void Reset(void);

  void Write(RegData data, PrivLevel priv, ContextId ctxt) {
        if (IsPrivileged(priv) == false) return;
  }

  RegData Read(PrivLevel priv, ContextId ctxt) const {
        if (IsPrivileged (priv) == false) return 0;
     RegData ret = 0x0
            | (GetPID () << 24)
            | (GetFID () << 8)
            | (GetVID () << 0);
     return ret;
  }

  std::string GetName(void) const { return "PID"; }

  CfSReg GetCfSRegId(void) const { return CF_SR_PID; }

  SRegPID(ContextId ctid, ProcElementBase* pe) :
     SRegBasicImpl (ctid, PRIV_PERMIT_SV, PRIV_PIE_SV, pe),
        m_pid (0),
        m_fid (0),
        m_vid (0) {}
};

/*!
  @brief PLMR register class
 */
class SRegHMPLMR : public SRegBasePLMR, SRegHMPLMR_IF {
private:
  RegData m_plm;

public:
  void SetPLM(RegData plm);
  RegData GetPLM(void) const { return m_plm; }

  void Reset(void) {
     m_plm = 0x10U;
  }

  void Write(RegData data, PrivLevel priv, ContextId ctxt) {
        if (IsPrivileged(priv) == false) return;
     SetPLM ((data >> 0) & RegData (0x000000ffU));
  }

  RegData Read(PrivLevel priv, ContextId ctxt) const {
        if (IsPrivileged (priv) == false) return 0;
     RegData ret = 0x0
            | (GetPLM () << 0);
     return ret;
  }

  std::string GetName(void) const { return "PLMR"; }

  CfSReg GetCfSRegId(void) const { return CF_SR_HMPLMR; }

  SRegHMPLMR(ContextId ctid, ProcElementBase* pe) :
     SRegBasePLMR (ctid, PRIV_PERMIT_SV, PRIV_PIE_SV, pe),
        m_plm (0) {}
};

/*!
  @brief PMCOUNT register class
 */
class SRegPMCOUNT : public SRegBasicImpl, SRegPMCOUNT_IF {
private:
  RegData m_pmcnt;
  uint32_t m_id;

public:
  void SetPMCNT(RegData pmcnt) { m_pmcnt = pmcnt; }
  RegData GetPMCNT(void) const { return m_pmcnt; }

  void Reset(void) {
     m_pmcnt = 0x0U;
  }

  void Write(RegData data, PrivLevel priv, ContextId ctxt);

  RegData Read(PrivLevel priv, ContextId ctxt) const;

  std::string GetName(void) const {
     std::ostringstream oss;
     oss << "PMCOUNT" << m_id;
     return oss.str();
  }

  CfSReg GetCfSRegId(void) const { return (m_pe->GetCoreVersion() >= CORE_VER_20)
                                                ? static_cast<CfSReg>(static_cast<uint32_t>(CF_SR_PMCOUNT0_G4MH20) + m_id)
                                                : static_cast<CfSReg>(static_cast<uint32_t>(CF_SR_PMCOUNT0_G4X1X) + (m_id << 1)); }

  SRegPMCOUNT(ContextId ctid, ProcElementBase* pe, uint32_t id) :
     SRegBasicImpl (ctid, PRIV_PERMIT_UM, PRIV_PIE_UM, pe),
        m_pmcnt (0),
        m_id (id) {}
};

/*!
  @brief PMCTRL register class
 */
class SRegPMCTRL : public SRegBasicImpl, SRegPMCTRL_IF {
private:
  bool    m_he;
  RegData m_ge;
  RegData m_cnd;
  bool    m_ovf;
  bool    m_cen;
  uint32_t m_id;

public:
  void SetHE(bool he) { m_he = he; }
  void SetGE(RegData ge) { m_ge = ge; }
  void SetCND(RegData cnd) { m_cnd = cnd; }
  void SetOVF(bool ovf) { m_ovf = ovf; }
  void SetCEN(bool cen) { m_cen = cen; }
  bool GetHE(void) const { return m_he; }
  RegData GetGE(void) const { return m_ge; }
  RegData GetCND(void) const { return m_cnd; }
  bool GetOVF(void) const { return m_ovf; }
  bool GetCEN(void) const { return m_cen; }

  void Reset(void) {
     m_he = 0x0U;
     m_ge = 0x0U;
     m_cnd = 0x0U;
     m_ovf = 0x0U;
     m_cen = 0x0U;
  }

  void Write(RegData data, PrivLevel priv, ContextId ctxt);

  RegData Read(PrivLevel priv, ContextId ctxt) const;

  std::string GetName(void) const {
     std::ostringstream oss;
     oss << "PMCTRL" << m_id;
     return oss.str();
  }

  CfSReg GetCfSRegId(void) const { return (m_pe->GetCoreVersion() >= CORE_VER_20)
                                                ? static_cast<CfSReg>(static_cast<uint32_t>(CF_SR_PMCTRL0_G4MH20) + m_id)
                                                : static_cast<CfSReg>(static_cast<uint32_t>(CF_SR_PMCTRL0_G4X1X) + (m_id << 1)); }

  SRegPMCTRL(ContextId ctid, ProcElementBase* pe, uint32_t id) :
     SRegBasicImpl (ctid, PRIV_PERMIT_UM, PRIV_PIE_UM, pe),
        m_he (0),
        m_ge (0),
        m_cnd (0),
        m_ovf (0),
        m_cen (0),
        m_id (id) {}
};

/*!
  @brief PMGMCTRL register class
 */
class SRegPMGMCTRL : public SRegBasicImpl, SRegPMGMCTRL_IF {
private:
  RegData m_pmge;

public:
  void SetPMGE(RegData pmge) { m_pmge = pmge; }
  RegData GetPMGE(void) const { return m_pmge; }

  void Reset(void) {
     m_pmge = 0x0U;
  }

  void Write(RegData data, PrivLevel priv, ContextId ctxt) {
        if (m_pe->IsHvEnabled() == false) return;
     SetPMGE ((data >> 0) & RegData (0x000000ffU));
  }

  RegData Read(PrivLevel priv, ContextId ctxt) const {
        if (m_pe->IsHvEnabled () == false) return 0;
     RegData ret = 0x0
            | (GetPMGE () << 0);
     return ret;
  }

  std::string GetName(void) const { return "PMGMCTRL"; }

  CfSReg GetCfSRegId(void) const { return CF_SR_PMGMCTRL; }

  SRegPMGMCTRL(ContextId ctid, ProcElementBase* pe) :
     SRegBasicImpl (ctid, PRIV_PERMIT_HV, PRIV_PIE_HV, pe),
        m_pmge (0) {}
};

/*!
  @brief PMSUBCND register class
 */
class SRegPMSUBCND : public SRegBasicImpl, SRegPMSUBCND_IF {
private:
  RegData m_subcnd;
  uint32_t m_id;

public:
  void SetSUBCND(RegData subcnd) { m_subcnd = subcnd; }
  RegData GetSUBCND(void) const { return m_subcnd; }

  void Reset(void) {
     m_subcnd = 0x0U;
  }

  void Write(RegData data, PrivLevel priv, ContextId ctxt);

  RegData Read(PrivLevel priv, ContextId ctxt) const;

  std::string GetName(void) const {
     std::ostringstream oss;
     oss << "PMSUBCND" << m_id;
     return oss.str();
  }

  CfSReg GetCfSRegId(void) const { return static_cast<CfSReg>(static_cast<uint32_t>(CF_SR_PMSUBCND0) + m_id); }

  SRegPMSUBCND(ContextId ctid, ProcElementBase* pe, uint32_t id) :
     SRegBasicImpl (ctid, PRIV_PERMIT_UM, PRIV_PIE_UM, pe),
        m_subcnd (0),
        m_id (id) {}
};

/*!
  @brief PMUMCTRL register class
 */
class SRegPMUMCTRL : public SRegBasicImpl, SRegPMUMCTRL_IF {
private:
  RegData m_pmue;

public:
  void SetPMUE(RegData pmue) { m_pmue = pmue; }
  RegData GetPMUE(void) const { return m_pmue; }

  void Reset(void) {
     m_pmue = 0x0U;
  }

  void Write(RegData data, PrivLevel priv, ContextId ctxt);

  RegData Read(PrivLevel priv, ContextId ctxt) const {
        if (IsPrivileged (priv) == false) return 0;
     RegData ret = 0x0
            | (GetPMUE () << 0);
     return ret;
  }

  std::string GetName(void) const { return "PMUMCTRL"; }

  CfSReg GetCfSRegId(void) const { return CF_SR_PMUMCTRL; }

  SRegPMUMCTRL(ContextId ctid, ProcElementBase* pe) :
     SRegBasicImpl (ctid, PRIV_PERMIT_SV, PRIV_PIE_SV, pe),
        m_pmue (0) {}
};

/*!
  @brief PSW register class
 */
class SRegHMPSW : public SRegBasePSW, SRegHMPSW_IF {
private:
  bool    m_um;
  RegData m_eimask;
  RegData m_cu;
  bool    m_ebv;
  bool    m_np;
  bool    m_ep;
  bool    m_id;
  bool    m_sat;
  bool    m_cy;
  bool    m_ov;
  bool    m_s;
  bool    m_z;

public:
  void SetUM(bool um);
  void SetEIMASK(RegData eimask);
  void SetCU(RegData cu);
  void SetEBV(bool ebv) { m_ebv = ebv; }
  void SetNP(bool np);
  void SetEP(bool ep) { m_ep = ep; }
  void SetID(bool id);
  void SetSAT(bool sat) { m_sat = sat; }
  void SetCY(bool cy) { m_cy = cy; }
  void SetOV(bool ov) { m_ov = ov; }
  void SetS(bool s) { m_s = s; }
  void SetZ(bool z) { m_z = z; }
  bool GetUM(void) const { return m_um; }
  RegData GetEIMASK(void) const;
  RegData GetCU(void) const { return m_cu; }
  bool GetEBV(void) const { return m_ebv; }
  bool GetNP(void) const { return m_np; }
  bool GetEP(void) const { return m_ep; }
  bool GetID(void) const { return m_id; }
  bool GetSAT(void) const { return m_sat; }
  bool GetCY(void) const { return m_cy; }
  bool GetOV(void) const { return m_ov; }
  bool GetS(void) const { return m_s; }
  bool GetZ(void) const { return m_z; }

  void Reset(void) {
     m_um = 0x0U;
     m_eimask = 0x0U;
     m_cu = 0x0U;
     m_ebv = 0x0U;
     m_np = 0x0U;
     m_ep = 0x0U;
     m_id = 0x1U;
     m_sat = 0x0U;
     m_cy = 0x0U;
     m_ov = 0x0U;
     m_s = 0x0U;
     m_z = 0x0U;
  }

  void Write(RegData data, PrivLevel priv, ContextId ctxt);

  RegData Read(PrivLevel priv, ContextId ctxt) const;

  std::string GetName(void) const { return "PSW"; }

  CfSReg GetCfSRegId(void) const { return CF_SR_HMPSW; }

  SRegHMPSW(ContextId ctid, ProcElementBase* pe) :
     SRegBasePSW (ctid, PRIV_PERMIT_UM, PRIV_PIE_UM, pe),
        m_um (0),
        m_eimask (0),
        m_cu (0),
        m_ebv (0),
        m_np (0),
        m_ep (0),
        m_id (0),
        m_sat (0),
        m_cy (0),
        m_ov (0),
        m_s (0),
        m_z (0) {}
};

/*!
  @brief PSWH register class
 */
class SRegPSWH : public SRegBasicImpl, SRegPSWH_IF {
private:
  bool    m_gm;
  RegData m_gpid;

public:
  void SetGM(bool gm);
  void SetGPID(RegData gpid);
  bool GetGM(void) const { return m_gm; }
  RegData GetGPID(void) const { return m_gpid; }

  void Reset(void);

  void Write(RegData data, PrivLevel priv, ContextId ctxt);

  RegData Read(PrivLevel priv, ContextId ctxt) const {
        if (m_pe->IsHvEnabled () == false) return 0;
     RegData ret = 0x0
            | (GetGM () << 31)
            | (GetGPID () << 8);
     return ret;
  }

  std::string GetName(void) const { return "PSWH"; }

  CfSReg GetCfSRegId(void) const { return CF_SR_PSWH; }

  SRegPSWH(ContextId ctid, ProcElementBase* pe) :
     SRegBasicImpl (ctid, PRIV_PERMIT_UM, PRIV_PIE_UM, pe),
        m_gm (0),
        m_gpid (0) {}
};

/*!
  @brief RBASE register class
 */
class SRegRBASE : public SRegBasicImpl, SRegRBASE_IF {
private:
  RegData m_rbase;
  bool    m_dv;
  bool    m_rint;

public:
  void SetRBASE(RegData rbase) { m_rbase = rbase; }
  void SetDV(bool dv) { m_dv = dv; }
  void SetRINT(bool rint) { m_rint = rint; }
  RegData GetRBASE(void) const { return m_rbase; }
  bool GetDV(void) const { return m_dv; }
  bool GetRINT(void) const { return m_rint; }

  void Reset(void);

  void Write(RegData data, PrivLevel priv, ContextId ctxt) {
        if (IsPrivileged(priv) == false) return;
  }

  RegData Read(PrivLevel priv, ContextId ctxt) const {
        if (IsPrivileged (priv) == false) return 0;
     RegData ret = 0x0
            | (GetRBASE () << 9)
            | (GetDV () << 1)
            | (GetRINT () << 0);
     return ret;
  }

  std::string GetName(void) const { return "RBASE"; }

  CfSReg GetCfSRegId(void) const { return CF_SR_RBASE; }

  SRegRBASE(ContextId ctid, ProcElementBase* pe) :
     SRegBasicImpl (ctid, PRIV_PERMIT_SV, PRIV_PIE_SV, pe),
        m_rbase (0),
        m_dv (0),
        m_rint (0) {}
};

/*!
  @brief RBCR0 register class
 */
class SRegRBCR0 : public SRegBasicImpl, SRegRBCR0_IF {
private:
  bool    m_md;
  RegData m_be;

public:
  void SetMD(bool md) { m_md = md; }
  void SetBE(RegData be) { m_be = be; }
  bool GetMD(void) const { return m_md; }
  RegData GetBE(void) const { return m_be; }

  void Reset(void) {
     m_md = 0x0U;
     m_be = 0x0U;
  }

  void Write(RegData data, PrivLevel priv, ContextId ctxt) {
        if (IsPrivileged(priv) == false) return;
     SetMD ((data >> 16) & RegData (0x00000001U));
     SetBE ((data >> 0) & RegData (0x0000ffffU));
  }

  RegData Read(PrivLevel priv, ContextId ctxt) const {
        if (IsPrivileged (priv) == false) return 0;
     RegData ret = 0x0
            | (GetMD () << 16)
            | (GetBE () << 0);
     return ret;
  }

  std::string GetName(void) const { return "RBCR0"; }

  CfSReg GetCfSRegId(void) const { return CF_SR_RBCR0; }

  SRegRBCR0(ContextId ctid, ProcElementBase* pe) :
     SRegBasicImpl (ctid, PRIV_PERMIT_SV, PRIV_PIE_SV, pe),
        m_md (0),
        m_be (0) {}
};

/*!
  @brief RBCR1 register class
 */
class SRegRBCR1 : public SRegBasicImpl, SRegRBCR1_IF {
private:
  RegData m_nc;

public:
  void SetNC(RegData nc) { m_nc = nc; }
  RegData GetNC(void) const { return m_nc; }

  void Reset(void) {
     m_nc = 0xFFFFU;
  }

  void Write(RegData data, PrivLevel priv, ContextId ctxt) {
        if (IsPrivileged(priv) == false) return;
     SetNC ((data >> 0) & RegData (0x0000ffffU));
  }

  RegData Read(PrivLevel priv, ContextId ctxt) const {
        if (IsPrivileged (priv) == false) return 0;
     RegData ret = 0x0
            | (GetNC () << 0);
     return ret;
  }

  std::string GetName(void) const { return "RBCR1"; }

  CfSReg GetCfSRegId(void) const { return CF_SR_RBCR1; }

  SRegRBCR1(ContextId ctid, ProcElementBase* pe) :
     SRegBasicImpl (ctid, PRIV_PERMIT_SV, PRIV_PIE_SV, pe),
        m_nc (0) {}
};

/*!
  @brief RBIP register class
 */
class SRegRBIP : public SRegBasicImpl, SRegRBIP_IF {
private:
  RegData m_rbip;

public:
  void SetRBIP(RegData rbip) { m_rbip = rbip; }
  RegData GetRBIP(void) const { return m_rbip; }

  void Reset(void) {
  }

  void Write(RegData data, PrivLevel priv, ContextId ctxt) {
        if (IsPrivileged(priv) == false) return;
     SetRBIP ((data >> 4) & RegData (0x0fffffffU));
  }

  RegData Read(PrivLevel priv, ContextId ctxt) const {
        if (IsPrivileged (priv) == false) return 0;
     RegData ret = 0x0
            | (GetRBIP () << 4);
     return ret;
  }

  std::string GetName(void) const { return "RBIP"; }

  CfSReg GetCfSRegId(void) const { return CF_SR_RBIP; }

  SRegRBIP(ContextId ctid, ProcElementBase* pe) :
     SRegBasicImpl (ctid, PRIV_PERMIT_SV, PRIV_PIE_SV, pe),
        m_rbip (0) {}
};

/*!
  @brief RBNR register class
 */
class SRegRBNR : public SRegBasicImpl, SRegRBNR_IF {
private:
  RegData m_bn;

public:
  void SetBN(RegData bn);
  RegData GetBN(void) const { return m_bn; }

  void Reset(void) {
     m_bn = 0x0U;
  }

  void Write(RegData data, PrivLevel priv, ContextId ctxt) {
        if (IsPrivileged(priv) == false) return;
     SetBN ((data >> 0) & RegData (0x000000ffU));
  }

  RegData Read(PrivLevel priv, ContextId ctxt) const {
        if (IsPrivileged (priv) == false) return 0;
     RegData ret = 0x0
            | (GetBN () << 0);
     return ret;
  }

  std::string GetName(void) const { return "RBNR"; }

  CfSReg GetCfSRegId(void) const { return CF_SR_RBNR; }

  SRegRBNR(ContextId ctid, ProcElementBase* pe) :
     SRegBasicImpl (ctid, PRIV_PERMIT_SV, PRIV_PIE_SV, pe),
        m_bn (0) {}
};

/*!
  @brief RDBACR register class
 */
class SRegRDBACR : public SRegBasicImpl, SRegRDBACR_IF {
private:
  RegData m_entry;
  bool    m_ulsel;

public:
  void SetENTRY(RegData entry) { m_entry = entry; }
  void SetULSEL(bool ulsel) { m_ulsel = ulsel; }
  RegData GetENTRY(void) const { return m_entry; }
  bool GetULSEL(void) const { return m_ulsel; }

  void Reset(void) {
     m_entry = 0x0U;
     m_ulsel = 0x0U;
  }

  void Write(RegData data, PrivLevel priv, ContextId ctxt);

  RegData Read(PrivLevel priv, ContextId ctxt) const;

  std::string GetName(void) const { return "RDBACR"; }

  CfSReg GetCfSRegId(void) const { return CF_SR_RDBACR; }

  SRegRDBACR(ContextId ctid, ProcElementBase* pe) :
     SRegBasicImpl (ctid, PRIV_PERMIT_SV, PRIV_PIE_SV, pe),
        m_entry (0),
        m_ulsel (0) {}
};

/*!
  @brief RDBADAT0 register class
 */
class SRegRDBADAT0 : public SRegBasicImpl, SRegRDBADAT0_IF {
private:
  RegData m_data;

public:
  void SetDATA(RegData data) { m_data = data; }
  RegData GetDATA(void) const { return m_data; }

  void Reset(void) {
     m_data = 0x0U;
  }

  void Write(RegData data, PrivLevel priv, ContextId ctxt);

  RegData Read(PrivLevel priv, ContextId ctxt) const;

  std::string GetName(void) const { return "RDBADAT0"; }

  CfSReg GetCfSRegId(void) const { return CF_SR_RDBADAT0; }

  SRegRDBADAT0(ContextId ctid, ProcElementBase* pe) :
     SRegBasicImpl (ctid, PRIV_PERMIT_SV, PRIV_PIE_SV, pe),
        m_data (0) {}
};

/*!
  @brief RDBADAT1 register class
 */
class SRegRDBADAT1 : public SRegBasicImpl, SRegRDBADAT1_IF {
private:
  RegData m_data;

public:
  void SetDATA(RegData data) { m_data = data; }
  RegData GetDATA(void) const { return m_data; }

  void Reset(void) {
     m_data = 0x0U;
  }

  void Write(RegData data, PrivLevel priv, ContextId ctxt);

  RegData Read(PrivLevel priv, ContextId ctxt) const;

  std::string GetName(void) const { return "RDBADAT1"; }

  CfSReg GetCfSRegId(void) const { return CF_SR_RDBADAT1; }

  SRegRDBADAT1(ContextId ctid, ProcElementBase* pe) :
     SRegBasicImpl (ctid, PRIV_PERMIT_SV, PRIV_PIE_SV, pe),
        m_data (0) {}
};

/*!
  @brief RDBADAT2 register class
 */
class SRegRDBADAT2 : public SRegBasicImpl, SRegRDBADAT2_IF {
private:
  RegData m_data;

public:
  void SetDATA(RegData data) { m_data = data; }
  RegData GetDATA(void) const { return m_data; }

  void Reset(void) {
     m_data = 0x0U;
  }

  void Write(RegData data, PrivLevel priv, ContextId ctxt);

  RegData Read(PrivLevel priv, ContextId ctxt) const;

  std::string GetName(void) const { return "RDBADAT2"; }

  CfSReg GetCfSRegId(void) const { return CF_SR_RDBADAT2; }

  SRegRDBADAT2(ContextId ctid, ProcElementBase* pe) :
     SRegBasicImpl (ctid, PRIV_PERMIT_SV, PRIV_PIE_SV, pe),
        m_data (0) {}
};

/*!
  @brief RDBADAT3 register class
 */
class SRegRDBADAT3 : public SRegBasicImpl, SRegRDBADAT3_IF {
private:
  RegData m_data;

public:
  void SetDATA(RegData data) { m_data = data; }
  RegData GetDATA(void) const { return m_data; }

  void Reset(void) {
     m_data = 0x0U;
  }

  void Write(RegData data, PrivLevel priv, ContextId ctxt);

  RegData Read(PrivLevel priv, ContextId ctxt) const;

  std::string GetName(void) const { return "RDBADAT3"; }

  CfSReg GetCfSRegId(void) const { return CF_SR_RDBADAT3; }

  SRegRDBADAT3(ContextId ctid, ProcElementBase* pe) :
     SRegBasicImpl (ctid, PRIV_PERMIT_SV, PRIV_PIE_SV, pe),
        m_data (0) {}
};

/*!
  @brief RDBATAG register class
 */
class SRegRDBATAG : public SRegBasicImpl, SRegRDBATAG_IF {
private:
  RegData m_ppn;
  bool    m_v;

public:
  void SetPPN(RegData ppn) { m_ppn = ppn; }
  void SetV(bool v) { m_v = v; }
  RegData GetPPN(void) const { return m_ppn; }
  bool GetV(void) const { return m_v; }

  void Reset(void) {
  }

  void Write(RegData data, PrivLevel priv, ContextId ctxt);

  RegData Read(PrivLevel priv, ContextId ctxt) const;

  std::string GetName(void) const { return "RDBATAG"; }

  CfSReg GetCfSRegId(void) const { return CF_SR_RDBATAG; }

  SRegRDBATAG(ContextId ctid, ProcElementBase* pe) :
     SRegBasicImpl (ctid, PRIV_PERMIT_SV, PRIV_PIE_SV, pe),
        m_ppn (0),
        m_v (0) {}
};

/*!
  @brief RDBCR register class
 */
class SRegRDBCR : public SRegBasicImpl, SRegRDBCR_IF {
private:
  bool    m_rdbdbe;
  bool    m_tpen;
  bool    m_apen;
  bool    m_rdbclr;
  bool    m_rdben;

public:
  void SetRDBDBE(bool rdbdbe) { m_rdbdbe = rdbdbe; }
  void SetTPEN(bool tpen) { m_tpen = tpen; }
  void SetAPEN(bool apen) { m_apen = apen; }
  void SetRDBCLR(bool rdbclr);
  void SetRDBEN(bool rdben) { m_rdben = rdben; }
  bool GetRDBDBE(void) const { return m_rdbdbe; }
  bool GetTPEN(void) const { return m_tpen; }
  bool GetAPEN(void) const { return m_apen; }
  bool GetRDBCLR(void) const { return m_rdbclr; }
  bool GetRDBEN(void) const { return m_rdben; }

  void Reset(void) {
     m_rdbdbe = 0x0U;
     m_tpen = 0x1U;
     m_apen = 0x1U;
     m_rdbclr = 0x0U;
     m_rdben = 0x1U;
  }

  void Write(RegData data, PrivLevel priv, ContextId ctxt);

  RegData Read(PrivLevel priv, ContextId ctxt) const;

  std::string GetName(void) const { return "RDBCR"; }

  CfSReg GetCfSRegId(void) const { return CF_SR_RDBCR; }

  SRegRDBCR(ContextId ctid, ProcElementBase* pe) :
     SRegBasicImpl (ctid, PRIV_PERMIT_SV, PRIV_PIE_SV, pe),
        m_rdbdbe (0),
        m_tpen (0),
        m_apen (0),
        m_rdbclr (0),
        m_rdben (0) {}
};

/*!
  @brief RDBSTAT register class
 */
class SRegRDBSTAT : public SRegBasicImpl, SRegRDBSTAT_IF {
private:
  RegData m_lru;

public:
  void SetLRU(RegData lru) { m_lru = lru; }
  RegData GetLRU(void) const { return m_lru; }

  void Reset(void) {
     m_lru = 0x0U;
  }

  void Write(RegData data, PrivLevel priv, ContextId ctxt);

  RegData Read(PrivLevel priv, ContextId ctxt) const;

  std::string GetName(void) const { return "RDBSTAT"; }

  CfSReg GetCfSRegId(void) const { return CF_SR_RDBSTAT; }

  SRegRDBSTAT(ContextId ctid, ProcElementBase* pe) :
     SRegBasicImpl (ctid, PRIV_PERMIT_SV, PRIV_PIE_SV, pe),
        m_lru (0) {}
};

/*!
  @brief SCBP register class
 */
class SRegSCBP : public SRegBasicImpl, SRegSCBP_IF {
private:
  RegData m_scbp;

public:
  void SetSCBP(RegData scbp) { m_scbp = scbp; }
  RegData GetSCBP(void) const { return m_scbp; }

  void Reset(void) {
     m_scbp = 0x0U;
  }

  void Write(RegData data, PrivLevel priv, ContextId ctxt) {
        if (IsPrivileged(priv) == false) return;
     SetSCBP ((data >> 2) & RegData (0x3fffffffU));
  }

  RegData Read(PrivLevel priv, ContextId ctxt) const {
        if (IsPrivileged (priv) == false) return 0;
     RegData ret = 0x0
            | (GetSCBP () << 2);
     return ret;
  }

  std::string GetName(void) const { return "SCBP"; }

  CfSReg GetCfSRegId(void) const { return CF_SR_SCBP; }

  SRegSCBP(ContextId ctid, ProcElementBase* pe) :
     SRegBasicImpl (ctid, PRIV_PERMIT_SV, PRIV_PIE_SV, pe),
        m_scbp (0) {}
};

/*!
  @brief SCCFG register class
 */
class SRegSCCFG : public SRegBasicImpl, SRegSCCFG_IF {
private:
  RegData m_size;

public:
  void SetSIZE(RegData size) { m_size = size; }
  RegData GetSIZE(void) const { return m_size; }

  void Reset(void) {
     m_size = 0x0U;
  }

  void Write(RegData data, PrivLevel priv, ContextId ctxt) {
        if (IsPrivileged(priv) == false) return;
     SetSIZE ((data >> 0) & RegData (0x000000ffU));
  }

  RegData Read(PrivLevel priv, ContextId ctxt) const {
        if (IsPrivileged (priv) == false) return 0;
     RegData ret = 0x0
            | (GetSIZE () << 0);
     return ret;
  }

  std::string GetName(void) const { return "SCCFG"; }

  CfSReg GetCfSRegId(void) const { return CF_SR_SCCFG; }

  SRegSCCFG(ContextId ctid, ProcElementBase* pe) :
     SRegBasicImpl (ctid, PRIV_PERMIT_SV, PRIV_PIE_SV, pe),
        m_size (0) {}
};

/*!
  @brief SNZCFG register class
 */
class SRegSNZCFG : public SRegBasicImpl, SRegSNZCFG_IF {
private:
  RegData m_snzcnt;

public:
  void SetSNZCNT(RegData snzcnt) { m_snzcnt = snzcnt; }
  RegData GetSNZCNT(void) const { return m_snzcnt; }

  void Reset(void);

  void Write(RegData data, PrivLevel priv, ContextId ctxt);

  RegData Read(PrivLevel priv, ContextId ctxt) const;

  std::string GetName(void) const { return "SNZCFG"; }

  CfSReg GetCfSRegId(void) const { return CF_SR_SNZCFG; }

  SRegSNZCFG(ContextId ctid, ProcElementBase* pe) :
     SRegBasicImpl (ctid, PRIV_PERMIT_HV, PRIV_PIE_HV, pe),
        m_snzcnt (0) {}
};

/*!
  @brief SPID register class
 */
class SRegHMSPID : public SRegBaseSPID, SRegHMSPID_IF {
private:
  RegData m_spid;

public:
  void SetSPID(RegData spid);
  RegData GetSPID(void) const { return m_spid; }

  void Reset(void);

  void Write(RegData data, PrivLevel priv, ContextId ctxt);

  RegData Read(PrivLevel priv, ContextId ctxt) const {
        if (IsPrivileged (priv) == false) return 0;
     RegData ret = 0x0
            | (GetSPID () << 0);
     return ret;
  }

  std::string GetName(void) const { return "SPID"; }

  CfSReg GetCfSRegId(void) const { return CF_SR_HMSPID; }

  SRegHMSPID(ContextId ctid, ProcElementBase* pe) :
     SRegBaseSPID (ctid, PRIV_PERMIT_SV, PRIV_PIE_SV, pe),
        m_spid (0) {}
};

/*!
  @brief SPIDLIST register class
 */
class SRegHMSPIDLIST : public SRegBaseSPIDLIST, SRegHMSPIDLIST_IF {
private:
  RegData m_spidlist;

public:
  void SetSPIDLIST(RegData spidlist) { m_spidlist = spidlist; }
  RegData GetSPIDLIST(void) const { return m_spidlist; }

  void Reset(void);

  void Write(RegData data, PrivLevel priv, ContextId ctxt) {
        if (IsPrivileged(priv) == false) return;
  }

  RegData Read(PrivLevel priv, ContextId ctxt) const {
        if (IsPrivileged (priv) == false) return 0;
     RegData ret = 0x0
            | (GetSPIDLIST () << 0);
     return ret;
  }

  std::string GetName(void) const { return "SPIDLIST"; }

  CfSReg GetCfSRegId(void) const { return CF_SR_HMSPIDLIST; }

  SRegHMSPIDLIST(ContextId ctid, ProcElementBase* pe) :
     SRegBaseSPIDLIST (ctid, PRIV_PERMIT_SV, PRIV_PIE_SV, pe),
        m_spidlist (0) {}
};

/*!
  @brief SVLOCK register class
 */
class SRegHMSVLOCK : public SRegBaseSVLOCK, SRegHMSVLOCK_IF {
private:
  bool    m_svl;

public:
  void SetSVL(bool svl) { m_svl = svl; }
  bool GetSVL(void) const { return m_svl; }

  void Reset(void) {
     m_svl = 0x0U;
  }

  void Write(RegData data, PrivLevel priv, ContextId ctxt) {
        if (IsPrivileged(priv) == false) return;
     SetSVL ((data >> 0) & RegData (0x00000001U));
  }

  RegData Read(PrivLevel priv, ContextId ctxt) const {
        if (IsPrivileged (priv) == false) return 0;
     RegData ret = 0x0
            | (GetSVL () << 0);
     return ret;
  }

  std::string GetName(void) const { return "SVLOCK"; }

  CfSReg GetCfSRegId(void) const { return CF_SR_HMSVLOCK; }

  SRegHMSVLOCK(ContextId ctid, ProcElementBase* pe) :
     SRegBaseSVLOCK (ctid, PRIV_PERMIT_SV, PRIV_PIE_SV, pe),
        m_svl (0) {}
};

/*!
  @brief TSCOUNTH register class
 */
class SRegTSCOUNTH : public SRegBasicImpl, SRegTSCOUNTH_IF {
private:
  RegData m_tscnth;

public:
  void SetTSCNTH(RegData tscnth) { m_tscnth = tscnth; }
  RegData GetTSCNTH(void) const { return m_tscnth; }

  void Reset(void) {
     m_tscnth = 0x0U;
  }

  void Write(RegData data, PrivLevel priv, ContextId ctxt);

  RegData Read(PrivLevel priv, ContextId ctxt) const;

  std::string GetName(void) const { return "TSCOUNTH"; }

  CfSReg GetCfSRegId(void) const { return CF_SR_TSCOUNTH; }

  SRegTSCOUNTH(ContextId ctid, ProcElementBase* pe) :
     SRegBasicImpl (ctid, PRIV_PERMIT_UM, PRIV_PIE_UM, pe),
        m_tscnth (0) {}
};

/*!
  @brief TSCOUNTL register class
 */
class SRegTSCOUNTL : public SRegBasicImpl, SRegTSCOUNTL_IF {
private:
  RegData m_tscntl;

public:
  void SetTSCNTL(RegData tscntl) { m_tscntl = tscntl; }
  RegData GetTSCNTL(void) const { return m_tscntl; }

  void Reset(void) {
     m_tscntl = 0x0U;
  }

  void Write(RegData data, PrivLevel priv, ContextId ctxt);

  RegData Read(PrivLevel priv, ContextId ctxt) const;

  std::string GetName(void) const { return "TSCOUNTL"; }

  CfSReg GetCfSRegId(void) const { return CF_SR_TSCOUNTL; }

  SRegTSCOUNTL(ContextId ctid, ProcElementBase* pe) :
     SRegBasicImpl (ctid, PRIV_PERMIT_UM, PRIV_PIE_UM, pe),
        m_tscntl (0) {}
};

/*!
  @brief TSCTRL register class
 */
class SRegTSCTRL : public SRegBasicImpl, SRegTSCTRL_IF {
private:
  bool    m_ovf;
  bool    m_cen;

public:
  void SetOVF(bool ovf) { m_ovf = ovf; }
  void SetCEN(bool cen) { m_cen = cen; }
  bool GetOVF(void) const { return m_ovf; }
  bool GetCEN(void) const { return m_cen; }

  void Reset(void) {
     m_ovf = 0x0U;
     m_cen = 0x0U;
  }

  void Write(RegData data, PrivLevel priv, ContextId ctxt);

  RegData Read(PrivLevel priv, ContextId ctxt) const;

  std::string GetName(void) const { return "TSCTRL"; }

  CfSReg GetCfSRegId(void) const { return CF_SR_TSCTRL; }

  SRegTSCTRL(ContextId ctid, ProcElementBase* pe) :
     SRegBasicImpl (ctid, PRIV_PERMIT_SV, PRIV_PIE_SV, pe),
        m_ovf (0),
        m_cen (0) {}
};

/*!
   DUMMY class
 */
class SRegDUMMY : public SRegBasicImpl {
public:
  void Reset(void) {}
  void Write(RegData data, PrivLevel priv, ContextId ctxt) {
    CfMsg::DPrint (MSG_WAR,
                   "--:--:--:-: <Warning: invalid SR access : write ignored>\n");
  }
  RegData Read(PrivLevel priv, ContextId ctxt) const {
    CfMsg::DPrint (MSG_WAR,
                   "--:--:--:-: <Warning: invalid SR access : read 0x0>\n");
    return 0x0;
  }
  std::string GetName(void) const { return "invalid SR"; }
  CfSReg GetCfSRegId(void) const { return CF_SR_DUMMY; };
  SRegDUMMY() : SRegBasicImpl(0, PRIV_PERMIT_SV, PRIV_PIE_SV, NULL) {}
};
/*!
   DUMMY class (Cause PIE always for direct access to GMxxx registers on Guest mode)
 */
class SRegDUMMYHV : public SRegBasicImpl {
public:
  void Reset (void) {}
  void Write (RegData data, PrivLevel priv, ContextId ctxt);
  RegData Read (PrivLevel priv, ContextId ctxt) const;
  std::string GetName (void) const { return "GMxxx SR"; }
  CfSReg GetCfSRegId (void) const { return CF_SR_DUMMY; };
  explicit SRegDUMMYHV (ProcElementBase* pe) : SRegBasicImpl (0, PRIV_PERMIT_HV, PRIV_PIE_HV, pe) {}
};
