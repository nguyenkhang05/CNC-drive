// ---------------------------------------------------------------------
// $Id: ECCUEABFunc.h,v 1.1 2013/08/19 02:44:41 dungvannguyen Exp $
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
#ifndef __ECCUEABFUNC_H__
#define __ECCUEABFUNC_H__
#include "BusSlaveFuncIf.h"
#include "CmErrMsg.h" //For WinMsg
#include "Common.h"
#include "ECCUEABAgent.h"
#include "eccueab_regif.h"

class ECCUEABFunc: public Ceccueab_regif,
                   public BusSlaveFuncIf
{
public:
    struct ErrorInfoStructure {//Define error information structure
        bool addr_ecc_2bit_error_status;
        bool addr_ecc_1bit_error_status;
        bool addr_parity_error_status;
        bool data_ecc_2bit_error_status;
        bool data_ecc_1bit_error_status;
        unsigned int error_addr;
        ErrorInfoStructure (void) {
            addr_ecc_2bit_error_status = false;
            addr_ecc_1bit_error_status = false;
            addr_parity_error_status = false;
            data_ecc_2bit_error_status = false;
            data_ecc_1bit_error_status = false;
            error_addr = 0x0;
        }
        void Initialize (void) {
            addr_ecc_2bit_error_status = false;
            addr_ecc_1bit_error_status = false;
            addr_parity_error_status = false;
            data_ecc_2bit_error_status = false;
            data_ecc_1bit_error_status = false;
            error_addr = 0x0;
        }
    };
    ECCUEABFunc (std::string name, 
                 ECCUEABAgent *ECCUEABAgentPtr,
                 unsigned int CapDepth = 1,
                 unsigned int ERRINTInitial = 0);
    ~ECCUEABFunc (void);

    void RegisterHandler (const std::vector<std::string>& args);
    void EnableReset (const bool is_active, const std::string clock_domain);
    bool CheckCapturedDoubleErrorInfo (void);
    void CheckCapturedDataECC1BitErrorInfo (ErrorInfoStructure &data_1bit_error_info);
    void StoreDoubleErrorInfo (ErrorInfoStructure double_error_info);
    void StoreDataECC1BitErrorInfo (ErrorInfoStructure data_1bit_error_info, const unsigned int store_pos);
    void CheckOverflow (bool &overflow, unsigned int &store_pos);
    void SetOverFlowStatus (void);
    unsigned int GetERRINTValue(const std::string bit_name);

    //Virtual functions of BusSlaveFuncIf class
    void read (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug);
    void write (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug);

private:
    enum eMaxCapDepth {
        emMaxCapDepth = 32
    };
    ECCUEABAgent *mECCUEABAgent;
    std::string mModuleName;
    unsigned int mCapDepth;

    //Declare virtual function of eccueab_regif class
    void cb_SSTCLR_SSTCLR00 (RegCBstr str);
    void cb_DSTCLR_DSTCLR00 (RegCBstr str);
};
#endif
