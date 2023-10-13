// ---------------------------------------------------------------------
// $Id: HWCORE_RSLVIDE.h,v 1.3 2020/10/22 04:06:16 ngathunguyen Exp $
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
// Description: Copyright of HWCORE_RSLVIDE.h
// Ref: {HWCORE_RSLVIDE_UD_Copyright_001, HWCORE_RSLVIDE_UD_ATT1_010, HWCORE_RSLVIDE_UD_ATT1_012}
#ifndef __HWCORE_RSLVIDE_H__
#define __HWCORE_RSLVIDE_H__
#include "OSCI2.h"
#include "global.h"
#include "HWCORE_RSLVIDE_AgentController.h"
#include "CRWInterface.h"
#include "rvc_common_model.h"
#include <vector>

class HWCORE_RSLVIDE_Func;

// Description: Declaration of HWCORE_RSLVIDE class
// Ref: {HWCORE_RSLVIDE_UD_Summary_001, HWCORE_RSLVIDE_UD_ATT1_004, HWCORE_RSLVIDE_UD_ATT1_011, HWCORE_RSLVIDE_UD_ReferenceDoc_001, HWCORE_RSLVIDE_UD_BlockDiagram_001, HWCORE_RSLVIDE_UD_Feature_001, HWCORE_RSLVIDE_UD_Ports_001, HWCORE_RSLVIDE_UD_Ports_002, HWCORE_RSLVIDE_UD_Ports_003, HWCORE_RSLVIDE_UD_Ports_004, HWCORE_RSLVIDE_UD_Ports_005, HWCORE_RSLVIDE_UD_Ports_006, HWCORE_RSLVIDE_UD_Ports_007, HWCORE_RSLVIDE_UD_Ports_008, HWCORE_RSLVIDE_UD_Ports_009, HWCORE_RSLVIDE_UD_Ports_010, HWCORE_RSLVIDE_UD_Ports_011, HWCORE_RSLVIDE_UD_Ports_012, HWCORE_RSLVIDE_UD_Ports_014, HWCORE_RSLVIDE_UD_Ports_015, HWCORE_RSLVIDE_UD_Ports_016, HWCORE_RSLVIDE_UD_Ports_017, HWCORE_RSLVIDE_UD_Ports_018, HWCORE_RSLVIDE_UD_Ports_019, HWCORE_RSLVIDE_UD_Ports_020, HWCORE_RSLVIDE_UD_Ports_021, HWCORE_RSLVIDE_UD_Ports_022, HWCORE_RSLVIDE_UD_Ports_023, HWCORE_RSLVIDE_UD_Ports_024, HWCORE_RSLVIDE_UD_Ports_025, HWCORE_RSLVIDE_UD_Direction_018}
class HWCORE_RSLVIDE : public sc_module
    , public HWCORE_RSLVIDE_AgentController
    , public CRWInterface
    , public rvc_common_model
{
public:
    // Clock/Reset
    sc_in<sc_dt::uint64>        PCLK;                      //clock port
    sc_in<bool>                 PRESETn;                   //reset port
    sc_in<bool>                 mip_prst_n;                //reset port
    
    // Input port from HWCORE_MEASURE
    sc_in<unsigned short>       phi_out_mip_clk;           //Resolver angle data signal
    //Input port connect to EMU3S_HWCORE
    sc_in<unsigned int>         FUNCFLGRPB;                //It contain the value of EMU3nFUNCFLGRPB
    sc_in<unsigned short>       FUNCIDLEGRPB;              //It contain the value of EMU3nFUNCIDLEGRPB
    sc_in<unsigned short>       FUNCWAITGRPB;              //It contain the value of EMU3nFUNCWAITGRPB
    // Input port from HWCORE_RCTG
    sc_in<unsigned short>       em2icmp0;                  //Comparison value software input register for angle compare 0
    sc_in<bool>                 wc_em2icmp0;               //Comparison value software input register for angle compare 0 (write update pulse, H pulse with 1 clock width))
    sc_in<unsigned short>       em2icmp1;                  //Comparison value software input register for angle compare 1
    sc_in<bool>                 wc_em2icmp1;               //Comparison value software input register for angle compare 1 (write update pulse, H pulse of 1 clock width)
    sc_in<unsigned short>       rdata_em2iipcmp0;          //Comparison value IP output register for angle compare 0
    sc_in<bool>                 em2ibrecctr_slctcmp0_reg;  //Angle comparison 0 match detection compare value select
    sc_in<bool>                 output_rctg_end_sp_reg;    //Square wave output processing completion signal
     // Output port
    sc_out<unsigned short>       rslv_func_st;              //Angle generation IP operation status
    sc_out<unsigned short>       rslv_func_end;             //Angle generation IP completion function information
    sc_out<bool>                 rslv_func_wait;            //Outputs "1" only when the angle generation IP FSM status is P_RSLV_IDE_WAIT
    sc_out<bool>                 rslv_func_busy;            //Outputs "1" only when the angle generation IP FSM status is other than P_RSLV_IDE_WAIT and P_RSLV_IDE_IDLE
    sc_out<unsigned char>        ANGCTR;                    //Angle generation IP control register (angle information selection bit)
    sc_out<unsigned short>       RESTHSFT;                  //It contain the value of EMU3nRESTHSFT
    sc_out<unsigned short>       rdata_em2ithtresfix;       //It contain the value of EMU3nTHTRESFIX
    sc_out<unsigned short>       rdata_em2ithtefix;         //It contain the value of EMU3nTHTEFIX
    sc_out<bool>                 presml_ang_comp0_eq_sp;    //Compare 0 match
    sc_out<bool>                 ang_comp1_eq_sp;           //Compare 1 match
    
    // Class HWCORE_RSLVIDE_Func pointer
    HWCORE_RSLVIDE_Func          *mpHWCORE_RSLVIDEFunc;     //pointer of HWCORE_RSLVIDE_Func

    SC_HAS_PROCESS(HWCORE_RSLVIDE);
    //Constructor/Destructor
    HWCORE_RSLVIDE(sc_module_name name);
    ~HWCORE_RSLVIDE(void);

    // Python IF
    void SetMessageLevel(const std::string msg_lv);
    void DumpRegisterRW(const std::string is_enable);
    void EnableRegisterMessage(const std::string is_enable);
    void DumpInterrupt(const std::string is_enable);
    void EnableStatInfo(const std::string is_enable);
    void DumpStatInfo (void);
    void AssertReset(const std::string reset_name, const double start_time, const double period);
    void SetCLKFreq(const std::string clock_name, const sc_dt::uint64 freq, const std::string unit);
    void GetCLKFreq(const std::string clock_name);
    void ForceRegister(const std::string reg_name, const unsigned int reg_value);
    void ReleaseRegister(const std::string reg_name);
    void WriteRegister(const std::string reg_name, const unsigned int reg_value);
    void ReadRegister(const std::string reg_name);
    void ListRegister(void);
    void Help(const std::string type);
    void read (unsigned int offsetAddress, unsigned char* data_ptr, unsigned int size, bool debug);
    void write (unsigned int offsetAddress, unsigned char* data_ptr, unsigned int size, bool debug);
    void ChgProcessingTime (const std::string func_name, const unsigned int value);
private:
    enum eResetInfo{
        emPRESETnId            = 0,
        emMip_prst_nId         = 1,
        emResetSignalNum       = 2
    };
    enum eVARIABLE_VALUE{
        emRESOLVER_CURR_VALUE   = 0,
        emTHTEFIX_PRE_VALUE     = 1
    };
    enum eANG_STATUS {
        emANG_BUSY = 0,
        emANG_IDLE = 1,
        emANG_WAIT = 2    
    };
    enum eANG_FUNC {
        emANG00        = 0,
        emANG01        = 1,
        emANG02        = 2,
        emANG03        = 3,
        emANG_FUNC_NUM = 4
    };
    
    enum eRSLVIDE_OUPUT_PORT_EDGE {
        emRSLV_FUNC_ST           = 0,
        emRSLV_FUNC_END          = 1,
        emRSLV_FUNC_WAIT         = 2,
        emRSLV_FUNC_BUSY         = 3,
        emANGCTR                 = 4,
        emRESTHSFT               = 5,
        emRDATA_EM2ITHTRESFIX    = 6,
        emRDATA_EM2ITHTEFIX      = 7,
        emPRESML_ANG_COMP0_EQ_SP = 8,
        emANG_COMP1_EQ_SP        = 9,
        emOUPUT_PORT_NUM         = 10

    };
    // Variables
    sc_dt::uint64   mPCLKPeriod;                    //Period of PCLK
    sc_dt::uint64   mPCLKOrgFreq;                   //Previous frequency of PCLK
    sc_dt::uint64   mPCLKFreq;                      //Frequency of PCLK
    std::string     mPCLKFreqUnit;                  //Frequency unit of PCLK
    double          mStartPCLKTime;                 // Start time of clock

    sc_dt::uint64   mTimeResolutionValue;           //Time resolution value of simulation
    sc_time_unit    mTimeResolutionUnit;            //Time resolution unit of simulation
    unsigned short  m_output_port_value[emOUPUT_PORT_NUM];
    
    bool            mResetCurVal[emResetSignalNum];         //Store current value of PRESETn, mip_prst_n
    bool            mIsResetHardActive[emResetSignalNum];   //Reset operation status of PRESETn, mip_prst_n signal
    bool            mIsResetCmdReceive[emResetSignalNum];   //Receive status of AssertReset command of PRESETn, mip_prst_n
    bool            mIsResetCmdActive[emResetSignalNum];    //Active status of AssertReset command of PRESETn, mip_prst_n
    double          mResetCmdPeriod[emResetSignalNum];      //Period of AssertReset command of PRESETn, mip_prst_n
   
    bool            mEnableRegisterMessage;         //Enable/disable info/warning/error message of register IF
    bool            mDumpInterrupt;                 //Enable/disable info/warning/error message of register IF
    bool            mEnableStatInfo;                //Dump buffer register
    //--------------------------------------- 
    unsigned char   m_ang_status;
    unsigned int    m_ang_func_cyc[emANG_FUNC_NUM];
    unsigned int    m_ang_ip_cyc_num;
    unsigned int    m_ang_ip_border_cyc;
    unsigned short  m_resolver_ang_pre_value;
    unsigned short  m_resolver_ang_curr_value;
    unsigned short  m_angofs_buff_value;
    unsigned short  m_thtresfix_pre_value;
    unsigned short  m_thtefix_pre_value;
    unsigned char   m_rescnt_pre_value;
    
    bool            m_ang3_comp0match_check_flag;
    bool            m_ang3_comp0_match_flag;
    bool            m_ang3_comp1_match_flag;
    bool            m_comp0_match1;
    bool            m_comp1_match;
    bool            m_wc_em2icmp0_value;
    bool            m_wc_em2icmp1_value;
    bool            m_output_rctg_end_sp_reg_value;
    // Events
    // Reset Events
    sc_event        mResetHardEvent[emResetSignalNum];         //Call HandleResetHardMethod when PRESETn or rst_n is active
    sc_event        mResetCmdEvent[emResetSignalNum];          //Call HandleResetCmdMethod when AssertReset is called
    sc_event        mResetCmdCancelEvent[emResetSignalNum];    //Call CancelResetCmdMethod when AssertReset is over
    //---------------------------------------
    sc_event        m_update_output_port_event[emOUPUT_PORT_NUM];                      
    sc_event        m_ang_add_cyc_event[emANG_FUNC_NUM];                      
    sc_event        m_ang_start_event[emANG_FUNC_NUM];                      
    
    // Methods
    void HandlePCLKSignalMethod(void);
    void HandlePRESETNSignalMethod(void);
    void HandleMip_prst_nSignalMethod(void);

    // Reset handler
    void HandleResetHardMethod(const unsigned int reset_id);
    void HandleResetCmdMethod(const unsigned int reset_id);
    void CancelResetCmdMethod(const unsigned int reset_id);
    
    // Functions
    void InitializeVariable(const unsigned int reset_id);
    void InitializeOutputPort(void);
    void EnableReset(const bool is_active, const unsigned int reset_id);    
    void CancelEvents(const unsigned int id);
    void DumpStatInfo(std::string block, std::string reg_name, int nOpeValue);
    // Overwrite functions of Agent Controller class
    bool GetResetStatus();
    bool CheckClockPeriod();  
    bool ModelinResetStateorZeroClock();
 
    void HandleOutputEdgePortMethod(const unsigned int port_id);
    void HandlePhiOutPortMethod(void);
    void HandleRCTGSignalMethod(void);
    void StartAngleIP(void);
    void HandleFuncwaitgrpbPortMethod(void);
    void CalAngleCycle(unsigned char BLOCK_ID);
    void SetAngleState(unsigned char ANGLE_STATE);
    void SetBuffRegister(void);
    void GeneralAngleFunc (unsigned char ANGLE_FUNC);  
    void ang2(unsigned short rthti, unsigned short rthti_old, unsigned char rescnti, unsigned short ethti_old, unsigned short *ethto, unsigned short *ethto_old, unsigned char  *rescnto);   
    void ang3_comp0 (unsigned short ethti, unsigned short ethti_old, unsigned short comp0i, bool comp0_match_flag, bool *comp0_match1, bool *comp0_match2 );
    void ang3_comp1 (unsigned short rthti, unsigned short rthti_old, unsigned short comp1i, bool comp1_match_flag, bool *comp1_match );
    void CheckNextOperation(unsigned char ANGLE_FUNC);
    //Function is call from HWCORE_RSLVIDE_Func
    void UpdateANGCTRPort(unsigned char value);
    void UpdateRESTHSFTPort(unsigned short value);
    void UpdateTHTEFIXPort(unsigned short value);
    void UpdateTHTRESFIXPort(unsigned short value);
    void UpdateVariableValue(unsigned char var, unsigned short value);
    unsigned short GetResolverAngleValue();
    void OffsetAddition(void);
    void ElectricalAngleCalculation(void);
    void AngleCompareMatch(void);
        
};
#endif // __HWCORE_RSLVIDE_H__
