// ---------------------------------------------------------------------
// $Id: APBFSGuard.h,v 1.9 2014/04/11 05:44:31 duymai Exp $
//
// Copyright(c) 2012 Renesas Electronics Corporation
// Copyright(c) 2012 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------

#ifndef __APBFSGUARD_H__
#define __APBFSGUARD_H__
#include "BusSlaveFuncIf.h"
#include "CmErrMsg.h" // for WinMsg
#include "Common.h"
#include "ApgAgentController.h"
#include "apbfsguard_regif.h"
#include <map>

/// APBFSGUARD model class
class APBFSGuard:   public Capbfsguard_regif
                   ,public BusSlaveFuncIf
{
public:
    APBFSGuard(std::string name, 
               ApgAgentController *AgentController_ptr,
               const unsigned int version = 0,
               const bool lock_support = true,
               const unsigned int filter_no = 1);
    ~APBFSGuard();

    void Reset(const bool is_active);
    bool CheckAccessibility(TlmBasicPayload &trans, const bool debug, bool &is_legal_trans);
    void SetFilterInfo(const bool is_set, const unsigned int id, 
                       const unsigned int start_addr, const long long size, const std::string property);
    void DumpStatusInfo(void);
    std::string RegisterHandler(const std::vector<std::string> &args);
    void SetRegInitValue(const unsigned int filter_id, const std::string reg_name, const unsigned int init_val=0);

    // virtual functions of BusSlaveFuncIf class
    void read(unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug);
    void write(unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug);
private:
    enum eRegDefaultValue {
        emPROTDefaultCCCUBE = 0x07FFFE1F,
        emPROTDefaultOthers = 0x07FFFFFF,
        emSPIDDefault       = 0xFFFFFFFF
    };
    struct AreaInformation {
        unsigned int filter_id;
        unsigned int start_addr;
        long long size;
        AreaInformation(unsigned int _filter_id = 0x0,
                        unsigned int _start_addr = 0x0, 
                        long long _size = 0x0)
        {
            this->filter_id = _filter_id;
            this->start_addr = _start_addr;
            this->size = _size;
        }
    };
    struct FilterInformation {
        bool is_enable;
        std::string property;
        FilterInformation(bool _is_enable = false, std::string _property = "RW")
        {
            this->is_enable = _is_enable;
            this->property = _property;
        }
    };
    ApgAgentController *mAgentController;
    std::vector<AreaInformation> mAreaInfo;
    FilterInformation mFilterInfo[16];
    unsigned int mVersion;
    bool mLockEnable;
    unsigned int mFilterNo;
    bool mIsDefaultValue;
    void cb_PROT_LOCK(RegCBstr str);
};

#endif //__APBFSGUARD_H__
