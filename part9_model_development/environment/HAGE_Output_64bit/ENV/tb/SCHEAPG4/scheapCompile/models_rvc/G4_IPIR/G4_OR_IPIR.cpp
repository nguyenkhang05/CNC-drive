// ---------------------------------------------------------------------
// $Id: G4_OR_IPIR.cpp,v 1.4 2019/07/10 11:25:13 nhuvo Exp $
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

#include "G4_OR_IPIR.h"

/// Constructor and Destructor
G4_OR_IPIR::G4_OR_IPIR(sc_module_name name) :sc_module(name)
{//{{{
    // Create instances of ports and initialize port
    char inst_name[128];
    for (int i = 0; i < EI_INTC1_CH_NUM; i++){
        sprintf(inst_name, "pe_eiint%d", i);
        pe_eiint[i] = new sc_in<bool>(inst_name);
    }
    for (int i = 0; i < INTC_BCAST_NUM; i++){
        sprintf(inst_name, "intc2_broadcast%d", i);
        intc2_broadcast[i] = new sc_in<bool>(inst_name);
    }
    for (int i = 0; i < INTC_IPIR_CH_NUM; i++){
        sprintf(inst_name, "ipir_int_pe_ch%d", i);
        ipir_int_pe_ch[i] = new sc_in<bool>(inst_name);
    }
    for (int i = 0; i < INTC1_CH_NUM; i++){
        sprintf(inst_name, "out_pe_ch%d", i);
        out_pe_ch[i] = new sc_out<bool>(inst_name);
        out_pe_ch[i]->initialize(false);
    }
    
    SC_METHOD(ORMethod);
    dont_initialize();
    for (int i = 0; i < EI_INTC1_CH_NUM; i++){
        sensitive << (*pe_eiint[i]);
    }
    for (int i = 0; i < INTC_IPIR_CH_NUM; i++){
        sensitive << (*ipir_int_pe_ch[i]);
    }
    for (int i = 0; i < INTC_BCAST_NUM; i++){
        sensitive << (*intc2_broadcast[i]);
    }
}//}}}

/// Destructor
G4_OR_IPIR::~G4_OR_IPIR()
{//{{{
    for (int i = 0; i < EI_INTC1_CH_NUM; i++){
        delete pe_eiint[i];
    }
    for (int i = 0; i < INTC1_CH_NUM; i++){
        delete out_pe_ch[i];
    }
    for (int i = 0; i < INTC_BCAST_NUM; i++){
        delete intc2_broadcast[i];
    }
    for (int i = 0; i < INTC_IPIR_CH_NUM; i++){
        delete ipir_int_pe_ch[i];
    }
}//}}}

/// ORMethod merges all inputs and issue outputs
void G4_OR_IPIR::ORMethod()
{//{{{
    bool tempsig[INTC1_CH_NUM];
    // OR and Merge
    for (int i = 0; i < INTC_IPIR_CH_NUM; i++){
        tempsig[i] = ipir_int_pe_ch[i]->read() | pe_eiint[i]->read();
    }
    for (int i = 0; i < INTC_BCAST_NUM; i++){
        tempsig[INTC_IPIR_CH_NUM + i] = intc2_broadcast[i]->read() | pe_eiint[INTC_IPIR_CH_NUM + i]->read();
    }
    for (int i = INTC_IPIR_CH_NUM + INTC_BCAST_NUM; i < INTC1_CH_NUM; i++){
        tempsig[i] = pe_eiint[i]->read();
    }
    // Write output
    for (int i = 0; i < INTC1_CH_NUM; i++){
        out_pe_ch[i]->write(tempsig[i]);
    }
}//}}}

// vim600: set foldmethod=marker :
