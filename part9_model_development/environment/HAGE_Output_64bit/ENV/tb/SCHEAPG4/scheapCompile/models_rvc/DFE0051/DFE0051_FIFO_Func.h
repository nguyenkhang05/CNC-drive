// ---------------------------------------------------------------------
// $Id: DFE0051_FIFO_Func.h,v 1.2 2017/07/21 08:48:22 chuonghoangle Exp $
//
// Copyright(c) 2016 Renesas Electronics Corporation
// Copyright(c) 2016 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------
#ifndef DFE0051_FIFO_FUNC_H_
#define DFE0051_FIFO_FUNC_H_

/// forward declaration of classes in a relationship
class DFE0051_FIFO;
class DFE0051_FIFO_Func;
/// the wrapper class
#include "DFE0051_FIFO.h"

/// super class that incorporates the register block
#include "dfe0051_fifo_regif.h"
#include <stdint.h>
#include <string>
using std::string;

#include "BusSlaveFuncIf.h"

/**@brief DFE0051_FIFO_Func is a core, a function model, a register block,
 *        a bus-slave-FUNCTION model
 */
class DFE0051_FIFO_Func: public Cdfe0051_fifo_regif
                   , public BusSlaveFuncIf {
friend class DFE0051_FIFO;
//------------------------------------------------------------------------------
// public attributes
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// internal attributes
//------------------------------------------------------------------------------
private:
    DFE0051_FIFO* pWrapper;    ///< implement relationship between the core and the wrapper
    string    mModuleName; ///< name of the function model

//------------------------------------------------------------------------------
// public interface
//------------------------------------------------------------------------------
public:
    DFE0051_FIFO_Func(string     iname
                 ,DFE0051_FIFO*  pWrapper);
    virtual ~DFE0051_FIFO_Func();

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
    void read  (uint32_t            offsetAddress
               ,TlmBasicPayload&    trans
               ,BusTime_t*          t
               ,bool                debug);
    void write (uint32_t            offsetAddress
               ,TlmBasicPayload&    trans
               ,BusTime_t*          t
               ,bool                debug);
    void EnableReset(const bool& isActive);
//------------------------------------------------------------------------------
// implement pure virtual call-back function, the means allowing base class
// register block Catu5_timerg_regif inform the derived class
// Because of these are used internally in communication between the base and
// the derived class, they are private.
//------------------------------------------------------------------------------
private:
    void cb_DFBFACCTL_AEN   (RegCBstr str);
    void cb_DFBFACCTL_AIEO  (RegCBstr str);
    void cb_DFBFACCTL_AIEE  (RegCBstr str);
    void cb_DFBFACCTL_AUNE  (RegCBstr str);
    void cb_DFBFACTLCH_CHEN (RegCBstr str);
    void cb_DFBFACTLCH_ADSL (RegCBstr str);
    void cb_DFBFACTLCH_BFACH(RegCBstr str);
    void cb_DFBFADOCH_ADO   (RegCBstr str);
    void cb_DFBFACLR_CLRA   (RegCBstr str);
    void cb_DFBFACST_OVFA   (RegCBstr str);
    void cb_DFBFACST_EMPA   (RegCBstr str);
    void cb_DFBFBCTL_BEN    (RegCBstr str);
    void cb_DFBFBCTL_BIEO   (RegCBstr str);
    void cb_DFBFBCTL_BIEE   (RegCBstr str);
    void cb_DFBFBCTL_BDSL   (RegCBstr str);
    void cb_DFBFBCTL_BFBCH  (RegCBstr str);
    void cb_DFBFBDO_BDO     (RegCBstr str);
    void cb_DFBFBCLR_CLRB   (RegCBstr str);
    void cb_DFBFBST_OVFB    (RegCBstr str);
    void cb_DFBFBST_EMPB    (RegCBstr str);
};

#endif /* DFE0051_FIFO_FUNC_H_ */
