// ---------------------------------------------------------------------
// $Id: ATU5010_TIMERB_Func.h,v 1.1 2017/07/04 10:15:45 binhnguyen Exp $
//
// Copyright(c) 2016 Renesas Electronics Corporation
// Copyright(c) 2016 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------
#ifndef __ATU5010_TIMERB_FUNC_H__
#define __ATU5010_TIMERB_FUNC_H__

#include "atu5010_timerb_regif.h"
#include "OSCI2.h"

class ATU5010_TIMERB;
class ATU5010_TIMERB_Func: public Catu5010_timerb_regif
{
friend class ATU5010_TIMERB;
public:
    ATU5010_TIMERB_Func(std::string name, ATU5010_TIMERB *ATU5010_TIB_ptr);
    ~ATU5010_TIMERB_Func (void);

    void read (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug);
    void write (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug);
    unsigned int GetCLKBusSel (void);

private:
    ATU5010_TIMERB                     *mATU5010_TIB;
    std::string                     mModuleName;

    void PreReadCall (const unsigned int addr);
    void RegisterHandler (const std::vector<std::string> &args);
    void EnableReset (const bool is_active);

    unsigned int GetChannelSel (void);
    unsigned int GetTCNTB0 (const unsigned int block_num);
    unsigned int GetTCNTB1 (const unsigned int block_num);
    unsigned int GetTCNTB2 (void);
    unsigned int GetTCNTB3 (void);
    unsigned int GetTCNTB4 (void);
    unsigned int GetTCNTB5 (void);
    unsigned int GetTCCLRB (void);
    unsigned int GetOCRB1 (const unsigned int block_num);
    unsigned int GetOCRB10 (const unsigned int block_num);
    unsigned int GetOCRB11 (const unsigned int block_num);
    unsigned int GetOCRB12 (const unsigned int block_num);
    unsigned int GetICRB0 (const unsigned int block_num);
    unsigned int GetICRB1 (const unsigned int block_num);
    unsigned int GetOCRB20_43 (const unsigned int state);
    unsigned int GetTEPCFB (const unsigned int state);
    unsigned int GetPIM (void);
    unsigned int GetPIM1 (void);
    unsigned int GetRLDB (void);
    unsigned int GetLDVal (void);
    unsigned int GetTICRBVal (void);
    unsigned int GetTIORBVal (void);
    bool GetTSEQNENB (const unsigned int state);
    bool GetFON (const unsigned int state);
    bool GetWriteCondition (const unsigned int addr);
    bool GetTimerStatus (const unsigned int interrupt_id);
    bool IsInterruptEnable (const std::string bit_name);
    bool IsEventInputEnable (void);
    bool IsLDEN (void);
    double CalCycleCorrection (const unsigned int state);
    void SetTCNTB0 (const unsigned int block_num, const unsigned int value);
    void SetTCNTB1 (const unsigned int block_num, const bool clear_trig, const bool clrb1sel);
    void SetTCNTB2 (const unsigned int value);
    void SetTCNTB3 (const bool clear_trig, const unsigned int clrb3sel);
    bool SetTCNTB4 (void);
    void SetTCNTB5 (const unsigned int value);
    void SetTCNTB6 (const bool clear_trig);
    void SetICRB0 (const unsigned int block_num, const bool agck_trig, const unsigned int value);
    void SetICRB1 (const unsigned int block_num, const bool clear_trig, const bool agck_trig, const unsigned int value);
    void SetICRB2 (const unsigned int block_num, const unsigned int value);
    void SetICRB3 (const unsigned int event_id, const unsigned int value);
    void SetTSEQRB (const unsigned int state);
    unsigned int GetTSEQRB (void);
    void SetTEPCVALRB0 (const unsigned int state);
    void UpdateTimerStatus (const unsigned int interrupt_id);
    void SetRLDB (const unsigned int value);
    void SetTCCLFB (const unsigned int value, const bool clear_trig);
    void UpdateRBURB (void);
    
    bool mIsCMFB6M;

    void cb_TCRB_CKSELB(RegCBstr str);
    void cb_TIORB_LDSEL(RegCBstr str);
    void cb_TSCRB_CMFCB0(RegCBstr str);
    void cb_TICRB_IREGB6(RegCBstr str);
    void cb_TCNTB0S1_CNTB0S1(RegCBstr str);
    void cb_OCRB0S1_OCB0S1(RegCBstr str);
    void cb_TCNTB0S2_CNTB0S2(RegCBstr str);
    void cb_OCRB0S2_OCB0S2(RegCBstr str);
    void cb_TCNTB0_CNTB0(RegCBstr str);
    void cb_OCRB0_OCB0(RegCBstr str);
    void cb_OCRB_OCB(RegCBstr str);
    void cb_TCNTB2_CNTB2(RegCBstr str);
    void cb_PIMR1_PIM1(RegCBstr str);
    void cb_TSEQCRB_EVSEQENB(RegCBstr str);
    void cb_TEPCFB_CRFIT2(RegCBstr str);
    void cb_TCNTB4_CNTB4(RegCBstr str);
    void cb_TCNTB5_CNTB5(RegCBstr str);
    void cb_TCCLFSRB_TCCLFSB(RegCBstr str);
    void cb_TCCLFCRB_TCCLFCB(RegCBstr str);
    void cb_TCCLRB_CCLRB(RegCBstr str);
};
#endif
