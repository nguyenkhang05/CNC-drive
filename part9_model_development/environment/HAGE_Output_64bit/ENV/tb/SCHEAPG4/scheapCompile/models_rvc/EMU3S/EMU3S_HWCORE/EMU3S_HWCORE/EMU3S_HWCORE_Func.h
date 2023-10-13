// ---------------------------------------------------------------------
// $Id: EMU3S_HWCORE_Func.h,v 1.2 2020/10/27 04:26:15 huyquocpham Exp $
//
// Copyright(c) 2020 Renesas Electronics Corporation
// Copyright(c) 2020 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------
// Description: Copyright of .EMU3S_HWCORE_Func.h
// Ref: {EMU3S_HWCORE_UD_Copyright_001}

#ifndef _EMU3S_HWCORE_FUNC_H_
#define _EMU3S_HWCORE_FUNC_H_

#include "global.h"
#include "BusSlaveFuncIf.h"
#include "emu3s_hwcore_regif.h"


// Description: EMU3S_HWCORE_Func class
// Ref: {EMU3S_HWCORE_UD_ATT1_003, EMU3S_HWCORE_UD_ATT1_006, EMU3S_HWCORE_UD_ATT1_008}
class EMU3S_HWCORE_AgentController;
class EMU3S_HWCORE_Func : public Cemu3s_hwcore_regif
                        , public BusSlaveFuncIf
{
public:
    /// Constructor & Destructor
    EMU3S_HWCORE_Func(std::string name, EMU3S_HWCORE_AgentController* pEMU3S_HWCORE_AgentController);
    ~EMU3S_HWCORE_Func();

    /// Functions
    void EnableSWReset(const bool is_active);
    void EnableReset(const bool is_active);
    void RegisterHandler(const std::vector<std::string> &args);

    // Virtual functions of BusSlaveFuncIf class
    void read(unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug);
    void write(unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug);
    void SetRegVal(std::string RegName, unsigned int val);
    void SetRegVal(std::string RegName, std::string BitName, unsigned int val);
    unsigned int GetRegVal(std::string RegName);
    unsigned int GetRegVal(std::string RegName, std::string BitName);

    void Initialize(void);
private:
    /// Variables
    EMU3S_HWCORE_AgentController* mpEMU3S_HWCORE_AgentController;
    std::string                   mModuleName;               // Module name

    /// Overwrite virtual functions of emu3s_hwcore_regif class
    void cb_PRT_MEM0PRT(RegCBstr str);          
    void cb_CTR_EMUST(RegCBstr str);
    void cb_REFCTR_FPWMREFPER(RegCBstr str);
    void cb_AHBPRT_HPRT(RegCBstr str);          
    void cb_IPSFT_INIPSFT(RegCBstr str); 
    void cb_TBLADR_DATA(RegCBstr str);          
    void cb_IPTRGCTR_TNEN(RegCBstr str);
    void cb_IPTRGNUM_ADTRGNUM(RegCBstr str);
    void cb_IPTRGMON_ADTRGNUM(RegCBstr str);
    void cb_FUNCIDLEGRPA0_SFTEN(RegCBstr str);
    void cb_FUNCIDLEGRPA1_SFTEN(RegCBstr str);  
    void cb_FUNCIDLEGRPA2_SFTEN(RegCBstr str);  
    void cb_FUNCIDLEGRPB_IP(RegCBstr str);
    void cb_FUNCWAITGRPA_IP(RegCBstr str);
    void cb_FUNCWAITGRPB_IP(RegCBstr str);      
    void cb_FUNCSTGRPA_INPUT0(RegCBstr str);
    void cb_FUNCSTGRPB_ANG0(RegCBstr str);      
    void cb_FUNCFLGRPA0_INPUT0(RegCBstr str);
    void cb_FUNCFLGRPA1_PI0(RegCBstr str);      
    void cb_FUNCFLGRPA2_PWM0(RegCBstr str);     
    void cb_FUNCFLGRPB_ANG0(RegCBstr str); 
    void cb_ADRDCSL_ADSEL(RegCBstr str);
    void cb_EDCCTR_EDCDIS(RegCBstr str);
    void cb_SFERC_SENE(RegCBstr str);
    void cb_EDCSECLR_SESCLR(RegCBstr str);
    void cb_EDCDECLR_DESCLR(RegCBstr str);

    std::map<std::string, vpcl::re_register*> mRegisterMap;        // Map value of registers

    bool mIsPRTWritePermitted; 
};
#endif
