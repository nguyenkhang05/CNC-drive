// ---------------------------------------------------------------------
// $Id: ATU5010_TIMERE_SUB_Func.h,v 1.1 2018/01/26 10:15:45 binhnguyen Exp $
//
// Copyright(c) 2016-2018 Renesas Electronics Corporation
// Copyright(c) 2016-2018 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------
#ifndef __ATU5010_TIMERE_SUB_FUNC_H__
#define __ATU5010_TIMERE_SUB_FUNC_H__
#include "atu5010_timere_sub_regif.h"
#include "ATU5010_TIMERE_SUB_AgentController.h"

class ATU5010_TIMERE_SUB_Func: public Catu5010_timere_sub_regif
{
friend class ATU5010_TIMERE_SUB;
public:
  ATU5010_TIMERE_SUB_Func (std::string name, 
              ATU5010_TIMERE_SUB_AgentController *mTIMERE_SUBAgentPtr, unsigned int subnum);
  ~ATU5010_TIMERE_SUB_Func (void);

  //Virtual functions of BusSlaveFuncIf class
  void RegisterHandler (const std::vector<std::string>& args);
  void read (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug);
  void write (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug);
  bool mIsCounterWorking;
	void UpdateDTREReg(unsigned int channel);
	void UpdateCYLREReg(unsigned int channel);
	bool IsInterruptCMEE(const unsigned int channel);
	bool IsInterruptDMEE(const unsigned int channel);
	bool IsReloadEnable(const unsigned int channel);
	bool IsShutOffEnable(unsigned int channel);
	bool GetShutOffActiveInputLevel();
	bool GetShutOffOutputLevel(unsigned int channel);
	bool CheckTOEInversionSetting(unsigned int channel);
	unsigned int GetTCNTERegValue(unsigned int channel);
	void SetTCNTERegValue(unsigned int channel,unsigned int value);
	void TurnCMFEFlag(unsigned int channel);
	void TurnOVFEFlag(unsigned int channel);
	void TurnDMFEFlag(unsigned int channel);
	unsigned int GetChannelClockRatio(unsigned int channel);
	unsigned int GetSubBlockClockRatio(void);

private:

  ATU5010_TIMERE_SUB_AgentController  *mTIMERE_SUBAgent;
  std::string  mModuleName;

	void cb_DTRE_DTRE(RegCBstr str);
	void cb_CYLRE_CYLRE(RegCBstr str);
	void cb_SSTRE_SSTRE0(RegCBstr str);
	void cb_RLDCRE_RLDENE0(RegCBstr str);
	void cb_FCTRGE_FCMT0(RegCBstr str);
  void cb_POECRE_POECRKEY(RegCBstr str);
  void cb_TCNTE_TCNTE(RegCBstr str);
  void cb_TSCRE_CMFCE0(RegCBstr str);
  void cb_TOCRE_TONEE0(RegCBstr str);
  void cb_TCRE_CKSELE(RegCBstr str);
  void cb_PSCRE_PSCE(RegCBstr str);
  void cb_PSCCRE_PSCCE(RegCBstr str);
  void EnableReset (const bool is_active);
  unsigned int getDRLDEValue(unsigned int channel);      //Get value of DRLDE register, update TSRE[DMFE] or not 

};
#endif
