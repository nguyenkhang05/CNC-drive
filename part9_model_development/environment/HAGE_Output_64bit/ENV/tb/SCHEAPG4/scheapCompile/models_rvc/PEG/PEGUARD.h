// ---------------------------------------------------------------------
// $Id: PEGUARD.h,v 1.6 2015/06/28 02:51:24 dungvannguyen Exp $
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

#ifndef __PEGUARD_H__
#define __PEGUARD_H__
#include "BusSlaveFuncIf.h"
#include "peguard_regif.h"

class PEG_AgentController;
struct Cerrinfo;
struct Csyserrvci;

/// PEGUARD model class
class PEGUARD:  public Cpeguard_regif
               ,public BusSlaveFuncIf
{
public:
    PEGUARD(std::string name, 
            PEG_AgentController *AgentController_ptr,
            unsigned int id_number,
            std::string version = "G3M_V30");
    ~PEGUARD();

    void Reset(const std::string reset_name, const bool is_active);
    bool CheckAccessibility(TlmBasicPayload &trans, Cerrinfo &err_info, Csyserrvci &syserrvci);
    void RegisterHandler(const std::vector<std::string> &args);
    bool SetAreaAddr(const std::string name, const unsigned int start_addr, const unsigned int size, const std::string pe_id);
    void SetAccessMode(const std::string mode, const bool is_enable);
    void SetG0InitialVal(const std::string bit_name, const unsigned int value, const bool is_started_simulation);

    // virtual functions of BusSlaveFuncIf class
    void read(unsigned int offsetAddress, 
              TlmBasicPayload& trans, 
              BusTime_t* t, 
              bool debug);
    void write(unsigned int offsetAddress, 
               TlmBasicPayload& trans, 
               BusTime_t* t, 
               bool debug);
private:
    struct AreaInformation {
        std::string area_name;
        unsigned int start_addr;
        unsigned int end_addr;
        std::string peid;
        AreaInformation(std::string _area_name = "",
                        unsigned int _start_addr = 0,
                        unsigned int _end_addr = 0,
                        std::string _peid = "")
        {
            this->area_name = _area_name;
            this->start_addr = _start_addr;
            this->end_addr = _end_addr;
            this->peid = _peid;
        }
    };
    enum eNumberOfSPIDArea {
        emNumberOfSPIDArea_V30_Other = 8,
        emNumberOfSPIDArea_V20_G3MH_G3KH = 4
    };
    PEG_AgentController *mAgentController;
    std::string mModuleName;
    unsigned int mPEGUARD_ID;
    std::string mVersion;
    std::vector<AreaInformation> mAreaInfo;

    void cb_PEGGMK_MASK(RegCBstr str);
    void cb_PEGGBA_EN(RegCBstr str);
    void cb_PEGGSP_SPID(RegCBstr str);

    bool CheckTransAddress(const unsigned int trans_addr, const std::string mode);
    bool CheckPEGUARD_ID(const unsigned int area_id);
    bool CheckDebugAccess(const unsigned int trans_addr, const unsigned int peid_val);
    bool CheckSafetyAccess(const unsigned int trans_addr, const unsigned int peid_val, const unsigned int vcid_val, const bool vm_val);
    bool CheckSPID_PEIDAccess(const unsigned int trans_addr, const unsigned int spid_val, const unsigned int peid_val,
                              const bool trans_type, const bool is_secured);
    bool CheckSecurityAccess(const unsigned int trans_addr, const unsigned int spid_val, const bool trans_type);
};

#endif //__PEGUARD_H__
