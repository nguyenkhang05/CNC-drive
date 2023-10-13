// ---------------------------------------------------------------------
// $Id: HWCORE_RCTG.h,v 1.3 2020/08/24 03:06:10 khoaanhnguyen Exp $
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
// Description: Copyright of HWCORE_RCTG.h
// Ref: {HWCORE_RCTG_UD_Copyright_001}
#ifndef ___HWCORE_RCTR_H__
#define ___HWCORE_RCTR_H__
// Ref: {HWCORE_RCTG_UD_ATT1_002}
#include "HWCORE_RCTG_AgentController.h"
#include "CRWInterface.h"
#include "rvc_common_model.h"

#define DEFAULT_PROC_TIME   32    // Default processing time
class HWCORE_RCTG_Func;

// Description: Declaration of HWCORE_RCTG class
// Ref: {HWCORE_RCTG_UD_ATT1_004, HWCORE_RCTG_UD_ReferenceDoc_001, HWCORE_RCTG_UD_BlockDiagram_001, HWCORE_RCTG_UD_BlockDiagram_002, HWCORE_RCTG_UD_Ports_001, HWCORE_RCTG_UD_Ports_002, HWCORE_RCTG_UD_Ports_003}
// Ref: {HWCORE_RCTG_UD_Ports_004, HWCORE_RCTG_UD_Ports_007, HWCORE_RCTG_UD_Ports_008, HWCORE_RCTG_UD_Ports_009, HWCORE_RCTG_UD_Ports_010, HWCORE_RCTG_UD_Ports_011, HWCORE_RCTG_UD_Ports_012, HWCORE_RCTG_UD_Ports_013}
// Ref: {HWCORE_RCTG_UD_Ports_014, HWCORE_RCTG_UD_Ports_015, HWCORE_RCTG_UD_Ports_016, HWCORE_RCTG_UD_Ports_017, HWCORE_RCTG_UD_ATT1_008}
class HWCORE_RCTG : public sc_module
    , public HWCORE_RCTG_AgentController
    , public CRWInterface
    , public rvc_common_model
{
public:
    ////////////// Interface 
    ////////////////////////
    // Clock/Reset ports
    sc_in<sc_dt::uint64>    PCLK;                 //clock port
    sc_in<bool>             PRESETn;              //reset port
    sc_in<bool>             rst_n;                //reset port (Software reset)

    //Input ports
    sc_in<bool> output_rctg_start_sp;             //Trigger port for rectangle IP
    sc_in<bool> ang_comp0_eq_sp_80m;              //Trigger port for batch rectangle IP

    //Output unsigned short ports
    sc_out<unsigned short> rdata_em2iipcmp0;      //Value for use in angle comparison 0 of Angle Generation IP
    sc_out<unsigned short> em2icmp0;              //Software setting value for use in angle comparision 0
    sc_out<unsigned short> em2icmp1;              //Software setting value for use in angle comparision 1
    //Output bool ports
    sc_out<bool> em2ibrecctr_slctcmp0_reg;        //Value to determine angle value for use in angle comparision 0 of Angle Generation IP
    sc_out<bool> rctg_u_80m_reg;                  //Output rectangle wave value of U phase
    sc_out<bool> rctg_v_80m_reg;                  //Output rectangle wave value of V phase
    sc_out<bool> rctg_w_80m_reg;                  //Output rectangle wave value of W phase

    //Interrupts
    sc_out<bool> output_rctg_end_sp_reg;          //Done processing interrupt of Rectangle IP
    sc_out<bool> wc_em2icmp0;                     //Interrupt when EMU3CMP0 is written
    sc_out<bool> wc_em2icmp1;                     //Interrupt when EMU3CMP1 is written

    /////////////// Constructor/Destructor
    //////////////////////////////////////
    SC_HAS_PROCESS(HWCORE_RCTG);
    HWCORE_RCTG(sc_module_name name);
    ~HWCORE_RCTG();

    /////////////// Python IF
    /////////////////////////
    // Parameters
    void SetMessageLevel(const std::string msg_lv);
    void DumpRegisterRW(const std::string is_enable);
    void EnableRegisterMessage(const std::string is_enable);
    void DumpInterrupt(const std::string is_enable);
    void EnableStatInfo(const std::string is_enable);
    void ChgProcessingTime(const unsigned int value);

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
    enum eRCTGState{
        emRCTG_IDLE = 0,
        emRCTG_BUSY,
        // Number of states
        emNumState
    };
    enum eResetInfo {//Define number of reset signal
        emPRESETnId = 0,
        emrst_nId,
        // Number of resets
        emNumReset
    };

    /////////////// Class HWCORE_RCTG_Func pointer
    ////////////////////////////////////////////////////
    HWCORE_RCTG_Func            *mpHWCORE_RCTG_Func;       //pointer of HWCORE_RCTG_Func

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
    unsigned int    mProcessingTime;

    // Member variables for functional operation.
    std::map<unsigned int, sc_out<bool>*> mMapIntr;                   //Map of interrupt ports
    std::map<unsigned int, sc_out<unsigned short>*> mMapOut_UShort;   //Map of output ports with type unsigned short
    std::map<unsigned int, sc_out<bool>*> mMapOut_Bool;     //Map of output ports with type bool

    unsigned short mOutputValue[emOUTPUT_PORTS_NUM];
    unsigned int mCurRectIPState;

    /////////////// Events
    //////////////////////
    // Events for reset operations
    sc_event        mResetHardEvent[emNumReset];                //Call HandleResetHardMethod when reset is active
    sc_event        mResetCmdEvent[emNumReset];                 //Call HandleResetCmdMethod when AssertReset is called
    sc_event        mResetCmdCancelEvent[emNumReset];           //Event to cancel reset by command

    // Events for operations
    sc_event        mTriggerRectIPEvent;
    sc_event        mTriggerBRectIPEvent;
    sc_event        mProcessRectangleIPEvent;
    sc_event        mProcessBatchRectangleIPEvent;
    sc_event        mAssert_DeassertInterruptEvent[emINTERRUPT_NUM];
    sc_event        mUpdateOutputEvent[emOUTPUT_PORTS_NUM];
    sc_event        mProcessAfterPCLKChangeEvent;

    /////////////// Threads/Methods
    ///////////////////////////////
    // Thread/methods for Clock/Reset ports control
    void HandlePCLKSignalMethod(void);
    void HandleResetSignalMethod_PRESETn(void);
    void HandleResetSignalMethod_rst_n(void);

    // Threads/Methods for Clock/Reset operations control 
    void HandleResetHardMethod(const unsigned int reset_id);
    void HandleResetCmdMethod(const unsigned int reset_id);
    void CancelResetCmdMethod(const unsigned int reset_id);

    // Threads/Methods for input triggers control
    void HandleRectIPTriggerPortMethod();
    void HandleBRectIPTriggerPortMethod();

    // Threads/Methods for trigger operations
    void TriggerRectIPMethod();
    void TriggerBRectIPMethod();

    // Threads/Methods for process operations
    void RectangleIPProcessMethod();
    void BRectangleIPProcessMethod();

    // Threads/Methods for update output ports
    void Assert_DeassertInterruptMethod(unsigned int InterruptIndex);
    void UpdateOutputMethod(unsigned int OutputIndex);
    void ProcessAfterPCLKChangeMethod();

    ////////////// Functions
    ////////////////////////
    // Common functions
    void Initialize(void);
    void EnableReset(const bool is_active);
    void CancelEvents(void);

    // Overwrite functions of Agent Controller class
    bool GetResetStatus(const std::string reset_name);
    bool CheckClockPeriod(void);

    void NotifyAssert_DeassertInterrupt(unsigned int InterruptIndex);
    void NotifyUpdateOutputPorts(unsigned int OutputIndex, unsigned int value);
};
#endif // __HWCORE_RCTG_H__
