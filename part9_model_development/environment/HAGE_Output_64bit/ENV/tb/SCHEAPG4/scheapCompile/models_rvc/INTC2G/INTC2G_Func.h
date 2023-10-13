// ---------------------------------------------------------------------
// $Id: INTC2G_Func.h,v 1.2 2019/09/19 10:16:17 ngathunguyen Exp $
//
// Copyright(c) 2017-2019 Renesas Electronics Corporation
// Copyright(c) 2017-2019 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------

#ifndef INTC2G_INTC2G_FUNC_H_
#define INTC2G_INTC2G_FUNC_H_

#include "INTC2G.h"
/// super class that incorporates the register block
#include "intc2g_regif.h"
/// the interface defines read and write transaction APIs
#include "BusSlaveFuncIf.h"
/// utility libraries
#include <stdint.h>
#include <string>
#include "global.h"
using std::string;

/// forward declaration of classes in a relationship
class INTC2G;
/**@brief INTC2G_Func is a core, a function model, a register block,
 *        a bus-slave-FUNCTION model
 */
class INTC2G_Func: public BusSlaveFuncIf
                 , public Cintc2g_regif
{
friend class INTC2G;

//------------------------------------------------------------------------------
// internal attributes
//------------------------------------------------------------------------------
private:
    INTC2G*             pWrapper;       ///< implement relationship between the core and the wrapper
    string              mModuleName;    ///< name of the function model
    uint32_t            mINTC2GPROT_num;
    const uint32_t      KEY_CODE_PROT;  ///< the key code protect illegal write into validating setting registers
    
//------------------------------------------------------------------------------
// public interface
//------------------------------------------------------------------------------
public:
    INTC2G_Func(string iname, INTC2G* ipWrapper, uint32_t INTC2GPROT_m_num);
    virtual ~INTC2G_Func();
    void RegisterHandler (const std::vector<std::string>& args);
    
private:
//------------------------------------------------------------------------------
// implement the role of a BusSlaveFuncIf the interface allows read/write
// access into register block from the Initiator via the BusSlaveIf (the wrapper)
//------------------------------------------------------------------------------
    void read  (uint32_t            offsetAddress
               ,TlmBasicPayload&    trans
               ,BusTime_t*          t
               ,bool                debug);
    void write (uint32_t            offsetAddress
               ,TlmBasicPayload&    trans
               ,BusTime_t*          t
               ,bool                debug);
    uint32_t GetINTC2GERRSTAT();
    void SetINTC2GPROTmNum(unsigned int channelnum);

//------------------------------------------------------------------------------
// implement pure virtual call-back function, the means allowing base class
// register block Cintc2g_regif inform the derived class
//------------------------------------------------------------------------------
    void cb_INTC2GKCPROT_KCE(RegCBstr str);
    void cb_INTC2GERRCLR_CLRE(RegCBstr str);
    void cb_INTC2GMPID_SPID(RegCBstr str);
    void cb_INTC2GPROT_GR_RG(RegCBstr str);
    void cb_INTC2GPROT_RG(RegCBstr str);
};

#endif /*INTC2G_INTC2G_FUNC_H_*/
