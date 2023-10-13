// ---------------------------------------------------------------------
// $Id: HWCORE_RCTG_Func.h,v 1.3 2020/08/24 03:06:10 khoaanhnguyen Exp $
//
// Copyright(c) 2020 Renesas Electronics Corporation
// Copyright(c) 2020 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation:
// ---------------------------------------------------------------------
// Description: Copyright of HWCORE_RCTG_Func.h
// Ref: {HWCORE_RCTG_UD_Copyright_001}
#ifndef __HWCORE_RCTG_FUNC_H__
#define __HWCORE_RCTG_FUNC_H__
// Ref: {HWCORE_RCTG_UD_ATT1_002}
#include "hwcore_rctg_regif.h"

#define SIGNBIT_IDX 11    // Index of signed bit
#define MAX_SHORT 0xFFFF  // Max hexadecimal number of unsigned short

// Description: Declaration of HWCORE_RCTG_Func class
class HWCORE_RCTG_Func : public Chwcore_rctg_regif
{
public:
    SC_HAS_PROCESS(HWCORE_RCTG_Func);
    HWCORE_RCTG_Func(std::string name, HWCORE_RCTG_AgentController* HWCORE_RCTG_AgentControllerPtr);
    ~HWCORE_RCTG_Func();

    void EnableReset(const bool is_active);
    void RegisterHandler(const std::vector<std::string> &args);

    // Read/write registers API
    void read(unsigned int offsetAddress, unsigned char* data_ptr, unsigned int size, bool debug);
    void write(unsigned int offsetAddress, unsigned char* data_ptr, unsigned int size, bool debug);

    // Set/Get registers APIs
    void SetRegVal(std::string RegName, unsigned int val);
    unsigned int GetRegVal(std::string RegName, std::string BitName);
    unsigned int GetRegVal(std::string RegName);
    unsigned int GetRegBufferVal(std::string BufferName);

    //Buffer related APIs
    void UpdateRegBuffer();
    // APIs for core-processes of rectangle IP/batch rectangle IP
    void RectangleIPProcess();
    void BRectangleIPProcess();

private:
    enum eRCTGPhases{
        emU_PHASE = 0,
        emV_PHASE,
        emW_PHASE,
        //Number of phases
        emNumPhase
    };

    // Variables
    HWCORE_RCTG_AgentController * mpRCTG_AgentController;    // HWCORE_RCTG Agent controller pointer
    std::string     mModuleName;                           // Module name
    std::map<std::string, vpcl::re_register*> mRegisterMap;        // Map value of registers
    std::map<std::string, unsigned int> mBufferMap;        // Map value of registers
    bool mRectPtn[emNumPhase];

    // Function
    void Initialize(void);
    unsigned short CalcSignExt(unsigned short Reg_value, unsigned char signbit_index);
    void RectangleIPCoreProcess(unsigned char* psw, unsigned short* ipcomp0, bool* ptn);
    void BRectangleIPCoreProcess(bool* ptn);

    // Callback
    void cb_RECCTR_SLCTCMP0(RegCBstr str);
    void cb_CMP0_DATA(RegCBstr str);
    void cb_CMP1_DATA(RegCBstr str);
    void cb_PHQSFT_DATA(RegCBstr str);
    void cb_PSWSFT_DATA(RegCBstr str);
};

#endif // __HWCORE_RCTG_FUNC_H__
