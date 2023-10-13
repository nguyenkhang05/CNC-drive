// ---------------------------------------------------------------------
// $Id: ATU5010_TIMERD_SUB_Func.h,v 1.0 2016/11/24 10:15:45 binhnguyen Exp $
//
// Copyright(c); 2016 Renesas Electronics Corporation
// Copyright(c); 2016 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------
#ifndef __ATU5010_TIMERD_SUB_FUNC_H__
#define __ATU5010_TIMERD_SUB_FUNC_H__
#include "atu5010_timerd_sub_regif.h"
#include "ATU5010_TIMERD_SUB_AgentController.h"

class ATU5010_TIMERD_SUB_Func: public Catu5010_timerd_sub_regif
{
public:
    ATU5010_TIMERD_SUB_Func (std::string name,
                ATU5010_TIMERD_SUB_AgentController *mTIMERD_SUBAgentPtr);;
    ~ATU5010_TIMERD_SUB_Func (void);;
     void EnableReset (const bool is_active);;

     void Set_CAPD(const std::string name, const unsigned int ch_id);
     void TurnOVF1Flag(void);
     void TurnOVF2Flag(void);
     void TurnCMFBFlag(unsigned int channel);
     void TurnCMFAFlag(unsigned int channel);
     void SetTCNT1DRegVal(unsigned int val);
     void SetTCNT2DRegVal(unsigned int val);
     void SetOSBRDRegVal(unsigned int val);
     unsigned int GetTCNT1DRegValue(void);
     unsigned int GetTCNT2DRegValue(void);
     void SetOFMICNTDRegValue(unsigned int channel, unsigned int val);
     void SetONMICNTDRegValue(unsigned int channel, unsigned int val);
     void SetOTONMICNTDRegValue(unsigned int channel, unsigned int val);
     unsigned int GetOFMINDRegValue(unsigned int channel);
     unsigned int GetONMINDRegValue(unsigned int channel);
     unsigned int GetOTOMINDRegValue(unsigned int channel);
     unsigned int GetOFMICNTDRegValue(unsigned int channel);
     unsigned int GetONMICNTDRegValue(unsigned int channel);
     unsigned int GetOTONMICNTDRegValue(unsigned int channel);
     void SetICR1DVal(unsigned int channel, unsigned int val);
     void SetICR2DVal(unsigned int channel, unsigned int val);
     void SetOCR1DVal(unsigned int channel, unsigned int val);
     void SetOCR2DVal(unsigned int channel, unsigned int val);

     unsigned int GetTIDSELDVal(void);
     void cb_TCRD_OBRED(RegCBstr str);
     void cb_TCCRLRD_CLRSEL2D(RegCBstr str);
     void cb_TIER1D_OV2ED(RegCBstr str);
     void cb_TIER2D_UNDED3(RegCBstr str);
     void cb_TIER3D_FEIED3(RegCBstr str);
     void cb_OCR1D_OC1D(RegCBstr str);
     void cb_OCR2D_OC2D(RegCBstr str);
     void cb_TCMPED_CMPE2D3(RegCBstr str);
     void cb_TIOR1D_OSSD3(RegCBstr str);
     void cb_TIOR2D_IOBD3(RegCBstr str);
     void cb_OSELRD_OSELBD3(RegCBstr str);
     void cb_ODRD_ODBD3(RegCBstr str);
     void cb_TOCRD_TONEBD(RegCBstr str);
     void cb_TOCCRD_TONEBD3(RegCBstr str);
     void cb_TSCRD_OVFC2D(RegCBstr str);
     void cb_TIORD_IO2D3(RegCBstr str);
     void cb_RCR1D_RC1D(RegCBstr str);
     void cb_RCR2D_RC2D(RegCBstr str);
     void cb_DSCRD_DWRFCD0 (RegCBstr str);
     void cb_CUCR1D_CUCR1D (RegCBstr str);
     void cb_CUCR2D_CUCR2D (RegCBstr str);
     void cb_TCNT1D_TCNT1D (RegCBstr str);
     void cb_TCNT2D_TCNT2D (RegCBstr str);
     void cb_OFMICNTD_OFMICNTD (RegCBstr str);
     void cb_ONMICNTD_ONMICNTD (RegCBstr str);
     void cb_OTONMICNTD_OTONMICNTD (RegCBstr str);
     void cb_TSCR2D_FEFCD3 (RegCBstr str);
     unsigned int GetDcntTrigger(const unsigned int ch_id);
     void Set_TSR2D_FEFD (const unsigned int ch_id, unsigned int value);
     void Set_TSR2D_REFD (const unsigned int ch_id, unsigned int value);
     void Set_DSR1D_DSFD (const unsigned int ch_id, unsigned int value);
     void Set_DSR1D_DWFD (const unsigned int ch_id, unsigned int value);
     void Set_DSR2D_DWRFD (const unsigned int ch_id, unsigned int value);
     void Set_TSRD_UDFD (const unsigned int ch_id, unsigned int value);
     unsigned int Get_DCNTD(const unsigned int ch_id);
     void Set_DCNTD(const unsigned int ch_id, unsigned int value);
     bool Get_TOCRD_TONEBD(const unsigned int ch_id);
     bool Get_TOCCRD_TONEBD (const unsigned int ch_id);
     bool Get_MIGCRD_MIGEND (const unsigned int ch_id);
     bool Get_MIGSELD_MIGSEL (const unsigned int ch_id);
     unsigned int Get_OFMIND (const unsigned int ch_id);
     unsigned int Get_ONMIND (const unsigned int ch_id);
     unsigned int Get_OTOMIND (const unsigned int ch_id);
     bool Get_OSELRD_OSELBD (const unsigned int ch_id);
     bool Get_ODRD_ODBD (const unsigned int ch_id);

    void RegisterHandler (const std::vector<std::string>& args);
    void read (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug);
    void write (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug);
private:
     ATU5010_TIMERD_SUB_AgentController  *mTIMERD_SUBAgent;
     std::string  mModuleName;
    //virtual function
};
#endif
