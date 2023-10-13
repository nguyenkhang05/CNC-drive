// ---------------------------------------------------------------------
// $Id: IBG_Func.h,v 1.6 2019/05/30 01:49:33 nhuvo Exp $
//
// Copyright(c) 2016-2019 Renesas Electronics Corporation
// Copyright(c) 2016-2019 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------

#ifndef IBG_IBG_FUNC_H_
#define IBG_IBG_FUNC_H_

/// forward declaration of classes in a relationship
class IBG;
class IBG_Func;
/// the wrapper class
#include "IBG.h"
/// super class that incorporates the register block
#include "ibg_regif.h"
/// the interface defines read and write transaction APIs
#include "BusSlaveFuncIf.h"
/// utility libraries
#include <stdint.h>
#include <string>
using std::string;

/**@brief IBG_Func is a core, a function model, a register block,
*        a bus-slave-FUNCTION model
*/
class IBG_Func : public Cibg_regif
    , public BusSlaveFuncIf {
        friend class IBG;
        //------------------------------------------------------------------------------
        // constants and data types
        //------------------------------------------------------------------------------
private:
    const uint32_t KEY_CODE_PROT; ///< the key code protect illegal write into validating setting registers
    //------------------------------------------------------------------------------
    // public attributes
    //------------------------------------------------------------------------------

    //------------------------------------------------------------------------------
    // internal attributes
    //------------------------------------------------------------------------------
private:
    IBG*    pWrapper;    ///< implement relationship between the core and the wrapper
    string  mModuleName; ///< name of the function model
    //------------------------------------------------------------------------------
    // public interface
    //------------------------------------------------------------------------------
public:
    IBG_Func(string iname, IBG* ipWrapper, uint iChannel);
    virtual ~IBG_Func();
    void RegisterHandler (const std::vector<std::string>& args);
    //------------------------------------------------------------------------------
    // helper methods
    //------------------------------------------------------------------------------
private:
    void Initialize ();
    void CancelEvent();
    bool validate_configuration();
    //------------------------------------------------------------------------------
    // implement the role of a BusSlaveFuncIf the interface allows read/write
    // access into register block from the Initiator via the BusSlaveIf (the wrapper)
    //------------------------------------------------------------------------------
public:
    uint32_t GetGERRSTAT();
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
    //------------------------------------------------------------------------------
    // implement pure virtual call-back function, the means allowing base class
    // register block Catu5_timerg_regif inform the derived class
    // Because of these are used internally in communication between the base and
    // the derived class, they are private.
    //------------------------------------------------------------------------------
    void cb_GERRCLR_CLRE(RegCBstr str);
    void cb_GERRCLR_CLRO(RegCBstr str);
    void cb_GKCPROT_KCE(RegCBstr str);
    void cb_GKCPROT_KCPROT(RegCBstr str);
    void cb_GPROT0_RG(RegCBstr str);
    void cb_GPROT0_WG(RegCBstr str);
    void cb_GPROT1_SPID(RegCBstr str);
};

#endif /* IBG_IBG_FUNC_H_ */
