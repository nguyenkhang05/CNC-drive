// ---------------------------------------------------------------------
// $Id: SFMA_Func.h,v 1.2 2020/01/30 08:01:36 huepham Exp $
//
// Copyright(c) 2018-2020 Renesas Electronics Corporation
// Copyright(c) 2018-2020 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------
#ifndef __SFMA_FUNC_H__
#define __SFMA_FUNC_H__

#include "BusSlaveFuncIf.h"
#include "sfma_regif.h"
#include "SFMA_AgentController.h"

class SFMA_Func: public Csfma_regif
                ,public BusSlaveFuncIf
{
public:
    SFMA_Func(std::string name, SFMA_AgentController* SFMAAgentController);
    ~SFMA_Func();

    void EnableReset(const bool is_active);
    void RegisterHandler(const std::vector<std::string> &args);
    void DumpStatusInfo(void);

    // Virtual functions of BusSlaveFuncIf class
    void read(unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug);
    void write(unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug);
    
    // Set/get value of bit in register
    unsigned int GetBitField(const std::string regName, const std::string bitName);
    void SetBitField(const std::string regName, const std::string bitName, unsigned int value);

private:
    // Transfer data size
    enum eTransferDataSize {
        emNotTransferData = 0
        ,emOneByteData = 0x8
        ,emTwoBytesData = 0xC
        ,emFourBytesData = 0xF
    };

    // Register address mask, Offset size = 7
    enum eRegAddrMask {
        emRegAddrMask = 0x7F
    };

    // Variables
    SFMA_AgentController   *mpSFMAAgentController;    // SFMA Agent controller pointer
    std::string            mModuleName;               // Module name
    // Function
    void Initialize(void);

    // Callback
    void cb_SFMACMNCR_SSLP(RegCBstr str);
    void cb_SFMASSLDR_SPNDL(RegCBstr str);
    void cb_SFMASPBCR_SPBR(RegCBstr str);
    void cb_SFMADRCR_SSLN(RegCBstr str);
    void cb_SFMADRCMR_CMD(RegCBstr str);
    void cb_SFMADREAR_EAV(RegCBstr str);
    void cb_SFMADROPR_OPD3(RegCBstr str);
    void cb_SFMADRENR_CDB(RegCBstr str);
    void cb_SFMASMCR_SPIE(RegCBstr str);
    void cb_SFMASMCMR_CMD(RegCBstr str);
    void cb_SFMASMADR_ADR(RegCBstr str);
    void cb_SFMASMOPR_OPD3(RegCBstr str);
    void cb_SFMASMENR_CDB(RegCBstr str);
    void cb_SFMASMWDR_WDATA(RegCBstr str);
    void cb_SFMADRDMCR_DMDB(RegCBstr str);
    void cb_SFMASMDMCR_DMDB(RegCBstr str);
};
#endif //__SFMA_FUNC_H__
