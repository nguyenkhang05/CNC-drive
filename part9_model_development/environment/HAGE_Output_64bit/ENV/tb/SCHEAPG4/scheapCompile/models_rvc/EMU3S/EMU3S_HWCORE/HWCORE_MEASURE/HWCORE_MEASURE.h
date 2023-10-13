// ---------------------------------------------------------------------
// $Id: HWCORE_MEASURE.h,v 1.6 2020/11/04 09:02:20 phuongtran Exp $
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
// Description: Copyright of HWCORE_MEASURE.h
// Ref: {HWCORE_MEASURE_UD_Copyright_001, HWCORE_MEASURE_UD_ATT1_010, HWCORE_MEASURE_UD_Limitation_002}
#ifndef __HWCORE_MEASURE_H__
#define __HWCORE_MEASURE_H__
#include "global.h"
#include "HWCORE_MEASURE_AgentController.h"
#include "CRWInterface.h"
#include "rvc_common_model.h"
#include <vector>

class HWCORE_MEASURE_Func;

// Description: Declaration of HWCORE_MEASURE class
// Ref: {HWCORE_MEASURE_UD_ATT1_004, HWCORE_MEASURE_UD_ReferenceDoc_001, HWCORE_MEASURE_UD_BlockDiagram_001, HWCORE_MEASURE_UD_Ports_001, HWCORE_MEASURE_UD_Ports_002, HWCORE_MEASURE_UD_Ports_003, HWCORE_MEASURE_UD_Ports_004, HWCORE_MEASURE_UD_Ports_005, HWCORE_MEASURE_UD_Ports_006, HWCORE_MEASURE_UD_Ports_007, HWCORE_MEASURE_UD_Ports_008, HWCORE_MEASURE_UD_Ports_010, HWCORE_MEASURE_UD_Ports_011, HWCORE_MEASURE_UD_Ports_012, HWCORE_MEASURE_UD_Ports_013, HWCORE_MEASURE_UD_Ports_014}
class HWCORE_MEASURE : public sc_module
    , public HWCORE_MEASURE_AgentController
    , public CRWInterface
    , public rvc_common_model
{
public:
    sc_in<sc_dt::uint64>    PCLK;                 //SSCG clock port
    sc_in<sc_dt::uint64>    CCLK;                 //CLEAN clock port
    sc_in<bool>             PRESETn;              //APB reset port
    sc_in<bool>             mip_crst_n;           //CLEAN S/W reset port
    sc_in<bool>             mip_prst_n;           //SSCG S/W reset port
    sc_in<unsigned char>    CTR;                  //EMU3nCTR register
    sc_in<bool>             rdc_z;                //Z signal from RDC (Ch0)
    sc_in<unsigned short>   rdc_phi;              //Resolver angle data from RDC (Ch0) (with the 4 lower-order bits truncated)
    sc_in<unsigned char>    ANGCTR;               //EMU3nANGCTR register
    sc_in<unsigned short>   RESTHSFT;             //EMU3nRESTHSFT register

    sc_out<bool>            pmt2cp;       //Capture interrupt of resolver angle measurement timer
    sc_out<bool>            pmt2of;       //Overflow interrupt of resolver angle measurement timer
    sc_out<unsigned short>  phi_out;      //Resolver angle data

    SC_HAS_PROCESS(HWCORE_MEASURE);
    //Constructor/Destructor
    HWCORE_MEASURE(sc_module_name name);
    ~HWCORE_MEASURE();

    // Python IF
    void SetMessageLevel(const std::string msg_lv);
    void DumpRegisterRW(const std::string is_enable);
    void EnableRegisterMessage(const std::string is_enable);
    void DumpInterrupt (const std::string is_enable);

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

    void read(unsigned int offsetAddress, unsigned char* data_ptr, unsigned int size, bool debug);
    void write(unsigned int offsetAddress, unsigned char* data_ptr, unsigned int size, bool debug);

private:
    // Class HWCORE_MEASURE_Func pointer
    HWCORE_MEASURE_Func            *mpHWCORE_MEASUREFunc;       //pointer of HWCORE_MEASURE_Func

    // Variables
    sc_dt::uint64   mPCLKPeriod;                    //Period of PCLK
    sc_dt::uint64   mPCLKOrgFreq;                   //Previous frequency of PCLK
    sc_dt::uint64   mPCLKFreq;                      //Frequency of PCLK
    std::string     mPCLKFreqUnit;                  //Frequency unit of PCLK
    sc_dt::uint64   mCCLKPeriod;                    //Period of CCLK
    sc_dt::uint64   mCCLKOrgFreq;                   //Previous frequency of CCLK
    sc_dt::uint64   mCCLKFreq;                      //Frequency of CCLK
    std::string     mCCLKFreqUnit;                  //Frequency unit of CCLK

    sc_dt::uint64   mTimeResolutionValue;           //Time resolution value of simulation
    sc_time_unit    mTimeResolutionUnit;            //Time resolution unit of simulation
    double          mStartCCLKTime;                 // Start time of CCLK
    double          mStartPCLKTime;                 // Start time of PCLK

    enum eResetActiveLevel {                        // Define active level of reset signal
        emResetActive = 0
    };
    enum eResetName {                               // Define reset name
        emPRESETn     = 0,
        emMip_crst_n     = 1,
        emMip_prst_n     = 2,
        emCTR         = 3,
        emResetSignalNum = 4
    };

    bool            mResetCurVal[emResetSignalNum];        //Store current value of PRESETn or mip_crst_n
    bool            mIsResetHardActive[emResetSignalNum];  //Reset operation status of PRESETn or mip_crst_n signal
    bool            mIsResetCmdReceive[emResetSignalNum];  //Receive status of AssertReset command of PRESETn or mip_crst_n
    bool            mIsResetCmdActive[emResetSignalNum];   //Active status of AssertReset command of PRESETn or mip_crst_n
    double          mResetCmdPeriod[emResetSignalNum];     //Period of AssertReset command of PRESETn or mip_crst_n
    bool            mEnableRegisterMessage;         //Enable/disable info/warning/error message of register IF
    bool            mDumpInterrupt;                 //Enable/Disable dumping the interrupt info

    double          pmt2_last_proc_time;
    double          pmt2_stop_time;
    unsigned short  pmt2_invl_div[17];
    unsigned short  pmt2_past_angle;
    bool            g_capture_rotation_pos;
    unsigned long   g_pmt2_compare_cnt;
    unsigned short  res_value_temp;
    unsigned short  res_value_temp_old;

    bool            mPmt2cp_int;                    // Value of pmt2cp interrupt
    bool            mPmt2of_int;                    // Value of pmt2of interrupt
    unsigned short  mPhi_out;                       // Value of phi_out data

    // Events
    sc_event        mResetHardEvent[emResetSignalNum];         //Call HandleResetHardMethod when PRESETn or mip_crst_n is active
    sc_event        mResetCmdEvent[emResetSignalNum];          //Call HandleResetCmdMethod when AssertReset is called
    sc_event        mResetCmdCancelEvent[emResetSignalNum];    //Call CancelResetCmdMethod when AssertReset is over

    sc_event        m_pmt2_over_flowEvent;          // Call pmt2_over_flowMethod
    sc_event        mAssertPmt2cp_intCLEANEvent;    // Call AssertPmt2cp_intCLEANMethod() to assert pmt2cp interrupt sync with CLEAN clock
    sc_event        mAssertPmt2cp_intSSCGEvent;     // Call AssertPmt2cp_intSSCGMethod() to assert pmt2cp interrupt sync with SSCG clock
    sc_event        mDeassertPmt2cp_intEvent;       // Call DeassertPmt2cp_intMethod() to de-assert pmt2cp interrupt
    sc_event        mWritePmt2cp_intEvent;          // Call WritePmt2cp_intMethod() to control pmt2cp port
    sc_event        mAssertPmt2of_intCLEANEvent;    // Call AssertPmt2of_intCLEANMethod() to assert pmt2of interrupt sync with CLEAN clock
    sc_event        mAssertPmt2of_intSSCGEvent;     // Call AssertPmt2of_intSSCGMethod() to assert pmt2of interrupt sync with SSCG clock
    sc_event        mDeassertPmt2of_intEvent;       // Call DeassertPmt2of_intMethod() to de-assert pmt2of interrupt
    sc_event        mWritePmt2of_intEvent;          // Call WritePmt2of_intMethod() to control pmt2of port
    sc_event        mWritePhi_outCLEANEvent;        // Call WritePhi_outCLEANMethod() to control phi_out port sync with CLEAN clock
    sc_event        mWritePhi_outSSCGEvent;         // Call WritePhi_outSSCGMethod() to control phi_out port sync with SSCG clock

    enum MAX_VALUE
    {
        PMT2_CNT_MAX = 0xFFFFFFFF
    };

    // Threads/Methods
    void HandlePCLKSignalMethod(void);
    void HandleCCLKSignalMethod(void);
    void HandlePRESETnSignalMethod(void);
    void HandleMip_crst_nSignalMethod(void);
    void HandleMip_prst_nSignalMethod(void);
    void HandleCTRSignalMethod(void);
    void HandleResetHardMethod(const unsigned int reset_id);
    void HandleResetCmdMethod(const unsigned int reset_id);
    void CancelResetCmdMethod(const unsigned int reset_id);

    // Functions
    void EnableReset(const std::string reset_name, const bool is_active);
    void Initialize_CLEAN(void);
    void Initialize_SSCG(void);
    void Initialize_OutputPort(void);
    void CancelEvents_CLEAN(void);
    void CancelEvents_SSCG(void);

    // Overwrite functions of Agent Controller class
    bool CheckClockPeriod(const std::string clock_name);
    bool GetResetStatus(const std::string domain);
    void rdc_phi_procMethod(void);
    void rdc_z_pos_procMethod(void);
    void RESTHSFT_procMethod(void);
    void pmt2_over_flowMethod(void);
    void pmt2_capture(void);
    void judge_pmt2_capture(unsigned short now_res_angle);
    void judge_pmt2_capture_pre(unsigned short res_value);
    void NextOverFlow(double pmt2cnt);
    void UpdatePhi_outVal(void);
    void AssertPmt2cp_intCLEANMethod(void);
    void AssertPmt2cp_intSSCGMethod(void);
    void DeassertPmt2cp_intMethod(void);
    void WritePmt2cp_intMethod(void);
    void AssertPmt2of_intCLEANMethod(void);
    void AssertPmt2of_intSSCGMethod(void);
    void DeassertPmt2of_intMethod(void);
    void WritePmt2of_intMethod(void);
    void WritePhi_outCLEANMethod(void);
    void WritePhi_outSSCGMethod(void);
    void callback_PMT2CTR_EN(void);
    void callback_PMT2SFT_SCAPTRG(void);
    void callback_PMT2CNT_DATA_write(void);
    void callback_PMT2CNT_DATA_read(void);
};
#endif // __HWCORE_MEASURE_H__
