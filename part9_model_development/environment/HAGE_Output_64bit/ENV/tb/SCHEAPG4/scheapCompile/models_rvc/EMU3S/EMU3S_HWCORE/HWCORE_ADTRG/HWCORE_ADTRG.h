// ---------------------------------------------------------------------
// $Id: HWCORE_ADTRG.h,v 1.7 2020/10/20 06:41:31 huepham Exp $
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
// Description: Copyright of HWCORE_ADTRG.h
// Ref: {HWCORE_ADTRG_UD_Copyright_001, HWCORE_ADTRG_UD_ATT1_010}
#ifndef __HWCORE_ADTRG_H__
#define __HWCORE_ADTRG_H__
#include "OSCI2.h"
#include "global.h"
#include "HWCORE_ADTRG_AgentController.h"
#include "CRWInterface.h"
#include "rvc_common_model.h"
#include <vector>

class HWCORE_ADTRG_Func;

// Description: Declaration of HWCORE_ADTRG class
// Ref: {HWCORE_ADTRG_UD_Feature_001, HWCORE_ADTRG_UD_ATT1_004, HWCORE_ADTRG_UD_ReferenceDoc_001, HWCORE_ADTRG_UD_BlockDiagram_001, HWCORE_ADTRG_UD_Ports_001, HWCORE_ADTRG_UD_Ports_002, HWCORE_ADTRG_UD_Ports_003, HWCORE_ADTRG_UD_Ports_004, HWCORE_ADTRG_UD_Ports_005, HWCORE_ADTRG_UD_Ports_006, HWCORE_ADTRG_UD_Ports_007, HWCORE_ADTRG_UD_Ports_008, HWCORE_ADTRG_UD_Ports_009, HWCORE_ADTRG_UD_Ports_010, HWCORE_ADTRG_UD_Ports_011, HWCORE_ADTRG_UD_Ports_012, HWCORE_ADTRG_UD_Ports_013, HWCORE_ADTRG_UD_Ports_014, HWCORE_ADTRG_UD_Ports_015, HWCORE_ADTRG_UD_Ports_016, HWCORE_ADTRG_UD_Ports_017, HWCORE_ADTRG_UD_Ports_018, HWCORE_ADTRG_UD_Ports_019, HWCORE_ADTRG_UD_Ports_020, HWCORE_ADTRG_UD_Ports_021, HWCORE_ADTRG_UD_Ports_022, HWCORE_ADTRG_UD_Ports_023, HWCORE_ADTRG_UD_Ports_024, HWCORE_ADTRG_UD_Ports_025, HWCORE_ADTRG_UD_Ports_026, HWCORE_ADTRG_UD_Ports_027, HWCORE_ADTRG_UD_Ports_028, HWCORE_ADTRG_UD_Ports_029, HWCORE_ADTRG_UD_Ports_030, HWCORE_ADTRG_UD_Ports_031, HWCORE_ADTRG_UD_Ports_032, HWCORE_ADTRG_UD_Ports_033, HWCORE_ADTRG_UD_Ports_034, HWCORE_ADTRG_UD_Ports_035, HWCORE_ADTRG_UD_Ports_036, HWCORE_ADTRG_UD_Ports_037, HWCORE_ADTRG_UD_Ports_038, HWCORE_ADTRG_UD_Ports_039, HWCORE_ADTRG_UD_Ports_040, HWCORE_ADTRG_UD_Ports_041, HWCORE_ADTRG_UD_Ports_042, HWCORE_ADTRG_UD_Ports_043, HWCORE_ADTRG_UD_Ports_044, HWCORE_ADTRG_UD_Ports_045, HWCORE_ADTRG_UD_Ports_046, HWCORE_ADTRG_UD_Ports_047, HWCORE_ADTRG_UD_Ports_048, HWCORE_ADTRG_UD_Ports_049, HWCORE_ADTRG_UD_Ports_050, HWCORE_ADTRG_UD_Ports_051, HWCORE_ADTRG_UD_Ports_052, HWCORE_ADTRG_UD_Ports_053, HWCORE_ADTRG_UD_Ports_054, HWCORE_ADTRG_UD_Limitation_001}
class HWCORE_ADTRG : public sc_module
    , public HWCORE_ADTRG_AgentController
    , public CRWInterface
    , public BusMasterBase<BUS_WIDTH_AHB, 1>
    , public rvc_common_model
{
public:
    // Clock/Reset
    sc_in<sc_dt::uint64>    PCLK;                 //clock port
    sc_in<sc_dt::uint64>    CCLK;                 //clock port
    sc_in<bool>             PRESETn;              //reset port
    sc_in<bool>             crst_n;               //reset port
    // Input port from HWCORE_RSLVIDE
    sc_in<unsigned short>     theta_res_fix_reg;    //Resolver angle data signal
    sc_in<unsigned short>     theta_e_fix_reg;      //Electrical angle data signal
    sc_in<bool>             ang_comp0_eq_sp;      //Angle compare 0 match notification signal
    // Input port connect to HWCORE_IRCTG3
    sc_in<bool>             irctg3_compmatch_u;   //Independent rectangle I P 3 U-phase compare match notification signal
    sc_in<bool>             irctg3_compmatch_v;   //Independent rectangle I P 3 V-phase compare match notification signal
    sc_in<bool>             irctg3_compmatch_w;   //Independent rectangle I P 3 W-phase compare match notification signal
    //Input port connect to EMU3S_Input IP
    sc_in<unsigned char>    em2ictrinmd_instctr;  //It contains EMU3nCTRINMD.INSTCTR 
    //Input port connect to EMU3S_HWCORE
    sc_in<bool>             emu3ictr_emust;        //EMU3S_HWCORE(CTR)
    sc_in<bool>             em3iadsel;            //It contain the value of EMU3nADRDCSL.ADSEL
    sc_in<unsigned char>    emu3iptrgctr;         //It contain the value of EMU3nIPTRGCTR.TNEN bit
    sc_in<unsigned int>     emu3iptrgnum;         //It contain the value of EMU3nIPTRGNUM.ADTRGNUM bit
    sc_in<bool>             adc_grpend;           //A /D group completion notification signal (ADC0 )
    sc_in<bool>             adc_ch0end;          //A /D channel 0 completion notification signal (ADC0 )
    sc_in<bool>             adc_ch1end;          //A /D channel 1 completion notification signal (ADC0 )
    sc_in<bool>             adc_ch2end;          //A /D channel 2 completion notification signal (ADC0 )
    sc_in<unsigned short>   adc_ch0_dat;          //ADC CH0 input data (ADC0)
    sc_in<unsigned short>   adc_ch1_dat;          //ADC CH1 input data (ADC1)
    sc_in<unsigned short>   adc_ch2_dat;          //ADC CH2 input data (ADC2)
    sc_in<bool>             tstpektim;            //Carrier mountain trigger signal
    sc_in<bool>             tstvlytim;            //Carrier valley trigger signal
    sc_in<bool>             tstadt0;              //Carrier valley trigger signal (TSTAD0 terminal)
    sc_in<bool>             tstadt1;              //Carrier valley trigger signal (TSTAD1 terminal)
    sc_in<bool>             tstint3;              //Interrupt signal (TSTINT 3 pin)
    sc_in<bool>             tstint4;              //Interrupt signal (TSTINT 4 pin)
    sc_in<unsigned int>     emu3tbladr;           //The start address of memory area 0
    sc_in<unsigned char>    ahbprt;               //AHB protection control
    //Input port connect to EMU3S_IIR
    sc_in<signed int>       iiroutdat0;           //IIR Filter conversion result channel 0
    sc_in<signed int>       iiroutdat1;           //IIR Filter conversion result channel 1
    sc_in<signed int>       iiroutdat2;           //IIR Filter conversion result channel 2

    //Output port connect to EMU3S_HWCORE
    sc_out<bool>            adc_start;
    sc_out<unsigned char>   em3iadctr_adend;
    sc_out<unsigned char>   emu3_iptrgmon_adtrgnum;
    //Output port connect to HWCORE_IIR Filter
    sc_out<unsigned short>  rdata_em3iad0;
    sc_out<unsigned short>  rdata_em3iad1;
    sc_out<unsigned short>  rdata_em3iad2;
    //Output port connect to HWCORE_INT
    sc_out<bool>            adtrg_end_trig;
    sc_out<bool>            ad_end_trig_sp;
    sc_out<bool>            adtrg_mvint0;
    sc_out<bool>            adtrg_mvint1;
    sc_out<bool>            adtrg_mem_rg_chg;
    sc_out<bool>            adtrg_smerr;
    sc_out<bool>            adtrg_amerr;
    sc_out<bool>            ad_start_mou_sp_80m;
    sc_out<bool>            ad_start_vall_sp_80m;
    //Output port connect to HWCORE_Input IP
    sc_out<unsigned short>  adtrg_ch0_dat;
    sc_out<unsigned short>  adtrg_ch1_dat;
    sc_out<unsigned short>  adtrg_ch2_dat;
    sc_out<unsigned short>  adtrg_thte;
    sc_out<unsigned short>  adtrg_thtr;
     
     
    // Socket
    TlmInitiatorSocket<BUS_WIDTH_AHB>  *is;                       //target socket to access register

    // Class HWCORE_ADTRG_Func pointer
    HWCORE_ADTRG_Func                  *mpHWCORE_ADTRGFunc;       //pointer of HWCORE_ADTRG_Func

    SC_HAS_PROCESS(HWCORE_ADTRG);
    //Constructor/Destructor
    HWCORE_ADTRG(sc_module_name name, SIM_MODE_T simmode);
    ~HWCORE_ADTRG(void);

    // Python IF
    void SetMessageLevel(const std::string msg_lv);
    void DumpRegisterRW(const std::string is_enable);
    void EnableRegisterMessage(const std::string is_enable);
    void DumpInterrupt(const std::string is_enable);

    void AssertReset(const std::string reset_name, const double start_time, const double period);
    void SetCLKFreq(const std::string clock_name, const sc_dt::uint64 freq, const std::string unit);
    void GetCLKFreq(const std::string clock_name);
    void ForceRegister(const std::string reg_name, const unsigned int reg_value);
    void ReleaseRegister(const std::string reg_name);
    void WriteRegister(const std::string reg_name, const unsigned int reg_value);
    void ReadRegister(const std::string reg_name);
    void SetAHBExtension(unsigned int extension);
    void ListRegister(void);
    void Help(const std::string type);
    void read (unsigned int offsetAddress, unsigned char* data_ptr, unsigned int size, bool debug);
    void write (unsigned int offsetAddress, unsigned char* data_ptr, unsigned int size, bool debug);
    
private:
    // Payload infomation
    unsigned int *mpData;
    unsigned int m_Data0;
    unsigned int m_Data1;
    unsigned int m_pAddr;
    // Memory write number
    unsigned short m_mnnum;
    
    TlmBasicPayload            *mAHBTrans;
    TlmG3mExtension            *mAHBTransG3Ext;
    TlmAhbExtension            *mAHBTransExt;

    BusMasterIf<BUS_WIDTH_AHB> *mAhbMasterIf;
    
    enum eResetInfo{
        emPRESETnId            = 0,
        emcrst_nId             = 1,
        emctr_emustId          = 2,
        emResetSignalNum       = 3
    };
    enum eTRG_SOURCE {
        emTSTADT0_ATT     = 0,
        emTSTADT1_ATT     = 1,
        emTSTPEKTIM_ATT   = 2,
        emTSTVLYTIM_ATT   = 3,
        emANG_COMP0       = 4,
        emSFTAD           = 5,
        emIR3U            = 5,
        emIR3V            = 6,
        emIR3W            = 7,
        emAD_SAMP_PEAK    = 8,
        emAD_SAMP_TROUGH  = 9
    };
    enum eADC_STATUS {
        emAD_BUSY = 0,
        emAD_IDLE = 1
    };
    enum eADTRG_OUPUT_PORT_EDGE {
        emADC_START_PORT            = 0,
        emADTRG_END_TRIG_PORT       = 1,
        emAD_END_TRIG_SP_PORT       = 2,
        emADTRG_MVINT0_PORT         = 3,
        emADTRG_MVINT1_PORT         = 4,
        emADTRG_MEM_RG_CHG_PORT     = 5,
        emADTRG_SMERR_PORT          = 6,
        emADTRG_AMERR_PORT          = 7,
        emAD_START_MOU_SP_80M_PORT  = 8,
        emAD_START_VALL_SP_80M_PORT = 9,
        emOUPUT_PORT_EDGE_NUM       = 10,
    };
    enum eADTRG_OUPUT_PORT_DATA {
        emEM3IADCTR_ADEND_PORT = 0,
        emEMU3_IPTRGMON_ADTRGNUM_PORT = 1,
        emRDATA_EM3IAD0_PORT = 2,
        emRDATA_EM3IAD1_PORT = 3,
        emRDATA_EM3IAD2_PORT = 4,
        emADTRG_CH0_DAT_PORT = 5,
        emADTRG_CH1_DAT_PORT = 6,
        emADTRG_CH2_DAT_PORT = 7,
        emADTRG_THTE_PORT = 8,
        emADTRG_THTR_PORT = 9,
        emOUPUT_PORT_DATA_NUM = 10
    };
    
    enum eMASK_CONTROL {
        emAD_START_TRIG = 0,
        emAD_COMP_TRIG  = 1,
        emANG_START_TRG = 3
    };
    
    enum eRESET_CONTROL{
        emINITIALIZE = 0,
        emRESET      = 1
    };
    
    // Variables
    sc_dt::uint64   mPCLKPeriod;                    //Period of PCLK
    sc_dt::uint64   mPCLKOrgFreq;                   //Previous frequency of PCLK
    sc_dt::uint64   mPCLKFreq;                      //Frequency of PCLK
    std::string     mPCLKFreqUnit;                  //Frequency unit of PCLK
    double          mStartPCLKTime;                 // Start time of clock

    sc_dt::uint64   mCCLKPeriod;                    //Period of CCLK
    sc_dt::uint64   mCCLKOrgFreq;                   //Previous frequency of CCLK
    sc_dt::uint64   mCCLKFreq;                      //Frequency of CCLK
    std::string     mCCLKFreqUnit;                  //Frequency unit of CCLK
    double          mStartCCLKTime;                 // Start time of clock

    sc_dt::uint64   mTimeResolutionValue;           //Time resolution value of simulation
    sc_time_unit    mTimeResolutionUnit;            //Time resolution unit of simulation
    SIM_MODE_T      mSimMode;
    
    bool            mResetCurVal[emResetSignalNum];         //Store current value of PRESETn, crst_n, emu3ictr_emust

    bool            mIsResetHardActive[emResetSignalNum];   //Reset operation status of PRESETn, crst_n, emu3ictr_emust signal
    bool            mIsResetCmdReceive[emResetSignalNum];   //Receive status of AssertReset command of PRESETn, crst_n, emu3ictr_emust
    bool            mIsResetCmdActive[emResetSignalNum];    //Active status of AssertReset command of PRESETn crst_n, emu3ictr_emust
    double          mResetCmdPeriod[emResetSignalNum];      //Period of AssertReset command of PRESETn, rst_n, emu3ictr_emust
   
    bool            mEnableRegisterMessage;         //Enable/disable info/warning/error message of register IF
    bool            mDumpInterrupt;                 //Enable/disable info/warning/error message of register IF
    //--------------------------------------- 
    unsigned int    m_adtrg_att;
    unsigned int    m_ad_trig_count;
    double          m_ad_start_trg_delay;
    double          m_ang_start_trg_delay;
    bool            m_adc_status;
    bool            m_output_port_edge_value[emOUPUT_PORT_EDGE_NUM];
    unsigned short  m_output_port_data_value[emOUPUT_PORT_DATA_NUM];
    unsigned short  m_emu3_iptrgmon_adtrgnum_value;
    unsigned char   m_adtrg_mem_write_time;
    bool            m_em3iadsel_pre_val;
    bool            m_switching_area;
    unsigned int    mSPID;
    bool            m_ADTRG0;
    bool            m_ADTRG1;
    bool            m_adtrg_in_delay_period;
    
    unsigned char   m_adtrg0_skip_count;
    unsigned char   m_adtrg1_skip_count;
    unsigned char   m_ang_samp_mou_lat_count;
    unsigned char   m_ang_samp_vall_lat_count;
    unsigned char   m_ad_samp_mou_count;
    unsigned char   m_ad_samp_vall_count;

    // Events
    // Reset Events
    sc_event        mResetHardEvent[emResetSignalNum];         //Call HandleResetHardMethod when PRESETn or rst_n is active
    sc_event        mResetCmdEvent[emResetSignalNum];          //Call HandleResetCmdMethod when AssertReset is called
    sc_event        mResetCmdCancelEvent[emResetSignalNum];    //Call CancelResetCmdMethod when AssertReset is over
    //---------------------------------------
    sc_event        m_update_output_edge_event[emOUPUT_PORT_EDGE_NUM];
    sc_event        m_update_output_data_event[emOUPUT_PORT_DATA_NUM];
    sc_event_queue  m_rd_trg_event;
    sc_event_queue  m_ad_start_trg_event;
    sc_event_queue  m_ad_sampling_mou_trg_event_queue;
    sc_event_queue  m_ad_sampling_vall_trg_event_queue;
    sc_event        m_emu3_iptrgmon_adtrgnum_event;
    
    sc_event        m_adc_completion_event;
    sc_event        m_adc_data_event;
    sc_event        m_ang_latch_event;
    
    sc_event        m_issue_write;
    sc_event        m_trans_comp_ca;
    sc_event        m_abnormal_event;
    // Methods
    void HandlePCLKSignalMethod(void);
    void HandleCCLKSignalMethod(void);
    void HandlePRESETNSignalMethod(void);
    void HandleCRSTNSignalMethod(void);
    void HandleEMUSTSignalMethod(void);

    // Reset handler
    void HandleResetHardMethod(const unsigned int reset_id);
    void HandleResetCmdMethod(const unsigned int reset_id);
    void CancelResetCmdMethod(const unsigned int reset_id);
    
    // Functions
    void InitializeSSCG(eRESET_CONTROL emType);
    void InitializeCLEAN(eRESET_CONTROL emType);
    void EnableReset(const bool is_active, const unsigned int reset_id);
    
    void CancelEventsSSCG(void);
    void CancelEventsCLEAN(void);
    void DumpInterruptMsg (const std::string intr_name, const bool value);

    // Overwrite functions of Agent Controller class
    bool GetResetStatus(std::string clock_name);
    bool CheckClockPeriod(std::string clock_name);
    void GetDelayTimer(bool swtrg);
    
    bool ModelinResetStateorZeroClock(std::string domain);
    bool IsSkipTrigger(void);
    void MonitorSkipStartupTriggerSource(bool reset_counter);
    // Handle em3iadsel
    void HandleADSelectSignalMethod(void);
    // Start up trigger source handler
    void HandleCarrierPeakMethod(void);
    void HandleCarrierTroughMethod(void);
    void HandleAngComp0Method(void);
    void HandleADStartTriggerMethod(void);
    void HandleADSamplingPeakMethod(void);
    void HandleADSamplingTroughMethod(void);
    void HandleOutputEdgePortMethod(const unsigned int port_id);
    void HandleOutputDataPortMethod(const unsigned int port_id);
    void HandleAngLatchingRegisterMethod(void);
    void HandleAngLatchingPortMethod(void);
    void HandleIR3UPhaseMethod(void);
    void HandleIR3VPhaseMethod(void);
    void HandleIR3WPhaseMethod(void);
    void HandleSkipStartupTriggerCounterMethod(void);
    bool IsDelayPeriod(void);
    // A/D conversion completion
    void HandleCompletionSignalfromADCMethod(void);
    void HandleCompletionSignalfromTSG3Method(void);
    void HandleADConversionResult(unsigned short AD0, unsigned short AD1, unsigned short AD2);
    void HandleADCompletionMethod(void);
    void SkipADCompletionTrigger(void);
    // Write AD data to memory
    void WriteADDatatoMemory(void);

    void notifyTransComp(TlmTransactionType &trans, BusTime_t &t);
    // Handle IIR output port
    void HandleIIRCompletionMethod(void);
    // Distribution function
    void DistributionADCompletion(void);
    //Handle output port em3iadctr_adend
    void UpdateADCompletionTriggerControlPort(unsigned char adend);
    // Mask trigger control
    bool ADTriggerMaskControl(eMASK_CONTROL mType);
    void ProcessWriteMemoryThread(void);
};
#endif // __HWCORE_ADTRG_H__

