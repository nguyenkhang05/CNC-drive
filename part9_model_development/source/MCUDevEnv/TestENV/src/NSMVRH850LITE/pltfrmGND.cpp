/*************************************************************************
 *
 *  pltfrmGND.cpp
 *
 * Copyright(c) 2016-2018 Renesas Electronics Corporation
 * RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
 * This program must be used solely for the purpose for which
 * it was furnished by Renesas Electronics Corporation. No part of this
 * program may be reproduced or disclosed to others, in any
 * form, without the prior written permission of Renesas Electronics
 * Corporation.
 *
 *************************************************************************/

 //************* User include header *************//
#include "NSMVG4SSLITE.h"

using namespace sc_dt;

extern sc_signal<uint64, sc_core::SC_UNCHECKED_WRITERS> *glb_pe_clk_sig; // temporary implementation before CLK-GEN being made

namespace PltfrmGnd
{
    //temporal implementation before resetIP being made
    sc_signal<bool>  *reset_sig;
}

using namespace PltfrmGnd;

//************* pltfrm *************//

/// <summary>platform connection function for unused ports</summary>
/// <param name="G4SS">G4SS pointer</param>
/// <param name="pfRh850">PFRH850 pointer</param>
/// <param name="config_file">configration file name</param>
void pltfrmGND(NSMVG4SSLITE *G4SS, void *pfRh850, const char *config_file = NULL)
{
#if defined(PLTFRM_TST)
    cout << "pltfrmGND() was called." << endl;
#endif //PLTFRM_TST

    char sname[64];


    //----- reset connection
    reset_sig = new sc_signal<bool>("reset_sig");
    *reset_sig = true;
    if (!pfRh850) {
        G4SS->sys_reset(*reset_sig);
    }  // added by request from ASTC

    G4SS->GAPB_SG1_clk(*glb_pe_clk_sig);
}

/// <summary>delete function</summary>
void pltfrmGNDDelete()
{

#if defined(PLTFRM_TST)
    cout << "pltfrmGNDDelete() was called." << endl;
#endif //PLTFRM_TST

    delete reset_sig;
}

