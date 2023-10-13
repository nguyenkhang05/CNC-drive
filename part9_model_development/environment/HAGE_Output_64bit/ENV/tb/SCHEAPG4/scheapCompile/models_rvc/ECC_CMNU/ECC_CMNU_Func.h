// ---------------------------------------------------------------------
// $Id: ECC_CMNU_Func.h,v 1.3 2015/11/06 02:01:36 ngankimtran2 Exp $
//
// Copyright(c) 2014 - 2015 Renesas Electronics Corporation
// Copyright(c) 2014 - 2015 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------
#ifndef __ECC_CMNU_FUNC_H__
#define __ECC_CMNU_FUNC_H__
#include "BusSlaveFuncIf.h"
#include "ecc_cmnu_regif.h"
#include "ECC_CMNU_AgentController.h"

class ECC_CMNU_Func: public Cecc_cmnu_regif
                 ,public BusSlaveFuncIf
{
public:
    ECC_CMNU_Func (std::string name, 
                ECC_CMNU_AgentController *ECCCMNUAgentPtr,
                unsigned int ECCCTLInitial = 0,
                unsigned int ERRINTInitial = 0,
                bool SupDuperr = false,
                unsigned int addr_awidth = 32,
                unsigned int addr_awidth_lsb = 0,
                std::string model_version = "G3M_V30");
    ~ECC_CMNU_Func (void);

    void RegisterHandler (const std::vector<std::string>& args);
    void EnableReset (const bool is_active, const std::string clock_domain);
    void ProcessErrorInfo (ECC_CMNU_AgentController::ErrorInfoStructure error_info,const unsigned int bank_no);
    unsigned int GetECCCTLVal (void);

    //Virtual functions of BusSlaveFuncIf class
    void read (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug);
    void write (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug);

private:
    enum eBankNo {
        emBank1                     = 1,    //for CFB_G3KH, TCM_G3KH, CFB_X2X, ICDM_G3MH, ICTM_G3MH versions
        emBank2                     = 2,    //for CFB_G3MH, GRAM_G3MH, TCM_G3MH versions
        emBankNo                    = 4     //for G3M_V30 version
    };//Define bank memory number

    ECC_CMNU_AgentController    *mECCCMNUAgent;
    std::string                 mModuleName;
    bool                        mSupDuperr;                         //Suppress duplicated error
    unsigned int                mAwidth;                            //The bit width of captured address
    unsigned int                mAwidth_lsb;                        //The low bit of captured address
    std::string                 mVersion;                           //Model's version (G3M_V30, CFB_G3KH, TCM_G3KH, CFB_G3MH, CFB_X2X, GRAM_G3MH, ICDM_G3MH, ICTM_G3MH, TCM_G3MH)
    bool                        mDoubleErrorBufferStatus[emBankNo]; //Double error buffer status
    bool                        mSingleErrorBufferStatus[emBankNo]; //Single error buffer status

    void Initialize (void);
    void SetECCCTLInitVal (const unsigned int init_val);
    void SetERRINTInitVal (const unsigned int init_val);
    void FilterCapturedError (ECC_CMNU_AgentController::ErrorInfoStructure input_info, bool &is_legal,
                              bool &is_dup, const unsigned int bank_no);
    bool IsReceivedSingleErr (ECC_CMNU_AgentController::ErrorInfoStructure error_info);
    bool IsReceivedDoubleErr (ECC_CMNU_AgentController::ErrorInfoStructure error_info);
    unsigned int GetCapturedAddress (const unsigned int bank_no);
    unsigned int GetCapturedStatus (const unsigned int bank_no);
    void StoreErrorInfo (ECC_CMNU_AgentController::ErrorInfoStructure error_info, const unsigned int bank_no,
                              const bool sedbuf, const bool dedbuf);
    void SetOverflow (const unsigned int bank_no);
    void ProcessErrorOutput (ECC_CMNU_AgentController::ErrorInfoStructure error_info);

    //Declare virtual function of ecccom_regif class
    void cb_ECCCTL_ECCDIS(RegCBstr str);
    void cb_STCLR_STCLR0(RegCBstr str);
    void cb_STSTCTL_ECCTST(RegCBstr str);
};
#endif
