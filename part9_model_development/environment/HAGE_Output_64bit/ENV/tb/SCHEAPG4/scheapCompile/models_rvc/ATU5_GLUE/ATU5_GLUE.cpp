// ---------------------------------------------------------------------
// $Id: ATU5_GLUE.cpp 1196 2019-10-17 05:33:04Z chuonghoangle $
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
#include "ATU5_GLUE.h"
#include "ATU5_GLUE_Func.h"

///Constructor of ATU5_GLUE class
ATU5_GLUE::ATU5_GLUE (sc_module_name name,
            unsigned int rLatency,
            unsigned int wLatency):
            sc_module(name),
            ATU5_GLUE_AgentController(),
            BusSlaveBase<32,1>(),
            PCLK("PCLK"),
            PRESETZ("PRESETZ"),
            // Input ports
            // TimerA
            INTATUAICIA0("INTATUAICIA0"),
            INTATUAICIA1("INTATUAICIA1"),
            INTATUAICIA2("INTATUAICIA2"),
            INTATUAICIA3("INTATUAICIA3"),
            INTATUAICIA4("INTATUAICIA4"),
            INTATUAICIA5("INTATUAICIA5"),
            INTATUAICIA6("INTATUAICIA6"),
            INTATUAICIA7("INTATUAICIA7"),
            // OVIA
            OVIA("OVIA"),
            // TimerB
            INTATUBCMIB0("INTATUBCMIB0"),
            INTATUBCMIB1("INTATUBCMIB1"),
            INTATUBCMIB6("INTATUBCMIB6"),
            INTATUBCMIB10("INTATUBCMIB10"),
            INTATUBCMIB11("INTATUBCMIB11"),
            INTATUBCMIB12("INTATUBCMIB12"),
            INTATUBCMIB6M("INTATUBCMIB6M"),
            INTATUBCMIB66M("INTATUBCMIB66M"),
            INTATUBICIB0("INTATUBICIB0"),
            // OVICx
            OVIC0("OVIC0"),
            OVIC1("OVIC1"),
            OVIC2("OVIC2"),
            OVIC3("OVIC3"),
            OVIC4("OVIC4"),
            OVIC5("OVIC5"),
            OVIC6("OVIC6"),
            OVIC7("OVIC7"),
            OVIC8("OVIC8"),
            OVIC9("OVIC9"),
            OVIC10("OVIC10"),
            // IMIGCxy
            IMIGC00("IMIGC00"),
            IMIGC01("IMIGC01"),
            IMIGC02("IMIGC02"),
            IMIGC03("IMIGC03"),
            IMIGC10("IMIGC10"),
            IMIGC11("IMIGC11"),
            IMIGC12("IMIGC12"),
            IMIGC13("IMIGC13"),
            IMIGC20("IMIGC20"),
            IMIGC21("IMIGC21"),
            IMIGC22("IMIGC22"),
            IMIGC23("IMIGC23"),
            IMIGC30("IMIGC30"),
            IMIGC31("IMIGC31"),
            IMIGC32("IMIGC32"),
            IMIGC33("IMIGC33"),
            IMIGC40("IMIGC40"),
            IMIGC41("IMIGC41"),
            IMIGC42("IMIGC42"),
            IMIGC43("IMIGC43"),
            IMIGC50("IMIGC50"),
            IMIGC51("IMIGC51"),
            IMIGC52("IMIGC52"),
            IMIGC53("IMIGC53"),
            IMIGC60("IMIGC60"),
            IMIGC61("IMIGC61"),
            IMIGC62("IMIGC62"),
            IMIGC63("IMIGC63"),
            IMIGC70("IMIGC70"),
            IMIGC71("IMIGC71"),
            IMIGC72("IMIGC72"),
            IMIGC73("IMIGC73"),
            IMIGC80("IMIGC80"),
            IMIGC81("IMIGC81"),
            IMIGC82("IMIGC82"),
            IMIGC83("IMIGC83"),
            IMIGC90("IMIGC90"),
            IMIGC91("IMIGC91"),
            IMIGC92("IMIGC92"),
            IMIGC93("IMIGC93"),
            IMIGC100("IMIGC100"),
            IMIGC101("IMIGC101"),
            IMIGC102("IMIGC102"),
            IMIGC103("IMIGC103"),
            // IMIOCxy
            IMIOC00("IMIOC00"),
            IMIOC01("IMIOC01"),
            IMIOC02("IMIOC02"),
            IMIOC03("IMIOC03"),
            IMIOC10("IMIOC10"),
            IMIOC11("IMIOC11"),
            IMIOC12("IMIOC12"),
            IMIOC13("IMIOC13"),
            IMIOC20("IMIOC20"),
            IMIOC21("IMIOC21"),
            IMIOC22("IMIOC22"),
            IMIOC23("IMIOC23"),
            IMIOC30("IMIOC30"),
            IMIOC31("IMIOC31"),
            IMIOC32("IMIOC32"),
            IMIOC33("IMIOC33"),
            IMIOC40("IMIOC40"),
            IMIOC41("IMIOC41"),
            IMIOC42("IMIOC42"),
            IMIOC43("IMIOC43"),
            IMIOC50("IMIOC50"),
            IMIOC51("IMIOC51"),
            IMIOC52("IMIOC52"),
            IMIOC53("IMIOC53"),
            IMIOC60("IMIOC60"),
            IMIOC61("IMIOC61"),
            IMIOC62("IMIOC62"),
            IMIOC63("IMIOC63"),
            IMIOC70("IMIOC70"),
            IMIOC71("IMIOC71"),
            IMIOC72("IMIOC72"),
            IMIOC73("IMIOC73"),
            IMIOC80("IMIOC80"),
            IMIOC81("IMIOC81"),
            IMIOC82("IMIOC82"),
            IMIOC83("IMIOC83"),
            IMIOC90("IMIOC90"),
            IMIOC91("IMIOC91"),
            IMIOC92("IMIOC92"),
            IMIOC93("IMIOC93"),
            IMIOC100("IMIOC100"),
            IMIOC101("IMIOC101"),
            IMIOC102("IMIOC102"),
            IMIOC103("IMIOC103"),
            // TOCxy
            TOC00("TOC00"),
            TOC01("TOC01"),
            TOC02("TOC02"),
            TOC03("TOC03"),
            TOC10("TOC10"),
            TOC11("TOC11"),
            TOC12("TOC12"),
            TOC13("TOC13"),
            TOC20("TOC20"),
            TOC21("TOC21"),
            TOC22("TOC22"),
            TOC23("TOC23"),
            TOC30("TOC30"),
            TOC31("TOC31"),
            // CMIADxy
            CMIAD00("CMIAD00"),
            CMIAD01("CMIAD01"),
            CMIAD02("CMIAD02"),
            CMIAD03("CMIAD03"),
            CMIAD10("CMIAD10"),
            CMIAD11("CMIAD11"),
            CMIAD12("CMIAD12"),
            CMIAD13("CMIAD13"),
            CMIAD20("CMIAD20"),
            CMIAD21("CMIAD21"),
            CMIAD22("CMIAD22"),
            CMIAD23("CMIAD23"),
            CMIAD30("CMIAD30"),
            CMIAD31("CMIAD31"),
            CMIAD32("CMIAD32"),
            CMIAD33("CMIAD33"),
            CMIAD40("CMIAD40"),
            CMIAD41("CMIAD41"),
            CMIAD42("CMIAD42"),
            CMIAD43("CMIAD43"),
            CMIAD50("CMIAD50"),
            CMIAD51("CMIAD51"),
            CMIAD52("CMIAD52"),
            CMIAD53("CMIAD53"),
            CMIAD60("CMIAD60"),
            CMIAD61("CMIAD61"),
            CMIAD62("CMIAD62"),
            CMIAD63("CMIAD63"),
            CMIAD70("CMIAD70"),
            CMIAD71("CMIAD71"),
            CMIAD72("CMIAD72"),
            CMIAD73("CMIAD73"),
            CMIAD80("CMIAD80"),
            CMIAD81("CMIAD81"),
            CMIAD82("CMIAD82"),
            CMIAD83("CMIAD83"),
            // CMIBDxy
            CMIBD00("CMIBD00"),
            CMIBD01("CMIBD01"),
            CMIBD02("CMIBD02"),
            CMIBD03("CMIBD03"),
            CMIBD10("CMIBD10"),
            CMIBD11("CMIBD11"),
            CMIBD12("CMIBD12"),
            CMIBD13("CMIBD13"),
            CMIBD20("CMIBD20"),
            CMIBD21("CMIBD21"),
            CMIBD22("CMIBD22"),
            CMIBD23("CMIBD23"),
            CMIBD30("CMIBD30"),
            CMIBD31("CMIBD31"),
            CMIBD32("CMIBD32"),
            CMIBD33("CMIBD33"),
            CMIBD40("CMIBD40"),
            CMIBD41("CMIBD41"),
            CMIBD42("CMIBD42"),
            CMIBD43("CMIBD43"),
            CMIBD50("CMIBD50"),
            CMIBD51("CMIBD51"),
            CMIBD52("CMIBD52"),
            CMIBD53("CMIBD53"),
            CMIBD60("CMIBD60"),
            CMIBD61("CMIBD61"),
            CMIBD62("CMIBD62"),
            CMIBD63("CMIBD63"),
            CMIBD70("CMIBD70"),
            CMIBD71("CMIBD71"),
            CMIBD72("CMIBD72"),
            CMIBD73("CMIBD73"),
            CMIBD80("CMIBD80"),
            CMIBD81("CMIBD81"),
            CMIBD82("CMIBD82"),
            CMIBD83("CMIBD83"),
            // OVI1Dx
            OVI1D0("OVI1D0"),
            OVI1D1("OVI1D1"),
            OVI1D2("OVI1D2"),
            OVI1D3("OVI1D3"),
            OVI1D4("OVI1D4"),
            OVI1D5("OVI1D5"),
            OVI1D6("OVI1D6"),
            OVI1D7("OVI1D7"),
            OVI1D8("OVI1D8"),
            // OVI2Dx
            OVI2D0("OVI2D0"),
            OVI2D1("OVI2D1"),
            OVI2D2("OVI2D2"),
            OVI2D3("OVI2D3"),
            OVI2D4("OVI2D4"),
            OVI2D5("OVI2D5"),
            OVI2D6("OVI2D6"),
            OVI2D7("OVI2D7"),
            OVI2D8("OVI2D8"),
            // TODxyA
            TOD00A("TOD00A"),
            TOD01A("TOD01A"),
            TOD02A("TOD02A"),
            TOD03A("TOD03A"),
            TOD10A("TOD10A"),
            TOD11A("TOD11A"),
            TOD12A("TOD12A"),
            TOD13A("TOD13A"),
            TOD20A("TOD20A"),
            TOD21A("TOD21A"),
            TOD22A("TOD22A"),
            TOD23A("TOD23A"),
            TOD30A("TOD30A"),
            TOD31A("TOD31A"),
            // TODxyB
            TOD00B("TOD00B"),
            TOD01B("TOD01B"),
            TOD02B("TOD02B"),
            TOD03B("TOD03B"),
            TOD10B("TOD10B"),
            TOD11B("TOD11B"),
            TOD12B("TOD12B"),
            TOD13B("TOD13B"),
            TOD20B("TOD20B"),
            TOD21B("TOD21B"),
            TOD22B("TOD22B"),
            TOD23B("TOD23B"),
            TOD30B("TOD30B"),
            TOD31B("TOD31B"),
            // UDIDxy
            UDID00("UDID00"),
            UDID01("UDID01"),
            UDID02("UDID02"),
            UDID03("UDID03"),
            UDID10("UDID10"),
            UDID11("UDID11"),
            UDID12("UDID12"),
            UDID13("UDID13"),
            UDID20("UDID20"),
            UDID21("UDID21"),
            UDID22("UDID22"),
            UDID23("UDID23"),
            UDID30("UDID30"),
            UDID31("UDID31"),
            UDID32("UDID32"),
            UDID33("UDID33"),
            UDID40("UDID40"),
            UDID41("UDID41"),
            UDID42("UDID42"),
            UDID43("UDID43"),
            UDID50("UDID50"),
            UDID51("UDID51"),
            UDID52("UDID52"),
            UDID53("UDID53"),
            UDID60("UDID60"),
            UDID61("UDID61"),
            UDID62("UDID62"),
            UDID63("UDID63"),
            UDID70("UDID70"),
            UDID71("UDID71"),
            UDID72("UDID72"),
            UDID73("UDID73"),
            UDID80("UDID80"),
            UDID81("UDID81"),
            UDID82("UDID82"),
            UDID83("UDID83"),
            // PREIDxy
            PREID00("PREID00"),
            PREID01("PREID01"),
            PREID02("PREID02"),
            PREID03("PREID03"),
            PREID10("PREID10"),
            PREID11("PREID11"),
            PREID12("PREID12"),
            PREID13("PREID13"),
            PREID20("PREID20"),
            PREID21("PREID21"),
            PREID22("PREID22"),
            PREID23("PREID23"),
            PREID30("PREID30"),
            PREID31("PREID31"),
            PREID32("PREID32"),
            PREID33("PREID33"),
            PREID40("PREID40"),
            PREID41("PREID41"),
            PREID42("PREID42"),
            PREID43("PREID43"),
            PREID50("PREID50"),
            PREID51("PREID51"),
            PREID52("PREID52"),
            PREID53("PREID53"),
            PREID60("PREID60"),
            PREID61("PREID61"),
            PREID62("PREID62"),
            PREID63("PREID63"),
            PREID70("PREID70"),
            PREID71("PREID71"),
            PREID72("PREID72"),
            PREID73("PREID73"),
            PREID80("PREID80"),
            PREID81("PREID81"),
            PREID82("PREID82"),
            PREID83("PREID83"),
            // PFEIDxy
            PFEID00("PFEID00"),
            PFEID01("PFEID01"),
            PFEID02("PFEID02"),
            PFEID03("PFEID03"),
            PFEID10("PFEID10"),
            PFEID11("PFEID11"),
            PFEID12("PFEID12"),
            PFEID13("PFEID13"),
            PFEID20("PFEID20"),
            PFEID21("PFEID21"),
            PFEID22("PFEID22"),
            PFEID23("PFEID23"),
            PFEID30("PFEID30"),
            PFEID31("PFEID31"),
            PFEID32("PFEID32"),
            PFEID33("PFEID33"),
            PFEID40("PFEID40"),
            PFEID41("PFEID41"),
            PFEID42("PFEID42"),
            PFEID43("PFEID43"),
            PFEID50("PFEID50"),
            PFEID51("PFEID51"),
            PFEID52("PFEID52"),
            PFEID53("PFEID53"),
            PFEID60("PFEID60"),
            PFEID61("PFEID61"),
            PFEID62("PFEID62"),
            PFEID63("PFEID63"),
            PFEID70("PFEID70"),
            PFEID71("PFEID71"),
            PFEID72("PFEID72"),
            PFEID73("PFEID73"),
            PFEID80("PFEID80"),
            PFEID81("PFEID81"),
            PFEID82("PFEID82"),
            PFEID83("PFEID83"),
            // TimerE
            INTATUECMIE00("INTATUECMIE00"),
            INTATUECMIE01("INTATUECMIE01"),
            INTATUECMIE02("INTATUECMIE02"),
            INTATUECMIE03("INTATUECMIE03"),
            INTATUECMIE10("INTATUECMIE10"),
            INTATUECMIE11("INTATUECMIE11"),
            INTATUECMIE12("INTATUECMIE12"),
            INTATUECMIE13("INTATUECMIE13"),
            INTATUECMIE20("INTATUECMIE20"),
            INTATUECMIE21("INTATUECMIE21"),
            INTATUECMIE22("INTATUECMIE22"),
            INTATUECMIE23("INTATUECMIE23"),
            INTATUECMIE30("INTATUECMIE30"),
            INTATUECMIE31("INTATUECMIE31"),
            INTATUECMIE32("INTATUECMIE32"),
            INTATUECMIE33("INTATUECMIE33"),
            INTATUECMIE40("INTATUECMIE40"),
            INTATUECMIE41("INTATUECMIE41"),
            INTATUECMIE42("INTATUECMIE42"),
            INTATUECMIE43("INTATUECMIE43"),
            INTATUECMIE50("INTATUECMIE50"),
            INTATUECMIE51("INTATUECMIE51"),
            INTATUECMIE52("INTATUECMIE52"),
            INTATUECMIE53("INTATUECMIE53"),
            INTATUECMIE60("INTATUECMIE60"),
            INTATUECMIE61("INTATUECMIE61"),
            INTATUECMIE62("INTATUECMIE62"),
            INTATUECMIE63("INTATUECMIE63"),
            INTATUECMIE70("INTATUECMIE70"),
            INTATUECMIE71("INTATUECMIE71"),
            INTATUECMIE72("INTATUECMIE72"),
            INTATUECMIE73("INTATUECMIE73"),
            INTATUECMIE80("INTATUECMIE80"),
            INTATUECMIE81("INTATUECMIE81"),
            INTATUECMIE82("INTATUECMIE82"),
            INTATUECMIE83("INTATUECMIE83"),
            // TOExy
            TOE00("TOE00"),
            TOE01("TOE01"),
            TOE02("TOE02"),
            TOE03("TOE03"),
            TOE10("TOE10"),
            TOE11("TOE11"),
            TOE12("TOE12"),
            TOE13("TOE13"),
            TOE20("TOE20"),
            TOE21("TOE21"),
            TOE22("TOE22"),
            TOE23("TOE23"),
            TOE30("TOE30"),
            TOE31("TOE31"),
            // TimerF
            INTATUFICIF0("INTATUFICIF0"),
            INTATUFICIF1("INTATUFICIF1"),
            INTATUFICIF2("INTATUFICIF2"),
            INTATUFICIF3("INTATUFICIF3"),
            INTATUFICIF4("INTATUFICIF4"),
            INTATUFICIF5("INTATUFICIF5"),
            INTATUFICIF6("INTATUFICIF6"),
            INTATUFICIF7("INTATUFICIF7"),
            INTATUFICIF8("INTATUFICIF8"),
            INTATUFICIF9("INTATUFICIF9"),
            INTATUFICIF10("INTATUFICIF10"),
            INTATUFICIF11("INTATUFICIF11"),
            INTATUFICIF12("INTATUFICIF12"),
            INTATUFICIF13("INTATUFICIF13"),
            INTATUFICIF14("INTATUFICIF14"),
            INTATUFICIF15("INTATUFICIF15"),
            // OVIFxy
            OVIF0("OVIF0"),
            OVIF1("OVIF1"),
            OVIF2("OVIF2"),
            OVIF3("OVIF3"),
            OVIF4("OVIF4"),
            OVIF5("OVIF5"),
            OVIF6("OVIF6"),
            OVIF7("OVIF7"),
            OVIF8("OVIF8"),
            OVIF9("OVIF9"),
            OVIF10("OVIF10"),
            OVIF11("OVIF11"),
            OVIF12("OVIF12"),
            OVIF13("OVIF13"),
            OVIF14("OVIF14"),
            OVIF15("OVIF15"),
            // TimerG
            INTATUGCMIG0("INTATUGCMIG0"),
            INTATUGCMIG1("INTATUGCMIG1"),
            INTATUGCMIG2("INTATUGCMIG2"),
            INTATUGCMIG3("INTATUGCMIG3"),
            INTATUGCMIG4("INTATUGCMIG4"),
            INTATUGCMIG5("INTATUGCMIG5"),
            INTATUGCMIG6("INTATUGCMIG6"),
            INTATUGCMIG7("INTATUGCMIG7"),
            INTATUGCMIG8("INTATUGCMIG8"),
            INTATUGCMIG9("INTATUGCMIG9"),
            // DMA
            INTATUCTRLDMAREQ00("INTATUCTRLDMAREQ00"),
            INTATUCTRLDMAREQ01("INTATUCTRLDMAREQ01"),
            INTATUCTRLDMAREQ10("INTATUCTRLDMAREQ10"),
            INTATUCTRLDMAREQ11("INTATUCTRLDMAREQ11"),
            // Output ports
            // PSI5
            P5TGC0("P5TGC0"),
            P5TGC1("P5TGC1"),
            P5TGC2("P5TGC2"),
            P5TGC3("P5TGC3"),
            P5TGC4("P5TGC4"),
            P5TGC5("P5TGC5"),
            P5TGC6("P5TGC6"),
            P5TGC7("P5TGC7"),
            P5CKTSA("P5CKTSA"),
            P5CKTSB("P5CKTSB"),
            P5CRTSA("P5CRTSA"),
            P5CRTSB("P5CRTSB"),
            P5SSTSA("P5SSTSA"),
            P5SSTSB("P5SSTSB"),
            // DFE
            DEG1TRG("DEG1TRG"),
            DFC800("DFC800"),
            DFC801("DFC801"),
            DFC802("DFC802"),
            DFC810("DFC810"),
            DFC811("DFC811"),
            DFC812("DFC812"),
            DFC820("DFC820"),
            DFC821("DFC821"),
            DFC822("DFC822"),
            DFC830("DFC830"),
            DFC831("DFC831"),
            DFC832("DFC832"),
            DFC900("DFC900"),
            DFC901("DFC901"),
            DFC902("DFC902"),
            DFC910("DFC910"),
            DFC911("DFC911"),
            DFC912("DFC912"),
            DFD000("DFD000"),
            DFD001("DFD001"),
            DFD002("DFD002"),
            DFD010("DFD010"),
            DFD011("DFD011"),
            DFD012("DFD012"),
            DFD020("DFD020"),
            DFD021("DFD021"),
            DFD022("DFD022"),
            DFD030("DFD030"),
            DFD031("DFD031"),
            DFD032("DFD032"),
            DFD100("DFD100"),
            DFD101("DFD101"),
            DFD102("DFD102"),
            DFD110("DFD110"),
            DFD111("DFD111"),
            DFD112("DFD112"),
            // ADC
            DSTS0("DSTS0"),
            DSTS1("DSTS1"),
            DSTS2("DSTS2"),
            DSTS3("DSTS3"),
            DSTS4("DSTS4"),
            DSTS5("DSTS5"),
            DSTS6("DSTS6"),
            DSTS7("DSTS7"),
            DSTS8("DSTS8"),
            DSTS9("DSTS9"),
            CATS("CATS"),
            // PGL
            // TimerA
            ICIA0("ICIA0"),
            ICIA1("ICIA1"),
            ICIA2("ICIA2"),
            ICIA3("ICIA3"),
            ICIA4("ICIA4"),
            ICIA5("ICIA5"),
            ICIA6("ICIA6"),
            ICIA7("ICIA7"),
            // TimerB
            CMIB0("CMIB0"),
            CMIB1("CMIB1"),
            CMIB6("CMIB6"),
            CMIB10("CMIB10"),
            CMIB11("CMIB11"),
            CMIB12("CMIB12"),
            CMIB6M("CMIB6M"),
            CMIB66M("CMIB66M"),
            ICIB0("ICIB0"),
            // TimerE
            CMIE00("CMIE00"),
            CMIE01("CMIE01"),
            CMIE02("CMIE02"),
            CMIE03("CMIE03"),
            CMIE10("CMIE10"),
            CMIE11("CMIE11"),
            CMIE12("CMIE12"),
            CMIE13("CMIE13"),
            CMIE20("CMIE20"),
            CMIE21("CMIE21"),
            CMIE22("CMIE22"),
            CMIE23("CMIE23"),
            CMIE30("CMIE30"),
            CMIE31("CMIE31"),
            CMIE32("CMIE32"),
            CMIE33("CMIE33"),
            CMIE40("CMIE40"),
            CMIE41("CMIE41"),
            CMIE42("CMIE42"),
            CMIE43("CMIE43"),
            CMIE50("CMIE50"),
            CMIE51("CMIE51"),
            CMIE52("CMIE52"),
            CMIE53("CMIE53"),
            CMIE60("CMIE60"),
            CMIE61("CMIE61"),
            CMIE62("CMIE62"),
            CMIE63("CMIE63"),
            CMIE70("CMIE70"),
            CMIE71("CMIE71"),
            CMIE72("CMIE72"),
            CMIE73("CMIE73"),
            CMIE80("CMIE80"),
            CMIE81("CMIE81"),
            CMIE82("CMIE82"),
            CMIE83("CMIE83"),
            // TimerF
            ICIF0("ICIF0"),
            ICIF1("ICIF1"),
            ICIF2("ICIF2"),
            ICIF3("ICIF3"),
            ICIF4("ICIF4"),
            ICIF5("ICIF5"),
            ICIF6("ICIF6"),
            ICIF7("ICIF7"),
            ICIF8("ICIF8"),
            ICIF9("ICIF9"),
            ICIF10("ICIF10"),
            ICIF11("ICIF11"),
            ICIF12("ICIF12"),
            ICIF13("ICIF13"),
            ICIF14("ICIF14"),
            ICIF15("ICIF15"),
            // TimerG
            CMIG0("CMIG0"),
            CMIG1("CMIG1"),
            CMIG2("CMIG2"),
            CMIG3("CMIG3"),
            CMIG4("CMIG4"),
            CMIG5("CMIG5"),
            CMIG6("CMIG6"),
            CMIG7("CMIG7"),
            CMIG8("CMIG8"),
            CMIG9("CMIG9"),
            // DMA
            DMAREQ00("DMAREQ00"),
            DMAREQ01("DMAREQ01"),
            DMAREQ10("DMAREQ10"),
            DMAREQ11("DMAREQ11"),
            // Overflow
            OVF("OVF"),
            // TimerA - select
            SLIA0("SLIA0"),
            SLIA1("SLIA1"),
            SLIA2("SLIA2"),
            SLIA3("SLIA3"),
            // TimerB - select
            SLDB0("SLDB0"),
            SLDB1("SLDB1"),
            SLDB2("SLDB2"),
            SLDB3("SLDB3"),
            // TimerC select
            SLIC00("SLIC00"),
            SLIC01("SLIC01"),
            SLIC02("SLIC02"),
            SLIC03("SLIC03"),
            SLIC10("SLIC10"),
            SLIC11("SLIC11"),
            SLIC12("SLIC12"),
            SLIC13("SLIC13"),
            SLIC20("SLIC20"),
            SLIC21("SLIC21"),
            SLIC22("SLIC22"),
            SLIC23("SLIC23"),
            SLIC30("SLIC30"),
            SLIC31("SLIC31"),
            SLIC32("SLIC32"),
            SLIC33("SLIC33"),
            SLIC40("SLIC40"),
            SLIC41("SLIC41"),
            SLIC42("SLIC42"),
            SLIC43("SLIC43"),
            SLIC50("SLIC50"),
            SLIC51("SLIC51"),
            SLIC52("SLIC52"),
            SLIC53("SLIC53"),
            SLIC60("SLIC60"),
            SLIC61("SLIC61"),
            SLIC62("SLIC62"),
            SLIC63("SLIC63"),
            SLIC70("SLIC70"),
            SLIC71("SLIC71"),
            SLIC72("SLIC72"),
            SLIC73("SLIC73"),
            SLIC80("SLIC80"),
            SLIC81("SLIC81"),
            SLIC82("SLIC82"),
            SLIC83("SLIC83"),
            SLIC90("SLIC90"),
            SLIC91("SLIC91"),
            SLIC92("SLIC92"),
            SLIC93("SLIC93"),
            SLIC100("SLIC100"),
            SLIC101("SLIC101"),
            SLIC102("SLIC102"),
            SLIC103("SLIC103"),
            // TimerD - select
            SLID00("SLID00"),
            SLID01("SLID01"),
            SLID02("SLID02"),
            SLID03("SLID03"),
            SLID10("SLID10"),
            SLID11("SLID11"),
            SLID12("SLID12"),
            SLID13("SLID13"),
            SLID20("SLID20"),
            SLID21("SLID21"),
            SLID22("SLID22"),
            SLID23("SLID23"),
            SLID30("SLID30"),
            SLID31("SLID31"),
            SLID32("SLID32"),
            SLID33("SLID33"),
            SLID40("SLID40"),
            SLID41("SLID41"),
            SLID42("SLID42"),
            SLID43("SLID43"),
            SLID50("SLID50"),
            SLID51("SLID51"),
            SLID52("SLID52"),
            SLID53("SLID53"),
            SLID60("SLID60"),
            SLID61("SLID61"),
            SLID62("SLID62"),
            SLID63("SLID63"),
            SLID70("SLID70"),
            SLID71("SLID71"),
            SLID72("SLID72"),
            SLID73("SLID73"),
            SLID80("SLID80"),
            SLID81("SLID81"),
            SLID82("SLID82"),
            SLID83("SLID83"),
            // TimerC-D - select
            SLDCD0("SLDCD0"),
            SLDCD1("SLDCD1"),
            SLDCD2("SLDCD2"),
            SLDCD3("SLDCD3"),
            SLDCD4("SLDCD4"),
            SLDCD5("SLDCD5"),
            SLDCD6("SLDCD6"),
            SLDCD7("SLDCD7"),
            SLDCD8("SLDCD8"),
            SLDCD9("SLDCD9"),
            SLDCD10("SLDCD10"),
            SLDCD11("SLDCD11"),
            SLDCD12("SLDCD12"),
            SLDCD13("SLDCD13"),
            SLDCD14("SLDCD14"),
            SLDCD15("SLDCD15"),
            SLDCD16("SLDCD16"),
            SLDCD17("SLDCD17"),
            SLDCD18("SLDCD18"),
            SLDCD19("SLDCD19"),
            SLDCD20("SLDCD20"),
            SLDCD21("SLDCD21"),
            SLDCD22("SLDCD22"),
            SLDCD23("SLDCD23"),
            SLDCD24("SLDCD24"),
            SLDCD25("SLDCD25"),
            SLDCD26("SLDCD26"),
            SLDCD27("SLDCD27"),
            SLDCD28("SLDCD28"),
            SLDCD29("SLDCD29"),
            SLDCD30("SLDCD30"),
            SLDCD31("SLDCD31"),
            SLDCD32("SLDCD32"),
            SLDCD33("SLDCD33"),
            SLDCD34("SLDCD34"),
            SLDCD35("SLDCD35"),
            SLDCD36("SLDCD36"),
            SLDCD37("SLDCD37"),
            SLDCD38("SLDCD38"),
            SLDCD39("SLDCD39"),
            SLDCD40("SLDCD40"),
            SLDCD41("SLDCD41"),
            SLDCD42("SLDCD42"),
            SLDCD43("SLDCD43"),
            SLDCD44("SLDCD44"),
            SLDCD45("SLDCD45"),
            SLDCD46("SLDCD46"),
            SLDCD47("SLDCD47"),
            SLDCD48("SLDCD48"),
            SLDCD49("SLDCD49"),
            SLDCD50("SLDCD50"),
            SLDCD51("SLDCD51"),
            SLDCD52("SLDCD52"),
            SLDCD53("SLDCD53"),
            SLDCD54("SLDCD54"),
            SLDCD55("SLDCD55"),
            SLDCD56("SLDCD56"),
            SLDCD57("SLDCD57"),
            SLDCD58("SLDCD58"),
            SLDCD59("SLDCD59"),
            SLDCD60("SLDCD60"),
            SLDCD61("SLDCD61"),
            SLDCD62("SLDCD62"),
            SLDCD63("SLDCD63"),
            // TimerE - select
            SLDE0("SLDE0"),
            SLDE1("SLDE1"),
            SLDE2("SLDE2"),
            SLDE3("SLDE3"),
            SLDE4("SLDE4"),
            SLDE5("SLDE5"),
            SLDE6("SLDE6"),
            SLDE7("SLDE7"),
            SLDE8("SLDE8")
{//{{{
    //Bind clock, reset signal and socket to Bus IF
    setSlaveFreqPort32(&PCLK);
    setSlaveResetPort32(&PRESETZ);
    setTargetSocket32((char*)"ts");
    ts = BusSlaveBase<32,1>::tSocket32[0]; //Alias for binding

    mATU5_GLUE_Func = new ATU5_GLUE_Func((std::string)name, this);
    sc_assert(mATU5_GLUE_Func != NULL);
    mBusSlaveIf32[0]->setFuncModulePtr(mATU5_GLUE_Func);
    mBusSlaveIf32[0]->setBusProtocol(BUS_APB);
    mBusSlaveIf32[0]->setReadInitLatency(rLatency);
    mBusSlaveIf32[0]->setWriteInitLatency(wLatency);
    mBusSlaveIf32[0]->setTransNmbLmt(0x1);

    mTimeResolutionValue = 1;
    mTimeResolutionUnit = SC_NS;
    GetTimeResolution(mTimeResolutionValue, mTimeResolutionUnit);
    
    //Initialize variables
    mPCLKPeriod = 0;
    mPCLKFreq = 0;
    mPCLKOrgFreq = 0;
    mPCLKFreqUnit = "Hz";

    mResetCurVal = true;
    mResetPreVal = true;
    mIsResetHardActive = false;
    mIsResetCmdReceive = false;
    mIsResetCmdActive = false;
    mResetCmdPeriod =  0;

    mDumpInterrupt = false;

    // Initialize Output ports
    // PSI5
    P5TGC0.initialize(false);
    P5TGC1.initialize(false);
    P5TGC2.initialize(false);
    P5TGC3.initialize(false);
    P5TGC4.initialize(false);
    P5TGC5.initialize(false);
    P5TGC6.initialize(false);
    P5TGC7.initialize(false);
    P5CKTSA.initialize(false);
    P5CKTSB.initialize(false);
    P5CRTSA.initialize(false);
    P5CRTSB.initialize(false);
    P5SSTSA.initialize(false);
    P5SSTSB.initialize(false);
    // DFE
    DEG1TRG.initialize(false);
    DFC800.initialize(false);
    DFC801.initialize(false);
    DFC802.initialize(false);
    DFC810.initialize(false);
    DFC811.initialize(false);
    DFC812.initialize(false);
    DFC820.initialize(false);
    DFC821.initialize(false);
    DFC822.initialize(false);
    DFC830.initialize(false);
    DFC831.initialize(false);
    DFC832.initialize(false);
    DFC900.initialize(false);
    DFC901.initialize(false);
    DFC902.initialize(false);
    DFC910.initialize(false);
    DFC911.initialize(false);
    DFC912.initialize(false);
    DFD000.initialize(false);
    DFD001.initialize(false);
    DFD002.initialize(false);
    DFD010.initialize(false);
    DFD011.initialize(false);
    DFD012.initialize(false);
    DFD020.initialize(false);
    DFD021.initialize(false);
    DFD022.initialize(false);
    DFD030.initialize(false);
    DFD031.initialize(false);
    DFD032.initialize(false);
    DFD100.initialize(false);
    DFD101.initialize(false);
    DFD102.initialize(false);
    DFD110.initialize(false);
    DFD111.initialize(false);
    DFD112.initialize(false);
    // ADC
    DSTS0.initialize(false);
    DSTS1.initialize(false);
    DSTS2.initialize(false);
    DSTS3.initialize(false);
    DSTS4.initialize(false);
    DSTS5.initialize(false);
    DSTS6.initialize(false);
    DSTS7.initialize(false);
    DSTS8.initialize(false);
    DSTS9.initialize(false);
    CATS.initialize(false);
    // PGL
    // TimerA
    ICIA0.initialize(false);
    ICIA1.initialize(false);
    ICIA2.initialize(false);
    ICIA3.initialize(false);
    ICIA4.initialize(false);
    ICIA5.initialize(false);
    ICIA6.initialize(false);
    ICIA7.initialize(false);
    // TimerB
    CMIB0.initialize(false);
    CMIB1.initialize(false);
    CMIB6.initialize(false);
    CMIB10.initialize(false);
    CMIB11.initialize(false);
    CMIB12.initialize(false);
    CMIB6M.initialize(false);
    CMIB66M.initialize(false);
    ICIB0.initialize(false);
    // TimerE
    CMIE00.initialize(false);
    CMIE01.initialize(false);
    CMIE02.initialize(false);
    CMIE03.initialize(false);
    CMIE10.initialize(false);
    CMIE11.initialize(false);
    CMIE12.initialize(false);
    CMIE13.initialize(false);
    CMIE20.initialize(false);
    CMIE21.initialize(false);
    CMIE22.initialize(false);
    CMIE23.initialize(false);
    CMIE30.initialize(false);
    CMIE31.initialize(false);
    CMIE32.initialize(false);
    CMIE33.initialize(false);
    CMIE40.initialize(false);
    CMIE41.initialize(false);
    CMIE42.initialize(false);
    CMIE43.initialize(false);
    CMIE50.initialize(false);
    CMIE51.initialize(false);
    CMIE52.initialize(false);
    CMIE53.initialize(false);
    CMIE60.initialize(false);
    CMIE61.initialize(false);
    CMIE62.initialize(false);
    CMIE63.initialize(false);
    CMIE70.initialize(false);
    CMIE71.initialize(false);
    CMIE72.initialize(false);
    CMIE73.initialize(false);
    CMIE80.initialize(false);
    CMIE81.initialize(false);
    CMIE82.initialize(false);
    CMIE83.initialize(false);
    // TimerF
    ICIF0.initialize(false);
    ICIF1.initialize(false);
    ICIF2.initialize(false);
    ICIF3.initialize(false);
    ICIF4.initialize(false);
    ICIF5.initialize(false);
    ICIF6.initialize(false);
    ICIF7.initialize(false);
    ICIF8.initialize(false);
    ICIF9.initialize(false);
    ICIF10.initialize(false);
    ICIF11.initialize(false);
    ICIF12.initialize(false);
    ICIF13.initialize(false);
    ICIF14.initialize(false);
    ICIF15.initialize(false);
    // TimerG
    CMIG0.initialize(false);
    CMIG1.initialize(false);
    CMIG2.initialize(false);
    CMIG3.initialize(false);
    CMIG4.initialize(false);
    CMIG5.initialize(false);
    CMIG6.initialize(false);
    CMIG7.initialize(false);
    CMIG8.initialize(false);
    CMIG9.initialize(false);
    DMAREQ00.initialize(false);
    DMAREQ01.initialize(false);
    DMAREQ10.initialize(false);
    DMAREQ11.initialize(false);
    // Overflow
    OVF.initialize(false);
    // TimerA - select
    SLIA0.initialize(false);
    SLIA1.initialize(false);
    SLIA2.initialize(false);
    SLIA3.initialize(false);
    // TimerB - select
    SLDB0.initialize(false);
    SLDB1.initialize(false);
    SLDB2.initialize(false);
    SLDB3.initialize(false);
    // TimerC - select
    SLIC00.initialize(false);
    SLIC01.initialize(false);
    SLIC02.initialize(false);
    SLIC03.initialize(false);
    SLIC10.initialize(false);
    SLIC11.initialize(false);
    SLIC12.initialize(false);
    SLIC13.initialize(false);
    SLIC20.initialize(false);
    SLIC21.initialize(false);
    SLIC22.initialize(false);
    SLIC23.initialize(false);
    SLIC30.initialize(false);
    SLIC31.initialize(false);
    SLIC32.initialize(false);
    SLIC33.initialize(false);
    SLIC40.initialize(false);
    SLIC41.initialize(false);
    SLIC42.initialize(false);
    SLIC43.initialize(false);
    SLIC50.initialize(false);
    SLIC51.initialize(false);
    SLIC52.initialize(false);
    SLIC53.initialize(false);
    SLIC60.initialize(false);
    SLIC61.initialize(false);
    SLIC62.initialize(false);
    SLIC63.initialize(false);
    SLIC70.initialize(false);
    SLIC71.initialize(false);
    SLIC72.initialize(false);
    SLIC73.initialize(false);
    SLIC80.initialize(false);
    SLIC81.initialize(false);
    SLIC82.initialize(false);
    SLIC83.initialize(false);
    SLIC90.initialize(false);
    SLIC91.initialize(false);
    SLIC92.initialize(false);
    SLIC93.initialize(false);
    SLIC100.initialize(false);
    SLIC101.initialize(false);
    SLIC102.initialize(false);
    SLIC103.initialize(false);
    // TimerD - select
    SLID00.initialize(false);
    SLID01.initialize(false);
    SLID02.initialize(false);
    SLID03.initialize(false);
    SLID10.initialize(false);
    SLID11.initialize(false);
    SLID12.initialize(false);
    SLID13.initialize(false);
    SLID20.initialize(false);
    SLID21.initialize(false);
    SLID22.initialize(false);
    SLID23.initialize(false);
    SLID30.initialize(false);
    SLID31.initialize(false);
    SLID32.initialize(false);
    SLID33.initialize(false);
    SLID40.initialize(false);
    SLID41.initialize(false);
    SLID42.initialize(false);
    SLID43.initialize(false);
    SLID50.initialize(false);
    SLID51.initialize(false);
    SLID52.initialize(false);
    SLID53.initialize(false);
    SLID60.initialize(false);
    SLID61.initialize(false);
    SLID62.initialize(false);
    SLID63.initialize(false);
    SLID70.initialize(false);
    SLID71.initialize(false);
    SLID72.initialize(false);
    SLID73.initialize(false);
    SLID80.initialize(false);
    SLID81.initialize(false);
    SLID82.initialize(false);
    SLID83.initialize(false);
    // TimerC-D - select
    SLDCD0.initialize(false);
    SLDCD1.initialize(false);
    SLDCD2.initialize(false);
    SLDCD3.initialize(false);
    SLDCD4.initialize(false);
    SLDCD5.initialize(false);
    SLDCD6.initialize(false);
    SLDCD7.initialize(false);
    SLDCD8.initialize(false);
    SLDCD9.initialize(false);
    SLDCD10.initialize(false);
    SLDCD11.initialize(false);
    SLDCD12.initialize(false);
    SLDCD13.initialize(false);
    SLDCD14.initialize(false);
    SLDCD15.initialize(false);
    SLDCD16.initialize(false);
    SLDCD17.initialize(false);
    SLDCD18.initialize(false);
    SLDCD19.initialize(false);
    SLDCD20.initialize(false);
    SLDCD21.initialize(false);
    SLDCD22.initialize(false);
    SLDCD23.initialize(false);
    SLDCD24.initialize(false);
    SLDCD25.initialize(false);
    SLDCD26.initialize(false);
    SLDCD27.initialize(false);
    SLDCD28.initialize(false);
    SLDCD29.initialize(false);
    SLDCD30.initialize(false);
    SLDCD31.initialize(false);
    SLDCD32.initialize(false);
    SLDCD33.initialize(false);
    SLDCD34.initialize(false);
    SLDCD35.initialize(false);
    SLDCD36.initialize(false);
    SLDCD37.initialize(false);
    SLDCD38.initialize(false);
    SLDCD39.initialize(false);
    SLDCD40.initialize(false);
    SLDCD41.initialize(false);
    SLDCD42.initialize(false);
    SLDCD43.initialize(false);
    SLDCD44.initialize(false);
    SLDCD45.initialize(false);
    SLDCD46.initialize(false);
    SLDCD47.initialize(false);
    SLDCD48.initialize(false);
    SLDCD49.initialize(false);
    SLDCD50.initialize(false);
    SLDCD51.initialize(false);
    SLDCD52.initialize(false);
    SLDCD53.initialize(false);
    SLDCD54.initialize(false);
    SLDCD55.initialize(false);
    SLDCD56.initialize(false);
    SLDCD57.initialize(false);
    SLDCD58.initialize(false);
    SLDCD59.initialize(false);
    SLDCD60.initialize(false);
    SLDCD61.initialize(false);
    SLDCD62.initialize(false);
    SLDCD63.initialize(false);
    // TimerE - select
    SLDE0.initialize(false);
    SLDE1.initialize(false);
    SLDE2.initialize(false);
    SLDE3.initialize(false);
    SLDE4.initialize(false);
    SLDE5.initialize(false);
    SLDE6.initialize(false);
    SLDE7.initialize(false);
    SLDE8.initialize(false);
    
    SC_METHOD(HandlePCLKSignalMethod);
    dont_initialize();
    sensitive << PCLK;
    
    SC_METHOD(HandlePRESETZSignalMethod);
    sensitive << PRESETZ;
    
    SC_METHOD(WriteSLIAxMethod);
    dont_initialize();
    sensitive << INTATUAICIA0;
    sensitive << INTATUAICIA1;
    sensitive << INTATUAICIA2;
    sensitive << INTATUAICIA3;
    sensitive << INTATUAICIA4;
    sensitive << INTATUAICIA5;
    sensitive << INTATUAICIA6;
    sensitive << INTATUAICIA7;
    sensitive << mINTSELAregChangeEvent;

    SC_METHOD(WriteSLDBxMethod);
    dont_initialize();
    sensitive << INTATUBCMIB0;
    sensitive << INTATUBCMIB1;
    sensitive << INTATUBCMIB6;
    sensitive << INTATUBCMIB10;
    sensitive << INTATUBCMIB11;
    sensitive << INTATUBCMIB12;
    sensitive << INTATUBCMIB6M;
    sensitive << INTATUBCMIB66M;
    sensitive << INTATUBICIB0;
    sensitive << mDMASELBregChangeEvent;
    
    SC_METHOD(WriteOVFMethod);
    dont_initialize();
    sensitive << OVIA;
    sensitive << OVIC0;
    sensitive << OVIC1;
    sensitive << OVIC2;
    sensitive << OVIC3;
    sensitive << OVIC4;
    sensitive << OVIC5;
    sensitive << OVIC6;
    sensitive << OVIC7;
    sensitive << OVIC8;
    sensitive << OVIC9;
    sensitive << OVIC10;
    sensitive << OVI1D0;
    sensitive << OVI1D1;
    sensitive << OVI1D2;
    sensitive << OVI1D3;
    sensitive << OVI1D4;
    sensitive << OVI1D5;
    sensitive << OVI1D6;
    sensitive << OVI1D7;
    sensitive << OVI1D8;
    sensitive << OVI2D0;
    sensitive << OVI2D1;
    sensitive << OVI2D2;
    sensitive << OVI2D3;
    sensitive << OVI2D4;
    sensitive << OVI2D5;
    sensitive << OVI2D6;
    sensitive << OVI2D7;
    sensitive << OVI2D8;
    sensitive << OVIF0;
    sensitive << OVIF1;
    sensitive << OVIF2;
    sensitive << OVIF3;
    sensitive << OVIF4;
    sensitive << OVIF5;
    sensitive << OVIF6;
    sensitive << OVIF7;
    sensitive << OVIF8;
    sensitive << OVIF9;
    sensitive << OVIF10;
    sensitive << OVIF11;
    sensitive << OVIF12;
    sensitive << OVIF13;
    sensitive << OVIF14;
    sensitive << OVIF15;
    
    for (unsigned int i = 0; i < emSLICNum; i++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        switch (i) {
            case 0:
                opt.set_sensitivity(&IMIGC00);
                opt.set_sensitivity(&IMIOC00);
                break;
            case 1:
                opt.set_sensitivity(&IMIGC01);
                opt.set_sensitivity(&IMIOC01);
                break;
            case 2:
                opt.set_sensitivity(&IMIGC02);
                opt.set_sensitivity(&IMIOC02);
                break;
            case 3:
                opt.set_sensitivity(&IMIGC03);
                opt.set_sensitivity(&IMIOC03);
                break;
            case 4:
                opt.set_sensitivity(&IMIGC10);
                opt.set_sensitivity(&IMIOC10);
                break;
            case 5:
                opt.set_sensitivity(&IMIGC11);
                opt.set_sensitivity(&IMIOC11);
                break;
            case 6:
                opt.set_sensitivity(&IMIGC12);
                opt.set_sensitivity(&IMIOC12);
                break;
            case 7:
                opt.set_sensitivity(&IMIGC13);
                opt.set_sensitivity(&IMIOC13);
                break;
            case 8:
                opt.set_sensitivity(&IMIGC20);
                opt.set_sensitivity(&IMIOC20);
                break;
            case 9:
                opt.set_sensitivity(&IMIGC21);
                opt.set_sensitivity(&IMIOC21);
                break;
            case 10:
                opt.set_sensitivity(&IMIGC22);
                opt.set_sensitivity(&IMIOC22);
                break;
            case 11:
                opt.set_sensitivity(&IMIGC23);
                opt.set_sensitivity(&IMIOC23);
                break;
            case 12:
                opt.set_sensitivity(&IMIGC30);
                opt.set_sensitivity(&IMIOC30);
                break;
            case 13:
                opt.set_sensitivity(&IMIGC31);
                opt.set_sensitivity(&IMIOC31);
                break;
            case 14:
                opt.set_sensitivity(&IMIGC32);
                opt.set_sensitivity(&IMIOC32);
                break;
            case 15:
                opt.set_sensitivity(&IMIGC33);
                opt.set_sensitivity(&IMIOC33);
                break;
            case 16:
                opt.set_sensitivity(&IMIGC40);
                opt.set_sensitivity(&IMIOC40);
                break;
            case 17:
                opt.set_sensitivity(&IMIGC41);
                opt.set_sensitivity(&IMIOC41);
                break;
            case 18:
                opt.set_sensitivity(&IMIGC42);
                opt.set_sensitivity(&IMIOC42);
                break;
            case 19:
                opt.set_sensitivity(&IMIGC43);
                opt.set_sensitivity(&IMIOC43);
                break;
            case 20:
                opt.set_sensitivity(&IMIGC50);
                opt.set_sensitivity(&IMIOC50);
                break;
            case 21:
                opt.set_sensitivity(&IMIGC51);
                opt.set_sensitivity(&IMIOC51);
                break;
            case 22:
                opt.set_sensitivity(&IMIGC52);
                opt.set_sensitivity(&IMIOC52);
                break;
            case 23:
                opt.set_sensitivity(&IMIGC53);
                opt.set_sensitivity(&IMIOC53);
                break;
            case 24:
                opt.set_sensitivity(&IMIGC60);
                opt.set_sensitivity(&IMIOC60);
                break;
            case 25:
                opt.set_sensitivity(&IMIGC61);
                opt.set_sensitivity(&IMIOC61);
                break;
            case 26:
                opt.set_sensitivity(&IMIGC62);
                opt.set_sensitivity(&IMIOC62);
                break;
            case 27:
                opt.set_sensitivity(&IMIGC63);
                opt.set_sensitivity(&IMIOC63);
                break;
            case 28:
                opt.set_sensitivity(&IMIGC70);
                opt.set_sensitivity(&IMIOC70);
                break;
            case 29:
                opt.set_sensitivity(&IMIGC71);
                opt.set_sensitivity(&IMIOC71);
                break;
            case 30:
                opt.set_sensitivity(&IMIGC72);
                opt.set_sensitivity(&IMIOC72);
                break;
            case 31:
                opt.set_sensitivity(&IMIGC73);
                opt.set_sensitivity(&IMIOC73);
                break;
            case 32:
                opt.set_sensitivity(&IMIGC80);
                opt.set_sensitivity(&IMIOC80);
                break;
            case 33:
                opt.set_sensitivity(&IMIGC81);
                opt.set_sensitivity(&IMIOC81);
                break;
            case 34:
                opt.set_sensitivity(&IMIGC82);
                opt.set_sensitivity(&IMIOC82);
                break;
            case 35:
                opt.set_sensitivity(&IMIGC83);
                opt.set_sensitivity(&IMIOC83);
            case 36:
                opt.set_sensitivity(&IMIGC90);
                opt.set_sensitivity(&IMIOC90);
            case 37:
                opt.set_sensitivity(&IMIGC91);
                opt.set_sensitivity(&IMIOC91);
            case 38:
                opt.set_sensitivity(&IMIGC92);
                opt.set_sensitivity(&IMIOC92);
            case 39:
                opt.set_sensitivity(&IMIGC93);
                opt.set_sensitivity(&IMIOC93);
            case 40:
                opt.set_sensitivity(&IMIGC100);
                opt.set_sensitivity(&IMIOC100);
            case 41:
                opt.set_sensitivity(&IMIGC101);
                opt.set_sensitivity(&IMIOC101);
            case 42:
                opt.set_sensitivity(&IMIGC102);
                opt.set_sensitivity(&IMIOC102);
            default:    // 43
                opt.set_sensitivity(&IMIGC103);
                opt.set_sensitivity(&IMIOC103);
                break;
        }
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ATU5_GLUE::WriteSLICxMethod, this, i),
                          sc_core::sc_gen_unique_name("WriteSLICxMethod"), &opt);
    }
    for (unsigned int i = 0; i < emSLIDNum; i++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        switch (i) {
            case 0:
                opt.set_sensitivity(&CMIAD00);
                opt.set_sensitivity(&CMIBD00);
                opt.set_sensitivity(&PREID00);
                opt.set_sensitivity(&PFEID00);
                opt.set_sensitivity(&UDID00);
                break;
            case 1:
                opt.set_sensitivity(&CMIAD01);
                opt.set_sensitivity(&CMIBD01);
                opt.set_sensitivity(&PREID01);
                opt.set_sensitivity(&PFEID01);
                opt.set_sensitivity(&UDID01);
                break;
            case 2:
                opt.set_sensitivity(&CMIAD02);
                opt.set_sensitivity(&CMIBD02);
                opt.set_sensitivity(&PREID02);
                opt.set_sensitivity(&PFEID02);
                opt.set_sensitivity(&UDID02);
                break;
            case 3:
                opt.set_sensitivity(&CMIAD03);
                opt.set_sensitivity(&CMIBD03);
                opt.set_sensitivity(&PREID03);
                opt.set_sensitivity(&PFEID03);
                opt.set_sensitivity(&UDID03);
                break;
            case 4:
                opt.set_sensitivity(&CMIAD10);
                opt.set_sensitivity(&CMIBD10);
                opt.set_sensitivity(&PREID10);
                opt.set_sensitivity(&PFEID10);
                opt.set_sensitivity(&UDID10);
                break;
            case 5:
                opt.set_sensitivity(&CMIAD11);
                opt.set_sensitivity(&CMIBD11);
                opt.set_sensitivity(&PREID11);
                opt.set_sensitivity(&PFEID11);
                opt.set_sensitivity(&UDID11);
                break;
            case 6:
                opt.set_sensitivity(&CMIAD12);
                opt.set_sensitivity(&CMIBD12);
                opt.set_sensitivity(&PREID12);
                opt.set_sensitivity(&PFEID12);
                opt.set_sensitivity(&UDID12);
                break;
            case 7:
                opt.set_sensitivity(&CMIAD13);
                opt.set_sensitivity(&CMIBD13);
                opt.set_sensitivity(&PREID13);
                opt.set_sensitivity(&PFEID13);
                opt.set_sensitivity(&UDID13);
                break;
            case 8:
                opt.set_sensitivity(&CMIAD20);
                opt.set_sensitivity(&CMIBD20);
                opt.set_sensitivity(&PREID20);
                opt.set_sensitivity(&PFEID20);
                opt.set_sensitivity(&UDID20);
                break;
            case 9:
                opt.set_sensitivity(&CMIAD21);
                opt.set_sensitivity(&CMIBD21);
                opt.set_sensitivity(&PREID21);
                opt.set_sensitivity(&PFEID21);
                opt.set_sensitivity(&UDID21);
                break;
            case 10:
                opt.set_sensitivity(&CMIAD22);
                opt.set_sensitivity(&CMIBD22);
                opt.set_sensitivity(&PREID22);
                opt.set_sensitivity(&PFEID22);
                opt.set_sensitivity(&UDID22);
                break;
            case 11:
                opt.set_sensitivity(&CMIAD23);
                opt.set_sensitivity(&CMIBD23);
                opt.set_sensitivity(&PREID23);
                opt.set_sensitivity(&PFEID23);
                opt.set_sensitivity(&UDID23);
                break;
            case 12:
                opt.set_sensitivity(&CMIAD30);
                opt.set_sensitivity(&CMIBD30);
                opt.set_sensitivity(&PREID30);
                opt.set_sensitivity(&PFEID30);
                opt.set_sensitivity(&UDID30);
                break;
            case 13:
                opt.set_sensitivity(&CMIAD31);
                opt.set_sensitivity(&CMIBD31);
                opt.set_sensitivity(&PREID31);
                opt.set_sensitivity(&PFEID31);
                opt.set_sensitivity(&UDID31);
                break;
            case 14:
                opt.set_sensitivity(&CMIAD32);
                opt.set_sensitivity(&CMIBD32);
                opt.set_sensitivity(&PREID32);
                opt.set_sensitivity(&PFEID32);
                opt.set_sensitivity(&UDID32);
                break;
            case 15:
                opt.set_sensitivity(&CMIAD33);
                opt.set_sensitivity(&CMIBD33);
                opt.set_sensitivity(&PREID33);
                opt.set_sensitivity(&PFEID33);
                opt.set_sensitivity(&UDID33);
                break;
            case 16:
                opt.set_sensitivity(&CMIAD40);
                opt.set_sensitivity(&CMIBD40);
                opt.set_sensitivity(&PREID40);
                opt.set_sensitivity(&PFEID40);
                opt.set_sensitivity(&UDID40);
                break;
            case 17:
                opt.set_sensitivity(&CMIAD41);
                opt.set_sensitivity(&CMIBD41);
                opt.set_sensitivity(&PREID41);
                opt.set_sensitivity(&PFEID41);
                opt.set_sensitivity(&UDID41);
                break;
            case 18:
                opt.set_sensitivity(&CMIAD42);
                opt.set_sensitivity(&CMIBD42);
                opt.set_sensitivity(&PREID42);
                opt.set_sensitivity(&PFEID42);
                opt.set_sensitivity(&UDID42);
                break;
            case 19:
                opt.set_sensitivity(&CMIAD43);
                opt.set_sensitivity(&CMIBD43);
                opt.set_sensitivity(&PREID43);
                opt.set_sensitivity(&PFEID43);
                opt.set_sensitivity(&UDID43);
                break;
            case 20:
                opt.set_sensitivity(&CMIAD50);
                opt.set_sensitivity(&CMIBD50);
                opt.set_sensitivity(&PREID50);
                opt.set_sensitivity(&PFEID50);
                opt.set_sensitivity(&UDID50);
                break;
            case 21:
                opt.set_sensitivity(&CMIAD51);
                opt.set_sensitivity(&CMIBD51);
                opt.set_sensitivity(&PREID51);
                opt.set_sensitivity(&PFEID51);
                opt.set_sensitivity(&UDID51);
                break;
            case 22:
                opt.set_sensitivity(&CMIAD52);
                opt.set_sensitivity(&CMIBD52);
                opt.set_sensitivity(&PREID52);
                opt.set_sensitivity(&PFEID52);
                opt.set_sensitivity(&UDID52);
                break;
            case 23:
                opt.set_sensitivity(&CMIAD53);
                opt.set_sensitivity(&CMIBD53);
                opt.set_sensitivity(&PREID53);
                opt.set_sensitivity(&PFEID53);
                opt.set_sensitivity(&UDID53);
                break;
            case 24:
                opt.set_sensitivity(&CMIAD60);
                opt.set_sensitivity(&CMIBD60);
                opt.set_sensitivity(&PREID60);
                opt.set_sensitivity(&PFEID60);
                opt.set_sensitivity(&UDID60);
                break;
            case 25:
                opt.set_sensitivity(&CMIAD61);
                opt.set_sensitivity(&CMIBD61);
                opt.set_sensitivity(&PREID61);
                opt.set_sensitivity(&PFEID61);
                opt.set_sensitivity(&UDID61);
                break;
            case 26:
                opt.set_sensitivity(&CMIAD62);
                opt.set_sensitivity(&CMIBD62);
                opt.set_sensitivity(&PREID62);
                opt.set_sensitivity(&PFEID62);
                opt.set_sensitivity(&UDID62);
                break;
            case 27:
                opt.set_sensitivity(&CMIAD63);
                opt.set_sensitivity(&CMIBD63);
                opt.set_sensitivity(&PREID63);
                opt.set_sensitivity(&PFEID63);
                opt.set_sensitivity(&UDID63);
                break;
            case 28:
                opt.set_sensitivity(&CMIAD70);
                opt.set_sensitivity(&CMIBD70);
                opt.set_sensitivity(&PREID70);
                opt.set_sensitivity(&PFEID70);
                opt.set_sensitivity(&UDID70);
                break;
            case 29:
                opt.set_sensitivity(&CMIAD71);
                opt.set_sensitivity(&CMIBD71);
                opt.set_sensitivity(&PREID71);
                opt.set_sensitivity(&PFEID71);
                opt.set_sensitivity(&UDID71);
                break;
            case 30:
                opt.set_sensitivity(&CMIAD72);
                opt.set_sensitivity(&CMIBD72);
                opt.set_sensitivity(&PREID72);
                opt.set_sensitivity(&PFEID72);
                opt.set_sensitivity(&UDID72);
                break;
            case 31:
                opt.set_sensitivity(&CMIAD73);
                opt.set_sensitivity(&CMIBD73);
                opt.set_sensitivity(&PREID73);
                opt.set_sensitivity(&PFEID73);
                opt.set_sensitivity(&UDID73);
                break;
            case 32:
                opt.set_sensitivity(&CMIAD80);
                opt.set_sensitivity(&CMIBD80);
                opt.set_sensitivity(&PREID80);
                opt.set_sensitivity(&PFEID80);
                opt.set_sensitivity(&UDID80);
                break;
            case 33:
                opt.set_sensitivity(&CMIAD81);
                opt.set_sensitivity(&CMIBD81);
                opt.set_sensitivity(&PREID81);
                opt.set_sensitivity(&PFEID81);
                opt.set_sensitivity(&UDID81);
                break;
            case 34:
                opt.set_sensitivity(&CMIAD82);
                opt.set_sensitivity(&CMIBD82);
                opt.set_sensitivity(&PREID82);
                opt.set_sensitivity(&PFEID82);
                opt.set_sensitivity(&UDID82);
                break;
            default:    // 35
                opt.set_sensitivity(&CMIAD83);
                opt.set_sensitivity(&CMIBD83);
                opt.set_sensitivity(&PREID83);
                opt.set_sensitivity(&PFEID83);
                opt.set_sensitivity(&UDID83);
                break;
        }
        opt.set_sensitivity(&mINTSELDregChangeEvent[i >> 3]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ATU5_GLUE::WriteSLIDxMethod, this, i),
                          sc_core::sc_gen_unique_name("WriteSLIDxMethod"), &opt);
    }
    
    for (unsigned int i = 0; i < emSLDCDNum; i++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        switch (i) {
            case 0:
                opt.set_sensitivity(&IMIGC00);
                opt.set_sensitivity(&IMIOC00);
                opt.set_sensitivity(&CMIAD80);
                opt.set_sensitivity(&CMIBD80);
                opt.set_sensitivity(&PREID80);
                opt.set_sensitivity(&PFEID80);
                opt.set_sensitivity(&UDID80);
                break;
            case 1:
                opt.set_sensitivity(&IMIGC01);
                opt.set_sensitivity(&IMIOC01);
                opt.set_sensitivity(&CMIAD81);
                opt.set_sensitivity(&CMIBD81);
                opt.set_sensitivity(&PREID81);
                opt.set_sensitivity(&PFEID81);
                opt.set_sensitivity(&UDID81);
                break;
            case 2:
                opt.set_sensitivity(&IMIGC02);
                opt.set_sensitivity(&IMIOC02);
                opt.set_sensitivity(&CMIAD82);
                opt.set_sensitivity(&CMIBD82);
                opt.set_sensitivity(&PREID82);
                opt.set_sensitivity(&PFEID82);
                opt.set_sensitivity(&UDID82);
                break;
            case 3:
                opt.set_sensitivity(&IMIGC03);
                opt.set_sensitivity(&IMIOC03);
                opt.set_sensitivity(&CMIAD83);
                opt.set_sensitivity(&CMIBD83);
                opt.set_sensitivity(&PREID83);
                opt.set_sensitivity(&PFEID83);
                opt.set_sensitivity(&UDID83);
                break;
            case 4:
                opt.set_sensitivity(&IMIGC10);
                opt.set_sensitivity(&IMIOC10);
                break;
            case 5:
                opt.set_sensitivity(&IMIGC11);
                opt.set_sensitivity(&IMIOC11);
                break;
            case 6:
                opt.set_sensitivity(&IMIGC12);
                opt.set_sensitivity(&IMIOC12);
                break;
            case 7:
                opt.set_sensitivity(&IMIGC13);
                opt.set_sensitivity(&IMIOC13);
                break;
            case 8:
                opt.set_sensitivity(&IMIGC20);
                opt.set_sensitivity(&IMIOC20);
                break;
            case 9:
                opt.set_sensitivity(&IMIGC21);
                opt.set_sensitivity(&IMIOC21);
                break;
            case 10:
                opt.set_sensitivity(&IMIGC22);
                opt.set_sensitivity(&IMIOC22);
                break;
            case 11:
                opt.set_sensitivity(&IMIGC23);
                opt.set_sensitivity(&IMIOC23);
                break;
            case 12:
                opt.set_sensitivity(&IMIGC30);
                opt.set_sensitivity(&IMIOC30);
                break;
            case 13:
                opt.set_sensitivity(&IMIGC31);
                opt.set_sensitivity(&IMIOC31);
                break;
            case 14:
                opt.set_sensitivity(&IMIGC32);
                opt.set_sensitivity(&IMIOC32);
                break;
            case 15:
                opt.set_sensitivity(&IMIGC33);
                opt.set_sensitivity(&IMIOC33);
                break;
            case 16:
                opt.set_sensitivity(&IMIGC40);
                opt.set_sensitivity(&IMIOC40);
                break;
            case 17:
                opt.set_sensitivity(&IMIGC41);
                opt.set_sensitivity(&IMIOC41);
                break;
            case 18:
                opt.set_sensitivity(&IMIGC42);
                opt.set_sensitivity(&IMIOC42);
                break;
            case 19:
                opt.set_sensitivity(&IMIGC43);
                opt.set_sensitivity(&IMIOC43);
                break;
            case 20:
                opt.set_sensitivity(&IMIGC50);
                opt.set_sensitivity(&IMIOC50);
                break;
            case 21:
                opt.set_sensitivity(&IMIGC51);
                opt.set_sensitivity(&IMIOC51);
                break;
            case 22:
                opt.set_sensitivity(&IMIGC52);
                opt.set_sensitivity(&IMIOC52);
                break;
            case 23:
                opt.set_sensitivity(&IMIGC53);
                opt.set_sensitivity(&IMIOC53);
                break;
            case 24:
                opt.set_sensitivity(&IMIGC60);
                opt.set_sensitivity(&IMIOC60);
                break;
            case 25:
                opt.set_sensitivity(&IMIGC61);
                opt.set_sensitivity(&IMIOC61);
                break;
            case 26:
                opt.set_sensitivity(&IMIGC62);
                opt.set_sensitivity(&IMIOC62);
                break;
            case 27:
                opt.set_sensitivity(&IMIGC63);
                opt.set_sensitivity(&IMIOC63);
                break;
            case 28:
                opt.set_sensitivity(&IMIGC70);
                opt.set_sensitivity(&IMIOC70);
                break;
            case 29:
                opt.set_sensitivity(&IMIGC71);
                opt.set_sensitivity(&IMIOC71);
                break;
            case 30:
                opt.set_sensitivity(&IMIGC72);
                opt.set_sensitivity(&IMIOC72);
                break;
            case 31:
                opt.set_sensitivity(&IMIGC73);
                opt.set_sensitivity(&IMIOC73);
                break;
            case 32:
                opt.set_sensitivity(&IMIGC80);
                opt.set_sensitivity(&IMIOC80);
                opt.set_sensitivity(&CMIAD00);
                opt.set_sensitivity(&CMIBD00);
                opt.set_sensitivity(&PREID00);
                opt.set_sensitivity(&PFEID00);
                opt.set_sensitivity(&UDID00);
                break;
            case 33:
                opt.set_sensitivity(&IMIGC81);
                opt.set_sensitivity(&IMIOC81);
                opt.set_sensitivity(&CMIAD01);
                opt.set_sensitivity(&CMIBD01);
                opt.set_sensitivity(&PREID01);
                opt.set_sensitivity(&PFEID01);
                opt.set_sensitivity(&UDID01);
                break;
            case 34:
                opt.set_sensitivity(&IMIGC82);
                opt.set_sensitivity(&IMIOC82);
                opt.set_sensitivity(&CMIAD02);
                opt.set_sensitivity(&CMIBD02);
                opt.set_sensitivity(&PREID02);
                opt.set_sensitivity(&PFEID02);
                opt.set_sensitivity(&UDID02);
                break;
            case 35:
                opt.set_sensitivity(&IMIGC83);
                opt.set_sensitivity(&IMIOC83);
                opt.set_sensitivity(&CMIAD03);
                opt.set_sensitivity(&CMIBD03);
                opt.set_sensitivity(&PREID03);
                opt.set_sensitivity(&PFEID03);
                opt.set_sensitivity(&UDID03);
                break;
            case 36:
                opt.set_sensitivity(&IMIGC90);
                opt.set_sensitivity(&IMIOC90);
                opt.set_sensitivity(&CMIAD10);
                opt.set_sensitivity(&CMIBD10);
                opt.set_sensitivity(&PREID10);
                opt.set_sensitivity(&PFEID10);
                opt.set_sensitivity(&UDID10);
                break;
            case 37:
                opt.set_sensitivity(&IMIGC91);
                opt.set_sensitivity(&IMIOC91);
                opt.set_sensitivity(&CMIAD11);
                opt.set_sensitivity(&CMIBD11);
                opt.set_sensitivity(&PREID11);
                opt.set_sensitivity(&PFEID11);
                opt.set_sensitivity(&UDID11);
                break;
            case 38:
                opt.set_sensitivity(&IMIGC92);
                opt.set_sensitivity(&IMIOC92);
                opt.set_sensitivity(&CMIAD12);
                opt.set_sensitivity(&CMIBD12);
                opt.set_sensitivity(&PREID12);
                opt.set_sensitivity(&PFEID12);
                opt.set_sensitivity(&UDID12);
                break;
            case 39:
                opt.set_sensitivity(&IMIGC93);
                opt.set_sensitivity(&IMIOC93);
                opt.set_sensitivity(&CMIAD13);
                opt.set_sensitivity(&CMIBD13);
                opt.set_sensitivity(&PREID13);
                opt.set_sensitivity(&PFEID13);
                opt.set_sensitivity(&UDID13);
                break;
            case 40:
                opt.set_sensitivity(&IMIGC100);
                opt.set_sensitivity(&IMIOC100);
                opt.set_sensitivity(&CMIAD20);
                opt.set_sensitivity(&CMIBD20);
                opt.set_sensitivity(&PREID20);
                opt.set_sensitivity(&PFEID20);
                opt.set_sensitivity(&UDID20);
                break;
            case 41:
                opt.set_sensitivity(&IMIGC101);
                opt.set_sensitivity(&IMIOC101);
                opt.set_sensitivity(&CMIAD21);
                opt.set_sensitivity(&CMIBD21);
                opt.set_sensitivity(&PREID21);
                opt.set_sensitivity(&PFEID21);
                opt.set_sensitivity(&UDID21);
                break;
            case 42:
                opt.set_sensitivity(&IMIGC102);
                opt.set_sensitivity(&IMIOC102);
                opt.set_sensitivity(&CMIAD22);
                opt.set_sensitivity(&CMIBD22);
                opt.set_sensitivity(&PREID22);
                opt.set_sensitivity(&PFEID22);
                opt.set_sensitivity(&UDID22);
                break;
            case 43:
                opt.set_sensitivity(&IMIGC103);
                opt.set_sensitivity(&IMIOC103);
                opt.set_sensitivity(&CMIAD23);
                opt.set_sensitivity(&CMIBD23);
                opt.set_sensitivity(&PREID23);
                opt.set_sensitivity(&PFEID23);
                opt.set_sensitivity(&UDID23);
                break;
            case 44:
                opt.set_sensitivity(&CMIAD30);
                opt.set_sensitivity(&CMIBD30);
                opt.set_sensitivity(&PREID30);
                opt.set_sensitivity(&PFEID30);
                opt.set_sensitivity(&UDID30);
                break;
            case 45:
                opt.set_sensitivity(&CMIAD31);
                opt.set_sensitivity(&CMIBD31);
                opt.set_sensitivity(&PREID31);
                opt.set_sensitivity(&PFEID31);
                opt.set_sensitivity(&UDID31);
                break;
            case 46:
                opt.set_sensitivity(&CMIAD32);
                opt.set_sensitivity(&CMIBD32);
                opt.set_sensitivity(&PREID32);
                opt.set_sensitivity(&PFEID32);
                opt.set_sensitivity(&UDID32);
                break;
            case 47:
                opt.set_sensitivity(&CMIAD33);
                opt.set_sensitivity(&CMIBD33);
                opt.set_sensitivity(&PREID33);
                opt.set_sensitivity(&PFEID33);
                opt.set_sensitivity(&UDID33);
                break;
            case 48:
                opt.set_sensitivity(&CMIAD40);
                opt.set_sensitivity(&CMIBD40);
                opt.set_sensitivity(&PREID40);
                opt.set_sensitivity(&PFEID40);
                opt.set_sensitivity(&UDID40);
                break;
            case 49:
                opt.set_sensitivity(&CMIAD41);
                opt.set_sensitivity(&CMIBD41);
                opt.set_sensitivity(&PREID41);
                opt.set_sensitivity(&PFEID41);
                opt.set_sensitivity(&UDID41);
                break;
            case 50:
                opt.set_sensitivity(&CMIAD42);
                opt.set_sensitivity(&CMIBD42);
                opt.set_sensitivity(&PREID42);
                opt.set_sensitivity(&PFEID42);
                opt.set_sensitivity(&UDID42);
                break;
            case 51:
                opt.set_sensitivity(&CMIAD43);
                opt.set_sensitivity(&CMIBD43);
                opt.set_sensitivity(&PREID43);
                opt.set_sensitivity(&PFEID43);
                opt.set_sensitivity(&UDID43);
                break;
            case 52:
                opt.set_sensitivity(&CMIAD50);
                opt.set_sensitivity(&CMIBD50);
                opt.set_sensitivity(&PREID50);
                opt.set_sensitivity(&PFEID50);
                opt.set_sensitivity(&UDID50);
                break;
            case 53:
                opt.set_sensitivity(&CMIAD51);
                opt.set_sensitivity(&CMIBD51);
                opt.set_sensitivity(&PREID51);
                opt.set_sensitivity(&PFEID51);
                opt.set_sensitivity(&UDID51);
                break;
            case 54:
                opt.set_sensitivity(&CMIAD52);
                opt.set_sensitivity(&CMIBD52);
                opt.set_sensitivity(&PREID52);
                opt.set_sensitivity(&PFEID52);
                opt.set_sensitivity(&UDID52);
                break;
            case 55:
                opt.set_sensitivity(&CMIAD53);
                opt.set_sensitivity(&CMIBD53);
                opt.set_sensitivity(&PREID53);
                opt.set_sensitivity(&PFEID53);
                opt.set_sensitivity(&UDID53);
                break;
            case 56:
                opt.set_sensitivity(&CMIAD60);
                opt.set_sensitivity(&CMIBD60);
                opt.set_sensitivity(&PREID60);
                opt.set_sensitivity(&PFEID60);
                opt.set_sensitivity(&UDID60);
                break;
            case 57:
                opt.set_sensitivity(&CMIAD61);
                opt.set_sensitivity(&CMIBD61);
                opt.set_sensitivity(&PREID61);
                opt.set_sensitivity(&PFEID61);
                opt.set_sensitivity(&UDID61);
                break;
            case 58:
                opt.set_sensitivity(&CMIAD62);
                opt.set_sensitivity(&CMIBD62);
                opt.set_sensitivity(&PREID62);
                opt.set_sensitivity(&PFEID62);
                opt.set_sensitivity(&UDID62);
                break;
            case 59:
                opt.set_sensitivity(&CMIAD63);
                opt.set_sensitivity(&CMIBD63);
                opt.set_sensitivity(&PREID63);
                opt.set_sensitivity(&PFEID63);
                opt.set_sensitivity(&UDID63);
                break;
            case 60:
                opt.set_sensitivity(&CMIAD70);
                opt.set_sensitivity(&CMIBD70);
                opt.set_sensitivity(&PREID70);
                opt.set_sensitivity(&PFEID70);
                opt.set_sensitivity(&UDID70);
                break;
            case 61:
                opt.set_sensitivity(&CMIAD71);
                opt.set_sensitivity(&CMIBD71);
                opt.set_sensitivity(&PREID71);
                opt.set_sensitivity(&PFEID71);
                opt.set_sensitivity(&UDID71);
                break;
            case 62:
                opt.set_sensitivity(&CMIAD72);
                opt.set_sensitivity(&CMIBD72);
                opt.set_sensitivity(&PREID72);
                opt.set_sensitivity(&PFEID72);
                opt.set_sensitivity(&UDID72);
                break;
            default:    // 63
                opt.set_sensitivity(&CMIAD73);
                opt.set_sensitivity(&CMIBD73);
                opt.set_sensitivity(&PREID73);
                opt.set_sensitivity(&PFEID73);
                opt.set_sensitivity(&UDID73);
                break;
        }
        opt.set_sensitivity(&mDMASELCDregChangeEvent[i >> 3]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ATU5_GLUE::WriteSLDCDxMethod, this, i),
                          sc_core::sc_gen_unique_name("WriteSLDCDxMethod"), &opt);
    }
    
    SC_METHOD(WriteSLDExMethod);
    dont_initialize();
    sensitive << INTATUECMIE00;
    sensitive << INTATUECMIE01;
    sensitive << INTATUECMIE02;
    sensitive << INTATUECMIE03;
    sensitive << INTATUECMIE10;
    sensitive << INTATUECMIE11;
    sensitive << INTATUECMIE12;
    sensitive << INTATUECMIE13;
    sensitive << INTATUECMIE20;
    sensitive << INTATUECMIE21;
    sensitive << INTATUECMIE22;
    sensitive << INTATUECMIE23;
    sensitive << INTATUECMIE30;
    sensitive << INTATUECMIE31;
    sensitive << INTATUECMIE32;
    sensitive << INTATUECMIE33;
    sensitive << INTATUECMIE40;
    sensitive << INTATUECMIE41;
    sensitive << INTATUECMIE42;
    sensitive << INTATUECMIE43;
    sensitive << INTATUECMIE50;
    sensitive << INTATUECMIE51;
    sensitive << INTATUECMIE52;
    sensitive << INTATUECMIE53;
    sensitive << INTATUECMIE60;
    sensitive << INTATUECMIE61;
    sensitive << INTATUECMIE62;
    sensitive << INTATUECMIE63;
    sensitive << INTATUECMIE70;
    sensitive << INTATUECMIE71;
    sensitive << INTATUECMIE72;
    sensitive << INTATUECMIE73;
    sensitive << INTATUECMIE80;
    sensitive << INTATUECMIE81;
    sensitive << INTATUECMIE82;
    sensitive << INTATUECMIE83;
    sensitive << mDMASELEregChangeEvent;
    
    for (unsigned int i = 0; i < emTQNum; i++) {
        for (unsigned int j = 0; j < emDFEFactorNum; j++) {
            sc_core::sc_spawn_options opt;
            opt.spawn_method();
            switch (j) {
                case 0:
                    opt.set_sensitivity(&CMIAD00);
                    opt.set_sensitivity(&CMIBD00);
                    opt.set_sensitivity(&PREID00);
                    opt.set_sensitivity(&PFEID00);
                    opt.set_sensitivity(&UDID00);
                    break;
                case 1:
                    opt.set_sensitivity(&CMIAD01);
                    opt.set_sensitivity(&CMIBD01);
                    opt.set_sensitivity(&PREID01);
                    opt.set_sensitivity(&PFEID01);
                    opt.set_sensitivity(&UDID01);
                    break;
                case 2:
                    opt.set_sensitivity(&CMIAD02);
                    opt.set_sensitivity(&CMIBD02);
                    opt.set_sensitivity(&PREID02);
                    opt.set_sensitivity(&PFEID02);
                    opt.set_sensitivity(&UDID02);
                    break;
                case 3:
                    opt.set_sensitivity(&CMIAD03);
                    opt.set_sensitivity(&CMIBD03);
                    opt.set_sensitivity(&PREID03);
                    opt.set_sensitivity(&PFEID03);
                    opt.set_sensitivity(&UDID03);
                    break;
                case 4:
                    opt.set_sensitivity(&CMIAD10);
                    opt.set_sensitivity(&CMIBD10);
                    opt.set_sensitivity(&PREID10);
                    opt.set_sensitivity(&PFEID10);
                    opt.set_sensitivity(&UDID10);
                    break;
                case 5:
                    opt.set_sensitivity(&CMIAD11);
                    opt.set_sensitivity(&CMIBD11);
                    opt.set_sensitivity(&PREID11);
                    opt.set_sensitivity(&PFEID11);
                    opt.set_sensitivity(&UDID11);
                    break;
                case 6:
                    opt.set_sensitivity(&IMIGC80);
                    opt.set_sensitivity(&IMIOC80);
                    break;
                case 7:
                    opt.set_sensitivity(&IMIGC81);
                    opt.set_sensitivity(&IMIOC81);
                    break;
                case 8:
                    opt.set_sensitivity(&IMIGC82);
                    opt.set_sensitivity(&IMIOC82);
                    break;
                case 9:
                    opt.set_sensitivity(&IMIGC83);
                    opt.set_sensitivity(&IMIOC83);
                    break;
                case 10:
                    opt.set_sensitivity(&IMIGC90);
                    opt.set_sensitivity(&IMIOC90);
                    break;
                default:    // 11
                    opt.set_sensitivity(&IMIGC91);
                    opt.set_sensitivity(&IMIOC91);
                    break;
            }
            opt.set_sensitivity(&mDFESELregChangeEvent[i]);
            opt.set_sensitivity(&mDFEENTQregChangeEvent[i]);
            opt.dont_initialize();
            sc_core::sc_spawn(sc_bind(&ATU5_GLUE::WriteDFxMethod, this, i * emDFEFactorNum + j),
                            sc_core::sc_gen_unique_name("WriteDFxMethod"), &opt);
        }
    }
    
    SC_METHOD(WriteDEG1TRGMethod);
    dont_initialize();
    sensitive << INTATUGCMIG0;
    sensitive << INTATUGCMIG1;
    sensitive << INTATUGCMIG2;
    sensitive << INTATUGCMIG3;
    sensitive << INTATUGCMIG4;
    sensitive << INTATUGCMIG5;
    sensitive << INTATUGCMIG6;
    sensitive << INTATUGCMIG7;
    sensitive << mDFESELD1TregChangeEvent;
    
    for (unsigned int i = 0; i < emDSTSNum; i++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        switch (i) {
            case 0:
                opt.set_sensitivity(&TOC00);
                opt.set_sensitivity(&TOD00A);
                opt.set_sensitivity(&TOD00B);
                opt.set_sensitivity(&TOE00);
                break;
            case 1:
                opt.set_sensitivity(&TOC01);
                opt.set_sensitivity(&TOD01A);
                opt.set_sensitivity(&TOD01B);
                opt.set_sensitivity(&TOE01);
                break;
            case 2:
                opt.set_sensitivity(&TOC02);
                opt.set_sensitivity(&TOD02A);
                opt.set_sensitivity(&TOD02B);
                opt.set_sensitivity(&TOE02);
                break;
            case 3:
                opt.set_sensitivity(&TOC03);
                opt.set_sensitivity(&TOD03A);
                opt.set_sensitivity(&TOD03B);
                opt.set_sensitivity(&TOE03);
                break;
            case 4:
                opt.set_sensitivity(&TOC10);
                opt.set_sensitivity(&TOD10A);
                opt.set_sensitivity(&TOD10B);
                opt.set_sensitivity(&TOE10);
                break;
            case 5:
                opt.set_sensitivity(&TOC11);
                opt.set_sensitivity(&TOD11A);
                opt.set_sensitivity(&TOD11B);
                opt.set_sensitivity(&TOE11);
                break;
            case 6:
                opt.set_sensitivity(&TOC12);
                opt.set_sensitivity(&TOD12A);
                opt.set_sensitivity(&TOD12B);
                opt.set_sensitivity(&TOE12);
                break;
            case 7:
                opt.set_sensitivity(&TOC13);
                opt.set_sensitivity(&TOD13A);
                opt.set_sensitivity(&TOD13B);
                opt.set_sensitivity(&TOE13);
                break;
            case 8:
                opt.set_sensitivity(&TOC20);
                opt.set_sensitivity(&TOD20A);
                opt.set_sensitivity(&TOD20B);
                opt.set_sensitivity(&TOE20);
                break;
            default:    // 9
                opt.set_sensitivity(&TOC21);
                opt.set_sensitivity(&TOD21A);
                opt.set_sensitivity(&TOD21B);
                opt.set_sensitivity(&TOE21);
                break;
        }
        opt.set_sensitivity(&mDSSELDSTSregChangeEvent);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ATU5_GLUE::WriteDSTSxMethod, this, i),
                          sc_core::sc_gen_unique_name("WriteDSTSxMethod"), &opt);
    }

    SC_METHOD(WriteCATSMethod);
    dont_initialize();
    sensitive << TOC22;
    sensitive << TOD22A;
    sensitive << TOD22B;
    sensitive << TOE22;
    sensitive << mCASELCATSregChangeEvent;
    
    for (unsigned int i = 1; i < emP5Num; i++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        switch (i) {// Note: case 0 is removed. It never be called because ATUP5SSEL[1:0] are reserved bit
            case 1:     // TGC1
                opt.set_sensitivity(&TOC01);
                opt.set_sensitivity(&TOD01A);
                opt.set_sensitivity(&TOD01B);
                opt.set_sensitivity(&TOE01);
                break;
            case 2:
                opt.set_sensitivity(&TOC02);
                opt.set_sensitivity(&TOD02A);
                opt.set_sensitivity(&TOD02B);
                opt.set_sensitivity(&TOE02);
                break;
            case 3:
                opt.set_sensitivity(&TOC03);
                opt.set_sensitivity(&TOD03A);
                opt.set_sensitivity(&TOD03B);
                opt.set_sensitivity(&TOE03);
                break;
            case 4:
                opt.set_sensitivity(&TOC10);
                opt.set_sensitivity(&TOD10A);
                opt.set_sensitivity(&TOD10B);
                opt.set_sensitivity(&TOE10);
                break;
            case 5:
                opt.set_sensitivity(&TOC11);
                opt.set_sensitivity(&TOD11A);
                opt.set_sensitivity(&TOD11B);
                opt.set_sensitivity(&TOE11);
                break;
            case 6:
                opt.set_sensitivity(&TOC12);
                opt.set_sensitivity(&TOD12A);
                opt.set_sensitivity(&TOD12B);
                opt.set_sensitivity(&TOE12);
                break;
            case 7:
                opt.set_sensitivity(&TOC13);
                opt.set_sensitivity(&TOD13A);
                opt.set_sensitivity(&TOD13B);
                opt.set_sensitivity(&TOE13);
                break;
            case 8:     // CKTSA
                opt.set_sensitivity(&TOC20);
                opt.set_sensitivity(&TOD20A);
                opt.set_sensitivity(&TOD20B);
                opt.set_sensitivity(&TOE20);
                break;
            case 9:     // CKTSB
                opt.set_sensitivity(&TOC21);
                opt.set_sensitivity(&TOD21A);
                opt.set_sensitivity(&TOD21B);
                opt.set_sensitivity(&TOE21);
                break;
            case 10:     // CRTSA
                opt.set_sensitivity(&TOC22);
                opt.set_sensitivity(&TOD22A);
                opt.set_sensitivity(&TOD22B);
                opt.set_sensitivity(&TOE22);
                break;
            case 11:     // CRTSB
                opt.set_sensitivity(&TOC23);
                opt.set_sensitivity(&TOD23A);
                opt.set_sensitivity(&TOD23B);
                opt.set_sensitivity(&TOE23);
                break;
            case 12:     // SSTSA
                opt.set_sensitivity(&TOC30);
                opt.set_sensitivity(&TOD30A);
                opt.set_sensitivity(&TOD30B);
                opt.set_sensitivity(&TOE30);
                break;
            default:     // SSTSB
                opt.set_sensitivity(&TOC31);
                opt.set_sensitivity(&TOD31A);
                opt.set_sensitivity(&TOD31B);
                opt.set_sensitivity(&TOE31);
                break;
        }
        opt.set_sensitivity(&mP5SSELregChangeEvent);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ATU5_GLUE::WriteP5xMethod, this, i),
                          sc_core::sc_gen_unique_name("WriteP5xMethod"), &opt);
    }
    
    SC_METHOD(WriteICIAxMethod);
    dont_initialize();
    sensitive << INTATUAICIA0;
    sensitive << INTATUAICIA1;
    sensitive << INTATUAICIA2;
    sensitive << INTATUAICIA3;
    sensitive << INTATUAICIA4;
    sensitive << INTATUAICIA5;
    sensitive << INTATUAICIA6;
    sensitive << INTATUAICIA7;
    
    SC_METHOD(WriteCIBxMethod);
    dont_initialize();
    sensitive << INTATUBCMIB0;
    sensitive << INTATUBCMIB1;
    sensitive << INTATUBCMIB6;
    sensitive << INTATUBCMIB10;
    sensitive << INTATUBCMIB11;
    sensitive << INTATUBCMIB12;
    sensitive << INTATUBCMIB6M;
    sensitive << INTATUBCMIB66M;
    sensitive << INTATUBICIB0;
    
    SC_METHOD(WriteCMIExMethod);
    dont_initialize();
    sensitive << INTATUECMIE00;
    sensitive << INTATUECMIE01;
    sensitive << INTATUECMIE02;
    sensitive << INTATUECMIE03;
    sensitive << INTATUECMIE10;
    sensitive << INTATUECMIE11;
    sensitive << INTATUECMIE12;
    sensitive << INTATUECMIE13;
    sensitive << INTATUECMIE20;
    sensitive << INTATUECMIE21;
    sensitive << INTATUECMIE22;
    sensitive << INTATUECMIE23;
    sensitive << INTATUECMIE30;
    sensitive << INTATUECMIE31;
    sensitive << INTATUECMIE32;
    sensitive << INTATUECMIE33;
    sensitive << INTATUECMIE40;
    sensitive << INTATUECMIE41;
    sensitive << INTATUECMIE42;
    sensitive << INTATUECMIE43;
    sensitive << INTATUECMIE50;
    sensitive << INTATUECMIE51;
    sensitive << INTATUECMIE52;
    sensitive << INTATUECMIE53;
    sensitive << INTATUECMIE60;
    sensitive << INTATUECMIE61;
    sensitive << INTATUECMIE62;
    sensitive << INTATUECMIE63;
    sensitive << INTATUECMIE70;
    sensitive << INTATUECMIE71;
    sensitive << INTATUECMIE72;
    sensitive << INTATUECMIE73;
    sensitive << INTATUECMIE80;
    sensitive << INTATUECMIE81;
    sensitive << INTATUECMIE82;
    sensitive << INTATUECMIE83;
    
    SC_METHOD(WriteICIFxMethod);
    dont_initialize();
    sensitive << INTATUFICIF0;
    sensitive << INTATUFICIF1;
    sensitive << INTATUFICIF2;
    sensitive << INTATUFICIF3;
    sensitive << INTATUFICIF4;
    sensitive << INTATUFICIF5;
    sensitive << INTATUFICIF6;
    sensitive << INTATUFICIF7;
    sensitive << INTATUFICIF8;
    sensitive << INTATUFICIF9;
    sensitive << INTATUFICIF10;
    sensitive << INTATUFICIF11;
    sensitive << INTATUFICIF12;
    sensitive << INTATUFICIF13;
    sensitive << INTATUFICIF14;
    sensitive << INTATUFICIF15;
    
    SC_METHOD(WriteCMIGxMethod);
    dont_initialize();
    sensitive << INTATUGCMIG0;
    sensitive << INTATUGCMIG1;
    sensitive << INTATUGCMIG2;
    sensitive << INTATUGCMIG3;
    sensitive << INTATUGCMIG4;
    sensitive << INTATUGCMIG5;
    sensitive << INTATUGCMIG6;
    sensitive << INTATUGCMIG7;
    sensitive << INTATUGCMIG8;
    sensitive << INTATUGCMIG9;
    
    SC_METHOD(WriteDMAREQxMethod);
    dont_initialize();
    sensitive << INTATUCTRLDMAREQ00;
    sensitive << INTATUCTRLDMAREQ01;
    sensitive << INTATUCTRLDMAREQ10;
    sensitive << INTATUCTRLDMAREQ11;
    
    SC_METHOD(HandleResetHardMethod);
    dont_initialize();
    sensitive << mResetHardEvent;
    
    SC_METHOD(HandleResetCmdMethod);
    dont_initialize();
    sensitive << mResetCmdEvent;

    SC_METHOD(CancelResetCmdMethod);
    dont_initialize();
    sensitive << mResetCmdCancelEvent;

}//}}}

///Destructor of ATU5_GLUE class
ATU5_GLUE::~ATU5_GLUE (void)
{//{{{
    delete mATU5_GLUE_Func;
}//}}}

///Assert reset by software
void ATU5_GLUE::AssertReset (const std::string reset_name, const double start_time, const double period)
{//{{{
    if (reset_name == "PRESETZ") {
        if ((!mIsResetHardActive) && (!mIsResetCmdReceive)) {
            mIsResetCmdReceive = true;
            re_printf("info","The model will be reset (PRESETZ) for %f ns after %f ns.\n", period, start_time);
            mResetCmdEvent.notify(start_time, SC_NS);
            mResetCmdPeriod = period;
        } else {
            re_printf("warning","The software reset of PRESETZ is called in the reset operation of the model. So it is ignored\n");
        }
    }
}//}}}

///Set clock value and clock unit
void ATU5_GLUE::SetCLKFreq (const std::string clock_name, const sc_dt::uint64 freq, const std::string unit)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, clock_name);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "PCLK") {
            mPCLKOrgFreq = freq;
            ConvertClockFreq (mPCLKFreq, mPCLKFreqUnit, freq, unit);
            if (mPCLKFreq > 0) {
                mPCLKPeriod = (sc_dt::uint64)(((1/(double)mPCLKFreq)*(double)mTimeResolutionValue) + 0.5);
            } else {
                mPCLKPeriod = 0;
            }
        }
    } else {
        re_printf("warning","Clock name (%s) is invalid\n", clock_name.c_str());
    }
}//}}}

///Get clock value
void ATU5_GLUE::GetCLKFreq (const std::string clock_name)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, clock_name);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "PCLK") {
            re_printf("info","PCLK frequency is %0.0f %s\n", (double)mPCLKOrgFreq, mPCLKFreqUnit.c_str());
        } else {
            re_printf("warning","Clock name (%s) is invalid\n", word_vector[0].c_str());
        }
    } else {
        re_printf("warning","Clock name (%s) is invalid\n", clock_name.c_str());
    }
}//}}}

///Force value to register
void ATU5_GLUE::ForceRegister (const std::string reg_name, const unsigned int reg_value)
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
            mATU5_GLUE_Func->RegisterHandler(cmd);
        } else {
            re_printf("warning","Register name (%s) is invalid\n", reg_name.c_str());
        }
    } else {
        re_printf("warning","Register name (%s) is invalid\n", reg_name.c_str());
    }
}//}}}

///Release forced value after forcing registers
void ATU5_GLUE::ReleaseRegister (const std::string reg_name)
{//{{{
    std::vector<std::string> word_vector;
    std::vector<std::string> cmd;
    if (reg_name != "") {
        SeparateString(word_vector, reg_name);
        if (word_vector.size() == 1) {
            cmd.push_back("reg");
            SeparateString(cmd, reg_name);
            cmd.push_back("release");
            mATU5_GLUE_Func->RegisterHandler(cmd);
        } else {
            re_printf("warning","Register name (%s) is invalid\n", reg_name.c_str());
        }
    } else {
        re_printf("warning","Register name (%s) is invalid\n", reg_name.c_str());
    }
}//}}}

///Write value to registers by software
void ATU5_GLUE::WriteRegister (const std::string reg_name, const unsigned int reg_value)
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
            mATU5_GLUE_Func->RegisterHandler(cmd);
        } else {
            re_printf("warning","Register name (%s) is invalid\n", reg_name.c_str());
        }
    } else {
        re_printf("warning","Register name (%s) is invalid\n", reg_name.c_str());
    }
}//}}}

///Read value of register by software
void ATU5_GLUE::ReadRegister (const std::string reg_name)
{//{{{
    std::vector<std::string> word_vector;
    std::vector<std::string> cmd;
    if (reg_name != "") {
        SeparateString(word_vector, reg_name);
        if (word_vector.size() == 1) {
            cmd.push_back("reg");
            SeparateString(cmd, reg_name);
            mATU5_GLUE_Func->RegisterHandler(cmd);
        } else {
            re_printf("warning","Register name (%s) is invalid\n", reg_name.c_str());
        }
    } else {
        re_printf("warning","Register name (%s) is invalid\n", reg_name.c_str());
    }
}//}}}

///List all registers name
void ATU5_GLUE::ListRegister (void)
{//{{{
    std::vector<std::string> cmd;
    cmd.push_back("reg");
    mATU5_GLUE_Func->RegisterHandler(cmd);
}//}}}

///Dump help message of all parameters or commands
void ATU5_GLUE::Help (const std::string type)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, type);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "parameters") {
            SC_REPORT_INFO(this->basename(),"ATU5_GLUE_MessageLevel (fatal|error|warning|info)          Set debug message level (Default: fatal|error).");
            SC_REPORT_INFO(this->basename(),"ATU5_GLUE_DumpRegisterRW (true/false)                      Enable/disable dumping access register (Default: false).");
        } else if (word_vector[0] == "commands") {
            SC_REPORT_INFO(this->basename(),"ATU5_GLUE_AssertReset (reset_name, start_time, period)     Assert and de-assert reset signal.");
            SC_REPORT_INFO(this->basename(),"ATU5_GLUE_SetCLKFreq (clock_name, freq, unit)              Set clock frequency value of model.");
            SC_REPORT_INFO(this->basename(),"ATU5_GLUE_GetCLKFreq (clock_name)                          Get clock frequency value of model.");
            SC_REPORT_INFO(this->basename(),"ATU5_GLUE_ForceRegister (reg_name, reg_value)              Force ATU5_GLUE’s register with setting value.");
            SC_REPORT_INFO(this->basename(),"ATU5_GLUE_ReleaseRegister (reg_name)                       Release ATU5_GLUE’s register from force value.");
            SC_REPORT_INFO(this->basename(),"ATU5_GLUE_WriteRegister (reg_name, reg_value)              Write a value to a ATU5_GLUE’s register.");
            SC_REPORT_INFO(this->basename(),"ATU5_GLUE_ReadRegister (reg_name)                          Read a value from a ATU5_GLUE’s register.");
            SC_REPORT_INFO(this->basename(),"ATU5_GLUE_ListRegister ()                                  Dump name of model's registers.");
        } else {
            re_printf("warning","The name (%s) of ATU5_GLUE_Help argument is wrong (commands or parameters)\n", type.c_str());
        }
    } else {
        re_printf("warning","The name (%s) of ATU5_GLUE_Help argument is wrong (commands or parameters)\n", type.c_str());
    }
}//}}}

///Set message level (fatal, error, warning, info)
void ATU5_GLUE::SetMessageLevel (const std::string msg_lv)
{//{{{
    std::vector<std::string> cmd;
    cmd.push_back("command");
    cmd.push_back("MessageLevel");
    SeparateString(cmd, msg_lv);
    std::string msg_return = handleCommand(cmd);
    if (msg_return != "") {
        SC_REPORT_INFO(this->basename(), msg_return.c_str());
    }

    //Set message level for Register IF class
    cmd.clear();
    cmd.push_back("reg");
    cmd.push_back("MessageLevel");
    SeparateString(cmd, msg_lv);
    mATU5_GLUE_Func->RegisterHandler(cmd);

    //Set message level for each register in Register IF class
    cmd.clear();
    cmd.push_back("reg");
    cmd.push_back("*");
    cmd.push_back("MessageLevel");
    SeparateString(cmd, msg_lv);
    mATU5_GLUE_Func->RegisterHandler(cmd);
}//}}}

///Enable/disable dumping message when users access registers
void ATU5_GLUE::DumpRegisterRW (const std::string is_enable)
{//{{{
    std::vector<std::string> cmd;
    cmd.push_back("reg");
    cmd.push_back("DumpRegisterRW");
    SeparateString(cmd, is_enable);
    mATU5_GLUE_Func->RegisterHandler(cmd);
}//}}}

///Check frequency value and frequency unit
void ATU5_GLUE::ConvertClockFreq (sc_dt::uint64 &freq_out, std::string &unit_out,
                             sc_dt::uint64 freq_in, std::string unit_in)
{//{{{
    if(unit_in == "GHz"){
        freq_out = freq_in * (sc_dt::uint64)(1.0e+9);
        unit_out = "GHz";
    } else if (unit_in == "MHz") {
        freq_out = freq_in * (sc_dt::uint64)(1.0e+6);
        unit_out = "MHz";
    } else if (unit_in == "KHz") {
        freq_out = freq_in * (sc_dt::uint64)(1.0e+3);
        unit_out = "KHz";
    } else if (unit_in == "Hz") {
        freq_out = freq_in;
        unit_out = "Hz";
    } else {// Frequency unit is wrong, frequency unit is set as Hz default.
        freq_out = freq_in;
        unit_out = "Hz";
        re_printf("warning","Frequency unit (%s) is wrong, frequency unit is set as unit Hz default.\n", unit_in.c_str());
    }
}//}}}

///Separate some words from a string to store a vector
void ATU5_GLUE::SeparateString (std::vector<std::string> &vtr, const std::string msg)
{//{{{
    std::istringstream cmd;
    cmd.str(msg);
    std::string temp = "a"; //Make temp to be not empty
    while (temp != "") {
        std::string command;
        cmd >> command;
        if (command != "") {
            vtr.push_back(command);
        }
        temp = command;
    }
}//}}}

///Get time resolution
void ATU5_GLUE::GetTimeResolution (sc_dt::uint64 &resolution_value, sc_time_unit &resolution_unit)
{//{{{
    if (sc_get_time_resolution() < sc_time(1, SC_PS)) {
        resolution_value = 1000000000000000LL;
        resolution_unit = SC_FS;
    } else if (sc_get_time_resolution() < sc_time(1, SC_NS)) {
        resolution_value = 1000000000000LL;
        resolution_unit = SC_PS;
    } else if (sc_get_time_resolution() < sc_time(1, SC_US)) {
        resolution_value = 1000000000;
        resolution_unit = SC_NS;
    } else if (sc_get_time_resolution() < sc_time(1, SC_MS)) {
        resolution_value = 1000000;
        resolution_unit = SC_US;
    } else if (sc_get_time_resolution() < sc_time(1, SC_SEC)) {
        resolution_value = 1000;
        resolution_unit = SC_MS;
    } else {
        resolution_value = 1;
        resolution_unit = SC_SEC;
    }
}//}}}

///Check reset status
bool ATU5_GLUE::GetResetStatus (const std::string reset_name)
{//{{{
    bool reset_status = false;
    if (mIsResetHardActive || mIsResetCmdActive) {
        reset_status = true;
    }
    return reset_status;
}//}}}

///Check clock period value
bool ATU5_GLUE::CheckClockPeriod (const std::string clock_name)
{//{{{
    bool clock_status = false;
    if (mPCLKPeriod > 0) {
        clock_status = true;
    } else {
        clock_status = false;
        re_printf("info","%s frequency is zero\n", clock_name.c_str());
    }
    return clock_status;
}//}}}

///Handle PCLK signal
void ATU5_GLUE::HandlePCLKSignalMethod (void)
{//{{{
    sc_dt::uint64 freq_value = PCLK.read();
    SetCLKFreq("PCLK", freq_value, "Hz");
}//}}}

///Handle PRESETZ signal
void ATU5_GLUE::HandlePRESETZSignalMethod (void)
{//{{{
    mResetCurVal = PRESETZ.read();
    if (mResetCurVal != mResetPreVal) {       //mResetPreVal is "true" at initial state
        mResetPreVal = mResetCurVal;
        // Reset signals are asynchronous
        mResetHardEvent.notify(SC_ZERO_TIME);
    }
}//}}}

///Process reset function when reset port is active
void ATU5_GLUE::HandleResetHardMethod (void)
{//{{{
    if (mResetCurVal == emResetActive) {
        mIsResetHardActive = true;
        re_printf("info","The reset port PRESETZ is asserted.\n");
        this->EnableReset("PRESETZ", mIsResetHardActive);
    } else {
        if (mIsResetHardActive) {
            mIsResetHardActive = false;
            re_printf("info","The reset port PRESETZ is de-asserted.\n");
            this->EnableReset("PRESETZ", mIsResetHardActive);
        }
    }
    //Cancel AssertReset command when reset port is active
    if (mIsResetCmdReceive) {
        mResetCmdEvent.cancel();
        mResetCmdCancelEvent.cancel();
        mIsResetCmdReceive = false;
        mIsResetCmdActive = false;
    }
}//}}}

///Process reset function when reset command is active
void ATU5_GLUE::HandleResetCmdMethod (void)
{//{{{
    std::string reset_name = "PRESETZ";
    mIsResetCmdActive = true;
    re_printf("info","The model is reset by AssertReset command of %s.\n", reset_name.c_str());
    this->EnableReset(reset_name, mIsResetCmdActive);
    mResetCmdCancelEvent.notify(mResetCmdPeriod, SC_NS);
}//}}}

///Cancel reset function when reset command is active
void ATU5_GLUE::CancelResetCmdMethod (void)
{//{{{
    std::string reset_name = "PRESETZ";
    mIsResetCmdActive = false;
    mIsResetCmdReceive = false;
    re_printf("info","Reset period of %s is over.\n", reset_name.c_str());
    this->EnableReset(reset_name, mIsResetCmdActive);
}//}}}

///Execute reset operation
void ATU5_GLUE::EnableReset (const std::string reset_name, const bool is_active)
{//{{{
    mATU5_GLUE_Func->Reset(is_active);
    if (is_active) {
        // Initialize internal signals & output
        // PSI5
        P5TGC0.write(false);
        P5TGC1.write(false);
        P5TGC2.write(false);
        P5TGC3.write(false);
        P5TGC4.write(false);
        P5TGC5.write(false);
        P5TGC6.write(false);
        P5TGC7.write(false);
        P5CKTSA.write(false);
        P5CKTSB.write(false);
        P5CRTSA.write(false);
        P5CRTSB.write(false);
        P5SSTSA.write(false);
        P5SSTSB.write(false);
        // DFE
        DEG1TRG.write(false);
        DFC800.write(false);
        DFC801.write(false);
        DFC802.write(false);
        DFC810.write(false);
        DFC811.write(false);
        DFC812.write(false);
        DFC820.write(false);
        DFC821.write(false);
        DFC822.write(false);
        DFC830.write(false);
        DFC831.write(false);
        DFC832.write(false);
        DFC900.write(false);
        DFC901.write(false);
        DFC902.write(false);
        DFC910.write(false);
        DFC911.write(false);
        DFC912.write(false);
        DFD000.write(false);
        DFD001.write(false);
        DFD002.write(false);
        DFD010.write(false);
        DFD011.write(false);
        DFD012.write(false);
        DFD020.write(false);
        DFD021.write(false);
        DFD022.write(false);
        DFD030.write(false);
        DFD031.write(false);
        DFD032.write(false);
        DFD100.write(false);
        DFD101.write(false);
        DFD102.write(false);
        DFD110.write(false);
        DFD111.write(false);
        DFD112.write(false);
        // ADC
        DSTS0.write(false);
        DSTS1.write(false);
        DSTS2.write(false);
        DSTS3.write(false);
        DSTS4.write(false);
        DSTS5.write(false);
        DSTS6.write(false);
        DSTS7.write(false);
        DSTS8.write(false);
        DSTS9.write(false);
        CATS.write(false);
        // PGL
        // TimerA
        ICIA0.write(false);
        ICIA1.write(false);
        ICIA2.write(false);
        ICIA3.write(false);
        ICIA4.write(false);
        ICIA5.write(false);
        ICIA6.write(false);
        ICIA7.write(false);
        // TimerB
        CMIB0.write(false);
        CMIB1.write(false);
        CMIB6.write(false);
        CMIB10.write(false);
        CMIB11.write(false);
        CMIB12.write(false);
        CMIB6M.write(false);
        CMIB66M.write(false);
        ICIB0.write(false);
        // TimerE
        CMIE00.write(false);
        CMIE01.write(false);
        CMIE02.write(false);
        CMIE03.write(false);
        CMIE10.write(false);
        CMIE11.write(false);
        CMIE12.write(false);
        CMIE13.write(false);
        CMIE20.write(false);
        CMIE21.write(false);
        CMIE22.write(false);
        CMIE23.write(false);
        CMIE30.write(false);
        CMIE31.write(false);
        CMIE32.write(false);
        CMIE33.write(false);
        CMIE40.write(false);
        CMIE41.write(false);
        CMIE42.write(false);
        CMIE43.write(false);
        CMIE50.write(false);
        CMIE51.write(false);
        CMIE52.write(false);
        CMIE53.write(false);
        CMIE60.write(false);
        CMIE61.write(false);
        CMIE62.write(false);
        CMIE63.write(false);
        CMIE70.write(false);
        CMIE71.write(false);
        CMIE72.write(false);
        CMIE73.write(false);
        CMIE80.write(false);
        CMIE81.write(false);
        CMIE82.write(false);
        CMIE83.write(false);
        // TimerF
        ICIF0.write(false);
        ICIF1.write(false);
        ICIF2.write(false);
        ICIF3.write(false);
        ICIF4.write(false);
        ICIF5.write(false);
        ICIF6.write(false);
        ICIF7.write(false);
        ICIF8.write(false);
        ICIF9.write(false);
        ICIF10.write(false);
        ICIF11.write(false);
        ICIF12.write(false);
        ICIF13.write(false);
        ICIF14.write(false);
        ICIF15.write(false);
        // TimerG
        CMIG0.write(false);
        CMIG1.write(false);
        CMIG2.write(false);
        CMIG3.write(false);
        CMIG4.write(false);
        CMIG5.write(false);
        CMIG6.write(false);
        CMIG7.write(false);
        CMIG8.write(false);
        CMIG9.write(false);
        DMAREQ00.write(false);
        DMAREQ01.write(false);
        DMAREQ10.write(false);
        DMAREQ11.write(false);
        // Overflow
        OVF.write(false);
        // TimerA - select
        SLIA0.write(false);
        SLIA1.write(false);
        SLIA2.write(false);
        SLIA3.write(false);
        // TimerB - select
        SLDB0.write(false);
        SLDB1.write(false);
        SLDB2.write(false);
        SLDB3.write(false);
        // TimerC - select
        SLIC00.write(false);
        SLIC01.write(false);
        SLIC02.write(false);
        SLIC03.write(false);
        SLIC10.write(false);
        SLIC11.write(false);
        SLIC12.write(false);
        SLIC13.write(false);
        SLIC20.write(false);
        SLIC21.write(false);
        SLIC22.write(false);
        SLIC23.write(false);
        SLIC30.write(false);
        SLIC31.write(false);
        SLIC32.write(false);
        SLIC33.write(false);
        SLIC40.write(false);
        SLIC41.write(false);
        SLIC42.write(false);
        SLIC43.write(false);
        SLIC50.write(false);
        SLIC51.write(false);
        SLIC52.write(false);
        SLIC53.write(false);
        SLIC60.write(false);
        SLIC61.write(false);
        SLIC62.write(false);
        SLIC63.write(false);
        SLIC70.write(false);
        SLIC71.write(false);
        SLIC72.write(false);
        SLIC73.write(false);
        SLIC80.write(false);
        SLIC81.write(false);
        SLIC82.write(false);
        SLIC83.write(false);
        SLIC90.write(false);
        SLIC91.write(false);
        SLIC92.write(false);
        SLIC93.write(false);
        SLIC100.write(false);
        SLIC101.write(false);
        SLIC102.write(false);
        SLIC103.write(false);
        // TimerD - select
        SLID00.write(false);
        SLID01.write(false);
        SLID02.write(false);
        SLID03.write(false);
        SLID10.write(false);
        SLID11.write(false);
        SLID12.write(false);
        SLID13.write(false);
        SLID20.write(false);
        SLID21.write(false);
        SLID22.write(false);
        SLID23.write(false);
        SLID30.write(false);
        SLID31.write(false);
        SLID32.write(false);
        SLID33.write(false);
        SLID40.write(false);
        SLID41.write(false);
        SLID42.write(false);
        SLID43.write(false);
        SLID50.write(false);
        SLID51.write(false);
        SLID52.write(false);
        SLID53.write(false);
        SLID60.write(false);
        SLID61.write(false);
        SLID62.write(false);
        SLID63.write(false);
        SLID70.write(false);
        SLID71.write(false);
        SLID72.write(false);
        SLID73.write(false);
        SLID80.write(false);
        SLID81.write(false);
        SLID82.write(false);
        SLID83.write(false);
        // TimerC-D - select
        SLDCD0.write(false);
        SLDCD1.write(false);
        SLDCD2.write(false);
        SLDCD3.write(false);
        SLDCD4.write(false);
        SLDCD5.write(false);
        SLDCD6.write(false);
        SLDCD7.write(false);
        SLDCD8.write(false);
        SLDCD9.write(false);
        SLDCD10.write(false);
        SLDCD11.write(false);
        SLDCD12.write(false);
        SLDCD13.write(false);
        SLDCD14.write(false);
        SLDCD15.write(false);
        SLDCD16.write(false);
        SLDCD17.write(false);
        SLDCD18.write(false);
        SLDCD19.write(false);
        SLDCD20.write(false);
        SLDCD21.write(false);
        SLDCD22.write(false);
        SLDCD23.write(false);
        SLDCD24.write(false);
        SLDCD25.write(false);
        SLDCD26.write(false);
        SLDCD27.write(false);
        SLDCD28.write(false);
        SLDCD29.write(false);
        SLDCD30.write(false);
        SLDCD31.write(false);
        SLDCD32.write(false);
        SLDCD33.write(false);
        SLDCD34.write(false);
        SLDCD35.write(false);
        SLDCD36.write(false);
        SLDCD37.write(false);
        SLDCD38.write(false);
        SLDCD39.write(false);
        SLDCD40.write(false);
        SLDCD41.write(false);
        SLDCD42.write(false);
        SLDCD43.write(false);
        SLDCD44.write(false);
        SLDCD45.write(false);
        SLDCD46.write(false);
        SLDCD47.write(false);
        SLDCD48.write(false);
        SLDCD49.write(false);
        SLDCD50.write(false);
        SLDCD51.write(false);
        SLDCD52.write(false);
        SLDCD53.write(false);
        SLDCD54.write(false);
        SLDCD55.write(false);
        SLDCD56.write(false);
        SLDCD57.write(false);
        SLDCD58.write(false);
        SLDCD59.write(false);
        SLDCD60.write(false);
        SLDCD61.write(false);
        SLDCD62.write(false);
        SLDCD63.write(false);
        // TimerE - select
        SLDE0.write(false);
        SLDE1.write(false);
        SLDE2.write(false);
        SLDE3.write(false);
        SLDE4.write(false);
        SLDE5.write(false);
        SLDE6.write(false);
        SLDE7.write(false);
        SLDE8.write(false);
    }
}//}}}

void ATU5_GLUE::WriteSLIAxMethod (void)
{//{{{
    bool value[8];
    value[0] = INTATUAICIA0.read();
    value[1] = INTATUAICIA1.read();
    value[2] = INTATUAICIA2.read();
    value[3] = INTATUAICIA3.read();
    value[4] = INTATUAICIA4.read();
    value[5] = INTATUAICIA5.read();
    value[6] = INTATUAICIA6.read();
    value[7] = INTATUAICIA7.read();
    
    unsigned int int_sel_a = mATU5_GLUE_Func->GetRegSelectSetting(emINTSELA);
    
    SLIA0.write(value[(int_sel_a) & 0x7]);
    SLIA1.write(value[(int_sel_a >> 4) & 0x7]);
    SLIA2.write(value[(int_sel_a >> 8) & 0x7]);
    SLIA3.write(value[(int_sel_a >> 12) & 0x7]);
}//}}}

void ATU5_GLUE::WriteSLDBxMethod (void)
{//{{{
    bool value[9];
    value[0] = INTATUBCMIB0.read();
    value[1] = INTATUBCMIB1.read();
    value[2] = INTATUBCMIB6.read();
    value[3] = INTATUBCMIB10.read();
    value[4] = INTATUBCMIB11.read();
    value[5] = INTATUBCMIB12.read();
    value[6] = INTATUBCMIB6M.read();
    value[7] = INTATUBCMIB66M.read();
    value[8] = INTATUBICIB0.read();
    
    unsigned int dma_sel_b = mATU5_GLUE_Func->GetRegSelectSetting(emDMASELB);
    unsigned int temp[4];
    temp[0] = (dma_sel_b) & 0xF;
    temp[1] = (dma_sel_b >> 4) & 0xF;
    temp[2] = (dma_sel_b >> 8) & 0xF;
    temp[3] = (dma_sel_b >> 12) & 0xF;
    
    bool invalid = false;
    for (unsigned int i = 0; i < 4; i++) {
        if (temp[i] > 8) {
            invalid = true;
        }
    }
    
    if (invalid) {
        re_printf("warning","The setting in ATUDMASELB register is prohibited.\n");
    } else {
        SLDB0.write(value[temp[0]]);
        SLDB1.write(value[temp[1]]);
        SLDB2.write(value[temp[2]]);
        SLDB3.write(value[temp[3]]);
    }
}//}}}

void ATU5_GLUE::WriteSLICxMethod (unsigned int id)
{//{{{
    switch (id) {
            case 0:
                SLIC00.write(IMIGC00.read() | IMIOC00.read());
                break;
            case 1:
                SLIC01.write(IMIGC01.read() | IMIOC01.read());
                break;
            case 2:
                SLIC02.write(IMIGC02.read() | IMIOC02.read());
                break;
            case 3:
                SLIC03.write(IMIGC03.read() | IMIOC03.read());
                break;
            case 4:
                SLIC10.write(IMIGC10.read() | IMIOC10.read());
                break;
            case 5:
                SLIC11.write(IMIGC11.read() | IMIOC11.read());
                break;
            case 6:
                SLIC12.write(IMIGC12.read() | IMIOC12.read());
                break;
            case 7:
                SLIC13.write(IMIGC13.read() | IMIOC13.read());
                break;
            case 8:
                SLIC20.write(IMIGC20.read() | IMIOC20.read());
                break;
            case 9:
                SLIC21.write(IMIGC21.read() | IMIOC21.read());
                break;
            case 10:
                SLIC22.write(IMIGC22.read() | IMIOC22.read());
                break;
            case 11:
                SLIC23.write(IMIGC23.read() | IMIOC23.read());
                break;
            case 12:
                SLIC30.write(IMIGC30.read() | IMIOC30.read());
                break;
            case 13:
                SLIC31.write(IMIGC31.read() | IMIOC31.read());
                break;
            case 14:
                SLIC32.write(IMIGC32.read() | IMIOC32.read());
                break;
            case 15:
                SLIC33.write(IMIGC33.read() | IMIOC33.read());
                break;
            case 16:
                SLIC40.write(IMIGC40.read() | IMIOC40.read());
                break;
            case 17:
                SLIC41.write(IMIGC41.read() | IMIOC41.read());
                break;
            case 18:
                SLIC42.write(IMIGC42.read() | IMIOC42.read());
                break;
            case 19:
                SLIC43.write(IMIGC43.read() | IMIOC43.read());
                break;
            case 20:
                SLIC50.write(IMIGC50.read() | IMIOC50.read());
                break;
            case 21:
                SLIC51.write(IMIGC51.read() | IMIOC51.read());
                break;
            case 22:
                SLIC52.write(IMIGC52.read() | IMIOC52.read());
                break;
            case 23:
                SLIC53.write(IMIGC53.read() | IMIOC53.read());
                break;
            case 24:
                SLIC60.write(IMIGC60.read() | IMIOC60.read());
                break;
            case 25:
                SLIC61.write(IMIGC61.read() | IMIOC61.read());
                break;
            case 26:
                SLIC62.write(IMIGC62.read() | IMIOC62.read());
                break;
            case 27:
                SLIC63.write(IMIGC63.read() | IMIOC63.read());
                break;
            case 28:
                SLIC70.write(IMIGC70.read() | IMIOC70.read());
                break;
            case 29:
                SLIC71.write(IMIGC71.read() | IMIOC71.read());
                break;
            case 30:
                SLIC72.write(IMIGC72.read() | IMIOC72.read());
                break;
            case 31:
                SLIC73.write(IMIGC73.read() | IMIOC73.read());
                break;
            case 32:
                SLIC80.write(IMIGC80.read() | IMIOC80.read());
                break;
            case 33:
                SLIC81.write(IMIGC81.read() | IMIOC81.read());
                break;
            case 34:
                SLIC82.write(IMIGC82.read() | IMIOC82.read());
                break;
            case 35:
                SLIC83.write(IMIGC83.read() | IMIOC83.read());
                break;
            case 36:
                SLIC90.write(IMIGC90.read() | IMIOC90.read());
                break;
            case 37:
                SLIC91.write(IMIGC91.read() | IMIOC91.read());
                break;
            case 38:
                SLIC92.write(IMIGC92.read() | IMIOC92.read());
                break;
            case 39:
                SLIC93.write(IMIGC93.read() | IMIOC93.read());
                break;
            case 40:
                SLIC100.write(IMIGC100.read() | IMIOC100.read());
                break;
            case 41:
                SLIC101.write(IMIGC101.read() | IMIOC101.read());
                break;
            case 42:
                SLIC102.write(IMIGC102.read() | IMIOC102.read());
                break;
            default: //case 43:
                SLIC103.write(IMIGC103.read() | IMIOC103.read());
                break;
        }
}//}}}

void ATU5_GLUE::WriteSLIDxMethod (unsigned int id)
{//{{{
    bool value[6];
    switch (id) {
        case 0:
            value[0] = CMIAD00.read();
            value[1] = CMIBD00.read();
            value[2] = PREID00.read();
            value[3] = PFEID00.read();
            value[4] = UDID00.read();
            value[5] = CMIAD00.read() | CMIBD00.read() | PREID00.read() | PFEID00.read() | UDID00.read();
            break;
        case 1:
            value[0] = CMIAD01.read();
            value[1] = CMIBD01.read();
            value[2] = PREID01.read();
            value[3] = PFEID01.read();
            value[4] = UDID01.read();
            value[5] = CMIAD01.read() | CMIBD01.read() | PREID01.read() | PFEID01.read() | UDID01.read();
            break;
        case 2:
            value[0] = CMIAD02.read();
            value[1] = CMIBD02.read();
            value[2] = PREID02.read();
            value[3] = PFEID02.read();
            value[4] = UDID02.read();
            value[5] = CMIAD02.read() | CMIBD02.read() | PREID02.read() | PFEID02.read() | UDID02.read();
            break;
        case 3:
            value[0] = CMIAD03.read();
            value[1] = CMIBD03.read();
            value[2] = PREID03.read();
            value[3] = PFEID03.read();
            value[4] = UDID03.read();
            value[5] = CMIAD03.read() | CMIBD03.read() | PREID03.read() | PFEID03.read() | UDID03.read();
            break;
        case 4:
            value[0] = CMIAD10.read();
            value[1] = CMIBD10.read();
            value[2] = PREID10.read();
            value[3] = PFEID10.read();
            value[4] = UDID10.read();
            value[5] = CMIAD10.read() | CMIBD10.read() | PREID10.read() | PFEID10.read() | UDID10.read();
            break;
        case 5:
            value[0] = CMIAD11.read();
            value[1] = CMIBD11.read();
            value[2] = PREID11.read();
            value[3] = PFEID11.read();
            value[4] = UDID11.read();
            value[5] = CMIAD11.read() | CMIBD11.read() | PREID11.read() | PFEID11.read() | UDID11.read();
            break;
        case 6:
            value[0] = CMIAD12.read();
            value[1] = CMIBD12.read();
            value[2] = PREID12.read();
            value[3] = PFEID12.read();
            value[4] = UDID12.read();
            value[5] = CMIAD12.read() | CMIBD12.read() | PREID12.read() | PFEID12.read() | UDID12.read();
            break;
        case 7:
            value[0] = CMIAD13.read();
            value[1] = CMIBD13.read();
            value[2] = PREID13.read();
            value[3] = PFEID13.read();
            value[4] = UDID13.read();
            value[5] = CMIAD13.read() | CMIBD13.read() | PREID13.read() | PFEID13.read() | UDID13.read();
            break;
        case 8:
            value[0] = CMIAD20.read();
            value[1] = CMIBD20.read();
            value[2] = PREID20.read();
            value[3] = PFEID20.read();
            value[4] = UDID20.read();
            value[5] = CMIAD20.read() | CMIBD20.read() | PREID20.read() | PFEID20.read() | UDID20.read();
            break;
        case 9:
            value[0] = CMIAD21.read();
            value[1] = CMIBD21.read();
            value[2] = PREID21.read();
            value[3] = PFEID21.read();
            value[4] = UDID21.read();
            value[5] = CMIAD21.read() | CMIBD21.read() | PREID21.read() | PFEID21.read() | UDID21.read();
            break;
        case 10:
            value[0] = CMIAD22.read();
            value[1] = CMIBD22.read();
            value[2] = PREID22.read();
            value[3] = PFEID22.read();
            value[4] = UDID22.read();
            value[5] = CMIAD22.read() | CMIBD22.read() | PREID22.read() | PFEID22.read() | UDID22.read();
            break;
        case 11:
            value[0] = CMIAD23.read();
            value[1] = CMIBD23.read();
            value[2] = PREID23.read();
            value[3] = PFEID23.read();
            value[4] = UDID23.read();
            value[5] = CMIAD23.read() | CMIBD23.read() | PREID23.read() | PFEID23.read() | UDID23.read();
            break;
        case 12:
            value[0] = CMIAD30.read();
            value[1] = CMIBD30.read();
            value[2] = PREID30.read();
            value[3] = PFEID30.read();
            value[4] = UDID30.read();
            value[5] = CMIAD30.read() | CMIBD30.read() | PREID30.read() | PFEID30.read() | UDID30.read();
            break;
        case 13:
            value[0] = CMIAD31.read();
            value[1] = CMIBD31.read();
            value[2] = PREID31.read();
            value[3] = PFEID31.read();
            value[4] = UDID31.read();
            value[5] = CMIAD31.read() | CMIBD31.read() | PREID31.read() | PFEID31.read() | UDID31.read();
            break;
        case 14:
            value[0] = CMIAD32.read();
            value[1] = CMIBD32.read();
            value[2] = PREID32.read();
            value[3] = PFEID32.read();
            value[4] = UDID32.read();
            value[5] = CMIAD32.read() | CMIBD32.read() | PREID32.read() | PFEID32.read() | UDID32.read();
            break;
        case 15:
            value[0] = CMIAD33.read();
            value[1] = CMIBD33.read();
            value[2] = PREID33.read();
            value[3] = PFEID33.read();
            value[4] = UDID33.read();
            value[5] = CMIAD33.read() | CMIBD33.read() | PREID33.read() | PFEID33.read() | UDID33.read();
            break;
        case 16:
            value[0] = CMIAD40.read();
            value[1] = CMIBD40.read();
            value[2] = PREID40.read();
            value[3] = PFEID40.read();
            value[4] = UDID40.read();
            value[5] = CMIAD40.read() | CMIBD40.read() | PREID40.read() | PFEID40.read() | UDID40.read();
            break;
        case 17:
            value[0] = CMIAD41.read();
            value[1] = CMIBD41.read();
            value[2] = PREID41.read();
            value[3] = PFEID41.read();
            value[4] = UDID41.read();
            value[5] = CMIAD41.read() | CMIBD41.read() | PREID41.read() | PFEID41.read() | UDID41.read();
            break;
        case 18:
            value[0] = CMIAD42.read();
            value[1] = CMIBD42.read();
            value[2] = PREID42.read();
            value[3] = PFEID42.read();
            value[4] = UDID42.read();
            value[5] = CMIAD42.read() | CMIBD42.read() | PREID42.read() | PFEID42.read() | UDID42.read();
            break;
        case 19:
            value[0] = CMIAD43.read();
            value[1] = CMIBD43.read();
            value[2] = PREID43.read();
            value[3] = PFEID43.read();
            value[4] = UDID43.read();
            value[5] = CMIAD43.read() | CMIBD43.read() | PREID43.read() | PFEID43.read() | UDID43.read();
            break;
        case 20:
            value[0] = CMIAD50.read();
            value[1] = CMIBD50.read();
            value[2] = PREID50.read();
            value[3] = PFEID50.read();
            value[4] = UDID50.read();
            value[5] = CMIAD50.read() | CMIBD50.read() | PREID50.read() | PFEID50.read() | UDID50.read();
            break;
        case 21:
            value[0] = CMIAD51.read();
            value[1] = CMIBD51.read();
            value[2] = PREID51.read();
            value[3] = PFEID51.read();
            value[4] = UDID51.read();
            value[5] = CMIAD51.read() | CMIBD51.read() | PREID51.read() | PFEID51.read() | UDID51.read();
            break;
        case 22:
            value[0] = CMIAD52.read();
            value[1] = CMIBD52.read();
            value[2] = PREID52.read();
            value[3] = PFEID52.read();
            value[4] = UDID52.read();
            value[5] = CMIAD52.read() | CMIBD52.read() | PREID52.read() | PFEID52.read() | UDID52.read();
            break;
        case 23:
            value[0] = CMIAD53.read();
            value[1] = CMIBD53.read();
            value[2] = PREID53.read();
            value[3] = PFEID53.read();
            value[4] = UDID53.read();
            value[5] = CMIAD53.read() | CMIBD53.read() | PREID53.read() | PFEID53.read() | UDID53.read();
            break;
        case 24:
            value[0] = CMIAD60.read();
            value[1] = CMIBD60.read();
            value[2] = PREID60.read();
            value[3] = PFEID60.read();
            value[4] = UDID60.read();
            value[5] = CMIAD60.read() | CMIBD60.read() | PREID60.read() | PFEID60.read() | UDID60.read();
            break;
        case 25:
            value[0] = CMIAD61.read();
            value[1] = CMIBD61.read();
            value[2] = PREID61.read();
            value[3] = PFEID61.read();
            value[4] = UDID61.read();
            value[5] = CMIAD61.read() | CMIBD61.read() | PREID61.read() | PFEID61.read() | UDID61.read();
            break;
        case 26:
            value[0] = CMIAD62.read();
            value[1] = CMIBD62.read();
            value[2] = PREID62.read();
            value[3] = PFEID62.read();
            value[4] = UDID62.read();
            value[5] = CMIAD62.read() | CMIBD62.read() | PREID62.read() | PFEID62.read() | UDID62.read();
            break;
        case 27:
            value[0] = CMIAD63.read();
            value[1] = CMIBD63.read();
            value[2] = PREID63.read();
            value[3] = PFEID63.read();
            value[4] = UDID63.read();
            value[5] = CMIAD63.read() | CMIBD63.read() | PREID63.read() | PFEID63.read() | UDID63.read();
            break;
        case 28:
            value[0] = CMIAD70.read();
            value[1] = CMIBD70.read();
            value[2] = PREID70.read();
            value[3] = PFEID70.read();
            value[4] = UDID70.read();
            value[5] = CMIAD70.read() | CMIBD70.read() | PREID70.read() | PFEID70.read() | UDID70.read();
            break;
        case 29:
            value[0] = CMIAD71.read();
            value[1] = CMIBD71.read();
            value[2] = PREID71.read();
            value[3] = PFEID71.read();
            value[4] = UDID71.read();
            value[5] = CMIAD71.read() | CMIBD71.read() | PREID71.read() | PFEID71.read() | UDID71.read();
            break;
        case 30:
            value[0] = CMIAD72.read();
            value[1] = CMIBD72.read();
            value[2] = PREID72.read();
            value[3] = PFEID72.read();
            value[4] = UDID72.read();
            value[5] = CMIAD72.read() | CMIBD72.read() | PREID72.read() | PFEID72.read() | UDID72.read();
            break;
        case 31:
            value[0] = CMIAD73.read();
            value[1] = CMIBD73.read();
            value[2] = PREID73.read();
            value[3] = PFEID73.read();
            value[4] = UDID73.read();
            value[5] = CMIAD73.read() | CMIBD73.read() | PREID73.read() | PFEID73.read() | UDID73.read();
            break;
        case 32:
            value[0] = CMIAD80.read();
            value[1] = CMIBD80.read();
            value[2] = PREID80.read();
            value[3] = PFEID80.read();
            value[4] = UDID80.read();
            value[5] = CMIAD80.read() | CMIBD80.read() | PREID80.read() | PFEID80.read() | UDID80.read();
            break;
        case 33:
            value[0] = CMIAD81.read();
            value[1] = CMIBD81.read();
            value[2] = PREID81.read();
            value[3] = PFEID81.read();
            value[4] = UDID81.read();
            value[5] = CMIAD81.read() | CMIBD81.read() | PREID81.read() | PFEID81.read() | UDID81.read();
            break;
        case 34:
            value[0] = CMIAD82.read();
            value[1] = CMIBD82.read();
            value[2] = PREID82.read();
            value[3] = PFEID82.read();
            value[4] = UDID82.read();
            value[5] = CMIAD82.read() | CMIBD82.read() | PREID82.read() | PFEID82.read() | UDID82.read();
            break;
        default:    // 35
            value[0] = CMIAD83.read();
            value[1] = CMIBD83.read();
            value[2] = PREID83.read();
            value[3] = PFEID83.read();
            value[4] = UDID83.read();
            value[5] = CMIAD83.read() | CMIBD83.read() | PREID83.read() | PFEID83.read() | UDID83.read();
            break;
    }
    
    unsigned int int_sel_d = mATU5_GLUE_Func->GetRegSelectSetting(emINTSELD0 + (id >> 3));
    unsigned bit_val = (int_sel_d >> ((id % 8) * 4)) & 0x7;
    
    bool invalid = false;
    if (bit_val > 5) {
        invalid = true;
    }

    if (invalid) {
        re_printf("warning","The setting in ATUINTSELD%d.ATU_INTSEL_D%d%d bit is prohibited.\n", id >> 3, (id >> 2) & 0x1, id & 0x3);
    } else {
        switch (id) {
            case 0:
                SLID00.write(value[bit_val]);
                break;
            case 1:
                SLID01.write(value[bit_val]);
                break;
            case 2:
                SLID02.write(value[bit_val]);
                break;
            case 3:
                SLID03.write(value[bit_val]);
                break;
            case 4:
                SLID10.write(value[bit_val]);
                break;
            case 5:
                SLID11.write(value[bit_val]);
                break;
            case 6:
                SLID12.write(value[bit_val]);
                break;
            case 7:
                SLID13.write(value[bit_val]);
                break;
            case 8:
                SLID20.write(value[bit_val]);
                break;
            case 9:
                SLID21.write(value[bit_val]);
                break;
            case 10:
                SLID22.write(value[bit_val]);
                break;
            case 11:
                SLID23.write(value[bit_val]);
                break;
            case 12:
                SLID30.write(value[bit_val]);
                break;
            case 13:
                SLID31.write(value[bit_val]);
                break;
            case 14:
                SLID32.write(value[bit_val]);
                break;
            case 15:
                SLID33.write(value[bit_val]);
                break;
            case 16:
                SLID40.write(value[bit_val]);
                break;
            case 17:
                SLID41.write(value[bit_val]);
                break;
            case 18:
                SLID42.write(value[bit_val]);
                break;
            case 19:
                SLID43.write(value[bit_val]);
                break;
            case 20:
                SLID50.write(value[bit_val]);
                break;
            case 21:
                SLID51.write(value[bit_val]);
                break;
            case 22:
                SLID52.write(value[bit_val]);
                break;
            case 23:
                SLID53.write(value[bit_val]);
                break;
            case 24:
                SLID60.write(value[bit_val]);
                break;
            case 25:
                SLID61.write(value[bit_val]);
                break;
            case 26:
                SLID62.write(value[bit_val]);
                break;
            case 27:
                SLID63.write(value[bit_val]);
                break;
            case 28:
                SLID70.write(value[bit_val]);
                break;
            case 29:
                SLID71.write(value[bit_val]);
                break;
            case 30:
                SLID72.write(value[bit_val]);
                break;
            case 31:
                SLID73.write(value[bit_val]);
                break;
            case 32:
                SLID80.write(value[bit_val]);
                break;
            case 33:
                SLID81.write(value[bit_val]);
                break;
            case 34:
                SLID82.write(value[bit_val]);
                break;
            default:    // 35
                SLID83.write(value[bit_val]);
                break;
        }
    }
}//}}}

void ATU5_GLUE::WriteSLDCDxMethod (unsigned int id)
{//{{{
    bool value[7];
    switch (id) {
        case 0:
            value[0] = IMIGC00.read();
            value[1] = IMIOC00.read();
            value[2] = CMIAD80.read();
            value[3] = CMIBD80.read();
            value[4] = PREID80.read();
            value[5] = PFEID80.read();
            value[6] = UDID80.read();
            break;
        case 1:
            value[0] = IMIGC01.read();
            value[1] = IMIOC01.read();
            value[2] = CMIAD81.read();
            value[3] = CMIBD81.read();
            value[4] = PREID81.read();
            value[5] = PFEID81.read();
            value[6] = UDID81.read();
            break;
        case 2:
            value[0] = IMIGC02.read();
            value[1] = IMIOC02.read();
            value[2] = CMIAD82.read();
            value[3] = CMIBD82.read();
            value[4] = PREID82.read();
            value[5] = PFEID82.read();
            value[6] = UDID82.read();
            break;
        case 3:
            value[0] = IMIGC03.read();
            value[1] = IMIOC03.read();
            value[2] = CMIAD83.read();
            value[3] = CMIBD83.read();
            value[4] = PREID83.read();
            value[5] = PFEID83.read();
            value[6] = UDID83.read();
            break;
        case 4:
            value[0] = IMIGC10.read();
            value[1] = IMIOC10.read();
            break;
        case 5:
            value[0] = IMIGC11.read();
            value[1] = IMIOC11.read();
            break;
        case 6:
            value[0] = IMIGC12.read();
            value[1] = IMIOC12.read();
            break;
        case 7:
            value[0] = IMIGC13.read();
            value[1] = IMIOC13.read();
            break;
        case 8:
            value[0] = IMIGC20.read();
            value[1] = IMIOC20.read();
            break;
        case 9:
            value[0] = IMIGC21.read();
            value[1] = IMIOC21.read();
            break;
        case 10:
            value[0] = IMIGC22.read();
            value[1] = IMIOC22.read();
            break;
        case 11:
            value[0] = IMIGC23.read();
            value[1] = IMIOC23.read();
            break;
        case 12:
            value[0] = IMIGC30.read();
            value[1] = IMIOC30.read();
            break;
        case 13:
            value[0] = IMIGC31.read();
            value[1] = IMIOC31.read();
            break;
        case 14:
            value[0] = IMIGC32.read();
            value[1] = IMIOC32.read();
            break;
        case 15:
            value[0] = IMIGC33.read();
            value[1] = IMIOC33.read();
            break;
        case 16:
            value[0] = IMIGC40.read();
            value[1] = IMIOC40.read();
            break;
        case 17:
            value[0] = IMIGC41.read();
            value[1] = IMIOC41.read();
            break;
        case 18:
            value[0] = IMIGC42.read();
            value[1] = IMIOC42.read();
            break;
        case 19:
            value[0] = IMIGC43.read();
            value[1] = IMIOC43.read();
            break;
        case 20:
            value[0] = IMIGC50.read();
            value[1] = IMIOC50.read();
            break;
        case 21:
            value[0] = IMIGC51.read();
            value[1] = IMIOC51.read();
            break;
        case 22:
            value[0] = IMIGC52.read();
            value[1] = IMIOC52.read();
            break;
        case 23:
            value[0] = IMIGC53.read();
            value[1] = IMIOC53.read();
            break;
        case 24:
            value[0] = IMIGC60.read();
            value[1] = IMIOC60.read();
            break;
        case 25:
            value[0] = IMIGC61.read();
            value[1] = IMIOC61.read();
            break;
        case 26:
            value[0] = IMIGC62.read();
            value[1] = IMIOC62.read();
            break;
        case 27:
            value[0] = IMIGC63.read();
            value[1] = IMIOC63.read();
            break;
        case 28:
            value[0] = IMIGC70.read();
            value[1] = IMIOC70.read();
            break;
        case 29:
            value[0] = IMIGC71.read();
            value[1] = IMIOC71.read();
            break;
        case 30:
            value[0] = IMIGC72.read();
            value[1] = IMIOC72.read();
            break;
        case 31:
            value[0] = IMIGC73.read();
            value[1] = IMIOC73.read();
            break;
        case 32:
            value[0] = IMIGC80.read();
            value[1] = IMIOC80.read();
            value[2] = CMIAD00.read();
            value[3] = CMIBD00.read();
            value[4] = PREID00.read();
            value[5] = PFEID00.read();
            value[6] = UDID00.read();
            break;
        case 33:
            value[0] = IMIGC81.read();
            value[1] = IMIOC81.read();
            value[2] = CMIAD01.read();
            value[3] = CMIBD01.read();
            value[4] = PREID01.read();
            value[5] = PFEID01.read();
            value[6] = UDID01.read();
            break;
        case 34:
            value[0] = IMIGC82.read();
            value[1] = IMIOC82.read();
            value[2] = CMIAD02.read();
            value[3] = CMIBD02.read();
            value[4] = PREID02.read();
            value[5] = PFEID02.read();
            value[6] = UDID02.read();
            break;
        case 35:
            value[0] = IMIGC83.read();
            value[1] = IMIOC83.read();
            value[2] = CMIAD03.read();
            value[3] = CMIBD03.read();
            value[4] = PREID03.read();
            value[5] = PFEID03.read();
            value[6] = UDID03.read();
            break;
        case 36:
            value[0] = IMIGC90.read();
            value[1] = IMIOC90.read();
            value[2] = CMIAD10.read();
            value[3] = CMIBD10.read();
            value[4] = PREID10.read();
            value[5] = PFEID10.read();
            value[6] = UDID10.read();
            break;
        case 37:
            value[0] = IMIGC91.read();
            value[1] = IMIOC91.read();
            value[2] = CMIAD11.read();
            value[3] = CMIBD11.read();
            value[4] = PREID11.read();
            value[5] = PFEID11.read();
            value[6] = UDID11.read();
            break;
        case 38:
            value[0] = IMIGC92.read();
            value[1] = IMIOC92.read();
            value[2] = CMIAD12.read();
            value[3] = CMIBD12.read();
            value[4] = PREID12.read();
            value[5] = PFEID12.read();
            value[6] = UDID12.read();
            break;
        case 39:
            value[0] = IMIGC93.read();
            value[1] = IMIOC93.read();
            value[2] = CMIAD13.read();
            value[3] = CMIBD13.read();
            value[4] = PREID13.read();
            value[5] = PFEID13.read();
            value[6] = UDID13.read();
            break;
        case 40:
            value[0] = IMIGC100.read();
            value[1] = IMIOC100.read();
            value[2] = CMIAD20.read();
            value[3] = CMIBD20.read();
            value[4] = PREID20.read();
            value[5] = PFEID20.read();
            value[6] = UDID20.read();
            break;
        case 41:
            value[0] = IMIGC101.read();
            value[1] = IMIOC101.read();
            value[2] = CMIAD21.read();
            value[3] = CMIBD21.read();
            value[4] = PREID21.read();
            value[5] = PFEID21.read();
            value[6] = UDID21.read();
            break;
        case 42:
            value[0] = IMIGC102.read();
            value[1] = IMIOC102.read();
            value[2] = CMIAD22.read();
            value[3] = CMIBD22.read();
            value[4] = PREID22.read();
            value[5] = PFEID22.read();
            value[6] = UDID22.read();
            break;
        case 43:
            value[0] = IMIGC103.read();
            value[1] = IMIOC103.read();
            value[2] = CMIAD23.read();
            value[3] = CMIBD23.read();
            value[4] = PREID23.read();
            value[5] = PFEID23.read();
            value[6] = UDID23.read();
            break;
        case 44:
            value[2] = CMIAD30.read();
            value[3] = CMIBD30.read();
            value[4] = PREID30.read();
            value[5] = PFEID30.read();
            value[6] = UDID30.read();
            break;
        case 45:
            value[2] = CMIAD31.read();
            value[3] = CMIBD31.read();
            value[4] = PREID31.read();
            value[5] = PFEID31.read();
            value[6] = UDID31.read();
            break;
        case 46:
            value[2] = CMIAD32.read();
            value[3] = CMIBD32.read();
            value[4] = PREID32.read();
            value[5] = PFEID32.read();
            value[6] = UDID32.read();
            break;
        case 47:
            value[2] = CMIAD33.read();
            value[3] = CMIBD33.read();
            value[4] = PREID33.read();
            value[5] = PFEID33.read();
            value[6] = UDID33.read();
            break;
        case 48:
            value[2] = CMIAD40.read();
            value[3] = CMIBD40.read();
            value[4] = PREID40.read();
            value[5] = PFEID40.read();
            value[6] = UDID40.read();
            break;
        case 49:
            value[2] = CMIAD41.read();
            value[3] = CMIBD41.read();
            value[4] = PREID41.read();
            value[5] = PFEID41.read();
            value[6] = UDID41.read();
            break;
        case 50:
            value[2] = CMIAD42.read();
            value[3] = CMIBD42.read();
            value[4] = PREID42.read();
            value[5] = PFEID42.read();
            value[6] = UDID42.read();
            break;
        case 51:
            value[2] = CMIAD43.read();
            value[3] = CMIBD43.read();
            value[4] = PREID43.read();
            value[5] = PFEID43.read();
            value[6] = UDID43.read();
            break;
        case 52:
            value[2] = CMIAD50.read();
            value[3] = CMIBD50.read();
            value[4] = PREID50.read();
            value[5] = PFEID50.read();
            value[6] = UDID50.read();
            break;
        case 53:
            value[2] = CMIAD51.read();
            value[3] = CMIBD51.read();
            value[4] = PREID51.read();
            value[5] = PFEID51.read();
            value[6] = UDID51.read();
            break;
        case 54:
            value[2] = CMIAD52.read();
            value[3] = CMIBD52.read();
            value[4] = PREID52.read();
            value[5] = PFEID52.read();
            value[6] = UDID52.read();
            break;
        case 55:
            value[2] = CMIAD53.read();
            value[3] = CMIBD53.read();
            value[4] = PREID53.read();
            value[5] = PFEID53.read();
            value[6] = UDID53.read();
            break;
        case 56:
            value[2] = CMIAD60.read();
            value[3] = CMIBD60.read();
            value[4] = PREID60.read();
            value[5] = PFEID60.read();
            value[6] = UDID60.read();
            break;
        case 57:
            value[2] = CMIAD61.read();
            value[3] = CMIBD61.read();
            value[4] = PREID61.read();
            value[5] = PFEID61.read();
            value[6] = UDID61.read();
            break;
        case 58:
            value[2] = CMIAD62.read();
            value[3] = CMIBD62.read();
            value[4] = PREID62.read();
            value[5] = PFEID62.read();
            value[6] = UDID62.read();
            break;
        case 59:
            value[2] = CMIAD63.read();
            value[3] = CMIBD63.read();
            value[4] = PREID63.read();
            value[5] = PFEID63.read();
            value[6] = UDID63.read();
            break;
        case 60:
            value[2] = CMIAD70.read();
            value[3] = CMIBD70.read();
            value[4] = PREID70.read();
            value[5] = PFEID70.read();
            value[6] = UDID70.read();
            break;
        case 61:
            value[2] = CMIAD71.read();
            value[3] = CMIBD71.read();
            value[4] = PREID71.read();
            value[5] = PFEID71.read();
            value[6] = UDID71.read();
            break;
        case 62:
            value[2] = CMIAD72.read();
            value[3] = CMIBD72.read();
            value[4] = PREID72.read();
            value[5] = PFEID72.read();
            value[6] = UDID72.read();
            break;
        default:    // 63
            value[2] = CMIAD73.read();
            value[3] = CMIBD73.read();
            value[4] = PREID73.read();
            value[5] = PFEID73.read();
            value[6] = UDID73.read();
            break;
    }

    unsigned int int_sel_cd = mATU5_GLUE_Func->GetRegSelectSetting(emDMASELCD0 + (id >> 3));
    unsigned bit_val = (int_sel_cd >> ((id % 8) * 4)) & 0x7;
    
    bool invalid = false;
    if (((id < 4) && (bit_val > 6)) ||
        ((4 <= id) && (id < 32) && (bit_val > 1)) ||
        ((32 <= id) && (id < 44) && (bit_val > 6)) ||
        ((44 <= id) && ((bit_val < 2) || (bit_val > 6))))
    {
        invalid = true;
    }
    
    if (invalid) {
        re_printf("warning","The setting in ATUDMASELCD%d.ATU_DMASEL_CD%d%d bit is prohibited.\n", id >> 3, (id >> 2) & 0x1, id & 0x3);
    } else {
        switch (id) {
            case 0:
                SLDCD0.write(value[bit_val]);
                break;
            case 1:
                SLDCD1.write(value[bit_val]);
                break;
            case 2:
                SLDCD2.write(value[bit_val]);
                break;
            case 3:
                SLDCD3.write(value[bit_val]);
                break;
            case 4:
                SLDCD4.write(value[bit_val]);
                break;
            case 5:
                SLDCD5.write(value[bit_val]);
                break;
            case 6:
                SLDCD6.write(value[bit_val]);
                break;
            case 7:
                SLDCD7.write(value[bit_val]);
                break;
            case 8:
                SLDCD8.write(value[bit_val]);
                break;
            case 9:
                SLDCD9.write(value[bit_val]);
                break;
            case 10:
                SLDCD10.write(value[bit_val]);
                break;
            case 11:
                SLDCD11.write(value[bit_val]);
                break;
            case 12:
                SLDCD12.write(value[bit_val]);
                break;
            case 13:
                SLDCD13.write(value[bit_val]);
                break;
            case 14:
                SLDCD14.write(value[bit_val]);
                break;
            case 15:
                SLDCD15.write(value[bit_val]);
                break;
            case 16:
                SLDCD16.write(value[bit_val]);
                break;
            case 17:
                SLDCD17.write(value[bit_val]);
                break;
            case 18:
                SLDCD18.write(value[bit_val]);
                break;
            case 19:
                SLDCD19.write(value[bit_val]);
                break;
            case 20:
                SLDCD20.write(value[bit_val]);
                break;
            case 21:
                SLDCD21.write(value[bit_val]);
                break;
            case 22:
                SLDCD22.write(value[bit_val]);
                break;
            case 23:
                SLDCD23.write(value[bit_val]);
                break;
            case 24:
                SLDCD24.write(value[bit_val]);
                break;
            case 25:
                SLDCD25.write(value[bit_val]);
                break;
            case 26:
                SLDCD26.write(value[bit_val]);
                break;
            case 27:
                SLDCD27.write(value[bit_val]);
                break;
            case 28:
                SLDCD28.write(value[bit_val]);
                break;
            case 29:
                SLDCD29.write(value[bit_val]);
                break;
            case 30:
                SLDCD30.write(value[bit_val]);
                break;
            case 31:
                SLDCD31.write(value[bit_val]);
                break;
            case 32:
                SLDCD32.write(value[bit_val]);
                break;
            case 33:
                SLDCD33.write(value[bit_val]);
                break;
            case 34:
                SLDCD34.write(value[bit_val]);
                break;
            case 35:
                SLDCD35.write(value[bit_val]);
                break;
            case 36:
                SLDCD36.write(value[bit_val]);
                break;
            case 37:
                SLDCD37.write(value[bit_val]);
                break;
            case 38:
                SLDCD38.write(value[bit_val]);
                break;
            case 39:
                SLDCD39.write(value[bit_val]);
                break;
            case 40:
                SLDCD40.write(value[bit_val]);
                break;
            case 41:
                SLDCD41.write(value[bit_val]);
                break;
            case 42:
                SLDCD42.write(value[bit_val]);
                break;
            case 43:
                SLDCD43.write(value[bit_val]);
                break;
            case 44:
                SLDCD44.write(value[bit_val]);
                break;
            case 45:
                SLDCD45.write(value[bit_val]);
                break;
            case 46:
                SLDCD46.write(value[bit_val]);
                break;
            case 47:
                SLDCD47.write(value[bit_val]);
                break;
            case 48:
                SLDCD48.write(value[bit_val]);
                break;
            case 49:
                SLDCD49.write(value[bit_val]);
                break;
            case 50:
                SLDCD50.write(value[bit_val]);
                break;
            case 51:
                SLDCD51.write(value[bit_val]);
                break;
            case 52:
                SLDCD52.write(value[bit_val]);
                break;
            case 53:
                SLDCD53.write(value[bit_val]);
                break;
            case 54:
                SLDCD54.write(value[bit_val]);
                break;
            case 55:
                SLDCD55.write(value[bit_val]);
                break;
            case 56:
                SLDCD56.write(value[bit_val]);
                break;
            case 57:
                SLDCD57.write(value[bit_val]);
                break;
            case 58:
                SLDCD58.write(value[bit_val]);
                break;
            case 59:
                SLDCD59.write(value[bit_val]);
                break;
            case 60:
                SLDCD60.write(value[bit_val]);
                break;
            case 61:
                SLDCD61.write(value[bit_val]);
                break;
            case 62:
                SLDCD62.write(value[bit_val]);
                break;
            default:    // 63
                SLDCD63.write(value[bit_val]);
                break;
        }
    }
}//}}}

void ATU5_GLUE::WriteSLDExMethod (void)
{//{{{
    bool value[9][4];
    value[0][0] = INTATUECMIE00.read();
    value[0][1] = INTATUECMIE01.read();
    value[0][2] = INTATUECMIE02.read();
    value[0][3] = INTATUECMIE03.read();
    value[1][0] = INTATUECMIE10.read();
    value[1][1] = INTATUECMIE11.read();
    value[1][2] = INTATUECMIE12.read();
    value[1][3] = INTATUECMIE13.read();
    value[2][0] = INTATUECMIE20.read();
    value[2][1] = INTATUECMIE21.read();
    value[2][2] = INTATUECMIE22.read();
    value[2][3] = INTATUECMIE23.read();
    value[3][0] = INTATUECMIE30.read();
    value[3][1] = INTATUECMIE31.read();
    value[3][2] = INTATUECMIE32.read();
    value[3][3] = INTATUECMIE33.read();
    value[4][0] = INTATUECMIE40.read();
    value[4][1] = INTATUECMIE41.read();
    value[4][2] = INTATUECMIE42.read();
    value[4][3] = INTATUECMIE43.read();
    value[5][0] = INTATUECMIE50.read();
    value[5][1] = INTATUECMIE51.read();
    value[5][2] = INTATUECMIE52.read();
    value[5][3] = INTATUECMIE53.read();
    value[6][0] = INTATUECMIE60.read();
    value[6][1] = INTATUECMIE61.read();
    value[6][2] = INTATUECMIE62.read();
    value[6][3] = INTATUECMIE63.read();
    value[7][0] = INTATUECMIE70.read();
    value[7][1] = INTATUECMIE71.read();
    value[7][2] = INTATUECMIE72.read();
    value[7][3] = INTATUECMIE73.read();
    value[8][0] = INTATUECMIE80.read();
    value[8][1] = INTATUECMIE81.read();
    value[8][2] = INTATUECMIE82.read();
    value[8][3] = INTATUECMIE83.read();

    unsigned int dma_sel_e = mATU5_GLUE_Func->GetRegSelectSetting(emDMASELE);

    SLDE0.write(value[0][dma_sel_e & 0x3]);
    SLDE1.write(value[1][(dma_sel_e >> 2) & 0x3]);
    SLDE2.write(value[2][(dma_sel_e >> 4) & 0x3]);
    SLDE3.write(value[3][(dma_sel_e >> 6) & 0x3]);
    SLDE4.write(value[4][(dma_sel_e >> 8) & 0x3]);
    SLDE5.write(value[5][(dma_sel_e >> 10) & 0x3]);
    SLDE6.write(value[6][(dma_sel_e >> 12) & 0x3]);
    SLDE7.write(value[7][(dma_sel_e >> 14) & 0x3]);
    SLDE8.write(value[8][(dma_sel_e >> 16) & 0x3]);
}//}}}

void ATU5_GLUE::WriteOVFMethod (void)
{//{{{
    bool value = OVIA.read();
    value |= (bool) OVIC0.read();
    value |= (bool) OVIC1.read();
    value |= (bool) OVIC2.read();
    value |= (bool) OVIC3.read();
    value |= (bool) OVIC4.read();
    value |= (bool) OVIC5.read();
    value |= (bool) OVIC6.read();
    value |= (bool) OVIC7.read();
    value |= (bool) OVIC8.read();
    value |= (bool) OVIC9.read();
    value |= (bool) OVIC10.read();
    value |= (bool) OVI1D0.read();
    value |= (bool) OVI1D1.read();
    value |= (bool) OVI1D2.read();
    value |= (bool) OVI1D3.read();
    value |= (bool) OVI1D4.read();
    value |= (bool) OVI1D5.read();
    value |= (bool) OVI1D6.read();
    value |= (bool) OVI1D7.read();
    value |= (bool) OVI1D8.read();
    value |= (bool) OVI2D0.read();
    value |= (bool) OVI2D1.read();
    value |= (bool) OVI2D2.read();
    value |= (bool) OVI2D3.read();
    value |= (bool) OVI2D4.read();
    value |= (bool) OVI2D5.read();
    value |= (bool) OVI2D6.read();
    value |= (bool) OVI2D7.read();
    value |= (bool) OVI2D8.read();
    value |= (bool) OVIF0.read();
    value |= (bool) OVIF1.read();
    value |= (bool) OVIF2.read();
    value |= (bool) OVIF3.read();
    value |= (bool) OVIF4.read();
    value |= (bool) OVIF5.read();
    value |= (bool) OVIF6.read();
    value |= (bool) OVIF7.read();
    value |= (bool) OVIF8.read();
    value |= (bool) OVIF9.read();
    value |= (bool) OVIF10.read();
    value |= (bool) OVIF11.read();
    value |= (bool) OVIF12.read();
    value |= (bool) OVIF13.read();
    value |= (bool) OVIF14.read();
    value |= (bool) OVIF15.read();
    
    OVF.write(value);
}//}}}

void ATU5_GLUE::WriteDFxMethod (unsigned int id)
{//{{{
    unsigned int id_mod = id % emDFEFactorNum;
    unsigned int reg_id = (unsigned int)(id / emDFEFactorNum);
    
    bool value[5];
    switch (id_mod) {
        case 0:     // DFD00i
            value[0] = CMIAD00.read();
            value[1] = CMIBD00.read();
            value[2] = PREID00.read();
            value[3] = PFEID00.read();
            value[4] = UDID00.read();
            break;
        case 1:     // DFD01i
            value[0] = CMIAD01.read();
            value[1] = CMIBD01.read();
            value[2] = PREID01.read();
            value[3] = PFEID01.read();
            value[4] = UDID01.read();
            break;
        case 2:
            value[0] = CMIAD02.read();
            value[1] = CMIBD02.read();
            value[2] = PREID02.read();
            value[3] = PFEID02.read();
            value[4] = UDID02.read();
            break;
        case 3:
            value[0] = CMIAD03.read();
            value[1] = CMIBD03.read();
            value[2] = PREID03.read();
            value[3] = PFEID03.read();
            value[4] = UDID03.read();
            break;
        case 4:
            value[0] = CMIAD10.read();
            value[1] = CMIBD10.read();
            value[2] = PREID10.read();
            value[3] = PFEID10.read();
            value[4] = UDID10.read();
            break;
        case 5:
            value[0] = CMIAD11.read();
            value[1] = CMIBD11.read();
            value[2] = PREID11.read();
            value[3] = PFEID11.read();
            value[4] = UDID11.read();
            break;
        case 6:     // DFC80i
            value[0] = IMIGC80.read();
            value[1] = IMIOC80.read();
            break;
        case 7:     // DFC81i
            value[0] = IMIGC81.read();
            value[1] = IMIOC81.read();
            break;
        case 8:
            value[0] = IMIGC82.read();
            value[1] = IMIOC82.read();
            break;
        case 9:
            value[0] = IMIGC83.read();
            value[1] = IMIOC83.read();
            break;
        case 10:
            value[0] = IMIGC90.read();
            value[1] = IMIOC90.read();
            break;
        default:    // 11
            value[0] = IMIGC91.read();
            value[1] = IMIOC91.read();
            break;
    }
    
    unsigned int dfe_sel = mATU5_GLUE_Func->GetRegSelectSetting(emDFESEL0 + reg_id);
    bool int_en = (bool)((mATU5_GLUE_Func->GetRegSelectSetting(emDFEENTQ0 + reg_id) >> id_mod) & 0x1);
    unsigned bit_val = 0;
    if (id_mod < 6) {
        // DFDx
        bit_val = (dfe_sel >> (id_mod * 4)) & 0x7;
    } else {
        // DFCx
        bit_val = (dfe_sel >> ((id_mod -6) + 24)) & 0x1;
    }
    
    bool invalid = false;
    if (bit_val > 4) {
        invalid = true;
    }
    
    if (invalid) {
        re_printf("warning","The setting in ATUDFESEL%d.ATU_DFESEL_D%d%d bit is prohibited.\n", reg_id, (id_mod >> 2) & 0x1, id_mod & 0x3);
    } else {
        if (int_en) {
            switch (reg_id) {
                case 0:
                    switch (id_mod) {
                        case 0:
                            DFD000.write(value[bit_val]);
                            break;
                        case 1:
                            DFD010.write(value[bit_val]);
                            break;
                        case 2:
                            DFD020.write(value[bit_val]);
                            break;
                        case 3:
                            DFD030.write(value[bit_val]);
                            break;
                        case 4:
                            DFD100.write(value[bit_val]);
                            break;
                        case 5:
                            DFD110.write(value[bit_val]);
                            break;
                        case 6:
                            DFC800.write(value[bit_val]);
                            break;
                        case 7:
                            DFC810.write(value[bit_val]);
                            break;
                        case 8:
                            DFC820.write(value[bit_val]);
                            break;
                        case 9:
                            DFC830.write(value[bit_val]);
                            break;
                        case 10:
                            DFC900.write(value[bit_val]);
                            break;
                        default:    // 11
                            DFC910.write(value[bit_val]);
                            break;
                    }
                    break;
                case 1:
                    switch (id_mod) {
                        case 0:
                            DFD001.write(value[bit_val]);
                            break;
                        case 1:
                            DFD011.write(value[bit_val]);
                            break;
                        case 2:
                            DFD021.write(value[bit_val]);
                            break;
                        case 3:
                            DFD031.write(value[bit_val]);
                            break;
                        case 4:
                            DFD101.write(value[bit_val]);
                            break;
                        case 5:
                            DFD111.write(value[bit_val]);
                            break;
                        case 6:
                            DFC801.write(value[bit_val]);
                            break;
                        case 7:
                            DFC811.write(value[bit_val]);
                            break;
                        case 8:
                            DFC821.write(value[bit_val]);
                            break;
                        case 9:
                            DFC831.write(value[bit_val]);
                            break;
                        case 10:
                            DFC901.write(value[bit_val]);
                            break;
                        default:    // 11
                            DFC911.write(value[bit_val]);
                            break;
                    }
                    break;
                default:    // 2
                    switch (id_mod) {
                        case 0:
                            DFD002.write(value[bit_val]);
                            break;
                        case 1:
                            DFD012.write(value[bit_val]);
                            break;
                        case 2:
                            DFD022.write(value[bit_val]);
                            break;
                        case 3:
                            DFD032.write(value[bit_val]);
                            break;
                        case 4:
                            DFD102.write(value[bit_val]);
                            break;
                        case 5:
                            DFD112.write(value[bit_val]);
                            break;
                        case 6:
                            DFC802.write(value[bit_val]);
                            break;
                        case 7:
                            DFC812.write(value[bit_val]);
                            break;
                        case 8:
                            DFC822.write(value[bit_val]);
                            break;
                        case 9:
                            DFC832.write(value[bit_val]);
                            break;
                        case 10:
                            DFC902.write(value[bit_val]);
                            break;
                        default:    // 11
                            DFC912.write(value[bit_val]);
                            break;
                    }
                    break;
            }
        } else {
            switch (reg_id) {
                case 0:
                    switch (id_mod) {
                        case 0:
                            DFD000.write(false);
                            break;
                        case 1:
                            DFD010.write(false);
                            break;
                        case 2:
                            DFD020.write(false);
                            break;
                        case 3:
                            DFD030.write(false);
                            break;
                        case 4:
                            DFD100.write(false);
                            break;
                        case 5:
                            DFD110.write(false);
                            break;
                        case 6:
                            DFC800.write(false);
                            break;
                        case 7:
                            DFC810.write(false);
                            break;
                        case 8:
                            DFC820.write(false);
                            break;
                        case 9:
                            DFC830.write(false);
                            break;
                        case 10:
                            DFC900.write(false);
                            break;
                        default:    // 11
                            DFC910.write(false);
                            break;
                    }
                    break;
                case 1:
                    switch (id_mod) {
                        case 0:
                            DFD001.write(false);
                            break;
                        case 1:
                            DFD011.write(false);
                            break;
                        case 2:
                            DFD021.write(false);
                            break;
                        case 3:
                            DFD031.write(false);
                            break;
                        case 4:
                            DFD101.write(false);
                            break;
                        case 5:
                            DFD111.write(false);
                            break;
                        case 6:
                            DFC801.write(false);
                            break;
                        case 7:
                            DFC811.write(false);
                            break;
                        case 8:
                            DFC821.write(false);
                            break;
                        case 9:
                            DFC831.write(false);
                            break;
                        case 10:
                            DFC901.write(false);
                            break;
                        default:    // 11
                            DFC911.write(false);
                            break;
                    }
                    break;
                default:    // 2
                    switch (id_mod) {
                        case 0:
                            DFD002.write(false);
                            break;
                        case 1:
                            DFD012.write(false);
                            break;
                        case 2:
                            DFD022.write(false);
                            break;
                        case 3:
                            DFD032.write(false);
                            break;
                        case 4:
                            DFD102.write(false);
                            break;
                        case 5:
                            DFD112.write(false);
                            break;
                        case 6:
                            DFC802.write(false);
                            break;
                        case 7:
                            DFC812.write(false);
                            break;
                        case 8:
                            DFC822.write(false);
                            break;
                        case 9:
                            DFC832.write(false);
                            break;
                        case 10:
                            DFC902.write(false);
                            break;
                        default:    // 11
                            DFC912.write(false);
                            break;
                    }
                    break;
            }
        }
    }
}//}}}

void ATU5_GLUE::WriteDEG1TRGMethod (void)
{//{{{
    bool value[8];
    value[0] = INTATUGCMIG0.read();
    value[1] = INTATUGCMIG1.read();
    value[2] = INTATUGCMIG2.read();
    value[3] = INTATUGCMIG3.read();
    value[4] = INTATUGCMIG4.read();
    value[5] = INTATUGCMIG5.read();
    value[6] = INTATUGCMIG6.read();
    value[7] = INTATUGCMIG7.read();
    
    unsigned int bit_val = mATU5_GLUE_Func->GetRegSelectSetting(emDFESELD1T) & 0x7;
    DEG1TRG.write(value[bit_val]);
}//}}}

void ATU5_GLUE::WriteDSTSxMethod (unsigned int id)
{//{{{
    bool value[4];
    switch (id) {
        case 0:
        value[0] = TOC00.read();
        value[1] = TOD00A.read();
        value[2] = TOD00B.read();
        value[3] = TOE00.read();
            break;
        case 1:
            value[0] = TOC01.read();
            value[1] = TOD01A.read();
            value[2] = TOD01B.read();
            value[3] = TOE01.read();
            break;
        case 2:
            value[0] = TOC02.read();
            value[1] = TOD02A.read();
            value[2] = TOD02B.read();
            value[3] = TOE02.read();
            break;
        case 3:
            value[0] = TOC03.read();
            value[1] = TOD03A.read();
            value[2] = TOD03B.read();
            value[3] = TOE03.read();
            break;
        case 4:
            value[0] = TOC10.read();
            value[1] = TOD10A.read();
            value[2] = TOD10B.read();
            value[3] = TOE10.read();
            break;
        case 5:
            value[0] = TOC11.read();
            value[1] = TOD11A.read();
            value[2] = TOD11B.read();
            value[3] = TOE11.read();
            break;
        case 6:
            value[0] = TOC12.read();
            value[1] = TOD12A.read();
            value[2] = TOD12B.read();
            value[3] = TOE12.read();
            break;
        case 7:
            value[0] = TOC13.read();
            value[1] = TOD13A.read();
            value[2] = TOD13B.read();
            value[3] = TOE13.read();
            break;
        case 8:
            value[0] = TOC20.read();
            value[1] = TOD20A.read();
            value[2] = TOD20B.read();
            value[3] = TOE20.read();
            break;
        default:    // 9
            value[0] = TOC21.read();
            value[1] = TOD21A.read();
            value[2] = TOD21B.read();
            value[3] = TOE21.read();
            break;
    }
    
    unsigned int ds_sel_dsts = mATU5_GLUE_Func->GetRegSelectSetting(emDSSELDSTS);
    unsigned int bit_val = (ds_sel_dsts >> (id * 2)) & 0x3;
    
    switch (id) {
        case 0:
            DSTS0.write(value[bit_val]);
            break;
        case 1:
            DSTS1.write(value[bit_val]);
            break;
        case 2:
            DSTS2.write(value[bit_val]);
            break;
        case 3:
            DSTS3.write(value[bit_val]);
            break;
        case 4:
            DSTS4.write(value[bit_val]);
            break;
        case 5:
            DSTS5.write(value[bit_val]);
            break;
        case 6:
            DSTS6.write(value[bit_val]);
            break;
        case 7:
            DSTS7.write(value[bit_val]);
            break;
        case 8:
            DSTS8.write(value[bit_val]);
            break;
        default:    // 9
            DSTS9.write(value[bit_val]);
            break;
    }
}//}}}

void ATU5_GLUE::WriteCATSMethod (void)
{//{{{
    bool value[4];
    value[0] = TOC22.read();
    value[1] = TOD22A.read();
    value[2] = TOD22B.read();
    value[3] = TOE22.read();
    
    unsigned int bit_val = mATU5_GLUE_Func->GetRegSelectSetting(emCASELCATS) & 0x3;
    CATS.write(value[bit_val]);
}//}}}

void ATU5_GLUE::WriteP5xMethod (unsigned int id)
{//{{{
    bool value[4];
    switch (id) {// Note: case 0 is removed. It never be called because ATUP5SSEL[1:0] are reserved bit
        case 1:
            value[0] = TOC01.read();
            value[1] = TOD01A.read();
            value[2] = TOD01B.read();
            value[3] = TOE01.read();
            break;
        case 2:
            value[0] = TOC02.read();
            value[1] = TOD02A.read();
            value[2] = TOD02B.read();
            value[3] = TOE02.read();
            break;
        case 3:
            value[0] = TOC03.read();
            value[1] = TOD03A.read();
            value[2] = TOD03B.read();
            value[3] = TOE03.read();
            break;
        case 4:
            value[0] = TOC10.read();
            value[1] = TOD10A.read();
            value[2] = TOD10B.read();
            value[3] = TOE10.read();
            break;
        case 5:
            value[0] = TOC11.read();
            value[1] = TOD11A.read();
            value[2] = TOD11B.read();
            value[3] = TOE11.read();
            break;
        case 6:
            value[0] = TOC12.read();
            value[1] = TOD12A.read();
            value[2] = TOD12B.read();
            value[3] = TOE12.read();
            break;
        case 7:
            value[0] = TOC13.read();
            value[1] = TOD13A.read();
            value[2] = TOD13B.read();
            value[3] = TOE13.read();
            break;
        case 8:
            value[0] = TOC20.read();
            value[1] = TOD20A.read();
            value[2] = TOD20B.read();
            value[3] = TOE20.read();
            break;
        case 9:
            value[0] = TOC21.read();
            value[1] = TOD21A.read();
            value[2] = TOD21B.read();
            value[3] = TOE21.read();
            break;
        case 10:
            value[0] = TOC22.read();
            value[1] = TOD22A.read();
            value[2] = TOD22B.read();
            value[3] = TOE22.read();
            break;
        case 11:
            value[0] = TOC23.read();
            value[1] = TOD23A.read();
            value[2] = TOD23B.read();
            value[3] = TOE23.read();
            break;
        case 12:
            value[0] = TOC30.read();
            value[1] = TOD30A.read();
            value[2] = TOD30B.read();
            value[3] = TOE30.read();
            break;
        default:    // 13
            value[0] = TOC31.read();
            value[1] = TOD31A.read();
            value[2] = TOD31B.read();
            value[3] = TOE31.read();
            break;
    }
    
    unsigned int p5s_sel = mATU5_GLUE_Func->GetRegSelectSetting(emP5SSEL);
    unsigned int bit_val = (p5s_sel >> (id * 2)) & 0x3;
    
    switch (id) {// Note: case 0 is removed. It never be called because ATUP5SSEL[1:0] are reserved bit
        case 1:
            P5TGC1.write(value[bit_val]);
            break;
        case 2:
            P5TGC2.write(value[bit_val]);
            break;
        case 3:
            P5TGC3.write(value[bit_val]);
            break;
        case 4:
            P5TGC4.write(value[bit_val]);
            break;
        case 5:
            P5TGC5.write(value[bit_val]);
            break;
        case 6:
            P5TGC6.write(value[bit_val]);
            break;
        case 7:
            P5TGC7.write(value[bit_val]);
            break;
        case 8:
            P5CKTSA.write(value[bit_val]);
            break;
        case 9:
            P5CKTSB.write(value[bit_val]);
            break;
        case 10:
            P5CRTSA.write(value[bit_val]);
            break;
        case 11:
            P5CRTSB.write(value[bit_val]);
            break;
        case 12:
            P5SSTSA.write(value[bit_val]);
            break;
        default:    // 13
            P5SSTSB.write(value[bit_val]);
            break;
    }
}//}}}

void ATU5_GLUE::WriteICIAxMethod (void)
{//{{{
    ICIA0.write(INTATUAICIA0.read());
    ICIA1.write(INTATUAICIA1.read());
    ICIA2.write(INTATUAICIA2.read());
    ICIA3.write(INTATUAICIA3.read());
    ICIA4.write(INTATUAICIA4.read());
    ICIA5.write(INTATUAICIA5.read());
    ICIA6.write(INTATUAICIA6.read());
    ICIA7.write(INTATUAICIA7.read());
}//}}}

void ATU5_GLUE::WriteCIBxMethod (void)
{//{{{
    CMIB0.write(INTATUBCMIB0.read());
    CMIB1.write(INTATUBCMIB1.read());
    CMIB6.write(INTATUBCMIB6.read());
    CMIB10.write(INTATUBCMIB10.read());
    CMIB11.write(INTATUBCMIB11.read());
    CMIB12.write(INTATUBCMIB12.read());
    CMIB6M.write(INTATUBCMIB6M.read());
    CMIB66M.write(INTATUBCMIB66M.read());
    ICIB0.write(INTATUBICIB0.read());
}//}}}

void ATU5_GLUE::WriteCMIExMethod (void)
{//{{{
    CMIE00.write(INTATUECMIE00.read());
    CMIE01.write(INTATUECMIE01.read());
    CMIE02.write(INTATUECMIE02.read());
    CMIE03.write(INTATUECMIE03.read());
    CMIE10.write(INTATUECMIE10.read());
    CMIE11.write(INTATUECMIE11.read());
    CMIE12.write(INTATUECMIE12.read());
    CMIE13.write(INTATUECMIE13.read());
    CMIE20.write(INTATUECMIE20.read());
    CMIE21.write(INTATUECMIE21.read());
    CMIE22.write(INTATUECMIE22.read());
    CMIE23.write(INTATUECMIE23.read());
    CMIE30.write(INTATUECMIE30.read());
    CMIE31.write(INTATUECMIE31.read());
    CMIE32.write(INTATUECMIE32.read());
    CMIE33.write(INTATUECMIE33.read());
    CMIE40.write(INTATUECMIE40.read());
    CMIE41.write(INTATUECMIE41.read());
    CMIE42.write(INTATUECMIE42.read());
    CMIE43.write(INTATUECMIE43.read());
    CMIE50.write(INTATUECMIE50.read());
    CMIE51.write(INTATUECMIE51.read());
    CMIE52.write(INTATUECMIE52.read());
    CMIE53.write(INTATUECMIE53.read());
    CMIE60.write(INTATUECMIE60.read());
    CMIE61.write(INTATUECMIE61.read());
    CMIE62.write(INTATUECMIE62.read());
    CMIE63.write(INTATUECMIE63.read());
    CMIE70.write(INTATUECMIE70.read());
    CMIE71.write(INTATUECMIE71.read());
    CMIE72.write(INTATUECMIE72.read());
    CMIE73.write(INTATUECMIE73.read());
    CMIE80.write(INTATUECMIE80.read());
    CMIE81.write(INTATUECMIE81.read());
    CMIE82.write(INTATUECMIE82.read());
    CMIE83.write(INTATUECMIE83.read());
}//}}}

void ATU5_GLUE::WriteICIFxMethod (void)
{//{{{
    ICIF0.write(INTATUFICIF0.read());
    ICIF1.write(INTATUFICIF1.read());
    ICIF2.write(INTATUFICIF2.read());
    ICIF3.write(INTATUFICIF3.read());
    ICIF4.write(INTATUFICIF4.read());
    ICIF5.write(INTATUFICIF5.read());
    ICIF6.write(INTATUFICIF6.read());
    ICIF7.write(INTATUFICIF7.read());
    ICIF8.write(INTATUFICIF8.read());
    ICIF9.write(INTATUFICIF9.read());
    ICIF10.write(INTATUFICIF10.read());
    ICIF11.write(INTATUFICIF11.read());
    ICIF12.write(INTATUFICIF12.read());
    ICIF13.write(INTATUFICIF13.read());
    ICIF14.write(INTATUFICIF14.read());
    ICIF15.write(INTATUFICIF15.read());
}//}}}

void ATU5_GLUE::WriteCMIGxMethod (void)
{//{{{
    CMIG0.write(INTATUGCMIG0.read());
    CMIG1.write(INTATUGCMIG1.read());
    CMIG2.write(INTATUGCMIG2.read());
    CMIG3.write(INTATUGCMIG3.read());
    CMIG4.write(INTATUGCMIG4.read());
    CMIG5.write(INTATUGCMIG5.read());
    CMIG6.write(INTATUGCMIG6.read());
    CMIG7.write(INTATUGCMIG7.read());
    CMIG8.write(INTATUGCMIG8.read());
    CMIG9.write(INTATUGCMIG9.read());
}//}}}

void ATU5_GLUE::WriteDMAREQxMethod (void)
{//{{{
    DMAREQ00.write(INTATUCTRLDMAREQ00.read());
    DMAREQ01.write(INTATUCTRLDMAREQ01.read());
    DMAREQ10.write(INTATUCTRLDMAREQ10.read());
    DMAREQ11.write(INTATUCTRLDMAREQ11.read());
}//}}}

void ATU5_GLUE::NotifyEvent (const unsigned int event, const unsigned int channel)
{//{{{
    switch (event) {
        case emINTSELA :
            mINTSELAregChangeEvent.notify(SC_ZERO_TIME);
            break;
        case emINTSELD0 :
            mINTSELDregChangeEvent[channel].notify(SC_ZERO_TIME);
            break;
        case emDMASELB :
            mDMASELBregChangeEvent.notify(SC_ZERO_TIME);
            break;
        case emDMASELCD0 :
            mDMASELCDregChangeEvent[channel].notify(SC_ZERO_TIME);
            break;
        case emDFEENTQ0 :
            mDFEENTQregChangeEvent[channel].notify(SC_ZERO_TIME);
            break;
        case emDFESEL0 :
            mDFESELregChangeEvent[channel].notify(SC_ZERO_TIME);
            break;
        case emDFESELD1T :
            mDFESELD1TregChangeEvent.notify(SC_ZERO_TIME);
            break;
        case emDSSELDSTS :
            mDSSELDSTSregChangeEvent.notify(SC_ZERO_TIME);
            break;
        case emCASELCATS :
            mCASELCATSregChangeEvent.notify(SC_ZERO_TIME);
            break;
        case emP5SSEL :
            mP5SSELregChangeEvent.notify(SC_ZERO_TIME);
            break;
        default :       // emDMASELE
            mDMASELEregChangeEvent.notify(SC_ZERO_TIME);
            break;
    }
}//}}}

// vim600: set foldmethod=marker :
