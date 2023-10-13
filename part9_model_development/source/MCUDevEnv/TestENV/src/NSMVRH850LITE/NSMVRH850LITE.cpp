/***********************************************************************
*
* NSMVRH850LITE.cpp
*
* Copyright(c) 2016-2021 Renesas Electronics Corporation
* RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
* This program must be used solely for the purpose for which
* it was furnished by Renesas Electronics Corporation. No part of this
* program may be reproduced or disclosed to others, in any
* form, without the prior written permission of Renesas Electronics
* Corporation.
*
***********************************************************************/

#include "systemc.h"
#include "NSMVRH850LITE.h"
#include "NSMVG4SSLITE.h"

typedef NSMVG4SSLITE NSMVG4SSV01;
typedef void PFRH850;

extern void pltfrm(NSMVG4SSV01 *G4SS, PFRH850 *pfRh850, const char *config_file);
extern void pltfrmDelete();
extern void pltfrmGND(NSMVG4SSV01 *G4SS, PFRH850 *pfRh850, const char *config_file);
extern void pltfrmGNDDelete();

NSMVRH850LITE::NSMVRH850LITE(sc_module_name moduleName, const char *configFile):
    sc_module(moduleName)
    , G4SS(0)
{
    G4SS = new NSMVG4SSLITE("G4SS", configFile);
    pltfrm(G4SS, NULL, configFile);
    pltfrmGND(G4SS, NULL, configFile);
};

NSMVRH850LITE::~NSMVRH850LITE(void)
{
    pltfrmDelete();
    pltfrmGNDDelete();
    delete G4SS;
};
