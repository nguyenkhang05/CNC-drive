// ---------------------------------------------------------------------
// $Id: AHBERRSLV.h,v 1.3 2014/03/03 06:28:41 dungvannguyen Exp $
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

#ifndef __AHBERRSLV_H__
#define __AHBERRSLV_H__
#include "BusSlaveFuncIf.h"
#include "CmErrMsg.h" // for WinMsg
#include "Common.h"
#include "AhgAgentController.h"
#include "ahberrslv_regif.h"

/// AHBERRSLV model class
class AHBERRSLV: public Cahberrslv_regif
                ,public BusSlaveFuncIf
{
public:
    AHBERRSLV(std::string name,
              AhgAgentController *AhgAgentController_ptr,
              unsigned int version = 0);
    ~AHBERRSLV();

    void ClearErrorFlags(bool ovf_flag);
    void Reset(const bool is_active, const bool register_affect);
    void InformError(TlmBasicPayload &trans);
    void HandleErrInfo(void);
    void DumpStatusInfo(void);
    std::string RegisterHandler(const std::vector<std::string> &args);

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
    struct TransInfo
    {
        unsigned int peid;
        unsigned int vcid;
        unsigned int spid;
        bool um;
        bool vm;
        unsigned int size;
        bool is_write;
        unsigned int addr;
        TransInfo(unsigned int _peid=0,
                  unsigned int _vcid=0,
                  unsigned int _spid=0,
                  bool _um=false,
                  bool _vm=false,
                  unsigned int _size=0,
                  bool _is_write=false,
                  unsigned int _addr=0
                  )
        {
            this->peid = _peid;
            this->vcid = _vcid;
            this->spid = _spid;
            this->um = _um;
            this->vm = _vm;
            this->size = _size;
            this->is_write = _is_write;
            this->addr = _addr;
        };
        void ResetExtInfo()
        {
            peid = 0;
            vcid = 0;
            spid = 0;
            um = false;
            vm = false;
            size = 0;
            is_write = false;
            addr = 0;
        }
    };
    AhgAgentController *mAgentController;
    TransInfo mTempTransInfo; 

    void cb_CTL_CLRE(RegCBstr str);
};

#endif //__AHBERRSLV_H__
