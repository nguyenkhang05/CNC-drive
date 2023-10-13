// ---------------------------------------------------------------------
// $Id: APBERRSLV.h,v 1.6 2013/06/11 08:59:37 dungvannguyen Exp $
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

#ifndef __APBERRSLV_H__
#define __APBERRSLV_H__
#include "BusSlaveFuncIf.h"
#include "CmErrMsg.h" // for WinMsg
#include "Common.h"
#include "ApgAgentController.h"
#include "apberrslv_regif.h"

/// APBERRSLV model class
class APBERRSLV: public Capberrslv_regif
                ,public BusSlaveFuncIf
{
public:
    APBERRSLV(std::string name, 
              ApgAgentController *AgentController_ptr,
              unsigned int version = 0);
    ~APBERRSLV();

    void Reset(const bool is_active);
    void InformError(TlmBasicPayload &trans);
    void HandleErrInfo();
    void DumpStatusInfo(void);
    std::string RegisterHandler(const std::vector<std::string> &args);

    // virtual functions of BusSlaveFuncIf class
    void read(unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug);
    void write(unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug);
private:
    struct TransInfo
    {
        unsigned int peid;
        unsigned int vcid;
        unsigned int spid;
        bool um;
        bool vm;
        unsigned int strb;
        bool is_write;
        unsigned int addr;
        TransInfo(unsigned int _peid=0,
                  unsigned int _vcid=0,
                  unsigned int _spid=0,
                  bool _um=false,
                  bool _vm=false,
                  unsigned int _strb=0,
                  bool _is_write=false,
                  unsigned int _addr=0
                  )
        {
            this->peid = _peid;
            this->vcid = _vcid;
            this->spid = _spid;
            this->um = _um;
            this->vm = _vm;
            this->strb = _strb;
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
            strb = 0;
            is_write = false;
            addr = 0;
        }
    };
    ApgAgentController *mAgentController;
    TransInfo mTempTransInfo; 

    void cb_CTL_CLRE(RegCBstr str);
};

#endif //__APBERRSLV_H__
