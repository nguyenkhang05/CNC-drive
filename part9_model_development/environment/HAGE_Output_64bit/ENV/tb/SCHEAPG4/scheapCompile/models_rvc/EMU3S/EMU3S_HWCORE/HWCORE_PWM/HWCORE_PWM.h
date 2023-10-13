// -----------------------------------------------------------------------------
// $Id: HWCORE_PWM.h,v 1.5 2020/10/27 01:44:51 ducla Exp $
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
// Description: Copyright of HWCORE_PWM.h
// Ref: {HWCORE_PWM_UD_Copyright_001}
// Description: About the include header files of rvc_common_model.cpp
// Ref: {HWCORE_PWM_UD_ATT1_009}
#ifndef __HWCORE_PWM_H__
#define __HWCORE_PWM_H__

#include "HWCORE_PWM_AgentController.h"
#include "CRWInterface.h"
#include "re_define.h"
#include "Common.h"
#include "rvc_common_model.h"

#define DEFAULT_PROC_TIME_PWM0  0x01  // Default processing time for func(pwm0)
#define DEFAULT_PROC_TIME_PWM1  0x07  // Default processing time for func(pwm1)
#define DEFAULT_PROC_TIME_PWM2  0x02
#define DEFAULT_PROC_TIME_PWM3  0x11
#define DEFAULT_PROC_TIME_PWM4  0x13
#define DEFAULT_PROC_TIME_PWM5  0x0E
#define DEFAULT_PROC_TIME_PWM6  0x02
#define DEFAULT_PROC_TIME_PWM7  0x06
#define DEFAULT_PROC_TIME_PWM8  0x06
#define DEFAULT_PROC_TIME_PWM9  0x05
#define DEFAULT_PROC_TIME_PWM10 0x05
#define DEFAULT_BORDER_CYCCLE_PWM   0x06   // Default border cycle
/// HWCORE_PWM model class
class HWCORE_PWM_Func;
class HWCORE_PWM: public sc_module
            ,public HWCORE_PWM_AgentController
            ,public CRWInterface
            ,public rvc_common_model
{
public:
    // Clock/Reset
    sc_in<sc_dt::uint64>     PCLK;               // clock for APB
    sc_in<sc_dt::uint64>     CCLK;               // clock for output
    sc_in<bool>              PRESETn; 
    sc_in<bool>              rst_n;
    sc_in<bool>              crst_n;             // reset for CCLK
    
    //--------------------------------------------- 
    //                   Input Port
    //--------------------------------------------- 
    // from Overall Control
    sc_in<bool>              output_pwm_start_sp;
    sc_in<unsigned short>    FUNCIDLEGRPA2;
    sc_in<unsigned short>    FUNCWAITGRPA;
    sc_in<unsigned int>      FUNCFLGRPA2;
    sc_in<unsigned char>     REFCTR;
    // from INPUT IP
    sc_in<unsigned short>    theta_e_reg;
    sc_in<int>               ia_u_lo_fix_reg;
    sc_in<int>               ia_v_lo_fix_reg;
    sc_in<int>               ia_w_lo_fix_reg;
    // from PI Control IP
    sc_in<int>               id_sel_out;
    sc_in<int>               iq_sel_out;
    sc_in<int>               id_dir_reg;
    sc_in<int>               iq_dir_reg;
    sc_in<int>               id_reg;
    sc_in<int>               iq_reg;
    sc_in<int>               rdata_em2ivd;
    sc_in<int>               rdata_em2ivq;
    // from Angle Generation IP
    sc_in<unsigned short>    theta_e_fix_reg;
    // from RDC(external terminal)
    sc_in<int>               rdc_high_omega_pclk;        
    
    //--------------------------------------------- 
    //                   Output Port
    //--------------------------------------------- 
    // to Overall Control
    sc_out<bool>             output_pwm_end_sp_reg;
    sc_out<unsigned short>   pwm_func_end;
    sc_out<unsigned short>   pwm_func_st;
    sc_out<bool>             pwm_func_wait;
    sc_out<bool>             pwm_func_busy;
    // to Verification buffer
    sc_out<unsigned int>     tstwdcmpu_pclk;
    sc_out<unsigned int>     tstwdcmpv_pclk;
    sc_out<unsigned int>     tstwdcmpw_pclk;
    // to TSG3 
    sc_out<unsigned int>     tstwdcmp0;      // Carrier period signal
    sc_out<unsigned int>     tstwdcmpu;
    sc_out<unsigned int>     tstwdcmpv;
    sc_out<unsigned int>     tstwdcmpw;
    sc_out<bool>             tstwecmp;       // Write enable of carrier cycle/compare value
    // to Interrupt Control 
    sc_out<unsigned char>    pwm_ovfl_func;  // Func ID that the interrupt signal occurs
    sc_out<bool>             pwm_ovfl_out;   // PWM IP overflow occurred
    sc_out<bool>             pwm_zdfl_out;   // PWM IP division by zero occurs


    SC_HAS_PROCESS (HWCORE_PWM);
    HWCORE_PWM (sc_module_name name);
    ~HWCORE_PWM ();
    
    //--------------------------------------------- 
    //                   Python IF
    //--------------------------------------------- 
    // Parameter
    void SetMessageLevel (const std::string msg_lv);
    void DumpRegisterRW (const std::string is_enable);
    void DumpInterrupt (const std::string is_enable);
    void EnableRegisterMessage(const std::string is_enable);
    void EnableStatInfo(const std::string is_enable);
    // Command
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

    // Functions of HWCORE_PWM_Func class
    void read(unsigned int offsetAddress, unsigned char* data_ptr, unsigned int size, bool debug);
    void write(unsigned int offsetAddress, unsigned char* data_ptr, unsigned int size, bool debug);

private:
    // Func
    HWCORE_PWM_Func     *mpHWCORE_PWMFunc;  // Pointer of HWCORE_PWM Func
    enum eClockInfo{//Define number of clock signal
        emPCLKId                = 0
        ,emCCLKId               = 1
        ,emNumClk               = 2
    };
    enum eResetInfo{//Define number of reset signal
        emPRESETnId             = 0
        ,emrst_nId              = 1
        ,emcrst_nId             = 2
        ,emNumReset             = 3
    };
    enum BIT_SHIFT_FOR_WAIT
    {
        BLOCK_00 = 0
        , BLOCK_01 = 2
        , BLOCK_02 = 4
        , BLOCK_03 = 6
        , BLOCK_04 = 8
        , BLOCK_05 = 10
        , BLOCK_06 = 12
        , BLOCK_07 = 14
        , BLOCK_08 = 16
        , BLOCK_09 = 18
        , BLOCK_10 = 20
    };
    //----------------------------------------------
    //                   Variables
    //---------------------------------------------- 
    /* PCLK */
    sc_dt::uint64   mPCLKPeriod;                    // Period of PCLK
    sc_dt::uint64   mPCLKOrgFreq;                   // Previous frequency of PCLK
    sc_dt::uint64   mPCLKFreq;                      // Frequency of PCLK
    /* CCLK */
    sc_dt::uint64   mCCLKPeriod;                    // Period of CCLK
    sc_dt::uint64   mCCLKOrgFreq;                   // Previous frequency of CCLK
    sc_dt::uint64   mCCLKFreq;                      // Frequency of CCLK

    std::string     mPCLKFreqUnit;                  // Frequency unit of PCLK
    std::string     mCCLKFreqUnit;                  // Frequency unit of PCLK

    double          mStartPCLKTime;                 // Start time of PCLK
    double          mStartCCLKTime;

    sc_dt::uint64   mTimeResolutionValue;           // Time resolution value of simulation
    sc_time_unit    mTimeResolutionUnit;            // Time resolution unit of simulation

    bool            mResetCurVal[emNumReset];       // Store current value of reset signals
    bool            mResetPreVal[emNumReset];       // Store previous value of reset signals
    bool            mIsResetHardActive[emNumReset]; // Is reset by port
    bool            mIsResetCmdReceive[emNumReset]; // Is receive cmd reset
    bool            mIsResetCmdActive[emNumReset];  // Is reset by cmd
    double          mResetCmdPeriod[emNumReset];    // Period of cmd reset
    bool            mEnableRegisterMessage;         // Enable/disable info/warning/error message of register IF
    bool            mEnableStatInfo;                // Enable/Disable dumping the status info
    bool            mDumpInterrupt;                 // Enable/Disable dumping the interrupt info

    //EMU3SHW
    bool           m_pwm_start_sp;                  // Store value from output_pwm_start_sp port

    bool           m_pwm_end_sp_reg;                // Store value to write to output_pwm_end_sp_reg port;
    unsigned short m_pwm_func_end;                  // Store value to write to pwm_func_end port
    unsigned short m_pwm_func_st;                   // Store value to write to pwm_func_st port
    bool           m_pwm_func_wait;
    bool           m_pwm_func_busy;
    bool           m_pwm_ovfl_out;                  // Store value to write to pwm_ovfl_out port
    bool           m_pwm_zdiv_out;                  // Store value to write to pwm_zdiv_out port
    unsigned char  m_pwm_ovfl_func;

    //Registers value
    /* pwm1 */ 
    int            m_buf_rdc_omega;                  // RDC omega input buffer
    int            m_vdcrct;                         // EMU3nVDCRCT
    int            m_vqcrct;                         // EMU3nVQCRCT
    bool           m_pwmctr_setdec;                  // EMU3nPWMCTR.SETDEC
    bool           m_pwmctr_setvel;                  // EMU3nPWMCTR.SETVEL
    unsigned char  m_pwmctr_decidq;                  // EMU3nPWMCTR.DECIDQ

    int            m_velsft;                         // EMU3nVELSFT
    int            m_vel;                            // EMU3nVEL
    int            m_decvelg;                        // EMU3nDECVELG
    int            m_decflux;                        // EMU3nDECFLUX
    int            m_decld;                          // EMU3nDECLD
    int            m_declq;                          // EMU3nDECLQ
    unsigned int   m_vd2max;                         // EMU3nVD2MAX
    unsigned int   m_vq2max;                         // EMU3nVQ2MAX

    /* pwm2 */
    unsigned char  m_pwmctr_flininip;                // EMU3nPWMCTR.FLININIP
    unsigned short m_thtforesft;                     // EMU3nTHTFORESFT
    short int      m_phi;                            // EMU3nPHI
    unsigned int   m_gtht;                           // EMU3nGTHT

    /* pwm3 */
    bool           m_pwmctr_vdqsel;                  // EMU3nPWMCTR.VDQSEL
    int            m_sr23;

    /* pwm4 */
    unsigned char  m_pwmctr_setharm;                 // EMU3nPWMCTR.SETHARM

    /* pwm5 */
    int            m_pwmk1;                          // EMU3nPWMK1
    short int      m_volv;                           // EMU3nVOLV 

    /* pwm6 */
    short int      m_vuofs;                          // EMU3nVUOFS
    short int      m_vvofs;                          // EMU3nVVOFS
    short int      m_vwofs;                          // EMU3nVWOFS

    /* pwm7 */
    int            m_dtul;                           // EMU3nDTUL
    int            m_dtll;                           // EMU3nDTLL

    /* pwm8 */
    bool           m_pwmctr_pwmsel;                  // EMU3nPWMCTR.PWMSEL
    unsigned char  m_pwmctr_pwmop;                   // EMU3nPWMCTR.PWMOP
    unsigned int   m_carr;                           // EMU3nCARR
    unsigned int   m_dtt;                            // EMU3nDTT
    int            m_pwmk2;                          // EMU3nPWMK2

    /* pwm9 */
    bool           m_pwmctr_setdto;                  // EMU3nPWMCTR.SETDTO
    unsigned int   m_dtoth;                          // EMU3nDTOTH
    int            m_dtopv;                          // EMU3nDTOPV
    int            m_dtonv;                          // EMU3nDTONV

    /* pwm10 */
    unsigned int   m_pwmul;                          // EMU3nPWMUL
    unsigned int   m_pwmll;                          // EMU3nPWMLL

    /* pwm_out */
    bool           m_pwmctr_setpwm;                  // EMU3nPWMCTR.SETPWM
    bool           m_pwmctr_shipwm;                  // EMU3nPWMCTR.SHIPWM
    bool           m_pwmdt_pwmdt;                    // EMU3nPWMDT.PWMDT
    unsigned int   m_pwmu;                           // EMU3nPWMU
    unsigned int   m_pwmv;                           // EMU3nPWMV
    unsigned int   m_pwmw;                           // EMU3nPWMW

    /* pwm end */
    bool           m_pwmdt_write_flag;               // PWMDT write flag
    bool           m_pwm_end_flag;
    // temporary variables for TSG3 output
    unsigned int   m_tstwdcmpu_temp;
    unsigned int   m_tstwdcmpv_temp;
    unsigned int   m_tstwdcmpw_temp;
    unsigned int   m_tstwdcmp0_temp;
    bool           m_tstwecmp_temp;

    bool           m_change_tstwecmp_flag;

    //Cycle value
    unsigned long  m_pwm_00_cyc;
    unsigned long  m_pwm_01_cyc;
    unsigned long  m_pwm_02_cyc;
    unsigned long  m_pwm_03_cyc;
    unsigned long  m_pwm_04_cyc;
    unsigned long  m_pwm_05_cyc;
    unsigned long  m_pwm_06_cyc;
    unsigned long  m_pwm_07_cyc;
    unsigned long  m_pwm_08_cyc;
    unsigned long  m_pwm_09_cyc;
    unsigned long  m_pwm_10_cyc;
    unsigned short m_pwm_ip_cyc_sum;
    unsigned long  m_pwm_ip_border_cyc;
    // Buffer value

    //----------------------------------------------
    //                   Events
    //----------------------------------------------
    // Events
    sc_event        mResetHardEvent[emNumReset];                // Call HandleResetHardMethod when reset is active
    sc_event        mResetCmdEvent[emNumReset];                 // Call HandleResetCmdMethod when AssertReset is called
    sc_event        mResetCmdCancelEvent[emNumReset];           // Event to cancel reset by command
    sc_event        mInit_CCLK_output_port;                     // Event to Init output port related to CCLK domain
 
    //EMU3SHW
    sc_event        mAssert_pwm_end_sp_regEvent;                // Assert output_pwm_end_sp_reg interrupt event
    sc_event        mWrite_pwm_func_endEvent;
    sc_event        mWrite_pwm_func_stEvent;
    sc_event        mWrite_pwm_func_waitEvent;
    sc_event        mWrite_pwm_func_busyEvent;

    //INT
    sc_event        mWrite_pwm_ovfl_outEvent;                   // Write port in_ovfl_out event
    sc_event        mWrite_pwm_ovfl_funcEvent;                  // Write port in_ovfl_out event
    sc_event        mWrite_pwm_zdfl_outEvent;                   // Write port in_ovfl_out event
 
    // Main Func
    sc_event        m_pwm00;
    sc_event        m_pwm01;
    sc_event        m_pwm02;
    sc_event        m_pwm03;
    sc_event        m_pwm04;
    sc_event        m_pwm05;
    sc_event        m_pwm06;
    sc_event        m_pwm07;
    sc_event        m_pwm08;
    sc_event        m_pwm09;
    sc_event        m_pwm10;
    sc_event        m_pwm_out;
 
    // Cycle
    sc_event        m_pwm00_add_cyc;
    sc_event        m_pwm01_add_cyc;
    sc_event        m_pwm02_add_cyc;
    sc_event        m_pwm03_add_cyc;
    sc_event        m_pwm04_add_cyc;
    sc_event        m_pwm05_add_cyc;
    sc_event        m_pwm06_add_cyc;
    sc_event        m_pwm07_add_cyc;
    sc_event        m_pwm08_add_cyc;
    sc_event        m_pwm09_add_cyc;
    sc_event        m_pwm10_add_cyc;
    // TSG3 output
    sc_event        m_tsg_output_from_pwm_out;
    
    // Verification Buffer
    sc_event        m_output_to_verification_buff;

    //----------------------------------------------
    //             Communication method
    //----------------------------------------------
    // Clock handle
    void HandlePCLKSignalMethod(void);
    void HandleCCLKSignalMethod(void);
    // Input handle method
    // Reset handle
    void HandlePRESETnSignalMethod(void);
    void Handle_rst_SignalMethod(void);
    void Handle_crst_SignalMethod(void);
    void HandleResetHardMethod(const unsigned int reset_id);
    void HandleResetCmdMethod(const unsigned int reset_id);
    void CancelResetCmdMethod(const unsigned int reset_id);
    
    // Output
    void Handle_pwm_end_sp_regMethod(void);    //Handle port pwm_end_sp_reg

    void Write_pwm_func_endMethod(void);       //Write port pwm_func_end
    void Write_pwm_func_stMethod(void);        //Write port pwm_func_st
    void Write_pwm_func_waitMethod(void);      //Write port pwm_func_st
    void Write_pwm_func_busyMethod(void);      //Write port pwm_func_st
    //INT  
    void Write_pwm_ovfl_outMethod(void);       //Write port pwm_ovfl_out
    void Write_pwm_ovfl_funcMethod(void);      //Write port pwm_ovfl_func
    void Write_pwm_zdfl_outMethod(void);       //Write port pwm_zdfl_out
    
    // TSG3
    void Tsg_output_from_pwm_outMethod(void);  // Output to TSG3
    
    // Verification buffer
    void Ver_output_from_pwm_outMethod(void);        // Output to verification buffer
    
    //----------------------------------------------
    //                Core Operation
    //----------------------------------------------
    // Start
    void Start_pwm_ipMethod(void);                   ///Method to handle output_pwm_start_sp
    void Pwm_ip_restart_from_waitMethod(void);       ///Method to handle FUNCWAITGRPA

    // Handle state
    void Func_pwm_ip_00Method(void);                 // Handle State pwm0
    void Func_pwm_ip_01Method(void);                 // Handle State pwm1
    void Func_pwm_ip_02Method(void);                 // Handle State pwm2
    void Func_pwm_ip_03Method(void);                 // Handle State pwm3
    void Func_pwm_ip_04Method(void);                 // Handle State pwm4
    void Func_pwm_ip_05Method(void);                 // Handle State pwm5
    void Func_pwm_ip_06Method(void);                 // Handle State pwm6
    void Func_pwm_ip_07Method(void);                 // Handle State pwm7
    void Func_pwm_ip_08Method(void);                 // Handle State pwm8
    void Func_pwm_ip_09Method(void);                 // Handle State pwm9
    void Func_pwm_ip_10Method(void);                 // Handle State pwm10
    void Func_pwm_ip_outMethod(void);                // Handle State pwm_out
    
    // cycle
    unsigned short Judge_cyc_pwm_ip(unsigned short cyc, bool end_flag); // Helper
    void Add_cyc_pwm_ip_00Method(void);              // Handle Cycle pwm0
    void Add_cyc_pwm_ip_01Method(void);              // Handle Cycle pwm1
    void Add_cyc_pwm_ip_02Method(void);              // Handle Cycle pwm2
    void Add_cyc_pwm_ip_03Method(void);              // Handle Cycle pwm3
    void Add_cyc_pwm_ip_04Method(void);              // Handle Cycle pwm4
    void Add_cyc_pwm_ip_05Method(void);              // Handle Cycle pwm5
    void Add_cyc_pwm_ip_06Method(void);              // Handle Cycle pwm6
    void Add_cyc_pwm_ip_07Method(void);              // Handle Cycle pwm7
    void Add_cyc_pwm_ip_08Method(void);              // Handle Cycle pwm8
    void Add_cyc_pwm_ip_09Method(void);              // Handle Cycle pwm9
    void Add_cyc_pwm_ip_10Method(void);              // Handle Cycle pwm10
 
    // End model
    bool Judge_pwm_ip_end(unsigned char shift); // Helper
    void End_pwm_ip(void);

    // Internal Functions
    void Init_output_port(void);
    void Reset_output_port(void);
    void Initialize (void);
    void EnableReset (const bool is_active);
    void EnableResetRegister(const bool is_active);
    void EnableReset_CCLK(const bool is_active);
    void CancelEvents(void);
    void CancelEvents_CCLK(void);
    void DumpInterruptMsg (const std::string intr_name, const bool value);

    // Overwrite virtual functions of HWCORE_PWM_AgentController
    bool CheckClockPeriod(const std::string clock_name);
    bool GetResetStatus(const std::string reset_name);
    void Handle_PWMDT(void);

    // Main function
    void Pwm0(void);
    void Pwm1(int vel, int vdi, int vqi, int idi, int iqi, int *vdo, int *vqo);
    void Pwm2(unsigned short etht1i, unsigned short etht2i, unsigned short *ethto);
    void Pwm3(unsigned short ethti, unsigned short vthti, int vdi, int vqi, int vdqi, int *vuo, int *vvo, int *vwo);
    void Pwm4(int tmax, unsigned short ethti, unsigned short vthti, int vdqi, int vui,  int vvi, int vwi, int *vuo, int *vvo, int *vwo);
    void Pwm5(int vui, int vvi, int vwi, int *vuo, int *vvo, int *vwo);
    void Pwm6(int vui, int vvi, int vwi, int *vuo, int *vvo, int *vwo);
    void Pwm7(int vu1i, int vv1i, int vw1i, int *vuo, int *vvo, int *vwo);
    void Pwm8(int vui, int vvi, int vwi, int *pwmuo, int *pwmvo, int *pwmwo);
    void Pwm9(int pwmui, int pwmvi, int pwmwi, int iui, int ivi, int iwi, int *pwmuo, int *pwmvo, int *pwmwo);
    void Pwm10(int pwmui, int pwmvi, int pwmwi, unsigned int *pwmuo, unsigned int *pwmvo, unsigned int *pwmwo, unsigned int *carro);
    void Pwm_out(unsigned int pwmui, unsigned int pwmvi, unsigned int pwmwi, unsigned int carri, unsigned int *pwmuo, unsigned int *pwmvo, unsigned int *pwmwo, unsigned int *carro);
    
    void Pwm0_io(void);
    void Pwm1_io(void);
    void Pwm2_io(void);
    void Pwm3_io(void);
    void Pwm4_io(void);
    void Pwm5_io(void);
    void Pwm6_io(void);
    void Pwm7_io(void);
    void Pwm8_io(void);
    void Pwm9_io(void);
    void Pwm10_io(void);

    // Overflow Check
    typedef unsigned long long OVFC_U;
    typedef signed long long   OVFC_S;
    void Chk_overflow_u(OVFC_U input, OVFC_U check, bool* ovfflag);
    void Chk_overflow_s(OVFC_S input, OVFC_U check, bool* ovfflag);
    void Calc_division(int devidend, short int divisor, int *ret, bool *pwm_Zdiv); //helper

    // Preprocessing for pwm
    void Set_buf_pwm_ip_start(void);
    void Set_state_idle_ip(void);
    void Set_state_wait_ip(void);
    void Set_state_busy_ip(void);

};
#endif //__HWCORE_PWM_H__
