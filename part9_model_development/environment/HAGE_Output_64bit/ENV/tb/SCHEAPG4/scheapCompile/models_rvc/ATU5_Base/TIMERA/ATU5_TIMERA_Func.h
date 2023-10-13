// ---------------------------------------------------------------------
// $Id: ATU5_TIMERA_Func.h 1196 2019-10-17 05:33:04Z chuonghoangle $
//
// Copyright(c) 2016-2019 Renesas Electronics Corporation
// Copyright(c) 2016-2019 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------
#ifndef __ATU5_TIMERA_FUNC_H__
#define __ATU5_TIMERA_FUNC_H__

#include "atu5_timera_regif.h"
#include "OSCI2.h"

class ATU5_TIMERA_Base;
class ATU5_TIMERA_Func: public Catu5_timera_regif
{
public:
    ATU5_TIMERA_Func(std::string name, ATU5_TIMERA_Base *ATU5_TIA_Base_ptr);
    ~ATU5_TIMERA_Func (void);

    void read (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug);
    void write (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug);

    void RegisterHandler (const std::vector<std::string> &args);
    void EnableReset (const bool is_active);

    unsigned int GetCLKBusSel (void);
    unsigned int GetNoiseCancelMode (const unsigned int chan_num);
    unsigned int GetNoiseCLKSel (const unsigned int chan_num);
    unsigned int GetTCNTA (void);
    unsigned int GetNCNTA (const unsigned int chan_num);
    unsigned int GetICRA2SEL (void);
    bool IsOutEvent (const std::string event_name,const unsigned int chan_num, const bool is_raisingedge);
    bool IsNoiseCancelEn (const unsigned int chan_num);
    bool IsInterruptEnable (const std::string bit_name);
    void UpdateTimerStatus (const bool is_captured, const unsigned int chan_num);
    void UpdateICRAVal (const unsigned int value, const unsigned int chan_num, const bool is_icra2x);
    void SetTCNTA (const unsigned int value);
    void SetNCNTA (const unsigned int value, const unsigned int chan_num);
private:
    ATU5_TIMERA_Base                *mATU5_TIA_Base;
    std::string                     mModuleName;

    void cb_TCR1A_CKSELA(RegCBstr str);
    void cb_TSCRA_ICFCA(RegCBstr str);
    void cb_NCMCR1A_NCM1A(RegCBstr str);
    void cb_NCMCR2A_NCM2A(RegCBstr str);
    void cb_TIOR2A_NCEA(RegCBstr str);
    void cb_TCNTA_CNTA(RegCBstr str);
    void cb_NCNTA_NCNTA(const unsigned int chan_num);
    void cb_NCRA_NCTA(const unsigned int chan_num);
};
#endif
