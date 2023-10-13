// ---------------------------------------------------------------------
// $Id: ECC_DATAFLASH_Func.h,v 1.4 2015/08/06 11:11:14 huepham Exp $
//
// Copyright(c) 2014-2015 Renesas Electronics Corporation
// Copyright(c) 2014-2015 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------
#ifndef __ECC_DATAFLASH_Func_H__
#define __ECC_DATAFLASH_Func_H__
#include "BusSlaveFuncIf.h"
#include "ecc_dataflash_regif.h"

class ECC_DATAFLASH_AgentController;
/// ECC_DATAFLASH_Func model class
class ECC_DATAFLASH_Func:    public Cecc_dataflash_regif,
                            public BusSlaveFuncIf
{
private:
    enum eOutputPort {
        emECCSED = 0,
        emECCDED = 1,
        emECCOVF = 2,
        emEccDis = 3,
        emSecDis = 4
    };
    enum eErrorType {
        em1BitError = 0,
        em2BitError = 1
    };
    std::string mModuleName;
    ECC_DATAFLASH_AgentController *mAgentController;

    std::string mVersion;// ECC_DATAFLASH version (G3M_V30, G3KH)
    unsigned int mDFERRINTInitValue;//Initial value of DFERRINT register
    bool mIsAddrCaptureEnable;
    unsigned int mPawidth;

    void cb_DFECCCTL_ECCDIS(RegCBstr str);
    void cb_DFERSTC_ERRCLR(RegCBstr str);
    void cb_DFOVFSTC_ERROVFCLR(RegCBstr str);
    void cb_DFTSTCTL_ECCTST(RegCBstr str);

    bool CheckEccStatus(void);
    bool CheckUpdateErrSttCondition(void);
    bool CheckUpdateOvfSttCondition(const unsigned int err_type, const unsigned int err_addr);
    void TriggerOffOutputSignal(const unsigned int output_id);
    void UpdateErrorStatus(const unsigned int err_type, const unsigned int err_addr);

public:
    ECC_DATAFLASH_Func (std::string name, const bool is_addcap_enable, unsigned int addr_pawidth, ECC_DATAFLASH_AgentController *AgentControllerPtr, std::string ECC_DATAFLASH_VERSION);
    ~ECC_DATAFLASH_Func (void);

    void Reset (const bool is_active);
    void DumpStatusInfo (void);
    void RegisterHandler (const std::vector<std::string> &args);
    void SetSEDIEInitValue (const bool value, const bool is_update_now);
    void SetDFERRINTInitValue (const unsigned int value);
    void RecordErrInfo(const unsigned int err_type, const unsigned int err_addr);

    //virtual functions of BusSlaveFuncIf class
    void read (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug);
    void write (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug);

};
#endif //__ECC_DATAFLASH_Func_H__
// vim600: set foldmethod=marker :
