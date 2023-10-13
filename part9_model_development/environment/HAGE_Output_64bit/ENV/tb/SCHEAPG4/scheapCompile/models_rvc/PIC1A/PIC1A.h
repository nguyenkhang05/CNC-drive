// ---------------------------------------------------------------------
// $Id: PIC1A.h,v 1.1 2014/12/15 09:31:43 binhnguyen Exp $
//
// Copyright(c) 2014 Renesas Electronics Corporation
// Copyright(c) 2014 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------
#ifndef __PIC1A_H__
#define __PIC1A_H__
#include "BusSlaveBase.h"
#include "PIC1A_AgentController.h"

class PIC1A_Func;
class PIC1A;

class PIC_Port: public sc_module
{
friend class PIC1A;

public:
    sc_in<bool>  *InputPort      [PIC1A_AgentController::emInputNum];
    sc_out<bool> *OutputPort     [PIC1A_AgentController::emOutputNum];
    sc_out<bool> *SyncOutputPort [PIC1A_AgentController::emSyncOutputNum];
    
    SC_HAS_PROCESS (PIC_Port);
    PIC_Port (sc_module_name name, PIC1A *PIC_Ptr);
    ~PIC_Port (void);
    
private:
    PIC1A *mPIC_Ptr;

    bool mWriteValue [PIC1A_AgentController::emOutputNum];
    bool mWriteSyncValue [PIC1A_AgentController::emSyncOutputNum];
    bool mDT_Input [PIC1A_AgentController::emDTnum];           // Store the value of DTnm inputs
    bool mRS_SetInput [PIC1A_AgentController::emRSnum];        // Store the value of S input of RSnm
    bool mRS_ResetInput [PIC1A_AgentController::emRSnum];      // Store the value of R input of RSnm
    bool mWNG_CKENInput [2][PIC1A_AgentController::emWNGnum];  // Store the value of CKEN input of WNGnm
    
    void HandleInputPortMethod        (const unsigned int index);
    void HandleInputChangeMethod      (const unsigned int index);
    void WriteOutputPortMethod        (const unsigned int index);
    void AssertSyncOutputPortMethod   (const unsigned int index);
    void DeassertSyncOutputPortMethod (const unsigned int index);
    void WriteSyncOutputPortMethod    (const unsigned int index);

    sc_event mHandleInputChangeEvent  [PIC1A_AgentController::emInputNum];
    sc_event mWriteOutputPortEvent    [PIC1A_AgentController::emOutputNum];
    sc_event mAssertSyncOutputEvent   [PIC1A_AgentController::emSyncOutputNum];
    sc_event mDeassertSyncOutputEvent [PIC1A_AgentController::emSyncOutputNum];
    sc_event mWriteSyncOutputEvent    [PIC1A_AgentController::emSyncOutputNum];
    
    sc_event mUpdateInputEvent;         // Update value of input ports after reset
    
    /// Internal functions
    void Initialize (void);
};

class PIC1A: public sc_module,
            public PIC1A_AgentController,
            public BusSlaveBase<32,1>
{
friend class PIC_Port;
    
private:
    enum eResetActiveLevel {    // Define active level of PRESETZ port
        emPRESETZActive = 0
    };

    PIC1A_Func *mPIC1A_Func;            //PIC1A_Func class instance
    PIC_Port *mPIC_Port;            //PIC_Port class instance

    /// Declare variables
    sc_dt::uint64 mPCLKPeriod;          //Period value of PCLK clock
    sc_dt::uint64 mPCLKFreq;            //Frequency value of PCLK clock
    sc_dt::uint64 mPCLKOrgFreq;         //Orginal frequency value of PCLK clock
    std::string mPCLKFreqUnit;          //Frequency unit of PCLK clock
    
    sc_dt::uint64 mSync_chsb_clsbPeriod;    //Period value of sync_chsb_clsb clock
    sc_dt::uint64 mSync_chsb_clsbFreq;      //Frequency value of sync_chsb_clsb clock

    sc_dt::uint64 mTimeResolutionValue;     //Time resolution value of simulation
    sc_time_unit mTimeResolutionUnit;       //Time resolution unit of simulation

    bool mPRESETZSignalCurrentValue;    //Store current value of PRESETZ signal
    bool mPRESETZSignalPreviousValue;   //Store previous value of PRESETZ signal
    bool mIsPRESETZHardActive;          //Reset operation status of PRESETZ signal
    bool mIsPRESETZCmdReceive;          //Receive status of AssertReset command of PRESETZ signal
    bool mIsPRESETZCmdActive;           //Active status of AssertReset command of PRESETZ signal
    double mPRESETZCmdPeriod;           //Period of AssertReset command of PRESETZ signal

    /// Declare events
    sc_event mPRESETZHardEvent;         //Call HandlePRESETZHardMethod() when PRESETZ is active
    sc_event mPRESETZCmdEvent;          //Call HandlePRESETZCmdMethod() when AssertReset of PRESETZ is called
    sc_event mPRESETZCmdCancelEvent;    //Call CancelPRESETZCmdMethod() when AssertReset of PRESETZ is over
    
    sc_event mUpdateFlagEvent [2][PIC1A_AgentController::emWNGnum];           //Call UpdateFlagMethod() to update WNGFn flag
    sc_event mUpdateWNG_FFiOuputEvent [2][PIC1A_AgentController::emWNGnum];   //Call UpdateWNG_FFiOuputMethod() to update FFi output
    sc_event mUpdateWNG_FFgOuputEvent [2][PIC1A_AgentController::emWNGnum];   //Call UpdateWNG_FFgOuputMethod() to update FFg output
    sc_event mUpdateREG513_FFOuputEvent [2][PIC1A_AgentController::emFFnum];  //Call UpdateREG513_FFOuputMethod() to update FF output of PIC0REG5n[13]
    sc_event mUpdateREG514_FFOuputEvent [2][PIC1A_AgentController::emFFnum];  //Call UpdateREG514_FFOuputMethod() to update FF output of PIC0REG5n[14]
    sc_event mUpdateTSGOUTCTR_FFOuputEvent [2][PIC1A_AgentController::emFFnum]; //Call UpdateTSGOUTCTR_FFOuputMethod() to update FF output of PIC0TSGOUTCTRn[1-6]
    sc_event mHandleResetRSEvent [PIC1A_AgentController::emRSnum];              //Call HandleResetRSMethod() to reset RSnm
    sc_event mHandleResetDTEvent [PIC1A_AgentController::emDTnum];              //Call HandleResetRSMethod() to reset DTnm
    sc_event mHandleResetRTOEvent [2][PIC1A_AgentController::emRTOnum];         //Call HandleResetRTOMethod() to reset RTOnm

public:
    /// Declare input signals
    sc_in<sc_dt::uint64> PCLK;
    sc_in<bool> PRESETZ;
    // ENCA0
    sc_in<bool> ENCAT0IOV;
    sc_in<bool> ENCAT0IEC;
    sc_in<bool> ENCAT0INT0;
    sc_in<bool> ENCAT0EQ0;
    sc_in<bool> ENCAT0ECS;
    sc_in<bool> ENCAT0EQ1;
    sc_in<bool> ENCAT0INT1;
    sc_in<bool> ENCAT0IUD;
    // ENCA1
    sc_in<bool> ENCAT1IOV;
    sc_in<bool> ENCAT1IEC;
    sc_in<bool> ENCAT1INT0;
    sc_in<bool> ENCAT1EQ0;
    sc_in<bool> ENCAT1ECS;
    sc_in<bool> ENCAT1EQ1;
    sc_in<bool> ENCAT1INT1;
    // ENCA2
    sc_in<bool> ENCAT2IOV;
    sc_in<bool> ENCAT2INT0;
    sc_in<bool> ENCAT2INT1;
    // ENCA3
    sc_in<bool> ENCAT3IOV;
    sc_in<bool> ENCAT3INT0;
    sc_in<bool> ENCAT3INT1;
    // TAUD0
    sc_in<bool> *TAUD0CDEN[emPortNumTAUD];
    sc_in<bool> *TAUD0TOUT[emPortNumTAUD];
    sc_in<bool> TAUD0UDC0;
    sc_in<bool> TAUD0UDC2;
    sc_in<bool> TAUD0UDC8;
    sc_in<bool> *TAUD0INT[emPortNumTAUD];
    sc_in<bool> TAUD0CKEN10;
    sc_in<bool> TAUD0CKEN12;
    sc_in<bool> TAUD0CKEN14;
    // TAUD1
    sc_in<bool> *TAUD1CDEN[emPortNumTAUD];
    sc_in<bool> *TAUD1TOUT[emPortNumTAUD];
    sc_in<bool> TAUD1UDC0;
    sc_in<bool> TAUD1UDC2;
    sc_in<bool> TAUD1UDC8;
    sc_in<bool> *TAUD1INT[emPortNumTAUD];
    sc_in<bool> TAUD1CKEN10;
    sc_in<bool> TAUD1CKEN12;
    sc_in<bool> TAUD1CKEN14;
    // TAUJ0
    sc_in<bool> TAUJ0TOUT3;
    // TAUJ1
    sc_in<bool> TAUJ1TOUT3;
    // TPBA0
    sc_in<bool> TPB0INTPAT;
    // TSG0
    sc_in<bool> TSG0TSTPTE;
    sc_in<bool> TSG0TS2PEC;
    sc_in<bool> TSG0TST2PUD;
    sc_in<bool> TSG0TO1;
    sc_in<bool> TSG0TO2;
    sc_in<bool> TSG0TO3;
    sc_in<bool> TSG0TO4;
    sc_in<bool> TSG0TO5;
    sc_in<bool> TSG0TO6;
    sc_in<bool> TSG0INT0;
    sc_in<bool> TSG0INT2;
    sc_in<bool> TSG0INT6;
    sc_in<bool> TSG0INT10;
    sc_in<bool> TSG0TSTIER;
    // TSG1
    sc_in<bool> TSG1TSTPTE;
    sc_in<bool> TSG1TS2PEC;
    sc_in<bool> TSG1TST2PUD;
    sc_in<bool> TSG1TO1;
    sc_in<bool> TSG1TO2;
    sc_in<bool> TSG1TO3;
    sc_in<bool> TSG1TO4;
    sc_in<bool> TSG1TO5;
    sc_in<bool> TSG1TO6;
    sc_in<bool> TSG1INT0;
    sc_in<bool> TSG1INT2;
    sc_in<bool> TSG1INT6;
    sc_in<bool> TSG1INT10;
    sc_in<bool> TSG1TSTIER;
    // PIC2
    sc_in<bool> *ADCATTOUT0[emPortNumPIC2];
    sc_in<bool> *ADCATTOUT1[emPortNumPIC2];
    // DNF
    sc_in<bool> *DNFTAUD0TIN[emPortNumTAUD];
    sc_in<bool> *DNFTAUD1TIN[emPortNumTAUD];
    sc_in<bool> DNFENCAT0AIN;
    sc_in<bool> DNFENCAT0BIN;
    sc_in<bool> DNFENCAT0TIN1;
    sc_in<bool> DNFENCAT0ZIN;
    sc_in<bool> DNFENCAT1AIN;
    sc_in<bool> DNFENCAT1BIN;
    sc_in<bool> DNFENCAT1TIN1;
    sc_in<bool> DNFENCAT1ZIN;
    sc_in<bool> *DNFTAUJ0TIN[emPortNumTAUJ];
    sc_in<bool> DNFTSG0TSTCKE;
    sc_in<bool> DNFTSG1TSTCKE;
    sc_in<bool> DNFSIF0IREF;
    sc_in<bool> DNFSIF1IREF;
    sc_in<bool> DNFENCAT2TIN1;
    sc_in<bool> DNFENCAT3TIN1;
    sc_in<bool> *ANFESO[emPortNumANFESO];
    // ECM
    sc_in<bool> ERROROUTZ;
    // HiZ Source
    sc_in<bool> *RSVHIZIN[emPortNumHiZ];
    // PORT(TSG0 Hall Sensor)
    sc_in<bool> *PORTTSG0TSTAPT[emPortNumTSG];
    // PORT(TSG1 Hall Sensor)
    sc_in<bool> *PORTTSG1TSTAPT[emPortNumTSG];
    // RDC0
    sc_in<bool> SAT0AOUT;
    sc_in<bool> SAT0BOUT;
    sc_in<bool> SAT0ZOUT;
    // RDC1
    sc_in<bool> SAT1AOUT;
    sc_in<bool> SAT1BOUT;
    sc_in<bool> SAT1ZOUT;
    // Others
    sc_in<sc_dt::uint64> sync_chsb_clsb;

    /// Declare output signals
    // ENCA0
    sc_out<bool> ENCAT0TSST;
    sc_out<bool> ENCAT0AIN;
    sc_out<bool> ENCAT0BIN;
    sc_out<bool> ENCAT0ZIN;
    sc_out<bool> ENCAT0TIN1;
    // ENCA1
    sc_out<bool> ENCAT1TSST;
    sc_out<bool> ENCAT1AIN;
    sc_out<bool> ENCAT1BIN;
    sc_out<bool> ENCAT1ZIN;
    sc_out<bool> ENCAT1TIN1;
    // ENCA2
    sc_out<bool> ENCAT2TSST;
    sc_out<bool> ENCAT2AIN;
    sc_out<bool> ENCAT2BIN;
    sc_out<bool> ENCAT2ZIN;
    sc_out<bool> ENCAT2TIN1;
    // ENCA3
    sc_out<bool> ENCAT3TSST;
    sc_out<bool> ENCAT3AIN;
    sc_out<bool> ENCAT3BIN;
    sc_out<bool> ENCAT3ZIN;
    sc_out<bool> ENCAT3TIN1;
    // TAPA0
    sc_out<bool> TOP0TAPATHASIN;
    sc_out<bool> TOP0TAPATSIM0;
    sc_out<bool> TOP0TAPATUDCM0;
    // TAPA1
    sc_out<bool> TOP1TAPATHASIN;
    sc_out<bool> TOP1TAPATSIM0;
    sc_out<bool> TOP1TAPATUDCM0;
    // TAPA2
    sc_out<bool> TOP2TAPATHASIN;
    // TAPA3
    sc_out<bool> TOP3TAPATHASIN;
    // TAUD0
    sc_out<bool> *TAUD0SST[emPortNumTAUD];
    sc_out<bool> *TAUD0TIN[emPortNumTAUD];
    // TAUD1
    sc_out<bool> *TAUD1SST[emPortNumTAUD];
    sc_out<bool> *TAUD1TIN[emPortNumTAUD];
    // TAUJ0
    sc_out<bool> *TAUJ0TSST[emPortNumTAUJ];
    sc_out<bool> *TAUJ0TIN[emPortNumTAUJ];
    // TAUJ1
    sc_out<bool> *TAUJ1TSST[emPortNumTAUJ];
    // TPBA0
    sc_out<bool> TPB0TSST;
    // TPBA1
    sc_out<bool> TPB1TSST;
    // TSG0
    sc_out<bool> TSG0TSST;
    sc_out<bool> TSG0TSTOPS;
    sc_out<bool> TSG0TSTOPC0;
    sc_out<bool> TSG0TSTOPC1;
    sc_out<bool> TSG0TSTCKE;
    sc_out<bool> *TSG0TSTAPT[emPortNumTSG];
    sc_out<bool> TSG0TSTIUS;
    sc_out<bool> TSG0TSTIUC;
    sc_out<bool> TSG0TSTIVS;
    sc_out<bool> TSG0TSTIVC;
    sc_out<bool> TSG0TSTIWS;
    sc_out<bool> TSG0TSTIWC;
    // TSG1
    sc_out<bool> TSG1TSST;
    sc_out<bool> TSG1TSTOPS;
    sc_out<bool> TSG1TSTOPC0;
    sc_out<bool> TSG1TSTOPC1;
    sc_out<bool> TSG1TSTCKE;
    sc_out<bool> *TSG1TSTAPT[emPortNumTSG];
    sc_out<bool> TSG1TSTIUS;
    sc_out<bool> TSG1TSTIUC;
    sc_out<bool> TSG1TSTIVS;
    sc_out<bool> TSG1TSTIVC;
    sc_out<bool> TSG1TSTIWS;
    sc_out<bool> TSG1TSTIWC;
    // OSTM0
    sc_out<bool> OST0TSST;
    // OSTM1
    sc_out<bool> OST1TSST;
    // OSTM2
    sc_out<bool> OST2TSST;
    // OSTM3
    sc_out<bool> OST3TSST;
    // INTC
    sc_out<bool> INTENC0CM0;
    sc_out<bool> INTENC1CM0;
    sc_out<bool> INTENC2CM0;
    sc_out<bool> INTENC3CM0;
    // PORT(TAUD0)
    sc_out<bool> TOP0U;
    sc_out<bool> TOP0UB;
    sc_out<bool> TOP0V;
    sc_out<bool> TOP0VB;
    sc_out<bool> TOP0W;
    sc_out<bool> TOP0WB;
    // PORT(TAUD1)
    sc_out<bool> TOP1U;
    sc_out<bool> TOP1UB;
    sc_out<bool> TOP1V;
    sc_out<bool> TOP1VB;
    sc_out<bool> TOP1W;
    sc_out<bool> TOP1WB;
    // PORT(TSG0)
    sc_out<bool> TOP2U;
    sc_out<bool> TOP2UB;
    sc_out<bool> TOP2V;
    sc_out<bool> TOP2VB;
    sc_out<bool> TOP2W;
    sc_out<bool> TOP2WB;
    // PORT(TSG1)
    sc_out<bool> TOP3U;
    sc_out<bool> TOP3UB;
    sc_out<bool> TOP3V;
    sc_out<bool> TOP3VB;
    sc_out<bool> TOP3W;
    sc_out<bool> TOP3WB;

    /// Declare target socket
    TlmTargetSocket<32> *ts;

    SC_HAS_PROCESS (PIC1A);
    PIC1A (sc_module_name name,
          unsigned int rLatency,
          unsigned int wLatency);
    ~PIC1A (void);
    
    /// PythonIF
    void AssertReset (const double start_time, const double period);
    void SetCLKFreq (const sc_dt::uint64  freq, const std::string unit);
    void GetCLKFreq (void);
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
    void HandlePRESETZHardMethod (void);
    void HandlePRESETZCmdMethod (void);
    void CancelPRESETZCmdMethod (void);
    
    void HandleSync_chsb_clsbSignalMethod (void);
    void UpdateFlagMethod (const unsigned int module, const unsigned int index);
    void UpdateWNG_FFiOuputMethod (const unsigned int module, const unsigned int index);
    void UpdateWNG_FFgOuputMethod (const unsigned int module, const unsigned int index);
    void UpdateREG513_FFOuputMethod (const unsigned int module, const unsigned int index);
    void UpdateREG514_FFOuputMethod (const unsigned int module, const unsigned int index);
    void UpdateTSGOUTCTR_FFOuputMethod (const unsigned int module, const unsigned int index);
    void HandleResetRSMethod (const unsigned int index);
    void HandleResetDTMethod (const unsigned int index);
    void HandleResetRTOMethod (const unsigned int module, const unsigned int index);

    /// Internal functions
    void EnableReset (const bool is_active);
    void ConvertClockFreq (sc_dt::uint64 &freq_out, std::string &unit_out,
                           sc_dt::uint64 freq_in, std::string unit_in);
    void SeparateString (std::vector<std::string> &vtr, const std::string msg);
    void GetTimeResolution (sc_dt::uint64 &resolution_value, sc_time_unit &resolution_unit);
    double CalculateCLKPosEdge (const std::string clock_name, const double time_point);
    
    void Initialize (void);

    /// Virtual functions of PIC1A_AgentController class
    bool GetResetStatus (void);
    bool CheckClockPeriod (const std::string clock_name);
    
    void TriggerSyncStart (const unsigned int index);
    void UpdateOutputPort (const unsigned int index, const bool value);
    void TriggerSettingFlag (const unsigned int module, const unsigned int index);
    void WNG_FFiChange (const unsigned int module, const unsigned int index);
    void WNG_FFgChange (const unsigned int module, const unsigned int index);
    void REG513_FFChange (const unsigned int module, const unsigned int index);
    void REG514_FFChange (const unsigned int module, const unsigned int index);
    void TSGOUTCTR_FFChange (const unsigned int module, const unsigned int index);
    void ResetRS (const unsigned int index);
    void ResetDT (const unsigned int index);
    void ResetRTO (const unsigned int module, const unsigned int index);
};
#endif
