// ---------------------------------------------------------------------
// $Id: ECC_BUS_MER_AD_Func.cpp,v 1.4 2020/04/01 06:41:19 thunguyen3 Exp $
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
#include "ECC_BUS_MER_AD_Reg.h"
#include "ECC_BUS_APEC_Reg.h"
#include "ECC_BUS_MER_AD_Func.h"
#ifndef func_re_printf
#define func_re_printf mpECC_BUS_MER_ADAgentController->get_fileline(__FILE__, __LINE__); mpECC_BUS_MER_ADAgentController->_re_printf
#endif//func_re_printf

//========================================================
//============ Constructor/Destructor/Common APIs=========
//========================================================
/// Constructor
ECC_BUS_MER_AD_Func::ECC_BUS_MER_AD_Func (std::string name, std::string kind, ECC_BUS_MER_AD_AgentController* ECC_BUS_MER_ADAgentController):
                BusSlaveFuncIf()
{//{{{ 
    assert(ECC_BUS_MER_ADAgentController != NULL);
    assert ((kind == "APEC") || (kind == "HBEC"));
    mpECC_BUS_MER_ADAgentController = ECC_BUS_MER_ADAgentController;
    mpECC_BUS_MER_ADAgentController->CommandInit(name);
    mModuleName = name;
    mKind = kind;

    if (mKind == "APEC"){
        mpAPECRegBlock = new ECC_BUS_APEC_Reg(name, this, ECC_BUS_MER_ADAgentController);
    }else{
        mpMERRegBlock = new ECC_BUS_MER_AD_Reg(name, this, ECC_BUS_MER_ADAgentController);
    }
}//}}}

/// Destructor
ECC_BUS_MER_AD_Func::~ECC_BUS_MER_AD_Func()
{//{{{
    if (mKind == "APEC"){
        delete mpAPECRegBlock;
    }else{
        delete mpMERRegBlock;
    }
}//}}}

/// Reset Func and registers
void ECC_BUS_MER_AD_Func::EnableReset(const bool is_active)
{//{{{
    if (mKind == "APEC"){
        mpAPECRegBlock->EnableReset(is_active);
    }else{
        mpMERRegBlock->EnableReset(is_active);
    }
}//}}}

/// Read API (target socket)
void ECC_BUS_MER_AD_Func::read (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug)
{//{{{
    TlmG3mExtension *pExtInfo;
    trans.get_extension(pExtInfo);

    memset(trans.get_data_ptr(), 0, trans.get_data_length());
    // Zero clock or reset active
    if ((!mpECC_BUS_MER_ADAgentController->CheckClockPeriod()) || (mpECC_BUS_MER_ADAgentController->GetResetStatus())) {
        if (pExtInfo != NULL) {
            if (!(debug && (pExtInfo->getPeId() == DEBUG_MASTER_BMID))) {
                func_re_printf("warning", "Cannot access register when clock is 0 or reset port is asserted.\n");
                return;
            }
        }
        else {
            func_re_printf("warning", "Cannot access register when clock is 0 or reset port is asserted.\n");
            return;
        }
    }
    if (mKind == "APEC"){
        mpAPECRegBlock->read(offsetAddress, trans.get_data_ptr(), trans.get_data_length(), debug);
    }else{
        mpMERRegBlock->read(offsetAddress, trans.get_data_ptr(), trans.get_data_length(), debug);
    }
}//}}}

/// Write API (target socket)
void ECC_BUS_MER_AD_Func::write (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug)
{//{{{
    TlmG3mExtension *pExtInfo;
    trans.get_extension(pExtInfo);

    if (!mpECC_BUS_MER_ADAgentController->CheckClockPeriod()) {
        // Software debugger
        if ((pExtInfo != NULL)) {
            if (!(debug && (pExtInfo->getPeId() == DEBUG_MASTER_BMID))) {
                func_re_printf("warning", "Cannot write register when clock is 0.\n");
                return;
            }
        }
        else {
            func_re_printf("warning", "Cannot write register when clock is 0.\n");
            return;
        }
    }

    if (mKind == "APEC"){
        mpAPECRegBlock->write(offsetAddress, trans.get_data_ptr(), trans.get_data_length(), debug);
    }else{
        mpMERRegBlock->write(offsetAddress, trans.get_data_ptr(), trans.get_data_length(), debug);
    }
}//}}}

/// Handler for register IF
void ECC_BUS_MER_AD_Func::RegisterHandler (const std::vector<std::string> &args)
{//{{{
    if (mKind == "APEC"){
        mpAPECRegBlock->RegisterHandler(args);
    }else{
        mpMERRegBlock->RegisterHandler(args);
    }
}//}}}

/// Process error of data
void ECC_BUS_MER_AD_Func::ProcessDataError(ErrorInfo data_error)
{//{{{
    if (mKind == "APEC"){
        mpAPECRegBlock->ProcessDataError(data_error);
    }else{
        mpMERRegBlock->ProcessDataError(data_error);
    }
}//}}}

/// Process error of address
void ECC_BUS_MER_AD_Func::ProcessAddressError(ErrorInfo address_error)
{//{{{ 
    if (mKind == "APEC"){
        mpAPECRegBlock->ProcessAddressError(address_error);
    }else{
        mpMERRegBlock->ProcessAddressError(address_error);
    }
}//}}}

/// Get ECCCTL register value
unsigned int ECC_BUS_MER_AD_Func::GetECCCTLReg()
{//{{{
    unsigned int ret = 0;
    if (mKind == "APEC"){
        ret = mpAPECRegBlock->GetECCCTLReg();
    }else{
        ret = mpMERRegBlock->GetECCCTLReg();
    }
    return ret;
}//}}}

/// Dump Status info
void ECC_BUS_MER_AD_Func::DumpStatusInfo()
{//{{{
    if (mKind == "APEC"){
        mpAPECRegBlock->DumpStatusInfo();
    }else{
        mpMERRegBlock->DumpStatusInfo();
    }
}//}}}

/// Assert interrupt according to interrupt name
void ECC_BUS_MER_AD_Func::AssertInterrupt(std::string int_name)
{//{{{
    mpECC_BUS_MER_ADAgentController->AssertInterrupt(int_name);
}//}}}

/// Write disable port
void ECC_BUS_MER_AD_Func::WriteDisablePort(unsigned int value)
{//{{{
    mpECC_BUS_MER_ADAgentController->WriteDisablePort(value);
}//}}}

// vim600: set foldmethod=marker :
