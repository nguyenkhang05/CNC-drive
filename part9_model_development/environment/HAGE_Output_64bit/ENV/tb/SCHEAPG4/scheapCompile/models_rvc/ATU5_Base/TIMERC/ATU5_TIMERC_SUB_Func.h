// ---------------------------------------------------------------------
// $Id: ATU5_TIMERC_SUB_Func.h 1196 2019-10-17 05:33:04Z chuonghoangle $
//
// Copyright(c) 2018-2019 Renesas Electronics Corporation
// Copyright(c) 2018-2019 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------
#ifndef __ATU5_TIMERC_SUB_FUNC_H__
#define __ATU5_TIMERC_SUB_FUNC_H__
#include "atu5_timerc_sub_regif.h"
#include "OSCI2.h"


class ATU5_TIMERC_SUB_AgentController;
class ATU5_TIMERC_SUB_Func: public Catu5_timerc_sub_regif
{
public:
    ATU5_TIMERC_SUB_Func (std::string name, 
                ATU5_TIMERC_SUB_AgentController *TIMERC_SUBAgentPtr, unsigned int sub_id);
	
    ~ATU5_TIMERC_SUB_Func (void);
    void RegisterHandler (const std::vector<std::string>& args);
    void EnableReset (const bool is_active);
    void TurnOVFFlag(void);
    void TurnGRCFlag(const unsigned int channel, const unsigned int value);
    void TurnOCRCFlag(const unsigned int channel, const unsigned int value);
    void SetNCNTC(const unsigned int channel,const unsigned int value);
    bool IsInterruptOVF(void);
    bool IsInterruptGRC(const unsigned int channel);
    bool IsInterruptOCRC(const unsigned int channel);
    void SetCounterStatus (const bool flag);
    unsigned int GetNCNTC(const unsigned int channel);
    unsigned int GetTCNTC(void);
    unsigned int GetCLKBusSel(void);
    void UpdateGRCReg(const unsigned int cap_value, const unsigned int channel);
    void UpdateOCRCReg(const unsigned int cap_value, const unsigned int channel);
    void SetTCNTC(const unsigned int value);
    unsigned int GetCUCRC(void);

    void read (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug);
    void write (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug);
    bool TimingClearCounter(void);
private:
    ATU5_TIMERC_SUB_AgentController  *mTIMERC_SUBAgent;
    std::string  mModuleName;
    unsigned int mSub_id;
    unsigned int mPre_CntClk;
    unsigned int mPre_PWMbit;
    bool mPre_TCRC_CLRC;
    bool mIsCounterWorking;
    void CheckPWM(void);
    void Callback_TSCRC(void);
    void Callback_TCRC(void);
    void Callback_GRC(unsigned int channel);
    void Callback_OCRC(unsigned int channel);
    void Callback_NCNTCn(unsigned int channel);
    void Callback_NCRCn(unsigned int channel);
    void SetModeChannel(const unsigned int channel, const unsigned int value);
    void SetModeInRunning(const bool mode_ch0, const bool mode_ch1, const bool mode_ch2, const bool mode_ch3);

    //cb function
    void cb_TIORC_IOC3(RegCBstr str);
    void cb_TCNTC_CNTC(RegCBstr str);
	

    //virtual function
};
#endif
