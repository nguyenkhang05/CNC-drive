// ---------------------------------------------------------------------
// $Id: G4_OR_IPIR.h,v 1.3 2019/07/10 11:25:13 nhuvo Exp $
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
#ifndef __G4ORIPIR_H__
#define __G4ORIPIR_H__
#include <systemc.h>
#include "re_define.h"
#include <stdint.h>
#include "global.h"
#include "G4_intc_def.h"

class G4_OR_IPIR: public sc_module
{
public:
    // Ports
    sc_in<bool>*    pe_eiint[EI_INTC1_CH_NUM];
    sc_in<bool>*    intc2_broadcast[INTC_BCAST_NUM];
    sc_in<bool>*    ipir_int_pe_ch[INTC_IPIR_CH_NUM];

    sc_out<bool>*   out_pe_ch[INTC1_CH_NUM];

    SC_HAS_PROCESS(G4_OR_IPIR);
    G4_OR_IPIR(sc_module_name name);
    ~G4_OR_IPIR();
private:
    void ORMethod();
};
#endif //__G4_OR_IPIR_H__
