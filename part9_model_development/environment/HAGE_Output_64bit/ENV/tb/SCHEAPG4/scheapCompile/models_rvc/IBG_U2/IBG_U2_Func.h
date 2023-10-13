// ---------------------------------------------------------------------
// $Id: IBG_U2_Func.h,v 1.2 2020/02/26 06:51:33 duongphan Exp $
//
// Copyright(c) 2018-2020 Renesas Electronics Corporation
// Copyright(c) 2018-2020 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------

#ifndef IBG_U2_IBG_U2_FUNC_H_
#define IBG_U2_IBG_U2_FUNC_H_

/// forward declaration of classes in a relationship
class IBG_U2;
class IBG_U2_Func;
/// the wrapper class
#include "IBG_U2.h"
/// super class that incorporates the register block
#include "ibg_u2_regif.h"

// Add include "global" by Quang
#include "global.h"

/// the interface defines read and write transaction APIs
#include "BusSlaveFuncIf.h"
/// utility libraries
#include <stdint.h>
#include <string>

// Added by Quang
#include "IBG_U2_CommonGuardAgentControllerIf.h"
//
using std::string;

/**@brief IBG_U2_Func is a core, a function model, a register block,
 *        a bus-slave-FUNCTION model
 */
class IBG_U2_Func : public Cibg_u2_regif
               , public BusSlaveFuncIf {
friend class IBG_U2;
//------------------------------------------------------------------------------
// constants and data types
//------------------------------------------------------------------------------
private:

//Added by Quang
    IBG_U2_CommonGuardAgentControllerIf *mIBG_U2_CommonGuardAgentControllerIf;
     
    const uint32_t KEY_CODE_PROT; ///< the key code protect illegal write into validating setting registers
//------------------------------------------------------------------------------
// public attributes
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// internal attributes
//------------------------------------------------------------------------------
private:
    IBG_U2*    pWrapper;    ///< implement relationship between the core and the wrapper
    string  mModuleName; ///< name of the function model
//------------------------------------------------------------------------------
// public interface
//------------------------------------------------------------------------------
public:
    IBG_U2_Func(string iname, IBG_U2* ipWrapper, uint8_t ichanNum, IBG_U2_CommonGuardAgentControllerIf *IBG_U2_CommonGuardAgentControllerIfPtr);
    virtual ~IBG_U2_Func();
    void RegisterHandler (const std::vector<std::string>& args);
//------------------------------------------------------------------------------
// implement the role of a BusSlaveFuncIf the interface allows read/write
// access into register block from the Initiator via the BusSlaveIf (the wrapper)
//------------------------------------------------------------------------------
public:
    void read  (uint32_t            offsetAddress
               ,TlmBasicPayload&    trans
               ,BusTime_t*          t
               ,bool                debug);
    void write (uint32_t            offsetAddress
               ,TlmBasicPayload&    trans
               ,BusTime_t*          t
               ,bool                debug);
    void EnableReset(const bool& isActive);
    void EnablePRESETnReset(const bool& isActive);
    void UpdateErrorTransInfo(const uint32_t  addr
                              ,const bool      isDebug
                              ,const bool      isUserMode
                              ,const bool      isWrite
                              ,const uint8_t   SPID
                              );
    void NotifyError();
    uint32_t GetGSPIDERRSTAT();
    uint32_t GetGOVFSTAT();
//------------------------------------------------------------------------------
// implement pure virtual call-back function, the means allowing base class
// register block Cibg_regif inform the derived class
// Because of these are used internally in communication between the base and
// the derived class, they are private.
//------------------------------------------------------------------------------
    
    void cb_GOVFCLR_CLRO(RegCBstr str);
    void cb_GSPIDERRCLR_SPIDCLR(RegCBstr str);
    void cb_GKCPROT_KCE(RegCBstr str);
    void cb_GPROT0_RG(RegCBstr str);
    void cb_GPROT1_SPID(RegCBstr str);
};

#endif /* IBG_U2_IBG_U2_FUNC_H_ */
