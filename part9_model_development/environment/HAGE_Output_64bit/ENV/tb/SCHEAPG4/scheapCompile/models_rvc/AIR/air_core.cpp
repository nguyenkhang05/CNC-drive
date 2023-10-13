// ----------------------------------------------------------------------
// $Id: $
//
// Copyright(c) 2016 - 2017 Renesas System Design Co., Ltd.
// Copyright(c) 2016 - 2017 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ----------------------------------------------------------------------

#include "air_core.h"
#include "air.h"

// Constructor of Cair_core class
Cair_core::Cair_core(std::string name, Cair *air): Cair_core_regif(name, 32)
{//{{{
    assert(air != NULL);
    pAirWrapper = air;

    // INT
    mux_table_int.push_back(air_mux_table(0, &(*AIR_ISELR0)["ISEL0"] , emResource0, 0 , 0));//ADMPXI0;
    mux_table_int.push_back(air_mux_table(0, &(*AIR_ISELR0)["ISEL2"] , emResource0, 2 , 0));//ADMPXI1;
    mux_table_int.push_back(air_mux_table(0, &(*AIR_ISELR0)["ISEL1"] , emResource0, 1 , 0));//ADMPXI2;
    mux_table_int.push_back(air_mux_table(0, &(*AIR_ISELR0)["ISEL3"] , emResource0, 3 , 0));//ADMPXI3;
    mux_table_int.push_back(air_mux_table(0, &(*AIR_ISELR0)["ISEL4"] , emResource0, 4 , 0));//ADI00;
    mux_table_int.push_back(air_mux_table(0, &(*AIR_ISELR0)["ISEL5"] , emResource0, 5 , 0));//ADI01;
    mux_table_int.push_back(air_mux_table(0, &(*AIR_ISELR0)["ISEL6"] , emResource0, 6 , 0));//ADI02;
    mux_table_int.push_back(air_mux_table(0, &(*AIR_ISELR0)["ISEL7"] , emResource0, 7 , 0));//ADI03;
    mux_table_int.push_back(air_mux_table(0, &(*AIR_ISELR0)["ISEL8"] , emResource0, 8 , 0));//ADI04;
    mux_table_int.push_back(air_mux_table(0, &(*AIR_ISELR0)["ISEL14"], emResource0, 14, 0));//ADI10;
    mux_table_int.push_back(air_mux_table(0, &(*AIR_ISELR0)["ISEL15"], emResource0, 15, 0));//ADI11;
    mux_table_int.push_back(air_mux_table(0, &(*AIR_ISELR0)["ISEL16"], emResource0, 16, 0));//ADI12;
    mux_table_int.push_back(air_mux_table(0, &(*AIR_ISELR0)["ISEL17"], emResource0, 17, 0));//ADI13;
    mux_table_int.push_back(air_mux_table(0, &(*AIR_ISELR0)["ISEL18"], emResource0, 18, 0));//ADI14;
    mux_table_int.push_back(air_mux_table(0, &(*AIR_ISELR0)["ISEL9"] , emResource0, 9 , 0));//ADI20;
    mux_table_int.push_back(air_mux_table(0, &(*AIR_ISELR0)["ISEL10"], emResource0, 10, 0));//ADI21;
    mux_table_int.push_back(air_mux_table(0, &(*AIR_ISELR0)["ISEL11"], emResource0, 11, 0));//ADI22;
    mux_table_int.push_back(air_mux_table(0, &(*AIR_ISELR0)["ISEL12"], emResource0, 12, 0));//ADI23;
    mux_table_int.push_back(air_mux_table(0, &(*AIR_ISELR0)["ISEL13"], emResource0, 13, 0));//ADI24;
    mux_table_int.push_back(air_mux_table(0, &(*AIR_ISELR0)["ISEL19"], emResource0, 19, 0));//ADI30;
    mux_table_int.push_back(air_mux_table(0, &(*AIR_ISELR0)["ISEL20"], emResource0, 20, 0));//ADI31;
    mux_table_int.push_back(air_mux_table(0, &(*AIR_ISELR0)["ISEL21"], emResource0, 21, 0));//ADI32;
    mux_table_int.push_back(air_mux_table(0, &(*AIR_ISELR0)["ISEL22"], emResource0, 22, 0));//ADI33;
    mux_table_int.push_back(air_mux_table(0, &(*AIR_ISELR0)["ISEL23"], emResource0, 23, 0));//ADI34;
    mux_table_int.push_back(air_mux_table(0, &(*AIR_ISELR0)["ISEL24"], emResource0, 24, 0));//ADE0;
    mux_table_int.push_back(air_mux_table(0, &(*AIR_ISELR0)["ISEL26"], emResource0, 26, 0));//ADE1;
    mux_table_int.push_back(air_mux_table(0, &(*AIR_ISELR0)["ISEL25"], emResource0, 25, 0));//ADE2;
    mux_table_int.push_back(air_mux_table(0, &(*AIR_ISELR0)["ISEL27"], emResource0, 27, 0));//ADE3;
    mux_table_int.push_back(air_mux_table(0, &(*AIR_ISELR0)["ISEL28"], emResource0, 28, 0));//DSADE00;
    mux_table_int.push_back(air_mux_table(0, &(*AIR_ISELR0)["ISEL29"], emResource0, 29, 0));//DSADE10;
    mux_table_int.push_back(air_mux_table(0, &(*AIR_ISELR0)["ISEL30"], emResource0, 30, 0));//DSADE20;
    mux_table_int.push_back(air_mux_table(0, &(*AIR_ISELR1)["ISEL33"], emResource0, 33, 0));//DSADE11;
    mux_table_int.push_back(air_mux_table(0, &(*AIR_ISELR0)["ISEL31"], emResource0, 31, 0));//DSADE12;
    mux_table_int.push_back(air_mux_table(0, &(*AIR_ISELR1)["ISEL32"], emResource0, 32, 0));//DSADE13;
    mux_table_int.push_back(air_mux_table(0, NULL, emNotResource, 0, 0));//DSADI00;
    mux_table_int.push_back(air_mux_table(0, NULL, emNotResource, 0, 0));//DSADI10;
    mux_table_int.push_back(air_mux_table(0, NULL, emNotResource, 0, 0));//DSADI20;
    mux_table_int.push_back(air_mux_table(0, NULL, emNotResource, 0, 0));//DSADI11;
    mux_table_int.push_back(air_mux_table(0, NULL, emNotResource, 0, 0));//DSADI12;
    mux_table_int.push_back(air_mux_table(0, NULL, emNotResource, 0, 0));//DSADI13;
    mux_table_int.push_back(air_mux_table(0, &(*AIR_ISELR1)["ISEL38"], emResource0, 38, 0));//CADE00;
    mux_table_int.push_back(air_mux_table(0, NULL, emNotResource, 0, 0));//CADI00;
    mux_table_int.push_back(air_mux_table(0, &(*AIR_ISELR1)["ISEL39"], emResource0, 39, 0));//ASI0;
    mux_table_int.push_back(air_mux_table(0, &(*AIR_ISELR1)["ISEL40"], emResource0, 40, 0));//ASI1;
    mux_table_int.push_back(air_mux_table(0, &(*AIR_ISELR1)["ISEL41"], emResource0, 41, 0));//ASI2;
    mux_table_int.push_back(air_mux_table(0, &(*AIR_ISELR1)["ISEL42"], emResource0, 42, 0));//ASI3;
    mux_table_int.push_back(air_mux_table(0, &(*AIR_ISELR1)["ISEL43"], emResource0, 43, 0));//ASI4;
    mux_table_int.push_back(air_mux_table(0, &(*AIR_ISELR1)["ISEL44"], emResource0, 44, 0));//ASI5;
    mux_table_int.push_back(air_mux_table(0, &(*AIR_ISELR1)["ISEL45"], emResource0, 45, 0));//ASI6;
    mux_table_int.push_back(air_mux_table(0, &(*AIR_ISELR1)["ISEL46"], emResource0, 46, 0));//ASI7;
    mux_table_int.push_back(air_mux_table(0, &(*AIR_ISELR1)["ISEL47"], emResource0, 47, 0));//ASI8;
    mux_table_int.push_back(air_mux_table(0, &(*AIR_ISELR1)["ISEL48"], emResource0, 48, 0));//ASI9;
    mux_table_int.push_back(air_mux_table(0, &(*AIR_ISELR1)["ISEL49"], emResource0, 49, 0));//ASI10;
    mux_table_int.push_back(air_mux_table(0, &(*AIR_ISELR1)["ISEL50"], emResource0, 50, 0));//ASI11;
    mux_table_int.push_back(air_mux_table(0, &(*AIR_ISELR1)["ISEL51"], emResource0, 51, 0));//ASI12;
    mux_table_int.push_back(air_mux_table(0, &(*AIR_ISELR1)["ISEL52"], emResource0, 52, 0));//ASI13;
    mux_table_int.push_back(air_mux_table(0, &(*AIR_ISELR1)["ISEL53"], emResource0, 53, 0));//ASI14;
    mux_table_int.push_back(air_mux_table(0, &(*AIR_ISELR1)["ISEL54"], emResource0, 54, 0));//ASI15;
    mux_table_int.push_back(air_mux_table(0, &(*AIR_ISELR1)["ISEL55"], emResource1, 55, 0));//BFP0;
    mux_table_int.push_back(air_mux_table(0, &(*AIR_ISELR1)["ISEL56"], emResource1, 56, 0));//BFP1;
    mux_table_int.push_back(air_mux_table(0, &(*AIR_ISELR1)["ISEL57"], emResource1, 57, 0));//BFP2;
    mux_table_int.push_back(air_mux_table(0, &(*AIR_ISELR1)["ISEL58"], emResource1, 58, 0));//BFP3;
    mux_table_int.push_back(air_mux_table(0, &(*AIR_ISELR1)["ISEL59"], emResource1, 59, 0));//BFP4;
    mux_table_int.push_back(air_mux_table(0, &(*AIR_ISELR1)["ISEL60"], emResource1, 60, 0));//BFP5;
    mux_table_int.push_back(air_mux_table(0, &(*AIR_ISELR1)["ISEL61"], emResource1, 61, 0));//BFP6;
    mux_table_int.push_back(air_mux_table(0, &(*AIR_ISELR1)["ISEL62"], emResource1, 62, 0));//BFP7;
    mux_table_int.push_back(air_mux_table(0, &(*AIR_ISELR1)["ISEL63"], emResource1, 63, 0));//BFP8;
    mux_table_int.push_back(air_mux_table(0, &(*AIR_ISELR1)["ISEL34"], emResource1, 34, 0));//BFP9;
    mux_table_int.push_back(air_mux_table(0, &(*AIR_ISELR1)["ISEL35"], emResource1, 35, 0));//BFP10;
    mux_table_int.push_back(air_mux_table(0, &(*AIR_ISELR1)["ISEL36"], emResource1, 36, 0));//BFP11;
    mux_table_int.push_back(air_mux_table(0, &(*AIR_ISELR1)["ISEL37"], emResource1, 37, 0));//BFP12;
    mux_table_int.push_back(air_mux_table(0, &(*AIR_ISELR0)["ISEL26"], emResource1, 26, 0));//BFP13;
    mux_table_int.push_back(air_mux_table(0, &(*AIR_ISELR0)["ISEL27"], emResource1, 27, 0));//BFP14;
    mux_table_int.push_back(air_mux_table(0, &(*AIR_ISELR0)["ISEL31"], emResource1, 31, 0));//BFP15;
    mux_table_int.push_back(air_mux_table(0, &(*AIR_ISELR1)["ISEL32"], emResource1, 32, 0));//BFP16;
    mux_table_int.push_back(air_mux_table(0, &(*AIR_ISELR1)["ISEL33"], emResource1, 33, 0));//BFP17;
    mux_table_int.push_back(air_mux_table(0, &(*AIR_ISELR0)["ISEL2"] , emResource1, 2 , 0));//BFP18;
    mux_table_int.push_back(air_mux_table(0, &(*AIR_ISELR0)["ISEL3"] , emResource1, 3 , 0));//BFP19;
    mux_table_int.push_back(air_mux_table(0, &(*AIR_ISELR0)["ISEL14"], emResource1, 14, 0));//BFP20;
    mux_table_int.push_back(air_mux_table(0, &(*AIR_ISELR0)["ISEL15"], emResource1, 15, 0));//BFP21;
    mux_table_int.push_back(air_mux_table(0, &(*AIR_ISELR0)["ISEL16"], emResource1, 16, 0));//BFP22;
    mux_table_int.push_back(air_mux_table(0, &(*AIR_ISELR0)["ISEL17"], emResource1, 17, 0));//BFP23;
    mux_table_int.push_back(air_mux_table(0, &(*AIR_ISELR0)["ISEL18"], emResource1, 18, 0));//BFP24;
    mux_table_int.push_back(air_mux_table(0, &(*AIR_ISELR0)["ISEL19"], emResource1, 19, 0));//BFP25;
    mux_table_int.push_back(air_mux_table(0, &(*AIR_ISELR0)["ISEL20"], emResource1, 20, 0));//BFP26;
    mux_table_int.push_back(air_mux_table(0, &(*AIR_ISELR0)["ISEL21"], emResource1, 21, 0));//BFP27;
    mux_table_int.push_back(air_mux_table(0, &(*AIR_ISELR0)["ISEL22"], emResource1, 22, 0));//BFP28;
    mux_table_int.push_back(air_mux_table(0, &(*AIR_ISELR0)["ISEL23"], emResource1, 23, 0));//BFP29;
    mux_table_int.push_back(air_mux_table(0, &(*AIR_ISELR1)["ISEL39"], emResource1, 39, 0));//BFP30;
    mux_table_int.push_back(air_mux_table(0, &(*AIR_ISELR1)["ISEL40"], emResource1, 40, 0));//BFP31;
    mux_table_int.push_back(air_mux_table(0, &(*AIR_ISELR1)["ISEL41"], emResource1, 41, 0));//BFP32;
    mux_table_int.push_back(air_mux_table(0, &(*AIR_ISELR1)["ISEL42"], emResource1, 42, 0));//BFP33;
    mux_table_int.push_back(air_mux_table(0, &(*AIR_ISELR1)["ISEL43"], emResource1, 43, 0));//BFP34;
    mux_table_int.push_back(air_mux_table(0, &(*AIR_ISELR1)["ISEL44"], emResource1, 44, 0));//BFP35;
    mux_table_int.push_back(air_mux_table(0, &(*AIR_ISELR1)["ISEL45"], emResource1, 45, 0));//BFP36;
    mux_table_int.push_back(air_mux_table(0, &(*AIR_ISELR1)["ISEL46"], emResource1, 46, 0));//BFP37;
    mux_table_int.push_back(air_mux_table(0, &(*AIR_ISELR1)["ISEL47"], emResource1, 47, 0));//BFP38;
    mux_table_int.push_back(air_mux_table(0, &(*AIR_ISELR1)["ISEL48"], emResource1, 48, 0));//BFP39;
    //Supported for E2x_FCC2
    mux_table_int.push_back(air_mux_table(0, &(*AIR_ISELR1)["ISEL35"], emResource0, 35, 0));//DSADE14;
    mux_table_int.push_back(air_mux_table(0, &(*AIR_ISELR1)["ISEL34"], emResource0, 34, 0));//DSADE15;
    mux_table_int.push_back(air_mux_table(0, &(*AIR_ISELR1)["ISEL37"], emResource0, 37, 0));//DSADE21;
    mux_table_int.push_back(air_mux_table(0, &(*AIR_ISELR1)["ISEL36"], emResource0, 36, 0));//DSADE22;
    mux_table_int.push_back(air_mux_table(0, NULL, emNotResource, 0, 0));//DSADI14;
    mux_table_int.push_back(air_mux_table(0, NULL, emNotResource, 0, 0));//DSADI15;
    mux_table_int.push_back(air_mux_table(0, NULL, emNotResource, 0, 0));//DSADI21;
    mux_table_int.push_back(air_mux_table(0, NULL, emNotResource, 0, 0));//DSADI22;
    mux_table_int.push_back(air_mux_table(0, &(*AIR_ISELR1)["ISEL49"], emResource1, 49, 0));//BFP40;
    mux_table_int.push_back(air_mux_table(0, &(*AIR_ISELR1)["ISEL50"], emResource1, 50, 0));//BFP41;
    mux_table_int.push_back(air_mux_table(0, &(*AIR_ISELR1)["ISEL51"], emResource1, 51, 0));//BFP42;
    mux_table_int.push_back(air_mux_table(0, &(*AIR_ISELR1)["ISEL52"], emResource1, 52, 0));//BFP43;
    mux_table_int.push_back(air_mux_table(0, &(*AIR_ISELR1)["ISEL53"], emResource1, 53, 0));//BFP44;
    mux_table_int.push_back(air_mux_table(0, &(*AIR_ISELR1)["ISEL54"], emResource1, 54, 0));//BFP45;
    mux_table_int.push_back(air_mux_table(0, &(*AIR_ISELR0)["ISEL10"], emResource1, 10, 0));//BFP46;
    mux_table_int.push_back(air_mux_table(0, &(*AIR_ISELR0)["ISEL11"], emResource1, 11, 0));//BFP47;

    // DMA
    mux_table_dma.push_back(air_mux_table(0, &(*AIR_DSELR0)["DSEL0"] , emResource0, 0 , 0));//ADMPXI0;
    mux_table_dma.push_back(air_mux_table(0, &(*AIR_DSELR0)["DSEL2"] , emResource0, 2 , 0));//ADMPXI1;
    mux_table_dma.push_back(air_mux_table(0, &(*AIR_DSELR0)["DSEL1"] , emResource0, 1 , 0));//ADMPXI2;
    mux_table_dma.push_back(air_mux_table(0, &(*AIR_DSELR0)["DSEL3"] , emResource0, 3 , 0));//ADMPXI3;
    mux_table_dma.push_back(air_mux_table(0, &(*AIR_DSELR0)["DSEL4"] , emResource0, 4 , 0));//ADI00;
    mux_table_dma.push_back(air_mux_table(0, &(*AIR_DSELR0)["DSEL5"] , emResource0, 5 , 0));//ADI01;
    mux_table_dma.push_back(air_mux_table(0, &(*AIR_DSELR0)["DSEL6"] , emResource0, 6 , 0));//ADI02;
    mux_table_dma.push_back(air_mux_table(0, &(*AIR_DSELR0)["DSEL7"] , emResource0, 7 , 0));//ADI03;
    mux_table_dma.push_back(air_mux_table(0, &(*AIR_DSELR0)["DSEL8"] , emResource0, 8 , 0));//ADI04;
    mux_table_dma.push_back(air_mux_table(0, &(*AIR_DSELR0)["DSEL14"], emResource0, 14, 0));//ADI10;
    mux_table_dma.push_back(air_mux_table(0, &(*AIR_DSELR0)["DSEL15"], emResource0, 15, 0));//ADI11;
    mux_table_dma.push_back(air_mux_table(0, &(*AIR_DSELR0)["DSEL16"], emResource0, 16, 0));//ADI12;
    mux_table_dma.push_back(air_mux_table(0, &(*AIR_DSELR0)["DSEL17"], emResource0, 17, 0));//ADI13;
    mux_table_dma.push_back(air_mux_table(0, &(*AIR_DSELR0)["DSEL18"], emResource0, 18, 0));//ADI14;
    mux_table_dma.push_back(air_mux_table(0, &(*AIR_DSELR0)["DSEL9"] , emResource0, 9 , 0));//ADI20;
    mux_table_dma.push_back(air_mux_table(0, &(*AIR_DSELR0)["DSEL10"], emResource0, 10, 0));//ADI21;
    mux_table_dma.push_back(air_mux_table(0, &(*AIR_DSELR0)["DSEL11"], emResource0, 11, 0));//ADI22;
    mux_table_dma.push_back(air_mux_table(0, &(*AIR_DSELR0)["DSEL12"], emResource0, 12, 0));//ADI23;
    mux_table_dma.push_back(air_mux_table(0, &(*AIR_DSELR0)["DSEL13"], emResource0, 13, 0));//ADI24;
    mux_table_dma.push_back(air_mux_table(0, &(*AIR_DSELR0)["DSEL19"], emResource0, 19, 0));//ADI30;
    mux_table_dma.push_back(air_mux_table(0, &(*AIR_DSELR0)["DSEL20"], emResource0, 20, 0));//ADI31;
    mux_table_dma.push_back(air_mux_table(0, &(*AIR_DSELR0)["DSEL21"], emResource0, 21, 0));//ADI32;
    mux_table_dma.push_back(air_mux_table(0, &(*AIR_DSELR0)["DSEL22"], emResource0, 22, 0));//ADI33;
    mux_table_dma.push_back(air_mux_table(0, &(*AIR_DSELR0)["DSEL23"], emResource0, 23, 0));//ADI34;
    mux_table_dma.push_back(air_mux_table(0, NULL, emNotResource, 0, 0));//ADE0;
    mux_table_dma.push_back(air_mux_table(0, NULL, emNotResource, 0, 0));//ADE1;
    mux_table_dma.push_back(air_mux_table(0, NULL, emNotResource, 0, 0));//ADE2;
    mux_table_dma.push_back(air_mux_table(0, NULL, emNotResource, 0, 0));//ADE3;
    mux_table_dma.push_back(air_mux_table(0, NULL, emNotResource, 0, 0));//DSADE00;
    mux_table_dma.push_back(air_mux_table(0, NULL, emNotResource, 0, 0));//DSADE10;
    mux_table_dma.push_back(air_mux_table(0, NULL, emNotResource, 0, 0));//DSADE20;
    mux_table_dma.push_back(air_mux_table(0, NULL, emNotResource, 0, 0));//DSADE11;
    mux_table_dma.push_back(air_mux_table(0, NULL, emNotResource, 0, 0));//DSADE12;
    mux_table_dma.push_back(air_mux_table(0, NULL, emNotResource, 0, 0));//DSADE13;
    mux_table_dma.push_back(air_mux_table(0, &(*AIR_DSELR0)["DSEL28"], emResource0, 28, 0));//DSADI00;
    mux_table_dma.push_back(air_mux_table(0, &(*AIR_DSELR0)["DSEL29"], emResource0, 29, 0));//DSADI10;
    mux_table_dma.push_back(air_mux_table(0, &(*AIR_DSELR0)["DSEL30"], emResource0, 30, 0));//DSADI20;
    mux_table_dma.push_back(air_mux_table(0, &(*AIR_DSELR1)["DSEL33"], emResource0, 33, 0));//DSADI11;
    mux_table_dma.push_back(air_mux_table(0, &(*AIR_DSELR0)["DSEL31"], emResource0, 31, 0));//DSADI12;
    mux_table_dma.push_back(air_mux_table(0, &(*AIR_DSELR1)["DSEL32"], emResource0, 32, 0));//DSADI13;
    mux_table_dma.push_back(air_mux_table(0, NULL, emNotResource, 0, 0));//CADE00;
    mux_table_dma.push_back(air_mux_table(0, &(*AIR_DSELR1)["DSEL38"], emResource0, 38, 0));//CADI00;
    mux_table_dma.push_back(air_mux_table(0, &(*AIR_DSELR1)["DSEL39"], emResource0, 39, 0));//ASI0;
    mux_table_dma.push_back(air_mux_table(0, &(*AIR_DSELR1)["DSEL40"], emResource0, 40, 0));//ASI1;
    mux_table_dma.push_back(air_mux_table(0, &(*AIR_DSELR1)["DSEL41"], emResource0, 41, 0));//ASI2;
    mux_table_dma.push_back(air_mux_table(0, &(*AIR_DSELR1)["DSEL42"], emResource0, 42, 0));//ASI3;
    mux_table_dma.push_back(air_mux_table(0, &(*AIR_DSELR1)["DSEL43"], emResource0, 43, 0));//ASI4;
    mux_table_dma.push_back(air_mux_table(0, &(*AIR_DSELR1)["DSEL44"], emResource0, 44, 0));//ASI5;
    mux_table_dma.push_back(air_mux_table(0, &(*AIR_DSELR1)["DSEL45"], emResource0, 45, 0));//ASI6;
    mux_table_dma.push_back(air_mux_table(0, &(*AIR_DSELR1)["DSEL46"], emResource0, 46, 0));//ASI7;
    mux_table_dma.push_back(air_mux_table(0, &(*AIR_DSELR1)["DSEL47"], emResource0, 47, 0));//ASI8;
    mux_table_dma.push_back(air_mux_table(0, &(*AIR_DSELR1)["DSEL48"], emResource0, 48, 0));//ASI9;
    mux_table_dma.push_back(air_mux_table(0, &(*AIR_DSELR1)["DSEL49"], emResource0, 49, 0));//ASI10;
    mux_table_dma.push_back(air_mux_table(0, &(*AIR_DSELR1)["DSEL50"], emResource0, 50, 0));//ASI11;
    mux_table_dma.push_back(air_mux_table(0, &(*AIR_DSELR1)["DSEL51"], emResource0, 51, 0));//ASI12;
    mux_table_dma.push_back(air_mux_table(0, &(*AIR_DSELR1)["DSEL52"], emResource0, 52, 0));//ASI13;
    mux_table_dma.push_back(air_mux_table(0, &(*AIR_DSELR1)["DSEL53"], emResource0, 53, 0));//ASI14;
    mux_table_dma.push_back(air_mux_table(0, &(*AIR_DSELR1)["DSEL54"], emResource0, 54, 0));//ASI15;
    mux_table_dma.push_back(air_mux_table(0, &(*AIR_DSELR1)["DSEL55"], emResource1, 55, 0));//BFP0;
    mux_table_dma.push_back(air_mux_table(0, &(*AIR_DSELR1)["DSEL56"], emResource1, 56, 0));//BFP1;
    mux_table_dma.push_back(air_mux_table(0, &(*AIR_DSELR1)["DSEL57"], emResource1, 57, 0));//BFP2;
    mux_table_dma.push_back(air_mux_table(0, &(*AIR_DSELR1)["DSEL58"], emResource1, 58, 0));//BFP3;
    mux_table_dma.push_back(air_mux_table(0, &(*AIR_DSELR1)["DSEL59"], emResource1, 59, 0));//BFP4;
    mux_table_dma.push_back(air_mux_table(0, &(*AIR_DSELR1)["DSEL60"], emResource1, 60, 0));//BFP5;
    mux_table_dma.push_back(air_mux_table(0, &(*AIR_DSELR1)["DSEL61"], emResource1, 61, 0));//BFP6;
    mux_table_dma.push_back(air_mux_table(0, &(*AIR_DSELR1)["DSEL62"], emResource1, 62, 0));//BFP7;
    mux_table_dma.push_back(air_mux_table(0, &(*AIR_DSELR1)["DSEL63"], emResource1, 63, 0));//BFP8;
    mux_table_dma.push_back(air_mux_table(0, &(*AIR_DSELR0)["DSEL24"], emResource1, 24, 0));//BFP9;
    mux_table_dma.push_back(air_mux_table(0, &(*AIR_DSELR0)["DSEL25"], emResource1, 25, 0));//BFP10;
    mux_table_dma.push_back(air_mux_table(0, &(*AIR_DSELR0)["DSEL26"], emResource1, 26, 0));//BFP11;
    mux_table_dma.push_back(air_mux_table(0, &(*AIR_DSELR0)["DSEL27"], emResource1, 27, 0));//BFP12;
    mux_table_dma.push_back(air_mux_table(0, &(*AIR_DSELR1)["DSEL34"], emResource1, 34, 0));//BFP13;
    mux_table_dma.push_back(air_mux_table(0, &(*AIR_DSELR1)["DSEL35"], emResource1, 35, 0));//BFP14;
    mux_table_dma.push_back(air_mux_table(0, &(*AIR_DSELR1)["DSEL36"], emResource1, 36, 0));//BFP15;
    mux_table_dma.push_back(air_mux_table(0, &(*AIR_DSELR1)["DSEL37"], emResource1, 37, 0));//BFP16;
    mux_table_dma.push_back(air_mux_table(0, &(*AIR_DSELR0)["DSEL31"], emResource1, 31, 0));//BFP17;
    mux_table_dma.push_back(air_mux_table(0, &(*AIR_DSELR0)["DSEL2"] , emResource1, 2 , 0));//BFP18;
    mux_table_dma.push_back(air_mux_table(0, &(*AIR_DSELR0)["DSEL3"] , emResource1, 3 , 0));//BFP19;
    mux_table_dma.push_back(air_mux_table(0, &(*AIR_DSELR0)["DSEL14"], emResource1, 14, 0));//BFP20;
    mux_table_dma.push_back(air_mux_table(0, &(*AIR_DSELR0)["DSEL15"], emResource1, 15, 0));//BFP21;
    mux_table_dma.push_back(air_mux_table(0, &(*AIR_DSELR0)["DSEL16"], emResource1, 16, 0));//BFP22;
    mux_table_dma.push_back(air_mux_table(0, &(*AIR_DSELR0)["DSEL17"], emResource1, 17, 0));//BFP23;
    mux_table_dma.push_back(air_mux_table(0, &(*AIR_DSELR0)["DSEL18"], emResource1, 18, 0));//BFP24;
    mux_table_dma.push_back(air_mux_table(0, &(*AIR_DSELR0)["DSEL19"], emResource1, 19, 0));//BFP25;
    mux_table_dma.push_back(air_mux_table(0, &(*AIR_DSELR0)["DSEL20"], emResource1, 20, 0));//BFP26;
    mux_table_dma.push_back(air_mux_table(0, &(*AIR_DSELR0)["DSEL21"], emResource1, 21, 0));//BFP27;
    mux_table_dma.push_back(air_mux_table(0, &(*AIR_DSELR0)["DSEL22"], emResource1, 22, 0));//BFP28;
    mux_table_dma.push_back(air_mux_table(0, &(*AIR_DSELR0)["DSEL23"], emResource1, 23, 0));//BFP29;
    mux_table_dma.push_back(air_mux_table(0, &(*AIR_DSELR1)["DSEL39"], emResource1, 39, 0));//BFP30;
    mux_table_dma.push_back(air_mux_table(0, &(*AIR_DSELR1)["DSEL40"], emResource1, 40, 0));//BFP31;
    mux_table_dma.push_back(air_mux_table(0, &(*AIR_DSELR1)["DSEL41"], emResource1, 41, 0));//BFP32;
    mux_table_dma.push_back(air_mux_table(0, &(*AIR_DSELR1)["DSEL42"], emResource1, 42, 0));//BFP33;
    mux_table_dma.push_back(air_mux_table(0, &(*AIR_DSELR1)["DSEL43"], emResource1, 43, 0));//BFP34;
    mux_table_dma.push_back(air_mux_table(0, &(*AIR_DSELR1)["DSEL44"], emResource1, 44, 0));//BFP35;
    mux_table_dma.push_back(air_mux_table(0, &(*AIR_DSELR1)["DSEL45"], emResource1, 45, 0));//BFP36;
    mux_table_dma.push_back(air_mux_table(0, &(*AIR_DSELR1)["DSEL46"], emResource1, 46, 0));//BFP37;
    mux_table_dma.push_back(air_mux_table(0, &(*AIR_DSELR1)["DSEL47"], emResource1, 47, 0));//BFP38;
    mux_table_dma.push_back(air_mux_table(0, &(*AIR_DSELR1)["DSEL48"], emResource1, 48, 0));//BFP39;
    //Supported for E2x_FCC2
    mux_table_dma.push_back(air_mux_table(0, NULL, emNotResource, 0, 0));//DSADE14;
    mux_table_dma.push_back(air_mux_table(0, NULL, emNotResource, 0, 0));//DSADE15;
    mux_table_dma.push_back(air_mux_table(0, NULL, emNotResource, 0, 0));//DSADE21;
    mux_table_dma.push_back(air_mux_table(0, NULL, emNotResource, 0, 0));//DSADE22;
    mux_table_dma.push_back(air_mux_table(0, &(*AIR_DSELR1)["DSEL35"], emResource0, 35, 0));//DSADI14;
    mux_table_dma.push_back(air_mux_table(0, &(*AIR_DSELR1)["DSEL34"], emResource0, 34, 0));//DSADI15;
    mux_table_dma.push_back(air_mux_table(0, &(*AIR_DSELR1)["DSEL37"], emResource0, 37, 0));//DSADI21;
    mux_table_dma.push_back(air_mux_table(0, &(*AIR_DSELR1)["DSEL36"], emResource0, 36, 0));//DSADI22;
    mux_table_dma.push_back(air_mux_table(0, &(*AIR_DSELR1)["DSEL49"], emResource1, 49, 0));//BFP40;
    mux_table_dma.push_back(air_mux_table(0, &(*AIR_DSELR1)["DSEL50"], emResource1, 50, 0));//BFP41;
    mux_table_dma.push_back(air_mux_table(0, &(*AIR_DSELR1)["DSEL51"], emResource1, 51, 0));//BFP42;
    mux_table_dma.push_back(air_mux_table(0, &(*AIR_DSELR1)["DSEL52"], emResource1, 52, 0));//BFP43;
    mux_table_dma.push_back(air_mux_table(0, &(*AIR_DSELR1)["DSEL53"], emResource1, 53, 0));//BFP44;
    mux_table_dma.push_back(air_mux_table(0, &(*AIR_DSELR1)["DSEL54"], emResource1, 54, 0));//BFP45;
    mux_table_dma.push_back(air_mux_table(0, &(*AIR_DSELR0)["DSEL10"], emResource1, 10, 0));//BFP46;
    mux_table_dma.push_back(air_mux_table(0, &(*AIR_DSELR0)["DSEL11"], emResource1, 11, 0));//BFP47;
}//}}}

// Destructor of Cair_core class
Cair_core::~Cair_core()
{//{{{
}//}}}

//Write to AIR_CORE registers
bool Cair_core::WriteRegister(cuint addr, const unsigned char *p_data, cuint size, bool is_debug)
{ //{{{
    if(!is_debug){
        return reg_wr(addr, p_data, size);    
    }else{
        return reg_wr_dbg(addr, p_data, size);    
    }
}//}}}

//Read from AIR_CORE registers
bool Cair_core::ReadRegister(cuint addr, unsigned char *p_data, cuint size, bool is_debug)
{//{{{
    if(!is_debug){
        return reg_rd(addr, p_data, size);
    }else{
        return reg_rd_dbg(addr, p_data, size);    
    }
}//}}}

// Intput port value is set from AIR model
void Cair_core::NotifyInputPortChange(unsigned int index, bool is_active)
{//{{{
    mux_table_int[index].inValue = is_active;
    mux_table_dma[index].inValue = is_active;
    if(mux_table_int[index].resourceID != emNotResource){
        if((unsigned int)*mux_table_int[index].mux == mux_table_int[index].resourceID){
            mux_table_int[index].outValue = is_active;
            pAirWrapper->WriteINTPortOut(mux_table_int[index].outPort, mux_table_int[index].outValue);
        }
    }
    if(mux_table_dma[index].resourceID != emNotResource){
        if((unsigned int)*mux_table_dma[index].mux == mux_table_dma[index].resourceID){
            mux_table_dma[index].outValue = is_active;
            pAirWrapper->WriteDMAPortOut(mux_table_dma[index].outPort, mux_table_dma[index].outValue);
        }
    }
}//}}}

// Get Interrupt request
bool Cair_core::GetInputPort(unsigned int index)
{//{{{
    return mux_table_int[index].inValue;
}//}}}

// Reset register
void Cair_core::ResetRegister(bool is_active)
{//{{{
    Cair_core_regif::EnableReset(is_active);
}//}}}

std::string Cair_core::regif_handle_command(std::vector<std::string>& args)
{//{{{
    std::string ret = "";
    std::string err_msg = "Error ("   + mInstName + ") ";   // error message

    std::string specified_cmd = " (";
    std::vector<std::string>::iterator cmd_it;
    for (cmd_it=args.begin() ; cmd_it!=args.end() ; cmd_it++) {
        specified_cmd += " " + *cmd_it;
    }
    specified_cmd += " ) : Type " + mInstName + " help";

    // dump register list
    if ((int)args.size() == 1) {
        vpcl::re_register *reg_p = first_reg_object();
        while (reg_p != NULL) {
            printf("%s\n", reg_p->name().c_str());
            reg_p = next_reg_object();
        }
    } else {
        bool reg_found = false;
        vpcl::re_register *reg_p = first_reg_object();
        std::vector<std::string> args_updated;
        std::copy( args.begin(), args.end(), back_inserter( args_updated ) );
        while (reg_p != NULL) {
            if (strmatch(args[1].c_str(), reg_p->name().c_str())) {
                reg_found = true;
                args_updated[1] = reg_p->name();
                ret += this->reg_handle_command(args_updated) + "\n";
            }
            reg_p = next_reg_object();
        }
        if (!reg_found) {
            ret = this->reg_handle_command(args_updated);
        }
    }
    return ret;
}//}}}

bool Cair_core::strmatch(const char *ptn, const char *str) 
{//{{{
    bool ret;
    if (ptn == NULL || str == NULL) return false;
    switch( *ptn ) {
        case '\0':
            ret = '\0' == *str;
            break;
        case '*' :
            ret = strmatch( &ptn[1], str ) || (('\0' != *str) && strmatch( ptn, &str[1] ));
            break;
        case '?' :
            ret = ('\0' != *str) && strmatch( &ptn[1], &str[1] );
            break;
        default  :
            ret = ((unsigned char)*ptn == (unsigned char)*str) && strmatch( &ptn[1], &str[1] );
            break;
    }
    return ret;
}//}}}

