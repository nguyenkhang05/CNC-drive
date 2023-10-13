// ---------------------------------------------------------------------
// $Id: HWCORE_INT.h,v 1.2 2020/11/20 02:27:36 huyquocpham Exp $
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
// Description: Copyright of HWCORE_INT.h
// Ref: {EMU3S_HWCORE_UD_ATT1_001, EMU3S_HWCORE_UD_ATT1_009}
#ifndef __HWCORE_INT_H__
#define __HWCORE_INT_H__
#include "HWCORE_INT_AgentController.h"
#include "CRWInterface.h"
#include "rvc_common_model.h"
#include <vector>

class HWCORE_INT_Func;
class HWCORE_VRFBUF_Func;

// Description: Declaration of HWCORE_INT class
// Ref: {EMU3S_HWCORE_UD_Summary_002, EMU3S_HWCORE_UD_Direction_023, EMU3S_HWCORE_UD_Direction_060}
class HWCORE_INT : public sc_module
    , public HWCORE_INT_AgentController
    , public CRWInterface
    , public rvc_common_model
{
public:
    // Description: Clock/Reset I/F
    // Ref: {EMU3S_HWCORE_UD_Ports_019}
    sc_in<sc_dt::uint64>    PCLK;                 //SSCG clock port
    sc_in<bool>             PRESETn;              //APB reset port
    sc_in<bool>             rst_n;                //SW reset port
    
    // Description: EMU3S_HWCORE connection
    // Ref: {EMU3S_HWCORE_UD_Ports_020}
    sc_in<bool>             funca_wait_int;
    sc_in<bool>             funcb_wait_int;
    sc_out<bool>            *emu_int[8];

    // Description: IIR Filter connection
    // Ref: {EMU3S_HWCORE_UD_Ports_021}
    sc_in<bool>             iir_end0;
    sc_in<bool>             iir_end1;
    sc_in<bool>             iir_end2;
    sc_in<bool>             iir_ovfl_in;
    sc_in<unsigned char>    iir_ovfl_func;

    // Description: Input IP connection
    // Ref: {EMU3S_HWCORE_UD_Ports_022}
    sc_in<bool>             input_end_sp_reg;    
    sc_in<bool>             kcl_int;
    sc_in<bool>             in_ovfl_in;
    sc_in<unsigned char>    in_ovfl_func;
    sc_in<bool>             in_zdfl_in;
    sc_in<unsigned short>   ia_1ch_lo_org_sel;
    sc_in<unsigned short>   ia_2ch_lo_org_sel;    
    sc_in<unsigned short>   ia_3ch_lo_org_sel;
    sc_in<unsigned short>   u16or_res_angle;
    sc_in<signed int>       s32or_id_lo;
    sc_in<signed int>       s32or_iq_lo;

    // Description: PI IP connection
    // Ref: {EMU3S_HWCORE_UD_Ports_023}
    sc_in<bool>             pi_end_sp_reg;
    sc_in<bool>             pi_ovfl_in;
    sc_in<bool>             pi_ovfl_func;

    // Description: PWM IP connection
    // Ref: {EMU3S_HWCORE_UD_Ports_024}
    sc_in<bool>             output_pwm_end_sp_reg;
    sc_in<unsigned int>     tstwdcmpu;
    sc_in<unsigned int>     tstwdcmpv;
    sc_in<unsigned int>     tstwdcmpw;
    sc_in<bool>             pwm_ovfl_in;
    sc_in<unsigned char>    pwm_ovfl_func;
    sc_in<bool>             pwm_zdfl_in;

    // Description: RCTG IP connection
    // Ref: {EMU3S_HWCORE_UD_Ports_025}
    sc_in<bool>             output_rctg_end_sp_reg;
    sc_in<bool>             rctg_u_80m_reg;
    sc_in<bool>             rctg_v_80m_reg;
    sc_in<bool>             rctg_w_80m_reg;

    // Description: Angle generation IP connection
    // Ref: {EMU3S_HWCORE_UD_Ports_026} 
    sc_in<bool>             ang_comp0_eq_sp;
    sc_in<bool>             ang_comp1_eq_sp;

    // Description: IRCTG3 IP connection
    // Ref: {EMU3S_HWCORE_UD_Ports_027}
    sc_in<bool>             irctg3_err;
    sc_in<bool>             irctg3_u_int;
    sc_in<bool>             irctg3_v_int;
    sc_in<bool>             irctg3_w_int;
    sc_in<bool>             irctg3_ucmp_trig;
    sc_in<bool>             irctg3_vcmp_trig;
    sc_in<bool>             irctg3_wcmp_trig;
    sc_in<bool>             irctg3_upatout_reg;
    sc_in<bool>             irctg3_vpatout_reg;
    sc_in<bool>             irctg3_wpatout_reg;

    // Description: ADTRG IP connection
    // Ref: {EMU3S_HWCORE_UD_Ports_028} 
    sc_in<bool>             tstpektim;
    sc_in<bool>             tstvlytim;
    sc_in<bool>             adc_trig;
    sc_in<bool>             ins_trig;
    sc_in<bool>             adtrg_mvint0;
    sc_in<bool>             adtrg_mvint1;
    sc_in<bool>             adtrg_mem_rg_chg;
    sc_in<bool>             adtrg_smerr;
    sc_in<bool>             adtrg_amerr;

    // Description: Angle Measurement IP connection
    // Ref: {EMU3S_HWCORE_UD_Ports_029}
    sc_in<bool>             pmt2of;
    sc_in<bool>             pmt2cp;

    SC_HAS_PROCESS(HWCORE_INT);
    //Constructor/Destructor
    HWCORE_INT(sc_module_name name);
    ~HWCORE_INT();

    // Virtual functions of CRWInterface class
    void read(unsigned int offsetAddress, unsigned char* data_ptr, unsigned int size, bool debug);
    void write(unsigned int offsetAddress, unsigned char* data_ptr, unsigned int size, bool debug);

    // Python IF
    void SetMessageLevel(const std::string msg_lv);
    void DumpRegisterRW(const std::string is_enable);
    void EnableRegisterMessage(const std::string is_enable);
    void DumpInterrupt (const std::string is_enable);
    

    void AssertReset(const std::string reset_name, const double start_time, const double period);
    void SetCLKFreq(const std::string clock_name, const sc_dt::uint64 freq, const std::string unit);
    void GetCLKFreq(const std::string clock_name);
    void ForceRegister(const std::string ip_name, const std::string reg_name, const unsigned int reg_value);
    void ReleaseRegister(const std::string ip_name, const std::string reg_name);
    void WriteRegister(const std::string ip_name, const std::string reg_name, const unsigned int reg_value);
    void ReadRegister(const std::string ip_name, const std::string reg_name);
    void ListRegister(const std::string ip_name);
    void Help(const std::string type);
    void ChgProcessingTime (const unsigned int cycle);

private:
    // Class HWCORE_INT_Func pointer
    HWCORE_INT_Func            *mpHWCORE_INTFunc;  
    HWCORE_VRFBUF_Func         *mpHWCORE_VRFBUFFunc;

    enum eResetInfo{
        emPRESETnId             = 0
        , emrst_nId             = 1
        , emResetSignalNum      = 2
    };

    enum eInterruptInfo
    {
        emINT_NUM = 8
    };

    // Variables
    sc_dt::uint64   mPCLKPeriod;                    //Period of PCLK
    sc_dt::uint64   mPCLKOrgFreq;                   //Previous frequency of PCLK
    sc_dt::uint64   mPCLKFreq;                      //Frequency of PCLK
    std::string     mPCLKFreqUnit;                  //Frequency unit of PCLK
    double          mTimePCLKChangePoint;                 // Start time of PCLK

    sc_dt::uint64   mTimeResolutionValue;           //Time resolution value of simulation
    sc_time_unit    mTimeResolutionUnit;            //Time resolution unit of simulation

    // Description: Reset variables
    // Ref: {EMU3S_HWCORE_UD_ATT1_004}
    bool            mResetCurVal[emResetSignalNum];            //Store current value of PRESETn or rst_n
    bool            mResetPreVal[emResetSignalNum];            //Store previous value of PRESETn or rst_n
    bool            mIsResetHardActive[emResetSignalNum];      //Reset operation status of PRESETn or rst_n signal
    bool            mIsResetCmdReceive[emResetSignalNum];      //Receive status of AssertReset command of PRESETn or rst_n
    bool            mIsResetCmdActive[emResetSignalNum];       //Active status of AssertReset command of PRESETn or rst_n
    double          mResetCmdPeriod[emResetSignalNum];         //Period of AssertReset command of PRESETn or rst_n
    bool            mEnableRegisterMessage;                    //Enable/disable info/warning/error message of register IF
    bool            mDumpInterrupt;                            //Enable/Disable dumping the interrupt info

    unsigned int    mINT_cyc;
    bool            mIntVal[emINT_NUM];

    // Buffer Variables
    unsigned int CBAD[3];
    unsigned int CBTHTRESFIXIN;
    unsigned int CBIDFIX;
    unsigned int CBIQFIX;
    unsigned int CBPWMUIP;
    unsigned int CBPWMVIP;
    unsigned int CBPWMWIP;
    unsigned int CBBREC;
    unsigned int CBIREC3;

    // Reset Events
    sc_event        mResetHardEvent[emResetSignalNum];         //Call HandleResetHardMethod when PRESETn or rst_n is active
    sc_event        mResetCmdEvent[emResetSignalNum];          //Call HandleResetCmdMethod when AssertReset is called
    sc_event        mResetCmdCancelEvent[emResetSignalNum];    //Call CancelResetCmdMethod when AssertReset is over
    sc_event        mResetPortsEvent;                          //Reset Ports when Reset

    sc_event        mAssertEmuIntEvent[emINT_NUM];               //Assert emu_int output
    sc_event        mDeassertEmuIntEvent[emINT_NUM];             //Deassert emu_int output
    sc_event        mWriteEmuIntEvent[emINT_NUM];

    sc_event        mBufferRegistersEvent;                     //Trigger Buffer Registers
    
    // Threads/Methods
    void HandlePCLKSignalMethod(void);
    void HandlePRESETnSignalMethod(void);
    void Handlerst_nSignalMethod(void);

    void HandleResetHardMethod(const unsigned int reset_id);
    void HandleResetCmdMethod(const unsigned int reset_id);
    void CancelResetCmdMethod(const unsigned int reset_id);

    void MonitorInputEndSPMethod(void);
    void MonitorPIEndSPMethod(void);
    void MonitorPWMEndSPMethod(void);
    void MonitorRCTGEndSPMethod(void);
    void MonitorTstvlytimMethod(void);
    void MonitorTstpektimMethod(void);
    void MonitorAngCmp0SPMethod(void);
    void MonitorAngCmp1SPMethod(void);
    void MonitorInsTrigMethod(void);
    void MonitorPmt2CpMethod(void);
    void MonitorADCTrigMethod(void);
    void MonitorADTRGMvint0Method(void);
    void MonitorADTRGMvint1Method(void);
    void MonitorADTRGMemMethod(void);
    void MonitorIIREnd0Method(void);
    void MonitorIIREnd1Method(void);
    void MonitorIIREnd2Method(void);
    void MonitorIRCTG3UIntMethod(void);
    void MonitorIRCTG3VIntMethod(void);
    void MonitorIRCTG3WIntMethod(void);
    void MonitorFuncAWaitMethod(void);
    void MonitorFuncBWaitMethod(void);

    void MonitorINPUTOvfMethod(void);
    void MonitorIIROvfMethod(void);
    void MonitorPIOvfMethod(void);
    void MonitorPWMOvfMethod(void);
    void MonitorINPUTDivByZeroMethod(void);
    void MonitorPWMDivByZeroMethod(void);
    void MonitorTimerOvfMethod(void);
    void MonitorKirLawViolateMethod(void);
    void MonitorOverlapStartADMethod(void);
    void MonitorOverlapWrADSamplMethod(void);
    void MonitorIRCTG3ErrorMethod(void);

    void MonitorIRCTG3UCmpMethod(void);
    void MonitorIRCTG3VCmpMethod(void);
    void MonitorIRCTG3WCmpMethod(void);

    void AssertEmuIntMethod(const unsigned int int_id);
    void DeassertEmuIntMethod(const unsigned int int_id);
    void WriteEmuIntMethod(const unsigned int int_id);
    
    void EnableReset(const bool is_active);
    void Initialize(void);
    void ResetPortsMethod(void);
    void CancelEvents(void);
    void DumpInterruptMsg(const std::string intr_name, const bool value);
    
    void JudgeInterruptOutput(std::string enable_bit_name);
    void JudgeErrorOutput(std::string enable_bit_name);
    void UpdateErrorReg(std::string reg_name, unsigned char ip_id, unsigned char func_id);
    void JudgeBufferRegisters(std::string enable_bit_name);
    void StartBufferRegisters();
    void TriggerBufferRegisters();

    // Overwrite functions of Agent Controller class
    bool CheckClockPeriod(void);
    bool GetResetStatus(const std::string reset_name);
};
#endif // __HWCORE_INT_H__
