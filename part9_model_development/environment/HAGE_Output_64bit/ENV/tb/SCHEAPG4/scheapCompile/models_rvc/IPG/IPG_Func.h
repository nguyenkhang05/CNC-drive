// ---------------------------------------------------------------------
// $Id: IPG_Func.h,v 1.4 2015/06/12 08:19:09 dungvannguyen Exp $
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
#ifndef __IPG_Func_H__
#define __IPG_Func_H__
#include "BusSlaveFuncIf.h"
#include "ipg_regif.h"

class IPG_AgentController;
struct Csyserripg;

class IPG_Func: public Cipg_regif
               ,public BusSlaveFuncIf
{
public:
    IPG_Func (std::string name,
              IPG_AgentController *IPG_AgentControllerPtr,
              std::string IPG_VERSION = "G3M_V30");
    ~IPG_Func (void);

    void Reset (const std::string reset_name, const bool is_active);
    void DumpStatusInfo (void);
    void RegisterHandler (const std::vector<std::string> &args);
    void SetGuardAreasInfo (const std::string area_name, const unsigned int start_addr, const unsigned int size);
    void GetGuardAreasInfo (const std::string area_name);
    bool CheckAccessAddress (const bool is_ifu, TlmBasicPayload& trans);
    //Virtual functions of BusSlaveFuncIf class
    void read (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug);
    void write (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug);

private:
    IPG_AgentController *mIPG_AgentController;
    std::string mModuleName;
    struct GuardAreaInfo {
        std::string area_name;
        unsigned int start_addr;
        unsigned int size;
        GuardAreaInfo (std::string _area_name = "NONE", unsigned int  _start_addr = 0x0, unsigned int _size = 0x0) {
            this->area_name = _area_name;
            this->start_addr = _start_addr;
            this->size = _size;
        }
    };
    GuardAreaInfo mAreaInfoValue;
    std::string mVersion;
    std::vector<GuardAreaInfo> mAreaInfo;
    Csyserripg *mErrPortValue;
    bool CheckIFUAccessPermission (std::vector<std::string> area_name, TlmBasicPayload& trans);
    bool CheckLSUAccessPermission (std::vector<std::string> area_name, TlmBasicPayload& trans);
    bool CheckIFUAreaAccessPermission (const std::string control_bit, vpcl::re_register *pmtum_reg,
                                       vpcl::re_register *pmtsv0_reg, vpcl::re_register *pmtum0_reg, TlmBasicPayload& trans);
    bool CheckLSUAreaAccessPermission (const std::string read_control_bit, const std::string write_control_bit,
                                       vpcl::re_register *pmtum_reg, vpcl::re_register *pmtsv0_reg,
                                       vpcl::re_register *pmtum0_reg, TlmBasicPayload& trans);
};
#endif
