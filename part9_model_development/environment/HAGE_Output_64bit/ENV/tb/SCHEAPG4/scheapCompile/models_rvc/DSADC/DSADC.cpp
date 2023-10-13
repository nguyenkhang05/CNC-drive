// ---------------------------------------------------------------------
// $Id: DSADC.cpp 1398 2019-11-26 06:16:27Z chuonghoangle $
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

#include "DSADC.h"
#include "DSADC_Func.h"
#include "DSADC_UNIT.h"
#include "DSADC_UNIT_Func.h"
/// Constructor of DSADC class
DSADC::DSADC (sc_module_name name,
            unsigned int rLatency,
            unsigned int wLatency):
    sc_module(name),
    DSADC_AgentController(),
    BusSlaveBase<32,1>(),
    CLK_HSB("CLK_HSB"),
    resetad_n("resetad_n"),
    /// Input ports
    ADSVREFH("ADSVREFH"),
    ADSVREFL("ADSVREFL"),

    DSAN000P("DSAN000P"),
    DSAN000N("DSAN000N"),
    DSAN001P("DSAN001P"),
    DSAN001N("DSAN001N"),
    DSAN002P("DSAN002P"),
    DSAN002N("DSAN002N"),
    DSAN003P("DSAN003P"),
    DSAN003N("DSAN003N"),

    DSAN100P("DSAN100P"),
    DSAN100N("DSAN100N"),
    DSAN101P("DSAN101P"),
    DSAN101N("DSAN101N"),

    DSAN200P("DSAN200P"),
    DSAN200N("DSAN200N"),

    DSAN120P("DSAN120P"),
    DSAN120N("DSAN120N"),
    DSAN121P("DSAN121P"),
    DSAN121N("DSAN121N"),

    DSAN130P("DSAN130P"),
    DSAN130N("DSAN130N"),
    DSAN131P("DSAN131P"),
    DSAN131N("DSAN131N"),

    DSAN110P("DSAN110P"),
    DSAN110N("DSAN110N"),
    DSAN111P("DSAN111P"),
    DSAN111N("DSAN111N"),

    DSAN150P("DSAN150P"),
    DSAN150N("DSAN150N"),
    DSAN151P("DSAN151P"),
    DSAN151N("DSAN151N"),

    DSAN140P("DSAN140P"),
    DSAN140N("DSAN140N"),
    DSAN141P("DSAN141P"),
    DSAN141N("DSAN141N"),
    DSAN142P("DSAN142P"),
    DSAN142N("DSAN142N"),
    DSAN143P("DSAN143P"),
    DSAN143N("DSAN143N"),

    DSAN220P("DSAN220P"),
    DSAN220N("DSAN220N"),

    DSAN210P("DSAN210P"),
    DSAN210N("DSAN210N"),

    DSADTRG00("DSADTRG00"),
    DSADTRG10("DSADTRG10"),
    DSADTRG20("DSADTRG20"),
    DSADTRG12("DSADTRG12"),
    DSADTRG13("DSADTRG13"),
    DSADTRG11("DSADTRG11"),
    DSADTRG15("DSADTRG15"),
    DSADTRG14("DSADTRG14"),
    DSADTRG22("DSADTRG22"),
    DSADTRG21("DSADTRG21"),

    DSADRGT00("DSADRGT00"),
    DSADRGT10("DSADRGT10"),
    DSADRGT20("DSADRGT20"),
    DSADRGT12("DSADRGT12"),
    DSADRGT13("DSADRGT13"),
    DSADRGT11("DSADRGT11"),
    DSADRGT15("DSADRGT15"),
    DSADRGT14("DSADRGT14"),
    DSADRGT22("DSADRGT22"),
    DSADRGT21("DSADRGT21"),
    /// Output ports
    DSADEND00("DSADEND00"),
    DSADEND10("DSADEND10"),
    DSADEND20("DSADEND20"),
    DSADEND12("DSADEND12"),
    DSADEND13("DSADEND13"),
    DSADEND11("DSADEND11"),
    DSADEND15("DSADEND15"),
    DSADEND14("DSADEND14"),
    DSADEND22("DSADEND22"),
    DSADEND21("DSADEND21"),

    DSADI00("DSADI00"),
    DSADI10("DSADI10"),
    DSADI20("DSADI20"),
    DSADI12("DSADI12"),
    DSADI13("DSADI13"),
    DSADI11("DSADI11"),
    DSADI15("DSADI15"),
    DSADI14("DSADI14"),
    DSADI22("DSADI22"),
    DSADI21("DSADI21"),

    DSADE00("DSADE00"),
    DSADE10("DSADE10"),
    DSADE20("DSADE20"),
    DSADE12("DSADE12"),
    DSADE13("DSADE13"),
    DSADE11("DSADE11"),
    DSADE15("DSADE15"),
    DSADE14("DSADE14"),
    DSADE22("DSADE22"),
    DSADE21("DSADE21"),

    DSADPE00("DSADPE00"),
    DSADPE10("DSADPE10"),
    DSADPE20("DSADPE20"),
    DSADPE12("DSADPE12"),
    DSADPE13("DSADPE13"),
    DSADPE11("DSADPE11"),
    DSADPE15("DSADPE15"),
    DSADPE14("DSADPE14"),
    DSADPE22("DSADPE22"),
    DSADPE21("DSADPE21"),

    DSADDFREQ000("DSADDFREQ000"),
    DSADDFREQ010("DSADDFREQ010"),
    DSADDFREQ020("DSADDFREQ020"),
    DSADDFREQ012("DSADDFREQ012"),
    DSADDFREQ013("DSADDFREQ013"),
    DSADDFREQ011("DSADDFREQ011"),
    DSADDFREQ015("DSADDFREQ015"),
    DSADDFREQ014("DSADDFREQ014"),
    DSADDFREQ022("DSADDFREQ022"),
    DSADDFREQ021("DSADDFREQ021"),

    DSADDFREQ100("DSADDFREQ100"),
    DSADDFREQ110("DSADDFREQ110"),
    DSADDFREQ120("DSADDFREQ120"),
    DSADDFREQ112("DSADDFREQ112"),
    DSADDFREQ113("DSADDFREQ113"),
    DSADDFREQ111("DSADDFREQ111"),
    DSADDFREQ115("DSADDFREQ115"),
    DSADDFREQ114("DSADDFREQ114"),
    DSADDFREQ122("DSADDFREQ122"),
    DSADDFREQ121("DSADDFREQ121"),

    DSADDFTAG00("DSADDFTAG00"),
    DSADDFTAG10("DSADDFTAG10"),
    DSADDFTAG20("DSADDFTAG20"),
    DSADDFTAG12("DSADDFTAG12"),
    DSADDFTAG13("DSADDFTAG13"),
    DSADDFTAG11("DSADDFTAG11"),
    DSADDFTAG15("DSADDFTAG15"),
    DSADDFTAG14("DSADDFTAG14"),
    DSADDFTAG22("DSADDFTAG22"),
    DSADDFTAG21("DSADDFTAG21"),

    DSADDFDATA00("DSADDFDATA00"),
    DSADDFDATA10("DSADDFDATA10"),
    DSADDFDATA20("DSADDFDATA20"),
    DSADDFDATA12("DSADDFDATA12"),
    DSADDFDATA13("DSADDFDATA13"),
    DSADDFDATA11("DSADDFDATA11"),
    DSADDFDATA15("DSADDFDATA15"),
    DSADDFDATA14("DSADDFDATA14"),
    DSADDFDATA22("DSADDFDATA22"),
    DSADDFDATA21("DSADDFDATA21"),

    DSADUE00("DSADUE00"),
    DSADUE10("DSADUE10"),
    DSADUE20("DSADUE20"),
    DSADUE12("DSADUE12"),
    DSADUE13("DSADUE13"),
    DSADUE11("DSADUE11"),
    DSADUE15("DSADUE15"),
    DSADUE14("DSADUE14"),
    DSADUE22("DSADUE22"),
    DSADUE21("DSADUE21"),

    DSADLE00("DSADLE00"),
    DSADLE10("DSADLE10"),
    DSADLE20("DSADLE20"),
    DSADLE12("DSADLE12"),
    DSADLE13("DSADLE13"),
    DSADLE11("DSADLE11"),
    DSADLE15("DSADLE15"),
    DSADLE14("DSADLE14"),
    DSADLE22("DSADLE22"),
    DSADLE21("DSADLE21")
{//{{{
    /// Bind clock, reset signal and socket to Bus IF
    setSlaveFreqPort32(&CLK_LSB);
    setSlaveResetPort32(&PRESETn);
    setTargetSocket32((char*)"ts");
    ts = tSocket32[0];  /// Alias for binding
    mDSADC_Func = new DSADC_Func((std::string)name, this);
    sc_assert(mDSADC_Func != NULL);
    mBusSlaveIf32[0]->setFuncModulePtr(mDSADC_Func);
    mBusSlaveIf32[0]->setBusProtocol(BUS_APB);
    mBusSlaveIf32[0]->setReadLatency(rLatency);
    mBusSlaveIf32[0]->setWriteLatency(wLatency);
    mBusSlaveIf32[0]->setTransNmbLmt(0x1);

    ts_AD00 = new TlmTargetSocket<32>("ts_AD00");
    ts_AD10 = new TlmTargetSocket<32>("ts_AD10");
    ts_AD11 = new TlmTargetSocket<32>("ts_AD11");
    ts_AD12 = new TlmTargetSocket<32>("ts_AD12");
    ts_AD13 = new TlmTargetSocket<32>("ts_AD13");
    ts_AD20 = new TlmTargetSocket<32>("ts_AD20");
    ts_AD14 = new TlmTargetSocket<32>("ts_AD14");
    ts_AD15 = new TlmTargetSocket<32>("ts_AD15");
    ts_AD22 = new TlmTargetSocket<32>("ts_AD22");
    ts_AD21 = new TlmTargetSocket<32>("ts_AD21");

    std::ostringstream port_name;
    for (unsigned int index = 0; index < emVCNum; index++) {
        port_name.str("");
        port_name << "DSADVCI00" << index;
        DSADVCI00[index] = new sc_out<bool>(port_name.str().c_str());
        sc_assert(DSADVCI00[index] != NULL);

        port_name.str("");
        port_name << "DSADVCI10" << index;
        DSADVCI10[index] = new sc_out<bool>(port_name.str().c_str());
        sc_assert(DSADVCI10[index] != NULL);

        port_name.str("");
        port_name << "DSADVCI20" << index;
        DSADVCI20[index] = new sc_out<bool>(port_name.str().c_str());
        sc_assert(DSADVCI20[index] != NULL);

        port_name.str("");
        port_name << "DSADVCI12" << index;
        DSADVCI12[index] = new sc_out<bool>(port_name.str().c_str());
        sc_assert(DSADVCI12[index] != NULL);

        port_name.str("");
        port_name << "DSADVCI13" << index;
        DSADVCI13[index] = new sc_out<bool>(port_name.str().c_str());
        sc_assert(DSADVCI13[index] != NULL);

        port_name.str("");
        port_name << "DSADVCI11" << index;
        DSADVCI11[index] = new sc_out<bool>(port_name.str().c_str());
        sc_assert(DSADVCI11[index] != NULL);

        port_name.str("");
        port_name << "DSADVCI15" << index;
        DSADVCI15[index] = new sc_out<bool>(port_name.str().c_str());
        sc_assert(DSADVCI15[index] != NULL);

        port_name.str("");
        port_name << "DSADVCI14" << index;
        DSADVCI14[index] = new sc_out<bool>(port_name.str().c_str());
        sc_assert(DSADVCI14[index] != NULL);

        port_name.str("");
        port_name << "DSADVCI22" << index;
        DSADVCI22[index] = new sc_out<bool>(port_name.str().c_str());
        sc_assert(DSADVCI22[index] != NULL);

        port_name.str("");
        port_name << "DSADVCI21" << index;
        DSADVCI21[index] = new sc_out<bool>(port_name.str().c_str());
        sc_assert(DSADVCI21[index] != NULL);
    }
    
    mAD_Unit[emAD00] = new DSADC_UNIT("AD00", this, emAD00, 8,0,0);
    mAD_Unit[emAD10] = new DSADC_UNIT("AD10", this, emAD10, 4,0,0);
    mAD_Unit[emAD20] = new DSADC_UNIT("AD20", this, emAD20, 2,0,0);
    mAD_Unit[emAD12] = new DSADC_UNIT("AD12", this, emAD12, 4,0,0);
    mAD_Unit[emAD13] = new DSADC_UNIT("AD13", this, emAD13, 4,0,0);
    mAD_Unit[emAD11] = new DSADC_UNIT("AD11", this, emAD11, 4,0,0);
    mAD_Unit[emAD15] = new DSADC_UNIT("AD15", this, emAD15, 4,0,0);
    mAD_Unit[emAD14] = new DSADC_UNIT("AD14", this, emAD14, 8,0,0);
    mAD_Unit[emAD22] = new DSADC_UNIT("AD22", this, emAD22, 2,0,0);
    mAD_Unit[emAD21] = new DSADC_UNIT("AD21", this, emAD21, 2,0,0);

    /// Blinding target socket to unit
    (*ts_AD00)(*(mAD_Unit[emAD00]->tsp));
    (*ts_AD10)(*(mAD_Unit[emAD10]->tsp));
    (*ts_AD20)(*(mAD_Unit[emAD20]->tsp));
    (*ts_AD12)(*(mAD_Unit[emAD12]->tsp));
    (*ts_AD13)(*(mAD_Unit[emAD13]->tsp));
    (*ts_AD11)(*(mAD_Unit[emAD11]->tsp));
    (*ts_AD15)(*(mAD_Unit[emAD15]->tsp));
    (*ts_AD14)(*(mAD_Unit[emAD14]->tsp));
    (*ts_AD22)(*(mAD_Unit[emAD22]->tsp));
    (*ts_AD21)(*(mAD_Unit[emAD21]->tsp));

    for (unsigned int i = 0; i < emADNum; i++) {
        sc_assert(mAD_Unit[i] != NULL);
    }
    
    /// Port connection
    for (unsigned int i = 0; i < emADNum; i++) {
        (mAD_Unit[i]->ADSVREFH)(ADSVREFH);
        (mAD_Unit[i]->ADSVREFL)(ADSVREFL);
    }
     
    (*(mAD_Unit[emAD00]->AN[0]))(DSAN000P);
    (*(mAD_Unit[emAD00]->AN[1]))(DSAN000N);
    (*(mAD_Unit[emAD00]->AN[2]))(DSAN001P);
    (*(mAD_Unit[emAD00]->AN[3]))(DSAN001N);
    (*(mAD_Unit[emAD00]->AN[4]))(DSAN002P);
    (*(mAD_Unit[emAD00]->AN[5]))(DSAN002N);
    (*(mAD_Unit[emAD00]->AN[6]))(DSAN003P);
    (*(mAD_Unit[emAD00]->AN[7]))(DSAN003N);
    
    (*(mAD_Unit[emAD10]->AN[0]))(DSAN100P);
    (*(mAD_Unit[emAD10]->AN[1]))(DSAN100N);
    (*(mAD_Unit[emAD10]->AN[2]))(DSAN101P);
    (*(mAD_Unit[emAD10]->AN[3]))(DSAN101N);
    
    (*(mAD_Unit[emAD20]->AN[0]))(DSAN200P);
    (*(mAD_Unit[emAD20]->AN[1]))(DSAN200N);
    
    (*(mAD_Unit[emAD12]->AN[0]))(DSAN120P);
    (*(mAD_Unit[emAD12]->AN[1]))(DSAN120N);
    (*(mAD_Unit[emAD12]->AN[2]))(DSAN121P);
    (*(mAD_Unit[emAD12]->AN[3]))(DSAN121N);
    
    (*(mAD_Unit[emAD13]->AN[0]))(DSAN130P);
    (*(mAD_Unit[emAD13]->AN[1]))(DSAN130N);
    (*(mAD_Unit[emAD13]->AN[2]))(DSAN131P);
    (*(mAD_Unit[emAD13]->AN[3]))(DSAN131N);
    
    (*(mAD_Unit[emAD11]->AN[0]))(DSAN110P);
    (*(mAD_Unit[emAD11]->AN[1]))(DSAN110N);
    (*(mAD_Unit[emAD11]->AN[2]))(DSAN111P);
    (*(mAD_Unit[emAD11]->AN[3]))(DSAN111N);
    
    (*(mAD_Unit[emAD15]->AN[0]))(DSAN150P);
    (*(mAD_Unit[emAD15]->AN[1]))(DSAN150N);
    (*(mAD_Unit[emAD15]->AN[2]))(DSAN151P);
    (*(mAD_Unit[emAD15]->AN[3]))(DSAN151N);

    (*(mAD_Unit[emAD14]->AN[0]))(DSAN140P);
    (*(mAD_Unit[emAD14]->AN[1]))(DSAN140N);
    (*(mAD_Unit[emAD14]->AN[2]))(DSAN141P);
    (*(mAD_Unit[emAD14]->AN[3]))(DSAN141N);
    (*(mAD_Unit[emAD14]->AN[4]))(DSAN142P);
    (*(mAD_Unit[emAD14]->AN[5]))(DSAN142N);
    (*(mAD_Unit[emAD14]->AN[6]))(DSAN143P);
    (*(mAD_Unit[emAD14]->AN[7]))(DSAN143N);

    (*(mAD_Unit[emAD22]->AN[0]))(DSAN220P);
    (*(mAD_Unit[emAD22]->AN[1]))(DSAN220N);

    (*(mAD_Unit[emAD21]->AN[0]))(DSAN210P);
    (*(mAD_Unit[emAD21]->AN[1]))(DSAN210N);

    (mAD_Unit[emAD00]->DSADTRG)(DSADTRG00);
    (mAD_Unit[emAD10]->DSADTRG)(DSADTRG10);
    (mAD_Unit[emAD20]->DSADTRG)(DSADTRG20);
    (mAD_Unit[emAD12]->DSADTRG)(DSADTRG12);
    (mAD_Unit[emAD13]->DSADTRG)(DSADTRG13);
    (mAD_Unit[emAD11]->DSADTRG)(DSADTRG11);
    (mAD_Unit[emAD15]->DSADTRG)(DSADTRG15);
    (mAD_Unit[emAD14]->DSADTRG)(DSADTRG14);
    (mAD_Unit[emAD22]->DSADTRG)(DSADTRG22);
    (mAD_Unit[emAD21]->DSADTRG)(DSADTRG21);
    
    (mAD_Unit[emAD00]->DSADRGT)(DSADRGT00);
    (mAD_Unit[emAD10]->DSADRGT)(DSADRGT10);
    (mAD_Unit[emAD20]->DSADRGT)(DSADRGT20);
    (mAD_Unit[emAD12]->DSADRGT)(DSADRGT12);
    (mAD_Unit[emAD13]->DSADRGT)(DSADRGT13);
    (mAD_Unit[emAD11]->DSADRGT)(DSADRGT11);
    (mAD_Unit[emAD15]->DSADRGT)(DSADRGT15);
    (mAD_Unit[emAD14]->DSADRGT)(DSADRGT14);
    (mAD_Unit[emAD22]->DSADRGT)(DSADRGT22);
    (mAD_Unit[emAD21]->DSADRGT)(DSADRGT21);
    
    (mAD_Unit[emAD00]->DSADEND)(DSADEND00);
    (mAD_Unit[emAD10]->DSADEND)(DSADEND10);
    (mAD_Unit[emAD20]->DSADEND)(DSADEND20);
    (mAD_Unit[emAD12]->DSADEND)(DSADEND12);
    (mAD_Unit[emAD13]->DSADEND)(DSADEND13);
    (mAD_Unit[emAD11]->DSADEND)(DSADEND11);
    (mAD_Unit[emAD15]->DSADEND)(DSADEND15);
    (mAD_Unit[emAD14]->DSADEND)(DSADEND14);
    (mAD_Unit[emAD22]->DSADEND)(DSADEND22);
    (mAD_Unit[emAD21]->DSADEND)(DSADEND21);
    
    (mAD_Unit[emAD00]->DSADI)(DSADI00);
    (mAD_Unit[emAD10]->DSADI)(DSADI10);
    (mAD_Unit[emAD20]->DSADI)(DSADI20);
    (mAD_Unit[emAD12]->DSADI)(DSADI12);
    (mAD_Unit[emAD13]->DSADI)(DSADI13);
    (mAD_Unit[emAD11]->DSADI)(DSADI11);
    (mAD_Unit[emAD15]->DSADI)(DSADI15);
    (mAD_Unit[emAD14]->DSADI)(DSADI14);
    (mAD_Unit[emAD22]->DSADI)(DSADI22);
    (mAD_Unit[emAD21]->DSADI)(DSADI21);
    
    (mAD_Unit[emAD00]->DSADE)(DSADE00);
    (mAD_Unit[emAD10]->DSADE)(DSADE10);
    (mAD_Unit[emAD20]->DSADE)(DSADE20);
    (mAD_Unit[emAD12]->DSADE)(DSADE12);
    (mAD_Unit[emAD13]->DSADE)(DSADE13);
    (mAD_Unit[emAD11]->DSADE)(DSADE11);
    (mAD_Unit[emAD15]->DSADE)(DSADE15);
    (mAD_Unit[emAD14]->DSADE)(DSADE14);
    (mAD_Unit[emAD22]->DSADE)(DSADE22);
    (mAD_Unit[emAD21]->DSADE)(DSADE21);

    (mAD_Unit[emAD00]->DSADPE)(DSADPE00);
    (mAD_Unit[emAD10]->DSADPE)(DSADPE10);
    (mAD_Unit[emAD20]->DSADPE)(DSADPE20);
    (mAD_Unit[emAD12]->DSADPE)(DSADPE12);
    (mAD_Unit[emAD13]->DSADPE)(DSADPE13);
    (mAD_Unit[emAD11]->DSADPE)(DSADPE11);
    (mAD_Unit[emAD15]->DSADPE)(DSADPE15);
    (mAD_Unit[emAD14]->DSADPE)(DSADPE14);
    (mAD_Unit[emAD22]->DSADPE)(DSADPE22);
    (mAD_Unit[emAD21]->DSADPE)(DSADPE21);

    (mAD_Unit[emAD00]->DSADDFREQ0)(DSADDFREQ000);
    (mAD_Unit[emAD10]->DSADDFREQ0)(DSADDFREQ010);
    (mAD_Unit[emAD20]->DSADDFREQ0)(DSADDFREQ020);
    (mAD_Unit[emAD12]->DSADDFREQ0)(DSADDFREQ012);
    (mAD_Unit[emAD13]->DSADDFREQ0)(DSADDFREQ013);
    (mAD_Unit[emAD11]->DSADDFREQ0)(DSADDFREQ011);
    (mAD_Unit[emAD15]->DSADDFREQ0)(DSADDFREQ015);
    (mAD_Unit[emAD14]->DSADDFREQ0)(DSADDFREQ014);
    (mAD_Unit[emAD22]->DSADDFREQ0)(DSADDFREQ022);
    (mAD_Unit[emAD21]->DSADDFREQ0)(DSADDFREQ021);
    
    (mAD_Unit[emAD00]->DSADDFREQ1)(DSADDFREQ100);
    (mAD_Unit[emAD10]->DSADDFREQ1)(DSADDFREQ110);
    (mAD_Unit[emAD20]->DSADDFREQ1)(DSADDFREQ120);
    (mAD_Unit[emAD12]->DSADDFREQ1)(DSADDFREQ112);
    (mAD_Unit[emAD13]->DSADDFREQ1)(DSADDFREQ113);
    (mAD_Unit[emAD11]->DSADDFREQ1)(DSADDFREQ111);
    (mAD_Unit[emAD15]->DSADDFREQ1)(DSADDFREQ115);
    (mAD_Unit[emAD14]->DSADDFREQ1)(DSADDFREQ114);
    (mAD_Unit[emAD22]->DSADDFREQ1)(DSADDFREQ122);
    (mAD_Unit[emAD21]->DSADDFREQ1)(DSADDFREQ121);
    
    (mAD_Unit[emAD00]->DSADDFTAG)(DSADDFTAG00);
    (mAD_Unit[emAD10]->DSADDFTAG)(DSADDFTAG10);
    (mAD_Unit[emAD20]->DSADDFTAG)(DSADDFTAG20);
    (mAD_Unit[emAD12]->DSADDFTAG)(DSADDFTAG12);
    (mAD_Unit[emAD13]->DSADDFTAG)(DSADDFTAG13);
    (mAD_Unit[emAD11]->DSADDFTAG)(DSADDFTAG11);
    (mAD_Unit[emAD15]->DSADDFTAG)(DSADDFTAG15);
    (mAD_Unit[emAD14]->DSADDFTAG)(DSADDFTAG14);
    (mAD_Unit[emAD22]->DSADDFTAG)(DSADDFTAG22);
    (mAD_Unit[emAD21]->DSADDFTAG)(DSADDFTAG21);
    
    (mAD_Unit[emAD00]->DSADDFDATA)(DSADDFDATA00);
    (mAD_Unit[emAD10]->DSADDFDATA)(DSADDFDATA10);
    (mAD_Unit[emAD20]->DSADDFDATA)(DSADDFDATA20);
    (mAD_Unit[emAD12]->DSADDFDATA)(DSADDFDATA12);
    (mAD_Unit[emAD13]->DSADDFDATA)(DSADDFDATA13);
    (mAD_Unit[emAD11]->DSADDFDATA)(DSADDFDATA11);
    (mAD_Unit[emAD15]->DSADDFDATA)(DSADDFDATA15);
    (mAD_Unit[emAD14]->DSADDFDATA)(DSADDFDATA14);
    (mAD_Unit[emAD22]->DSADDFDATA)(DSADDFDATA22);
    (mAD_Unit[emAD21]->DSADDFDATA)(DSADDFDATA21);
    
    (mAD_Unit[emAD00]->DSADUE)(DSADUE00);
    (mAD_Unit[emAD10]->DSADUE)(DSADUE10);
    (mAD_Unit[emAD20]->DSADUE)(DSADUE20);
    (mAD_Unit[emAD12]->DSADUE)(DSADUE12);
    (mAD_Unit[emAD13]->DSADUE)(DSADUE13);
    (mAD_Unit[emAD11]->DSADUE)(DSADUE11);
    (mAD_Unit[emAD15]->DSADUE)(DSADUE15);
    (mAD_Unit[emAD14]->DSADUE)(DSADUE14);
    (mAD_Unit[emAD22]->DSADUE)(DSADUE22);
    (mAD_Unit[emAD21]->DSADUE)(DSADUE21);

    (mAD_Unit[emAD00]->DSADLE)(DSADLE00);
    (mAD_Unit[emAD10]->DSADLE)(DSADLE10);
    (mAD_Unit[emAD20]->DSADLE)(DSADLE20);
    (mAD_Unit[emAD12]->DSADLE)(DSADLE12);
    (mAD_Unit[emAD13]->DSADLE)(DSADLE13);
    (mAD_Unit[emAD11]->DSADLE)(DSADLE11);
    (mAD_Unit[emAD15]->DSADLE)(DSADLE15);
    (mAD_Unit[emAD14]->DSADLE)(DSADLE14);
    (mAD_Unit[emAD22]->DSADLE)(DSADLE22);
    (mAD_Unit[emAD21]->DSADLE)(DSADLE21);
    
    for (unsigned int i = 0; i < emVCNum; i++) {
        (*(mAD_Unit[emAD00]->DSADVCI[i]))(*(DSADVCI00[i]));
        (*(mAD_Unit[emAD10]->DSADVCI[i]))(*(DSADVCI10[i]));
        (*(mAD_Unit[emAD20]->DSADVCI[i]))(*(DSADVCI20[i]));
        (*(mAD_Unit[emAD12]->DSADVCI[i]))(*(DSADVCI12[i]));
        (*(mAD_Unit[emAD13]->DSADVCI[i]))(*(DSADVCI13[i]));
        (*(mAD_Unit[emAD11]->DSADVCI[i]))(*(DSADVCI11[i]));
        (*(mAD_Unit[emAD15]->DSADVCI[i]))(*(DSADVCI15[i]));
        (*(mAD_Unit[emAD14]->DSADVCI[i]))(*(DSADVCI14[i]));
        (*(mAD_Unit[emAD22]->DSADVCI[i]))(*(DSADVCI22[i]));
        (*(mAD_Unit[emAD21]->DSADVCI[i]))(*(DSADVCI21[i]));
    }

    /// Initialize variables
    mCLK_LSBPeriod = 0;
    mCLK_LSBFreq = 0;
    mCLK_LSBOrgFreq = 0;
    mCLK_LSBFreqUnit = "Hz";
    mCLK_HSBPeriod = 0;
    mCLK_HSBFreq = 0;
    mCLK_HSBOrgFreq = 0;
    mCLK_HSBFreqUnit = "Hz";
    mFosPeriod = 125000;
    mFosFreq = 8000000;
    mFosOrgFreq = 8;
    mFosFreqUnit = "MHz";

    mCLK_LSBSyncTPoint = 0;
    mCLK_HSBSyncTPoint = 0;
    mCLK_FosSyncTPoint = 0;

    mTimeResolutionValue = 1;
    mTimeResolutionUnit = SC_NS;
    GetTimeResolution(mTimeResolutionValue, mTimeResolutionUnit);

    for (unsigned int reset_id = 0; reset_id < emResetSignalNum; reset_id++) {
        mResetCurVal[reset_id] = true;
        mResetPreVal[reset_id] = true;
        mIsResetHardActive[reset_id] = false;
        mIsResetCmdReceive[reset_id] = false;
        mIsResetCmdActive[reset_id] = false;
        mResetCmdPeriod[reset_id] =  0;
    }

    mDumpInterrupt = false;
    mADSVCC = 5;
    mADSVSS = 0;
    mtWOdd = 23;
    mtWEven = 25;
    mtDF = 380;
    mtS = 100;
    mtR = 20;
    mtCLB = 100;
    mtCoeff = 1;
    for (unsigned int i = 0; i < emADNum; i++) {
        mParityError[i] = false;
        mIDError[i] = false;
        mDisconnectError[i] = false;
    }

    mAdIdMap.insert(std::pair<std::string, unsigned int>("ad_00", emAD00));
    mAdIdMap.insert(std::pair<std::string, unsigned int>("ad_10", emAD10));
    mAdIdMap.insert(std::pair<std::string, unsigned int>("ad_20", emAD20));
    mAdIdMap.insert(std::pair<std::string, unsigned int>("ad_12", emAD12));
    mAdIdMap.insert(std::pair<std::string, unsigned int>("ad_13", emAD13));
    mAdIdMap.insert(std::pair<std::string, unsigned int>("ad_11", emAD11));
    mAdIdMap.insert(std::pair<std::string, unsigned int>("ad_15", emAD15));
    mAdIdMap.insert(std::pair<std::string, unsigned int>("ad_14", emAD14));
    mAdIdMap.insert(std::pair<std::string, unsigned int>("ad_22", emAD22));
    mAdIdMap.insert(std::pair<std::string, unsigned int>("ad_21", emAD21));
    
    SC_METHOD(HandleCLK_LSBSignalMethod);
    dont_initialize();
    sensitive << CLK_LSB;
    
    SC_METHOD(HandleCLK_HSBSignalMethod);
    dont_initialize();
    sensitive << CLK_HSB;

    SC_METHOD(HandlePRESETnSignalMethod);
    sensitive << PRESETn;
    
    SC_METHOD(HandleResetad_nSignalMethod);
    sensitive << resetad_n;

    for (unsigned int reset_id = 0; reset_id < emResetSignalNum; reset_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mResetHardEvent[reset_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&DSADC::HandleResetHardMethod, this, reset_id),
                          sc_core::sc_gen_unique_name("HandleResetHardMethod"), &opt);
    }

    for (unsigned int reset_id = 0; reset_id < emResetSignalNum; reset_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mResetCmdEvent[reset_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&DSADC::HandleResetCmdMethod, this, reset_id),
                          sc_core::sc_gen_unique_name("HandleResetCmdMethod"), &opt);
    }

    for (unsigned int reset_id = 0; reset_id < emResetSignalNum; reset_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mResetCmdCancelEvent[reset_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&DSADC::CancelResetCmdMethod, this, reset_id),
                          sc_core::sc_gen_unique_name("CancelResetCmdMethod"), &opt);
    }
}//}}}

/// Destructor of DSADC class
DSADC::~DSADC (void)
{//{{{
    delete mDSADC_Func;
    for (unsigned int i = 0; i < emADNum; i++) {
        delete mAD_Unit[i];
    }
    for (unsigned int j = 0; j < emVCNum; j++) {
    delete DSADVCI00[j];
    delete DSADVCI10[j];
    delete DSADVCI20[j];
    delete DSADVCI12[j];
    delete DSADVCI13[j];
    delete DSADVCI11[j];
    delete DSADVCI15[j];
    delete DSADVCI14[j];
    delete DSADVCI22[j];
    delete DSADVCI21[j];
    }
    delete ts_AD00;
    delete ts_AD10;
    delete ts_AD11;
    delete ts_AD12;
    delete ts_AD13;
    delete ts_AD20;
    delete ts_AD14;
    delete ts_AD15;
    delete ts_AD22;
    delete ts_AD21;
}//}}}

/// Assert reset by software
void DSADC::AssertReset (const std::string reset_name, const double start_time, const double period)
{//{{{
    if (reset_name == "PRESETn") {
        if ((!mIsResetHardActive[0])&&(!mIsResetCmdReceive[0])) {
            mIsResetCmdReceive[0] = true;
            re_printf("info","The model will be reset (PRESETn) for %f ns after %f ns.\n", period, start_time);
            mResetCmdEvent[0].notify(start_time, SC_NS);
            mResetCmdPeriod[0] = period;
        } else {
            re_printf("warning","The software reset of PRESETn is called in the reset operation of the model. So it is ignored\n");
        }
    } else if (reset_name == "resetad_n") {
        if ((!mIsResetHardActive[1])&&(!mIsResetCmdReceive[1])) {
            mIsResetCmdReceive[1] = true;
            re_printf("info","The model will be reset (resetad_n) for %f ns after %f ns.\n", period, start_time);
            mResetCmdEvent[1].notify(start_time, SC_NS);
            mResetCmdPeriod[1] = period;
        } else {
            re_printf("warning","The software reset of resetad_n is called in the reset operation of the model. So it is ignored\n");
        }
    } else {
        re_printf("warning","The reset name (%s) is wrong. It should be PRESETn or resetad_n\n",reset_name.c_str());
    }
}//}}}

/// Set clock value and clock unit
void DSADC::SetCLKFreq (const std::string clock_name, const sc_dt::uint64 freq, const std::string unit)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, clock_name);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "CLK_LSB") {
            mCLK_LSBOrgFreq = freq;
            ConvertClockFreq (mCLK_LSBFreq, mCLK_LSBFreqUnit, freq, unit);
            if (mCLK_LSBFreq > 0) {
                mCLK_LSBPeriod = (sc_dt::uint64)(((1/(double)mCLK_LSBFreq)*(double)mTimeResolutionValue) + 0.5);
                mCLK_LSBSyncTPoint = sc_time_stamp().to_double();
            } else {
                mCLK_LSBPeriod = 0;
                /// Cancel events
                CancelEvents();
                /// Notify clock is zero
                mClkZeroEvent[0].notify(SC_ZERO_TIME);
            }
        } else if (word_vector[0] == "CLK_HSB") {
            mCLK_HSBOrgFreq = freq;
            ConvertClockFreq (mCLK_HSBFreq, mCLK_HSBFreqUnit, freq, unit);
            if (mCLK_HSBFreq > 0) {
                mCLK_HSBPeriod = (sc_dt::uint64)(((1/(double)mCLK_HSBFreq)*(double)mTimeResolutionValue) + 0.5);
                mCLK_HSBSyncTPoint = sc_time_stamp().to_double();
            } else {
                mCLK_HSBPeriod = 0;
                /// Cancel events
                CancelEvents();
                /// Notify clock is zero
                mClkZeroEvent[1].notify(SC_ZERO_TIME);
            }
        } else if (word_vector[0] == "Fos") {
            mFosOrgFreq = freq;
            ConvertClockFreq (mFosFreq, mFosFreqUnit, freq, unit);
            if (mFosFreq > 0) {
                mFosPeriod = (sc_dt::uint64)(((1/(double)mFosFreq)*(double)mTimeResolutionValue) + 0.5);
                mCLK_FosSyncTPoint = sc_time_stamp().to_double();
            } else {
                mFosPeriod = 0;
            }
        } else {
            re_printf("warning","Clock name (%s) is invalid\n", word_vector[0].c_str());
        }
    } else {
        re_printf("warning","Clock name (%s) is invalid\n", clock_name.c_str());
    }
}//}}}

/// Get clock value
void DSADC::GetCLKFreq (const std::string clock_name)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, clock_name);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "CLK_LSB") {
            re_printf("info","CLK_LSB frequency is %0.0f %s\n", (double)mCLK_LSBOrgFreq, mCLK_LSBFreqUnit.c_str());
        } else if (word_vector[0] == "CLK_HSB") {
            re_printf("info","CLK_HSB frequency is %0.0f %s\n", (double)mCLK_HSBOrgFreq, mCLK_HSBFreqUnit.c_str());
        } else if (word_vector[0] == "Fos") {
            re_printf("info","Fos frequency is %0.0f %s\n", (double)mFosOrgFreq, mFosFreqUnit.c_str());
        } else {
            re_printf("warning","Clock name (%s) is invalid\n", word_vector[0].c_str());
        }
    } else {
        re_printf("warning","Clock name (%s) is invalid\n", clock_name.c_str());
    }
}//}}}


/// Force value to register
void DSADC::ForceRegister (const bool is_com_reg, const std::string ad_id, const std::string reg_name, const unsigned int reg_value)
{//{{{
    std::vector<std::string> word_vector;
    std::vector<std::string> cmd;
    if (reg_name != "") {
        SeparateString(word_vector, reg_name);
        if (word_vector.size() == 1) {
            cmd.push_back("reg");
            SeparateString(cmd, reg_name);
            cmd.push_back("force");
            std::ostringstream reg_value_str;
            reg_value_str<<"0x"<<std::hex<<reg_value;
            cmd.push_back(reg_value_str.str());
            RegisterHandlerWrapper(is_com_reg, ad_id, cmd);
        } else {
            re_printf("warning","Register name (%s) is invalid\n", reg_name.c_str());
        }
    } else {
        re_printf("warning","Register name (%s) is invalid\n", reg_name.c_str());
    }
}//}}}

/// Release forced value after forcing registers
void DSADC::ReleaseRegister (const bool is_com_reg, const std::string ad_id, const std::string reg_name)
{//{{{
    std::vector<std::string> word_vector;
    std::vector<std::string> cmd;
    if (reg_name != "") {
        SeparateString(word_vector, reg_name);
        if (word_vector.size() == 1) {
            cmd.push_back("reg");
            SeparateString(cmd, reg_name);
            cmd.push_back("release");
            RegisterHandlerWrapper(is_com_reg, ad_id, cmd);
        } else {
            re_printf("warning","Register name (%s) is invalid\n", reg_name.c_str());
        }
    } else {
        re_printf("warning","Register name (%s) is invalid\n", reg_name.c_str());
    }
}//}}}

/// Write value to registers by software
void DSADC::WriteRegister (const bool is_com_reg, const std::string ad_id, const std::string reg_name, const unsigned int reg_value)
{//{{{
    std::vector<std::string> word_vector;
    std::vector<std::string> cmd;
    if (reg_name != "") {
        SeparateString(word_vector, reg_name);
        if (word_vector.size() == 1) {
            cmd.push_back("reg");
            SeparateString(cmd, reg_name);
            std::ostringstream reg_value_str;
            reg_value_str<<"0x"<<std::hex<<reg_value;
            cmd.push_back(reg_value_str.str());
            RegisterHandlerWrapper(is_com_reg, ad_id, cmd);
        } else {
            re_printf("warning","Register name (%s) is invalid\n", reg_name.c_str());
        }
    } else {
        re_printf("warning","Register name (%s) is invalid\n", reg_name.c_str());
    }
}//}}}

/// Read value of register by software
void DSADC::ReadRegister (const bool is_com_reg, const std::string ad_id, const std::string reg_name)
{//{{{
    std::vector<std::string> word_vector;
    std::vector<std::string> cmd;
    if (reg_name != "") {
        SeparateString(word_vector, reg_name);
        if (word_vector.size() == 1) {
            cmd.push_back("reg");
            SeparateString(cmd, reg_name);
            RegisterHandlerWrapper(is_com_reg, ad_id, cmd);
        } else {
            re_printf("warning","Register name (%s) is invalid\n", reg_name.c_str());
        }
    } else {
        re_printf("warning","Register name (%s) is invalid\n", reg_name.c_str());
    }
}//}}}

/// Wrapper for accessing registers
void DSADC::RegisterHandlerWrapper (const bool is_com_reg, const std::string ad_id, const std::vector<std::string> cmd)
{//{{{
    if (is_com_reg) {
        mDSADC_Func->RegisterHandler(cmd);
    } else {
        std::map<std::string, unsigned int>::iterator it;
        bool is_match = false;
        for (it = mAdIdMap.begin(); it != mAdIdMap.end(); it++) {
            if (it->first == ad_id) {
                is_match = true;
                mAD_Unit[it->second]->mDSADC_UNIT_Func->RegisterHandler(cmd);
                break;
            }
        }
        if (!is_match) {
            re_printf("warning","AD id (%s) is invalid\n", ad_id.c_str());
        }
    }
}//}}}

/// List all registers name
void DSADC::ListRegister (void)
{//{{{
    std::vector<std::string> cmd;
    cmd.push_back("reg");
    mDSADC_Func->RegisterHandler(cmd);
    for (unsigned int i = 0; i < emADNum; i++) {
        mAD_Unit[i]->mDSADC_UNIT_Func->RegisterHandler(cmd);
    }
}//}}}

/// Dump help message of all parameters or commands
void DSADC::Help (const std::string type)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, type);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "parameters") {
            SC_REPORT_INFO(this->basename(),"DSADC_MessageLevel (fatal|error|warning|info)       Set debug message level (Default: fatal|error).");
            SC_REPORT_INFO(this->basename(),"DSADC_DumpRegisterRW (true/false)                   Enable/disable dumping access register (Default: false).");
            SC_REPORT_INFO(this->basename(),"DSADC_DumpInterrupt (true/false)                    Enable/disable dumping interrupt information (Default: false).");
            SC_REPORT_INFO(this->basename(),"DSADC_ADSVCC (value)                                Set voltage value of ADSVCC.");
            SC_REPORT_INFO(this->basename(),"DSADC_ADSVSS (value)                                Set voltage value of ADSVSS.");
            SC_REPORT_INFO(this->basename(),"DSADC_tW (value)                                    Set Internal stabilization time (number of CLK_LSB cycle).");
            SC_REPORT_INFO(this->basename(),"DSADC_tDF (value)                                   Set post-filter processing delay time (number of CLK_LSB cycle).");
            SC_REPORT_INFO(this->basename(),"DSADC_tS (value)                                    Set sampling cycle (number of CLK_LSB cycle).");
            SC_REPORT_INFO(this->basename(),"DSADC_tSD (value)                                   Set Virtual channel switching delay time (number of Over-sampling cycle).");
            SC_REPORT_INFO(this->basename(),"DSADC_tCLB (value)                                  Set Calibration processing time (number of CLK_LSB cycle).");
            SC_REPORT_INFO(this->basename(),"DSADC_tCoeff (value)                                Set correction coefficients obtained in calibration.");
        } else if (word_vector[0] == "commands") {
            SC_REPORT_INFO(this->basename(),"DSADC_AssertReset (reset_name, start_time, period)  Assert and de-assert reset signal.");
            SC_REPORT_INFO(this->basename(),"DSADC_SetCLKFreq (clock_name, freq, unit)           Set clock frequency value of model.");
            SC_REPORT_INFO(this->basename(),"DSADC_GetCLKFreq (clock_name)                       Get clock frequency value of model.");
            SC_REPORT_INFO(this->basename(),"DSADC_ForceRegister (reg_name, reg_value)           Force DSADC's common register with setting value.");
            SC_REPORT_INFO(this->basename(),"DSADC_ReleaseRegister (reg_name)                    Release DSADC's common register from force value.");
            SC_REPORT_INFO(this->basename(),"DSADC_WriteRegister (reg_name, reg_value)           Write a value to a DSADC's common register.");
            SC_REPORT_INFO(this->basename(),"DSADC_ReadRegister (reg_name)                       Read a value from a DSADC's common register.");
            SC_REPORT_INFO(this->basename(),"DSADC_ListRegister ()                               Dump name of model's registers.");
            SC_REPORT_INFO(this->basename(),"DSADC_SetParityError (ad_num, value)                Set parity error occurrence for each AD unit.");
            SC_REPORT_INFO(this->basename(),"DSADC_SetIDError (ad_num, value)                    Set ID error occurrence for each AD unit.");
            SC_REPORT_INFO(this->basename(),"DSADC_SetDisconnectError (ad_num, value)            Set input disconnection error occurrence for each AD unit.");
            SC_REPORT_INFO(this->basename(),"DSADC_UNIT_ForceRegister (ad_num, reg_name, value)  Force DSADC unit's specific register with setting value.");
            SC_REPORT_INFO(this->basename(),"DSADC_UNIT_ReleaseRegister (ad_num, reg_name)       Release DSADC unit's specific register from force value.");
            SC_REPORT_INFO(this->basename(),"DSADC_UNIT_WriteRegister (ad_num, reg_name, value)  Write a value to DSADC unit's specific register.");
            SC_REPORT_INFO(this->basename(),"DSADC_UNIT_ReadRegister (ad_num, reg_name)          Read value from DSADC unit's specific register.");
        } else {
            re_printf("warning","The name (%s) of DSADC_Help argument is wrong (commands or parameters)\n", type.c_str());
        }
    } else {
        re_printf("warning","The name (%s) of DSADC_Help argument is wrong (commands or parameters)\n", type.c_str());
    }
}//}}}

/// Set message level (fatal, error, warning, info)
void DSADC::SetMessageLevel (const std::string msg_lv)
{//{{{
    std::vector<std::string> cmd;
    cmd.push_back("command");
    cmd.push_back("MessageLevel");
    SeparateString(cmd, msg_lv);
    std::string msg_return = handleCommand(cmd);
    if (msg_return != "") {
        SC_REPORT_INFO(this->basename(), msg_return.c_str());
    }
    for (unsigned int i = 0; i < emADNum; i++) {
        std::string msg_ret = mAD_Unit[i]->handleCommand(cmd);
        if (msg_ret != "") {
            SC_REPORT_INFO(mAD_Unit[i]->basename(), msg_ret.c_str());
        }
    }

    /// Set message level for Register IF class
    cmd.clear();
    cmd.push_back("reg");
    cmd.push_back("MessageLevel");
    SeparateString(cmd, msg_lv);
    mDSADC_Func->RegisterHandler(cmd);
    for (unsigned int i = 0; i < emADNum; i++) {
        mAD_Unit[i]->mDSADC_UNIT_Func->RegisterHandler(cmd);
    }

    /// Set message level for each register in Register IF class
    cmd.clear();
    cmd.push_back("reg");
    cmd.push_back("*");
    cmd.push_back("MessageLevel");
    SeparateString(cmd, msg_lv);
    mDSADC_Func->RegisterHandler(cmd);
    for (unsigned int i = 0; i < emADNum; i++) {
        mAD_Unit[i]->mDSADC_UNIT_Func->RegisterHandler(cmd);
    }
}//}}}

/// Enable/disable dumping message when users access registers
void DSADC::DumpRegisterRW (const std::string is_enable)
{//{{{
    std::vector<std::string> cmd;
    cmd.push_back("reg");
    cmd.push_back("DumpRegisterRW");
    SeparateString(cmd, is_enable);
    mDSADC_Func->RegisterHandler(cmd);
    for (unsigned int i = 0; i < emADNum; i++) {
        mAD_Unit[i]->mDSADC_UNIT_Func->RegisterHandler(cmd);
    }
}//}}}

/// Enable/disable dumping interrupt information
void DSADC::DumpInterrupt (const std::string is_enable)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, is_enable);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "true") {
            mDumpInterrupt = true;
        } else if (word_vector[0] == "false") {
            mDumpInterrupt = false;
        } else {
            re_printf("warning","Invalid argument: DSADC_DumpInterrupt %s\n", is_enable.c_str());
        }
    } else if (word_vector.size() == 0){
        std::string temp = "false";
        if (mDumpInterrupt){
            temp = "true";
        } else {
            temp = "false";
        }
        re_printf("info","DSADC_DumpInterrupt %s\n",temp.c_str());
    } else {
        re_printf("warning","Invalid argument: DSADC_DumpInterrupt %s\n", is_enable.c_str());
    }
}//}}}

/// Calculate synchronous time
double DSADC::CalculateClockEdge (const std::string clock_name, const bool is_pos, const double time_point)
{//{{{    
    double period_num = 0;
    if (clock_name == "CLK_LSB") {
        period_num = (time_point - mCLK_LSBSyncTPoint) / (double)mCLK_LSBPeriod;
    } else if (clock_name == "CLK_HSB") {
        period_num = (time_point - mCLK_HSBSyncTPoint) / (double)mCLK_HSBPeriod;
    } else {    /// clock_name is Fos
        period_num = (time_point - mCLK_FosSyncTPoint) / (double)mFosPeriod;
    }
    sc_dt::uint64 mod_period_num = (sc_dt::uint64)period_num;
    if (is_pos) {
        if ((double)mod_period_num < period_num) {
            mod_period_num++;
        }
    } else {
        if (((double)mod_period_num + 0.5) < period_num) {
            mod_period_num++;
        }
    }
    double edge_point = 0;
    double mod_period_num_db = 0;
    if (is_pos) {
        mod_period_num_db = (double)mod_period_num;
    } else {
        mod_period_num_db = (double)mod_period_num + 0.5;
    }
    if (clock_name == "CLK_LSB") {
        edge_point = mod_period_num_db * (double)mCLK_LSBPeriod - (time_point - mCLK_LSBSyncTPoint);
    } else if (clock_name == "CLK_HSB") {
        edge_point = mod_period_num_db * (double)mCLK_HSBPeriod - (time_point - mCLK_HSBSyncTPoint);
    } else {    ///clock_name is Fos
        edge_point = mod_period_num_db * (double)mFosPeriod - (time_point - mCLK_FosSyncTPoint);
    }
    return edge_point;
}//}}}

/// Check reset status
bool DSADC::GetResetStatus (const std::string reset_name)
{//{{{
    bool reset_status = false;
    if (reset_name == "resetad_n") {
        if (mIsResetHardActive[1] || mIsResetCmdActive[1]) {
            reset_status = true;
        }
    } else {    /// PRESETn
        if (mIsResetHardActive[0] || mIsResetCmdActive[0]) {
            reset_status = true;
        }
    }
    return reset_status;
}//}}}

/// Check clock period value
bool DSADC::CheckClockPeriod (const std::string clock_name)
{//{{{
    bool clock_status = false;
    sc_dt::uint64 clock_period = 0;
    if (clock_name == "CLK_LSB") {
        clock_period = mCLK_LSBPeriod;
    } else {
        clock_period = mCLK_HSBPeriod;
    }
    if (clock_period > 0) {
        clock_status = true;
    } else {
        clock_status = false;
        re_printf("info","%s frequency is zero\n", clock_name.c_str());
    }
    return clock_status;
}//}}}

/// Set Vss/Vcc voltage
void DSADC::SetVoltage (const std::string name, const double value)
{//{{{
    if (name == "ADSVCC") {
        mADSVCC = value;
    } else if (name == "ADSVSS") {
        mADSVSS = value;
    }
}//}}}

/// Set tW/tDF/... parameters
void DSADC::SetParameter (const std::string name, const unsigned int value)
{//{{{
    if (name == "tWOdd") {
        mtWOdd = value;
    } else if (name == "tWEven") {
        mtWEven = value;
    } else if (name == "tDF") {
        mtDF = value;
    } else if (name == "tS") {
        mtS = value;
    } else if (name == "tR") {
        mtR = value;
    } else if (name == "tCLB") {
        mtCLB = value;
    }
}//}}}

/// Set Correction Coefficient value of Calibration
void DSADC::SetCoefficient (const double value)
{//{{{
    mtCoeff = value;
}//}}}

/// Issue error to model
void DSADC::SetError (const std::string name, const std::string ad_id, const std::string is_err)
{//{{{
    std::map<std::string, unsigned int>::iterator it;
    bool is_match = false;
    for (it = mAdIdMap.begin(); it != mAdIdMap.end(); it++) {
        if (it->first == ad_id) {
            is_match = true;
            if (name == "Parity") {
                if (is_err == "true") {
                    mParityError[it->second] = true;
                } else if (is_err == "false") {
                    mParityError[it->second] = false;
                } else {
                    re_printf("warning","Invalid argument: DSADC_SetParityError %s %s\n", ad_id.c_str(), is_err.c_str());
                }
            } else if (name == "ID") {
                if (is_err == "true") {
                    mIDError[it->second] = true;
                } else if (is_err == "false") {
                    mIDError[it->second] = false;
                } else {
                    re_printf("warning","Invalid argument: DSADC_SetIDError %s %s\n", ad_id.c_str(), is_err.c_str());
                }
            } else if (name == "Disconnect") {
                if (is_err == "true") {
                    mDisconnectError[it->second] = true;
                } else if (is_err == "false") {
                    mDisconnectError[it->second] = false;
                } else {
                    re_printf("warning","Invalid argument: DSADC_SetDisconnectError %s %s\n", ad_id.c_str(), is_err.c_str());
                }
            }
            break;
        }
    }
    if (!is_match) {
        re_printf("warning","AD id (%s) is invalid\n", ad_id.c_str());
    }
}//}}}

/// Handle CLK_LSB signal
void DSADC::HandleCLK_LSBSignalMethod (void)
{//{{{
    sc_dt::uint64 freq_value = CLK_LSB.read();
    SetCLKFreq("CLK_LSB", freq_value, "Hz");
}//}}}

/// Handle CLK_HSB signal
void DSADC::HandleCLK_HSBSignalMethod (void)
{//{{{
    sc_dt::uint64 freq_value = CLK_HSB.read();
    SetCLKFreq("CLK_HSB", freq_value, "Hz");
}//}}}

/// Handle PRESETn signal
void DSADC::HandlePRESETnSignalMethod (void)
{//{{{
    mResetCurVal[0] = PRESETn.read();
    if (mResetCurVal[0] != mResetPreVal[0]) {       /// mResetPreVal is "true" at initial state
        mResetPreVal[0] = mResetCurVal[0];
        /// Reset signals are asynchronous
        mResetHardEvent[0].notify(SC_ZERO_TIME);
    }
}//}}}

/// Handle resetad_n signal
void DSADC::HandleResetad_nSignalMethod (void)
{//{{{
    mResetCurVal[1] = resetad_n.read();
    if (mResetCurVal[1] != mResetPreVal[1]) {   //mResetPreVal is "true" at initial state
        mResetPreVal[1] = mResetCurVal[1];
        /// Reset signals are asynchronous
        mResetHardEvent[1].notify(SC_ZERO_TIME);
    }
}//}}}

/// Process reset function when reset port is active
void DSADC::HandleResetHardMethod (const unsigned int reset_id)
{//{{{
    if (mResetCurVal[reset_id] == vpcl::emResetActive) {
        mIsResetHardActive[reset_id] = true;
        if (reset_id == 0) {
            re_printf("info","The reset port PRESETn is asserted.\n");
            this->EnableReset("PRESETn", mIsResetHardActive[reset_id]);
        } else {
            re_printf("info","The reset port resetad_n is asserted.\n");
            this->EnableReset("resetad_n", mIsResetHardActive[reset_id]);
        }
    } else {
        if (mIsResetHardActive[reset_id]) {
            mIsResetHardActive[reset_id] = false;
            if (reset_id == 0) {
                re_printf("info","The reset port PRESETn is de-asserted.\n");
                this->EnableReset("PRESETn", mIsResetHardActive[reset_id]);
            } else {
                re_printf("info","The reset port resetad_n is de-asserted.\n");
                this->EnableReset("resetad_n", mIsResetHardActive[reset_id]);
            }
        }
    }
    /// Cancel AssertReset command when reset port is active
    if (mIsResetCmdReceive[reset_id]) {
        mResetCmdEvent[reset_id].cancel();
        mResetCmdCancelEvent[reset_id].cancel();
        mIsResetCmdReceive[reset_id] = false;
        mIsResetCmdActive[reset_id] = false;
    }
}//}}}

/// Process reset function when reset command is active
void DSADC::HandleResetCmdMethod (const unsigned int reset_id)
{//{{{
    std::string reset_name = "PRESETn";
    if (reset_id == 0) {
        reset_name = "PRESETn";
    } else { /// reset_id == 1
        reset_name = "resetad_n";
    }
    mIsResetCmdActive[reset_id] = true;
    re_printf("info","The model is reset by AssertReset command of %s.\n", reset_name.c_str());
    this->EnableReset(reset_name, mIsResetCmdActive[reset_id]);
    mResetCmdCancelEvent[reset_id].notify(mResetCmdPeriod[reset_id], SC_NS);
}//}}}

/// Cancel reset function when reset command is active
void DSADC::CancelResetCmdMethod (const unsigned int reset_id)
{//{{{
    std::string reset_name = "PRESETn";
    if (reset_id == 0) {
        reset_name = "PRESETn";
    } else { /// reset_id == 1
        reset_name = "resetad_n";
    }
    mIsResetCmdActive[reset_id] = false;
    mIsResetCmdReceive[reset_id] = false;
    re_printf("info","Reset period of %s is over.\n", reset_name.c_str());
    this->EnableReset(reset_name, mIsResetCmdActive[reset_id]);
}//}}}

/// Execute reset operation
void DSADC::EnableReset (const std::string reset_name, const bool is_active)
{//{{{
    if (reset_name == "PRESETn") {
        mDSADC_Func->Reset(is_active);
        for (unsigned int i = 0; i < emADNum; i++) {
            mAD_Unit[i]->mDSADC_UNIT_Func->Reset(is_active);
        }
    }
    if (is_active) {
        /// Cancel events
        CancelEvents();
        /// Initialize variables
        Initialize();
    }
}//}}}

/// Cancel operation events
void DSADC::CancelEvents (void)
{//{{{
    for (unsigned int i = 0; i < emADNum; i++) {
        mAD_Unit[i]->CancelEvents();
    }
}//}}}

/// Initialize internal variables
void DSADC::Initialize (void)
{//{{{
    for (unsigned int i = 0; i < emADNum; i++) {
        mAD_Unit[i]->Initialize();
        mAD_Unit[i]->mWriteDSADENDEvent.notify(SC_ZERO_TIME);
        mAD_Unit[i]->mWriteOutDSADIevent.notify(SC_ZERO_TIME);
        mAD_Unit[i]->mWriteOutDSADEevent.notify(SC_ZERO_TIME);
        mAD_Unit[i]->mWriteOutDSADPEevent.notify(SC_ZERO_TIME);
        mAD_Unit[i]->mWriteOutDFREQ0event.notify(SC_ZERO_TIME);
        mAD_Unit[i]->mWriteOutDFREQ1event.notify(SC_ZERO_TIME);
        mAD_Unit[i]->mWriteDSADDFTAGEvent.notify(SC_ZERO_TIME);
        mAD_Unit[i]->mWriteDSADDFDATAEvent.notify(SC_ZERO_TIME);
        mAD_Unit[i]->mWriteOutDSADUEevent.notify(SC_ZERO_TIME);
        mAD_Unit[i]->mWriteOutDSADLEevent.notify(SC_ZERO_TIME);
        for (unsigned int j = 0; j < DSADC_AgentController::emVCNum; j++) {
            mAD_Unit[i]->mWriteOutDSADVCIjEvent[j].notify(SC_ZERO_TIME);
        }
    }
}//}}}

/// Get DSADCADGCR.ODDE/ODE value
unsigned int DSADC::GetCheckDisconnectDetectSetup(void)
{//{{{
    return mDSADC_Func->CheckDisconnectDetectSetup();
}//}}}

/// Get DSADCADGCR.UNSND value   
bool DSADC::GetIsUnsignConfig(void)
{//{{{
    return mDSADC_Func->IsUnsignConfig();
}//}}}

/// Get DSADCTDCR.TDE value                        
bool DSADC::GetIsPinDiagEnable(void)                       
{//{{{
    return mDSADC_Func->IsPinDiagEnable();
}//}}}

/// brief dump status info of registers
void DSADC::DumpStatusInfo( )
{//{{{
    std::string type = "StatInfo";
    printf("PROFILE(%s): DSADC: Info [%20s] (%10s):\n", type.c_str( ), sc_time_stamp( ).to_string( ).c_str( ), this->name( ));
    for (int i = 0; i < emADNum; i++) {
        printf("PROFILE(%s)(%s): DSADC:   DSADnDIR bits:\n", type.c_str( ), this->mAD_Unit[i]->mModuleName.c_str( ));
        this->mAD_Unit[i]->mDSADC_UNIT_Func->GetDSADDIR( );
        printf("PROFILE(%s)(%s): DSADC:   DSADnADSR bits: 0x%08X\n", type.c_str( ), this->mAD_Unit[i]->mModuleName.c_str( ), this->mAD_Unit[i]->mDSADC_UNIT_Func->GetDSADADSR( ));
        printf("PROFILE(%s)(%s): DSADC:   DSADnUDPTRR bits: 0x%08X\n", type.c_str( ), this->mAD_Unit[i]->mModuleName.c_str( ), this->mAD_Unit[i]->mDSADC_UNIT_Func->GetDSADUDPTRR( ));
        printf("PROFILE(%s)(%s): DSADC:   DSADnUDIR bits: 0x%08X\n", type.c_str( ), this->mAD_Unit[i]->mModuleName.c_str( ), this->mAD_Unit[i]->mDSADC_UNIT_Func->GetDSADUDIR( ));
        printf("PROFILE(%s)(%s): DSADC:   DSADnTSVAL bits: 0x%08X\n", type.c_str( ), this->mAD_Unit[i]->mModuleName.c_str( ), this->mAD_Unit[i]->mDSADC_UNIT_Func->GetDSADTSVAL( ));
        printf("PROFILE(%s)(%s): DSADC:   DSADnER bits: 0x%08X\n", type.c_str( ), this->mAD_Unit[i]->mModuleName.c_str( ), this->mAD_Unit[i]->mDSADC_UNIT_Func->GetDSADER( ));
    }
}//}}}
// vim600: set foldmethod=marker :
