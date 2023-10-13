// ---------------------------------------------------------------------
// $Id: ECC_PRAM_E2_Func.h,v 1.4 2019/09/06 08:49:51 landang Exp $
//
// Copyright(c) 2017 Renesas Electronics Corporation
// Copyright(c) 2017 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------
#ifndef __ECC_PRAM_E2_FUNC_H__
#define __ECC_PRAM_E2_FUNC_H__

#include "BusSlaveFuncIf.h"
#include "ecc_pram_e2_regif.h"
#include "ECC_PRAM_E2_AgentController.h"
#include "global.h"

class ECC_PRAM_E2_Func: public Cecc_pram_e2_regif
                ,public BusSlaveFuncIf
{
public:
    SC_HAS_PROCESS (ECC_PRAM_E2_Func);
    ECC_PRAM_E2_Func(std::string name, ECC_PRAM_E2_AgentController* PRAMAgentController);
    ~ECC_PRAM_E2_Func();

    void EnableReset(const bool is_active);
    void RegisterHandler(const std::vector<std::string> &args);
    unsigned int GetECDEDFBits();
    unsigned int GetECSEDFBits();
    unsigned int GetECOVFFBit();
    unsigned int GetExEADn(unsigned int index);
    void UpdateErrorJudgement();
    bool CheckPermitJudgeError();
    void Process1bitError(unsigned int address);
    void Raise1bitErrorInterrupt();
    void ProcessFatalError(unsigned int address);
    void Capture2bitError(unsigned int address, unsigned int index);
    void Raise2bitErrorInterrupt();
    void SetECEMFbit(unsigned int value);
    unsigned int GetExCTLReg();

    // Virtual functions of BusSlaveFuncIf class
    void read(unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug);
    void write(unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug);

private:
    // enum
    enum eCapStageInfo{
        emNoStage       = -1
        ,emFull         = -2
        ,emAlreadyCap   = -3
    };
    // Variables
    ECC_PRAM_E2_AgentController   *mpPRAMAgentController;    // KCPROT Agent controller pointer
    std::string     mModuleName;// Module name
    unsigned int mAddrLatch;    // Indicate which index in address buffer already has address.
    unsigned int mLastIndex;    // Indicate last index in address buffer
    bool         mIsErrorJudgment;  // Indicate permit/prohibit error judgment

    // Function
    void Initialize();
    unsigned int ShiftAddress(unsigned int address);
    int FindRoomToCapAddr(unsigned int address, bool is_2bit);
    bool CheckAddressSameFactorCaptured(unsigned int address, bool is_2bit);
    bool CheckFull1bitErrorStatus();

    // Callback
    void cb_ExCTL_ECER1C(RegCBstr str);
    void cb_ExCTL_ECTHM(RegCBstr str);

};
#endif //__ECC_PRAM_E2_FUNC_H__
