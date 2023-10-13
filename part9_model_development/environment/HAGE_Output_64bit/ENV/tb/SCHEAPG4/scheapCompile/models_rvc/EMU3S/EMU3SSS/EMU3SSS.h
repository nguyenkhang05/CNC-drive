// ---------------------------------------------------------------------
// $Id: EMU3SSS.h,v 1.10 2020/11/20 17:00:28 thiepnguyen Exp $
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
// Description: Copyright of EMU3SSS.h
// Ref: {EMU3SSS_UD_Copyright_001}
#ifndef __EMU3SSS_H__
#define __EMU3SSS_H__

// Description: Rule about the include header files.
// Ref: {EMU3SSS_UD_ATT1_001, EMU3SSS_UD_ATT1_009, EMU3SSS_UD_ATT1_010, EMU3SSS_UD_Direction_001}

#include "OSCI2.h"
#include "global.h"
#include "rvc_common_model.h"
#include <iostream>
#include <cstring>
#include <iterator>
#include <map>
#include <stdarg.h>

#include "EMU3S_AHB_ARB.h"
#include "EMU3S_APB_DEC.h"
#include "EMU3S_HWCORE.h"

class EMU3SSS_Func;
class EMU3S_AHB_ARB;
class EMU3S_APB_DEC;
class EMU3S_HWCORE;

// Description: Declaration of EMU3SSS class
// Ref: {EMU3SSS_UD_ReferenceDoc_001, EMU3SSS_UD_Direction_002}
class EMU3SSS : public sc_module
    , public rvc_common_model
{
#include "EMU3SSS_cmdif.h"
public:

    SC_HAS_PROCESS(EMU3SSS);
    //Constructor/Destructor
    EMU3SSS(sc_module_name name, uint32_t rLatency, uint32_t wLatency, SIM_MODE_T simmode);
    ~EMU3SSS(void);

    // Description: PFSS interface
    // Ref: {EMU3SSS_UD_Flow_009}
    TlmTargetSocket<BUS_WIDTH_APB>    tsp_cpu; // target socket which connected to PFSS

    // Description: Memmory interface
    // Ref: {EMU3SSS_UD_Feature_008}
    TlmInitiatorSocket<BUS_WIDTH_AHB> ish;   // initiator socket which connected to PFSS


    // Description: Clock and resetinterface
    // Ref: {EMU3SSS_UD_Feature_002, EMU3SSS_UD_Feature_003}
    sc_in<sc_dt::uint64>     PCLK;
    sc_in<sc_dt::uint64>     CCLK;
    sc_in<bool>              PRESETn;
    sc_in<bool>              CRESETn;

    // Description: Interrupt interface
    // Ref: {EMU3SSS_UD_Feature_004, EMU3SSS_UD_Feature_003, EMU3SSS_UD_Feature_007}
    sc_out<bool>             *emu_int[8];

    // Description: Input and Output feature
    // Ref: {EMU3SSS_UD_Feature_004, EMU3SSS_UD_Feature_005}
    // TSG3 interface
    sc_in<bool>              tstpektim;
    sc_in<bool>              tstvlytim;
    sc_in<bool>              tstadt0;
    sc_in<bool>              tstadt1;
    sc_in<bool>              tstint3;
    sc_in<bool>              tstint4;
    sc_out<sc_uint<18> >     tstwdcmp0;
    sc_out<sc_uint<18> >     tstwdcmpu;
    sc_out<sc_uint<18> >     tstwdcmpv;
    sc_out<sc_uint<18> >     tstwdcmpw;
    sc_out<bool>             tstwecmp;
    sc_out<bool>             tstiup;
    sc_out<bool>             tstivp;
    sc_out<bool>             tstiwp;
    
    // ADC interface
    sc_out<bool>             adc_start;
    sc_in<bool>              adc_grpend;
    sc_in<bool>              adc_ch0end;
    sc_in<sc_uint<16> >      adc_ch0dat;
    sc_in<bool>              adc_ch1end;
    sc_in<sc_uint<16> >      adc_ch1dat;
    sc_in<bool>              adc_ch2end;
    sc_in<sc_uint<16> >      adc_ch2dat;
    sc_out<bool>             adc1_start;
    sc_in<bool>              adc1_grpend;
    sc_in<bool>              adc1_ch0end;
    sc_in<sc_uint<16> >      adc1_ch0dat;
    sc_in<bool>              adc1_ch1end;
    sc_in<sc_uint<16> >      adc1_ch1dat;
    sc_in<bool>              adc1_ch2end;
    sc_in<sc_uint<16> >      adc1_ch2dat;

    // RDC interface
    sc_in<sc_uint<16> >      rdc_phi;
    sc_in<bool>              rdc_z;
    sc_in<sc_uint<26> >      rdc_high_omega;
    sc_in<sc_uint<16> >      rdc1_phi;
    sc_in<bool>              rdc1_z;
    sc_in<sc_uint<26> >      rdc1_high_omega;

    // Description: System control i/f
    // Ref: {EMU3SSS_UD_Feature_010}
    sc_in<bool>              emu3_stop_req;
    sc_out<bool>             emu3_stop_ack;

    // Description: Input and Output feature
    // Ref: {EMU3SSS_UD_Feature_009, EMU3SSS_UD_Feature_010}
    sc_out<bool>             EMU3_LCSTERR_P;
    sc_out<bool>             EMU3_LCSTERR_C;
    sc_out<bool>             EMU3_EDCERR_P;
    sc_out<bool>             EMU3_EDCERR_C;

    //Functions support PY_COMMAND
    void SetMessageLevel(const std::string msg_lv, std::string sub_ip_name);
    void DumpRegisterRW(const std::string is_enable, std::string sub_ip_name);
    void EnableRegisterMessage(const std::string is_enable, std::string sub_ip_name);
    void DumpInterrupt(const std::string is_enable, std::string sub_ip_name);
    void EnableStatInfo(const std::string is_enable, std::string sub_ip_name);
    void AssertReset(const std::string reset_name, const double start_time, const double period, std::string sub_ip_name);
    void SetCLKFreq(const std::string clock_name, const sc_dt::uint64 freq, const std::string unit, std::string sub_ip_name);
    void GetCLKFreq(const std::string clock_name, std::string sub_ip_name);
    void ForceRegister(std::string reg_name, unsigned int value, std::string sub_ip_name);
    void ReleaseRegister(std::string reg_name, std::string sub_ip_name);
    void WriteRegister(std::string reg_name, unsigned int value, std::string sub_ip_name);
    void ReadRegister(std::string reg_name, std::string sub_ip_name);
    void ListRegister(std::string sub_ip_name);
    void DumpStatInfo(const std::string sub_ip_name);
    void ChgProcessingTime(const std::string sub_ip_name, std::string func_name, unsigned int value);
    void SetAHBExtension(unsigned int value);
    void SetFUSAError(unsigned char domain, unsigned char error_type, unsigned char status);
    void Help(const std::string type);

private:

    // SFERC register's value
    sc_signal<unsigned char>    sferc_sig;

    sc_signal<bool>    emu3_stop_req_sig;
    sc_signal<bool>    emu3_stop_ack_ahbarb_sig;
    sc_signal<bool>    emu3_stop_ack_apbdec_sig;
    sc_signal<bool>    edcerr_sed_pclk_sig;
    sc_signal<bool>    edcerr_ded_pclk_sig;
    sc_signal<bool>    edcerr_sed_cclk_sig;
    sc_signal<bool>    edcerr_ded_cclk_sig;

    sc_signal<sc_dt::uint64>    PCLK_sig;
    sc_signal<sc_dt::uint64>    CCLK_sig;
    sc_signal<bool>             PRESETn_sig;
    sc_signal<bool>             CRESETn_sig;
    
    // Description: Declaration of objects
    // Ref: {EMU3SSS_UD_ATT1_006, EMU3SSS_UD_ATT1_007, EMU3SSS_UD_ATT1_008}
    EMU3S_HWCORE    *mEMU3S_HWCORE;
    EMU3S_AHB_ARB   *mEMU3S_AHB_ARB;
    EMU3S_APB_DEC   *mEMU3S_APB_DEC;

    // Declare events which support assert reset from python commands                            
    sc_event        mPRESETnCmdEvent;             //Call HandlePRESETnCmdMethod when AssertReset of PRESETn is called
    sc_event        mPRESETnCmdCancelEvent;       //Call CancelPRESETnCmdMethod when AssertReset of PRESETn is over
    sc_event        mCRESETnCmdEvent;             //Call HandleCRESETnCmdMethod when AssertReset of CRESETn is called
    sc_event        mCRESETnCmdCancelEvent;       //Call CancelCRESETnCmdMethod when AssertReset of CRESETn is over
    sc_event        mPRESETnResetEvent;
    sc_event        mCRESETnResetEvent;

    sc_event        mWrite_EDCERR_P_Event;
    sc_event        mAssert_EDCERR_P_Event;
    sc_event        mDeassert_EDCERR_P_Event;
    sc_event        mWrite_EDCERR_C_Event;
    sc_event        mAssert_EDCERR_C_Event;
    sc_event        mDeassert_EDCERR_C_Event;
    sc_event        mWrite_LCSTERR_P_Event;
    sc_event        mAssert_LCSTERR_P_Event;
    sc_event        mDeassert_LCSTERR_P_Event;
    sc_event        mWrite_LCSTERR_C_Event;
    sc_event        mAssert_LCSTERR_C_Event;
    sc_event        mDeassert_LCSTERR_C_Event;
    sc_event        mWrite_STOP_ACK_Event;

    bool            mIsPRESETnActive;         //Store current value of PRESETn
    bool            mIsPRESETnHardActive;   //Reset operation status of PRESETn
    bool            mIsPRESETnCmdReceive;   //Receive status of AssertReset command of PRESETn
    bool            mIsPRESETnCmdActive;    //Active status of AssertReset command of PRESETn
    double          mPRESETnCmdPeriod;      //Period of AssertReset command of PRESETn
                   
    bool            mIsCRESETnActive;         //Store current value of CRESETn
    bool            mIsCRESETnHardActive;   //Reset operation status of CRESETn
    bool            mIsCRESETnCmdReceive;   //Receive status of AssertReset command of CRESETn
    bool            mIsCRESETnCmdActive;    //Active status of AssertReset command of CRESETn
    double          mCRESETnCmdPeriod;      //Period of AssertReset command of CRESETn

    bool            mEDCERR_P;
    bool            mEDCERR_C;
    bool            mLCSTERR_P;
    bool            mLCSTERR_C;
    bool            memu3_stop_ack;

    // Variables
    sc_dt::uint64   mPCLKPeriod;                    //Period of PCLK
    sc_dt::uint64   mPCLKOrgFreq;                   //Previous frequency of PCLK
    sc_dt::uint64   mPCLKFreq;                      //Frequency of PCLK
    std::string     mPCLKFreqUnit;                  //Frequency unit of PCLK
    double          mStartPCLKTime;                 //Start time of clock

    sc_dt::uint64   mCCLKPeriod;                    //Period of CCLK
    sc_dt::uint64   mCCLKOrgFreq;                   //Previous frequency of CCLK
    sc_dt::uint64   mCCLKFreq;                      //Frequency of CCLK
    std::string     mCCLKFreqUnit;                  //Frequency unit of CCLK
    double          mStartCCLKTime;                 //Start time of clock

    sc_dt::uint64   mTimeResolutionValue;           //Time resolution value of simulation
    sc_time_unit    mTimeResolutionUnit;            //Time resolution unit of simulation

    double          mTimePCLKChangePoint;           //The time PCLK clock is changed its value
    double          mTimeCCLKChangePoint;           //The time CCLK clock is changed its value
    bool            mDumpInterrupt;

    enum emInterrupt
    {
        emINTERRUPT_NUM = 8
    };

    enum eRESET_CONTROL{
        emINITIALIZE = 0,
        emRESET_CLEAN,
        emRESET_SSCG,
    };
    void start_of_simulation(void);
    void Initialize(eRESET_CONTROL emType);
    void CreateConnections(void);
    void MonitorPCLKMethod(void);
    void MonitorCCLKMethod(void);

    void MonitorPRESETnMethod(void);
    void MonitorCRESETnMethod(void);
    
    void HandlePRESETnCmdMethod(void);
    void HandleCRESETnCmdMethod(void);
    void CancelPRESETnCmdMethod(void);
    void CancelCRESETnCmdMethod(void);
   
    void HandlePRESETnMethod(void);
    void HandleCRESETnMethod(void);
    void DumpInterruptMsg (const std::string intr_name, const bool value);
    void DumpIntTop(const std::string is_enable);

    void Write_EDCERR_P_Method(void);
    void Assert_EDCERR_P_Method(void);
    void Deassert_EDCERR_P_Method(void);
    void Write_EDCERR_C_Method(void);
    void Assert_EDCERR_C_Method(void);
    void Deassert_EDCERR_C_Method(void);
    void Write_LCSTERR_P_Method(void);
    void Assert_LCSTERR_P_Method(void);
    void Deassert_LCSTERR_P_Method(void);
    void Write_LCSTERR_C_Method(void);
    void Assert_LCSTERR_C_Method(void);
    void Deassert_LCSTERR_C_Method(void);
    void Monitor_STOP_ACK_Method(void);
    void Monitor_STOP_REQ_Method(void);
    void Handle_STOP_ACK_Method(void);
    void Handle_EDC_P_Error_Method(void);
    void Handle_EDC_C_Error_Method(void);

    bool CheckClockPeriod(const std::string clock_name);
    bool GetResetStatus(const std::string reset_name);
};
#endif //__EMU3SSS_H__

