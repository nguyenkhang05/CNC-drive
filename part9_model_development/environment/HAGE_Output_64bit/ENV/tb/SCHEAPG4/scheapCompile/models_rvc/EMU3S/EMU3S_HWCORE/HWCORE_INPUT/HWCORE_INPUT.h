// -----------------------------------------------------------------------------
// $Id: HWCORE_INPUT.h,v 1.7 2020/10/26 02:43:33 quynhnhunguyen Exp $
//
// Copyright(c) 2020 Renesas Electronics Corporation
// Copyright(c) 2020 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// -----------------------------------------------------------------------------
// Description: Copyright of HWCORE_INPUT.h
// Ref: {HWCORE_INPUT_UD_Copyright_001}
// Description: About the include header files of rvc_common_model.cpp
// Ref: {HWCORE_INPUT_UD_ATT1_009}
#ifndef __HWCORE_INPUT_H__
#define __HWCORE_INPUT_H__

#include "HWCORE_INPUT_AgentController.h"
#include "CRWInterface.h"
#include "re_define.h"
#include "Common.h"
#include "rvc_common_model.h"

#define DEFAULT_PROC_TIME_INPUT0 1  // Default processing time for func(input0)
#define DEFAULT_PROC_TIME_INPUT1 1  // Default processing time for func(input1)
#define DEFAULT_PROC_TIME_INPUT2 8  // Default processing time for func(input2)
#define DEFAULT_PROC_TIME_INPUT3 14 // Default processing time for func(input3)
#define DEFAULT_BORDER_CYCCLE_INPUT 3     // Default border cycle
/// HWCORE_INPUT model class
class HWCORE_INPUT_Func;
class HWCORE_INPUT: public sc_module
            ,public HWCORE_INPUT_AgentController
            ,public CRWInterface
            ,public rvc_common_model
{
public:
    // Clock/Reset
    sc_in<sc_dt::uint64> PCLK;                                        // clock for APB
    sc_in<bool> PRESETn; 
    sc_in<bool> rst_n;
    
    // input
    sc_in<bool> input_start_sp;
    sc_in<unsigned short >    FUNCIDLEGRPA0;
    sc_in<unsigned short >    FUNCWAITGRPA;
    sc_in<unsigned short >    FUNCFLGRPA0;
    sc_in<unsigned short >    adtrg_ch0_dat;
    sc_in<unsigned short >    adtrg_ch1_dat;
    sc_in<unsigned short >    adtrg_ch2_dat;
    sc_in<unsigned short >    adtrg_thte;
    sc_in<unsigned short >    adtrg_thtr;

    // output
    sc_out<bool>            input_end_sp_reg;
    sc_out<unsigned char>   input_func_end;
    sc_out<unsigned char>   input_func_st;
    sc_out<bool>            input_func_wait;
    sc_out<bool>            input_func_busy;
    sc_out<unsigned char>   em2ictrinmd_instctr;
    sc_out<unsigned short>  theta_e_sel;
    sc_out<signed int >     ia_v_lo_fix_reg;
    sc_out<signed int >     ia_w_lo_fix_reg;
    sc_out<signed int >     ia_u_lo_fix_reg;
    sc_out<signed int >     rdata_em2iidfixobuf;
    sc_out<signed int >     rdata_em2iiqfixobuf;
    sc_out<bool>            em3ictrinmd_iirdq;
    sc_out<unsigned short > rdata_em2ithtrefixin;
    sc_out<bool>            kcl_int;
    sc_out<bool>            in_ovfl_out;
    sc_out<bool>            in_zdfl_out;
    sc_out<unsigned char>   in_ovfl_func;
    sc_out<unsigned short>  ia_1ch_lo_org_sel;
    sc_out<unsigned short>  ia_2ch_lo_org_sel;
    sc_out<unsigned short>  ia_3ch_lo_org_sel;

    SC_HAS_PROCESS (HWCORE_INPUT);
    HWCORE_INPUT (sc_module_name name);
    ~HWCORE_INPUT ();
    // PythonIF
    void SetMessageLevel (const std::string msg_lv);
    void DumpRegisterRW (const std::string is_enable);
    void DumpInterrupt (const std::string is_enable);
    void EnableRegisterMessage(const std::string is_enable);
    void EnableStatInfo(const std::string is_enable);
    void DumpStatInfo(void);
    void ChgProcessingTime(const std::string func_name, const unsigned int time_value);
    void AssertReset (const std::string reset_name, const double start_time, const double period);
    void SetCLKFreq (const std::string clock_name, const sc_dt::uint64  freq, const std::string unit);
    void GetCLKFreq (const std::string clock_name);
    void ForceRegister (const std::string reg_name, const unsigned int reg_value);
    void ReleaseRegister (const std::string reg_name);
    void WriteRegister (const std::string reg_name, const unsigned int reg_value);
    void ReadRegister (const std::string reg_name);
    void ListRegister (void);
    void Help (const std::string type);
    // Functions of HWCORE_INPUT_Func class
    void read(unsigned int offsetAddress, unsigned char* data_ptr, unsigned int size, bool debug);
    void write(unsigned int offsetAddress, unsigned char* data_ptr, unsigned int size, bool debug);

private:
    // Func
    HWCORE_INPUT_Func     *mpHWCORE_INPUTFunc;  // Pointer of HWCORE_INPUT Func
    enum eClockInfo{//Define number of clock signal
        emPCLKId                = 0
        ,emNumClk               = 1
    };
    enum eResetInfo{//Define number of reset signal
        emPRESETnId             = 0
        ,emrst_nId               = 1
        ,emNumReset             = 2
    };
    enum BIT_SHIFT_FOR_WAIT
    {
        BLOCK_00 = 0
        , BLOCK_01 = 2
        , BLOCK_02 = 4
        , BLOCK_03 = 6
    };
    // Variables
    sc_dt::uint64                       mPCLKPeriod;                    // Period of PCLK
    sc_dt::uint64                       mPCLKOrgFreq;                   // Previous frequency of PCLK
    sc_dt::uint64                       mPCLKFreq;                      // Frequency of PCLK
    std::string                         mPCLKFreqUnit;                  // Frequency unit of PCLK
    double                              mStartPCLKTime;                 // Start time of PCLK

    sc_dt::uint64   mTimeResolutionValue;           // Time resolution value of simulation
    sc_time_unit    mTimeResolutionUnit;            // Time resolution unit of simulation

    bool            mResetCurVal[emNumReset];       // Store current value of reset signals
    bool            mResetPreVal[emNumReset];       // Store previous value of reset signals
    bool            mIsResetHardActive[emNumReset]; // Is reset by port
    bool            mIsResetCmdReceive[emNumReset]; // Is receive cmd reset
    bool            mIsResetCmdActive[emNumReset];  // Is reset by cmd
    double          mResetCmdPeriod[emNumReset];    // Period of cmd reset
    bool            mEnableRegisterMessage;         // Enable/disable info/warning/error message of register IF
    bool            mEnableStatInfo;               // Enable/Disable dumping the status info
    bool            mDumpInterrupt;                 // Enable/Disable dumping the interrupt info

    //EMU3SHW
    bool minput_start_sp;                           // Store value to write to input_start_sp port   
    unsigned short mfuncidlegrpa0;                  // Store value to write to FUNCIDLEGRPA0 port              
    unsigned short mfuncflgrpa0;                    // Store value to write to FUNCFLGRPA0 port
    unsigned short madtrg_ch0_dat;                  // Store value to write to adtrg_ch0_dat port 
    unsigned short madtrg_ch1_dat;                  // Store value to write to adtrg_ch1_dat port 
    unsigned short madtrg_ch2_dat;                  // Store value to write to adtrg_ch2_dat port 
    unsigned short madtrg_thte;                     // Store value to write to adtrg_thte port
    unsigned short madtrg_thtr;                     // Store value to write to adtrg_thtr port
    unsigned short mfuncwaitgrpa;                   // Store value to write to FUNCWAITGRPA port

    bool minput_end_sp_reg;                         // Store value to write to input_end_sp_reg port
    unsigned char minput_func_end;                  // Store value to write to input_func_end port
    unsigned char minput_func_st;                   // Store value to write to input_func_st port
    bool min_ovfl_out;                              // Store value to write to in_ovfl_out port
    unsigned char min_ovfl_func;
    bool minput_func_wait;
    bool minput_func_busy;

    unsigned char mem2ictrinmd_instctr;
    unsigned short mtheta_e_sel;
    signed int mia_v_lo_fix_reg;
    signed int mia_w_lo_fix_reg;
    signed int mia_u_lo_fix_reg;
    signed int mrdata_em2iidfixobuf;
    signed int mrdata_em2iiqfixobuf;
    bool mem3ictrinmd_iirdq;
    unsigned short mrdata_em2ithtrefixin;
    bool mkcl_int;
    unsigned short mia_1ch_lo_org_sel;
    unsigned short mia_2ch_lo_org_sel;
    unsigned short mia_3ch_lo_org_sel;
    //Registers value
    bool mCTRINMD_KCL; 
    bool mCTRINMD_CMES;
    unsigned char mCTRINMD_CMUVM;
    signed int mIUFIX;
    signed int mIVFIX;
    signed int mIWFIX;
    unsigned char mKCLCTR_DATA;
    bool mKCLCTR_EN;
    unsigned int mKCLJUD;
    signed int mSR2;
    //Cycle value
    unsigned long minput_00_cyc;
    unsigned long minput_01_cyc;
    unsigned long minput_02_cyc;
    unsigned long minput_03_cyc;
    unsigned short minput_ip_cyc_sum;
    unsigned long minput_ip_border_cyc;
    // Buffer value
    bool mCTRINMD_FREGIN_buf_value;
    unsigned short mTHTESFT_buf_value;
    unsigned short mEARD_buf_value;
    signed short mADOFS0_buf_value;
    signed short mADOFS1_buf_value;
    signed short mADOFS2_buf_value;
    signed int mDIVLSB_buf_value;

    // Events
    sc_event        mResetHardEvent[emNumReset];                // Call HandleResetHardMethod when reset is active
    sc_event        mResetCmdEvent[emNumReset];                 // Call HandleResetCmdMethod when AssertReset is called
    sc_event        mResetCmdCancelEvent[emNumReset];           // Event to cancel reset by command
    //EMU3SHW
    sc_event        mWriteinput_end_sp_regEvent;     // Write port input_end_sp_reg event
    sc_event        mAssertinput_end_sp_regEvent;    // Assert input_end_sp_reg interrupt event
    sc_event        mDeassertinput_end_sp_regEvent;  // De-assert input_end_sp_reg interrupt event
    sc_event        mWriteinput_func_endEvent;
    sc_event        mWriteinput_func_stEvent;
    sc_event        mWriteinput_func_waitEvent;
    sc_event        mWriteinput_func_busyEvent;

    //INT
    sc_event        mWritein_ovfl_outEvent;     // Write port in_ovfl_out event
    sc_event        mWritein_zdfl_outEvent;     // Write port in_ovfl_out event
    sc_event        mWritein_ovfl_funcEvent;     // Write port in_ovfl_out event

    sc_event        mWriteem2ictrinmd_instctrEvent;        // Write port em2ictrinmd_instctr event
    sc_event        mWritetheta_e_selEvent;                // Write port theta_e_sel event
    sc_event        mWriteia_v_lo_fix_regEvent;            // Write port ia_v_lo_fix_reg event
    sc_event        mWriteia_w_lo_fix_regEvent;            // Write port ia_w_lo_fix_reg event
    sc_event        mWriteia_u_lo_fix_regEvent;            // Write port ia_u_lo_fix_reg event
    sc_event        mWriterdata_em2iidfixobufEvent;        // Write port rdata_em2iidfixobuf event
    sc_event        mWriterdata_em2iiqfixobufEvent;        // Write port rdata_em2iiqfixobuf event
    sc_event        mWriteem3ictrinmd_iirdqEvent;          // Write port em3ictrinmd_iirdq event
    sc_event        mWriterdata_em2ithtrefixinEvent;       // Write port rdata_em2ithtrefixin event
    sc_event        mWritekcl_intEvent;                    // Write port kcl_int event
    sc_event        mWriteia_1ch_lo_org_selEvent;          // Write port ia_1ch_lo_org_sel event
    sc_event        mWriteia_2ch_lo_org_selEvent;          // Write port ia_2ch_lo_org_sel event
    sc_event        mWriteia_3ch_lo_org_selEvent;          // Write port ia_3ch_lo_org_sel event

    // Main Func
    sc_event        m_input00;
    sc_event        m_input01;
    sc_event        m_input02;
    sc_event        m_input03;
    // Cycle
    sc_event        m_input00_add_cyc;
    sc_event        m_input01_add_cyc;
    sc_event        m_input02_add_cyc;
    sc_event        m_input03_add_cyc;
    // Communication method
    // Clock handle
    void HandlePCLKSignalMethod(void);
    //Input handle method
    // Reset handle
    void HandlePRESETnSignalMethod(void);
    void Handlerst_nSignalMethod(void);
    void HandleResetHardMethod(const unsigned int reset_id);
    void HandleResetCmdMethod(const unsigned int reset_id);
    void CancelResetCmdMethod(const unsigned int reset_id);
    // Input
    void Handleinput_startMethod(void);         ///Method to handle input_start_sp
    void HandleFUNCWAITGRPAMethod(void);        ///Method to handle FUNCWAITGRPA
    void HandleFUNCFLGRPA0Method(void);         ///Method to handle FUNCFLGRPA0
    void HandleADTRGMethod(void);               ///Handle adtrg_ch0_dat, adtrg_ch1_dat, adtrg_ch2_dat, adtrg_thte, adtrg_thtr Method
    //Output
    void Writeinput_end_sp_regMethod(void);     //Write port input_end_sp_reg
    void Assertinput_end_sp_regMethod(void);    //Assert port input_end_sp_reg
    void Deassertinput_end_sp_regMethod(void);  //De-assert port input_end_sp_reg

    void Writeinput_func_endMethod(void);       //Write port input_func_end
    void Writeinput_func_stMethod(void);        //Write port input_func_st
    void Writeinput_func_waitMethod(void);      //Write port input_func_st
    void Writeinput_func_busyMethod(void);      //Write port input_func_st
    //INT  
    void Writein_ovfl_outMethod(void);          //Write port in_ovfl_out
    void Writein_ovfl_funcMethod(void);         //Write port in_ovfl_func
    void Writein_zdfl_outMethod(void);          //Write port in_zdfl_out

    void Writeem2ictrinmd_instctrMethod(void);  //Write port em2ictrinmd_instctr
    void Writetheta_e_selMethod(void);          //Write port theta_e_sel
    void Writeia_v_lo_fix_regMethod(void);      //Write port ia_v_lo_fix_reg
    void Writeia_w_lo_fix_regMethod(void);      //Write port ia_w_lo_fix_reg
    void Writeia_u_lo_fix_regMethod(void);      //Write port ia_u_lo_fix_reg
    void Writerdata_em2iidfixobufMethod(void);  //Write port rdata_em2iidfixobuf
    void Writerdata_em2iiqfixobufMethod(void);  //Write port rdata_em2iiqfixobuf
    void Writeem3ictrinmd_iirdqMethod(void);    //Write port em3ictrinmd_iirdq
    void Writerdata_em2ithtrefixinMethod(void); //Write port rdata_em2ithtrefixin
    void Writekcl_intMethod(void);              //Write port kcl_int
    void Writeia_1ch_lo_org_selMethod(void);    //Write port ia_1ch_lo_org_sel
    void Writeia_2ch_lo_org_selMethod(void);    //Write port ia_2ch_lo_org_sel
    void Writeia_3ch_lo_org_selMethod(void);    //Write port ia_3ch_lo_org_sel

    void ResetOutputPort(void);

    void HandleState_input_ip_00Method(void);   // Handle State input0
    void HandleState_input_ip_01Method(void);   // Handle State input1
    void HandleState_input_ip_02Method(void);   // Handle State input2
    void HandleState_input_ip_03Method(void);   // Handle State input3
    void HandleAdd_cyc_input_ip_00Method(void); // Handle Cycle input0
    void HandleAdd_cyc_input_ip_01Method(void); // Handle Cycle input1
    void HandleAdd_cyc_input_ip_02Method(void); // Handle Cycle input2
    void HandleAdd_cyc_input_ip_03Method(void); // Handle Cycle input3

    // Internal Functions
    void Init_output_port(void);
    void Initialize (void);
    void EnableReset (const bool is_active);
    void CancelEvents();
    void DumpInterruptMsg (const std::string intr_name, const bool value);
    // Overwrite virtual functions of HWCORE_INPUT_AgentController
    bool CheckClockPeriod(void);
    bool GetResetStatus(const std::string reset_name);
    // Main function
    void Input0(void);
    void Input1_IO(void);
    void Input1(unsigned short ethti, unsigned short *ethto);
    void Input2_IO(void);
    void Input2(signed short adi0, signed short adi1, signed short adi2, signed int *iuo, signed int *ivo, signed int *iwo, bool *kclstat, unsigned char *kclcnt, bool *kclint);
    void Input3_IO(void);
    void Input3(signed int iui, signed int ivi, signed int iwi, unsigned short ethti, signed int *ido, signed int *iqo);
    //Overflow Check
    typedef unsigned long long OVFC_U;
    typedef signed long long   OVFC_S;
    void Chk_overflow_u(OVFC_U input, OVFC_U check, bool* ovfflag);
    void Chk_overflow_s(OVFC_S input, bool* ovfflag);
    //Restart from wait
    void Input_ip_restart_from_wait(void);
    //State for input
    void Set_buf_input_ip_start(void);
    void Set_state_idle_ip(void);
    void Set_state_wait_ip(void);
    void Set_state_busy_ip(void);
    //cycle
    unsigned short Judge_cyc_input_ip(unsigned short cyc, bool end_flag);
    // End model
    bool Judge_input_ip_end(unsigned char shift);
    void End_input_ip(void);
    // Update value of output port
    void NotifyUpdateOutputPorts(const std::string port_name, unsigned int value);
};
#endif //__HWCORE_INPUT_H__
