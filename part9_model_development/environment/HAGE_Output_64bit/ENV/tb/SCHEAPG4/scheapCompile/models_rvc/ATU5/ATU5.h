// ---------------------------------------------------------------------
// $Id: ATU5.h 1196 2019-10-17 05:33:04Z chuonghoangle $
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
#ifndef __ATU5_H__
#define __ATU5_H__

#include "BusSlaveBase.h"
#include "ATU5_AgentController.h"
#include <map>


#  ifdef IEEE_1666_SYSTEMC
#   ifndef SC_SIGNAL
#       define SC_SIGNAL(T) (sc_signal< T , SC_UNCHECKED_WRITERS>)
#   endif
#  else
#   ifndef SC_SIGNAL
#       define SC_SIGNAL(T) (sc_signal< T >)
#   endif
#  endif

class ATU5_Func;

class ATU5: public sc_module,
            public ATU5_AgentController,
            public BusSlaveBase<32,1>
{
friend class ATU5_Timer;
friend class ATU5_NoiseCanceler;
friend class ATU5_TIMERA;
friend class ATU5_TIMERA_Func;
friend class ATU5_TIMERB;
friend class ATU5_TIMERC;
friend class ATU5_TIMERC_SUB;
friend class ATU5_TIMERD;
friend class ATU5_TIMERD_SUB;
friend class ATU5_TIMERE;
friend class ATU5_TIMERE_SUB;
friend class ATU5_TIMERF;
friend class ATU5_TIMERF_Func;
friend class ATU5_TIMERF_Sub;
friend class ATU5_TIMERF_Sub_Func;
friend class ATU5_TIMERG;

private:

    ATU5_Func *mATU5_Func;            // ATU5_Func class instance

    sc_signal<sc_dt::uint64> *clk_bus_sig[emCLK_BUSnum];     //Internal
    sc_signal<sc_dt::uint64> nc_clk_sig;     //Internal
    SC_SIGNAL(bool) *timerA_TILOA_sig[emTimerA_TILOnum];

    SC_SIGNAL(bool) timerB_TCNTB6CM_sig;
    SC_SIGNAL(bool) *timerF_POE_sig[emTimerEF_POEnum];
    
    sc_signal<sc_dt::uint64> CLK_LSBperiod_sig;
    
    /// Declare variables
    sc_dt::uint64 mCLK_HSBPeriod;          //Period value of clock
    sc_dt::uint64 mCLK_HSBFreq;            //Frequency value of clock
    sc_dt::uint64 mCLK_HSBOrgFreq;         //Original frequency value of clock
    std::string mCLK_HSBFreqUnit;          //Frequency unit of clock
    sc_dt::uint64 mCLK_LSBFreq;            //Frequency value of clock
    sc_dt::uint64 mCLK_LSBOrgFreq;         //Original frequency value of clock
    std::string mCLK_LSBFreqUnit;          //Frequency unit of clock
    
    sc_dt::uint64 mTimeResolutionValue;    //Time resolution value of simulation

    bool mResetCurVal[emResetSignalNum];        //Store current value of reset signals
    bool mResetPreVal[emResetSignalNum];        //Store previous value of reset signals

    bool mIsResetCmdActive[emResetSignalNum];   //Active status of AssertReset command of reset signals
    double mResetCmdPeriod[emResetSignalNum];   //Period of AssertReset command of reset signals
    bool mINTATUCTRLDMAREQ[emWrapper_DMAnum];   //Store value to write to INTATUCTRLDMAREQxy
    bool m_at_saradtrg;                         //Store value to at_saradtrg

    
    std::map<std::string, unsigned int> mBlockIdMap;
    std::vector<std::string> mModuleIdVector;
    
    bool mPrePSCE;
    double mPSCSyncTime[emClkBusPSCNum]; // removed emNoiseCancelPCSNum by chuongle Jul/28/2018

    sc_dt::uint64 mClkBusPeriod[emCLK_BUSnum];
    sc_dt::uint64 mTCLKPeriod[emTCLKnum];
    sc_dt::uint64 mNoiseCancelClkPeriod;
    
    bool mIsClkBusZero[emCLK_BUSnum];
    bool mIsTimerASel;
    bool mIsTimerBSel;
    
    bool mIsDMASelectHigh;
    bool mAGCK1boolValue;
    double mAGCK1StartTime;

    /// Declare events
    sc_event mResetHardEvent[emResetSignalNum];         //Call HandleResetHardMethod when reset is active
    sc_event mResetCmdEvent[emResetSignalNum];          //Call HandleResetCmdMethod when AssertReset is called
    sc_event mResetCmdCancelEvent[emResetSignalNum];    //Call CancelResetCmdMethod when AssertReset is over


    sc_event mStartPrescalerEvent[emClkBusPSCNum];      //removed emNoiseCancelPCSNum by chuongle Jul/28/2018
    sc_event mStopPrescalerEvent[emClkBusPSCNum];       //removed emNoiseCancelPCSNum by chuongle Jul/28/2018
    sc_event mWriteClockBusEvent[emCLK_BUSnum];
    sc_event mWriteNoiseCancelClkEvent;
    
    sc_event mTCLKUpdateEvent[emTCLKnum];
    sc_event mEnableTimerABEvent;
    
    sc_event mUpdateDMATrgSelectEvent;
    sc_event mWriteat_saradtrgEvent;
    sc_event mWriteTrgDMAReqEvent[emWrapper_DMAnum];
    sc_event mHandleTRGSELDMAregChangeEvent[emWrapper_DMAnum];
    sc_event mAGCK1AssertEvent;
    sc_event mAGCK1DeassertEvent;
    sc_event mAGCK1ChangeEvent;
    sc_event mHandleTRGSELADregChangeEvent;
    
public:
    /// Declare signals
    // Input ports
    sc_in<sc_dt::uint64> CLK_HSB;
    sc_in<sc_dt::uint64> CLK_LSB;
    sc_in<bool> presetz;
    sc_in<bool> presetz_bif;
    sc_in<sc_dt::uint64> TCLKA;         //External
    sc_in<sc_dt::uint64> TCLKB;         //External
    // TimerA
    sc_in<bool> TIA0;         //External
    sc_in<bool> TIA1;         //External
    sc_in<bool> TIA2;         //External
    sc_in<bool> TIA3;         //External
    sc_in<bool> TIA4;         //External
    sc_in<bool> TIA5;         //External
    sc_in<bool> TIA6;         //External
    sc_in<bool> TIA7;         //External
    // TimerC
    sc_inout<bool> TIOC00;
    sc_inout<bool> TIOC01;
    sc_inout<bool> TIOC02;
    sc_inout<bool> TIOC03;
    sc_inout<bool> TIOC10;
    sc_inout<bool> TIOC11;
    sc_inout<bool> TIOC12;
    sc_inout<bool> TIOC13;
    sc_inout<bool> TIOC20;
    sc_inout<bool> TIOC21;
    sc_inout<bool> TIOC22;
    sc_inout<bool> TIOC23;
    sc_inout<bool> TIOC30;
    sc_inout<bool> TIOC31;
    sc_inout<bool> TIOC32;
    sc_inout<bool> TIOC33;
    sc_inout<bool> TIOC40;
    sc_inout<bool> TIOC41;
    sc_inout<bool> TIOC42;
    sc_inout<bool> TIOC43;
    sc_inout<bool> TIOC50;
    sc_inout<bool> TIOC51;
    sc_inout<bool> TIOC52;
    sc_inout<bool> TIOC53;
    sc_inout<bool> TIOC60;
    sc_inout<bool> TIOC61;
    sc_inout<bool> TIOC62;
    sc_inout<bool> TIOC63;
    sc_inout<bool> TIOC70;
    sc_inout<bool> TIOC71;
    sc_inout<bool> TIOC72;
    sc_inout<bool> TIOC73;
    sc_inout<bool> TIOC80;
    sc_inout<bool> TIOC81;
    sc_inout<bool> TIOC82;
    sc_inout<bool> TIOC83;
    sc_inout<bool> TIOC90;
    sc_inout<bool> TIOC91;
    sc_inout<bool> TIOC92;
    sc_inout<bool> TIOC93;
    sc_inout<bool> TIOC100;
    sc_inout<bool> TIOC101;
    sc_inout<bool> TIOC102;
    sc_inout<bool> TIOC103;
    // TimerD
    sc_in<bool> TID00;
    sc_in<bool> TID01;
    sc_in<bool> TID02;
    sc_in<bool> TID03;
    sc_in<bool> TID04;
    sc_in<bool> TID05;
    sc_in<bool> TID06;
    sc_in<bool> TID07;
    sc_in<bool> TID10;
    sc_in<bool> TID11;
    sc_in<bool> TID12;
    sc_in<bool> TID13;
    sc_in<bool> TID14;
    sc_in<bool> TID15;
    sc_in<bool> TID16;
    sc_in<bool> TID17;
    sc_in<bool> TID20;
    sc_in<bool> TID21;
    sc_in<bool> TID22;
    sc_in<bool> TID23;
    sc_in<bool> TID24;
    sc_in<bool> TID25;
    sc_in<bool> TID26;
    sc_in<bool> TID27;
    sc_in<bool> TID30;
    sc_in<bool> TID31;
    sc_in<bool> TID32;
    sc_in<bool> TID33;
    sc_in<bool> TID34;
    sc_in<bool> TID35;
    sc_in<bool> TID36;
    sc_in<bool> TID37;
    sc_in<bool> TID40;
    sc_in<bool> TID41;
    sc_in<bool> TID42;
    sc_in<bool> TID43;
    sc_in<bool> TID44;
    sc_in<bool> TID45;
    sc_in<bool> TID46;
    sc_in<bool> TID47;
    sc_in<bool> TID50;
    sc_in<bool> TID51;
    sc_in<bool> TID52;
    sc_in<bool> TID53;
    sc_in<bool> TID54;
    sc_in<bool> TID55;
    sc_in<bool> TID56;
    sc_in<bool> TID57;
    sc_in<bool> TID60;
    sc_in<bool> TID61;
    sc_in<bool> TID62;
    sc_in<bool> TID63;
    sc_in<bool> TID64;
    sc_in<bool> TID65;
    sc_in<bool> TID66;
    sc_in<bool> TID67;
    sc_in<bool> TID70;
    sc_in<bool> TID71;
    sc_in<bool> TID72;
    sc_in<bool> TID73;
    sc_in<bool> TID74;
    sc_in<bool> TID75;
    sc_in<bool> TID76;
    sc_in<bool> TID77;
    sc_in<bool> TID80;
    sc_in<bool> TID81;
    sc_in<bool> TID82;
    sc_in<bool> TID83;
    sc_in<bool> TID84;
    sc_in<bool> TID85;
    sc_in<bool> TID86;
    sc_in<bool> TID87;
    // TimerF
    sc_in<bool> TIF0A;
    sc_in<bool> TIF1A;
    sc_in<bool> TIF2A;
    sc_in<bool> TIF3;
    sc_in<bool> TIF4;
    sc_in<bool> TIF5;
    sc_in<bool> TIF6;
    sc_in<bool> TIF7;
    sc_in<bool> TIF8;
    sc_in<bool> TIF9;
    sc_in<bool> TIF10;
    sc_in<bool> TIF11;
    sc_in<bool> TIF12;
    sc_in<bool> TIF13;
    sc_in<bool> TIF14;
    sc_in<bool> TIF15;
    sc_in<bool> TIF0B;
    sc_in<bool> TIF1B;
    sc_in<bool> TIF2B;
    // DMA
    sc_in<bool> pf_dmai_s;
    sc_in<bool> pf_dmai0_a;
    sc_in<bool> pf_dmai1_a;
    sc_in<bool> pf_dmai2_a;
    sc_in<bool> pf_dmai3_a;
    sc_in<bool> pf_dmai4_a;

    // Output ports
    // TimerC
    // TOCExy
    sc_out<bool> TOCE00;
    sc_out<bool> TOCE01;
    sc_out<bool> TOCE02;
    sc_out<bool> TOCE03;
    sc_out<bool> TOCE10;
    sc_out<bool> TOCE11;
    sc_out<bool> TOCE12;
    sc_out<bool> TOCE13;
    sc_out<bool> TOCE20;
    sc_out<bool> TOCE21;
    sc_out<bool> TOCE22;
    sc_out<bool> TOCE23;
    sc_out<bool> TOCE30;
    sc_out<bool> TOCE31;
    sc_out<bool> TOCE32;
    sc_out<bool> TOCE33;
    sc_out<bool> TOCE40;
    sc_out<bool> TOCE41;
    sc_out<bool> TOCE42;
    sc_out<bool> TOCE43;
    sc_out<bool> TOCE50;
    sc_out<bool> TOCE51;
    sc_out<bool> TOCE52;
    sc_out<bool> TOCE53;
    sc_out<bool> TOCE60;
    sc_out<bool> TOCE61;
    sc_out<bool> TOCE62;
    sc_out<bool> TOCE63;
    sc_out<bool> TOCE70;
    sc_out<bool> TOCE71;
    sc_out<bool> TOCE72;
    sc_out<bool> TOCE73;
    sc_out<bool> TOCE80;
    sc_out<bool> TOCE81;
    sc_out<bool> TOCE82;
    sc_out<bool> TOCE83;
    sc_out<bool> TOCE90;
    sc_out<bool> TOCE91;
    sc_out<bool> TOCE92;
    sc_out<bool> TOCE93;
    sc_out<bool> TOCE100;
    sc_out<bool> TOCE101;
    sc_out<bool> TOCE102;
    sc_out<bool> TOCE103;
    // TOCxy
    sc_out<bool> TOC00;
    sc_out<bool> TOC01;
    sc_out<bool> TOC02;
    sc_out<bool> TOC03;
    sc_out<bool> TOC10;
    sc_out<bool> TOC11;
    sc_out<bool> TOC12;
    sc_out<bool> TOC13;
    sc_out<bool> TOC20;
    sc_out<bool> TOC21;
    sc_out<bool> TOC22;
    sc_out<bool> TOC23;
    sc_out<bool> TOC30;
    sc_out<bool> TOC31;
    // TimerD
    // TODxyA
    sc_out<bool> TOD00A;
    sc_out<bool> TOD01A;
    sc_out<bool> TOD02A;
    sc_out<bool> TOD03A;
    sc_out<bool> TOD10A;
    sc_out<bool> TOD11A;
    sc_out<bool> TOD12A;
    sc_out<bool> TOD13A;
    sc_out<bool> TOD20A;
    sc_out<bool> TOD21A;
    sc_out<bool> TOD22A;
    sc_out<bool> TOD23A;
    sc_out<bool> TOD30A;
    sc_out<bool> TOD31A;
    sc_out<bool> TOD32A;
    sc_out<bool> TOD33A;
    sc_out<bool> TOD40A;
    sc_out<bool> TOD41A;
    sc_out<bool> TOD42A;
    sc_out<bool> TOD43A;
    sc_out<bool> TOD50A;
    sc_out<bool> TOD51A;
    sc_out<bool> TOD52A;
    sc_out<bool> TOD53A;
    sc_out<bool> TOD60A;
    sc_out<bool> TOD61A;
    sc_out<bool> TOD62A;
    sc_out<bool> TOD63A;
    sc_out<bool> TOD70A;
    sc_out<bool> TOD71A;
    sc_out<bool> TOD72A;
    sc_out<bool> TOD73A;
    sc_out<bool> TOD80A;
    sc_out<bool> TOD81A;
    sc_out<bool> TOD82A;
    sc_out<bool> TOD83A;
    // TODxyB
    sc_out<bool> TOD00B;
    sc_out<bool> TOD01B;
    sc_out<bool> TOD02B;
    sc_out<bool> TOD03B;
    sc_out<bool> TOD10B;
    sc_out<bool> TOD11B;
    sc_out<bool> TOD12B;
    sc_out<bool> TOD13B;
    sc_out<bool> TOD20B;
    sc_out<bool> TOD21B;
    sc_out<bool> TOD22B;
    sc_out<bool> TOD23B;
    sc_out<bool> TOD30B;
    sc_out<bool> TOD31B;
    sc_out<bool> TOD32B;
    sc_out<bool> TOD33B;
    sc_out<bool> TOD40B;
    sc_out<bool> TOD41B;
    sc_out<bool> TOD42B;
    sc_out<bool> TOD43B;
    sc_out<bool> TOD50B;
    sc_out<bool> TOD51B;
    sc_out<bool> TOD52B;
    sc_out<bool> TOD53B;
    sc_out<bool> TOD60B;
    sc_out<bool> TOD61B;
    sc_out<bool> TOD62B;
    sc_out<bool> TOD63B;
    sc_out<bool> TOD70B;
    sc_out<bool> TOD71B;
    sc_out<bool> TOD72B;
    sc_out<bool> TOD73B;
    sc_out<bool> TOD80B;
    sc_out<bool> TOD81B;
    sc_out<bool> TOD82B;
    sc_out<bool> TOD83B;
    // TODMIGxy
    sc_out<bool> TODMIG00;
    sc_out<bool> TODMIG01;
    sc_out<bool> TODMIG02;
    sc_out<bool> TODMIG03;
    sc_out<bool> TODMIG10;
    sc_out<bool> TODMIG11;
    sc_out<bool> TODMIG12;
    sc_out<bool> TODMIG13;
    sc_out<bool> TODMIG20;
    sc_out<bool> TODMIG21;
    sc_out<bool> TODMIG22;
    sc_out<bool> TODMIG23;
    sc_out<bool> TODMIG30;
    sc_out<bool> TODMIG31;
    sc_out<bool> TODMIG32;
    sc_out<bool> TODMIG33;
    sc_out<bool> TODMIG40;
    sc_out<bool> TODMIG41;
    sc_out<bool> TODMIG42;
    sc_out<bool> TODMIG43;
    sc_out<bool> TODMIG50;
    sc_out<bool> TODMIG51;
    sc_out<bool> TODMIG52;
    sc_out<bool> TODMIG53;
    sc_out<bool> TODMIG60;
    sc_out<bool> TODMIG61;
    sc_out<bool> TODMIG62;
    sc_out<bool> TODMIG63;
    sc_out<bool> TODMIG70;
    sc_out<bool> TODMIG71;
    sc_out<bool> TODMIG72;
    sc_out<bool> TODMIG73;
    sc_out<bool> TODMIG80;
    sc_out<bool> TODMIG81;
    sc_out<bool> TODMIG82;
    sc_out<bool> TODMIG83;
    // TimerE
    sc_out<bool> TOE00;
    sc_out<bool> TOE01;
    sc_out<bool> TOE02;
    sc_out<bool> TOE03;
    sc_out<bool> TOE10;
    sc_out<bool> TOE11;
    sc_out<bool> TOE12;
    sc_out<bool> TOE13;
    sc_out<bool> TOE20;
    sc_out<bool> TOE21;
    sc_out<bool> TOE22;
    sc_out<bool> TOE23;
    sc_out<bool> TOE30;
    sc_out<bool> TOE31;
    sc_out<bool> TOE32;
    sc_out<bool> TOE33;
    sc_out<bool> TOE40;
    sc_out<bool> TOE41;
    sc_out<bool> TOE42;
    sc_out<bool> TOE43;
    sc_out<bool> TOE50;
    sc_out<bool> TOE51;
    sc_out<bool> TOE52;
    sc_out<bool> TOE53;
    sc_out<bool> TOE60;
    sc_out<bool> TOE61;
    sc_out<bool> TOE62;
    sc_out<bool> TOE63;
    sc_out<bool> TOE70;
    sc_out<bool> TOE71;
    sc_out<bool> TOE72;
    sc_out<bool> TOE73;
    sc_out<bool> TOE80;
    sc_out<bool> TOE81;
    sc_out<bool> TOE82;
    sc_out<bool> TOE83;
    
    // Interrupts
    // TimerA
    sc_out<bool> INTATUAICIA0;
    sc_out<bool> INTATUAICIA1;
    sc_out<bool> INTATUAICIA2;
    sc_out<bool> INTATUAICIA3;
    sc_out<bool> INTATUAICIA4;
    sc_out<bool> INTATUAICIA5;
    sc_out<bool> INTATUAICIA6;
    sc_out<bool> INTATUAICIA7;
    // OVIA
    sc_out<bool> OVIA;
    // TimerB
    sc_out<bool> INTATUBCMIB0;
    sc_out<bool> INTATUBCMIB1;
    sc_out<bool> INTATUBCMIB6;
    sc_out<bool> INTATUBCMIB10;
    sc_out<bool> INTATUBCMIB11;
    sc_out<bool> INTATUBCMIB12;
    sc_out<bool> INTATUBCMIB6M;
    sc_out<bool> INTATUBCMIB66M;
    sc_out<bool> INTATUBICIB0;
    // TimerC
    // OVICx
    sc_out<bool> OVIC0;
    sc_out<bool> OVIC1;
    sc_out<bool> OVIC2;
    sc_out<bool> OVIC3;
    sc_out<bool> OVIC4;
    sc_out<bool> OVIC5;
    sc_out<bool> OVIC6;
    sc_out<bool> OVIC7;
    sc_out<bool> OVIC8;
    sc_out<bool> OVIC9;
    sc_out<bool> OVIC10;
    // IMIGCxy
    sc_out<bool> IMIGC00;
    sc_out<bool> IMIGC01;
    sc_out<bool> IMIGC02;
    sc_out<bool> IMIGC03;
    sc_out<bool> IMIGC10;
    sc_out<bool> IMIGC11;
    sc_out<bool> IMIGC12;
    sc_out<bool> IMIGC13;
    sc_out<bool> IMIGC20;
    sc_out<bool> IMIGC21;
    sc_out<bool> IMIGC22;
    sc_out<bool> IMIGC23;
    sc_out<bool> IMIGC30;
    sc_out<bool> IMIGC31;
    sc_out<bool> IMIGC32;
    sc_out<bool> IMIGC33;
    sc_out<bool> IMIGC40;
    sc_out<bool> IMIGC41;
    sc_out<bool> IMIGC42;
    sc_out<bool> IMIGC43;
    sc_out<bool> IMIGC50;
    sc_out<bool> IMIGC51;
    sc_out<bool> IMIGC52;
    sc_out<bool> IMIGC53;
    sc_out<bool> IMIGC60;
    sc_out<bool> IMIGC61;
    sc_out<bool> IMIGC62;
    sc_out<bool> IMIGC63;
    sc_out<bool> IMIGC70;
    sc_out<bool> IMIGC71;
    sc_out<bool> IMIGC72;
    sc_out<bool> IMIGC73;
    sc_out<bool> IMIGC80;
    sc_out<bool> IMIGC81;
    sc_out<bool> IMIGC82;
    sc_out<bool> IMIGC83;
    sc_out<bool> IMIGC90;
    sc_out<bool> IMIGC91;
    sc_out<bool> IMIGC92;
    sc_out<bool> IMIGC93;
    sc_out<bool> IMIGC100;
    sc_out<bool> IMIGC101;
    sc_out<bool> IMIGC102;
    sc_out<bool> IMIGC103;
    // IMIOCxy
    sc_out<bool> IMIOC00;
    sc_out<bool> IMIOC01;
    sc_out<bool> IMIOC02;
    sc_out<bool> IMIOC03;
    sc_out<bool> IMIOC10;
    sc_out<bool> IMIOC11;
    sc_out<bool> IMIOC12;
    sc_out<bool> IMIOC13;
    sc_out<bool> IMIOC20;
    sc_out<bool> IMIOC21;
    sc_out<bool> IMIOC22;
    sc_out<bool> IMIOC23;
    sc_out<bool> IMIOC30;
    sc_out<bool> IMIOC31;
    sc_out<bool> IMIOC32;
    sc_out<bool> IMIOC33;
    sc_out<bool> IMIOC40;
    sc_out<bool> IMIOC41;
    sc_out<bool> IMIOC42;
    sc_out<bool> IMIOC43;
    sc_out<bool> IMIOC50;
    sc_out<bool> IMIOC51;
    sc_out<bool> IMIOC52;
    sc_out<bool> IMIOC53;
    sc_out<bool> IMIOC60;
    sc_out<bool> IMIOC61;
    sc_out<bool> IMIOC62;
    sc_out<bool> IMIOC63;
    sc_out<bool> IMIOC70;
    sc_out<bool> IMIOC71;
    sc_out<bool> IMIOC72;
    sc_out<bool> IMIOC73;
    sc_out<bool> IMIOC80;
    sc_out<bool> IMIOC81;
    sc_out<bool> IMIOC82;
    sc_out<bool> IMIOC83;
    sc_out<bool> IMIOC90;
    sc_out<bool> IMIOC91;
    sc_out<bool> IMIOC92;
    sc_out<bool> IMIOC93;
    sc_out<bool> IMIOC100;
    sc_out<bool> IMIOC101;
    sc_out<bool> IMIOC102;
    sc_out<bool> IMIOC103;
    // TimerD
    // CMIADxy
    sc_out<bool> CMIAD00;
    sc_out<bool> CMIAD01;
    sc_out<bool> CMIAD02;
    sc_out<bool> CMIAD03;
    sc_out<bool> CMIAD10;
    sc_out<bool> CMIAD11;
    sc_out<bool> CMIAD12;
    sc_out<bool> CMIAD13;
    sc_out<bool> CMIAD20;
    sc_out<bool> CMIAD21;
    sc_out<bool> CMIAD22;
    sc_out<bool> CMIAD23;
    sc_out<bool> CMIAD30;
    sc_out<bool> CMIAD31;
    sc_out<bool> CMIAD32;
    sc_out<bool> CMIAD33;
    sc_out<bool> CMIAD40;
    sc_out<bool> CMIAD41;
    sc_out<bool> CMIAD42;
    sc_out<bool> CMIAD43;
    sc_out<bool> CMIAD50;
    sc_out<bool> CMIAD51;
    sc_out<bool> CMIAD52;
    sc_out<bool> CMIAD53;
    sc_out<bool> CMIAD60;
    sc_out<bool> CMIAD61;
    sc_out<bool> CMIAD62;
    sc_out<bool> CMIAD63;
    sc_out<bool> CMIAD70;
    sc_out<bool> CMIAD71;
    sc_out<bool> CMIAD72;
    sc_out<bool> CMIAD73;
    sc_out<bool> CMIAD80;
    sc_out<bool> CMIAD81;
    sc_out<bool> CMIAD82;
    sc_out<bool> CMIAD83;
    // CMIBDxy
    sc_out<bool> CMIBD00;
    sc_out<bool> CMIBD01;
    sc_out<bool> CMIBD02;
    sc_out<bool> CMIBD03;
    sc_out<bool> CMIBD10;
    sc_out<bool> CMIBD11;
    sc_out<bool> CMIBD12;
    sc_out<bool> CMIBD13;
    sc_out<bool> CMIBD20;
    sc_out<bool> CMIBD21;
    sc_out<bool> CMIBD22;
    sc_out<bool> CMIBD23;
    sc_out<bool> CMIBD30;
    sc_out<bool> CMIBD31;
    sc_out<bool> CMIBD32;
    sc_out<bool> CMIBD33;
    sc_out<bool> CMIBD40;
    sc_out<bool> CMIBD41;
    sc_out<bool> CMIBD42;
    sc_out<bool> CMIBD43;
    sc_out<bool> CMIBD50;
    sc_out<bool> CMIBD51;
    sc_out<bool> CMIBD52;
    sc_out<bool> CMIBD53;
    sc_out<bool> CMIBD60;
    sc_out<bool> CMIBD61;
    sc_out<bool> CMIBD62;
    sc_out<bool> CMIBD63;
    sc_out<bool> CMIBD70;
    sc_out<bool> CMIBD71;
    sc_out<bool> CMIBD72;
    sc_out<bool> CMIBD73;
    sc_out<bool> CMIBD80;
    sc_out<bool> CMIBD81;
    sc_out<bool> CMIBD82;
    sc_out<bool> CMIBD83;
    // OVI1Dx
    sc_out<bool> OVI1D0;
    sc_out<bool> OVI1D1;
    sc_out<bool> OVI1D2;
    sc_out<bool> OVI1D3;
    sc_out<bool> OVI1D4;
    sc_out<bool> OVI1D5;
    sc_out<bool> OVI1D6;
    sc_out<bool> OVI1D7;
    sc_out<bool> OVI1D8;
    // OVI2Dx
    sc_out<bool> OVI2D0;
    sc_out<bool> OVI2D1;
    sc_out<bool> OVI2D2;
    sc_out<bool> OVI2D3;
    sc_out<bool> OVI2D4;
    sc_out<bool> OVI2D5;
    sc_out<bool> OVI2D6;
    sc_out<bool> OVI2D7;
    sc_out<bool> OVI2D8;
    // UDIDxy
    sc_out<bool> UDID00;
    sc_out<bool> UDID01;
    sc_out<bool> UDID02;
    sc_out<bool> UDID03;
    sc_out<bool> UDID10;
    sc_out<bool> UDID11;
    sc_out<bool> UDID12;
    sc_out<bool> UDID13;
    sc_out<bool> UDID20;
    sc_out<bool> UDID21;
    sc_out<bool> UDID22;
    sc_out<bool> UDID23;
    sc_out<bool> UDID30;
    sc_out<bool> UDID31;
    sc_out<bool> UDID32;
    sc_out<bool> UDID33;
    sc_out<bool> UDID40;
    sc_out<bool> UDID41;
    sc_out<bool> UDID42;
    sc_out<bool> UDID43;
    sc_out<bool> UDID50;
    sc_out<bool> UDID51;
    sc_out<bool> UDID52;
    sc_out<bool> UDID53;
    sc_out<bool> UDID60;
    sc_out<bool> UDID61;
    sc_out<bool> UDID62;
    sc_out<bool> UDID63;
    sc_out<bool> UDID70;
    sc_out<bool> UDID71;
    sc_out<bool> UDID72;
    sc_out<bool> UDID73;
    sc_out<bool> UDID80;
    sc_out<bool> UDID81;
    sc_out<bool> UDID82;
    sc_out<bool> UDID83;
    // PREIDxy
    sc_out<bool> PREID00;
    sc_out<bool> PREID01;
    sc_out<bool> PREID02;
    sc_out<bool> PREID03;
    sc_out<bool> PREID10;
    sc_out<bool> PREID11;
    sc_out<bool> PREID12;
    sc_out<bool> PREID13;
    sc_out<bool> PREID20;
    sc_out<bool> PREID21;
    sc_out<bool> PREID22;
    sc_out<bool> PREID23;
    sc_out<bool> PREID30;
    sc_out<bool> PREID31;
    sc_out<bool> PREID32;
    sc_out<bool> PREID33;
    sc_out<bool> PREID40;
    sc_out<bool> PREID41;
    sc_out<bool> PREID42;
    sc_out<bool> PREID43;
    sc_out<bool> PREID50;
    sc_out<bool> PREID51;
    sc_out<bool> PREID52;
    sc_out<bool> PREID53;
    sc_out<bool> PREID60;
    sc_out<bool> PREID61;
    sc_out<bool> PREID62;
    sc_out<bool> PREID63;
    sc_out<bool> PREID70;
    sc_out<bool> PREID71;
    sc_out<bool> PREID72;
    sc_out<bool> PREID73;
    sc_out<bool> PREID80;
    sc_out<bool> PREID81;
    sc_out<bool> PREID82;
    sc_out<bool> PREID83;
    // PFEIDxy
    sc_out<bool> PFEID00;
    sc_out<bool> PFEID01;
    sc_out<bool> PFEID02;
    sc_out<bool> PFEID03;
    sc_out<bool> PFEID10;
    sc_out<bool> PFEID11;
    sc_out<bool> PFEID12;
    sc_out<bool> PFEID13;
    sc_out<bool> PFEID20;
    sc_out<bool> PFEID21;
    sc_out<bool> PFEID22;
    sc_out<bool> PFEID23;
    sc_out<bool> PFEID30;
    sc_out<bool> PFEID31;
    sc_out<bool> PFEID32;
    sc_out<bool> PFEID33;
    sc_out<bool> PFEID40;
    sc_out<bool> PFEID41;
    sc_out<bool> PFEID42;
    sc_out<bool> PFEID43;
    sc_out<bool> PFEID50;
    sc_out<bool> PFEID51;
    sc_out<bool> PFEID52;
    sc_out<bool> PFEID53;
    sc_out<bool> PFEID60;
    sc_out<bool> PFEID61;
    sc_out<bool> PFEID62;
    sc_out<bool> PFEID63;
    sc_out<bool> PFEID70;
    sc_out<bool> PFEID71;
    sc_out<bool> PFEID72;
    sc_out<bool> PFEID73;
    sc_out<bool> PFEID80;
    sc_out<bool> PFEID81;
    sc_out<bool> PFEID82;
    sc_out<bool> PFEID83;
    // TimerE
    sc_out<bool> INTATUECMIE00;
    sc_out<bool> INTATUECMIE01;
    sc_out<bool> INTATUECMIE02;
    sc_out<bool> INTATUECMIE03;
    sc_out<bool> INTATUECMIE10;
    sc_out<bool> INTATUECMIE11;
    sc_out<bool> INTATUECMIE12;
    sc_out<bool> INTATUECMIE13;
    sc_out<bool> INTATUECMIE20;
    sc_out<bool> INTATUECMIE21;
    sc_out<bool> INTATUECMIE22;
    sc_out<bool> INTATUECMIE23;
    sc_out<bool> INTATUECMIE30;
    sc_out<bool> INTATUECMIE31;
    sc_out<bool> INTATUECMIE32;
    sc_out<bool> INTATUECMIE33;
    sc_out<bool> INTATUECMIE40;
    sc_out<bool> INTATUECMIE41;
    sc_out<bool> INTATUECMIE42;
    sc_out<bool> INTATUECMIE43;
    sc_out<bool> INTATUECMIE50;
    sc_out<bool> INTATUECMIE51;
    sc_out<bool> INTATUECMIE52;
    sc_out<bool> INTATUECMIE53;
    sc_out<bool> INTATUECMIE60;
    sc_out<bool> INTATUECMIE61;
    sc_out<bool> INTATUECMIE62;
    sc_out<bool> INTATUECMIE63;
    sc_out<bool> INTATUECMIE70;
    sc_out<bool> INTATUECMIE71;
    sc_out<bool> INTATUECMIE72;
    sc_out<bool> INTATUECMIE73;
    sc_out<bool> INTATUECMIE80;
    sc_out<bool> INTATUECMIE81;
    sc_out<bool> INTATUECMIE82;
    sc_out<bool> INTATUECMIE83;
    // TimerF
    sc_out<bool> INTATUFICIF0;
    sc_out<bool> INTATUFICIF1;
    sc_out<bool> INTATUFICIF2;
    sc_out<bool> INTATUFICIF3;
    sc_out<bool> INTATUFICIF4;
    sc_out<bool> INTATUFICIF5;
    sc_out<bool> INTATUFICIF6;
    sc_out<bool> INTATUFICIF7;
    sc_out<bool> INTATUFICIF8;
    sc_out<bool> INTATUFICIF9;
    sc_out<bool> INTATUFICIF10;
    sc_out<bool> INTATUFICIF11;
    sc_out<bool> INTATUFICIF12;
    sc_out<bool> INTATUFICIF13;
    sc_out<bool> INTATUFICIF14;
    sc_out<bool> INTATUFICIF15;
    // OVIFx
    sc_out<bool> OVIF0;
    sc_out<bool> OVIF1;
    sc_out<bool> OVIF2;
    sc_out<bool> OVIF3;
    sc_out<bool> OVIF4;
    sc_out<bool> OVIF5;
    sc_out<bool> OVIF6;
    sc_out<bool> OVIF7;
    sc_out<bool> OVIF8;
    sc_out<bool> OVIF9;
    sc_out<bool> OVIF10;
    sc_out<bool> OVIF11;
    sc_out<bool> OVIF12;
    sc_out<bool> OVIF13;
    sc_out<bool> OVIF14;
    sc_out<bool> OVIF15;
    // TimerG
    sc_out<bool> INTATUGCMIG0;
    sc_out<bool> INTATUGCMIG1;
    sc_out<bool> INTATUGCMIG2;
    sc_out<bool> INTATUGCMIG3;
    sc_out<bool> INTATUGCMIG4;
    sc_out<bool> INTATUGCMIG5;
    sc_out<bool> INTATUGCMIG6;
    sc_out<bool> INTATUGCMIG7;
    sc_out<bool> INTATUGCMIG8;
    sc_out<bool> INTATUGCMIG9;
    // DMA
    sc_out<bool> INTATUCTRLDMAREQ00;
    sc_out<bool> INTATUCTRLDMAREQ01;
    sc_out<bool> INTATUCTRLDMAREQ10;
    sc_out<bool> INTATUCTRLDMAREQ11;
    sc_out<bool> at_saradtrg;

    // Internal signals
    SC_SIGNAL(bool) timerA_evt_1_0_sig;
    SC_SIGNAL(bool) timerA_evt_1_1_sig;
    SC_SIGNAL(bool) timerA_evt_1_2_sig;
    SC_SIGNAL(bool) timerA_evt_1B_sig;
    SC_SIGNAL(bool) timerA_evt_1C_sig;
    SC_SIGNAL(bool) timerA_evt_1D_sig;
    SC_SIGNAL(bool) timerA_evt_1E_sig;
    SC_SIGNAL(bool) timerA_evt_1F_sig;
    SC_SIGNAL(bool) timerA_evt_1G_sig;
    SC_SIGNAL(bool) timerA_evt_1H_sig;
    SC_SIGNAL(bool) timerA_evt_1I_sig;
    SC_SIGNAL(bool) timerA_evt_2A_sig;
    SC_SIGNAL(bool) timerA_evt_2B_sig;
    SC_SIGNAL(sc_dt::uint64) timerB_AGCKM_sig;
    SC_SIGNAL(sc_dt::uint64) timerB_AGCK1_sig;
    SC_SIGNAL(bool) timerB_AGCKTIM_sig;
    SC_SIGNAL(bool) timerB_TCNTDCLR_sig;
    
    sc_signal<bool> dummy_TOC32;
    sc_signal<bool> dummy_TOC33;
    sc_signal<bool> dummy_TOC40;
    sc_signal<bool> dummy_TOC41;
    sc_signal<bool> dummy_TOC42;
    sc_signal<bool> dummy_TOC43;
    sc_signal<bool> dummy_TOC50;
    sc_signal<bool> dummy_TOC51;
    sc_signal<bool> dummy_TOC52;
    sc_signal<bool> dummy_TOC53;
    sc_signal<bool> dummy_TOC60;
    sc_signal<bool> dummy_TOC61;
    sc_signal<bool> dummy_TOC62;
    sc_signal<bool> dummy_TOC63;
    sc_signal<bool> dummy_TOC70;
    sc_signal<bool> dummy_TOC71;
    sc_signal<bool> dummy_TOC72;
    sc_signal<bool> dummy_TOC73;
    sc_signal<bool> dummy_TOC80;
    sc_signal<bool> dummy_TOC81;
    sc_signal<bool> dummy_TOC82;
    sc_signal<bool> dummy_TOC83;
    sc_signal<bool> dummy_TOC90;
    sc_signal<bool> dummy_TOC91;
    sc_signal<bool> dummy_TOC92;
    sc_signal<bool> dummy_TOC93;
    sc_signal<bool> dummy_TOC100;
    sc_signal<bool> dummy_TOC101;
    sc_signal<bool> dummy_TOC102;
    sc_signal<bool> dummy_TOC103;
    
    /// Socket declaration
    TlmTargetSocket<32> *ts;            // For ATU5's registers

    SC_HAS_PROCESS (ATU5);
    ATU5 (sc_module_name name,
          unsigned int rLatency,
          unsigned int wLatency);
    ~ATU5 (void);
    unsigned int GetClkBus5Setting();

    /// PythonIF
    void AssertReset (const std::string reset_name, const double start_time, const double period);
    void SetCLKFreq (const std::string clock_name, const sc_dt::uint64  freq, const std::string unit);
    void GetCLKFreq (const std::string clock_name);
    void ForceRegister (const std::string module_id, const std::string block_id, const std::string reg_name, const unsigned int reg_value);
    void ReleaseRegister (const std::string module_id, const std::string block_id, const std::string reg_name);
    void WriteRegister (const std::string module_id, const std::string block_id, const std::string reg_name, const unsigned int reg_value);
    void ReadRegister (const std::string module_id, const std::string block_id, const std::string reg_name);
    void ListRegister (void);
    void Help (const std::string type);
    void SetMessageLevel (const std::string msg_lv);
    void DumpRegisterRW (const std::string is_enable);
    void DumpInterrupt (const std::string is_enable);

private:
    /// Declare methods
    void HandleCLK_HSBSignalMethod (void);
    void HandleCLK_LSBSignalMethod (void);

    void HandlePresetz_bifSignalMethod (void);
    void HandlePresetzSignalMethod (void);
    void HandleResetHardMethod (const unsigned int reset_id);
    void HandleResetCmdMethod (const unsigned int reset_id);
    void CancelResetCmdMethod (const unsigned int reset_id);

    void StartPrescalerMethod (unsigned int ch_id);
    void StopPrescalerMethod (unsigned int ch_id);
    void WriteClockBusMethod (unsigned int ch_id);
    void WriteNoiseCancelClkMethod (void);
    
    void HandleTCLKASignalMethod (void);
    void HandleTCLKBSignalMethod (void);
    void TCLKUpdateMethod (unsigned int type);
    
    void EnableTimerABMethod (void);
    
    void HandleAGCK1SignalMethod (void);
    void HandleAGCK1AssertMethod (void);
    void HandleAGCK1DeassertMethod (void);
    
    void HandleDMATrgSelectChangeMethod (void);
    void UpdateDMATrgSelectMethod (void);
    void HandleDMA0TriggerInputMethod (void);
    void HandleDMA1TriggerInputMethod (void);
    void Writeat_saradtrgMethod(void);
    void WriteTrgDMAReqMethod (unsigned int id);
    void HandleTRGSELDMA0regChangeMethod (unsigned int id);
    void HandleTRGSELDMA1regChangeMethod (unsigned int id);
    void HandleSARADTriggerInputMethod (void);
    
    void HandleAGCKMSignalMethod (void);
    
    /// Internal functions
    void EnableReset (const std::string reset_name, const bool is_active);
    void ConvertClockFreq (sc_dt::uint64 &freq_out, std::string &unit_out,
                           sc_dt::uint64 freq_in, std::string unit_in);
    void SeparateString (std::vector<std::string> &vtr, const std::string msg);
    void GetTimeResolution (sc_dt::uint64 &resolution_value, sc_time_unit &resolution_unit);
    double CalculateNextClockEdge (const double clock_period, const bool is_pos, const double current_time, const double start_time);

    void CancelEvents (void);
    void Initialize (void);
    void RegisterHandlerWrapper (const std::string module_id, const std::string block_id, const std::vector<std::string> cmd);
    unsigned int GetNoiseCancelMode (const std::string timer_name);
    void CLK_LSBUpdate (void);
    
    /// Virtual functions of ATU5_AgentController class
    bool GetResetStatus (const std::string reset_name);
    bool CheckClockPeriod (const std::string clock_name);
    void EnablePrescaler (const bool is_enable);
    void PrescalerSetupChange (const unsigned int ch_id);
    void ClkBusSetupChange (void);
    void NCClkSetupChange (void);
    void CheckClkBusSel (const unsigned int timer_id);
    void NotifyEvent (const unsigned int event, const unsigned int channel);
};
#endif
