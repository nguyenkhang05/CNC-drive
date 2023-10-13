// ---------------------------------------------------------------------
// $Id: HWCORE_IRCTG3.h,v 1.5 2020/10/15 06:49:06 khoaanhnguyen Exp $
//
// Copyright(c) 2020 Renesas Electronics Corporation
// Copyright(c) 2020 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------
// Description: Copyright of HWCORE_IRCTG3.h
//Ref: {HWCORE_IRCTG3_UD_ATT1_008, HWCORE_IRCTG3_UD_ATT1_013}
#ifndef ___HWCORE_IRCTR3_H__
#define ___HWCORE_IRCTR3_H__
#include "HWCORE_IRCTG3_AgentController.h"
#include "CRWInterface.h"
#include "rvc_common_model.h"

class HWCORE_IRCTG3_Func;

// Description: Declaration of HWCORE_IRCTG3 class
class HWCORE_IRCTG3 : public sc_module
    , public HWCORE_IRCTG3_AgentController
    , public CRWInterface
    , public rvc_common_model
{
public:
    //Ref: {HWCORE_IRCTG3_UD_BlockDiagram_001, HWCORE_IRCTG3_UD_BlockDiagram_002, HWCORE_IRCTG3_UD_ReferenceDoc_001}
    ////////////// Interface 
    ////////////////////////
    // Clock/Reset ports
    //Ref: {HWCORE_IRCTG3_UD_Ports_001, HWCORE_IRCTG3_UD_Ports_002, HWCORE_IRCTG3_UD_Ports_003}
    sc_in<sc_dt::uint64>    PCLK;                 //clock port                  
    sc_in<bool>             PRESETn;              //reset port
    sc_in<bool>             mip_prst_n;           //reset port (Software reset)

    //Ref: {HWCORE_IRCTG3_UD_Ports_004}
    //Input ports
    sc_in<unsigned short>  e_angle;

    //Ref: {HWCORE_IRCTG3_UD_Ports_012, HWCORE_IRCTG3_UD_Ports_013, HWCORE_IRCTG3_UD_Ports_014, HWCORE_IRCTG3_UD_Summary_001}
    //Output bool ports
    sc_out<bool>  ir3_pat_u_r;
    sc_out<bool>  ir3_pat_v_r;
    sc_out<bool>  ir3_pat_w_r;

    //Ref: {HWCORE_IRCTG3_UD_Ports_005, HWCORE_IRCTG3_UD_Ports_006, HWCORE_IRCTG3_UD_Ports_007, HWCORE_IRCTG3_UD_Ports_008, HWCORE_IRCTG3_UD_Ports_009}
    //Ref: {HWCORE_IRCTG3_UD_Ports_010, HWCORE_IRCTG3_UD_Ports_011, HWCORE_IRCTG3_UD_Ports_015, HWCORE_IRCTG3_UD_Ports_016, HWCORE_IRCTG3_UD_Ports_017}
    //Interrupts
    sc_out<bool>  ir3_int_err_r;
    sc_out<bool>  ir3_int_u_r;
    sc_out<bool>  ir3_int_v_r;
    sc_out<bool>  ir3_int_w_r;
    sc_out<bool>  ir3_calc_u_r;
    sc_out<bool>  ir3_calc_v_r;
    sc_out<bool>  ir3_calc_w_r;
    sc_out<bool>  ir3_adc_u_r;
    sc_out<bool>  ir3_adc_v_r;
    sc_out<bool>  ir3_adc_w_r;

    /////////////// Constructor/Destructor
    //////////////////////////////////////
    SC_HAS_PROCESS(HWCORE_IRCTG3);
    HWCORE_IRCTG3(sc_module_name name);
    ~HWCORE_IRCTG3();

    /////////////// Python IF
    /////////////////////////
    // Parameters
    void SetMessageLevel(const std::string msg_lv);
    void DumpRegisterRW(const std::string is_enable);
    void EnableRegisterMessage(const std::string is_enable);
    void DumpInterrupt(const std::string is_enable);
    void EnableStatInfo(const std::string is_enable);

    // Commands
    void DumpStatInfo(void);
    void AssertReset(const std::string reset_name, const double start_time, const double period);
    void SetCLKFreq(const std::string clock_name, const sc_dt::uint64 freq, const std::string unit);
    void GetCLKFreq(const std::string clock_name);
    void ForceRegister(const std::string reg_name, const unsigned int reg_value);
    void ReleaseRegister(const std::string reg_name);
    void WriteRegister(const std::string reg_name, const unsigned int reg_value);
    void ReadRegister(const std::string reg_name);
    void ListRegister(void);
    void Help(const std::string type);

    // Overwrite functions of RWInterface class
    void read(unsigned int addr, unsigned char* p_data, unsigned int size, bool debug);
    void write(unsigned int addr, unsigned char* p_data, unsigned int size, bool debug);
private:
    enum eResetInfo {//Define number of reset signal
        emPRESETnId = 0,
        emmip_prst_nId,
        // Number of resets
        emNumReset
    };

    /////////////// Class HWCORE_IRCTG3_Func pointer
    ////////////////////////////////////////////////////
    HWCORE_IRCTG3_Func            *mpHWCORE_IRCTG3_Func;       //pointer of HWCORE_IRCTG3_Func

    /////////////// Member variables
    ////////////////////////////////
    // Member variables for clock operation.
    sc_dt::uint64   mPCLKPeriod;                    //Period of PCLK
    sc_dt::uint64   mPCLKOrgFreq;                   //Previous frequency of PCLK
    sc_dt::uint64   mPCLKFreq;                      //Frequency of PCLK
    std::string     mPCLKFreqUnit;                  //Frequency unit of PCLK
    double          mStartClkTime;                  // Start time of clock

    sc_dt::uint64   mTimeResolutionValue;           //Time resolution value of simulation
    sc_time_unit    mTimeResolutionUnit;            //Time resolution unit of simulation

    // Member variables for reset operation.
    bool            mResetCurVal[emNumReset];                   //Store current value of reset signals
    bool            mResetPreVal[emNumReset];                   //Store previous value of reset signals
    bool            mIsResetHardActive[emNumReset];             //Is reset by port
    bool            mIsResetCmdReceive[emNumReset];             //Is receive cmd reset
    bool            mIsResetCmdActive[emNumReset];              //Is reset by cmd
    double          mResetCmdPeriod[emNumReset];                //Period of cmd reset
    // Parameters
    bool            mEnableRegisterMessage;         //Enable/disable info/warning/error message of register IF
    bool            mDumpInterrupt;                 //Enable/disable dumping interrupt information
    bool            mDumpStatInfo;                  //Enable/disable dumping statistical information

    // Member variables for functional operation.
    std::map<unsigned int, sc_out<bool>*> mMapIntr;                   //Map of interrupt ports
    std::map<unsigned int, sc_out<bool>*> mMapOut;               //Map of output ports with type bool
    bool mOutputValue[emOUTPUT_PORTS_NUM];
    bool mInterruptValue[emINTERRUPT_NUM];
    unsigned short mPrevAngle;

    /////////////// Events
    //////////////////////
    // Events for reset operations
    sc_event        mResetHardEvent[emNumReset];                //Call HandleResetHardMethod when reset is active
    sc_event        mResetCmdEvent[emNumReset];                 //Call HandleResetCmdMethod when AssertReset is called
    sc_event        mResetCmdCancelEvent[emNumReset];           //Event to cancel reset by command

    // Events for operations
    sc_event        mProcessAfterPCLKChangeEvent;
    sc_event        mUpdateOutputPortEvent;
    sc_event        mUpdateInterruptPortEvent;
    sc_event        mRegisterBlockEvent;
    sc_event        mZoneSearchAndIterruptEvent;

    /////////////// Threads/Methods
    ///////////////////////////////
    // Thread/methods for Clock/Reset ports control
    void HandlePCLKSignalMethod(void);
    void HandleResetSignalMethod_PRESETn(void);
    void HandleResetSignalMethod_mip_prst_n(void);

    // Threads/Methods for Clock/Reset operations control 
    void HandleResetHardMethod(const unsigned int reset_id);
    void HandleResetCmdMethod(const unsigned int reset_id);
    void CancelResetCmdMethod(const unsigned int reset_id);

    // Threads/Methods for operation
    void HandleSignalMethod_e_angle();
    void RegisterBlockOperationMethod();
    void ZoneSeachAndInterruptOperationMethod();
    // Methods for update output ports
    void UpdateInterruptPortMethod();
    void UpdateOutputPortMethod();

    ////////////// Functions
    ////////////////////////
    // Common functions
    void Initialize(void);
    void EnableReset(const bool is_active);
    void CancelEvents(void);

    // Overwrite functions of Agent Controller class
    bool GetResetStatus(const std::string reset_name);
    bool CheckClockPeriod(void);

    //APIs for trigger event
    void NotifyStartupTriggerGenerated();
    void NotifyUpdateOutputPort();
    //APIs for updating output/interrupt values
    void UpdateInterruptValue(unsigned int InterruptIndex, bool value);
    void UpdateOutputValue(unsigned int OutputIndex, unsigned int value);
};
#endif // __HWCORE_IRCTG3_H__
