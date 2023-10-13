// ---------------------------------------------------------------------
// $Id: HWCORE_HANDSHAKE_Func.h,v 1.3 2020/09/07 06:18:13 huyquocpham Exp $
//
// Copyright(c) 2020 Renesas Electronics Corporation
// Copyright(c) 2020 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------
// Description: Copyright of .HWCORE_HANDSHAKE_Func.h
// Ref: {EMU3S_HWCORE_UD_Copyright_001}

#ifndef _HWCORE_HANDSHAKE_FUNC_H_
#define _HWCORE_HANDSHAKE_FUNC_H_
#include "CRWInterface.h"
#include "hwcore_handshake_regif.h"
class EMU3S_HWCORE_AgentController;
// Description: HWCORE_HANDSHAKE_Func class
// Ref: {EMU3S_HWCORE_UD_ATT1_003, EMU3S_HWCORE_UD_ATT1_006, EMU3S_HWCORE_UD_ATT1_008}
class HWCORE_HANDSHAKE_Func : public Chwcore_handshake_regif
                            , public CRWInterface
{
public:
    /// Constructor & Destructor
    HWCORE_HANDSHAKE_Func(std::string name, EMU3S_HWCORE_AgentController* pEMU3S_HWCORE_AgentController);
    ~HWCORE_HANDSHAKE_Func();

    /// Functions
    void EnableReset(const bool is_active);
    void RegisterHandler(const std::vector<std::string> &args);

    void Initialize(void);
    
    void read(unsigned int offsetAddress, unsigned char* data_ptr, unsigned int size, bool debug);
    void write(unsigned int offsetAddress, unsigned char* data_ptr, unsigned int size, bool debug);
private:
    /// Variables
    EMU3S_HWCORE_AgentController*               mpEMU3S_HWCORE_AgentController;
    std::string                                 mModuleName;               // Module name
    unsigned int* DATSETBUF;

    void TransferToBuffer(unsigned char set_index);
    void TransferFromBuffer(unsigned char set_index);
    
    /// Overwrite virtual functions of HWCORE_HANDSHAKE_regif class
    void cb_DATSETWB_TR0(RegCBstr str);
    void cb_DATSETBR_TR0(RegCBstr str);
};
#endif
