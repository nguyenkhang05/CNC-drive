// ---------------------------------------------------------------------
// $Id: ATU5011_GLUE.h 1196 2019-10-17 05:33:04Z chuonghoangle $
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
#ifndef __ATU5011_GLUE_H__
#define __ATU5011_GLUE_H__

#include "BusSlaveBase.h"
#include "ATU5011_GLUE_AgentController.h"

class ATU5011_GLUE_Func;

class ATU5011_GLUE: public sc_module,
            public ATU5011_GLUE_AgentController,
            public BusSlaveBase<32,1>
{
private:
    enum eResetActiveLevel {    // Define active level of reset signal
        emResetActive = 0,
        emResetSignalNum = 1
    };

    ATU5011_GLUE_Func *mATU5011_GLUE_Func;            // ATU5011_GLUE_Func class instance
    
    /// Declare variables
    sc_dt::uint64 mPCLKPeriod;          //Period value of clock
    sc_dt::uint64 mPCLKFreq;            //Frequency value of clock
    sc_dt::uint64 mPCLKOrgFreq;         //Original frequency value of clock
    std::string mPCLKFreqUnit;          //Frequency unit of clock
    
    sc_dt::uint64 mTimeResolutionValue;    //Time resolution value of simulation
    sc_time_unit mTimeResolutionUnit;      //Time resolution unit of simulation

    bool mResetCurVal;        //Store current value of reset signals
    bool mResetPreVal;        //Store previous value of reset signals
    bool mIsResetHardActive;  //Reset operation status of reset signals
    bool mIsResetCmdReceive;  //Receive status of AssertReset command of reset signals
    bool mIsResetCmdActive;   //Active status of AssertReset command of reset signals
    double mResetCmdPeriod;   //Period of AssertReset command of reset signals

    bool mDumpInterrupt;            // Enable/Disable dumping the interrupt info

    /// Declare events
    sc_event mResetHardEvent;         //Call HandleResetHardMethod when reset is active
    sc_event mResetCmdEvent;          //Call HandleResetCmdMethod when AssertReset is called
    sc_event mResetCmdCancelEvent;    //Call CancelResetCmdMethod when AssertReset is over

    sc_event mINTSELAregChangeEvent;
    sc_event mINTSELDregChangeEvent[8];
    sc_event mDMASELBregChangeEvent;
    sc_event mDMASELCDregChangeEvent[8];
    sc_event mDMASELEregChangeEvent;
    
    sc_event mDFEENTQregChangeEvent[emTQNum];
    sc_event mDFESELregChangeEvent[emTQNum];
    sc_event mDFESELD1TregChangeEvent;
    sc_event mDSSELDSTSregChangeEvent;
    sc_event mCASELCATSregChangeEvent;
    sc_event mP5SSELregChangeEvent;

public:
    /// Declare signals
    // Input ports
    sc_in<sc_dt::uint64> PCLK;
    sc_in<bool> PRESETZ;
    // TimerA
    sc_in<bool> INTATUAICIA0;
    sc_in<bool> INTATUAICIA1;
    sc_in<bool> INTATUAICIA2;
    sc_in<bool> INTATUAICIA3;
    sc_in<bool> INTATUAICIA4;
    sc_in<bool> INTATUAICIA5;
    sc_in<bool> INTATUAICIA6;
    sc_in<bool> INTATUAICIA7;
    // OVIA
    sc_in<bool> OVIA;
    // TimerB
    sc_in<bool> INTATUBCMIB0;
    sc_in<bool> INTATUBCMIB1;
    sc_in<bool> INTATUBCMIB6;
    sc_in<bool> INTATUBCMIB10;
    sc_in<bool> INTATUBCMIB11;
    sc_in<bool> INTATUBCMIB12;
    sc_in<bool> INTATUBCMIB6M;
    sc_in<bool> INTATUBCMIB66M;
    sc_in<bool> INTATUBICIB0;
	// TimerC
    // OVICx
    sc_in<bool> OVIC0;
    sc_in<bool> OVIC1;
    sc_in<bool> OVIC2;
    sc_in<bool> OVIC3;
    sc_in<bool> OVIC4;
    sc_in<bool> OVIC5;
    sc_in<bool> OVIC6;
    sc_in<bool> OVIC7;
    sc_in<bool> OVIC8;
    sc_in<bool> OVIC9;
    sc_in<bool> OVIC10;
	sc_in<bool> OVIC11;
	sc_in<bool> OVIC12;
	sc_in<bool> OVIC13;

    // IMIGCxy - GRCxy input capture/compare match interrupt
    sc_in<bool> IMIGC00;
    sc_in<bool> IMIGC01;
    sc_in<bool> IMIGC02;
    sc_in<bool> IMIGC03;
    sc_in<bool> IMIGC10;
    sc_in<bool> IMIGC11;
    sc_in<bool> IMIGC12;
    sc_in<bool> IMIGC13;
    sc_in<bool> IMIGC20;
    sc_in<bool> IMIGC21;
    sc_in<bool> IMIGC22;
    sc_in<bool> IMIGC23;
    sc_in<bool> IMIGC30;
    sc_in<bool> IMIGC31;
    sc_in<bool> IMIGC32;
    sc_in<bool> IMIGC33;
    sc_in<bool> IMIGC40;
    sc_in<bool> IMIGC41;
    sc_in<bool> IMIGC42;
    sc_in<bool> IMIGC43;
    sc_in<bool> IMIGC50;
    sc_in<bool> IMIGC51;
    sc_in<bool> IMIGC52;
    sc_in<bool> IMIGC53;
    sc_in<bool> IMIGC60;
    sc_in<bool> IMIGC61;
    sc_in<bool> IMIGC62;
    sc_in<bool> IMIGC63;
    sc_in<bool> IMIGC70;
    sc_in<bool> IMIGC71;
    sc_in<bool> IMIGC72;
    sc_in<bool> IMIGC73;
    sc_in<bool> IMIGC80;
    sc_in<bool> IMIGC81;
    sc_in<bool> IMIGC82;
    sc_in<bool> IMIGC83;
    sc_in<bool> IMIGC90;
    sc_in<bool> IMIGC91;
    sc_in<bool> IMIGC92;
    sc_in<bool> IMIGC93;
    sc_in<bool> IMIGC100;
    sc_in<bool> IMIGC101;
    sc_in<bool> IMIGC102;
    sc_in<bool> IMIGC103;
	sc_in<bool> IMIGC110;
	sc_in<bool> IMIGC111;
	sc_in<bool> IMIGC112;
	sc_in<bool> IMIGC113;
	sc_in<bool> IMIGC120;
	sc_in<bool> IMIGC121;
	sc_in<bool> IMIGC122;
	sc_in<bool> IMIGC123;
	sc_in<bool> IMIGC130;
	sc_in<bool> IMIGC131;
	sc_in<bool> IMIGC132;
	sc_in<bool> IMIGC133;
    // IMIOCxy - OCRCxy input capture/compare match interrupt
    sc_in<bool> IMIOC00;
    sc_in<bool> IMIOC01;
    sc_in<bool> IMIOC02;
    sc_in<bool> IMIOC03;
    sc_in<bool> IMIOC10;
    sc_in<bool> IMIOC11;
    sc_in<bool> IMIOC12;
    sc_in<bool> IMIOC13;
    sc_in<bool> IMIOC20;
    sc_in<bool> IMIOC21;
    sc_in<bool> IMIOC22;
    sc_in<bool> IMIOC23;
    sc_in<bool> IMIOC30;
    sc_in<bool> IMIOC31;
    sc_in<bool> IMIOC32;
    sc_in<bool> IMIOC33;
    sc_in<bool> IMIOC40;
    sc_in<bool> IMIOC41;
    sc_in<bool> IMIOC42;
    sc_in<bool> IMIOC43;
    sc_in<bool> IMIOC50;
    sc_in<bool> IMIOC51;
    sc_in<bool> IMIOC52;
    sc_in<bool> IMIOC53;
    sc_in<bool> IMIOC60;
    sc_in<bool> IMIOC61;
    sc_in<bool> IMIOC62;
    sc_in<bool> IMIOC63;
    sc_in<bool> IMIOC70;
    sc_in<bool> IMIOC71;
    sc_in<bool> IMIOC72;
    sc_in<bool> IMIOC73;
    sc_in<bool> IMIOC80;
    sc_in<bool> IMIOC81;
    sc_in<bool> IMIOC82;
    sc_in<bool> IMIOC83;
    sc_in<bool> IMIOC90;
    sc_in<bool> IMIOC91;
    sc_in<bool> IMIOC92;
    sc_in<bool> IMIOC93;
    sc_in<bool> IMIOC100;
    sc_in<bool> IMIOC101;
    sc_in<bool> IMIOC102;
    sc_in<bool> IMIOC103;
	sc_in<bool> IMIOC110;
	sc_in<bool> IMIOC111;
	sc_in<bool> IMIOC112;
	sc_in<bool> IMIOC113;
	sc_in<bool> IMIOC120;
	sc_in<bool> IMIOC121;
	sc_in<bool> IMIOC122;
	sc_in<bool> IMIOC123;
	sc_in<bool> IMIOC130;
	sc_in<bool> IMIOC131;
	sc_in<bool> IMIOC132;
	sc_in<bool> IMIOC133;

    // TOCxy
    sc_in<bool> TOC00;
    sc_in<bool> TOC01;
    sc_in<bool> TOC02;
    sc_in<bool> TOC03;
    sc_in<bool> TOC10;
    sc_in<bool> TOC11;
    sc_in<bool> TOC12;
    sc_in<bool> TOC13;
    sc_in<bool> TOC20;
    sc_in<bool> TOC21;
    sc_in<bool> TOC22;
    sc_in<bool> TOC23;
    sc_in<bool> TOC30;
    sc_in<bool> TOC31;
	// TimerD
    // CMIADxy
    sc_in<bool> CMIAD00;
    sc_in<bool> CMIAD01;
    sc_in<bool> CMIAD02;
    sc_in<bool> CMIAD03;
    sc_in<bool> CMIAD10;
    sc_in<bool> CMIAD11;
    sc_in<bool> CMIAD12;
    sc_in<bool> CMIAD13;
    sc_in<bool> CMIAD20;
    sc_in<bool> CMIAD21;
    sc_in<bool> CMIAD22;
    sc_in<bool> CMIAD23;
    sc_in<bool> CMIAD30;
    sc_in<bool> CMIAD31;
    sc_in<bool> CMIAD32;
    sc_in<bool> CMIAD33;
    sc_in<bool> CMIAD40;
    sc_in<bool> CMIAD41;
    sc_in<bool> CMIAD42;
    sc_in<bool> CMIAD43;
    sc_in<bool> CMIAD50;
    sc_in<bool> CMIAD51;
    sc_in<bool> CMIAD52;
    sc_in<bool> CMIAD53;
    sc_in<bool> CMIAD60;
    sc_in<bool> CMIAD61;
    sc_in<bool> CMIAD62;
    sc_in<bool> CMIAD63;
    sc_in<bool> CMIAD70;
    sc_in<bool> CMIAD71;
    sc_in<bool> CMIAD72;
    sc_in<bool> CMIAD73;
    sc_in<bool> CMIAD80;
    sc_in<bool> CMIAD81;
    sc_in<bool> CMIAD82;
    sc_in<bool> CMIAD83;
	sc_in<bool> CMIAD90;
	sc_in<bool> CMIAD91;
	sc_in<bool> CMIAD92;
	sc_in<bool> CMIAD93;
	sc_in<bool> CMIAD100;
	sc_in<bool> CMIAD101;
	sc_in<bool> CMIAD102;
	sc_in<bool> CMIAD103;
	sc_in<bool> CMIAD110;
	sc_in<bool> CMIAD111;
	sc_in<bool> CMIAD112;
	sc_in<bool> CMIAD113;
	sc_in<bool> CMIAD120;
	sc_in<bool> CMIAD121;
	sc_in<bool> CMIAD122;
	sc_in<bool> CMIAD123;
	sc_in<bool> CMIAD130;
	sc_in<bool> CMIAD131;
	sc_in<bool> CMIAD132;
	sc_in<bool> CMIAD133;
	sc_in<bool> CMIAD140;
	sc_in<bool> CMIAD141;
	sc_in<bool> CMIAD142;
	sc_in<bool> CMIAD143;
    // CMIBDxy
    sc_in<bool> CMIBD00;
    sc_in<bool> CMIBD01;
    sc_in<bool> CMIBD02;
    sc_in<bool> CMIBD03;
    sc_in<bool> CMIBD10;
    sc_in<bool> CMIBD11;
    sc_in<bool> CMIBD12;
    sc_in<bool> CMIBD13;
    sc_in<bool> CMIBD20;
    sc_in<bool> CMIBD21;
    sc_in<bool> CMIBD22;
    sc_in<bool> CMIBD23;
    sc_in<bool> CMIBD30;
    sc_in<bool> CMIBD31;
    sc_in<bool> CMIBD32;
    sc_in<bool> CMIBD33;
    sc_in<bool> CMIBD40;
    sc_in<bool> CMIBD41;
    sc_in<bool> CMIBD42;
    sc_in<bool> CMIBD43;
    sc_in<bool> CMIBD50;
    sc_in<bool> CMIBD51;
    sc_in<bool> CMIBD52;
    sc_in<bool> CMIBD53;
    sc_in<bool> CMIBD60;
    sc_in<bool> CMIBD61;
    sc_in<bool> CMIBD62;
    sc_in<bool> CMIBD63;
    sc_in<bool> CMIBD70;
    sc_in<bool> CMIBD71;
    sc_in<bool> CMIBD72;
    sc_in<bool> CMIBD73;
    sc_in<bool> CMIBD80;
    sc_in<bool> CMIBD81;
    sc_in<bool> CMIBD82;
    sc_in<bool> CMIBD83;
	sc_in<bool> CMIBD90;
	sc_in<bool> CMIBD91;
	sc_in<bool> CMIBD92;
	sc_in<bool> CMIBD93;
	sc_in<bool> CMIBD100;
	sc_in<bool> CMIBD101;
	sc_in<bool> CMIBD102;
	sc_in<bool> CMIBD103;
	sc_in<bool> CMIBD110;
	sc_in<bool> CMIBD111;
	sc_in<bool> CMIBD112;
	sc_in<bool> CMIBD113;
	sc_in<bool> CMIBD120;
	sc_in<bool> CMIBD121;
	sc_in<bool> CMIBD122;
	sc_in<bool> CMIBD123;
	sc_in<bool> CMIBD130;
	sc_in<bool> CMIBD131;
	sc_in<bool> CMIBD132;
	sc_in<bool> CMIBD133;
	sc_in<bool> CMIBD140;
	sc_in<bool> CMIBD141;
	sc_in<bool> CMIBD142;
	sc_in<bool> CMIBD143;
	
    // OVI1Dx
    sc_in<bool> OVI1D0;
    sc_in<bool> OVI1D1;
    sc_in<bool> OVI1D2;
    sc_in<bool> OVI1D3;
    sc_in<bool> OVI1D4;
    sc_in<bool> OVI1D5;
    sc_in<bool> OVI1D6;
    sc_in<bool> OVI1D7;
    sc_in<bool> OVI1D8;
	sc_in<bool> OVI1D9;
	sc_in<bool> OVI1D10;
	sc_in<bool> OVI1D11;
	sc_in<bool> OVI1D12;
	sc_in<bool> OVI1D13;
	sc_in<bool> OVI1D14;
    // OVI2Dx
    sc_in<bool> OVI2D0;
    sc_in<bool> OVI2D1;
    sc_in<bool> OVI2D2;
    sc_in<bool> OVI2D3;
    sc_in<bool> OVI2D4;
    sc_in<bool> OVI2D5;
    sc_in<bool> OVI2D6;
    sc_in<bool> OVI2D7;
    sc_in<bool> OVI2D8;
	sc_in<bool> OVI2D9;
	sc_in<bool> OVI2D10;
	sc_in<bool> OVI2D11;
	sc_in<bool> OVI2D12;
	sc_in<bool> OVI2D13;
	sc_in<bool> OVI2D14;
    // TODxyA
    sc_in<bool> TOD00A;
    sc_in<bool> TOD01A;
    sc_in<bool> TOD02A;
    sc_in<bool> TOD03A;
    sc_in<bool> TOD10A;
    sc_in<bool> TOD11A;
    sc_in<bool> TOD12A;
    sc_in<bool> TOD13A;
    sc_in<bool> TOD20A;
    sc_in<bool> TOD21A;
    sc_in<bool> TOD22A;
    sc_in<bool> TOD23A;
    sc_in<bool> TOD30A;
    sc_in<bool> TOD31A;
    // TODxyB
    sc_in<bool> TOD00B;
    sc_in<bool> TOD01B;
    sc_in<bool> TOD02B;
    sc_in<bool> TOD03B;
    sc_in<bool> TOD10B;
    sc_in<bool> TOD11B;
    sc_in<bool> TOD12B;
    sc_in<bool> TOD13B;
    sc_in<bool> TOD20B;
    sc_in<bool> TOD21B;
    sc_in<bool> TOD22B;
    sc_in<bool> TOD23B;
    sc_in<bool> TOD30B;
    sc_in<bool> TOD31B;
	//Timer D
    // UDIDxy - underflow interrupt of timer D
    sc_in<bool> UDID00;
    sc_in<bool> UDID01;
    sc_in<bool> UDID02;
    sc_in<bool> UDID03;
    sc_in<bool> UDID10;
    sc_in<bool> UDID11;
    sc_in<bool> UDID12;
    sc_in<bool> UDID13;
    sc_in<bool> UDID20;
    sc_in<bool> UDID21;
    sc_in<bool> UDID22;
    sc_in<bool> UDID23;
    sc_in<bool> UDID30;
    sc_in<bool> UDID31;
    sc_in<bool> UDID32;
    sc_in<bool> UDID33;
    sc_in<bool> UDID40;
    sc_in<bool> UDID41;
    sc_in<bool> UDID42;
    sc_in<bool> UDID43;
    sc_in<bool> UDID50;
    sc_in<bool> UDID51;
    sc_in<bool> UDID52;
    sc_in<bool> UDID53;
    sc_in<bool> UDID60;
    sc_in<bool> UDID61;
    sc_in<bool> UDID62;
    sc_in<bool> UDID63;
    sc_in<bool> UDID70;
    sc_in<bool> UDID71;
    sc_in<bool> UDID72;
    sc_in<bool> UDID73;
    sc_in<bool> UDID80;
    sc_in<bool> UDID81;
    sc_in<bool> UDID82;
    sc_in<bool> UDID83;
	sc_in<bool> UDID90;
	sc_in<bool> UDID91;
	sc_in<bool> UDID92;
	sc_in<bool> UDID93;
	sc_in<bool> UDID100;
	sc_in<bool> UDID101;
	sc_in<bool> UDID102;
	sc_in<bool> UDID103;
	sc_in<bool> UDID110;
	sc_in<bool> UDID111;
	sc_in<bool> UDID112;
	sc_in<bool> UDID113;
	sc_in<bool> UDID120;
	sc_in<bool> UDID121;
	sc_in<bool> UDID122;
	sc_in<bool> UDID123;
	sc_in<bool> UDID130;
	sc_in<bool> UDID131;
	sc_in<bool> UDID132;
	sc_in<bool> UDID133;
	sc_in<bool> UDID140;
	sc_in<bool> UDID141;
	sc_in<bool> UDID142;
	sc_in<bool> UDID143;
    // PREIDxy
    sc_in<bool> PREID00;
    sc_in<bool> PREID01;
    sc_in<bool> PREID02;
    sc_in<bool> PREID03;
    sc_in<bool> PREID10;
    sc_in<bool> PREID11;
    sc_in<bool> PREID12;
    sc_in<bool> PREID13;
    sc_in<bool> PREID20;
    sc_in<bool> PREID21;
    sc_in<bool> PREID22;
    sc_in<bool> PREID23;
    sc_in<bool> PREID30;
    sc_in<bool> PREID31;
    sc_in<bool> PREID32;
    sc_in<bool> PREID33;
    sc_in<bool> PREID40;
    sc_in<bool> PREID41;
    sc_in<bool> PREID42;
    sc_in<bool> PREID43;
    sc_in<bool> PREID50;
    sc_in<bool> PREID51;
    sc_in<bool> PREID52;
    sc_in<bool> PREID53;
    sc_in<bool> PREID60;
    sc_in<bool> PREID61;
    sc_in<bool> PREID62;
    sc_in<bool> PREID63;
    sc_in<bool> PREID70;
    sc_in<bool> PREID71;
    sc_in<bool> PREID72;
    sc_in<bool> PREID73;
    sc_in<bool> PREID80;
    sc_in<bool> PREID81;
    sc_in<bool> PREID82;
    sc_in<bool> PREID83;
	sc_in<bool> PREID90;
	sc_in<bool> PREID91;
	sc_in<bool> PREID92;
	sc_in<bool> PREID93;
	sc_in<bool> PREID100;
	sc_in<bool> PREID101;
	sc_in<bool> PREID102;
	sc_in<bool> PREID103;
	sc_in<bool> PREID110;
	sc_in<bool> PREID111;
	sc_in<bool> PREID112;
	sc_in<bool> PREID113;
	sc_in<bool> PREID120;
	sc_in<bool> PREID121;
	sc_in<bool> PREID122;
	sc_in<bool> PREID123;
	sc_in<bool> PREID130;
	sc_in<bool> PREID131;
	sc_in<bool> PREID132;
	sc_in<bool> PREID133;
	sc_in<bool> PREID140;
	sc_in<bool> PREID141;
	sc_in<bool> PREID142;
	sc_in<bool> PREID143;
    // PFEIDxy
    sc_in<bool> PFEID00;
    sc_in<bool> PFEID01;
    sc_in<bool> PFEID02;
    sc_in<bool> PFEID03;
    sc_in<bool> PFEID10;
    sc_in<bool> PFEID11;
    sc_in<bool> PFEID12;
    sc_in<bool> PFEID13;
    sc_in<bool> PFEID20;
    sc_in<bool> PFEID21;
    sc_in<bool> PFEID22;
    sc_in<bool> PFEID23;
    sc_in<bool> PFEID30;
    sc_in<bool> PFEID31;
    sc_in<bool> PFEID32;
    sc_in<bool> PFEID33;
    sc_in<bool> PFEID40;
    sc_in<bool> PFEID41;
    sc_in<bool> PFEID42;
    sc_in<bool> PFEID43;
    sc_in<bool> PFEID50;
    sc_in<bool> PFEID51;
    sc_in<bool> PFEID52;
    sc_in<bool> PFEID53;
    sc_in<bool> PFEID60;
    sc_in<bool> PFEID61;
    sc_in<bool> PFEID62;
    sc_in<bool> PFEID63;
    sc_in<bool> PFEID70;
    sc_in<bool> PFEID71;
    sc_in<bool> PFEID72;
    sc_in<bool> PFEID73;
    sc_in<bool> PFEID80;
    sc_in<bool> PFEID81;
    sc_in<bool> PFEID82;
    sc_in<bool> PFEID83;
	sc_in<bool> PFEID90;
	sc_in<bool> PFEID91;
	sc_in<bool> PFEID92;
	sc_in<bool> PFEID93;
	sc_in<bool> PFEID100;
	sc_in<bool> PFEID101;
	sc_in<bool> PFEID102;
	sc_in<bool> PFEID103;
	sc_in<bool> PFEID110;
	sc_in<bool> PFEID111;
	sc_in<bool> PFEID112;
	sc_in<bool> PFEID113;
	sc_in<bool> PFEID120;
	sc_in<bool> PFEID121;
	sc_in<bool> PFEID122;
	sc_in<bool> PFEID123;
	sc_in<bool> PFEID130;
	sc_in<bool> PFEID131;
	sc_in<bool> PFEID132;
	sc_in<bool> PFEID133;
	sc_in<bool> PFEID140;
	sc_in<bool> PFEID141;
	sc_in<bool> PFEID142;
	sc_in<bool> PFEID143;
    // TimerE
    sc_in<bool> INTATUECMIE00;
    sc_in<bool> INTATUECMIE01;
    sc_in<bool> INTATUECMIE02;
    sc_in<bool> INTATUECMIE03;
    sc_in<bool> INTATUECMIE10;
    sc_in<bool> INTATUECMIE11;
    sc_in<bool> INTATUECMIE12;
    sc_in<bool> INTATUECMIE13;
    sc_in<bool> INTATUECMIE20;
    sc_in<bool> INTATUECMIE21;
    sc_in<bool> INTATUECMIE22;
    sc_in<bool> INTATUECMIE23;
    sc_in<bool> INTATUECMIE30;
    sc_in<bool> INTATUECMIE31;
    sc_in<bool> INTATUECMIE32;
    sc_in<bool> INTATUECMIE33;
    sc_in<bool> INTATUECMIE40;
    sc_in<bool> INTATUECMIE41;
    sc_in<bool> INTATUECMIE42;
    sc_in<bool> INTATUECMIE43;
    sc_in<bool> INTATUECMIE50;
    sc_in<bool> INTATUECMIE51;
    sc_in<bool> INTATUECMIE52;
    sc_in<bool> INTATUECMIE53;
    sc_in<bool> INTATUECMIE60;
    sc_in<bool> INTATUECMIE61;
    sc_in<bool> INTATUECMIE62;
    sc_in<bool> INTATUECMIE63;
    sc_in<bool> INTATUECMIE70;
    sc_in<bool> INTATUECMIE71;
    sc_in<bool> INTATUECMIE72;
    sc_in<bool> INTATUECMIE73;
    sc_in<bool> INTATUECMIE80;
    sc_in<bool> INTATUECMIE81;
    sc_in<bool> INTATUECMIE82;
    sc_in<bool> INTATUECMIE83;
	sc_in<bool> INTATUECMIE90;
	sc_in<bool> INTATUECMIE91;
	sc_in<bool> INTATUECMIE92;
	sc_in<bool> INTATUECMIE93;
    // TOExy
    sc_in<bool> TOE00;
    sc_in<bool> TOE01;
    sc_in<bool> TOE02;
    sc_in<bool> TOE03;
    sc_in<bool> TOE10;
    sc_in<bool> TOE11;
    sc_in<bool> TOE12;
    sc_in<bool> TOE13;
    sc_in<bool> TOE20;
    sc_in<bool> TOE21;
    sc_in<bool> TOE22;
    sc_in<bool> TOE23;
    sc_in<bool> TOE30;
    sc_in<bool> TOE31;
    // TimerF
    sc_in<bool> INTATUFICIF0;
    sc_in<bool> INTATUFICIF1;
    sc_in<bool> INTATUFICIF2;
    sc_in<bool> INTATUFICIF3;
    sc_in<bool> INTATUFICIF4;
    sc_in<bool> INTATUFICIF5;
    sc_in<bool> INTATUFICIF6;
    sc_in<bool> INTATUFICIF7;
    sc_in<bool> INTATUFICIF8;
    sc_in<bool> INTATUFICIF9;
    sc_in<bool> INTATUFICIF10;
    sc_in<bool> INTATUFICIF11;
    sc_in<bool> INTATUFICIF12;
    sc_in<bool> INTATUFICIF13;
    sc_in<bool> INTATUFICIF14;
    sc_in<bool> INTATUFICIF15;
	sc_in<bool> INTATUFICIF16;
	sc_in<bool> INTATUFICIF17;
	sc_in<bool> INTATUFICIF18;
	sc_in<bool> INTATUFICIF19;
    // OVIFx
    sc_in<bool> OVIF0;
    sc_in<bool> OVIF1;
    sc_in<bool> OVIF2;
    sc_in<bool> OVIF3;
    sc_in<bool> OVIF4;
    sc_in<bool> OVIF5;
    sc_in<bool> OVIF6;
    sc_in<bool> OVIF7;
    sc_in<bool> OVIF8;
    sc_in<bool> OVIF9;
    sc_in<bool> OVIF10;
    sc_in<bool> OVIF11;
    sc_in<bool> OVIF12;
    sc_in<bool> OVIF13;
    sc_in<bool> OVIF14;
    sc_in<bool> OVIF15;
	sc_in<bool> OVIF16;
	sc_in<bool> OVIF17;
	sc_in<bool> OVIF18;
	sc_in<bool> OVIF19;
    // TimerG
    sc_in<bool> INTATUGCMIG0;
    sc_in<bool> INTATUGCMIG1;
    sc_in<bool> INTATUGCMIG2;
    sc_in<bool> INTATUGCMIG3;
    sc_in<bool> INTATUGCMIG4;
    sc_in<bool> INTATUGCMIG5;
    sc_in<bool> INTATUGCMIG6;
    sc_in<bool> INTATUGCMIG7;
    sc_in<bool> INTATUGCMIG8;
    sc_in<bool> INTATUGCMIG9;
    // DMA
    sc_in<bool> INTATUCTRLDMAREQ00;
    sc_in<bool> INTATUCTRLDMAREQ01;
    sc_in<bool> INTATUCTRLDMAREQ10;
    sc_in<bool> INTATUCTRLDMAREQ11;

    // Output ports
    // PSI5
    sc_out<bool> P5TGC0;
    sc_out<bool> P5TGC1;
    sc_out<bool> P5TGC2;
    sc_out<bool> P5TGC3;
    sc_out<bool> P5TGC4;
    sc_out<bool> P5TGC5;
    sc_out<bool> P5TGC6;
    sc_out<bool> P5TGC7;
    sc_out<bool> P5CKTSA;
    sc_out<bool> P5CKTSB;
    sc_out<bool> P5CRTSA;
    sc_out<bool> P5CRTSB;
    sc_out<bool> P5SSTSA;
    sc_out<bool> P5SSTSB;
    // DFE
    sc_out<bool> DEG1TRG;
    sc_out<bool> DFC800;
    sc_out<bool> DFC801;
    sc_out<bool> DFC802;
    sc_out<bool> DFC810;
    sc_out<bool> DFC811;
    sc_out<bool> DFC812;
    sc_out<bool> DFC820;
    sc_out<bool> DFC821;
    sc_out<bool> DFC822;
    sc_out<bool> DFC830;
    sc_out<bool> DFC831;
    sc_out<bool> DFC832;
    sc_out<bool> DFC900;
    sc_out<bool> DFC901;
    sc_out<bool> DFC902;
    sc_out<bool> DFC910;
    sc_out<bool> DFC911;
    sc_out<bool> DFC912;
    sc_out<bool> DFD000;
    sc_out<bool> DFD001;
    sc_out<bool> DFD002;
    sc_out<bool> DFD010;
    sc_out<bool> DFD011;
    sc_out<bool> DFD012;
    sc_out<bool> DFD020;
    sc_out<bool> DFD021;
    sc_out<bool> DFD022;
    sc_out<bool> DFD030;
    sc_out<bool> DFD031;
    sc_out<bool> DFD032;
    sc_out<bool> DFD100;
    sc_out<bool> DFD101;
    sc_out<bool> DFD102;
    sc_out<bool> DFD110;
    sc_out<bool> DFD111;
    sc_out<bool> DFD112;
    // ADC
    sc_out<bool> DSTS0;
    sc_out<bool> DSTS1;
    sc_out<bool> DSTS2;
    sc_out<bool> DSTS3;
    sc_out<bool> DSTS4;
    sc_out<bool> DSTS5;
    sc_out<bool> DSTS6;
    sc_out<bool> DSTS7;
    sc_out<bool> DSTS8;
    sc_out<bool> DSTS9;
    sc_out<bool> CATS;
    // PGL
    // TimerA
    sc_out<bool> ICIA0;
    sc_out<bool> ICIA1;
    sc_out<bool> ICIA2;
    sc_out<bool> ICIA3;
    sc_out<bool> ICIA4;
    sc_out<bool> ICIA5;
    sc_out<bool> ICIA6;
    sc_out<bool> ICIA7;
    // TimerB
    sc_out<bool> CMIB0;
    sc_out<bool> CMIB1;
    sc_out<bool> CMIB6;
    sc_out<bool> CMIB10;
    sc_out<bool> CMIB11;
    sc_out<bool> CMIB12;
    sc_out<bool> CMIB6M;
    sc_out<bool> CMIB66M;
    sc_out<bool> ICIB0;
    // TimerE
    sc_out<bool> CMIE00;
    sc_out<bool> CMIE01;
    sc_out<bool> CMIE02;
    sc_out<bool> CMIE03;
    sc_out<bool> CMIE10;
    sc_out<bool> CMIE11;
    sc_out<bool> CMIE12;
    sc_out<bool> CMIE13;
    sc_out<bool> CMIE20;
    sc_out<bool> CMIE21;
    sc_out<bool> CMIE22;
    sc_out<bool> CMIE23;
    sc_out<bool> CMIE30;
    sc_out<bool> CMIE31;
    sc_out<bool> CMIE32;
    sc_out<bool> CMIE33;
    sc_out<bool> CMIE40;
    sc_out<bool> CMIE41;
    sc_out<bool> CMIE42;
    sc_out<bool> CMIE43;
    sc_out<bool> CMIE50;
    sc_out<bool> CMIE51;
    sc_out<bool> CMIE52;
    sc_out<bool> CMIE53;
    sc_out<bool> CMIE60;
    sc_out<bool> CMIE61;
    sc_out<bool> CMIE62;
    sc_out<bool> CMIE63;
    sc_out<bool> CMIE70;
    sc_out<bool> CMIE71;
    sc_out<bool> CMIE72;
    sc_out<bool> CMIE73;
    sc_out<bool> CMIE80;
    sc_out<bool> CMIE81;
    sc_out<bool> CMIE82;
    sc_out<bool> CMIE83;
	sc_out<bool> CMIE90;
	sc_out<bool> CMIE91;
	sc_out<bool> CMIE92;
	sc_out<bool> CMIE93;
    // TimerF
    sc_out<bool> ICIF0;
    sc_out<bool> ICIF1;
    sc_out<bool> ICIF2;
    sc_out<bool> ICIF3;
    sc_out<bool> ICIF4;
    sc_out<bool> ICIF5;
    sc_out<bool> ICIF6;
    sc_out<bool> ICIF7;
    sc_out<bool> ICIF8;
    sc_out<bool> ICIF9;
    sc_out<bool> ICIF10;
    sc_out<bool> ICIF11;
    sc_out<bool> ICIF12;
    sc_out<bool> ICIF13;
    sc_out<bool> ICIF14;
    sc_out<bool> ICIF15;
	sc_out<bool> ICIF16;
	sc_out<bool> ICIF17;
	sc_out<bool> ICIF18;
	sc_out<bool> ICIF19;
    // TimerG
    sc_out<bool> CMIG0;
    sc_out<bool> CMIG1;
    sc_out<bool> CMIG2;
    sc_out<bool> CMIG3;
    sc_out<bool> CMIG4;
    sc_out<bool> CMIG5;
    sc_out<bool> CMIG6;
    sc_out<bool> CMIG7;
    sc_out<bool> CMIG8;
    sc_out<bool> CMIG9;
    // DMA
    sc_out<bool> DMAREQ00;
    sc_out<bool> DMAREQ01;
    sc_out<bool> DMAREQ10;
    sc_out<bool> DMAREQ11;
    
    // Overflow
    sc_out<bool> OVF;
    // TimerA - select
    sc_out<bool> SLIA0;
    sc_out<bool> SLIA1;
    sc_out<bool> SLIA2;
    sc_out<bool> SLIA3;
    // TimerB - select
    sc_out<bool> SLDB0;
    sc_out<bool> SLDB1;
    sc_out<bool> SLDB2;
    sc_out<bool> SLDB3;
    // TimerC - select
    sc_out<bool> SLIC00;
    sc_out<bool> SLIC01;
    sc_out<bool> SLIC02;
    sc_out<bool> SLIC03;
    sc_out<bool> SLIC10;
    sc_out<bool> SLIC11;
    sc_out<bool> SLIC12;
    sc_out<bool> SLIC13;
    sc_out<bool> SLIC20;
    sc_out<bool> SLIC21;
    sc_out<bool> SLIC22;
    sc_out<bool> SLIC23;
    sc_out<bool> SLIC30;
    sc_out<bool> SLIC31;
    sc_out<bool> SLIC32;
    sc_out<bool> SLIC33;
    sc_out<bool> SLIC40;
    sc_out<bool> SLIC41;
    sc_out<bool> SLIC42;
    sc_out<bool> SLIC43;
    sc_out<bool> SLIC50;
    sc_out<bool> SLIC51;
    sc_out<bool> SLIC52;
    sc_out<bool> SLIC53;
    sc_out<bool> SLIC60;
    sc_out<bool> SLIC61;
    sc_out<bool> SLIC62;
    sc_out<bool> SLIC63;
    sc_out<bool> SLIC70;
    sc_out<bool> SLIC71;
    sc_out<bool> SLIC72;
    sc_out<bool> SLIC73;
    sc_out<bool> SLIC80;
    sc_out<bool> SLIC81;
    sc_out<bool> SLIC82;
    sc_out<bool> SLIC83;
    sc_out<bool> SLIC90;
    sc_out<bool> SLIC91;
    sc_out<bool> SLIC92;
    sc_out<bool> SLIC93;
    sc_out<bool> SLIC100;
    sc_out<bool> SLIC101;
    sc_out<bool> SLIC102;
    sc_out<bool> SLIC103;
	sc_out<bool> SLIC110;
	sc_out<bool> SLIC111;
	sc_out<bool> SLIC112;
	sc_out<bool> SLIC113;
	sc_out<bool> SLIC120;
	sc_out<bool> SLIC121;
	sc_out<bool> SLIC122;
	sc_out<bool> SLIC123;
	sc_out<bool> SLIC130;
	sc_out<bool> SLIC131;
	sc_out<bool> SLIC132;
	sc_out<bool> SLIC133;

    // TimerD - select
    sc_out<bool> SLID00;
    sc_out<bool> SLID01;
    sc_out<bool> SLID02;
    sc_out<bool> SLID03;
    sc_out<bool> SLID10;
    sc_out<bool> SLID11;
    sc_out<bool> SLID12;
    sc_out<bool> SLID13;
    sc_out<bool> SLID20;
    sc_out<bool> SLID21;
    sc_out<bool> SLID22;
    sc_out<bool> SLID23;
    sc_out<bool> SLID30;
    sc_out<bool> SLID31;
    sc_out<bool> SLID32;
    sc_out<bool> SLID33;
    sc_out<bool> SLID40;
    sc_out<bool> SLID41;
    sc_out<bool> SLID42;
    sc_out<bool> SLID43;
    sc_out<bool> SLID50;
    sc_out<bool> SLID51;
    sc_out<bool> SLID52;
    sc_out<bool> SLID53;
    sc_out<bool> SLID60;
    sc_out<bool> SLID61;
    sc_out<bool> SLID62;
    sc_out<bool> SLID63;
    sc_out<bool> SLID70;
    sc_out<bool> SLID71;
    sc_out<bool> SLID72;
    sc_out<bool> SLID73;
    sc_out<bool> SLID80;
    sc_out<bool> SLID81;
    sc_out<bool> SLID82;
    sc_out<bool> SLID83;
	sc_out<bool> SLID90;
	sc_out<bool> SLID91;
	sc_out<bool> SLID92;
	sc_out<bool> SLID93;
	sc_out<bool> SLID100;
	sc_out<bool> SLID101;
	sc_out<bool> SLID102;
	sc_out<bool> SLID103;
	sc_out<bool> SLID110;
	sc_out<bool> SLID111;
	sc_out<bool> SLID112;
	sc_out<bool> SLID113;
	sc_out<bool> SLID120;
	sc_out<bool> SLID121;
	sc_out<bool> SLID122;
	sc_out<bool> SLID123;
	sc_out<bool> SLID130;
	sc_out<bool> SLID131;
	sc_out<bool> SLID132;
	sc_out<bool> SLID133;
	sc_out<bool> SLID140;
	sc_out<bool> SLID141;
	sc_out<bool> SLID142;
	sc_out<bool> SLID143;

    // TimerC-D - select
    sc_out<bool> SLDCD0;
    sc_out<bool> SLDCD1;
    sc_out<bool> SLDCD2;
    sc_out<bool> SLDCD3;
    sc_out<bool> SLDCD4;
    sc_out<bool> SLDCD5;
    sc_out<bool> SLDCD6;
    sc_out<bool> SLDCD7;
    sc_out<bool> SLDCD8;
    sc_out<bool> SLDCD9;
    sc_out<bool> SLDCD10;
    sc_out<bool> SLDCD11;
    sc_out<bool> SLDCD12;
    sc_out<bool> SLDCD13;
    sc_out<bool> SLDCD14;
    sc_out<bool> SLDCD15;
    sc_out<bool> SLDCD16;
    sc_out<bool> SLDCD17;
    sc_out<bool> SLDCD18;
    sc_out<bool> SLDCD19;
    sc_out<bool> SLDCD20;
    sc_out<bool> SLDCD21;
    sc_out<bool> SLDCD22;
    sc_out<bool> SLDCD23;
    sc_out<bool> SLDCD24;
    sc_out<bool> SLDCD25;
    sc_out<bool> SLDCD26;
    sc_out<bool> SLDCD27;
    sc_out<bool> SLDCD28;
    sc_out<bool> SLDCD29;
    sc_out<bool> SLDCD30;
    sc_out<bool> SLDCD31;
    sc_out<bool> SLDCD32;
    sc_out<bool> SLDCD33;
    sc_out<bool> SLDCD34;
    sc_out<bool> SLDCD35;
    sc_out<bool> SLDCD36;
    sc_out<bool> SLDCD37;
    sc_out<bool> SLDCD38;
    sc_out<bool> SLDCD39;
    sc_out<bool> SLDCD40;
    sc_out<bool> SLDCD41;
    sc_out<bool> SLDCD42;
    sc_out<bool> SLDCD43;
    sc_out<bool> SLDCD44;
    sc_out<bool> SLDCD45;
    sc_out<bool> SLDCD46;
    sc_out<bool> SLDCD47;
    sc_out<bool> SLDCD48;
    sc_out<bool> SLDCD49;
    sc_out<bool> SLDCD50;
    sc_out<bool> SLDCD51;
    sc_out<bool> SLDCD52;
    sc_out<bool> SLDCD53;
    sc_out<bool> SLDCD54;
    sc_out<bool> SLDCD55;
    sc_out<bool> SLDCD56;
    sc_out<bool> SLDCD57;
    sc_out<bool> SLDCD58;
    sc_out<bool> SLDCD59;
    sc_out<bool> SLDCD60;
    sc_out<bool> SLDCD61;
    sc_out<bool> SLDCD62;
    sc_out<bool> SLDCD63;
    // TimerE - select
    sc_out<bool> SLDE0;
    sc_out<bool> SLDE1;
    sc_out<bool> SLDE2;
    sc_out<bool> SLDE3;
    sc_out<bool> SLDE4;
    sc_out<bool> SLDE5;
    sc_out<bool> SLDE6;
    sc_out<bool> SLDE7;
    sc_out<bool> SLDE8;
	sc_out<bool> SLDE9;

    /// Socket declaration
    TlmTargetSocket<32> *ts;            // For ATU5011_GLUE's registers

    SC_HAS_PROCESS (ATU5011_GLUE);
    ATU5011_GLUE (sc_module_name name,
          unsigned int rLatency,
          unsigned int wLatency);
    ~ATU5011_GLUE (void);

    /// PythonIF
    void AssertReset (const std::string reset_name, const double start_time, const double period);
    void SetCLKFreq (const std::string clock_name, const sc_dt::uint64  freq, const std::string unit);
    void GetCLKFreq (const std::string clock_name);
    void ForceRegister (const std::string reg_name, const unsigned int reg_value);
    void ReleaseRegister (const std::string reg_name);
    void WriteRegister (const std::string reg_name, const unsigned int reg_value);
    void ReadRegister (const std::string reg_name);
    void ListRegister (void);
    void Help (const std::string type);
    void SetMessageLevel (const std::string msg_lv);
    void DumpRegisterRW (const std::string is_enable);

private:
    /// Declare methods
    void HandlePCLKSignalMethod (void);
    void HandlePRESETZSignalMethod (void);
    void HandleResetHardMethod (void);
    void HandleResetCmdMethod (void);
    void CancelResetCmdMethod (void);
 
    void WriteSLIAxMethod (void);
    void WriteSLDBxMethod (void);
    void WriteSLICxMethod (unsigned int id);
    void WriteSLIDxMethod (unsigned int id);
    void WriteSLDCDxMethod (unsigned int id);
    void WriteSLDExMethod (void);
    void WriteOVFMethod (void);
    
    void WriteDFxMethod (unsigned int id);
    void WriteDEG1TRGMethod (void);
    void WriteDSTSxMethod (unsigned int id);
    void WriteCATSMethod (void);
    void WriteP5xMethod (unsigned int id);
    void WriteICIAxMethod (void);
    void WriteCIBxMethod (void);
    void WriteCMIExMethod (void);
    void WriteICIFxMethod (void);
    void WriteCMIGxMethod (void);
    void WriteDMAREQxMethod (void);
    
    /// Internal functions
    void EnableReset (const std::string reset_name, const bool is_active);
    void ConvertClockFreq (sc_dt::uint64 &freq_out, std::string &unit_out,
                           sc_dt::uint64 freq_in, std::string unit_in);
    void SeparateString (std::vector<std::string> &vtr, const std::string msg);
    void GetTimeResolution (sc_dt::uint64 &resolution_value, sc_time_unit &resolution_unit);
    
    /// Virtual functions of ATU5011_GLUE_AgentController class
    bool GetResetStatus (const std::string reset_name);
    bool CheckClockPeriod (const std::string clock_name);
    void NotifyEvent (const unsigned int event, const unsigned int channel);
};
#endif
