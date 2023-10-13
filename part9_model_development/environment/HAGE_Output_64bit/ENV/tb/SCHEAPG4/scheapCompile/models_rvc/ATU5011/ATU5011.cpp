// ---------------------------------------------------------------------
// $Id: ATU5011.cpp 1196 2019-10-17 05:33:04Z chuonghoangle $
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
#include "ATU5011.h"
#include "ATU5011_Func.h"
#include "ATU5_TIMERA.h"
#include "ATU5_TIMERA_Func.h"
#include "ATU5_TIMERB.h"
#include "ATU5_TIMERB_Func.h"
#include "ATU5_TIMERC.h"
#include "ATU5_TIMERC_Func.h"
#include "ATU5_TIMERD.h"
#include "ATU5_TIMERD_Func.h"
#include "ATU5_TIMERE.h"
#include "ATU5_TIMERE_Func.h"
#include "ATU5_TIMERF.h"
#include "ATU5_TIMERF_Func.h"
#include "ATU5_TIMERG.h"
#include "ATU5_TIMERG_Func.h"

///Constructor of ATU5011 class
ATU5011::ATU5011 (sc_module_name name,
            unsigned int rLatency,
            unsigned int wLatency):
            sc_module(name),
            ATU5011_AgentController(),
            BusSlaveBase<32,1>(),
            CLK_LSB("CLK_LSB"),
            CLK_HSB("CLK_HSB"),
            presetz("presetz"),
            presetz_bif("presetz_bif"),
            TCLKB("TCLKB"),
            TCLKA("TCLKA"),
            // Input ports
            // TimerA
            TIA0("TIA0"),
            TIA1("TIA1"),
            TIA2("TIA2"),
            TIA3("TIA3"),
            TIA4("TIA4"),
            TIA5("TIA5"),
            TIA6("TIA6"),
            TIA7("TIA7"),
            // TimerC
            TIOC00("TIOC00"),
            TIOC01("TIOC01"),
            TIOC02("TIOC02"),
            TIOC03("TIOC03"),
            TIOC10("TIOC10"),
            TIOC11("TIOC11"),
            TIOC12("TIOC12"),
            TIOC13("TIOC13"),
            TIOC20("TIOC20"),
            TIOC21("TIOC21"),
            TIOC22("TIOC22"),
            TIOC23("TIOC23"),
            TIOC30("TIOC30"),
            TIOC31("TIOC31"),
            TIOC32("TIOC32"),
            TIOC33("TIOC33"),
            TIOC40("TIOC40"),
            TIOC41("TIOC41"),
            TIOC42("TIOC42"),
            TIOC43("TIOC43"),
            TIOC50("TIOC50"),
            TIOC51("TIOC51"),
            TIOC52("TIOC52"),
            TIOC53("TIOC53"),
            TIOC60("TIOC60"),
            TIOC61("TIOC61"),
            TIOC62("TIOC62"),
            TIOC63("TIOC63"),
            TIOC70("TIOC70"),
            TIOC71("TIOC71"),
            TIOC72("TIOC72"),
            TIOC73("TIOC73"),
            TIOC80("TIOC80"),
            TIOC81("TIOC81"),
            TIOC82("TIOC82"),
            TIOC83("TIOC83"),
            TIOC90("TIOC90"),
            TIOC91("TIOC91"),
            TIOC92("TIOC92"),
            TIOC93("TIOC93"),
            TIOC100("TIOC100"),
            TIOC101("TIOC101"),
            TIOC102("TIOC102"),
            TIOC103("TIOC103"),
			TIOC110("TIOC110"),
			TIOC111("TIOC111"),
			TIOC112("TIOC112"),
			TIOC113("TIOC113"),
			TIOC120("TIOC120"),
			TIOC121("TIOC121"),
			TIOC122("TIOC122"),
			TIOC123("TIOC123"),
			TIOC130("TIOC130"),
			TIOC131("TIOC131"),
			TIOC132("TIOC132"),
			TIOC133("TIOC133"),
            // TOCExy
            TOCE00("TOCE00"),
            TOCE01("TOCE01"),
            TOCE02("TOCE02"),
            TOCE03("TOCE03"),
            TOCE10("TOCE10"),
            TOCE11("TOCE11"),
            TOCE12("TOCE12"),
            TOCE13("TOCE13"),
            TOCE20("TOCE20"),
            TOCE21("TOCE21"),
            TOCE22("TOCE22"),
            TOCE23("TOCE23"),
            TOCE30("TOCE30"),
            TOCE31("TOCE31"),
            TOCE32("TOCE32"),
            TOCE33("TOCE33"),
            TOCE40("TOCE40"),
            TOCE41("TOCE41"),
            TOCE42("TOCE42"),
            TOCE43("TOCE43"),
            TOCE50("TOCE50"),
            TOCE51("TOCE51"),
            TOCE52("TOCE52"),
            TOCE53("TOCE53"),
            TOCE60("TOCE60"),
            TOCE61("TOCE61"),
            TOCE62("TOCE62"),
            TOCE63("TOCE63"),
            TOCE70("TOCE70"),
            TOCE71("TOCE71"),
            TOCE72("TOCE72"),
            TOCE73("TOCE73"),
            TOCE80("TOCE80"),
            TOCE81("TOCE81"),
            TOCE82("TOCE82"),
            TOCE83("TOCE83"),
            TOCE90("TOCE90"),
            TOCE91("TOCE91"),
            TOCE92("TOCE92"),
            TOCE93("TOCE93"),
            TOCE100("TOCE100"),
            TOCE101("TOCE101"),
            TOCE102("TOCE102"),
            TOCE103("TOCE103"),
			TOCE110("TOCE110"),
			TOCE111("TOCE111"),
			TOCE112("TOCE112"),
			TOCE113("TOCE113"),
			TOCE120("TOCE120"),
			TOCE121("TOCE121"),
			TOCE122("TOCE122"),
			TOCE123("TOCE123"),
			TOCE130("TOCE130"),
			TOCE131("TOCE131"),
			TOCE132("TOCE132"),
			TOCE133("TOCE133"),
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
            // TimerD
            TID00("TID00"),
            TID01("TID01"),
            TID02("TID02"),
            TID03("TID03"),
            TID04("TID04"),
            TID05("TID05"),
            TID06("TID06"),
            TID07("TID07"),
            TID10("TID10"),
            TID11("TID11"),
            TID12("TID12"),
            TID13("TID13"),
            TID14("TID14"),
            TID15("TID15"),
            TID16("TID16"),
            TID17("TID17"),
            TID20("TID20"),
            TID21("TID21"),
            TID22("TID22"),
            TID23("TID23"),
            TID24("TID24"),
            TID25("TID25"),
            TID26("TID26"),
            TID27("TID27"),
            TID30("TID30"),
            TID31("TID31"),
            TID32("TID32"),
            TID33("TID33"),
            TID34("TID34"),
            TID35("TID35"),
            TID36("TID36"),
            TID37("TID37"),
            TID40("TID40"),
            TID41("TID41"),
            TID42("TID42"),
            TID43("TID43"),
            TID44("TID44"),
            TID45("TID45"),
            TID46("TID46"),
            TID47("TID47"),
            TID50("TID50"),
            TID51("TID51"),
            TID52("TID52"),
            TID53("TID53"),
            TID54("TID54"),
            TID55("TID55"),
            TID56("TID56"),
            TID57("TID57"),
            TID60("TID60"),
            TID61("TID61"),
            TID62("TID62"),
            TID63("TID63"),
            TID64("TID64"),
            TID65("TID65"),
            TID66("TID66"),
            TID67("TID67"),
            TID70("TID70"),
            TID71("TID71"),
            TID72("TID72"),
            TID73("TID73"),
            TID74("TID74"),
            TID75("TID75"),
            TID76("TID76"),
            TID77("TID77"),
            TID80("TID80"),
            TID81("TID81"),
            TID82("TID82"),
            TID83("TID83"),
            TID84("TID84"),
            TID85("TID85"),
            TID86("TID86"),
            TID87("TID87"),

			TID90("TID90"),
			TID91("TID91"),
			TID92("TID92"),
			TID93("TID93"),
			TID94("TID94"),
			TID95("TID95"),
			TID96("TID96"),
			TID97("TID97"),
			TID100("TID100"),
			TID101("TID101"),
			TID102("TID102"),
			TID103("TID103"),
			TID104("TID104"),
			TID105("TID105"),
			TID106("TID106"),
			TID107("TID107"),
			TID110("TID110"),
			TID111("TID111"),
			TID112("TID112"),
			TID113("TID113"),
			TID114("TID114"),
			TID115("TID115"),
			TID116("TID116"),
			TID117("TID117"),
			TID120("TID120"),
			TID121("TID121"),
			TID122("TID122"),
			TID123("TID123"),
			TID124("TID124"),
			TID125("TID125"),
			TID126("TID126"),
			TID127("TID127"),
			TID130("TID130"),
			TID131("TID131"),
			TID132("TID132"),
			TID133("TID133"),
			TID134("TID134"),
			TID135("TID135"),
			TID136("TID136"),
			TID137("TID137"),
			TID140("TID140"),
			TID141("TID141"),
			TID142("TID142"),
			TID143("TID143"),
			TID144("TID144"),
			TID145("TID145"),
			TID146("TID146"),
			TID147("TID147"),

            // TimerF
            TIF0A("TIF0A"),
            TIF1A("TIF1A"),
            TIF2A("TIF2A"),
            TIF3("TIF3"),
            TIF4("TIF4"),
            TIF5("TIF5"),
            TIF6("TIF6"),
            TIF7("TIF7"),
            TIF8("TIF8"),
            TIF9("TIF9"),
            TIF10("TIF10"),
            TIF11("TIF11"),
            TIF12("TIF12"),
            TIF13("TIF13"),
            TIF14("TIF14"),
            TIF15("TIF15"),
            TIF0B("TIF0B"),
            TIF1B("TIF1B"),
            TIF2B("TIF2B"),
            // DMA
            pf_dmai_s("pf_dmai_s"),
            pf_dmai0_a("pf_dmai0_a"),
            pf_dmai1_a("pf_dmai1_a"),
            pf_dmai2_a("pf_dmai2_a"),
            pf_dmai3_a("pf_dmai3_a"),
            pf_dmai4_a("pf_dmai4_a"),
            // Output ports
            // TimerD
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
            TOD32A("TOD32A"),
            TOD33A("TOD33A"),
            TOD40A("TOD40A"),
            TOD41A("TOD41A"),
            TOD42A("TOD42A"),
            TOD43A("TOD43A"),
            TOD50A("TOD50A"),
            TOD51A("TOD51A"),
            TOD52A("TOD52A"),
            TOD53A("TOD53A"),
            TOD60A("TOD60A"),
            TOD61A("TOD61A"),
            TOD62A("TOD62A"),
            TOD63A("TOD63A"),
            TOD70A("TOD70A"),
            TOD71A("TOD71A"),
            TOD72A("TOD72A"),
            TOD73A("TOD73A"),
            TOD80A("TOD80A"),
            TOD81A("TOD81A"),
            TOD82A("TOD82A"),
            TOD83A("TOD83A"),
			TOD90A("TOD90A"),
			TOD91A("TOD91A"),
			TOD92A("TOD92A"),
			TOD93A("TOD93A"),
			TOD100A("TOD100A"),
			TOD101A("TOD101A"),
			TOD102A("TOD102A"),
			TOD103A("TOD103A"),
			TOD110A("TOD110A"),
			TOD111A("TOD111A"),
			TOD112A("TOD112A"),
			TOD113A("TOD113A"),
			TOD120A("TOD120A"),
			TOD121A("TOD121A"),
			TOD122A("TOD122A"),
			TOD123A("TOD123A"),
			TOD130A("TOD130A"),
			TOD131A("TOD131A"),
			TOD132A("TOD132A"),
			TOD133A("TOD133A"),
			TOD140A("TOD140A"),
			TOD141A("TOD141A"),
			TOD142A("TOD142A"),
			TOD143A("TOD143A"),

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
            TOD32B("TOD32B"),
            TOD33B("TOD33B"),
            TOD40B("TOD40B"),
            TOD41B("TOD41B"),
            TOD42B("TOD42B"),
            TOD43B("TOD43B"),
            TOD50B("TOD50B"),
            TOD51B("TOD51B"),
            TOD52B("TOD52B"),
            TOD53B("TOD53B"),
            TOD60B("TOD60B"),
            TOD61B("TOD61B"),
            TOD62B("TOD62B"),
            TOD63B("TOD63B"),
            TOD70B("TOD70B"),
            TOD71B("TOD71B"),
            TOD72B("TOD72B"),
            TOD73B("TOD73B"),
            TOD80B("TOD80B"),
            TOD81B("TOD81B"),
            TOD82B("TOD82B"),
            TOD83B("TOD83B"),
			TOD90B("TOD90B"),
			TOD91B("TOD91B"),
			TOD92B("TOD92B"),
			TOD93B("TOD93B"),
			TOD100B("TOD100B"),
			TOD101B("TOD101B"),
			TOD102B("TOD102B"),
			TOD103B("TOD103B"),
			TOD110B("TOD110B"),
			TOD111B("TOD111B"),
			TOD112B("TOD112B"),
			TOD113B("TOD113B"),
			TOD120B("TOD120B"),
			TOD121B("TOD121B"),
			TOD122B("TOD122B"),
			TOD123B("TOD123B"),
			TOD130B("TOD130B"),
			TOD131B("TOD131B"),
			TOD132B("TOD132B"),
			TOD133B("TOD133B"),
			TOD140B("TOD140B"),
			TOD141B("TOD141B"),
			TOD142B("TOD142B"),
			TOD143B("TOD143B"),


            // TODMIGxy
            TODMIG00("TODMIG00"),
            TODMIG01("TODMIG01"),
            TODMIG02("TODMIG02"),
            TODMIG03("TODMIG03"),
            TODMIG10("TODMIG10"),
            TODMIG11("TODMIG11"),
            TODMIG12("TODMIG12"),
            TODMIG13("TODMIG13"),
            TODMIG20("TODMIG20"),
            TODMIG21("TODMIG21"),
            TODMIG22("TODMIG22"),
            TODMIG23("TODMIG23"),
            TODMIG30("TODMIG30"),
            TODMIG31("TODMIG31"),
            TODMIG32("TODMIG32"),
            TODMIG33("TODMIG33"),
            TODMIG40("TODMIG40"),
            TODMIG41("TODMIG41"),
            TODMIG42("TODMIG42"),
            TODMIG43("TODMIG43"),
            TODMIG50("TODMIG50"),
            TODMIG51("TODMIG51"),
            TODMIG52("TODMIG52"),
            TODMIG53("TODMIG53"),
            TODMIG60("TODMIG60"),
            TODMIG61("TODMIG61"),
            TODMIG62("TODMIG62"),
            TODMIG63("TODMIG63"),
            TODMIG70("TODMIG70"),
            TODMIG71("TODMIG71"),
            TODMIG72("TODMIG72"),
            TODMIG73("TODMIG73"),
            TODMIG80("TODMIG80"),
            TODMIG81("TODMIG81"),
            TODMIG82("TODMIG82"),
            TODMIG83("TODMIG83"),
			TODMIG90("TODMIG90"),
			TODMIG91("TODMIG91"),
			TODMIG92("TODMIG92"),
			TODMIG93("TODMIG93"),
			TODMIG100("TODMIG100"),
			TODMIG101("TODMIG101"),
			TODMIG102("TODMIG102"),
			TODMIG103("TODMIG103"),
			TODMIG110("TODMIG110"),
			TODMIG111("TODMIG111"),
			TODMIG112("TODMIG112"),
			TODMIG113("TODMIG113"),
			TODMIG120("TODMIG120"),
			TODMIG121("TODMIG121"),
			TODMIG122("TODMIG122"),
			TODMIG123("TODMIG123"),
			TODMIG130("TODMIG130"),
			TODMIG131("TODMIG131"),
			TODMIG132("TODMIG132"),
			TODMIG133("TODMIG133"),
			TODMIG140("TODMIG140"),
			TODMIG141("TODMIG141"),
			TODMIG142("TODMIG142"),
			TODMIG143("TODMIG143"),


            // TimerE
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
            TOE32("TOE32"),
            TOE33("TOE33"),
            TOE40("TOE40"),
            TOE41("TOE41"),
            TOE42("TOE42"),
            TOE43("TOE43"),
            TOE50("TOE50"),
            TOE51("TOE51"),
            TOE52("TOE52"),
            TOE53("TOE53"),
            TOE60("TOE60"),
            TOE61("TOE61"),
            TOE62("TOE62"),
            TOE63("TOE63"),
            TOE70("TOE70"),
            TOE71("TOE71"),
            TOE72("TOE72"),
            TOE73("TOE73"),
            TOE80("TOE80"),
            TOE81("TOE81"),
            TOE82("TOE82"),
            TOE83("TOE83"),
            // Interrupts
            // TimerA
            INTATUAICIA0("INTATUAICIA0"),
            INTATUAICIA1("INTATUAICIA1"),
            INTATUAICIA2("INTATUAICIA2"),
            INTATUAICIA3("INTATUAICIA3"),
            INTATUAICIA4("INTATUAICIA4"),
            INTATUAICIA5("INTATUAICIA5"),
            INTATUAICIA6("INTATUAICIA6"),
            INTATUAICIA7("INTATUAICIA7"),
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
			OVIC11("OVIC11"),
			OVIC12("OVIC12"),
			OVIC13("OVIC13"),
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
			IMIGC110("IMIGC110"),
			IMIGC111("IMIGC111"),
			IMIGC112("IMIGC112"),
			IMIGC113("IMIGC113"),
			IMIGC120("IMIGC120"),
			IMIGC121("IMIGC121"),
			IMIGC122("IMIGC122"),
			IMIGC123("IMIGC123"),
			IMIGC130("IMIGC130"),
			IMIGC131("IMIGC131"),
			IMIGC132("IMIGC132"),
			IMIGC133("IMIGC133"),
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
			IMIOC110("IMIOC110"),
			IMIOC111("IMIOC111"),
			IMIOC112("IMIOC112"),
			IMIOC113("IMIOC113"),
			IMIOC120("IMIOC120"),
			IMIOC121("IMIOC121"),
			IMIOC122("IMIOC122"),
			IMIOC123("IMIOC123"),
			IMIOC130("IMIOC130"),
			IMIOC131("IMIOC131"),
			IMIOC132("IMIOC132"),
			IMIOC133("IMIOC133"),
            // TimerD
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
			CMIAD90("CMIAD90"),
			CMIAD91("CMIAD91"),
			CMIAD92("CMIAD92"),
			CMIAD93("CMIAD93"),
			CMIAD100("CMIAD100"),
			CMIAD101("CMIAD101"),
			CMIAD102("CMIAD102"),
			CMIAD103("CMIAD103"),
			CMIAD110("CMIAD110"),
			CMIAD111("CMIAD111"),
			CMIAD112("CMIAD112"),
			CMIAD113("CMIAD113"),
			CMIAD120("CMIAD120"),
			CMIAD121("CMIAD121"),
			CMIAD122("CMIAD122"),
			CMIAD123("CMIAD123"),
			CMIAD130("CMIAD130"),
			CMIAD131("CMIAD131"),
			CMIAD132("CMIAD132"),
			CMIAD133("CMIAD133"),
			CMIAD140("CMIAD140"),
			CMIAD141("CMIAD141"),
			CMIAD142("CMIAD142"),
			CMIAD143("CMIAD143"),

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
			CMIBD90("CMIBD90"),
			CMIBD91("CMIBD91"),
			CMIBD92("CMIBD92"),
			CMIBD93("CMIBD93"),
			CMIBD100("CMIBD100"),
			CMIBD101("CMIBD101"),
			CMIBD102("CMIBD102"),
			CMIBD103("CMIBD103"),
			CMIBD110("CMIBD110"),
			CMIBD111("CMIBD111"),
			CMIBD112("CMIBD112"),
			CMIBD113("CMIBD113"),
			CMIBD120("CMIBD120"),
			CMIBD121("CMIBD121"),
			CMIBD122("CMIBD122"),
			CMIBD123("CMIBD123"),
			CMIBD130("CMIBD130"),
			CMIBD131("CMIBD131"),
			CMIBD132("CMIBD132"),
			CMIBD133("CMIBD133"),
			CMIBD140("CMIBD140"),
			CMIBD141("CMIBD141"),
			CMIBD142("CMIBD142"),
			CMIBD143("CMIBD143"),
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
			OVI1D9("OVI1D9"),
			OVI1D10("OVI1D10"),
			OVI1D11("OVI1D11"),
			OVI1D12("OVI1D12"),
			OVI1D13("OVI1D13"),
			OVI1D14("OVI1D14"),
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
			OVI2D9("OVI2D9"),
			OVI2D10("OVI2D10"),
			OVI2D11("OVI2D11"),
			OVI2D12("OVI2D12"),
			OVI2D13("OVI2D13"),
			OVI2D14("OVI2D14"),
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
			UDID90("UDID90"),
			UDID91("UDID91"),
			UDID92("UDID92"),
			UDID93("UDID93"),
			UDID100("UDID100"),
			UDID101("UDID101"),
			UDID102("UDID102"),
			UDID103("UDID103"),
			UDID110("UDID110"),
			UDID111("UDID111"),
			UDID112("UDID112"),
			UDID113("UDID113"),
			UDID120("UDID120"),
			UDID121("UDID121"),
			UDID122("UDID122"),
			UDID123("UDID123"),
			UDID130("UDID130"),
			UDID131("UDID131"),
			UDID132("UDID132"),
			UDID133("UDID133"),
			UDID140("UDID140"),
			UDID141("UDID141"),
			UDID142("UDID142"),
			UDID143("UDID143"),
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
			PREID90("PREID90"),
			PREID91("PREID91"),
			PREID92("PREID92"),
			PREID93("PREID93"),
			PREID100("PREID100"),
			PREID101("PREID101"),
			PREID102("PREID102"),
			PREID103("PREID103"),
			PREID110("PREID110"),
			PREID111("PREID111"),
			PREID112("PREID112"),
			PREID113("PREID113"),
			PREID120("PREID120"),
			PREID121("PREID121"),
			PREID122("PREID122"),
			PREID123("PREID123"),
			PREID130("PREID130"),
			PREID131("PREID131"),
			PREID132("PREID132"),
			PREID133("PREID133"),
			PREID140("PREID140"),
			PREID141("PREID141"),
			PREID142("PREID142"),
			PREID143("PREID143"),
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
			PFEID90("PFEID90"),
			PFEID91("PFEID91"),
			PFEID92("PFEID92"),
			PFEID93("PFEID93"),
			PFEID100("PFEID100"),
			PFEID101("PFEID101"),
			PFEID102("PFEID102"),
			PFEID103("PFEID103"),
			PFEID110("PFEID110"),
			PFEID111("PFEID111"),
			PFEID112("PFEID112"),
			PFEID113("PFEID113"),
			PFEID120("PFEID120"),
			PFEID121("PFEID121"),
			PFEID122("PFEID122"),
			PFEID123("PFEID123"),
			PFEID130("PFEID130"),
			PFEID131("PFEID131"),
			PFEID132("PFEID132"),
			PFEID133("PFEID133"),
			PFEID140("PFEID140"),
			PFEID141("PFEID141"),
			PFEID142("PFEID142"),
			PFEID143("PFEID143"),

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
            INTATUFICIF16("INTATUFICIF16"),
            INTATUFICIF17("INTATUFICIF17"),
            INTATUFICIF18("INTATUFICIF18"),
            INTATUFICIF19("INTATUFICIF19"),
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
            at_saradtrg("at_saradtrg"),
            // Signal
            dummy_TOC32("dummy_TOC32"),
            dummy_TOC33("dummy_TOC33"),
            dummy_TOC40("dummy_TOC40"),
            dummy_TOC41("dummy_TOC41"),
            dummy_TOC42("dummy_TOC42"),
            dummy_TOC43("dummy_TOC43"),
            dummy_TOC50("dummy_TOC50"),
            dummy_TOC51("dummy_TOC51"),
            dummy_TOC52("dummy_TOC52"),
            dummy_TOC53("dummy_TOC53"),
            dummy_TOC60("dummy_TOC60"),
            dummy_TOC61("dummy_TOC61"),
            dummy_TOC62("dummy_TOC62"),
            dummy_TOC63("dummy_TOC63"),
            dummy_TOC70("dummy_TOC70"),
            dummy_TOC71("dummy_TOC71"),
            dummy_TOC72("dummy_TOC72"),
            dummy_TOC73("dummy_TOC73"),
            dummy_TOC80("dummy_TOC80"),
            dummy_TOC81("dummy_TOC81"),
            dummy_TOC82("dummy_TOC82"),
            dummy_TOC83("dummy_TOC83"),
            dummy_TOC90("dummy_TOC90"),
            dummy_TOC91("dummy_TOC91"),
            dummy_TOC92("dummy_TOC92"),
            dummy_TOC93("dummy_TOC93"),
            dummy_TOC100("dummy_TOC100"),
            dummy_TOC101("dummy_TOC101"),
            dummy_TOC102("dummy_TOC102"),
            dummy_TOC103("dummy_TOC103"),
			dummy_TOC110("dummy_TOC110"),
			dummy_TOC111("dummy_TOC111"),
			dummy_TOC112("dummy_TOC112"),
			dummy_TOC113("dummy_TOC113"),
			dummy_TOC120("dummy_TOC120"),
			dummy_TOC121("dummy_TOC121"),
			dummy_TOC122("dummy_TOC122"),
			dummy_TOC123("dummy_TOC123"),
			dummy_TOC130("dummy_TOC130"),
			dummy_TOC131("dummy_TOC131"),
			dummy_TOC132("dummy_TOC132"),
			dummy_TOC133("dummy_TOC133"),


            timerA_evt_1_0_sig("timerA_evt_1_0_sig"),
            timerA_evt_1_1_sig("timerA_evt_1_1_sig"),
            timerA_evt_1_2_sig("timerA_evt_1_2_sig"),
            timerA_evt_1B_sig("timerA_evt_1B_sig"),
            timerA_evt_1C_sig("timerA_evt_1C_sig"),
            timerA_evt_1D_sig("timerA_evt_1D_sig"),
            timerA_evt_1E_sig("timerA_evt_1E_sig"),
            timerA_evt_1F_sig("timerA_evt_1F_sig"),
            timerA_evt_1G_sig("timerA_evt_1G_sig"),
            timerA_evt_1H_sig("timerA_evt_1H_sig"),
            timerA_evt_1I_sig("timerA_evt_1I_sig"),
            timerA_evt_2A_sig("timerA_evt_2A_sig"),
            timerA_evt_2B_sig("timerA_evt_2B_sig"),
            timerB_AGCKM_sig("timerB_AGCKM_sig"),
            timerB_AGCK1_sig("timerB_AGCK1_sig"),
            timerB_AGCKTIM_sig("timerB_AGCKTIM_sig"),
            timerB_TCNTB6CM_sig("timerB_TCNTB6CM_sig"),
            timerB_TCNTDCLR_sig("timerB_TCNTDCLR_sig"),
            CLK_LSBperiod_sig("CLK_LSBperiod_sig"),
            nc_clk_sig("nc_clk_sig")
{//{{{
    //Bind clock, reset signal and socket to Bus IF
    setSlaveFreqPort32(&CLK_HSB);
    setSlaveResetPort32(&presetz_bif);
    setTargetSocket32((char*)"ts");
    ts = BusSlaveBase<32,1>::tSocket32[0]; //Alias for binding

    mATU5011_Func = new ATU5011_Func((std::string)name, this);
    sc_assert(mATU5011_Func != NULL);
    mBusSlaveIf32[0]->setFuncModulePtr(mATU5011_Func);
    mBusSlaveIf32[0]->setBusProtocol(BUS_APB);
    mBusSlaveIf32[0]->setReadInitLatency(rLatency);
    mBusSlaveIf32[0]->setWriteInitLatency(wLatency);
    mBusSlaveIf32[0]->setTransNmbLmt(0x1);

    mTimeResolutionValue = 1;
    mTimeResolutionUnit = SC_NS;
    GetTimeResolution(mTimeResolutionValue, mTimeResolutionUnit);
    
    mATU5_TimerA = new ATU5_TIMERA("ATU5011_TIMERA", this);
    sc_assert(mATU5_TimerA != NULL);
    mATU5_TimerB = new ATU5_TIMERB("ATU5011_TIMERB", this);
    sc_assert(mATU5_TimerB != NULL);
    mATU5_TimerC = new ATU5_TIMERC("ATU5011_TIMERC", this , emTimerC_SUBnum);
    sc_assert(mATU5_TimerC != NULL);
    mATU5_TimerD = new ATU5_TIMERD("ATU5011_TIMERD", this, emTimerD_SUBnum);
    sc_assert(mATU5_TimerD != NULL);
    mATU5_TimerE = new ATU5_TIMERE("ATU5011_TIMERE", this, emTimerE_SUBnum);
    sc_assert(mATU5_TimerE != NULL);
    mATU5_TimerF = new ATU5_TIMERF("ATU5011_TIMERF", this , emTimerF_SUBnum);
    sc_assert(mATU5_TimerF != NULL);
    mATU5_TimerG = new ATU5_TIMERG("ATU5011_TIMERG");
    sc_assert(mATU5_TimerG != NULL);
    
    std::ostringstream port_name;
    for (unsigned int index = 0; index < emTimerA_TILOnum; index++) {
        port_name.str("");
        port_name << "timerA_TILOA_sig" << index;
        timerA_TILOA_sig[index] = new SC_SIGNAL(bool)(port_name.str().c_str());
        sc_assert(timerA_TILOA_sig[index] != NULL);
    }
    for (unsigned int index = 0; index < emTimerEF_POEnum; index++) {
        port_name.str("");
        port_name << "timerF_POE_sig" << index;
        timerF_POE_sig[index] = new SC_SIGNAL(bool)(port_name.str().c_str());
        sc_assert(timerF_POE_sig[index] != NULL);
    }
    for (unsigned int index = 0; index < emCLK_BUSnum; index++) {
        port_name.str("");
        port_name << "clk_bus_sig" << index;
        clk_bus_sig[index] = new sc_signal<sc_dt::uint64>(port_name.str().c_str());
        sc_assert(clk_bus_sig[index] != NULL);
    }
    //Initialize output ports
    TIOC00.initialize(false);
    TIOC01.initialize(false);
    TIOC02.initialize(false);
    TIOC03.initialize(false);
    TIOC10.initialize(false);
    TIOC11.initialize(false);
    TIOC12.initialize(false);
    TIOC13.initialize(false);
    TIOC20.initialize(false);
    TIOC21.initialize(false);
    TIOC22.initialize(false);
    TIOC23.initialize(false);
    TIOC30.initialize(false);
    TIOC31.initialize(false);
    TIOC32.initialize(false);
    TIOC33.initialize(false);
    TIOC40.initialize(false);
    TIOC41.initialize(false);
    TIOC42.initialize(false);
    TIOC43.initialize(false);
    TIOC50.initialize(false);
    TIOC51.initialize(false);
    TIOC52.initialize(false);
    TIOC53.initialize(false);
    TIOC60.initialize(false);
    TIOC61.initialize(false);
    TIOC62.initialize(false);
    TIOC63.initialize(false);
    TIOC70.initialize(false);
    TIOC71.initialize(false);
    TIOC72.initialize(false);
    TIOC73.initialize(false);
    TIOC80.initialize(false);
    TIOC81.initialize(false);
    TIOC82.initialize(false);
    TIOC83.initialize(false);
    TIOC90.initialize(false);
    TIOC91.initialize(false);
    TIOC92.initialize(false);
    TIOC93.initialize(false);
    TIOC100.initialize(false);
    TIOC101.initialize(false);
    TIOC102.initialize(false);
    TIOC103.initialize(false);
    TIOC110.initialize(false);
    TIOC111.initialize(false);
    TIOC112.initialize(false);
    TIOC113.initialize(false);
    TIOC120.initialize(false);
    TIOC121.initialize(false);
    TIOC122.initialize(false);
    TIOC123.initialize(false);
    TIOC130.initialize(false);
    TIOC131.initialize(false);
    TIOC132.initialize(false);
    TIOC133.initialize(false);
    
    TOCE00.initialize(false);
    TOCE01.initialize(false);
    TOCE02.initialize(false);
    TOCE03.initialize(false);
    TOCE10.initialize(false);
    TOCE11.initialize(false);
    TOCE12.initialize(false);
    TOCE13.initialize(false);
    TOCE20.initialize(false);
    TOCE21.initialize(false);
    TOCE22.initialize(false);
    TOCE23.initialize(false);
    TOCE30.initialize(false);
    TOCE31.initialize(false);
    TOCE32.initialize(false);
    TOCE33.initialize(false);
    TOCE40.initialize(false);
    TOCE41.initialize(false);
    TOCE42.initialize(false);
    TOCE43.initialize(false);
    TOCE50.initialize(false);
    TOCE51.initialize(false);
    TOCE52.initialize(false);
    TOCE53.initialize(false);
    TOCE60.initialize(false);
    TOCE61.initialize(false);
    TOCE62.initialize(false);
    TOCE63.initialize(false);
    TOCE70.initialize(false);
    TOCE71.initialize(false);
    TOCE72.initialize(false);
    TOCE73.initialize(false);
    TOCE80.initialize(false);
    TOCE81.initialize(false);
    TOCE82.initialize(false);
    TOCE83.initialize(false);
    TOCE90.initialize(false);
    TOCE91.initialize(false);
    TOCE92.initialize(false);
    TOCE93.initialize(false);
    TOCE100.initialize(false);
    TOCE101.initialize(false);
    TOCE102.initialize(false);
    TOCE103.initialize(false);
    TOCE110.initialize(false);
    TOCE111.initialize(false);
    TOCE112.initialize(false);
    TOCE113.initialize(false);
    TOCE120.initialize(false);
    TOCE121.initialize(false);
    TOCE122.initialize(false);
    TOCE123.initialize(false);
    TOCE130.initialize(false);
    TOCE131.initialize(false);
    TOCE132.initialize(false);
    TOCE133.initialize(false);
    // TOCxy
    TOC00.initialize(false);
    TOC01.initialize(false);
    TOC02.initialize(false);
    TOC03.initialize(false);
    TOC10.initialize(false);
    TOC11.initialize(false);
    TOC12.initialize(false);
    TOC13.initialize(false);
    TOC20.initialize(false);
    TOC21.initialize(false);
    TOC22.initialize(false);
    TOC23.initialize(false);
    TOC30.initialize(false);
    TOC31.initialize(false);
    // TimerD
    // TODxyA
    TOD00A.initialize(false);
    TOD01A.initialize(false);
    TOD02A.initialize(false);
    TOD03A.initialize(false);
    TOD10A.initialize(false);
    TOD11A.initialize(false);
    TOD12A.initialize(false);
    TOD13A.initialize(false);
    TOD20A.initialize(false);
    TOD21A.initialize(false);
    TOD22A.initialize(false);
    TOD23A.initialize(false);
    TOD30A.initialize(false);
    TOD31A.initialize(false);
    TOD32A.initialize(false);
    TOD33A.initialize(false);
    TOD40A.initialize(false);
    TOD41A.initialize(false);
    TOD42A.initialize(false);
    TOD43A.initialize(false);
    TOD50A.initialize(false);
    TOD51A.initialize(false);
    TOD52A.initialize(false);
    TOD53A.initialize(false);
    TOD60A.initialize(false);
    TOD61A.initialize(false);
    TOD62A.initialize(false);
    TOD63A.initialize(false);
    TOD70A.initialize(false);
    TOD71A.initialize(false);
    TOD72A.initialize(false);
    TOD73A.initialize(false);
    TOD80A.initialize(false);
    TOD81A.initialize(false);
    TOD82A.initialize(false);
    TOD83A.initialize(false);
    TOD90A.initialize(false);
    TOD91A.initialize(false);
    TOD92A.initialize(false);
    TOD93A.initialize(false);
    TOD100A.initialize(false);
    TOD101A.initialize(false);
    TOD102A.initialize(false);
    TOD103A.initialize(false);
    TOD110A.initialize(false);
    TOD111A.initialize(false);
    TOD112A.initialize(false);
    TOD113A.initialize(false);
    TOD120A.initialize(false);
    TOD121A.initialize(false);
    TOD122A.initialize(false);
    TOD123A.initialize(false);
    TOD130A.initialize(false);
    TOD131A.initialize(false);
    TOD132A.initialize(false);
    TOD133A.initialize(false);
    TOD140A.initialize(false);
    TOD141A.initialize(false);
    TOD142A.initialize(false);
    TOD143A.initialize(false);

    // TODxyB
    TOD00B.initialize(false);
    TOD01B.initialize(false);
    TOD02B.initialize(false);
    TOD03B.initialize(false);
    TOD10B.initialize(false);
    TOD11B.initialize(false);
    TOD12B.initialize(false);
    TOD13B.initialize(false);
    TOD20B.initialize(false);
    TOD21B.initialize(false);
    TOD22B.initialize(false);
    TOD23B.initialize(false);
    TOD30B.initialize(false);
    TOD31B.initialize(false);
    TOD32B.initialize(false);
    TOD33B.initialize(false);
    TOD40B.initialize(false);
    TOD41B.initialize(false);
    TOD42B.initialize(false);
    TOD43B.initialize(false);
    TOD50B.initialize(false);
    TOD51B.initialize(false);
    TOD52B.initialize(false);
    TOD53B.initialize(false);
    TOD60B.initialize(false);
    TOD61B.initialize(false);
    TOD62B.initialize(false);
    TOD63B.initialize(false);
    TOD70B.initialize(false);
    TOD71B.initialize(false);
    TOD72B.initialize(false);
    TOD73B.initialize(false);
    TOD80B.initialize(false);
    TOD81B.initialize(false);
    TOD82B.initialize(false);
    TOD83B.initialize(false);
    TOD90B.initialize(false);
    TOD91B.initialize(false);
    TOD92B.initialize(false);
    TOD93B.initialize(false);
    TOD100B.initialize(false);
    TOD101B.initialize(false);
    TOD102B.initialize(false);
    TOD103B.initialize(false);
    TOD110B.initialize(false);
    TOD111B.initialize(false);
    TOD112B.initialize(false);
    TOD113B.initialize(false);
    TOD120B.initialize(false);
    TOD121B.initialize(false);
    TOD122B.initialize(false);
    TOD123B.initialize(false);
    TOD130B.initialize(false);
    TOD131B.initialize(false);
    TOD132B.initialize(false);
    TOD133B.initialize(false);
    TOD140B.initialize(false);
    TOD141B.initialize(false);
    TOD142B.initialize(false);
    TOD143B.initialize(false);

    // TODMIGxy
    TODMIG00.initialize(false);
    TODMIG01.initialize(false);
    TODMIG02.initialize(false);
    TODMIG03.initialize(false);
    TODMIG10.initialize(false);
    TODMIG11.initialize(false);
    TODMIG12.initialize(false);
    TODMIG13.initialize(false);
    TODMIG20.initialize(false);
    TODMIG21.initialize(false);
    TODMIG22.initialize(false);
    TODMIG23.initialize(false);
    TODMIG30.initialize(false);
    TODMIG31.initialize(false);
    TODMIG32.initialize(false);
    TODMIG33.initialize(false);
    TODMIG40.initialize(false);
    TODMIG41.initialize(false);
    TODMIG42.initialize(false);
    TODMIG43.initialize(false);
    TODMIG50.initialize(false);
    TODMIG51.initialize(false);
    TODMIG52.initialize(false);
    TODMIG53.initialize(false);
    TODMIG60.initialize(false);
    TODMIG61.initialize(false);
    TODMIG62.initialize(false);
    TODMIG63.initialize(false);
    TODMIG70.initialize(false);
    TODMIG71.initialize(false);
    TODMIG72.initialize(false);
    TODMIG73.initialize(false);
    TODMIG80.initialize(false);
    TODMIG81.initialize(false);
    TODMIG82.initialize(false);
    TODMIG83.initialize(false);
    TODMIG90.initialize(false);
    TODMIG91.initialize(false);
    TODMIG92.initialize(false);
    TODMIG93.initialize(false);
    TODMIG100.initialize(false);
    TODMIG101.initialize(false);
    TODMIG102.initialize(false);
    TODMIG103.initialize(false);
    TODMIG110.initialize(false);
    TODMIG111.initialize(false);
    TODMIG112.initialize(false);
    TODMIG113.initialize(false);
    TODMIG120.initialize(false);
    TODMIG121.initialize(false);
    TODMIG122.initialize(false);
    TODMIG123.initialize(false);
    TODMIG130.initialize(false);
    TODMIG131.initialize(false);
    TODMIG132.initialize(false);
    TODMIG133.initialize(false);
    TODMIG140.initialize(false);
    TODMIG141.initialize(false);
    TODMIG142.initialize(false);
    TODMIG143.initialize(false);

    // TimerE
    TOE00.initialize(false);
    TOE01.initialize(false);
    TOE02.initialize(false);
    TOE03.initialize(false);
    TOE10.initialize(false);
    TOE11.initialize(false);
    TOE12.initialize(false);
    TOE13.initialize(false);
    TOE20.initialize(false);
    TOE21.initialize(false);
    TOE22.initialize(false);
    TOE23.initialize(false);
    TOE30.initialize(false);
    TOE31.initialize(false);
    TOE32.initialize(false);
    TOE33.initialize(false);
    TOE40.initialize(false);
    TOE41.initialize(false);
    TOE42.initialize(false);
    TOE43.initialize(false);
    TOE50.initialize(false);
    TOE51.initialize(false);
    TOE52.initialize(false);
    TOE53.initialize(false);
    TOE60.initialize(false);
    TOE61.initialize(false);
    TOE62.initialize(false);
    TOE63.initialize(false);
    TOE70.initialize(false);
    TOE71.initialize(false);
    TOE72.initialize(false);
    TOE73.initialize(false);
    TOE80.initialize(false);
    TOE81.initialize(false);
    TOE82.initialize(false);
    TOE83.initialize(false);
    TOE90.initialize(false);
    TOE91.initialize(false);
    TOE92.initialize(false);
    TOE93.initialize(false);
    
    // Interrupts
    // TimerA
    INTATUAICIA0.initialize(false);
    INTATUAICIA1.initialize(false);
    INTATUAICIA2.initialize(false);
    INTATUAICIA3.initialize(false);
    INTATUAICIA4.initialize(false);
    INTATUAICIA5.initialize(false);
    INTATUAICIA6.initialize(false);
    INTATUAICIA7.initialize(false);
    // OVIA
    OVIA.initialize(false);
    // TimerB
    INTATUBCMIB0.initialize(false);
    INTATUBCMIB1.initialize(false);
    INTATUBCMIB6.initialize(false);
    INTATUBCMIB10.initialize(false);
    INTATUBCMIB11.initialize(false);
    INTATUBCMIB12.initialize(false);
    INTATUBCMIB6M.initialize(false);
    INTATUBCMIB66M.initialize(false);
    INTATUBICIB0.initialize(false);
    // TimerC
    // OVICx
    OVIC0.initialize(false);
    OVIC1.initialize(false);
    OVIC2.initialize(false);
    OVIC3.initialize(false);
    OVIC4.initialize(false);
    OVIC5.initialize(false);
    OVIC6.initialize(false);
    OVIC7.initialize(false);
    OVIC8.initialize(false);
    OVIC9.initialize(false);
    OVIC10.initialize(false);
    OVIC11.initialize(false);
    OVIC12.initialize(false);
    OVIC13.initialize(false);
    // IMIGCxy
    IMIGC00.initialize(false);
    IMIGC01.initialize(false);
    IMIGC02.initialize(false);
    IMIGC03.initialize(false);
    IMIGC10.initialize(false);
    IMIGC11.initialize(false);
    IMIGC12.initialize(false);
    IMIGC13.initialize(false);
    IMIGC20.initialize(false);
    IMIGC21.initialize(false);
    IMIGC22.initialize(false);
    IMIGC23.initialize(false);
    IMIGC30.initialize(false);
    IMIGC31.initialize(false);
    IMIGC32.initialize(false);
    IMIGC33.initialize(false);
    IMIGC40.initialize(false);
    IMIGC41.initialize(false);
    IMIGC42.initialize(false);
    IMIGC43.initialize(false);
    IMIGC50.initialize(false);
    IMIGC51.initialize(false);
    IMIGC52.initialize(false);
    IMIGC53.initialize(false);
    IMIGC60.initialize(false);
    IMIGC61.initialize(false);
    IMIGC62.initialize(false);
    IMIGC63.initialize(false);
    IMIGC70.initialize(false);
    IMIGC71.initialize(false);
    IMIGC72.initialize(false);
    IMIGC73.initialize(false);
    IMIGC80.initialize(false);
    IMIGC81.initialize(false);
    IMIGC82.initialize(false);
    IMIGC83.initialize(false);
    IMIGC90.initialize(false);
    IMIGC91.initialize(false);
    IMIGC92.initialize(false);
    IMIGC93.initialize(false);
    IMIGC100.initialize(false);
    IMIGC101.initialize(false);
    IMIGC102.initialize(false);
    IMIGC103.initialize(false);
    IMIGC110.initialize(false);
    IMIGC111.initialize(false);
    IMIGC112.initialize(false);
    IMIGC113.initialize(false);
    IMIGC120.initialize(false);
    IMIGC121.initialize(false);
    IMIGC122.initialize(false);
    IMIGC123.initialize(false);
    IMIGC130.initialize(false);
    IMIGC131.initialize(false);
    IMIGC132.initialize(false);
    IMIGC133.initialize(false);
    // IMIOCxy
    IMIOC00.initialize(false);
    IMIOC01.initialize(false);
    IMIOC02.initialize(false);
    IMIOC03.initialize(false);
    IMIOC10.initialize(false);
    IMIOC11.initialize(false);
    IMIOC12.initialize(false);
    IMIOC13.initialize(false);
    IMIOC20.initialize(false);
    IMIOC21.initialize(false);
    IMIOC22.initialize(false);
    IMIOC23.initialize(false);
    IMIOC30.initialize(false);
    IMIOC31.initialize(false);
    IMIOC32.initialize(false);
    IMIOC33.initialize(false);
    IMIOC40.initialize(false);
    IMIOC41.initialize(false);
    IMIOC42.initialize(false);
    IMIOC43.initialize(false);
    IMIOC50.initialize(false);
    IMIOC51.initialize(false);
    IMIOC52.initialize(false);
    IMIOC53.initialize(false);
    IMIOC60.initialize(false);
    IMIOC61.initialize(false);
    IMIOC62.initialize(false);
    IMIOC63.initialize(false);
    IMIOC70.initialize(false);
    IMIOC71.initialize(false);
    IMIOC72.initialize(false);
    IMIOC73.initialize(false);
    IMIOC80.initialize(false);
    IMIOC81.initialize(false);
    IMIOC82.initialize(false);
    IMIOC83.initialize(false);
    IMIOC90.initialize(false);
    IMIOC91.initialize(false);
    IMIOC92.initialize(false);
    IMIOC93.initialize(false);
    IMIOC100.initialize(false);
    IMIOC101.initialize(false);
    IMIOC102.initialize(false);
    IMIOC103.initialize(false);
    IMIOC110.initialize(false);
    IMIOC111.initialize(false);
    IMIOC112.initialize(false);
    IMIOC113.initialize(false);
    IMIOC120.initialize(false);
    IMIOC121.initialize(false);
    IMIOC122.initialize(false);
    IMIOC123.initialize(false);
    IMIOC130.initialize(false);
    IMIOC131.initialize(false);
    IMIOC132.initialize(false);
    IMIOC133.initialize(false);
    // TimerD
    // CMIADxy
    CMIAD00.initialize(false);
    CMIAD01.initialize(false);
    CMIAD02.initialize(false);
    CMIAD03.initialize(false);
    CMIAD10.initialize(false);
    CMIAD11.initialize(false);
    CMIAD12.initialize(false);
    CMIAD13.initialize(false);
    CMIAD20.initialize(false);
    CMIAD21.initialize(false);
    CMIAD22.initialize(false);
    CMIAD23.initialize(false);
    CMIAD30.initialize(false);
    CMIAD31.initialize(false);
    CMIAD32.initialize(false);
    CMIAD33.initialize(false);
    CMIAD40.initialize(false);
    CMIAD41.initialize(false);
    CMIAD42.initialize(false);
    CMIAD43.initialize(false);
    CMIAD50.initialize(false);
    CMIAD51.initialize(false);
    CMIAD52.initialize(false);
    CMIAD53.initialize(false);
    CMIAD60.initialize(false);
    CMIAD61.initialize(false);
    CMIAD62.initialize(false);
    CMIAD63.initialize(false);
    CMIAD70.initialize(false);
    CMIAD71.initialize(false);
    CMIAD72.initialize(false);
    CMIAD73.initialize(false);
    CMIAD80.initialize(false);
    CMIAD81.initialize(false);
    CMIAD82.initialize(false);
    CMIAD83.initialize(false);
    CMIAD90.initialize(false);
    CMIAD91.initialize(false);
    CMIAD92.initialize(false);
    CMIAD93.initialize(false);
    CMIAD100.initialize(false);
    CMIAD101.initialize(false);
    CMIAD102.initialize(false);
    CMIAD103.initialize(false);
    CMIAD110.initialize(false);
    CMIAD111.initialize(false);
    CMIAD112.initialize(false);
    CMIAD113.initialize(false);
    CMIAD120.initialize(false);
    CMIAD121.initialize(false);
    CMIAD122.initialize(false);
    CMIAD123.initialize(false);
    CMIAD130.initialize(false);
    CMIAD131.initialize(false);
    CMIAD132.initialize(false);
    CMIAD133.initialize(false);
    CMIAD140.initialize(false);
    CMIAD141.initialize(false);
    CMIAD142.initialize(false);
    CMIAD143.initialize(false);

    // CMIBDxy
    CMIBD00.initialize(false);
    CMIBD01.initialize(false);
    CMIBD02.initialize(false);
    CMIBD03.initialize(false);
    CMIBD10.initialize(false);
    CMIBD11.initialize(false);
    CMIBD12.initialize(false);
    CMIBD13.initialize(false);
    CMIBD20.initialize(false);
    CMIBD21.initialize(false);
    CMIBD22.initialize(false);
    CMIBD23.initialize(false);
    CMIBD30.initialize(false);
    CMIBD31.initialize(false);
    CMIBD32.initialize(false);
    CMIBD33.initialize(false);
    CMIBD40.initialize(false);
    CMIBD41.initialize(false);
    CMIBD42.initialize(false);
    CMIBD43.initialize(false);
    CMIBD50.initialize(false);
    CMIBD51.initialize(false);
    CMIBD52.initialize(false);
    CMIBD53.initialize(false);
    CMIBD60.initialize(false);
    CMIBD61.initialize(false);
    CMIBD62.initialize(false);
    CMIBD63.initialize(false);
    CMIBD70.initialize(false);
    CMIBD71.initialize(false);
    CMIBD72.initialize(false);
    CMIBD73.initialize(false);
    CMIBD80.initialize(false);
    CMIBD81.initialize(false);
    CMIBD82.initialize(false);
    CMIBD83.initialize(false);
    CMIBD90.initialize(false);
    CMIBD91.initialize(false);
    CMIBD92.initialize(false);
    CMIBD93.initialize(false);
    CMIBD100.initialize(false);
    CMIBD101.initialize(false);
    CMIBD102.initialize(false);
    CMIBD103.initialize(false);
    CMIBD110.initialize(false);
    CMIBD111.initialize(false);
    CMIBD112.initialize(false);
    CMIBD113.initialize(false);
    CMIBD120.initialize(false);
    CMIBD121.initialize(false);
    CMIBD122.initialize(false);
    CMIBD123.initialize(false);
    CMIBD130.initialize(false);
    CMIBD131.initialize(false);
    CMIBD132.initialize(false);
    CMIBD133.initialize(false);
    CMIBD140.initialize(false);
    CMIBD141.initialize(false);
    CMIBD142.initialize(false);
    CMIBD143.initialize(false);

    // OVI1Dx
    OVI1D0.initialize(false);
    OVI1D1.initialize(false);
    OVI1D2.initialize(false);
    OVI1D3.initialize(false);
    OVI1D4.initialize(false);
    OVI1D5.initialize(false);
    OVI1D6.initialize(false);
    OVI1D7.initialize(false);
    OVI1D8.initialize(false);
    OVI1D9.initialize(false);
    OVI1D10.initialize(false);
    OVI1D11.initialize(false);
    OVI1D12.initialize(false);
    OVI1D13.initialize(false);
    OVI1D14.initialize(false);

    // OVI2Dx
    OVI2D0.initialize(false);
    OVI2D1.initialize(false);
    OVI2D2.initialize(false);
    OVI2D3.initialize(false);
    OVI2D4.initialize(false);
    OVI2D5.initialize(false);
    OVI2D6.initialize(false);
    OVI2D7.initialize(false);
    OVI2D8.initialize(false);
    OVI2D9.initialize(false);
    OVI2D10.initialize(false);
    OVI2D11.initialize(false);
    OVI2D12.initialize(false);
    OVI2D13.initialize(false);
    OVI2D14.initialize(false);
    // UDIDxy
    UDID00.initialize(false);
    UDID01.initialize(false);
    UDID02.initialize(false);
    UDID03.initialize(false);
    UDID10.initialize(false);
    UDID11.initialize(false);
    UDID12.initialize(false);
    UDID13.initialize(false);
    UDID20.initialize(false);
    UDID21.initialize(false);
    UDID22.initialize(false);
    UDID23.initialize(false);
    UDID30.initialize(false);
    UDID31.initialize(false);
    UDID32.initialize(false);
    UDID33.initialize(false);
    UDID40.initialize(false);
    UDID41.initialize(false);
    UDID42.initialize(false);
    UDID43.initialize(false);
    UDID50.initialize(false);
    UDID51.initialize(false);
    UDID52.initialize(false);
    UDID53.initialize(false);
    UDID60.initialize(false);
    UDID61.initialize(false);
    UDID62.initialize(false);
    UDID63.initialize(false);
    UDID70.initialize(false);
    UDID71.initialize(false);
    UDID72.initialize(false);
    UDID73.initialize(false);
    UDID80.initialize(false);
    UDID81.initialize(false);
    UDID82.initialize(false);
    UDID83.initialize(false);
    UDID90.initialize(false);
    UDID91.initialize(false);
    UDID92.initialize(false);
    UDID93.initialize(false);
    UDID100.initialize(false);
    UDID101.initialize(false);
    UDID102.initialize(false);
    UDID103.initialize(false);
    UDID110.initialize(false);
    UDID111.initialize(false);
    UDID112.initialize(false);
    UDID113.initialize(false);
    UDID120.initialize(false);
    UDID121.initialize(false);
    UDID122.initialize(false);
    UDID123.initialize(false);
    UDID130.initialize(false);
    UDID131.initialize(false);
    UDID132.initialize(false);
    UDID133.initialize(false);
    UDID140.initialize(false);
    UDID141.initialize(false);
    UDID142.initialize(false);
    UDID143.initialize(false);
    // PREIDxy
    PREID00.initialize(false);
    PREID01.initialize(false);
    PREID02.initialize(false);
    PREID03.initialize(false);
    PREID10.initialize(false);
    PREID11.initialize(false);
    PREID12.initialize(false);
    PREID13.initialize(false);
    PREID20.initialize(false);
    PREID21.initialize(false);
    PREID22.initialize(false);
    PREID23.initialize(false);
    PREID30.initialize(false);
    PREID31.initialize(false);
    PREID32.initialize(false);
    PREID33.initialize(false);
    PREID40.initialize(false);
    PREID41.initialize(false);
    PREID42.initialize(false);
    PREID43.initialize(false);
    PREID50.initialize(false);
    PREID51.initialize(false);
    PREID52.initialize(false);
    PREID53.initialize(false);
    PREID60.initialize(false);
    PREID61.initialize(false);
    PREID62.initialize(false);
    PREID63.initialize(false);
    PREID70.initialize(false);
    PREID71.initialize(false);
    PREID72.initialize(false);
    PREID73.initialize(false);
    PREID80.initialize(false);
    PREID81.initialize(false);
    PREID82.initialize(false);
    PREID83.initialize(false);
    PREID90.initialize(false);
    PREID91.initialize(false);
    PREID92.initialize(false);
    PREID93.initialize(false);
    PREID100.initialize(false);
    PREID101.initialize(false);
    PREID102.initialize(false);
    PREID103.initialize(false);
    PREID110.initialize(false);
    PREID111.initialize(false);
    PREID112.initialize(false);
    PREID113.initialize(false);
    PREID120.initialize(false);
    PREID121.initialize(false);
    PREID122.initialize(false);
    PREID123.initialize(false);
    PREID130.initialize(false);
    PREID131.initialize(false);
    PREID132.initialize(false);
    PREID133.initialize(false);
    PREID140.initialize(false);
    PREID141.initialize(false);
    PREID142.initialize(false);
    PREID143.initialize(false);
    // PFEIDxy
    PFEID00.initialize(false);
    PFEID01.initialize(false);
    PFEID02.initialize(false);
    PFEID03.initialize(false);
    PFEID10.initialize(false);
    PFEID11.initialize(false);
    PFEID12.initialize(false);
    PFEID13.initialize(false);
    PFEID20.initialize(false);
    PFEID21.initialize(false);
    PFEID22.initialize(false);
    PFEID23.initialize(false);
    PFEID30.initialize(false);
    PFEID31.initialize(false);
    PFEID32.initialize(false);
    PFEID33.initialize(false);
    PFEID40.initialize(false);
    PFEID41.initialize(false);
    PFEID42.initialize(false);
    PFEID43.initialize(false);
    PFEID50.initialize(false);
    PFEID51.initialize(false);
    PFEID52.initialize(false);
    PFEID53.initialize(false);
    PFEID60.initialize(false);
    PFEID61.initialize(false);
    PFEID62.initialize(false);
    PFEID63.initialize(false);
    PFEID70.initialize(false);
    PFEID71.initialize(false);
    PFEID72.initialize(false);
    PFEID73.initialize(false);
    PFEID80.initialize(false);
    PFEID81.initialize(false);
    PFEID82.initialize(false);
    PFEID83.initialize(false);
    PFEID90.initialize(false);
    PFEID91.initialize(false);
    PFEID92.initialize(false);
    PFEID93.initialize(false);
    PFEID100.initialize(false);
    PFEID101.initialize(false);
    PFEID102.initialize(false);
    PFEID103.initialize(false);
    PFEID110.initialize(false);
    PFEID111.initialize(false);
    PFEID112.initialize(false);
    PFEID113.initialize(false);
    PFEID120.initialize(false);
    PFEID121.initialize(false);
    PFEID122.initialize(false);
    PFEID123.initialize(false);
    PFEID130.initialize(false);
    PFEID131.initialize(false);
    PFEID132.initialize(false);
    PFEID133.initialize(false);
    PFEID140.initialize(false);
    PFEID141.initialize(false);
    PFEID142.initialize(false);
    PFEID143.initialize(false);

    // TimerE
    INTATUECMIE00.initialize(false);
    INTATUECMIE01.initialize(false);
    INTATUECMIE02.initialize(false);
    INTATUECMIE03.initialize(false);
    INTATUECMIE10.initialize(false);
    INTATUECMIE11.initialize(false);
    INTATUECMIE12.initialize(false);
    INTATUECMIE13.initialize(false);
    INTATUECMIE20.initialize(false);
    INTATUECMIE21.initialize(false);
    INTATUECMIE22.initialize(false);
    INTATUECMIE23.initialize(false);
    INTATUECMIE30.initialize(false);
    INTATUECMIE31.initialize(false);
    INTATUECMIE32.initialize(false);
    INTATUECMIE33.initialize(false);
    INTATUECMIE40.initialize(false);
    INTATUECMIE41.initialize(false);
    INTATUECMIE42.initialize(false);
    INTATUECMIE43.initialize(false);
    INTATUECMIE50.initialize(false);
    INTATUECMIE51.initialize(false);
    INTATUECMIE52.initialize(false);
    INTATUECMIE53.initialize(false);
    INTATUECMIE60.initialize(false);
    INTATUECMIE61.initialize(false);
    INTATUECMIE62.initialize(false);
    INTATUECMIE63.initialize(false);
    INTATUECMIE70.initialize(false);
    INTATUECMIE71.initialize(false);
    INTATUECMIE72.initialize(false);
    INTATUECMIE73.initialize(false);
    INTATUECMIE80.initialize(false);
    INTATUECMIE81.initialize(false);
    INTATUECMIE82.initialize(false);
    INTATUECMIE83.initialize(false);
    INTATUECMIE90.initialize(false);
    INTATUECMIE91.initialize(false);
    INTATUECMIE92.initialize(false);
    INTATUECMIE93.initialize(false);

    // TimerF
    INTATUFICIF0.initialize(false);
    INTATUFICIF1.initialize(false);
    INTATUFICIF2.initialize(false);
    INTATUFICIF3.initialize(false);
    INTATUFICIF4.initialize(false);
    INTATUFICIF5.initialize(false);
    INTATUFICIF6.initialize(false);
    INTATUFICIF7.initialize(false);
    INTATUFICIF8.initialize(false);
    INTATUFICIF9.initialize(false);
    INTATUFICIF10.initialize(false);
    INTATUFICIF11.initialize(false);
    INTATUFICIF12.initialize(false);
    INTATUFICIF13.initialize(false);
    INTATUFICIF14.initialize(false);
    INTATUFICIF15.initialize(false);
    INTATUFICIF16.initialize(false);
    INTATUFICIF17.initialize(false);
    INTATUFICIF18.initialize(false);
    INTATUFICIF19.initialize(false);

    // OVIFx
    OVIF0.initialize(false);
    OVIF1.initialize(false);
    OVIF2.initialize(false);
    OVIF3.initialize(false);
    OVIF4.initialize(false);
    OVIF5.initialize(false);
    OVIF6.initialize(false);
    OVIF7.initialize(false);
    OVIF8.initialize(false);
    OVIF9.initialize(false);
    OVIF10.initialize(false);
    OVIF11.initialize(false);
    OVIF12.initialize(false);
    OVIF13.initialize(false);
    OVIF14.initialize(false);
    OVIF15.initialize(false);
    OVIF16.initialize(false);
    OVIF17.initialize(false);
    OVIF18.initialize(false);
    OVIF19.initialize(false);
    // TimerG
    INTATUGCMIG0.initialize(false);
    INTATUGCMIG1.initialize(false);
    INTATUGCMIG2.initialize(false);
    INTATUGCMIG3.initialize(false);
    INTATUGCMIG4.initialize(false);
    INTATUGCMIG5.initialize(false);
    INTATUGCMIG6.initialize(false);
    INTATUGCMIG7.initialize(false);
    INTATUGCMIG8.initialize(false);
    INTATUGCMIG9.initialize(false);
    /// Port connection
    // TImerA
    (*(mATU5_TimerA->TIA[0]))(TIA0);
    (*(mATU5_TimerA->TIA[1]))(TIA1);
    (*(mATU5_TimerA->TIA[2]))(TIA2);
    (*(mATU5_TimerA->TIA[3]))(TIA3);
    (*(mATU5_TimerA->TIA[4]))(TIA4);
    (*(mATU5_TimerA->TIA[5]))(TIA5);
    (*(mATU5_TimerA->TIA[6]))(TIA6);
    (*(mATU5_TimerA->TIA[7]))(TIA7);
    (*(mATU5_TimerA->ICIA[0]))(INTATUAICIA0);
    (*(mATU5_TimerA->ICIA[1]))(INTATUAICIA1);
    (*(mATU5_TimerA->ICIA[2]))(INTATUAICIA2);
    (*(mATU5_TimerA->ICIA[3]))(INTATUAICIA3);
    (*(mATU5_TimerA->ICIA[4]))(INTATUAICIA4);
    (*(mATU5_TimerA->ICIA[5]))(INTATUAICIA5);
    (*(mATU5_TimerA->ICIA[6]))(INTATUAICIA6);
    (*(mATU5_TimerA->ICIA[7]))(INTATUAICIA7);
    // TimerA - Internal
    (mATU5_TimerA->clkbus_0)(*(clk_bus_sig[0]));
    (mATU5_TimerA->clkbus_1)(*(clk_bus_sig[1]));
    (mATU5_TimerA->clkbus_2)(*(clk_bus_sig[2]));
    (mATU5_TimerA->clkbus_3)(*(clk_bus_sig[3]));
    (mATU5_TimerA->clkbus_4)(*(clk_bus_sig[4]));
    (mATU5_TimerA->clkbus_5)(*(clk_bus_sig[5]));
    (mATU5_TimerA->nc_clk_1)(nc_clk_sig);
    (mATU5_TimerA->nc_clk_2)(timerB_AGCK1_sig);
    (mATU5_TimerA->TCNTB6CM)(timerB_TCNTB6CM_sig);
    (mATU5_TimerA->OVIA)(OVIA);
    (mATU5_TimerA->ev_out_1_0)(timerA_evt_1_0_sig);
    (mATU5_TimerA->ev_out_1_1)(timerA_evt_1_1_sig);
    (mATU5_TimerA->ev_out_1_2)(timerA_evt_1_2_sig);
    (mATU5_TimerA->ev_out_1B)(timerA_evt_1B_sig);
    (mATU5_TimerA->ev_out_1C)(timerA_evt_1C_sig);
    (mATU5_TimerA->ev_out_1D)(timerA_evt_1D_sig);
    (mATU5_TimerA->ev_out_1E)(timerA_evt_1E_sig);
    (mATU5_TimerA->ev_out_1F)(timerA_evt_1F_sig);
    (mATU5_TimerA->ev_out_1G)(timerA_evt_1G_sig);
    (mATU5_TimerA->ev_out_1H)(timerA_evt_1H_sig);
    (mATU5_TimerA->ev_out_1I)(timerA_evt_1I_sig);
    (mATU5_TimerA->ev_out_2A)(timerA_evt_2A_sig);
    (mATU5_TimerA->ev_out_2B)(timerA_evt_2B_sig);
    for (unsigned int i = 0; i < emTimerA_TILOnum; i++) {
        (*(mATU5_TimerA->TILOA[i]))(*(timerA_TILOA_sig[i]));
    }
    // TimerB
    (mATU5_TimerB->CMIB0)(INTATUBCMIB0);
    (mATU5_TimerB->CMIB1)(INTATUBCMIB1);
    (mATU5_TimerB->CMIB6)(INTATUBCMIB6);
    (mATU5_TimerB->CMIB10)(INTATUBCMIB10);
    (mATU5_TimerB->CMIB11)(INTATUBCMIB11);
    (mATU5_TimerB->CMIB12)(INTATUBCMIB12);
    (mATU5_TimerB->CMIB6M)(INTATUBCMIB6M);
    (mATU5_TimerB->CMIB6E)(INTATUBCMIB66M);
    (mATU5_TimerB->ICIB)(INTATUBICIB0);
    // TimerB - Internal
    (mATU5_TimerB->clkbus_0)(*(clk_bus_sig[0]));
    (mATU5_TimerB->clkbus_1)(*(clk_bus_sig[1]));
    (mATU5_TimerB->clkbus_2)(*(clk_bus_sig[2]));
    (mATU5_TimerB->clkbus_3)(*(clk_bus_sig[3]));
    (mATU5_TimerB->ev_in_1_0)(timerA_evt_1_0_sig);
    (mATU5_TimerB->ev_in_1_1)(timerA_evt_1_1_sig);
    (mATU5_TimerB->ev_in_1_2)(timerA_evt_1_2_sig);
    (mATU5_TimerB->ev_in_1B)(timerA_evt_1B_sig);
    (mATU5_TimerB->ev_in_1C)(timerA_evt_1C_sig);
    (mATU5_TimerB->ev_in_1D)(timerA_evt_1D_sig);
    (mATU5_TimerB->ev_in_1E)(timerA_evt_1E_sig);
    (mATU5_TimerB->ev_in_1F)(timerA_evt_1F_sig);
    (mATU5_TimerB->ev_in_1G)(timerA_evt_1G_sig);
    (mATU5_TimerB->ev_in_1H)(timerA_evt_1H_sig);
    (mATU5_TimerB->ev_in_1I)(timerA_evt_1I_sig);
    (mATU5_TimerB->AGCKM)(timerB_AGCKM_sig);
    (mATU5_TimerB->AGCK1)(timerB_AGCK1_sig);
    (mATU5_TimerB->AGCKTIM)(timerB_AGCKTIM_sig);
    (mATU5_TimerB->TCNTB6CM)(timerB_TCNTB6CM_sig);
    (mATU5_TimerB->TCNTDCLR)(timerB_TCNTDCLR_sig);
    // TimerC
    (mATU5_TimerC->PCLK)(CLK_LSBperiod_sig);
    (*(mATU5_TimerC->TIOC0[0]))(TIOC00);
    (*(mATU5_TimerC->TIOC1[0]))(TIOC01);
    (*(mATU5_TimerC->TIOC2[0]))(TIOC02);
    (*(mATU5_TimerC->TIOC3[0]))(TIOC03);
    (*(mATU5_TimerC->TIOC0[1]))(TIOC10);
    (*(mATU5_TimerC->TIOC1[1]))(TIOC11);
    (*(mATU5_TimerC->TIOC2[1]))(TIOC12);
    (*(mATU5_TimerC->TIOC3[1]))(TIOC13);
    (*(mATU5_TimerC->TIOC0[2]))(TIOC20);
    (*(mATU5_TimerC->TIOC1[2]))(TIOC21);
    (*(mATU5_TimerC->TIOC2[2]))(TIOC22);
    (*(mATU5_TimerC->TIOC3[2]))(TIOC23);
    (*(mATU5_TimerC->TIOC0[3]))(TIOC30);
    (*(mATU5_TimerC->TIOC1[3]))(TIOC31);
    (*(mATU5_TimerC->TIOC2[3]))(TIOC32);
    (*(mATU5_TimerC->TIOC3[3]))(TIOC33);
    (*(mATU5_TimerC->TIOC0[4]))(TIOC40);
    (*(mATU5_TimerC->TIOC1[4]))(TIOC41);
    (*(mATU5_TimerC->TIOC2[4]))(TIOC42);
    (*(mATU5_TimerC->TIOC3[4]))(TIOC43);
    (*(mATU5_TimerC->TIOC0[5]))(TIOC50);
    (*(mATU5_TimerC->TIOC1[5]))(TIOC51);
    (*(mATU5_TimerC->TIOC2[5]))(TIOC52);
    (*(mATU5_TimerC->TIOC3[5]))(TIOC53);
    (*(mATU5_TimerC->TIOC0[6]))(TIOC60);
    (*(mATU5_TimerC->TIOC1[6]))(TIOC61);
    (*(mATU5_TimerC->TIOC2[6]))(TIOC62);
    (*(mATU5_TimerC->TIOC3[6]))(TIOC63);
    (*(mATU5_TimerC->TIOC0[7]))(TIOC70);
    (*(mATU5_TimerC->TIOC1[7]))(TIOC71);
    (*(mATU5_TimerC->TIOC2[7]))(TIOC72);
    (*(mATU5_TimerC->TIOC3[7]))(TIOC73);
    (*(mATU5_TimerC->TIOC0[8]))(TIOC80);
    (*(mATU5_TimerC->TIOC1[8]))(TIOC81);
    (*(mATU5_TimerC->TIOC2[8]))(TIOC82);
    (*(mATU5_TimerC->TIOC3[8]))(TIOC83);
    (*(mATU5_TimerC->TIOC0[9]))(TIOC90);
    (*(mATU5_TimerC->TIOC1[9]))(TIOC91);
    (*(mATU5_TimerC->TIOC2[9]))(TIOC92);
    (*(mATU5_TimerC->TIOC3[9]))(TIOC93);
    (*(mATU5_TimerC->TIOC0[10]))(TIOC100);
    (*(mATU5_TimerC->TIOC1[10]))(TIOC101);
    (*(mATU5_TimerC->TIOC2[10]))(TIOC102);
    (*(mATU5_TimerC->TIOC3[10]))(TIOC103);
    (*(mATU5_TimerC->TIOC0[11]))(TIOC110);
    (*(mATU5_TimerC->TIOC1[11]))(TIOC111);
    (*(mATU5_TimerC->TIOC2[11]))(TIOC112);
    (*(mATU5_TimerC->TIOC3[11]))(TIOC113);
    (*(mATU5_TimerC->TIOC0[12]))(TIOC120);
    (*(mATU5_TimerC->TIOC1[12]))(TIOC121);
    (*(mATU5_TimerC->TIOC2[12]))(TIOC122);
    (*(mATU5_TimerC->TIOC3[12]))(TIOC123);
    (*(mATU5_TimerC->TIOC0[13]))(TIOC130);
    (*(mATU5_TimerC->TIOC1[13]))(TIOC131);
    (*(mATU5_TimerC->TIOC2[13]))(TIOC132);
    (*(mATU5_TimerC->TIOC3[13]))(TIOC133);
    // TOCExy
    (*(mATU5_TimerC->TOCE0[0]))(TOCE00);
    (*(mATU5_TimerC->TOCE1[0]))(TOCE01);
    (*(mATU5_TimerC->TOCE2[0]))(TOCE02);
    (*(mATU5_TimerC->TOCE3[0]))(TOCE03);
    (*(mATU5_TimerC->TOCE0[1]))(TOCE10);
    (*(mATU5_TimerC->TOCE1[1]))(TOCE11);
    (*(mATU5_TimerC->TOCE2[1]))(TOCE12);
    (*(mATU5_TimerC->TOCE3[1]))(TOCE13);
    (*(mATU5_TimerC->TOCE0[2]))(TOCE20);
    (*(mATU5_TimerC->TOCE1[2]))(TOCE21);
    (*(mATU5_TimerC->TOCE2[2]))(TOCE22);
    (*(mATU5_TimerC->TOCE3[2]))(TOCE23);
    (*(mATU5_TimerC->TOCE0[3]))(TOCE30);
    (*(mATU5_TimerC->TOCE1[3]))(TOCE31);
    (*(mATU5_TimerC->TOCE2[3]))(TOCE32);
    (*(mATU5_TimerC->TOCE3[3]))(TOCE33);
    (*(mATU5_TimerC->TOCE0[4]))(TOCE40);
    (*(mATU5_TimerC->TOCE1[4]))(TOCE41);
    (*(mATU5_TimerC->TOCE2[4]))(TOCE42);
    (*(mATU5_TimerC->TOCE3[4]))(TOCE43);
    (*(mATU5_TimerC->TOCE0[5]))(TOCE50);
    (*(mATU5_TimerC->TOCE1[5]))(TOCE51);
    (*(mATU5_TimerC->TOCE2[5]))(TOCE52);
    (*(mATU5_TimerC->TOCE3[5]))(TOCE53);
    (*(mATU5_TimerC->TOCE0[6]))(TOCE60);
    (*(mATU5_TimerC->TOCE1[6]))(TOCE61);
    (*(mATU5_TimerC->TOCE2[6]))(TOCE62);
    (*(mATU5_TimerC->TOCE3[6]))(TOCE63);
    (*(mATU5_TimerC->TOCE0[7]))(TOCE70);
    (*(mATU5_TimerC->TOCE1[7]))(TOCE71);
    (*(mATU5_TimerC->TOCE2[7]))(TOCE72);
    (*(mATU5_TimerC->TOCE3[7]))(TOCE73);
    (*(mATU5_TimerC->TOCE0[8]))(TOCE80);
    (*(mATU5_TimerC->TOCE1[8]))(TOCE81);
    (*(mATU5_TimerC->TOCE2[8]))(TOCE82);
    (*(mATU5_TimerC->TOCE3[8]))(TOCE83);
    (*(mATU5_TimerC->TOCE0[9]))(TOCE90);
    (*(mATU5_TimerC->TOCE1[9]))(TOCE91);
    (*(mATU5_TimerC->TOCE2[9]))(TOCE92);
    (*(mATU5_TimerC->TOCE3[9]))(TOCE93);
    (*(mATU5_TimerC->TOCE0[10]))(TOCE100);
    (*(mATU5_TimerC->TOCE1[10]))(TOCE101);
    (*(mATU5_TimerC->TOCE2[10]))(TOCE102);
    (*(mATU5_TimerC->TOCE3[10]))(TOCE103);
    (*(mATU5_TimerC->TOCE0[11]))(TOCE110);
    (*(mATU5_TimerC->TOCE1[11]))(TOCE111);
    (*(mATU5_TimerC->TOCE2[11]))(TOCE112);
    (*(mATU5_TimerC->TOCE3[11]))(TOCE113);
    (*(mATU5_TimerC->TOCE0[12]))(TOCE120);
    (*(mATU5_TimerC->TOCE1[12]))(TOCE121);
    (*(mATU5_TimerC->TOCE2[12]))(TOCE122);
    (*(mATU5_TimerC->TOCE3[12]))(TOCE123);
    (*(mATU5_TimerC->TOCE0[13]))(TOCE130);
    (*(mATU5_TimerC->TOCE1[13]))(TOCE131);
    (*(mATU5_TimerC->TOCE2[13]))(TOCE132);
    (*(mATU5_TimerC->TOCE3[13]))(TOCE133);
    // TOCxy
    (*(mATU5_TimerC->TOC0[0]))(TOC00);
    (*(mATU5_TimerC->TOC1[0]))(TOC01);
    (*(mATU5_TimerC->TOC2[0]))(TOC02);
    (*(mATU5_TimerC->TOC3[0]))(TOC03);
    (*(mATU5_TimerC->TOC0[1]))(TOC10);
    (*(mATU5_TimerC->TOC1[1]))(TOC11);
    (*(mATU5_TimerC->TOC2[1]))(TOC12);
    (*(mATU5_TimerC->TOC3[1]))(TOC13);
    (*(mATU5_TimerC->TOC0[2]))(TOC20);
    (*(mATU5_TimerC->TOC1[2]))(TOC21);
    (*(mATU5_TimerC->TOC2[2]))(TOC22);
    (*(mATU5_TimerC->TOC3[2]))(TOC23);
    (*(mATU5_TimerC->TOC0[3]))(TOC30);
    (*(mATU5_TimerC->TOC1[3]))(TOC31);
    (*(mATU5_TimerC->TOC2[3]))(dummy_TOC32);
    (*(mATU5_TimerC->TOC3[3]))(dummy_TOC33);
    (*(mATU5_TimerC->TOC0[4]))(dummy_TOC40);
    (*(mATU5_TimerC->TOC1[4]))(dummy_TOC41);
    (*(mATU5_TimerC->TOC2[4]))(dummy_TOC42);
    (*(mATU5_TimerC->TOC3[4]))(dummy_TOC43);
    (*(mATU5_TimerC->TOC0[5]))(dummy_TOC50);
    (*(mATU5_TimerC->TOC1[5]))(dummy_TOC51);
    (*(mATU5_TimerC->TOC2[5]))(dummy_TOC52);
    (*(mATU5_TimerC->TOC3[5]))(dummy_TOC53);
    (*(mATU5_TimerC->TOC0[6]))(dummy_TOC60);
    (*(mATU5_TimerC->TOC1[6]))(dummy_TOC61);
    (*(mATU5_TimerC->TOC2[6]))(dummy_TOC62);
    (*(mATU5_TimerC->TOC3[6]))(dummy_TOC63);
    (*(mATU5_TimerC->TOC0[7]))(dummy_TOC70);
    (*(mATU5_TimerC->TOC1[7]))(dummy_TOC71);
    (*(mATU5_TimerC->TOC2[7]))(dummy_TOC72);
    (*(mATU5_TimerC->TOC3[7]))(dummy_TOC73);
    (*(mATU5_TimerC->TOC0[8]))(dummy_TOC80);
    (*(mATU5_TimerC->TOC1[8]))(dummy_TOC81);
    (*(mATU5_TimerC->TOC2[8]))(dummy_TOC82);
    (*(mATU5_TimerC->TOC3[8]))(dummy_TOC83);
    (*(mATU5_TimerC->TOC0[9]))(dummy_TOC90);
    (*(mATU5_TimerC->TOC1[9]))(dummy_TOC91);
    (*(mATU5_TimerC->TOC2[9]))(dummy_TOC92);
    (*(mATU5_TimerC->TOC3[9]))(dummy_TOC93);
    (*(mATU5_TimerC->TOC0[10]))(dummy_TOC100);
    (*(mATU5_TimerC->TOC1[10]))(dummy_TOC101);
    (*(mATU5_TimerC->TOC2[10]))(dummy_TOC102);
    (*(mATU5_TimerC->TOC3[10]))(dummy_TOC103);
    (*(mATU5_TimerC->TOC0[11]))(dummy_TOC110);
    (*(mATU5_TimerC->TOC1[11]))(dummy_TOC111);
    (*(mATU5_TimerC->TOC2[11]))(dummy_TOC112);
    (*(mATU5_TimerC->TOC3[11]))(dummy_TOC113);
    (*(mATU5_TimerC->TOC0[12]))(dummy_TOC120);
    (*(mATU5_TimerC->TOC1[12]))(dummy_TOC121);
    (*(mATU5_TimerC->TOC2[12]))(dummy_TOC122);
    (*(mATU5_TimerC->TOC3[12]))(dummy_TOC123);
    (*(mATU5_TimerC->TOC0[13]))(dummy_TOC130);
    (*(mATU5_TimerC->TOC1[13]))(dummy_TOC131);
    (*(mATU5_TimerC->TOC2[13]))(dummy_TOC132);
    (*(mATU5_TimerC->TOC3[13]))(dummy_TOC133);
    //OVICx
    (*(mATU5_TimerC->INT_OVI[0]))(OVIC0);
    (*(mATU5_TimerC->INT_OVI[1]))(OVIC1);
    (*(mATU5_TimerC->INT_OVI[2]))(OVIC2);
    (*(mATU5_TimerC->INT_OVI[3]))(OVIC3);
    (*(mATU5_TimerC->INT_OVI[4]))(OVIC4);
    (*(mATU5_TimerC->INT_OVI[5]))(OVIC5);
    (*(mATU5_TimerC->INT_OVI[6]))(OVIC6);
    (*(mATU5_TimerC->INT_OVI[7]))(OVIC7);
    (*(mATU5_TimerC->INT_OVI[8]))(OVIC8);
    (*(mATU5_TimerC->INT_OVI[9]))(OVIC9);
    (*(mATU5_TimerC->INT_OVI[10]))(OVIC10);
    (*(mATU5_TimerC->INT_OVI[11]))(OVIC11);
    (*(mATU5_TimerC->INT_OVI[12]))(OVIC12);
    (*(mATU5_TimerC->INT_OVI[13]))(OVIC13);
    // IMIGCxy
    (*(mATU5_TimerC->INT_GRC0[0]))(IMIGC00);
    (*(mATU5_TimerC->INT_GRC1[0]))(IMIGC01);
    (*(mATU5_TimerC->INT_GRC2[0]))(IMIGC02);
    (*(mATU5_TimerC->INT_GRC3[0]))(IMIGC03);
    (*(mATU5_TimerC->INT_GRC0[1]))(IMIGC10);
    (*(mATU5_TimerC->INT_GRC1[1]))(IMIGC11);
    (*(mATU5_TimerC->INT_GRC2[1]))(IMIGC12);
    (*(mATU5_TimerC->INT_GRC3[1]))(IMIGC13);
    (*(mATU5_TimerC->INT_GRC0[2]))(IMIGC20);
    (*(mATU5_TimerC->INT_GRC1[2]))(IMIGC21);
    (*(mATU5_TimerC->INT_GRC2[2]))(IMIGC22);
    (*(mATU5_TimerC->INT_GRC3[2]))(IMIGC23);
    (*(mATU5_TimerC->INT_GRC0[3]))(IMIGC30);
    (*(mATU5_TimerC->INT_GRC1[3]))(IMIGC31);
    (*(mATU5_TimerC->INT_GRC2[3]))(IMIGC32);
    (*(mATU5_TimerC->INT_GRC3[3]))(IMIGC33);
    (*(mATU5_TimerC->INT_GRC0[4]))(IMIGC40);
    (*(mATU5_TimerC->INT_GRC1[4]))(IMIGC41);
    (*(mATU5_TimerC->INT_GRC2[4]))(IMIGC42);
    (*(mATU5_TimerC->INT_GRC3[4]))(IMIGC43);
    (*(mATU5_TimerC->INT_GRC0[5]))(IMIGC50);
    (*(mATU5_TimerC->INT_GRC1[5]))(IMIGC51);
    (*(mATU5_TimerC->INT_GRC2[5]))(IMIGC52);
    (*(mATU5_TimerC->INT_GRC3[5]))(IMIGC53);
    (*(mATU5_TimerC->INT_GRC0[6]))(IMIGC60);
    (*(mATU5_TimerC->INT_GRC1[6]))(IMIGC61);
    (*(mATU5_TimerC->INT_GRC2[6]))(IMIGC62);
    (*(mATU5_TimerC->INT_GRC3[6]))(IMIGC63);
    (*(mATU5_TimerC->INT_GRC0[7]))(IMIGC70);
    (*(mATU5_TimerC->INT_GRC1[7]))(IMIGC71);
    (*(mATU5_TimerC->INT_GRC2[7]))(IMIGC72);
    (*(mATU5_TimerC->INT_GRC3[7]))(IMIGC73);
    (*(mATU5_TimerC->INT_GRC0[8]))(IMIGC80);
    (*(mATU5_TimerC->INT_GRC1[8]))(IMIGC81);
    (*(mATU5_TimerC->INT_GRC2[8]))(IMIGC82);
    (*(mATU5_TimerC->INT_GRC3[8]))(IMIGC83);
    (*(mATU5_TimerC->INT_GRC0[9]))(IMIGC90);
    (*(mATU5_TimerC->INT_GRC1[9]))(IMIGC91);
    (*(mATU5_TimerC->INT_GRC2[9]))(IMIGC92);
    (*(mATU5_TimerC->INT_GRC3[9]))(IMIGC93);
    (*(mATU5_TimerC->INT_GRC0[10]))(IMIGC100);
    (*(mATU5_TimerC->INT_GRC1[10]))(IMIGC101);
    (*(mATU5_TimerC->INT_GRC2[10]))(IMIGC102);
    (*(mATU5_TimerC->INT_GRC3[10]))(IMIGC103);
    (*(mATU5_TimerC->INT_GRC0[11]))(IMIGC110);
    (*(mATU5_TimerC->INT_GRC1[11]))(IMIGC111);
    (*(mATU5_TimerC->INT_GRC2[11]))(IMIGC112);
    (*(mATU5_TimerC->INT_GRC3[11]))(IMIGC113);
    (*(mATU5_TimerC->INT_GRC0[12]))(IMIGC120);
    (*(mATU5_TimerC->INT_GRC1[12]))(IMIGC121);
    (*(mATU5_TimerC->INT_GRC2[12]))(IMIGC122);
    (*(mATU5_TimerC->INT_GRC3[12]))(IMIGC123);
    (*(mATU5_TimerC->INT_GRC0[13]))(IMIGC130);
    (*(mATU5_TimerC->INT_GRC1[13]))(IMIGC131);
    (*(mATU5_TimerC->INT_GRC2[13]))(IMIGC132);
    (*(mATU5_TimerC->INT_GRC3[13]))(IMIGC133);
    // IMIOCxy
    (*(mATU5_TimerC->INT_OCRC0[0]))(IMIOC00);
    (*(mATU5_TimerC->INT_OCRC1[0]))(IMIOC01);
    (*(mATU5_TimerC->INT_OCRC2[0]))(IMIOC02);
    (*(mATU5_TimerC->INT_OCRC3[0]))(IMIOC03);
    (*(mATU5_TimerC->INT_OCRC0[1]))(IMIOC10);
    (*(mATU5_TimerC->INT_OCRC1[1]))(IMIOC11);
    (*(mATU5_TimerC->INT_OCRC2[1]))(IMIOC12);
    (*(mATU5_TimerC->INT_OCRC3[1]))(IMIOC13);
    (*(mATU5_TimerC->INT_OCRC0[2]))(IMIOC20);
    (*(mATU5_TimerC->INT_OCRC1[2]))(IMIOC21);
    (*(mATU5_TimerC->INT_OCRC2[2]))(IMIOC22);
    (*(mATU5_TimerC->INT_OCRC3[2]))(IMIOC23);
    (*(mATU5_TimerC->INT_OCRC0[3]))(IMIOC30);
    (*(mATU5_TimerC->INT_OCRC1[3]))(IMIOC31);
    (*(mATU5_TimerC->INT_OCRC2[3]))(IMIOC32);
    (*(mATU5_TimerC->INT_OCRC3[3]))(IMIOC33);
    (*(mATU5_TimerC->INT_OCRC0[4]))(IMIOC40);
    (*(mATU5_TimerC->INT_OCRC1[4]))(IMIOC41);
    (*(mATU5_TimerC->INT_OCRC2[4]))(IMIOC42);
    (*(mATU5_TimerC->INT_OCRC3[4]))(IMIOC43);
    (*(mATU5_TimerC->INT_OCRC0[5]))(IMIOC50);
    (*(mATU5_TimerC->INT_OCRC1[5]))(IMIOC51);
    (*(mATU5_TimerC->INT_OCRC2[5]))(IMIOC52);
    (*(mATU5_TimerC->INT_OCRC3[5]))(IMIOC53);
    (*(mATU5_TimerC->INT_OCRC0[6]))(IMIOC60);
    (*(mATU5_TimerC->INT_OCRC1[6]))(IMIOC61);
    (*(mATU5_TimerC->INT_OCRC2[6]))(IMIOC62);
    (*(mATU5_TimerC->INT_OCRC3[6]))(IMIOC63);
    (*(mATU5_TimerC->INT_OCRC0[7]))(IMIOC70);
    (*(mATU5_TimerC->INT_OCRC1[7]))(IMIOC71);
    (*(mATU5_TimerC->INT_OCRC2[7]))(IMIOC72);
    (*(mATU5_TimerC->INT_OCRC3[7]))(IMIOC73);
    (*(mATU5_TimerC->INT_OCRC0[8]))(IMIOC80);
    (*(mATU5_TimerC->INT_OCRC1[8]))(IMIOC81);
    (*(mATU5_TimerC->INT_OCRC2[8]))(IMIOC82);
    (*(mATU5_TimerC->INT_OCRC3[8]))(IMIOC83);
    (*(mATU5_TimerC->INT_OCRC0[9]))(IMIOC90);
    (*(mATU5_TimerC->INT_OCRC1[9]))(IMIOC91);
    (*(mATU5_TimerC->INT_OCRC2[9]))(IMIOC92);
    (*(mATU5_TimerC->INT_OCRC3[9]))(IMIOC93);
    (*(mATU5_TimerC->INT_OCRC0[10]))(IMIOC100);
    (*(mATU5_TimerC->INT_OCRC1[10]))(IMIOC101);
    (*(mATU5_TimerC->INT_OCRC2[10]))(IMIOC102);
    (*(mATU5_TimerC->INT_OCRC3[10]))(IMIOC103);
    (*(mATU5_TimerC->INT_OCRC0[11]))(IMIOC110);
    (*(mATU5_TimerC->INT_OCRC1[11]))(IMIOC111);
    (*(mATU5_TimerC->INT_OCRC2[11]))(IMIOC112);
    (*(mATU5_TimerC->INT_OCRC3[11]))(IMIOC113);
    (*(mATU5_TimerC->INT_OCRC0[12]))(IMIOC120);
    (*(mATU5_TimerC->INT_OCRC1[12]))(IMIOC121);
    (*(mATU5_TimerC->INT_OCRC2[12]))(IMIOC122);
    (*(mATU5_TimerC->INT_OCRC3[12]))(IMIOC123);
    (*(mATU5_TimerC->INT_OCRC0[13]))(IMIOC130);
    (*(mATU5_TimerC->INT_OCRC1[13]))(IMIOC131);
    (*(mATU5_TimerC->INT_OCRC2[13]))(IMIOC132);
    (*(mATU5_TimerC->INT_OCRC3[13]))(IMIOC133);
    // TimerC - Internal
    for (unsigned int i = 0; i < emCLK_BUSnum; i++) {
        (*(mATU5_TimerC->clock_bus[i]))(*(clk_bus_sig[i]));
    }
    (mATU5_TimerC->nc_clk)(nc_clk_sig);
    (mATU5_TimerC->i_event_1_0)(timerA_evt_1_0_sig);
    (mATU5_TimerC->i_event_2A)(timerA_evt_2A_sig);
    (mATU5_TimerC->i_event_2B)(timerA_evt_2B_sig);
    // TimerD
    (mATU5_TimerD->PCLK)(CLK_LSBperiod_sig);
    (*(mATU5_TimerD->PHUDFE[0][0]))(TID00);
    (*(mATU5_TimerD->PHUDFE[1][0]))(TID10);
    (*(mATU5_TimerD->PHUDFE[2][0]))(TID20);
    (*(mATU5_TimerD->PHUDFE[3][0]))(TID30);
    (*(mATU5_TimerD->PHUDFE[4][0]))(TID40);
    (*(mATU5_TimerD->PHUDFE[5][0]))(TID50);
    (*(mATU5_TimerD->PHUDFE[6][0]))(TID60);
    (*(mATU5_TimerD->PHUDFE[7][0]))(TID70);
    (*(mATU5_TimerD->PHUDFE[8][0]))(TID80);
    (*(mATU5_TimerD->PHUDFE[9][0]))(TID90);
    (*(mATU5_TimerD->PHUDFE[10][0]))(TID100);
    (*(mATU5_TimerD->PHUDFE[11][0]))(TID110);
    (*(mATU5_TimerD->PHUDFE[12][0]))(TID120);
    (*(mATU5_TimerD->PHUDFE[13][0]))(TID130);
    (*(mATU5_TimerD->PHUDFE[14][0]))(TID140);

    (*(mATU5_TimerD->PHUDFE[0][1]))(TID01);
    (*(mATU5_TimerD->PHUDFE[1][1]))(TID11);
    (*(mATU5_TimerD->PHUDFE[2][1]))(TID21);
    (*(mATU5_TimerD->PHUDFE[3][1]))(TID31);
    (*(mATU5_TimerD->PHUDFE[4][1]))(TID41);
    (*(mATU5_TimerD->PHUDFE[5][1]))(TID51);
    (*(mATU5_TimerD->PHUDFE[6][1]))(TID61);
    (*(mATU5_TimerD->PHUDFE[7][1]))(TID71);
    (*(mATU5_TimerD->PHUDFE[8][1]))(TID81);
    (*(mATU5_TimerD->PHUDFE[9][1]))(TID91);
    (*(mATU5_TimerD->PHUDFE[10][1]))(TID101);
    (*(mATU5_TimerD->PHUDFE[11][1]))(TID111);
    (*(mATU5_TimerD->PHUDFE[12][1]))(TID121);
    (*(mATU5_TimerD->PHUDFE[13][1]))(TID131);
    (*(mATU5_TimerD->PHUDFE[14][1]))(TID141);

    (*(mATU5_TimerD->PHUDFE[0][2]))(TID02);
    (*(mATU5_TimerD->PHUDFE[1][2]))(TID12);
    (*(mATU5_TimerD->PHUDFE[2][2]))(TID22);
    (*(mATU5_TimerD->PHUDFE[3][2]))(TID32);
    (*(mATU5_TimerD->PHUDFE[4][2]))(TID42);
    (*(mATU5_TimerD->PHUDFE[5][2]))(TID52);
    (*(mATU5_TimerD->PHUDFE[6][2]))(TID62);
    (*(mATU5_TimerD->PHUDFE[7][2]))(TID72);
    (*(mATU5_TimerD->PHUDFE[8][2]))(TID82);
    (*(mATU5_TimerD->PHUDFE[9][2]))(TID92);
    (*(mATU5_TimerD->PHUDFE[10][2]))(TID102);
    (*(mATU5_TimerD->PHUDFE[11][2]))(TID112);
    (*(mATU5_TimerD->PHUDFE[12][2]))(TID122);
    (*(mATU5_TimerD->PHUDFE[13][2]))(TID132);
    (*(mATU5_TimerD->PHUDFE[14][2]))(TID142);

    (*(mATU5_TimerD->PHUDFE[0][3]))(TID03);
    (*(mATU5_TimerD->PHUDFE[1][3]))(TID13);
    (*(mATU5_TimerD->PHUDFE[2][3]))(TID23);
    (*(mATU5_TimerD->PHUDFE[3][3]))(TID33);
    (*(mATU5_TimerD->PHUDFE[4][3]))(TID43);
    (*(mATU5_TimerD->PHUDFE[5][3]))(TID53);
    (*(mATU5_TimerD->PHUDFE[6][3]))(TID63);
    (*(mATU5_TimerD->PHUDFE[7][3]))(TID73);
    (*(mATU5_TimerD->PHUDFE[8][3]))(TID83);
    (*(mATU5_TimerD->PHUDFE[9][3]))(TID93);
    (*(mATU5_TimerD->PHUDFE[10][3]))(TID103);
    (*(mATU5_TimerD->PHUDFE[11][3]))(TID113);
    (*(mATU5_TimerD->PHUDFE[12][3]))(TID123);
    (*(mATU5_TimerD->PHUDFE[13][3]))(TID133);
    (*(mATU5_TimerD->PHUDFE[14][3]))(TID143);

    (*(mATU5_TimerD->PHUDFE[0][4]))(TID04);
    (*(mATU5_TimerD->PHUDFE[1][4]))(TID14);
    (*(mATU5_TimerD->PHUDFE[2][4]))(TID24);
    (*(mATU5_TimerD->PHUDFE[3][4]))(TID34);
    (*(mATU5_TimerD->PHUDFE[4][4]))(TID44);
    (*(mATU5_TimerD->PHUDFE[5][4]))(TID54);
    (*(mATU5_TimerD->PHUDFE[6][4]))(TID64);
    (*(mATU5_TimerD->PHUDFE[7][4]))(TID74);
    (*(mATU5_TimerD->PHUDFE[8][4]))(TID84);
    (*(mATU5_TimerD->PHUDFE[9][4]))(TID94);
    (*(mATU5_TimerD->PHUDFE[10][4]))(TID104);
    (*(mATU5_TimerD->PHUDFE[11][4]))(TID114);
    (*(mATU5_TimerD->PHUDFE[12][4]))(TID124);
    (*(mATU5_TimerD->PHUDFE[13][4]))(TID134);
    (*(mATU5_TimerD->PHUDFE[14][4]))(TID144);

    (*(mATU5_TimerD->PHUDFE[0][5]))(TID05);
    (*(mATU5_TimerD->PHUDFE[1][5]))(TID15);
    (*(mATU5_TimerD->PHUDFE[2][5]))(TID25);
    (*(mATU5_TimerD->PHUDFE[3][5]))(TID35);
    (*(mATU5_TimerD->PHUDFE[4][5]))(TID45);
    (*(mATU5_TimerD->PHUDFE[5][5]))(TID55);
    (*(mATU5_TimerD->PHUDFE[6][5]))(TID65);
    (*(mATU5_TimerD->PHUDFE[7][5]))(TID75);
    (*(mATU5_TimerD->PHUDFE[8][5]))(TID85);
    (*(mATU5_TimerD->PHUDFE[9][5]))(TID95);
    (*(mATU5_TimerD->PHUDFE[10][5]))(TID105);
    (*(mATU5_TimerD->PHUDFE[11][5]))(TID115);
    (*(mATU5_TimerD->PHUDFE[12][5]))(TID125);
    (*(mATU5_TimerD->PHUDFE[13][5]))(TID135);
    (*(mATU5_TimerD->PHUDFE[14][5]))(TID145);


    (*(mATU5_TimerD->PHUDFE[0][6]))(TID06);
    (*(mATU5_TimerD->PHUDFE[1][6]))(TID16);
    (*(mATU5_TimerD->PHUDFE[2][6]))(TID26);
    (*(mATU5_TimerD->PHUDFE[3][6]))(TID36);
    (*(mATU5_TimerD->PHUDFE[4][6]))(TID46);
    (*(mATU5_TimerD->PHUDFE[5][6]))(TID56);
    (*(mATU5_TimerD->PHUDFE[6][6]))(TID66);
    (*(mATU5_TimerD->PHUDFE[7][6]))(TID76);
    (*(mATU5_TimerD->PHUDFE[8][6]))(TID86);
    (*(mATU5_TimerD->PHUDFE[9][6]))(TID96);
    (*(mATU5_TimerD->PHUDFE[10][6]))(TID106);
    (*(mATU5_TimerD->PHUDFE[11][6]))(TID116);
    (*(mATU5_TimerD->PHUDFE[12][6]))(TID126);
    (*(mATU5_TimerD->PHUDFE[13][6]))(TID136);
    (*(mATU5_TimerD->PHUDFE[14][6]))(TID146);

    (*(mATU5_TimerD->PHUDFE[0][7]))(TID07);
    (*(mATU5_TimerD->PHUDFE[1][7]))(TID17);
    (*(mATU5_TimerD->PHUDFE[2][7]))(TID27);
    (*(mATU5_TimerD->PHUDFE[3][7]))(TID37);
    (*(mATU5_TimerD->PHUDFE[4][7]))(TID47);
    (*(mATU5_TimerD->PHUDFE[5][7]))(TID57);
    (*(mATU5_TimerD->PHUDFE[6][7]))(TID67);
    (*(mATU5_TimerD->PHUDFE[7][7]))(TID77);
    (*(mATU5_TimerD->PHUDFE[8][7]))(TID87);
    (*(mATU5_TimerD->PHUDFE[9][7]))(TID97);
    (*(mATU5_TimerD->PHUDFE[10][7]))(TID107);
    (*(mATU5_TimerD->PHUDFE[11][7]))(TID117);
    (*(mATU5_TimerD->PHUDFE[12][7]))(TID127);
    (*(mATU5_TimerD->PHUDFE[13][7]))(TID137);
    (*(mATU5_TimerD->PHUDFE[14][7]))(TID147);


    (*(mATU5_TimerD->TODA[0][0]))(TOD00A);
    (*(mATU5_TimerD->TODA[0][1]))(TOD01A);
    (*(mATU5_TimerD->TODA[0][2]))(TOD02A);
    (*(mATU5_TimerD->TODA[0][3]))(TOD03A);
    (*(mATU5_TimerD->TODA[1][0]))(TOD10A);
    (*(mATU5_TimerD->TODA[1][1]))(TOD11A);
    (*(mATU5_TimerD->TODA[1][2]))(TOD12A);
    (*(mATU5_TimerD->TODA[1][3]))(TOD13A);
    (*(mATU5_TimerD->TODA[2][0]))(TOD20A);
    (*(mATU5_TimerD->TODA[2][1]))(TOD21A);
    (*(mATU5_TimerD->TODA[2][2]))(TOD22A);
    (*(mATU5_TimerD->TODA[2][3]))(TOD23A);
    (*(mATU5_TimerD->TODA[3][0]))(TOD30A);
    (*(mATU5_TimerD->TODA[3][1]))(TOD31A);
    (*(mATU5_TimerD->TODA[3][2]))(TOD32A);
    (*(mATU5_TimerD->TODA[3][3]))(TOD33A);
    (*(mATU5_TimerD->TODA[4][0]))(TOD40A);
    (*(mATU5_TimerD->TODA[4][1]))(TOD41A);
    (*(mATU5_TimerD->TODA[4][2]))(TOD42A);
    (*(mATU5_TimerD->TODA[4][3]))(TOD43A);
    (*(mATU5_TimerD->TODA[5][0]))(TOD50A);
    (*(mATU5_TimerD->TODA[5][1]))(TOD51A);
    (*(mATU5_TimerD->TODA[5][2]))(TOD52A);
    (*(mATU5_TimerD->TODA[5][3]))(TOD53A);
    (*(mATU5_TimerD->TODA[6][0]))(TOD60A);
    (*(mATU5_TimerD->TODA[6][1]))(TOD61A);
    (*(mATU5_TimerD->TODA[6][2]))(TOD62A);
    (*(mATU5_TimerD->TODA[6][3]))(TOD63A);
    (*(mATU5_TimerD->TODA[7][0]))(TOD70A);
    (*(mATU5_TimerD->TODA[7][1]))(TOD71A);
    (*(mATU5_TimerD->TODA[7][2]))(TOD72A);
    (*(mATU5_TimerD->TODA[7][3]))(TOD73A);
    (*(mATU5_TimerD->TODA[8][0]))(TOD80A);
    (*(mATU5_TimerD->TODA[8][1]))(TOD81A);
    (*(mATU5_TimerD->TODA[8][2]))(TOD82A);
    (*(mATU5_TimerD->TODA[8][3]))(TOD83A);
    (*(mATU5_TimerD->TODA[9][0]))(TOD90A);
    (*(mATU5_TimerD->TODA[9][1]))(TOD91A);
    (*(mATU5_TimerD->TODA[9][2]))(TOD92A);
    (*(mATU5_TimerD->TODA[9][3]))(TOD93A);
    (*(mATU5_TimerD->TODA[10][0]))(TOD100A);
    (*(mATU5_TimerD->TODA[10][1]))(TOD101A);
    (*(mATU5_TimerD->TODA[10][2]))(TOD102A);
    (*(mATU5_TimerD->TODA[10][3]))(TOD103A);
    (*(mATU5_TimerD->TODA[11][0]))(TOD110A);
    (*(mATU5_TimerD->TODA[11][1]))(TOD111A);
    (*(mATU5_TimerD->TODA[11][2]))(TOD112A);
    (*(mATU5_TimerD->TODA[11][3]))(TOD113A);
    (*(mATU5_TimerD->TODA[12][0]))(TOD120A);
    (*(mATU5_TimerD->TODA[12][1]))(TOD121A);
    (*(mATU5_TimerD->TODA[12][2]))(TOD122A);
    (*(mATU5_TimerD->TODA[12][3]))(TOD123A);
    (*(mATU5_TimerD->TODA[13][0]))(TOD130A);
    (*(mATU5_TimerD->TODA[13][1]))(TOD131A);
    (*(mATU5_TimerD->TODA[13][2]))(TOD132A);
    (*(mATU5_TimerD->TODA[13][3]))(TOD133A);
    (*(mATU5_TimerD->TODA[14][0]))(TOD140A);
    (*(mATU5_TimerD->TODA[14][1]))(TOD141A);
    (*(mATU5_TimerD->TODA[14][2]))(TOD142A);
    (*(mATU5_TimerD->TODA[14][3]))(TOD143A);

    (*(mATU5_TimerD->TODB[0][0]))(TOD00B);
    (*(mATU5_TimerD->TODB[0][1]))(TOD01B);
    (*(mATU5_TimerD->TODB[0][2]))(TOD02B);
    (*(mATU5_TimerD->TODB[0][3]))(TOD03B);
    (*(mATU5_TimerD->TODB[1][0]))(TOD10B);
    (*(mATU5_TimerD->TODB[1][1]))(TOD11B);
    (*(mATU5_TimerD->TODB[1][2]))(TOD12B);
    (*(mATU5_TimerD->TODB[1][3]))(TOD13B);
    (*(mATU5_TimerD->TODB[2][0]))(TOD20B);
    (*(mATU5_TimerD->TODB[2][1]))(TOD21B);
    (*(mATU5_TimerD->TODB[2][2]))(TOD22B);
    (*(mATU5_TimerD->TODB[2][3]))(TOD23B);
    (*(mATU5_TimerD->TODB[3][0]))(TOD30B);
    (*(mATU5_TimerD->TODB[3][1]))(TOD31B);
    (*(mATU5_TimerD->TODB[3][2]))(TOD32B);
    (*(mATU5_TimerD->TODB[3][3]))(TOD33B);
    (*(mATU5_TimerD->TODB[4][0]))(TOD40B);
    (*(mATU5_TimerD->TODB[4][1]))(TOD41B);
    (*(mATU5_TimerD->TODB[4][2]))(TOD42B);
    (*(mATU5_TimerD->TODB[4][3]))(TOD43B);
    (*(mATU5_TimerD->TODB[5][0]))(TOD50B);
    (*(mATU5_TimerD->TODB[5][1]))(TOD51B);
    (*(mATU5_TimerD->TODB[5][2]))(TOD52B);
    (*(mATU5_TimerD->TODB[5][3]))(TOD53B);
    (*(mATU5_TimerD->TODB[6][0]))(TOD60B);
    (*(mATU5_TimerD->TODB[6][1]))(TOD61B);
    (*(mATU5_TimerD->TODB[6][2]))(TOD62B);
    (*(mATU5_TimerD->TODB[6][3]))(TOD63B);
    (*(mATU5_TimerD->TODB[7][0]))(TOD70B);
    (*(mATU5_TimerD->TODB[7][1]))(TOD71B);
    (*(mATU5_TimerD->TODB[7][2]))(TOD72B);
    (*(mATU5_TimerD->TODB[7][3]))(TOD73B);
    (*(mATU5_TimerD->TODB[8][0]))(TOD80B);
    (*(mATU5_TimerD->TODB[8][1]))(TOD81B);
    (*(mATU5_TimerD->TODB[8][2]))(TOD82B);
    (*(mATU5_TimerD->TODB[8][3]))(TOD83B);
    (*(mATU5_TimerD->TODB[9][0]))(TOD90B);
    (*(mATU5_TimerD->TODB[9][1]))(TOD91B);
    (*(mATU5_TimerD->TODB[9][2]))(TOD92B);
    (*(mATU5_TimerD->TODB[9][3]))(TOD93B);
    (*(mATU5_TimerD->TODB[10][0]))(TOD100B);
    (*(mATU5_TimerD->TODB[10][1]))(TOD101B);
    (*(mATU5_TimerD->TODB[10][2]))(TOD102B);
    (*(mATU5_TimerD->TODB[10][3]))(TOD103B);
    (*(mATU5_TimerD->TODB[11][0]))(TOD110B);
    (*(mATU5_TimerD->TODB[11][1]))(TOD111B);
    (*(mATU5_TimerD->TODB[11][2]))(TOD112B);
    (*(mATU5_TimerD->TODB[11][3]))(TOD113B);
    (*(mATU5_TimerD->TODB[12][0]))(TOD120B);
    (*(mATU5_TimerD->TODB[12][1]))(TOD121B);
    (*(mATU5_TimerD->TODB[12][2]))(TOD122B);
    (*(mATU5_TimerD->TODB[12][3]))(TOD123B);
    (*(mATU5_TimerD->TODB[13][0]))(TOD130B);
    (*(mATU5_TimerD->TODB[13][1]))(TOD131B);
    (*(mATU5_TimerD->TODB[13][2]))(TOD132B);
    (*(mATU5_TimerD->TODB[13][3]))(TOD133B);
    (*(mATU5_TimerD->TODB[14][0]))(TOD140B);
    (*(mATU5_TimerD->TODB[14][1]))(TOD141B);
    (*(mATU5_TimerD->TODB[14][2]))(TOD142B);
    (*(mATU5_TimerD->TODB[14][3]))(TOD143B);


    // TODMIGxy
    (*(mATU5_TimerD->TODMIG[0][0]))(TODMIG00);
    (*(mATU5_TimerD->TODMIG[0][1]))(TODMIG01);
    (*(mATU5_TimerD->TODMIG[0][2]))(TODMIG02);
    (*(mATU5_TimerD->TODMIG[0][3]))(TODMIG03);
    (*(mATU5_TimerD->TODMIG[1][0]))(TODMIG10);
    (*(mATU5_TimerD->TODMIG[1][1]))(TODMIG11);
    (*(mATU5_TimerD->TODMIG[1][2]))(TODMIG12);
    (*(mATU5_TimerD->TODMIG[1][3]))(TODMIG13);
    (*(mATU5_TimerD->TODMIG[2][0]))(TODMIG20);
    (*(mATU5_TimerD->TODMIG[2][1]))(TODMIG21);
    (*(mATU5_TimerD->TODMIG[2][2]))(TODMIG22);
    (*(mATU5_TimerD->TODMIG[2][3]))(TODMIG23);
    (*(mATU5_TimerD->TODMIG[3][0]))(TODMIG30);
    (*(mATU5_TimerD->TODMIG[3][1]))(TODMIG31);
    (*(mATU5_TimerD->TODMIG[3][2]))(TODMIG32);
    (*(mATU5_TimerD->TODMIG[3][3]))(TODMIG33);
    (*(mATU5_TimerD->TODMIG[4][0]))(TODMIG40);
    (*(mATU5_TimerD->TODMIG[4][1]))(TODMIG41);
    (*(mATU5_TimerD->TODMIG[4][2]))(TODMIG42);
    (*(mATU5_TimerD->TODMIG[4][3]))(TODMIG43);
    (*(mATU5_TimerD->TODMIG[5][0]))(TODMIG50);
    (*(mATU5_TimerD->TODMIG[5][1]))(TODMIG51);
    (*(mATU5_TimerD->TODMIG[5][2]))(TODMIG52);
    (*(mATU5_TimerD->TODMIG[5][3]))(TODMIG53);
    (*(mATU5_TimerD->TODMIG[6][0]))(TODMIG60);
    (*(mATU5_TimerD->TODMIG[6][1]))(TODMIG61);
    (*(mATU5_TimerD->TODMIG[6][2]))(TODMIG62);
    (*(mATU5_TimerD->TODMIG[6][3]))(TODMIG63);
    (*(mATU5_TimerD->TODMIG[7][0]))(TODMIG70);
    (*(mATU5_TimerD->TODMIG[7][1]))(TODMIG71);
    (*(mATU5_TimerD->TODMIG[7][2]))(TODMIG72);
    (*(mATU5_TimerD->TODMIG[7][3]))(TODMIG73);
    (*(mATU5_TimerD->TODMIG[8][0]))(TODMIG80);
    (*(mATU5_TimerD->TODMIG[8][1]))(TODMIG81);
    (*(mATU5_TimerD->TODMIG[8][2]))(TODMIG82);
    (*(mATU5_TimerD->TODMIG[8][3]))(TODMIG83);
    (*(mATU5_TimerD->TODMIG[9][0]))(TODMIG90);
    (*(mATU5_TimerD->TODMIG[9][1]))(TODMIG91);
    (*(mATU5_TimerD->TODMIG[9][2]))(TODMIG92);
    (*(mATU5_TimerD->TODMIG[9][3]))(TODMIG93);
    (*(mATU5_TimerD->TODMIG[10][0]))(TODMIG100);
    (*(mATU5_TimerD->TODMIG[10][1]))(TODMIG101);
    (*(mATU5_TimerD->TODMIG[10][2]))(TODMIG102);
    (*(mATU5_TimerD->TODMIG[10][3]))(TODMIG103);
    (*(mATU5_TimerD->TODMIG[11][0]))(TODMIG110);
    (*(mATU5_TimerD->TODMIG[11][1]))(TODMIG111);
    (*(mATU5_TimerD->TODMIG[11][2]))(TODMIG112);
    (*(mATU5_TimerD->TODMIG[11][3]))(TODMIG113);
    (*(mATU5_TimerD->TODMIG[12][0]))(TODMIG120);
    (*(mATU5_TimerD->TODMIG[12][1]))(TODMIG121);
    (*(mATU5_TimerD->TODMIG[12][2]))(TODMIG122);
    (*(mATU5_TimerD->TODMIG[12][3]))(TODMIG123);
    (*(mATU5_TimerD->TODMIG[13][0]))(TODMIG130);
    (*(mATU5_TimerD->TODMIG[13][1]))(TODMIG131);
    (*(mATU5_TimerD->TODMIG[13][2]))(TODMIG132);
    (*(mATU5_TimerD->TODMIG[13][3]))(TODMIG133);
    (*(mATU5_TimerD->TODMIG[14][0]))(TODMIG140);
    (*(mATU5_TimerD->TODMIG[14][1]))(TODMIG141);
    (*(mATU5_TimerD->TODMIG[14][2]))(TODMIG142);
    (*(mATU5_TimerD->TODMIG[14][3]))(TODMIG143);

    // CMIADxy
    (*(mATU5_TimerD->INT_CMPA[0][0]))(CMIAD00);
    (*(mATU5_TimerD->INT_CMPA[0][1]))(CMIAD01);
    (*(mATU5_TimerD->INT_CMPA[0][2]))(CMIAD02);
    (*(mATU5_TimerD->INT_CMPA[0][3]))(CMIAD03);
    (*(mATU5_TimerD->INT_CMPA[1][0]))(CMIAD10);
    (*(mATU5_TimerD->INT_CMPA[1][1]))(CMIAD11);
    (*(mATU5_TimerD->INT_CMPA[1][2]))(CMIAD12);
    (*(mATU5_TimerD->INT_CMPA[1][3]))(CMIAD13);
    (*(mATU5_TimerD->INT_CMPA[2][0]))(CMIAD20);
    (*(mATU5_TimerD->INT_CMPA[2][1]))(CMIAD21);
    (*(mATU5_TimerD->INT_CMPA[2][2]))(CMIAD22);
    (*(mATU5_TimerD->INT_CMPA[2][3]))(CMIAD23);
    (*(mATU5_TimerD->INT_CMPA[3][0]))(CMIAD30);
    (*(mATU5_TimerD->INT_CMPA[3][1]))(CMIAD31);
    (*(mATU5_TimerD->INT_CMPA[3][2]))(CMIAD32);
    (*(mATU5_TimerD->INT_CMPA[3][3]))(CMIAD33);
    (*(mATU5_TimerD->INT_CMPA[4][0]))(CMIAD40);
    (*(mATU5_TimerD->INT_CMPA[4][1]))(CMIAD41);
    (*(mATU5_TimerD->INT_CMPA[4][2]))(CMIAD42);
    (*(mATU5_TimerD->INT_CMPA[4][3]))(CMIAD43);
    (*(mATU5_TimerD->INT_CMPA[5][0]))(CMIAD50);
    (*(mATU5_TimerD->INT_CMPA[5][1]))(CMIAD51);
    (*(mATU5_TimerD->INT_CMPA[5][2]))(CMIAD52);
    (*(mATU5_TimerD->INT_CMPA[5][3]))(CMIAD53);
    (*(mATU5_TimerD->INT_CMPA[6][0]))(CMIAD60);
    (*(mATU5_TimerD->INT_CMPA[6][1]))(CMIAD61);
    (*(mATU5_TimerD->INT_CMPA[6][2]))(CMIAD62);
    (*(mATU5_TimerD->INT_CMPA[6][3]))(CMIAD63);
    (*(mATU5_TimerD->INT_CMPA[7][0]))(CMIAD70);
    (*(mATU5_TimerD->INT_CMPA[7][1]))(CMIAD71);
    (*(mATU5_TimerD->INT_CMPA[7][2]))(CMIAD72);
    (*(mATU5_TimerD->INT_CMPA[7][3]))(CMIAD73);
    (*(mATU5_TimerD->INT_CMPA[8][0]))(CMIAD80);
    (*(mATU5_TimerD->INT_CMPA[8][1]))(CMIAD81);
    (*(mATU5_TimerD->INT_CMPA[8][2]))(CMIAD82);
    (*(mATU5_TimerD->INT_CMPA[8][3]))(CMIAD83);
    (*(mATU5_TimerD->INT_CMPA[9][0]))(CMIAD90);
    (*(mATU5_TimerD->INT_CMPA[9][1]))(CMIAD91);
    (*(mATU5_TimerD->INT_CMPA[9][2]))(CMIAD92);
    (*(mATU5_TimerD->INT_CMPA[9][3]))(CMIAD93);
    (*(mATU5_TimerD->INT_CMPA[10][0]))(CMIAD100);
    (*(mATU5_TimerD->INT_CMPA[10][1]))(CMIAD101);
    (*(mATU5_TimerD->INT_CMPA[10][2]))(CMIAD102);
    (*(mATU5_TimerD->INT_CMPA[10][3]))(CMIAD103);
    (*(mATU5_TimerD->INT_CMPA[11][0]))(CMIAD110);
    (*(mATU5_TimerD->INT_CMPA[11][1]))(CMIAD111);
    (*(mATU5_TimerD->INT_CMPA[11][2]))(CMIAD112);
    (*(mATU5_TimerD->INT_CMPA[11][3]))(CMIAD113);
    (*(mATU5_TimerD->INT_CMPA[12][0]))(CMIAD120);
    (*(mATU5_TimerD->INT_CMPA[12][1]))(CMIAD121);
    (*(mATU5_TimerD->INT_CMPA[12][2]))(CMIAD122);
    (*(mATU5_TimerD->INT_CMPA[12][3]))(CMIAD123);
    (*(mATU5_TimerD->INT_CMPA[13][0]))(CMIAD130);
    (*(mATU5_TimerD->INT_CMPA[13][1]))(CMIAD131);
    (*(mATU5_TimerD->INT_CMPA[13][2]))(CMIAD132);
    (*(mATU5_TimerD->INT_CMPA[13][3]))(CMIAD133);
    (*(mATU5_TimerD->INT_CMPA[14][0]))(CMIAD140);
    (*(mATU5_TimerD->INT_CMPA[14][1]))(CMIAD141);
    (*(mATU5_TimerD->INT_CMPA[14][2]))(CMIAD142);
    (*(mATU5_TimerD->INT_CMPA[14][3]))(CMIAD143);

    // CMIBDxy
    (*(mATU5_TimerD->INT_CMPB[0][0]))(CMIBD00);
    (*(mATU5_TimerD->INT_CMPB[0][1]))(CMIBD01);
    (*(mATU5_TimerD->INT_CMPB[0][2]))(CMIBD02);
    (*(mATU5_TimerD->INT_CMPB[0][3]))(CMIBD03);
    (*(mATU5_TimerD->INT_CMPB[1][0]))(CMIBD10);
    (*(mATU5_TimerD->INT_CMPB[1][1]))(CMIBD11);
    (*(mATU5_TimerD->INT_CMPB[1][2]))(CMIBD12);
    (*(mATU5_TimerD->INT_CMPB[1][3]))(CMIBD13);
    (*(mATU5_TimerD->INT_CMPB[2][0]))(CMIBD20);
    (*(mATU5_TimerD->INT_CMPB[2][1]))(CMIBD21);
    (*(mATU5_TimerD->INT_CMPB[2][2]))(CMIBD22);
    (*(mATU5_TimerD->INT_CMPB[2][3]))(CMIBD23);
    (*(mATU5_TimerD->INT_CMPB[3][0]))(CMIBD30);
    (*(mATU5_TimerD->INT_CMPB[3][1]))(CMIBD31);
    (*(mATU5_TimerD->INT_CMPB[3][2]))(CMIBD32);
    (*(mATU5_TimerD->INT_CMPB[3][3]))(CMIBD33);
    (*(mATU5_TimerD->INT_CMPB[4][0]))(CMIBD40);
    (*(mATU5_TimerD->INT_CMPB[4][1]))(CMIBD41);
    (*(mATU5_TimerD->INT_CMPB[4][2]))(CMIBD42);
    (*(mATU5_TimerD->INT_CMPB[4][3]))(CMIBD43);
    (*(mATU5_TimerD->INT_CMPB[5][0]))(CMIBD50);
    (*(mATU5_TimerD->INT_CMPB[5][1]))(CMIBD51);
    (*(mATU5_TimerD->INT_CMPB[5][2]))(CMIBD52);
    (*(mATU5_TimerD->INT_CMPB[5][3]))(CMIBD53);
    (*(mATU5_TimerD->INT_CMPB[6][0]))(CMIBD60);
    (*(mATU5_TimerD->INT_CMPB[6][1]))(CMIBD61);
    (*(mATU5_TimerD->INT_CMPB[6][2]))(CMIBD62);
    (*(mATU5_TimerD->INT_CMPB[6][3]))(CMIBD63);
    (*(mATU5_TimerD->INT_CMPB[7][0]))(CMIBD70);
    (*(mATU5_TimerD->INT_CMPB[7][1]))(CMIBD71);
    (*(mATU5_TimerD->INT_CMPB[7][2]))(CMIBD72);
    (*(mATU5_TimerD->INT_CMPB[7][3]))(CMIBD73);
    (*(mATU5_TimerD->INT_CMPB[8][0]))(CMIBD80);
    (*(mATU5_TimerD->INT_CMPB[8][1]))(CMIBD81);
    (*(mATU5_TimerD->INT_CMPB[8][2]))(CMIBD82);
    (*(mATU5_TimerD->INT_CMPB[8][3]))(CMIBD83);
    (*(mATU5_TimerD->INT_CMPB[9][0]))(CMIBD90);
    (*(mATU5_TimerD->INT_CMPB[9][1]))(CMIBD91);
    (*(mATU5_TimerD->INT_CMPB[9][2]))(CMIBD92);
    (*(mATU5_TimerD->INT_CMPB[9][3]))(CMIBD93);
    (*(mATU5_TimerD->INT_CMPB[10][0]))(CMIBD100);
    (*(mATU5_TimerD->INT_CMPB[10][1]))(CMIBD101);
    (*(mATU5_TimerD->INT_CMPB[10][2]))(CMIBD102);
    (*(mATU5_TimerD->INT_CMPB[10][3]))(CMIBD103);
    (*(mATU5_TimerD->INT_CMPB[11][0]))(CMIBD110);
    (*(mATU5_TimerD->INT_CMPB[11][1]))(CMIBD111);
    (*(mATU5_TimerD->INT_CMPB[11][2]))(CMIBD112);
    (*(mATU5_TimerD->INT_CMPB[11][3]))(CMIBD113);
    (*(mATU5_TimerD->INT_CMPB[12][0]))(CMIBD120);
    (*(mATU5_TimerD->INT_CMPB[12][1]))(CMIBD121);
    (*(mATU5_TimerD->INT_CMPB[12][2]))(CMIBD122);
    (*(mATU5_TimerD->INT_CMPB[12][3]))(CMIBD123);
    (*(mATU5_TimerD->INT_CMPB[13][0]))(CMIBD130);
    (*(mATU5_TimerD->INT_CMPB[13][1]))(CMIBD131);
    (*(mATU5_TimerD->INT_CMPB[13][2]))(CMIBD132);
    (*(mATU5_TimerD->INT_CMPB[13][3]))(CMIBD133);
    (*(mATU5_TimerD->INT_CMPB[14][0]))(CMIBD140);
    (*(mATU5_TimerD->INT_CMPB[14][1]))(CMIBD141);
    (*(mATU5_TimerD->INT_CMPB[14][2]))(CMIBD142);
    (*(mATU5_TimerD->INT_CMPB[14][3]))(CMIBD143);
    // OVI1Dx
    (*(mATU5_TimerD->INT_OVF1[0]))(OVI1D0);
    (*(mATU5_TimerD->INT_OVF1[1]))(OVI1D1);
    (*(mATU5_TimerD->INT_OVF1[2]))(OVI1D2);
    (*(mATU5_TimerD->INT_OVF1[3]))(OVI1D3);
    (*(mATU5_TimerD->INT_OVF1[4]))(OVI1D4);
    (*(mATU5_TimerD->INT_OVF1[5]))(OVI1D5);
    (*(mATU5_TimerD->INT_OVF1[6]))(OVI1D6);
    (*(mATU5_TimerD->INT_OVF1[7]))(OVI1D7);
    (*(mATU5_TimerD->INT_OVF1[8]))(OVI1D8);
    (*(mATU5_TimerD->INT_OVF1[9]))(OVI1D9);
    (*(mATU5_TimerD->INT_OVF1[10]))(OVI1D10);
    (*(mATU5_TimerD->INT_OVF1[11]))(OVI1D11);
    (*(mATU5_TimerD->INT_OVF1[12]))(OVI1D12);
    (*(mATU5_TimerD->INT_OVF1[13]))(OVI1D13);
    (*(mATU5_TimerD->INT_OVF1[14]))(OVI1D14);
    // OVI2Dx
    (*(mATU5_TimerD->INT_OVF2[0]))(OVI2D0);
    (*(mATU5_TimerD->INT_OVF2[1]))(OVI2D1);
    (*(mATU5_TimerD->INT_OVF2[2]))(OVI2D2);
    (*(mATU5_TimerD->INT_OVF2[3]))(OVI2D3);
    (*(mATU5_TimerD->INT_OVF2[4]))(OVI2D4);
    (*(mATU5_TimerD->INT_OVF2[5]))(OVI2D5);
    (*(mATU5_TimerD->INT_OVF2[6]))(OVI2D6);
    (*(mATU5_TimerD->INT_OVF2[7]))(OVI2D7);
    (*(mATU5_TimerD->INT_OVF2[8]))(OVI2D8);
    (*(mATU5_TimerD->INT_OVF2[9]))(OVI2D9);
    (*(mATU5_TimerD->INT_OVF2[10]))(OVI2D10);
    (*(mATU5_TimerD->INT_OVF2[11]))(OVI2D11);
    (*(mATU5_TimerD->INT_OVF2[12]))(OVI2D12);
    (*(mATU5_TimerD->INT_OVF2[13]))(OVI2D13);
    (*(mATU5_TimerD->INT_OVF2[14]))(OVI2D14);
    // UDIDxy
    (*(mATU5_TimerD->INT_UDF[0][0]))(UDID00);
    (*(mATU5_TimerD->INT_UDF[0][1]))(UDID01);
    (*(mATU5_TimerD->INT_UDF[0][2]))(UDID02);
    (*(mATU5_TimerD->INT_UDF[0][3]))(UDID03);
    (*(mATU5_TimerD->INT_UDF[1][0]))(UDID10);
    (*(mATU5_TimerD->INT_UDF[1][1]))(UDID11);
    (*(mATU5_TimerD->INT_UDF[1][2]))(UDID12);
    (*(mATU5_TimerD->INT_UDF[1][3]))(UDID13);
    (*(mATU5_TimerD->INT_UDF[2][0]))(UDID20);
    (*(mATU5_TimerD->INT_UDF[2][1]))(UDID21);
    (*(mATU5_TimerD->INT_UDF[2][2]))(UDID22);
    (*(mATU5_TimerD->INT_UDF[2][3]))(UDID23);
    (*(mATU5_TimerD->INT_UDF[3][0]))(UDID30);
    (*(mATU5_TimerD->INT_UDF[3][1]))(UDID31);
    (*(mATU5_TimerD->INT_UDF[3][2]))(UDID32);
    (*(mATU5_TimerD->INT_UDF[3][3]))(UDID33);
    (*(mATU5_TimerD->INT_UDF[4][0]))(UDID40);
    (*(mATU5_TimerD->INT_UDF[4][1]))(UDID41);
    (*(mATU5_TimerD->INT_UDF[4][2]))(UDID42);
    (*(mATU5_TimerD->INT_UDF[4][3]))(UDID43);
    (*(mATU5_TimerD->INT_UDF[5][0]))(UDID50);
    (*(mATU5_TimerD->INT_UDF[5][1]))(UDID51);
    (*(mATU5_TimerD->INT_UDF[5][2]))(UDID52);
    (*(mATU5_TimerD->INT_UDF[5][3]))(UDID53);
    (*(mATU5_TimerD->INT_UDF[6][0]))(UDID60);
    (*(mATU5_TimerD->INT_UDF[6][1]))(UDID61);
    (*(mATU5_TimerD->INT_UDF[6][2]))(UDID62);
    (*(mATU5_TimerD->INT_UDF[6][3]))(UDID63);
    (*(mATU5_TimerD->INT_UDF[7][0]))(UDID70);
    (*(mATU5_TimerD->INT_UDF[7][1]))(UDID71);
    (*(mATU5_TimerD->INT_UDF[7][2]))(UDID72);
    (*(mATU5_TimerD->INT_UDF[7][3]))(UDID73);
    (*(mATU5_TimerD->INT_UDF[8][0]))(UDID80);
    (*(mATU5_TimerD->INT_UDF[8][1]))(UDID81);
    (*(mATU5_TimerD->INT_UDF[8][2]))(UDID82);
    (*(mATU5_TimerD->INT_UDF[8][3]))(UDID83);
    (*(mATU5_TimerD->INT_UDF[9][0]))(UDID90);
    (*(mATU5_TimerD->INT_UDF[9][1]))(UDID91);
    (*(mATU5_TimerD->INT_UDF[9][2]))(UDID92);
    (*(mATU5_TimerD->INT_UDF[9][3]))(UDID93);
    (*(mATU5_TimerD->INT_UDF[10][0]))(UDID100);
    (*(mATU5_TimerD->INT_UDF[10][1]))(UDID101);
    (*(mATU5_TimerD->INT_UDF[10][2]))(UDID102);
    (*(mATU5_TimerD->INT_UDF[10][3]))(UDID103);
    (*(mATU5_TimerD->INT_UDF[11][0]))(UDID110);
    (*(mATU5_TimerD->INT_UDF[11][1]))(UDID111);
    (*(mATU5_TimerD->INT_UDF[11][2]))(UDID112);
    (*(mATU5_TimerD->INT_UDF[11][3]))(UDID113);
    (*(mATU5_TimerD->INT_UDF[12][0]))(UDID120);
    (*(mATU5_TimerD->INT_UDF[12][1]))(UDID121);
    (*(mATU5_TimerD->INT_UDF[12][2]))(UDID122);
    (*(mATU5_TimerD->INT_UDF[12][3]))(UDID123);
    (*(mATU5_TimerD->INT_UDF[13][0]))(UDID130);
    (*(mATU5_TimerD->INT_UDF[13][1]))(UDID131);
    (*(mATU5_TimerD->INT_UDF[13][2]))(UDID132);
    (*(mATU5_TimerD->INT_UDF[13][3]))(UDID133);
    (*(mATU5_TimerD->INT_UDF[14][0]))(UDID140);
    (*(mATU5_TimerD->INT_UDF[14][1]))(UDID141);
    (*(mATU5_TimerD->INT_UDF[14][2]))(UDID142);
    (*(mATU5_TimerD->INT_UDF[14][3]))(UDID143);

    // PREIDxy
    (*(mATU5_TimerD->INT_REIED[0][0]))(PREID00);
    (*(mATU5_TimerD->INT_REIED[0][1]))(PREID01);
    (*(mATU5_TimerD->INT_REIED[0][2]))(PREID02);
    (*(mATU5_TimerD->INT_REIED[0][3]))(PREID03);
    (*(mATU5_TimerD->INT_REIED[1][0]))(PREID10);
    (*(mATU5_TimerD->INT_REIED[1][1]))(PREID11);
    (*(mATU5_TimerD->INT_REIED[1][2]))(PREID12);
    (*(mATU5_TimerD->INT_REIED[1][3]))(PREID13);
    (*(mATU5_TimerD->INT_REIED[2][0]))(PREID20);
    (*(mATU5_TimerD->INT_REIED[2][1]))(PREID21);
    (*(mATU5_TimerD->INT_REIED[2][2]))(PREID22);
    (*(mATU5_TimerD->INT_REIED[2][3]))(PREID23);
    (*(mATU5_TimerD->INT_REIED[3][0]))(PREID30);
    (*(mATU5_TimerD->INT_REIED[3][1]))(PREID31);
    (*(mATU5_TimerD->INT_REIED[3][2]))(PREID32);
    (*(mATU5_TimerD->INT_REIED[3][3]))(PREID33);
    (*(mATU5_TimerD->INT_REIED[4][0]))(PREID40);
    (*(mATU5_TimerD->INT_REIED[4][1]))(PREID41);
    (*(mATU5_TimerD->INT_REIED[4][2]))(PREID42);
    (*(mATU5_TimerD->INT_REIED[4][3]))(PREID43);
    (*(mATU5_TimerD->INT_REIED[5][0]))(PREID50);
    (*(mATU5_TimerD->INT_REIED[5][1]))(PREID51);
    (*(mATU5_TimerD->INT_REIED[5][2]))(PREID52);
    (*(mATU5_TimerD->INT_REIED[5][3]))(PREID53);
    (*(mATU5_TimerD->INT_REIED[6][0]))(PREID60);
    (*(mATU5_TimerD->INT_REIED[6][1]))(PREID61);
    (*(mATU5_TimerD->INT_REIED[6][2]))(PREID62);
    (*(mATU5_TimerD->INT_REIED[6][3]))(PREID63);
    (*(mATU5_TimerD->INT_REIED[7][0]))(PREID70);
    (*(mATU5_TimerD->INT_REIED[7][1]))(PREID71);
    (*(mATU5_TimerD->INT_REIED[7][2]))(PREID72);
    (*(mATU5_TimerD->INT_REIED[7][3]))(PREID73);
    (*(mATU5_TimerD->INT_REIED[8][0]))(PREID80);
    (*(mATU5_TimerD->INT_REIED[8][1]))(PREID81);
    (*(mATU5_TimerD->INT_REIED[8][2]))(PREID82);
    (*(mATU5_TimerD->INT_REIED[8][3]))(PREID83);
    (*(mATU5_TimerD->INT_REIED[9][0]))(PREID90);
    (*(mATU5_TimerD->INT_REIED[9][1]))(PREID91);
    (*(mATU5_TimerD->INT_REIED[9][2]))(PREID92);
    (*(mATU5_TimerD->INT_REIED[9][3]))(PREID93);
    (*(mATU5_TimerD->INT_REIED[10][0]))(PREID100);
    (*(mATU5_TimerD->INT_REIED[10][1]))(PREID101);
    (*(mATU5_TimerD->INT_REIED[10][2]))(PREID102);
    (*(mATU5_TimerD->INT_REIED[10][3]))(PREID103);
    (*(mATU5_TimerD->INT_REIED[11][0]))(PREID110);
    (*(mATU5_TimerD->INT_REIED[11][1]))(PREID111);
    (*(mATU5_TimerD->INT_REIED[11][2]))(PREID112);
    (*(mATU5_TimerD->INT_REIED[11][3]))(PREID113);
    (*(mATU5_TimerD->INT_REIED[12][0]))(PREID120);
    (*(mATU5_TimerD->INT_REIED[12][1]))(PREID121);
    (*(mATU5_TimerD->INT_REIED[12][2]))(PREID122);
    (*(mATU5_TimerD->INT_REIED[12][3]))(PREID123);
    (*(mATU5_TimerD->INT_REIED[13][0]))(PREID130);
    (*(mATU5_TimerD->INT_REIED[13][1]))(PREID131);
    (*(mATU5_TimerD->INT_REIED[13][2]))(PREID132);
    (*(mATU5_TimerD->INT_REIED[13][3]))(PREID133);
    (*(mATU5_TimerD->INT_REIED[14][0]))(PREID140);
    (*(mATU5_TimerD->INT_REIED[14][1]))(PREID141);
    (*(mATU5_TimerD->INT_REIED[14][2]))(PREID142);
    (*(mATU5_TimerD->INT_REIED[14][3]))(PREID143);

    // PFEIDxy
    (*(mATU5_TimerD->INT_FEIED[0][0]))(PFEID00);
    (*(mATU5_TimerD->INT_FEIED[0][1]))(PFEID01);
    (*(mATU5_TimerD->INT_FEIED[0][2]))(PFEID02);
    (*(mATU5_TimerD->INT_FEIED[0][3]))(PFEID03);
    (*(mATU5_TimerD->INT_FEIED[1][0]))(PFEID10);
    (*(mATU5_TimerD->INT_FEIED[1][1]))(PFEID11);
    (*(mATU5_TimerD->INT_FEIED[1][2]))(PFEID12);
    (*(mATU5_TimerD->INT_FEIED[1][3]))(PFEID13);
    (*(mATU5_TimerD->INT_FEIED[2][0]))(PFEID20);
    (*(mATU5_TimerD->INT_FEIED[2][1]))(PFEID21);
    (*(mATU5_TimerD->INT_FEIED[2][2]))(PFEID22);
    (*(mATU5_TimerD->INT_FEIED[2][3]))(PFEID23);
    (*(mATU5_TimerD->INT_FEIED[3][0]))(PFEID30);
    (*(mATU5_TimerD->INT_FEIED[3][1]))(PFEID31);
    (*(mATU5_TimerD->INT_FEIED[3][2]))(PFEID32);
    (*(mATU5_TimerD->INT_FEIED[3][3]))(PFEID33);
    (*(mATU5_TimerD->INT_FEIED[4][0]))(PFEID40);
    (*(mATU5_TimerD->INT_FEIED[4][1]))(PFEID41);
    (*(mATU5_TimerD->INT_FEIED[4][2]))(PFEID42);
    (*(mATU5_TimerD->INT_FEIED[4][3]))(PFEID43);
    (*(mATU5_TimerD->INT_FEIED[5][0]))(PFEID50);
    (*(mATU5_TimerD->INT_FEIED[5][1]))(PFEID51);
    (*(mATU5_TimerD->INT_FEIED[5][2]))(PFEID52);
    (*(mATU5_TimerD->INT_FEIED[5][3]))(PFEID53);
    (*(mATU5_TimerD->INT_FEIED[6][0]))(PFEID60);
    (*(mATU5_TimerD->INT_FEIED[6][1]))(PFEID61);
    (*(mATU5_TimerD->INT_FEIED[6][2]))(PFEID62);
    (*(mATU5_TimerD->INT_FEIED[6][3]))(PFEID63);
    (*(mATU5_TimerD->INT_FEIED[7][0]))(PFEID70);
    (*(mATU5_TimerD->INT_FEIED[7][1]))(PFEID71);
    (*(mATU5_TimerD->INT_FEIED[7][2]))(PFEID72);
    (*(mATU5_TimerD->INT_FEIED[7][3]))(PFEID73);
    (*(mATU5_TimerD->INT_FEIED[8][0]))(PFEID80);
    (*(mATU5_TimerD->INT_FEIED[8][1]))(PFEID81);
    (*(mATU5_TimerD->INT_FEIED[8][2]))(PFEID82);
    (*(mATU5_TimerD->INT_FEIED[8][3]))(PFEID83);
    (*(mATU5_TimerD->INT_FEIED[9][0]))(PFEID90);
    (*(mATU5_TimerD->INT_FEIED[9][1]))(PFEID91);
    (*(mATU5_TimerD->INT_FEIED[9][2]))(PFEID92);
    (*(mATU5_TimerD->INT_FEIED[9][3]))(PFEID93);
    (*(mATU5_TimerD->INT_FEIED[10][0]))(PFEID100);
    (*(mATU5_TimerD->INT_FEIED[10][1]))(PFEID101);
    (*(mATU5_TimerD->INT_FEIED[10][2]))(PFEID102);
    (*(mATU5_TimerD->INT_FEIED[10][3]))(PFEID103);
    (*(mATU5_TimerD->INT_FEIED[11][0]))(PFEID110);
    (*(mATU5_TimerD->INT_FEIED[11][1]))(PFEID111);
    (*(mATU5_TimerD->INT_FEIED[11][2]))(PFEID112);
    (*(mATU5_TimerD->INT_FEIED[11][3]))(PFEID113);
    (*(mATU5_TimerD->INT_FEIED[12][0]))(PFEID120);
    (*(mATU5_TimerD->INT_FEIED[12][1]))(PFEID121);
    (*(mATU5_TimerD->INT_FEIED[12][2]))(PFEID122);
    (*(mATU5_TimerD->INT_FEIED[12][3]))(PFEID123);
    (*(mATU5_TimerD->INT_FEIED[13][0]))(PFEID130);
    (*(mATU5_TimerD->INT_FEIED[13][1]))(PFEID131);
    (*(mATU5_TimerD->INT_FEIED[13][2]))(PFEID132);
    (*(mATU5_TimerD->INT_FEIED[13][3]))(PFEID133);
    (*(mATU5_TimerD->INT_FEIED[14][0]))(PFEID140);
    (*(mATU5_TimerD->INT_FEIED[14][1]))(PFEID141);
    (*(mATU5_TimerD->INT_FEIED[14][2]))(PFEID142);
    (*(mATU5_TimerD->INT_FEIED[14][3]))(PFEID143);

    // TimerD - Internal
    (mATU5_TimerD->clock_bus0)(*(clk_bus_sig[0]));
    (mATU5_TimerD->clock_bus1)(*(clk_bus_sig[1]));
    (mATU5_TimerD->clock_bus2)(*(clk_bus_sig[2]));
    (mATU5_TimerD->clock_bus3)(*(clk_bus_sig[3]));
    (mATU5_TimerD->clock_bus4)(*(clk_bus_sig[4]));
    (mATU5_TimerD->clock_bus5)(*(clk_bus_sig[5]));
    (mATU5_TimerD->TAEO2A)(timerA_evt_2A_sig);
    (mATU5_TimerD->TAEO2B)(timerA_evt_2B_sig);
    (mATU5_TimerD->CLRB)(timerB_TCNTDCLR_sig);
    // TimerE
    (mATU5_TimerE->PCLK)(CLK_LSBperiod_sig);
    (*(mATU5_TimerE->TOE[0][0]))(TOE00);
    (*(mATU5_TimerE->TOE[0][1]))(TOE01);
    (*(mATU5_TimerE->TOE[0][2]))(TOE02);
    (*(mATU5_TimerE->TOE[0][3]))(TOE03);
    (*(mATU5_TimerE->TOE[1][0]))(TOE10);
    (*(mATU5_TimerE->TOE[1][1]))(TOE11);
    (*(mATU5_TimerE->TOE[1][2]))(TOE12);
    (*(mATU5_TimerE->TOE[1][3]))(TOE13);
    (*(mATU5_TimerE->TOE[2][0]))(TOE20);
    (*(mATU5_TimerE->TOE[2][1]))(TOE21);
    (*(mATU5_TimerE->TOE[2][2]))(TOE22);
    (*(mATU5_TimerE->TOE[2][3]))(TOE23);
    (*(mATU5_TimerE->TOE[3][0]))(TOE30);
    (*(mATU5_TimerE->TOE[3][1]))(TOE31);
    (*(mATU5_TimerE->TOE[3][2]))(TOE32);
    (*(mATU5_TimerE->TOE[3][3]))(TOE33);
    (*(mATU5_TimerE->TOE[4][0]))(TOE40);
    (*(mATU5_TimerE->TOE[4][1]))(TOE41);
    (*(mATU5_TimerE->TOE[4][2]))(TOE42);
    (*(mATU5_TimerE->TOE[4][3]))(TOE43);
    (*(mATU5_TimerE->TOE[5][0]))(TOE50);
    (*(mATU5_TimerE->TOE[5][1]))(TOE51);
    (*(mATU5_TimerE->TOE[5][2]))(TOE52);
    (*(mATU5_TimerE->TOE[5][3]))(TOE53);
    (*(mATU5_TimerE->TOE[6][0]))(TOE60);
    (*(mATU5_TimerE->TOE[6][1]))(TOE61);
    (*(mATU5_TimerE->TOE[6][2]))(TOE62);
    (*(mATU5_TimerE->TOE[6][3]))(TOE63);
    (*(mATU5_TimerE->TOE[7][0]))(TOE70);
    (*(mATU5_TimerE->TOE[7][1]))(TOE71);
    (*(mATU5_TimerE->TOE[7][2]))(TOE72);
    (*(mATU5_TimerE->TOE[7][3]))(TOE73);
    (*(mATU5_TimerE->TOE[8][0]))(TOE80);
    (*(mATU5_TimerE->TOE[8][1]))(TOE81);
    (*(mATU5_TimerE->TOE[8][2]))(TOE82);
    (*(mATU5_TimerE->TOE[8][3]))(TOE83);
    (*(mATU5_TimerE->TOE[9][0]))(TOE90);
    (*(mATU5_TimerE->TOE[9][1]))(TOE91);
    (*(mATU5_TimerE->TOE[9][2]))(TOE92);
    (*(mATU5_TimerE->TOE[9][3]))(TOE93);

    (*(mATU5_TimerE->INTATUECMIE[0][0]))(INTATUECMIE00);
    (*(mATU5_TimerE->INTATUECMIE[0][1]))(INTATUECMIE01);
    (*(mATU5_TimerE->INTATUECMIE[0][2]))(INTATUECMIE02);
    (*(mATU5_TimerE->INTATUECMIE[0][3]))(INTATUECMIE03);
    (*(mATU5_TimerE->INTATUECMIE[1][0]))(INTATUECMIE10);
    (*(mATU5_TimerE->INTATUECMIE[1][1]))(INTATUECMIE11);
    (*(mATU5_TimerE->INTATUECMIE[1][2]))(INTATUECMIE12);
    (*(mATU5_TimerE->INTATUECMIE[1][3]))(INTATUECMIE13);
    (*(mATU5_TimerE->INTATUECMIE[2][0]))(INTATUECMIE20);
    (*(mATU5_TimerE->INTATUECMIE[2][1]))(INTATUECMIE21);
    (*(mATU5_TimerE->INTATUECMIE[2][2]))(INTATUECMIE22);
    (*(mATU5_TimerE->INTATUECMIE[2][3]))(INTATUECMIE23);
    (*(mATU5_TimerE->INTATUECMIE[3][0]))(INTATUECMIE30);
    (*(mATU5_TimerE->INTATUECMIE[3][1]))(INTATUECMIE31);
    (*(mATU5_TimerE->INTATUECMIE[3][2]))(INTATUECMIE32);
    (*(mATU5_TimerE->INTATUECMIE[3][3]))(INTATUECMIE33);
    (*(mATU5_TimerE->INTATUECMIE[4][0]))(INTATUECMIE40);
    (*(mATU5_TimerE->INTATUECMIE[4][1]))(INTATUECMIE41);
    (*(mATU5_TimerE->INTATUECMIE[4][2]))(INTATUECMIE42);
    (*(mATU5_TimerE->INTATUECMIE[4][3]))(INTATUECMIE43);
    (*(mATU5_TimerE->INTATUECMIE[5][0]))(INTATUECMIE50);
    (*(mATU5_TimerE->INTATUECMIE[5][1]))(INTATUECMIE51);
    (*(mATU5_TimerE->INTATUECMIE[5][2]))(INTATUECMIE52);
    (*(mATU5_TimerE->INTATUECMIE[5][3]))(INTATUECMIE53);
    (*(mATU5_TimerE->INTATUECMIE[6][0]))(INTATUECMIE60);
    (*(mATU5_TimerE->INTATUECMIE[6][1]))(INTATUECMIE61);
    (*(mATU5_TimerE->INTATUECMIE[6][2]))(INTATUECMIE62);
    (*(mATU5_TimerE->INTATUECMIE[6][3]))(INTATUECMIE63);
    (*(mATU5_TimerE->INTATUECMIE[7][0]))(INTATUECMIE70);
    (*(mATU5_TimerE->INTATUECMIE[7][1]))(INTATUECMIE71);
    (*(mATU5_TimerE->INTATUECMIE[7][2]))(INTATUECMIE72);
    (*(mATU5_TimerE->INTATUECMIE[7][3]))(INTATUECMIE73);
    (*(mATU5_TimerE->INTATUECMIE[8][0]))(INTATUECMIE80);
    (*(mATU5_TimerE->INTATUECMIE[8][1]))(INTATUECMIE81);
    (*(mATU5_TimerE->INTATUECMIE[8][2]))(INTATUECMIE82);
    (*(mATU5_TimerE->INTATUECMIE[8][3]))(INTATUECMIE83);
    (*(mATU5_TimerE->INTATUECMIE[9][0]))(INTATUECMIE90);
    (*(mATU5_TimerE->INTATUECMIE[9][1]))(INTATUECMIE91);
    (*(mATU5_TimerE->INTATUECMIE[9][2]))(INTATUECMIE92);
    (*(mATU5_TimerE->INTATUECMIE[9][3]))(INTATUECMIE93);

    // TimerE - Internal
    (mATU5_TimerE->clock_bus0)(*(clk_bus_sig[0]));
    (mATU5_TimerE->clock_bus1)(*(clk_bus_sig[1]));
    (mATU5_TimerE->clock_bus2)(*(clk_bus_sig[2]));
    (mATU5_TimerE->clock_bus3)(*(clk_bus_sig[3]));
    (mATU5_TimerE->clock_bus4)(*(clk_bus_sig[4]));
    (mATU5_TimerE->clock_bus5)(*(clk_bus_sig[5]));
    for (unsigned int i = 0; i < emTimerEF_POEnum; i++) {
        (*(mATU5_TimerE->POE[i]))(*(timerF_POE_sig[i]));
    }
    // TimerF
    (*(mATU5_TimerF->TIFA[0]))(TIF0A);
    (*(mATU5_TimerF->TIFA[1]))(TIF1A);
    (*(mATU5_TimerF->TIFA[2]))(TIF2A);
    (*(mATU5_TimerF->TIFA[3]))(TIF3);
    (*(mATU5_TimerF->TIFA[4]))(TIF4);
    (*(mATU5_TimerF->TIFA[5]))(TIF5);
    (*(mATU5_TimerF->TIFA[6]))(TIF6);
    (*(mATU5_TimerF->TIFA[7]))(TIF7);
    (*(mATU5_TimerF->TIFA[8]))(TIF8);
    (*(mATU5_TimerF->TIFA[9]))(TIF9);
    (*(mATU5_TimerF->TIFA[10]))(TIF10);
    (*(mATU5_TimerF->TIFA[11]))(TIF11);
    (*(mATU5_TimerF->TIFA[12]))(TIF12);
    (*(mATU5_TimerF->TIFA[13]))(TIF13);
    (*(mATU5_TimerF->TIFA[14]))(TIF14);
    (*(mATU5_TimerF->TIFA[15]))(TIF15);
    (*(mATU5_TimerF->TIFA[16]))(TIF16);
    (*(mATU5_TimerF->TIFA[17]))(TIF17);
    (*(mATU5_TimerF->TIFA[18]))(TIF18);
    (*(mATU5_TimerF->TIFA[19]))(TIF19);
    (*(mATU5_TimerF->TIFB[0]))(TIF0B);
    (*(mATU5_TimerF->TIFB[1]))(TIF1B);
    (*(mATU5_TimerF->TIFB[2]))(TIF2B);

    (*(mATU5_TimerF->ICFFI[0]))(INTATUFICIF0);
    (*(mATU5_TimerF->ICFFI[1]))(INTATUFICIF1);
    (*(mATU5_TimerF->ICFFI[2]))(INTATUFICIF2);
    (*(mATU5_TimerF->ICFFI[3]))(INTATUFICIF3);
    (*(mATU5_TimerF->ICFFI[4]))(INTATUFICIF4);
    (*(mATU5_TimerF->ICFFI[5]))(INTATUFICIF5);
    (*(mATU5_TimerF->ICFFI[6]))(INTATUFICIF6);
    (*(mATU5_TimerF->ICFFI[7]))(INTATUFICIF7);
    (*(mATU5_TimerF->ICFFI[8]))(INTATUFICIF8);
    (*(mATU5_TimerF->ICFFI[9]))(INTATUFICIF9);
    (*(mATU5_TimerF->ICFFI[10]))(INTATUFICIF10);
    (*(mATU5_TimerF->ICFFI[11]))(INTATUFICIF11);
    (*(mATU5_TimerF->ICFFI[12]))(INTATUFICIF12);
    (*(mATU5_TimerF->ICFFI[13]))(INTATUFICIF13);
    (*(mATU5_TimerF->ICFFI[14]))(INTATUFICIF14);
    (*(mATU5_TimerF->ICFFI[15]))(INTATUFICIF15);
    (*(mATU5_TimerF->ICFFI[16]))(INTATUFICIF16);
    (*(mATU5_TimerF->ICFFI[17]))(INTATUFICIF17);
    (*(mATU5_TimerF->ICFFI[18]))(INTATUFICIF18);
    (*(mATU5_TimerF->ICFFI[19]))(INTATUFICIF19);

    // TimerF - Internal
    (*(mATU5_TimerF->CLKBUS[0]))(*(clk_bus_sig[0]));
    (*(mATU5_TimerF->CLKBUS[1]))(*(clk_bus_sig[1]));
    (*(mATU5_TimerF->CLKBUS[2]))(*(clk_bus_sig[2]));
    (*(mATU5_TimerF->CLKBUS[3]))(*(clk_bus_sig[3]));
    (*(mATU5_TimerF->CLKBUS[4]))(*(clk_bus_sig[4]));
    (*(mATU5_TimerF->CLKBUS[5]))(*(clk_bus_sig[5]));
    (mATU5_TimerF->nc_clk)(nc_clk_sig);

    // OVIFxy
    (*(mATU5_TimerF->OVFFI[0]))(OVIF0);
    (*(mATU5_TimerF->OVFFI[1]))(OVIF1);
    (*(mATU5_TimerF->OVFFI[2]))(OVIF2);
    (*(mATU5_TimerF->OVFFI[3]))(OVIF3);
    (*(mATU5_TimerF->OVFFI[4]))(OVIF4);
    (*(mATU5_TimerF->OVFFI[5]))(OVIF5);
    (*(mATU5_TimerF->OVFFI[6]))(OVIF6);
    (*(mATU5_TimerF->OVFFI[7]))(OVIF7);
    (*(mATU5_TimerF->OVFFI[8]))(OVIF8);
    (*(mATU5_TimerF->OVFFI[9]))(OVIF9);
    (*(mATU5_TimerF->OVFFI[10]))(OVIF10);
    (*(mATU5_TimerF->OVFFI[11]))(OVIF11);
    (*(mATU5_TimerF->OVFFI[12]))(OVIF12);
    (*(mATU5_TimerF->OVFFI[13]))(OVIF13);
    (*(mATU5_TimerF->OVFFI[14]))(OVIF14);
    (*(mATU5_TimerF->OVFFI[15]))(OVIF15);
    (*(mATU5_TimerF->OVFFI[16]))(OVIF16);
    (*(mATU5_TimerF->OVFFI[17]))(OVIF17);
    (*(mATU5_TimerF->OVFFI[18]))(OVIF18);
    (*(mATU5_TimerF->OVFFI[19]))(OVIF19);

    for (unsigned int i = 0; i < emTimerA_TILOnum; i++) {
        (*(mATU5_TimerF->TIA[i]))(*(timerA_TILOA_sig[i]));
    }
    for (unsigned int i = 0; i < emTimerEF_POEnum; i++) {
        (*(mATU5_TimerF->POE[i]))(*(timerF_POE_sig[i]));
    }
    // TimerG
    (*(mATU5_TimerG->cmi_g[0]))(INTATUGCMIG0);
    (*(mATU5_TimerG->cmi_g[1]))(INTATUGCMIG1);
    (*(mATU5_TimerG->cmi_g[2]))(INTATUGCMIG2);
    (*(mATU5_TimerG->cmi_g[3]))(INTATUGCMIG3);
    (*(mATU5_TimerG->cmi_g[4]))(INTATUGCMIG4);
    (*(mATU5_TimerG->cmi_g[5]))(INTATUGCMIG5);
    (*(mATU5_TimerG->cmi_g[6]))(INTATUGCMIG6);
    (*(mATU5_TimerG->cmi_g[7]))(INTATUGCMIG7);
    (*(mATU5_TimerG->cmi_g[8]))(INTATUGCMIG8);
    (*(mATU5_TimerG->cmi_g[9]))(INTATUGCMIG9);
    // TimerG - Internal
    (*(mATU5_TimerG->events[0]))(timerB_AGCKTIM_sig);
    
    //Initialize variables
    mCLK_LSBPeriod = 0;
    mCLK_LSBFreq = 0;
    mCLK_LSBOrgFreq = 0;
    mCLK_LSBFreqUnit = "Hz";
    mCLK_HSBPeriod = 0;
    mCLK_HSBFreq = 0;
    mCLK_HSBOrgFreq = 0;
    mCLK_HSBFreqUnit = "Hz";

    mATU5_TimerG->setTimeUnit(mTimeResolutionUnit);

    for (unsigned int reset_id = 0; reset_id < emResetSignalNum; reset_id++) {
        mResetCurVal[reset_id] = true;
        mResetPreVal[reset_id] = true;
        mIsResetHardActive[reset_id] = false;
        mIsResetCmdReceive[reset_id] = false;
        mIsResetCmdActive[reset_id] = false;
        mResetCmdPeriod[reset_id] =  0;
    }

    mDumpInterrupt = false;
    
    mModuleIdVector.push_back("common");
    mModuleIdVector.push_back("timerA");
    mModuleIdVector.push_back("timerB");
    mModuleIdVector.push_back("timerC");
    mModuleIdVector.push_back("timerD");
    mModuleIdVector.push_back("timerE");
    mModuleIdVector.push_back("timerF");
    mModuleIdVector.push_back("timerG");
    
    mBlockIdMap.insert(std::pair<std::string, unsigned int>("sub0", 0));
    mBlockIdMap.insert(std::pair<std::string, unsigned int>("sub1", 1));
    mBlockIdMap.insert(std::pair<std::string, unsigned int>("sub2", 2));
    mBlockIdMap.insert(std::pair<std::string, unsigned int>("sub3", 3));
    mBlockIdMap.insert(std::pair<std::string, unsigned int>("sub4", 4));
    mBlockIdMap.insert(std::pair<std::string, unsigned int>("sub5", 5));
    mBlockIdMap.insert(std::pair<std::string, unsigned int>("sub6", 6));
    mBlockIdMap.insert(std::pair<std::string, unsigned int>("sub7", 7));
    mBlockIdMap.insert(std::pair<std::string, unsigned int>("sub8", 8));
    mBlockIdMap.insert(std::pair<std::string, unsigned int>("sub9", 9));
    mBlockIdMap.insert(std::pair<std::string, unsigned int>("sub10", 10));
    mBlockIdMap.insert(std::pair<std::string, unsigned int>("sub11", 11));
    mBlockIdMap.insert(std::pair<std::string, unsigned int>("sub12", 12));
    mBlockIdMap.insert(std::pair<std::string, unsigned int>("sub13", 13));
    mBlockIdMap.insert(std::pair<std::string, unsigned int>("sub14", 14));
    mBlockIdMap.insert(std::pair<std::string, unsigned int>("sub15", 15));
    mBlockIdMap.insert(std::pair<std::string, unsigned int>("sub16", 16));
    mBlockIdMap.insert(std::pair<std::string, unsigned int>("sub17", 17));
    mBlockIdMap.insert(std::pair<std::string, unsigned int>("sub18", 18));
    mBlockIdMap.insert(std::pair<std::string, unsigned int>("sub19", 19));
    
    mTimeCLKChangePoint[0] = 0;
    mTimeCLKChangePoint[1] = 0;
    mAGCK1StartTime = 0;
    
    // Initialize internal variables
    Initialize();
    mNoiseCancelClkPeriod = 0; // added to fix bug Redmine #84445
    for (unsigned int i = 0; i < emTCLKnum; i++) {
        mTCLKPeriod[i] = 0;
    }

    // Initialize internal signal
    nc_clk_sig.write(0x0);
    for (unsigned int i = 0; i < emCLK_BUSnum; i++) {
        mClkBusStartTime[i] = 0;
        clk_bus_sig[i]->write(0x0);
    }
    CLK_LSBperiod_sig.write(mCLK_LSBPeriod);
    
    // Initialize Output ports
    // DMA-AD select
    INTATUCTRLDMAREQ00.initialize(false);
    INTATUCTRLDMAREQ01.initialize(false);
    INTATUCTRLDMAREQ10.initialize(false);
    INTATUCTRLDMAREQ11.initialize(false);
    at_saradtrg.initialize(false);
    
    SC_METHOD(HandleCLK_LSBSignalMethod);
    dont_initialize();
    sensitive << CLK_LSB;

    SC_METHOD(HandleCLK_HSBSignalMethod);
    dont_initialize();
    sensitive << CLK_HSB;

    SC_METHOD(HandlePresetz_bifSignalMethod);
    sensitive << presetz_bif;
    
    SC_METHOD(HandlePresetzSignalMethod);
    sensitive << presetz;
    
    SC_METHOD(HandleTCLKASignalMethod);
    dont_initialize();
    sensitive << TCLKA;
    
    SC_METHOD(HandleTCLKBSignalMethod);
    dont_initialize();
    sensitive << TCLKB;
    
   for (unsigned int i = 0; i < emClkBusPSCNum; i++) { // removed emNoiseCancelPCSNum by chuongle Jul/28/2018
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mStartPrescalerEvent[i]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ATU5011::StartPrescalerMethod, this, i),
                          sc_core::sc_gen_unique_name("StartPrescalerMethod"), &opt);
    }
    
   for (unsigned int i = 0; i < emClkBusPSCNum; i++) { // removed emNoiseCancelPCSNum by chuongle Jul/28/2018
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mStopPrescalerEvent[i]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ATU5011::StopPrescalerMethod, this, i),
                          sc_core::sc_gen_unique_name("StopPrescalerMethod"), &opt);
    }
    
    for (unsigned int i = 0; i < emCLK_BUSnum; i++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mWriteClockBusEvent[i]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ATU5011::WriteClockBusMethod, this, i),
                          sc_core::sc_gen_unique_name("WriteClockBusMethod"), &opt);
    }
    
    SC_METHOD(WriteNoiseCancelClkMethod);
    dont_initialize();
    sensitive << mWriteNoiseCancelClkEvent;
    
    for (unsigned int reset_id = 0; reset_id < emResetSignalNum; reset_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mResetHardEvent[reset_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ATU5011::HandleResetHardMethod, this, reset_id),
                          sc_core::sc_gen_unique_name("HandleResetHardMethod"), &opt);
    }

    for (unsigned int reset_id = 0; reset_id < emResetSignalNum; reset_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mResetCmdEvent[reset_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ATU5011::HandleResetCmdMethod, this, reset_id),
                          sc_core::sc_gen_unique_name("HandleResetCmdMethod"), &opt);
    }

    for (unsigned int reset_id = 0; reset_id < emResetSignalNum; reset_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mResetCmdCancelEvent[reset_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ATU5011::CancelResetCmdMethod, this, reset_id),
                          sc_core::sc_gen_unique_name("CancelResetCmdMethod"), &opt);
    }
    
    for (unsigned int i = 0; i < emTCLKnum; i++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mTCLKUpdateEvent[i]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ATU5011::TCLKUpdateMethod, this, i),
                          sc_core::sc_gen_unique_name("TCLKUpdateMethod"), &opt);
    }
    
    SC_METHOD(EnableTimerABMethod);
    dont_initialize();
    sensitive << mEnableTimerABEvent;
    
    SC_METHOD(HandleAGCK1SignalMethod);
    dont_initialize();
    sensitive << timerB_AGCK1_sig;
    
    SC_METHOD(HandleDMATrgSelectChangeMethod);
    dont_initialize();
    sensitive << mATU5_TimerD->INT_CMPA[0][0]->pos();
    
    SC_METHOD(HandleAGCK1AssertMethod);
    dont_initialize();
    sensitive << mAGCK1AssertEvent;
    
    SC_METHOD(HandleAGCK1DeassertMethod);
    dont_initialize();
    sensitive << mAGCK1DeassertEvent;
    
    SC_METHOD(UpdateDMATrgSelectMethod);
    dont_initialize();
    sensitive << mUpdateDMATrgSelectEvent;
    
    SC_METHOD(HandleDMA0TriggerInputMethod);
    dont_initialize();
    sensitive << *mATU5_TimerG->cmi_g[1];
    sensitive << *mATU5_TimerG->cmi_g[2];
    sensitive << mAGCK1ChangeEvent;
    sensitive << pf_dmai_s;
    sensitive << pf_dmai0_a;
    sensitive << pf_dmai1_a;
    sensitive << pf_dmai2_a;
    sensitive << pf_dmai3_a;
    sensitive << pf_dmai4_a;
    
    SC_METHOD(HandleDMA1TriggerInputMethod);
    dont_initialize();
    sensitive << *mATU5_TimerG->cmi_g[3];
    sensitive << timerA_evt_1_0_sig;
    
    SC_METHOD(Writeat_saradtrgMethod);
    dont_initialize();
    sensitive << mWriteat_saradtrgEvent;
    
    for (unsigned int i = 0; i < emWrapper_DMAnum; i++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mWriteTrgDMAReqEvent[i]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ATU5011::WriteTrgDMAReqMethod, this, i),
                          sc_core::sc_gen_unique_name("WriteTrgDMAReqMethod"), &opt);
    }
    
    for (unsigned int i = 0; i < 2; i++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mHandleTRGSELDMAregChangeEvent[i]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ATU5011::HandleTRGSELDMA0regChangeMethod, this, i),
                          sc_core::sc_gen_unique_name("HandleTRGSELDMA0regChangeMethod"), &opt);
    }
    
    for (unsigned int i = 0; i < 2; i++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mHandleTRGSELDMAregChangeEvent[i+2]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ATU5011::HandleTRGSELDMA1regChangeMethod, this, i),
                          sc_core::sc_gen_unique_name("HandleTRGSELDMA1regChangeMethod"), &opt);
    }
    
    SC_METHOD(HandleSARADTriggerInputMethod);
    dont_initialize();
    sensitive << *mATU5_TimerG->cmi_g[1];
    sensitive << *mATU5_TimerG->cmi_g[2];
    sensitive << mAGCK1ChangeEvent;
    sensitive << mHandleTRGSELADregChangeEvent;
    
    
    SC_METHOD(HandleAGCKMSignalMethod);
    dont_initialize();
    sensitive << timerB_AGCKM_sig;
}//}}}

///Destructor of ATU5011 class
ATU5011::~ATU5011 (void)
{//{{{
    delete mATU5011_Func;
    delete mATU5_TimerA;
    delete mATU5_TimerB;
    delete mATU5_TimerC;
    delete mATU5_TimerD;
    delete mATU5_TimerE;
    delete mATU5_TimerF;
    delete mATU5_TimerG;
    for (unsigned int index = 0; index < emTimerA_TILOnum; index++) {
        delete timerA_TILOA_sig[index];
    }
    for (unsigned int index = 0; index < emTimerEF_POEnum; index++) {
        delete timerF_POE_sig[index];
    }
    for (unsigned int index = 0; index < emCLK_BUSnum; index++) {
        delete clk_bus_sig[index];
    }
}//}}}

///Assert reset by software
void ATU5011::AssertReset (const std::string reset_name, const double start_time, const double period)
{//{{{
    if (reset_name == "presetz_bif") {
        if ((!mIsResetHardActive[0])&&(!mIsResetCmdReceive[0])) {
            mIsResetCmdReceive[0] = true;
            re_printf("info","The model will be reset (presetz_bif) for %f ns after %f ns.\n", period, start_time);
            mResetCmdEvent[0].notify(start_time, SC_NS);
            mResetCmdPeriod[0] = period;
        } else {
            re_printf("warning","The software reset of presetz_bif is called in the reset operation of the model. So it is ignored\n");
        }
    } else if (reset_name == "presetz") {
        if ((!mIsResetHardActive[1])&&(!mIsResetCmdReceive[1])) {
            mIsResetCmdReceive[1] = true;
            re_printf("info","The model will be reset (presetz) for %f ns after %f ns.\n", period, start_time);
            mResetCmdEvent[1].notify(start_time, SC_NS);
            mResetCmdPeriod[1] = period;
        } else {
            re_printf("warning","The software reset of presetz is called in the reset operation of the model. So it is ignored\n");
        }
    } else {
        re_printf("warning","The reset name (%s) is wrong. It should be presetz_bif or presetz\n",reset_name.c_str());
    }
}//}}}

///Set clock value and clock unit
void ATU5011::SetCLKFreq (const std::string clock_name, const sc_dt::uint64 freq, const std::string unit)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, clock_name);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "CLK_LSB") {
            mCLK_LSBOrgFreq = freq;
            ConvertClockFreq (mCLK_LSBFreq, mCLK_LSBFreqUnit, freq, unit);
            if (mCLK_LSBFreq > 0) {
                mCLK_LSBPeriod = (sc_dt::uint64)(((1/(double)mCLK_LSBFreq)*(double)mTimeResolutionValue) + 0.5);
            } else {
                mCLK_LSBPeriod = 0;
                // Cancel events
                CancelEvents();
                // Notify clock is zero
                mClkZeroEvent[0].notify(SC_ZERO_TIME);
            }
            CLK_LSBUpdate();
        } else if (word_vector[0] == "CLK_HSB") {
            mCLK_HSBOrgFreq = freq;
            ConvertClockFreq (mCLK_HSBFreq, mCLK_HSBFreqUnit, freq, unit);
            if (mCLK_HSBFreq > 0) {
                mCLK_HSBPeriod = (sc_dt::uint64)(((1/(double)mCLK_HSBFreq)*(double)mTimeResolutionValue) + 0.5);
                mTimeCLKChangePoint[1] = sc_time_stamp().to_double();
            } else {
                mCLK_HSBPeriod = 0;
                // Cancel events
                CancelEvents();
                // Notify clock is zero
                mClkZeroEvent[1].notify(SC_ZERO_TIME);
            }
        } else if (word_vector[0] == "TCLKA") {
            if (freq > 0) {
                mTCLKPeriod[emTCLKA] = (sc_dt::uint64)(((1/(double)freq)*(double)mTimeResolutionValue) + 0.5);
            } else {
                mTCLKPeriod[emTCLKA] = 0;
            }
            mTCLKUpdateEvent[emTCLKA].notify(SC_ZERO_TIME);
        } else if (word_vector[0] == "TCLKB") {
            if (freq > 0) {
                mTCLKPeriod[emTCLKB] = (sc_dt::uint64)(((1/(double)freq)*(double)mTimeResolutionValue) + 0.5);
            } else {
                mTCLKPeriod[emTCLKB] = 0;
            }
            mTCLKUpdateEvent[emTCLKB].notify(SC_ZERO_TIME);
        } else {
            re_printf("warning","Clock name (%s) is invalid\n", word_vector[0].c_str());
        }
    } else {
        re_printf("warning","Clock name (%s) is invalid\n", clock_name.c_str());
    }
}//}}}

///Get clock value
void ATU5011::GetCLKFreq (const std::string clock_name)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, clock_name);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "CLK_LSB") {
            re_printf("info","CLK_LSB frequency is %0.0f %s\n", (double)mCLK_LSBOrgFreq, mCLK_LSBFreqUnit.c_str());
        } else if (word_vector[0] == "CLK_HSB") {
            re_printf("info","CLK_HSB frequency is %0.0f %s\n", (double)mCLK_HSBOrgFreq, mCLK_HSBFreqUnit.c_str());
        } else {
            re_printf("warning","Clock name (%s) is invalid\n", word_vector[0].c_str());
        }
    } else {
        re_printf("warning","Clock name (%s) is invalid\n", clock_name.c_str());
    }
}//}}}

///Force value to register
void ATU5011::ForceRegister (const std::string module_id, const std::string block_id, const std::string reg_name, const unsigned int reg_value)
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
            RegisterHandlerWrapper(module_id, block_id, cmd);
        } else {
            re_printf("warning","Register name (%s) is invalid\n", reg_name.c_str());
        }
    } else {
        re_printf("warning","Register name (%s) is invalid\n", reg_name.c_str());
    }
}//}}}

///Release forced value after forcing registers
void ATU5011::ReleaseRegister (const std::string module_id, const std::string block_id, const std::string reg_name)
{//{{{
    std::vector<std::string> word_vector;
    std::vector<std::string> cmd;
    if (reg_name != "") {
        SeparateString(word_vector, reg_name);
        if (word_vector.size() == 1) {
            cmd.push_back("reg");
            SeparateString(cmd, reg_name);
            cmd.push_back("release");
            RegisterHandlerWrapper(module_id, block_id, cmd);
        } else {
            re_printf("warning","Register name (%s) is invalid\n", reg_name.c_str());
        }
    } else {
        re_printf("warning","Register name (%s) is invalid\n", reg_name.c_str());
    }
}//}}}

///Write value to registers by software
void ATU5011::WriteRegister (const std::string module_id, const std::string block_id, const std::string reg_name, const unsigned int reg_value)
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
            RegisterHandlerWrapper(module_id, block_id, cmd);
        } else {
            re_printf("warning","Register name (%s) is invalid\n", reg_name.c_str());
        }
    } else {
        re_printf("warning","Register name (%s) is invalid\n", reg_name.c_str());
    }
}//}}}

///Read value of register by software
void ATU5011::ReadRegister (const std::string module_id, const std::string block_id, const std::string reg_name)
{//{{{
    std::vector<std::string> word_vector;
    std::vector<std::string> cmd;
    if (reg_name != "") {
        SeparateString(word_vector, reg_name);
        if (word_vector.size() == 1) {
            cmd.push_back("reg");
            SeparateString(cmd, reg_name);
            RegisterHandlerWrapper(module_id, block_id, cmd);
        } else {
            re_printf("warning","Register name (%s) is invalid\n", reg_name.c_str());
        }
    } else {
        re_printf("warning","Register name (%s) is invalid\n", reg_name.c_str());
    }
}//}}}

/// Wrapper for accessing registers
void ATU5011::RegisterHandlerWrapper (const std::string module_id, const std::string block_id, const std::vector<std::string> cmd)
{//{{{
    bool is_module_match = false;
    for (unsigned int i = 0; i < mModuleIdVector.size(); i++) {
        if (module_id == mModuleIdVector[i]) {
            is_module_match = true;
            break;
        }
    }
    
    if (is_module_match) {
        if (module_id == "common") {
            mATU5011_Func->RegisterHandler(cmd);
        } else if (module_id == "timerA") {
            mATU5_TimerA->RegisterHandlerTIA(cmd);
        } else if (module_id == "timerB") {
            mATU5_TimerB->RegisterHandlerTIB(cmd);
        } else {
            bool is_com_reg = false;
            bool is_block_match = false;
            unsigned int sub_id=0;
            if (block_id == "common") {
                is_com_reg = true;
                is_block_match = true;
            }
            std::map<std::string, unsigned int>::iterator it;
            for (it = mBlockIdMap.begin(); it != mBlockIdMap.end(); it++) {
                if (it->first == block_id) {
                    is_block_match = true;
                    sub_id = it->second;
                    break;
                }
            }
            if (is_block_match) {
                if (module_id == "timerC") {
                    mATU5_TimerC->mTimerC_Func->RegisterHandler(is_com_reg, sub_id, cmd);
                } else if (module_id == "timerD") {
                    mATU5_TimerD->mATU5_TIMERDFunc->RegisterHandler(is_com_reg, sub_id, cmd);
                } else if (module_id == "timerE") {
                    mATU5_TimerE->mATU5_TIMEREFunc->RegisterHandler(is_com_reg, sub_id, cmd);
                } else if (module_id == "timerF") {
                    mATU5_TimerF->mATU5_TIMERF_Func->RegisterHandler(is_com_reg, sub_id, cmd);
                } else if (module_id == "timerG") {
                    mATU5_TimerG->pFuncModel->RegisterHandler(is_com_reg, sub_id, cmd);
                }
            } else {
                re_printf("warning","Block id (%s) is invalid\n", block_id.c_str());
            }
        }
    } else {
        re_printf("warning","Module id (%s) is invalid\n", module_id.c_str());
    }
}//}}}

///List all registers name
void ATU5011::ListRegister (void)
{//{{{
    std::vector<std::string> cmd;
    cmd.push_back("reg");
    mATU5011_Func->RegisterHandler(cmd);
    mATU5_TimerA->RegisterHandlerTIA(cmd);
    mATU5_TimerB->RegisterHandlerTIB(cmd);
    mATU5_TimerC->mTimerC_Func->RegisterHandler(true, 0, cmd);
    mATU5_TimerD->mATU5_TIMERDFunc->RegisterHandler(true, 0, cmd);
    mATU5_TimerE->mATU5_TIMEREFunc->RegisterHandler(true, 0, cmd);
    mATU5_TimerF->mATU5_TIMERF_Func->RegisterHandler(true, 0, cmd);
    mATU5_TimerG->pFuncModel->RegisterHandler(true, 0, cmd);
}//}}}

///Dump help message of all parameters or commands
void ATU5011::Help (const std::string type)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, type);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "parameters") {
            SC_REPORT_INFO(this->basename(),"ATU5011_MessageLevel (fatal|error|warning|info)                       Set debug message level (Default: fatal|error).");
            SC_REPORT_INFO(this->basename(),"ATU5011_DumpRegisterRW (true/false)                                   Enable/disable dumping access register (Default: false).");
            SC_REPORT_INFO(this->basename(),"ATU5011_DumpInterrupt (true/false)                                    Enable/disable dumping interrupt information (Default: false).");
        } else if (word_vector[0] == "commands") {
            SC_REPORT_INFO(this->basename(),"ATU5011_AssertReset (reset_name, start_time, period)                  Assert and de-assert reset signal.");
            SC_REPORT_INFO(this->basename(),"ATU5011_SetCLKFreq (clock_name, freq, unit)                           Set clock frequency value of model.");
            SC_REPORT_INFO(this->basename(),"ATU5011_GetCLKFreq (clock_name)                                       Get clock frequency value of model.");
            SC_REPORT_INFO(this->basename(),"ATU5011_ForceRegister (module_name, block_name, reg_name, reg_value)  Force ATU5011’s register with setting value.");
            SC_REPORT_INFO(this->basename(),"ATU5011_ReleaseRegister (module_name, block_name, reg_name)           Release ATU5011’s register from force value.");
            SC_REPORT_INFO(this->basename(),"ATU5011_WriteRegister (module_name, block_name, reg_name, reg_value)  Write a value to a ATU5011’s register.");
            SC_REPORT_INFO(this->basename(),"ATU5011_ReadRegister (module_name, block_name, reg_name)              Read a value from a ATU5011’s register.");
            SC_REPORT_INFO(this->basename(),"ATU5011_ListRegister ()                                               Dump name of model's registers.");
        } else {
            re_printf("warning","The name (%s) of ATU5011_Help argument is wrong (commands or parameters)\n", type.c_str());
        }
    } else {
        re_printf("warning","The name (%s) of ATU5011_Help argument is wrong (commands or parameters)\n", type.c_str());
    }
}//}}}

///Set message level (fatal, error, warning, info)
void ATU5011::SetMessageLevel (const std::string msg_lv)
{//{{{
    std::vector<std::string> cmd;
    cmd.push_back("command");
    cmd.push_back("MessageLevel");
    SeparateString(cmd, msg_lv);
    std::string msg_return = handleCommand(cmd);
    if (msg_return != "") {
        SC_REPORT_INFO(this->basename(), msg_return.c_str());
    }
    std::string msg_ret = mATU5_TimerA->handleCommand(cmd);
    if (msg_ret != "") {
        SC_REPORT_INFO(mATU5_TimerA->basename(), msg_ret.c_str());
    }
    msg_ret = mATU5_TimerB->handleCommand(cmd);
    if (msg_ret != "") {
        SC_REPORT_INFO(mATU5_TimerB->basename(), msg_ret.c_str());
    }
    msg_ret = mATU5_TimerC->handleCommand(cmd);
    if (msg_ret != "") {
        SC_REPORT_INFO(mATU5_TimerC->basename(), msg_ret.c_str());
    }
    msg_ret = mATU5_TimerD->handleCommand(cmd);
    if (msg_ret != "") {
        SC_REPORT_INFO(mATU5_TimerD->basename(), msg_ret.c_str());
    }
    msg_ret = mATU5_TimerE->handleCommand(cmd);
    if (msg_ret != "") {
        SC_REPORT_INFO(mATU5_TimerE->basename(), msg_ret.c_str());
    }
    msg_ret = mATU5_TimerF->handleCommand(cmd);
    if (msg_ret != "") {
        SC_REPORT_INFO(mATU5_TimerF->basename(), msg_ret.c_str());
    }
    msg_ret = mATU5_TimerG->handleCommand(cmd);
    if (msg_ret != "") {
        SC_REPORT_INFO(mATU5_TimerG->basename(), msg_ret.c_str());
    }

    //Set message level for Register IF class
    cmd.clear();
    cmd.push_back("reg");
    cmd.push_back("MessageLevel");
    SeparateString(cmd, msg_lv);
    mATU5011_Func->RegisterHandler(cmd);
    mATU5_TimerA->RegisterHandlerTIA(cmd);
    mATU5_TimerB->RegisterHandlerTIB(cmd);
    mATU5_TimerC->mTimerC_Func->RegisterHandler(true, 0, cmd);
    mATU5_TimerD->mATU5_TIMERDFunc->RegisterHandler(true, 0, cmd);
    mATU5_TimerE->mATU5_TIMEREFunc->RegisterHandler(true, 0, cmd);
    mATU5_TimerF->mATU5_TIMERF_Func->RegisterHandler(true, 0, cmd);
    mATU5_TimerG->pFuncModel->RegisterHandler(true, 0, cmd);

    //Set message level for each register in Register IF class
    cmd.clear();
    cmd.push_back("reg");
    cmd.push_back("*");
    cmd.push_back("MessageLevel");
    SeparateString(cmd, msg_lv);
    mATU5011_Func->RegisterHandler(cmd);
    mATU5_TimerA->RegisterHandlerTIA(cmd);
    mATU5_TimerB->RegisterHandlerTIB(cmd);
    mATU5_TimerC->mTimerC_Func->RegisterHandler(true, 0, cmd);
    mATU5_TimerD->mATU5_TIMERDFunc->RegisterHandler(true, 0, cmd);
    mATU5_TimerE->mATU5_TIMEREFunc->RegisterHandler(true, 0, cmd);
    mATU5_TimerF->mATU5_TIMERF_Func->RegisterHandler(true, 0, cmd);
    mATU5_TimerG->pFuncModel->RegisterHandler(true, 0, cmd);
}//}}}

///Enable/disable dumping message when users access registers
void ATU5011::DumpRegisterRW (const std::string is_enable)
{//{{{
    std::vector<std::string> cmd;
    cmd.push_back("reg");
    cmd.push_back("DumpRegisterRW");
    SeparateString(cmd, is_enable);
    mATU5011_Func->RegisterHandler(cmd);
    mATU5_TimerA->RegisterHandlerTIA(cmd);
    mATU5_TimerB->RegisterHandlerTIB(cmd);
    mATU5_TimerC->mTimerC_Func->RegisterHandler(true, 0, cmd);
    mATU5_TimerD->mATU5_TIMERDFunc->RegisterHandler(true, 0, cmd);
    mATU5_TimerE->mATU5_TIMEREFunc->RegisterHandler(true, 0, cmd);
    mATU5_TimerF->mATU5_TIMERF_Func->RegisterHandler(true, 0, cmd);
    mATU5_TimerG->pFuncModel->RegisterHandler(true, 0, cmd);
}//}}}

///Enable/disable dumping interrupt information
void ATU5011::DumpInterrupt (const std::string is_enable)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, is_enable);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "true") {
            mDumpInterrupt = true;
        } else if (word_vector[0] == "false") {
            mDumpInterrupt = false;
        } else {
            re_printf("warning","Invalid argument: ATU5011_DumpInterrupt %s\n", is_enable.c_str());
        }
    } else if (word_vector.size() == 0){
        //std::string temp = (mDumpInterrupt) ? "true" : "false";
        std::string temp="false";
        if(mDumpInterrupt){
            temp="true";
        }else{
            temp="false";
        }
        re_printf("info","ATU5011_DumpInterrupt %s\n",temp.c_str());
    } else {
        re_printf("warning","Invalid argument: ATU5011_DumpInterrupt %s\n", is_enable.c_str());
    }
}//}}}

///Check frequency value and frequency unit
void ATU5011::ConvertClockFreq (sc_dt::uint64 &freq_out, std::string &unit_out,
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
void ATU5011::SeparateString (std::vector<std::string> &vtr, const std::string msg)
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
void ATU5011::GetTimeResolution (sc_dt::uint64 &resolution_value, sc_time_unit &resolution_unit)
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
bool ATU5011::GetResetStatus (const std::string reset_name)
{//{{{
    bool reset_status = false;
    if (mIsResetHardActive[1] || mIsResetCmdActive[1] || mIsResetHardActive[0] || mIsResetCmdActive[0]) {
        reset_status = true;
    }
    return reset_status;
}//}}}

///Check clock period value
bool ATU5011::CheckClockPeriod (const std::string clock_name)
{//{{{
    bool clock_status = false;
    sc_dt::uint64 clock_period = 0;
    //clock_period = (clock_name == "CLK_LSB") ? mCLK_LSBPeriod : mCLK_HSBPeriod;
    if(clock_name == "CLK_LSB"){
        clock_period=mCLK_LSBPeriod;
    }else{
        clock_period=mCLK_HSBPeriod;
    }
    if (clock_period > 0) {
        clock_status = true;
    } else {
        clock_status = false;
        re_printf("info","%s frequency is zero\n", clock_name.c_str());
    }
    return clock_status;
}//}}}

///Handle CLK_LSB signal
void ATU5011::HandleCLK_LSBSignalMethod (void)
{//{{{
    sc_dt::uint64 freq_value = CLK_LSB.read();
    SetCLKFreq("CLK_LSB", freq_value, "Hz");
    mCLKLSBUpdatedEvent.notify(SC_ZERO_TIME);
}//}}}

///Handle CLK_HSB signal
void ATU5011::HandleCLK_HSBSignalMethod (void)
{//{{{ 
    sc_dt::uint64 freq_value = CLK_HSB.read();
    SetCLKFreq("CLK_HSB", freq_value, "Hz");
}//}}}

/// Handle TCLKA signal
void ATU5011::HandleTCLKASignalMethod (void)
{//{{{
    sc_dt::uint64 freq_value = TCLKA.read();
    SetCLKFreq("TCLKA", freq_value, "Hz");
}//}}}

/// Handle TCLKB signal
void ATU5011::HandleTCLKBSignalMethod (void)
{//{{{
    sc_dt::uint64 freq_value = TCLKB.read();
    SetCLKFreq("TCLKB", freq_value, "Hz");
}//}}}

///Handle presetz_bif signal
void ATU5011::HandlePresetz_bifSignalMethod (void)
{//{{{
    mResetCurVal[0] = presetz_bif.read();
    if (mResetCurVal[0] != mResetPreVal[0]) {       //mResetPreVal is "true" at initial state
        mResetPreVal[0] = mResetCurVal[0];
        // Reset signals are asynchronous
        mResetHardEvent[0].notify(SC_ZERO_TIME);
    }
}//}}}

///Handle presetz signal
void ATU5011::HandlePresetzSignalMethod (void)
{//{{{
    mResetCurVal[1] = presetz.read();
    if (mResetCurVal[1] != mResetPreVal[1]) {   //mResetPreVal is "true" at initial state
        mResetPreVal[1] = mResetCurVal[1];
        // Reset signals are asynchronous
        mResetHardEvent[1].notify(SC_ZERO_TIME);
    }
}//}}}

///Process reset function when reset port is active
void ATU5011::HandleResetHardMethod (const unsigned int reset_id)
{//{{ {
    if (mResetCurVal[reset_id] == emResetActive) {
        mIsResetHardActive[reset_id] = true;
        if (reset_id == 0) {
            re_printf("info","The reset port presetz_bif is asserted.\n");
            this->EnableReset("presetz_bif", mIsResetHardActive[reset_id]);
        } else {
            re_printf("info","The reset port presetz is asserted.\n");
            this->EnableReset("presetz", mIsResetHardActive[reset_id]);
        }
    } else {
        if (mIsResetHardActive[reset_id]) {
            mIsResetHardActive[reset_id] = false;
            if (reset_id == 0) {
                re_printf("info","The reset port presetz_bif is de-asserted.\n");
                this->EnableReset("presetz_bif", mIsResetHardActive[reset_id]);
            } else {
                re_printf("info","The reset port presetz is de-asserted.\n");
                this->EnableReset("presetz", mIsResetHardActive[reset_id]);
            }
        }
    }
    //Cancel AssertReset command when reset port is active
    if (mIsResetCmdReceive[reset_id]) {
        mResetCmdEvent[reset_id].cancel();
        mResetCmdCancelEvent[reset_id].cancel();
        mIsResetCmdReceive[reset_id] = false;
        mIsResetCmdActive[reset_id] = false;
    }
}//}}}

///Process reset function when reset command is active
void ATU5011::HandleResetCmdMethod (const unsigned int reset_id)
{//{{{
    std::string reset_name = "presetz_bif";
    if (reset_id == 0) {
        reset_name = "presetz_bif";
    } else { //reset_id == 1
        reset_name = "presetz";
    }
    mIsResetCmdActive[reset_id] = true;
    re_printf("info","The model is reset by AssertReset command of %s.\n", reset_name.c_str());
    this->EnableReset(reset_name, mIsResetCmdActive[reset_id]);
    mResetCmdCancelEvent[reset_id].notify(mResetCmdPeriod[reset_id], SC_NS);
}//}}}

///Cancel reset function when reset command is active
void ATU5011::CancelResetCmdMethod (const unsigned int reset_id)
{//{{{
    std::string reset_name = "presetz_bif";
    if (reset_id == 0) {
        reset_name = "presetz_bif";
    } else { //reset_id == 1
        reset_name = "presetz";
    }
    mIsResetCmdActive[reset_id] = false;
    mIsResetCmdReceive[reset_id] = false;
    re_printf("info","Reset period of %s is over.\n", reset_name.c_str());
    this->EnableReset(reset_name, mIsResetCmdActive[reset_id]);
}//}}}

///Execute reset operation
void ATU5011::EnableReset (const std::string reset_name, const bool is_active)
{//{{{
    mATU5011_Func->Reset(is_active);
    mATU5_TimerA->EnableReset(is_active);
    mATU5_TimerB->EnableReset(is_active);
    mATU5_TimerC->EnableReset(is_active);
    mATU5_TimerD->EnableReset(is_active);
    mATU5_TimerE->EnableReset(is_active);
    mATU5_TimerF->EnableReset(is_active);
    mATU5_TimerG->EnableReset(is_active);
    if (is_active) {
        // Cancel events
        CancelEvents();
        // Initialize variables
        Initialize();
        // Initialize internal signals & output
        for (unsigned int i = 0; i < emCLK_BUSnum; i++) {
            mWriteClockBusEvent[i].notify(SC_ZERO_TIME);
        }
        mWriteNoiseCancelClkEvent.notify(SC_ZERO_TIME);
        // DMA-AD select
        for (unsigned int i = 0; i < emWrapper_DMAnum; i++){
            mWriteTrgDMAReqEvent[i].notify();
        }
        mWriteat_saradtrgEvent.notify();
    }
}//}}}

/// Cancel operation events
void ATU5011::CancelEvents (void)
{//{{{
    for (unsigned int i = 0; i < emClkBusPSCNum; i++) { // removed emNoiseCancelPCSNum by chuongle Jul/28/2018
        mStartPrescalerEvent[i].cancel();
        mStopPrescalerEvent[i].cancel();
    }
    for (unsigned int i = 0; i < emTCLKnum; i++) {
        mTCLKUpdateEvent[i].cancel();
    }
    mUpdateDMATrgSelectEvent.cancel();
    for (unsigned int i = 0; i < emWrapper_DMAnum; i++) {
        mWriteTrgDMAReqEvent[i].cancel();
    }
    mAGCK1AssertEvent.cancel();
    mAGCK1DeassertEvent.cancel();
}//}}}

/*********************************
// Function     : GetNoiseCancelMode
// Description  : Return noise cancellation mode
// Parameter    :
//      timer_name          Timer name ("Timer A", "Timer C", "Timer F")
// Return value : noise cancellation mode
//      0                   Premature-transition cancellation mode
//      1                   Minimum time-at-level cancellation mode
//      2                   Level-accumulated cancellation mode
**********************************/
unsigned int ATU5011::GetNoiseCancelMode (const std::string timer_name)
{//{{{
    return mATU5011_Func->GetNoiseCancelMode(timer_name);
}//}}}

///Calculate synchronous time to next clock edge
double ATU5011::CalculateNextClockEdge (const double clock_period, const bool is_pos, const double current_time, const double start_time)
{//{{{
    double period_num = 0;
    // Returns 0 if input clock is 0
    if (clock_period == 0) {
        return 0;
    }
    period_num = (current_time - start_time) / clock_period;
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
    double mod_period_num_db = 0;
    if (is_pos) {
        mod_period_num_db = (double)mod_period_num;
    } else {
        mod_period_num_db = (double)mod_period_num + 0.5;
    }
    return ((mod_period_num_db * (double)clock_period) - (current_time - start_time));
}//}}}

/// Prescaler is started/stopped
void ATU5011::EnablePrescaler (const bool is_enable)
{//{{{
    double current_time = sc_time_stamp().to_double();
    if ((!mPrePSCE) && (is_enable)) {
        // Start Prescaler
        for (unsigned int i = 0; i < emClkBusPSCNum; i++) { // removed emNoiseCancelPCSNum by chuongle Jul/28/2018
            mStartPrescalerEvent[i].notify(CalculateNextClockEdge((double)mCLK_LSBPeriod, true, current_time, mTimeCLKChangePoint[0]) + mCLK_LSBPeriod + mPSCSyncTime[i], mTimeResolutionUnit);
        }
    } else if ((mPrePSCE) && (!is_enable)) {
        // Stop Prescaler
        for (unsigned int i = 0; i < emClkBusPSCNum; i++) { // removed emNoiseCancelPCSNum by chuongle Jul/28/2018
            mStopPrescalerEvent[i].notify(CalculateNextClockEdge((double)mCLK_LSBPeriod, true, current_time, mTimeCLKChangePoint[0]) + mCLK_LSBPeriod, mTimeResolutionUnit);
        }
    }
    mPrePSCE = is_enable;
}//}}}

void ATU5011::StartPrescalerMethod (unsigned int ch_id)
{//{{{
    if (!GetResetStatus("presetz")) {
        if (ch_id < emClkBusPSCNum) {
            // Calculate Clock Bus period
            unsigned int ratio = mATU5011_Func->GetPrescalerRatio(ch_id);
            mClkBusPeriod[ch_id] = mCLK_LSBPeriod * (ratio + 1);
            mWriteClockBusEvent[ch_id].notify(SC_ZERO_TIME);
        } else {
            // Do nothing
        }
    }
}//}}}

void ATU5011::StopPrescalerMethod (unsigned int ch_id)
{//{{{
    if (!GetResetStatus("presetz") && CheckClockPeriod("CLK_LSB")) {
        // Calculate next rising edge
        double current_time = sc_time_stamp().to_double();
        if (ch_id < emClkBusPSCNum) {
            mPSCSyncTime[ch_id] = CalculateNextClockEdge((double)mClkBusPeriod[ch_id], true, current_time, mClkBusStartTime[ch_id]);
            mClkBusPeriod[ch_id] = 0;
            mWriteClockBusEvent[ch_id].notify(SC_ZERO_TIME);
        } else {
            // Do nothing
        }
    }
}//}}}

void ATU5011::WriteClockBusMethod (unsigned int ch_id)
{//{{{
    if (((ch_id == 4) && (mATU5011_Func->GetClkEdgeSetting(true) == emFallingEdge)) ||
          ((ch_id == 5) && (!mATU5011_Func->GetClkBus5Setting()) && (mATU5011_Func->GetClkEdgeSetting(false) == emFallingEdge))) {
        mClkBusPeriod[ch_id] = mTCLKPeriod[ch_id - 4];
    }
    if (!GetResetStatus("presetz")) {
        if ((mClkBusPeriod[ch_id] > 0) && (mIsClkBusZero[ch_id])) {
            if (mIsTimerBSel) {
                mATU5_TimerB->mIsWrapperCall = true;
            }
            mEnableTimerABEvent.notify(SC_ZERO_TIME);
        }
    }
    // Store start time
    mClkBusStartTime[ch_id] = sc_time_stamp().to_double();
    clk_bus_sig[ch_id]->write(mClkBusPeriod[ch_id]);
    mATU5_TimerG->setClkBusPeriod((double)mClkBusPeriod[ch_id], ch_id);
}//}}}

unsigned int ATU5011::GetClkBus5Setting()
{//{{{
    return mATU5011_Func->GetClkBus5Setting();
}//}}}

void ATU5011::WriteNoiseCancelClkMethod (void)
{//{{{
    // Store start time
    nc_clk_sig.write(mNoiseCancelClkPeriod);
}//}}}

void ATU5011::PrescalerSetupChange (const unsigned int ch_id)
{//{{{
    double current_time = sc_time_stamp().to_double();
    if (mATU5011_Func->GetEnableStatus(emPSCE)) {
       if(mClkBusPeriod[ch_id]) {
          mStartPrescalerEvent[ch_id].notify(CalculateNextClockEdge((double)mClkBusPeriod[ch_id], true, current_time, mClkBusStartTime[ch_id]), mTimeResolutionUnit);
       } else { // Added by TienTran fixing bug divide zeros when mClkBusPeriod equal zeros before writing to Prescaler.
          mStartPrescalerEvent[ch_id].notify(CalculateNextClockEdge((double)mCLK_LSBPeriod, true, current_time, mTimeCLKChangePoint[0]), mTimeResolutionUnit);
       }
    }
}//}}}

void ATU5011::CLK_LSBUpdate (void)
{//{{{
    double current_time = sc_time_stamp().to_double();
    if (mATU5011_Func->GetEnableStatus(emPSCE)) {
        for (unsigned int i = 0; i < emClkBusPSCNum; i++) {
            if (mCLK_LSBPeriod > 0) {
                if (mClkBusPeriod[i] > 0) {
                    mStartPrescalerEvent[i].notify(CalculateNextClockEdge((double)mClkBusPeriod[i], true, current_time, mClkBusStartTime[i]), mTimeResolutionUnit);
                } else {
                    mStartPrescalerEvent[i].notify(CalculateNextClockEdge((double)mCLK_LSBPeriod, true, current_time, mTimeCLKChangePoint[0]), mTimeResolutionUnit);
                }
            } else {
                mStartPrescalerEvent[i].notify(SC_ZERO_TIME);
            }
        }

    }
    mTimeCLKChangePoint[0] = current_time;
    mATU5_TimerG->setPCLKPeriod((double)mCLK_LSBPeriod);
    CLK_LSBperiod_sig.write(mCLK_LSBPeriod);

    // Updating nc_clk_1
    NCClkSetupChange(); // added by chuongle Jul/28/2018
}//}}}

void ATU5011::TCLKUpdateMethod (unsigned int type)
{//{{{
    bool is_ext_clk = true;
    unsigned int cbeg = 0;
    if (type == emTCLKA) {
        cbeg = mATU5011_Func->GetClkEdgeSetting(true);
    } else {
        if (mATU5011_Func->GetClkBus5Setting()) {
            is_ext_clk = false;
        } else {
            cbeg = mATU5011_Func->GetClkEdgeSetting(false);
        }
    }
    
    if (is_ext_clk) {
        // Use external clock bus
        if (mTCLKPeriod[type] > 0) {
            switch (cbeg) {
                case emRisingEdge:
                    mClkBusPeriod[type+4] = mTCLKPeriod[type];
                    mWriteClockBusEvent[type+4].notify(SC_ZERO_TIME);
                    break;
                case emFallingEdge:
                    mWriteClockBusEvent[type+4].notify((double)(mTCLKPeriod[type]/2), mTimeResolutionUnit);
                    break;
                case emBothEdge:
                    mClkBusPeriod[type+4] = mTCLKPeriod[type]/2;
                    mWriteClockBusEvent[type+4].notify(SC_ZERO_TIME);
                    break;
                default:        // emNoneEdge
                    mClkBusPeriod[type+4] = 0;
                    mWriteClockBusEvent[type+4].notify(SC_ZERO_TIME);
                    break;
            }
        } else {
            mClkBusPeriod[type+4] = 0;
            mWriteClockBusEvent[type+4].notify(SC_ZERO_TIME);
        }
    } else {
        // Use TimerB::AGCKM
        mClkBusPeriod[5] = timerB_AGCKM_sig.read();
        mWriteClockBusEvent[5].notify(SC_ZERO_TIME);
    }
}//}}}

void ATU5011::ClkBusSetupChange (void)
{//{{{
    double current_time = sc_time_stamp().to_double();
    if (mClkBusPeriod[emTCLKA+4] > 0) {
        mTCLKUpdateEvent[emTCLKA].notify(CalculateNextClockEdge((double)mClkBusPeriod[emTCLKA+4], true, current_time, mClkBusStartTime[emTCLKA+4]), mTimeResolutionUnit);
    } else {
        mTCLKUpdateEvent[emTCLKA].notify(SC_ZERO_TIME);
    }
    if (mClkBusPeriod[emTCLKB+4] > 0) {
        mTCLKUpdateEvent[emTCLKB].notify(CalculateNextClockEdge((double)mClkBusPeriod[emTCLKB+4], true, current_time, mClkBusStartTime[emTCLKB+4]), mTimeResolutionUnit);
    } else {
        mTCLKUpdateEvent[emTCLKB].notify(SC_ZERO_TIME);
    }
}//}}}

void ATU5011::NCClkSetupChange (void)
{//{{{
    // Calculate Noise Canceler clock
    if (mATU5011_Func->GetNCClkSel()) {
        mNoiseCancelClkPeriod = mCLK_LSBPeriod;
    } else {
        mNoiseCancelClkPeriod = mCLK_LSBPeriod * 128;
    }
    mWriteNoiseCancelClkEvent.notify(SC_ZERO_TIME);
}//}}}

void ATU5011::CheckClkBusSel (const unsigned int timer_id)
{//{{{
    unsigned int clock_sel = 0;
    if (timer_id == emTimerA) {
        clock_sel = mATU5_TimerA->mATU5_TIA_Func->GetCLKBusSel();
        mIsTimerASel = true;
    } else {
        // emTimerB
        clock_sel = mATU5_TimerB->mATU5_TIB_Func->GetCLKBusSel();
        mIsTimerBSel = true;
    }
    if (clock_sel < 6) {
        if (mClkBusPeriod[clock_sel] == 0) {
            mIsClkBusZero[clock_sel] = true;
        }
    }
}//}}}

void ATU5011::EnableTimerABMethod (void)
{//{{{
    unsigned int clock_sel = 0;
    if (mIsTimerASel) {
        clock_sel = mATU5_TimerA->mATU5_TIA_Func->GetCLKBusSel();
        if ((mClkBusPeriod[clock_sel] > 0) && (mIsClkBusZero[clock_sel])) {
            mATU5_TimerA->EnableTIA(true);
            mIsTimerASel = false;
            mIsClkBusZero[clock_sel] = false;
        }
    }
    if (mIsTimerBSel) {
        clock_sel = mATU5_TimerB->mATU5_TIB_Func->GetCLKBusSel();
        if ((mClkBusPeriod[clock_sel] > 0) && (mIsClkBusZero[clock_sel])) {
            mATU5_TimerB->EnableTIB(true);
            mIsTimerBSel = false;
            mIsClkBusZero[clock_sel] = false;
        }
    }
}//}}}

void ATU5011::HandleAGCK1SignalMethod (void)
{//{{{
    if (!GetResetStatus("presetz") && CheckClockPeriod("CLK_LSB")) {
        mAGCK1StartTime = sc_time_stamp().to_double();
        double agck1_period = (double)timerB_AGCK1_sig.read();
        unsigned int trg_sel_dma = mATU5011_Func->GetRegSelectSetting(emTRGSELDMA00 + mIsDMASelectHigh);
        unsigned int trg_sel_ad = mATU5011_Func->GetRegSelectSetting(emTRGSELAD);
        if ((agck1_period != 0) && ((trg_sel_dma == 1) || (trg_sel_ad == 1))) {
            mAGCK1AssertEvent.notify(SC_ZERO_TIME);
        }
    }
}//}}}

void ATU5011::HandleAGCK1AssertMethod (void)
{//{{{
    if (!GetResetStatus("presetz") && CheckClockPeriod("CLK_LSB")) {
        double agck1_period = (double)timerB_AGCK1_sig.read();
        unsigned int trg_sel_dma = mATU5011_Func->GetRegSelectSetting(emTRGSELDMA00 + mIsDMASelectHigh);
        unsigned int trg_sel_ad = mATU5011_Func->GetRegSelectSetting(emTRGSELAD);
        if ((agck1_period != 0) && ((trg_sel_dma == 1) || (trg_sel_ad == 1))) {
            mAGCK1boolValue = true;
            mAGCK1ChangeEvent.notify(SC_ZERO_TIME);
            mAGCK1DeassertEvent.notify((double)mCLK_LSBPeriod, mTimeResolutionUnit);
        }
    }
}//}}}

void ATU5011::HandleAGCK1DeassertMethod (void)
{//{{{
    if (!GetResetStatus("presetz") && CheckClockPeriod("CLK_LSB")) {
        double agck1_period = (double)timerB_AGCK1_sig.read();
        unsigned int trg_sel_dma = mATU5011_Func->GetRegSelectSetting(emTRGSELDMA00 + mIsDMASelectHigh);
        unsigned int trg_sel_ad = mATU5011_Func->GetRegSelectSetting(emTRGSELAD);
        mAGCK1boolValue = false;
        mAGCK1ChangeEvent.notify(SC_ZERO_TIME);
    }
}//}}}

void ATU5011::HandleDMATrgSelectChangeMethod (void)
{//{{{
    double current_time = sc_time_stamp().to_double();
    mUpdateDMATrgSelectEvent.notify(CalculateNextClockEdge((double)mCLK_LSBPeriod, true, current_time, mTimeCLKChangePoint[0]) + mCLK_LSBPeriod, mTimeResolutionUnit);
}//}}}

void ATU5011::UpdateDMATrgSelectMethod (void)
{//{{{
    if (!GetResetStatus("presetz") && CheckClockPeriod("CLK_LSB")) {
        mIsDMASelectHigh = !mIsDMASelectHigh;
        mATU5011_Func->SetTrgStatusDMAReg(mIsDMASelectHigh);
    }
}//}}}

void ATU5011::HandleDMA0TriggerInputMethod (void)
{//{{{
    bool value[13];
    value[0] = false;
    value[1] = mAGCK1boolValue;
    value[2] = mATU5_TimerG->cmi_g[1]->read();
    value[3] = mATU5_TimerG->cmi_g[2]->read();
    value[4] = pf_dmai_s.read();
    value[5] = false;
    value[6] = false;
    value[7] = false;
    value[8] = pf_dmai0_a.read();
    value[9] = pf_dmai1_a.read();
    value[10] = pf_dmai2_a.read();
    value[11] = pf_dmai3_a.read();
    value[12] = pf_dmai4_a.read();
    
    unsigned int trg_sel_dma = mATU5011_Func->GetRegSelectSetting(emTRGSELDMA00 + mIsDMASelectHigh);
    bool invalid = false;
    if (((5 <= trg_sel_dma) && (trg_sel_dma <= 7)) || (13 <= trg_sel_dma)) {
        invalid = true;
    }
    if (invalid) {
        re_printf("warning","The setting in TRGSELDMA0%d register is prohibited.\n", mIsDMASelectHigh & 0x1);
    } else {
        if (value[trg_sel_dma]) {
            mINTATUCTRLDMAREQ[mIsDMASelectHigh & 0x1] = true;
            mWriteTrgDMAReqEvent[mIsDMASelectHigh & 0x1].notify((double)mCLK_LSBPeriod, mTimeResolutionUnit);
        } else {
            mINTATUCTRLDMAREQ[mIsDMASelectHigh & 0x1] = false;
            mWriteTrgDMAReqEvent[mIsDMASelectHigh & 0x1].notify((double)mCLK_LSBPeriod, mTimeResolutionUnit);
        }
    }
}//}}}

void ATU5011::HandleDMA1TriggerInputMethod (void)
{//{{{
    bool value[3];
    value[0] = false;
    value[1] = timerA_evt_1_0_sig.read();
    value[2] = mATU5_TimerG->cmi_g[3]->read();
    
    unsigned int trg_sel_dma = mATU5011_Func->GetRegSelectSetting(emTRGSELDMA10 + mIsDMASelectHigh);
    bool invalid = false;
    if (3 <= trg_sel_dma) {
        invalid = true;
    }
    if (invalid) {
        re_printf("warning","The setting in TRGSELDMA1%d register is prohibited.\n", mIsDMASelectHigh & 0x1);
    } else {
        if (trg_sel_dma == 1) {
            if (value[trg_sel_dma]) {
                mINTATUCTRLDMAREQ[2 + (mIsDMASelectHigh & 0x1)] = true;
                mWriteTrgDMAReqEvent[2 + (mIsDMASelectHigh & 0x1)].notify((double)mCLK_LSBPeriod * 2, mTimeResolutionUnit);
            } else {
                mINTATUCTRLDMAREQ[2 + (mIsDMASelectHigh & 0x1)] = false;
                mWriteTrgDMAReqEvent[2 + (mIsDMASelectHigh & 0x1)].notify((double)mCLK_LSBPeriod * 2, mTimeResolutionUnit);
            }
        } else {
            if (value[trg_sel_dma]) {
                mINTATUCTRLDMAREQ[2 + (mIsDMASelectHigh & 0x1)] = true;
                mWriteTrgDMAReqEvent[2 + (mIsDMASelectHigh & 0x1)].notify((double)mCLK_LSBPeriod, mTimeResolutionUnit);
            } else {
                mINTATUCTRLDMAREQ[2 + (mIsDMASelectHigh & 0x1)] = false;
                mWriteTrgDMAReqEvent[2 + (mIsDMASelectHigh & 0x1)].notify((double)mCLK_LSBPeriod, mTimeResolutionUnit);
            }
        }
    }
}//}}}

void ATU5011::Writeat_saradtrgMethod (void)
{//{{{
    at_saradtrg.write(m_at_saradtrg);
}//}}}

void ATU5011::WriteTrgDMAReqMethod (unsigned int id)
{//{{{
    switch (id) {
        case 0 :
            INTATUCTRLDMAREQ00.write(mINTATUCTRLDMAREQ[id]);
            break;
        case 1 :
            INTATUCTRLDMAREQ01.write(mINTATUCTRLDMAREQ[id]);
            break;
        case 2 :
            INTATUCTRLDMAREQ10.write(mINTATUCTRLDMAREQ[id]);
            break;
        default :       // 3
            INTATUCTRLDMAREQ11.write(mINTATUCTRLDMAREQ[id]);
            break;
    }
}//}}}

void ATU5011::HandleTRGSELDMA0regChangeMethod (unsigned int id)
{//{{{
    if ((bool)id == mIsDMASelectHigh) {
        bool value[13];
        value[0] = false;
        value[1] = mAGCK1boolValue;
        value[2] = mATU5_TimerG->cmi_g[1]->read();
        value[3] = mATU5_TimerG->cmi_g[2]->read();
        value[4] = pf_dmai_s.read();
        value[5] = false;
        value[6] = false;
        value[7] = false;
        value[8] = pf_dmai0_a.read();
        value[9] = pf_dmai1_a.read();
        value[10] = pf_dmai2_a.read();
        value[11] = pf_dmai3_a.read();
        value[12] = pf_dmai4_a.read();
        
        unsigned int trg_sel_dma = mATU5011_Func->GetRegSelectSetting(emTRGSELDMA00 + id);
        bool invalid = false;
        if (((5 <= trg_sel_dma) && (trg_sel_dma <= 7)) || (13 <= trg_sel_dma)) {
            invalid = true;
        }
        if (invalid) {
            re_printf("warning","The setting in TRGSELDMA0%d register is prohibited.\n", id);
        } else {
            if (value[trg_sel_dma]) {
                mINTATUCTRLDMAREQ[id] = true;
                mWriteTrgDMAReqEvent[id].notify(SC_ZERO_TIME);
            } else {
                mINTATUCTRLDMAREQ[id] = false;
                mWriteTrgDMAReqEvent[id].notify(SC_ZERO_TIME);
            }
        }
    }
}//}}}

void ATU5011::HandleTRGSELDMA1regChangeMethod (unsigned int id)
{//{{{
    if ((bool)id == mIsDMASelectHigh) {
        bool value[3];
        value[0] = false;
        value[1] = timerA_evt_1_0_sig.read();
        value[2] = mATU5_TimerG->cmi_g[3]->read();
        
        unsigned int trg_sel_dma = mATU5011_Func->GetRegSelectSetting(emTRGSELDMA10 + id);
        bool invalid = false;
        if (3 <= trg_sel_dma) {
            invalid = true;
        }
        if (invalid) {
            re_printf("warning","The setting in TRGSELDMA1%d register is prohibited.\n", id & 0x1);
        } else {
            if (value[trg_sel_dma]) {
                mINTATUCTRLDMAREQ[id + 2] = true;
                mWriteTrgDMAReqEvent[id + 2].notify(SC_ZERO_TIME);
            } else {
                mINTATUCTRLDMAREQ[id + 2] = false;
                mWriteTrgDMAReqEvent[id + 2].notify(SC_ZERO_TIME);
            }
        }
    }
}//}}}

void ATU5011::HandleSARADTriggerInputMethod (void)
{//{{{
    bool value[4];
    value[0] = false;
    value[1] = mAGCK1boolValue;
    value[2] = mATU5_TimerG->cmi_g[1]->read();
    value[3] = mATU5_TimerG->cmi_g[2]->read();
    
    unsigned int trg_sel_ad = mATU5011_Func->GetRegSelectSetting(emTRGSELAD);
    m_at_saradtrg = value[trg_sel_ad];
    mWriteat_saradtrgEvent.notify((double)mCLK_LSBPeriod, mTimeResolutionUnit);
}//}}}


void ATU5011::HandleAGCKMSignalMethod (void)
{//{{{
    if (mATU5011_Func->GetClkBus5Setting()) {
        mClkBusPeriod[5] = timerB_AGCKM_sig.read();
        mWriteClockBusEvent[5].notify(SC_ZERO_TIME);
    }
}//}}}

void ATU5011::NotifyEvent (const unsigned int event, const unsigned int channel)
{//{{{
    switch (event) {
        case emTRGSELDMA00 :
            mHandleTRGSELDMAregChangeEvent[channel].notify(SC_ZERO_TIME);
            break;
        default :   // emTRGSELAD
            mHandleTRGSELADregChangeEvent.notify(SC_ZERO_TIME);
            break;
    }
}//}}}


/// Initialize internal variables
void ATU5011::Initialize (void)
{//{{{
    mPrePSCE = false;
    mIsTimerASel = false;
    mIsTimerBSel = false;
    mIsDMASelectHigh = false;
    for (unsigned int i = 0; i < emClkBusPSCNum; i++) {
        mPSCSyncTime[i] = 0;
    }
    for (unsigned int i = 0; i < emCLK_BUSnum; i++) {
        mClkBusPeriod[i] = 0;
        mIsClkBusZero[i] = false;
    }
    mAGCK1boolValue = false;
    for (unsigned int i = 0; i < emWrapper_DMAnum; i++){
        mINTATUCTRLDMAREQ[i] = false;
    }
    m_at_saradtrg = false;
}//}}}

// vim600: set foldmethod=marker :
