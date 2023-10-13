// ---------------------------------------------------------------------
// $Id: ECCComFunc.h,v 1.1 2013/08/19 02:26:10 dungvannguyen Exp $
//
// Copyright(c) 2013 Renesas Electronics Corporation
// Copyright(c) 2013 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------
#ifndef __ECCCOMFUNC_H__
#define __ECCCOMFUNC_H__
#include "BusSlaveFuncIf.h"
#include "CmErrMsg.h" //For WinMsg
#include "Common.h"
#include "ECCComAgent.h"
#include "ecccom_regif.h"

class ECCComFunc: public Cecccom_regif
                 ,public BusSlaveFuncIf
{
public:
    struct ErrorInfoStructure {//Define error information structure
        bool is_ready;
        bool addr_ecc_2bit_error_status;
        bool addr_ecc_1bit_error_status;
        bool data_ecc_2bit_error_status;
        bool data_ecc_1bit_error_status;
        bool addr_parity_error_read_status;
        bool addr_parity_error_write_status;
        bool parity_bit_error_status;
        bool data_ecc_2bit_error_ext_status;
        bool data_ecc_1bit_error_ext_status;
        unsigned int error_addr;
        ErrorInfoStructure (void) {
            is_ready = false;
            addr_ecc_2bit_error_status = false;
            addr_ecc_1bit_error_status = false;
            data_ecc_2bit_error_status = false;
            data_ecc_1bit_error_status = false;
            addr_parity_error_read_status = false;
            addr_parity_error_write_status = false;
            parity_bit_error_status = false;
            data_ecc_2bit_error_ext_status = false;
            data_ecc_1bit_error_ext_status = false;
            error_addr = 0x0;
        }
        void Initialize (void) {
            is_ready = false;
            addr_ecc_2bit_error_status = false;
            addr_ecc_1bit_error_status = false;
            data_ecc_2bit_error_status = false;
            data_ecc_1bit_error_status = false;
            addr_parity_error_read_status = false;
            addr_parity_error_write_status = false;
            parity_bit_error_status = false;
            data_ecc_2bit_error_ext_status = false;
            data_ecc_1bit_error_ext_status = false;
            error_addr = 0x0;
        }
    };
    ECCComFunc (std::string name, 
                ECCComAgent *ECCComAgentPtr,
                unsigned int CapDepth = 1,
                unsigned int ECCCTLInitial = 0,
                unsigned int ERRINTInitial = 0,
                bool SupDupsed = false,
                bool UeabMode = false);
    ~ECCComFunc (void);

    void RegisterHandler (const std::vector<std::string>& args);
    void EnableReset (const bool is_active, const std::string clock_domain);
    void ProcessErrorInfo (ErrorInfoStructure error_info,const unsigned int bank_no);
    void FilterCapturedError (ErrorInfoStructure input_info, ErrorInfoStructure &legal_info,
                             ErrorInfoStructure &duplicated_info,  const unsigned int bank_no);
    void StoreErrorInfo (const bool is_single_error, ErrorInfoStructure error_info, const unsigned int bank_no, const unsigned int depth=0);
    unsigned int GetECCCTLValue(void);
    unsigned int GetCapturedAddress (const bool is_single_error, const unsigned int bank_no, const unsigned int depth=0);
    unsigned int GetCapturedStatus (const bool is_single_error, const unsigned int bank_no, const unsigned int depth=0);
    void SetOverflow (const bool is_single_error, const unsigned int bank_no);
    void ProcessErrorOutput (const bool is_single_error, ErrorInfoStructure error_info, const unsigned int bank_no);

    //Virtual functions of BusSlaveFuncIf class
    void read (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug);
    void write (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug);

private:
    enum eBankNo {//Define bank memory number
        emBankNo = 4
    };
    enum eMaxCapDepth {//Define maximum capture depth
        emMaxCapDepth = 8
    };
    ECCComAgent *mECCComAgent;
    std::string mModuleName;
    vpcl::re_register *ERSTR[emMaxCapDepth];
    vpcl::re_register *EADR[emBankNo][emMaxCapDepth];
    bool mDoubleErrorBufferStatus[emBankNo];//Double error buffer status
    bool mSingleErrorBufferStatus[emBankNo][emMaxCapDepth];//Single error buffer status
    unsigned int mCapDepth;//Capture depth
    bool mSupDupsed;//Suspress duplicated SED error
    bool mUeabMode;//UEAB mode

    void Initialize (void);
    //Declare virtual function of ecccom_regif class
    void cb_ECCCTL_ECCDIS (RegCBstr str);
    void cb_SSTCLR_SSTCLR10 (RegCBstr str);
    void cb_DSTCLR_DSTCLR0 (RegCBstr str);
    void cb_STSTCTL_ECCTST (RegCBstr str);
};
#endif
