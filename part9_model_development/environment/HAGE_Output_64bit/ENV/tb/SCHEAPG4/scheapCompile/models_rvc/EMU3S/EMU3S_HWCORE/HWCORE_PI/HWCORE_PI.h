// ---------------------------------------------------------------------
// $Id: HWCORE_PI.h,v 1.4 2020/11/08 05:38:30 minhquocha Exp $
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
// Description: Copyright of HWCORE_PI.h
// Ref: {HWCORE_PI_UD_Copyright_001, HWCORE_PI_UD_ATT1_010}
#ifndef __HWCORE_PI_H__
#define __HWCORE_PI_H__
#include "HWCORE_PI_AgentController.h"
#include "CRWInterface.h"
#include "rvc_common_model.h"
#include <vector>

class HWCORE_PI_Func;

// Description: Declaration of HWCORE_PI class
// Ref: {HWCORE_PI_UD_ATT1_004, HWCORE_PI_UD_ATT1_005, HWCORE_PI_UD_BlockDiagram_001, HWCORE_PI_UD_Ports_001, HWCORE_PI_UD_Ports_002, HWCORE_PI_UD_Ports_003, HWCORE_PI_UD_Ports_004, HWCORE_PI_UD_Ports_005, HWCORE_PI_UD_Ports_006, HWCORE_PI_UD_Ports_007, HWCORE_PI_UD_Ports_008, HWCORE_PI_UD_Ports_009}
class HWCORE_PI : public sc_module
    , public HWCORE_PI_AgentController
    , public CRWInterface
    , public rvc_common_model
{
public:
    sc_in<sc_dt::uint64>    PCLK;                 //SSCG clock port
    sc_in<bool>             PRESETn;              //APB reset port
    sc_in<bool>             rst_n;                //SW reset port
    sc_in<bool>             pi_start_sp;          //HWCORE_PI startup trigger
    sc_in<unsigned short>   FUNCIDLEGRPA1;        //PI control IP IDLE start signal
    sc_in<unsigned short>   FUNCWAITGRPA;         //PI control IP WAIT start signal
    sc_in<unsigned char>    FUNCFLGRPA1;          //Transition control after function block completion
    sc_in<unsigned char>    IPTRG;                //PI control IP input data selection
    sc_in<signed int>       rdata_em2iidfixobuf;  //D-axis current value
    sc_in<signed int>       rdata_em2iiqfixobuf;  //Q-axis current value
    sc_in<signed int>       rdata_iiroutdat0;     //IIR filter conversion result channel 0
    sc_in<signed int>       rdata_iiroutdat1;     //IIR filter conversion result channel 1

    sc_out<unsigned char>   pi_func_end;          //PI control IP completion number notification signal
    sc_out<unsigned char>   pi_func_st;           //PI control IP execution number notification signal
    sc_out<bool>            pi_func_wait;         //PI control IP WAIT state
    sc_out<bool>            pi_func_busy;         //PI control IP execution state
    sc_out<signed int>      rdata_em2ivd;         //D-axis voltage value (EMU3nVD)
    sc_out<signed int>      rdata_em2ivq;         //Q-axis voltage value (EMU3nVQ)
    sc_out<signed int>      id_dir_reg;           //D-axis current command value (EMU3nIDIN)
    sc_out<signed int>      iq_dir_reg;           //Q-axis current command value (EMU3nIQIN)
    sc_out<signed int>      id_reg;               //D-axis current value soft input (EMU3nID)
    sc_out<signed int>      iq_reg;               //Q-axis current value soft input (EMU3nIQ)
    sc_out<signed int>      id_sel_out;           //D-axis current value (after input IP and IIR filter selection)
    sc_out<signed int>      iq_sel_out;           //Q-axis current value (after input IP and IIR filter selection)
    sc_out<bool>            pi_end_sp_reg;        //PI control IP completion signal
    sc_out<bool>            pi_ovfl_out;          //PI control IP overflow notification signal
    sc_out<bool>            pi_ovfl_func;         //PI control IP overflow channel

    SC_HAS_PROCESS(HWCORE_PI);
    //Constructor/Destructor
    HWCORE_PI(sc_module_name name);
    ~HWCORE_PI();

    // Virtual functions of CRWInterface class
    void read(unsigned int offsetAddress, unsigned char* data_ptr, unsigned int size, bool debug);
    void write(unsigned int offsetAddress, unsigned char* data_ptr, unsigned int size, bool debug);

    // Python IF
    void SetMessageLevel(const std::string msg_lv);
    void DumpRegisterRW(const std::string is_enable);
    void EnableRegisterMessage(const std::string is_enable);
    void DumpInterrupt (const std::string is_enable);
    void EnableStatInfo(const std::string is_enable);

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
    void ChgProcessingTime(const std::string func_name, const unsigned int cycle);

private:
    // Class HWCORE_PI_Func pointer
    HWCORE_PI_Func            *mpHWCORE_PIFunc;       //pointer of HWCORE_PI_Func

    enum eResetInfo{
        emPRESETnId             = 0
        , emrst_nId             = 1
        , emResetSignalNum      = 2
    };

    //PI control IP signal ports
    enum ePISignalPortsInfo{
        empi_func_end
        , empi_func_st
        , empi_end_sp_reg
        , empi_ovfl_out
        , emSignalPortNum
    };

    //PI control IP data ports
    enum ePIDataPortsInfo{
        emrdata_em2ivd
        , emrdata_em2ivq
        , emid_dir_reg
        , emiq_dir_reg
        , emid_reg
        , emiq_reg
        , emid_sel_out
        , emiq_sel_out
        , empi_func_wait
        , empi_func_busy
        , empi_ovfl_func
        , emDataPortsNum
    };

    //PI control IP function BLOCK info
    enum ePIBlockInfo {
        emBLOCK00   = 0
        , emBLOCK01 = 2
    };

    typedef signed long long   OVFC_S;

    // Variables
    sc_dt::uint64   mPCLKPeriod;                    //Period of PCLK
    sc_dt::uint64   mPCLKOrgFreq;                   //Previous frequency of PCLK
    sc_dt::uint64   mPCLKFreq;                      //Frequency of PCLK
    std::string     mPCLKFreqUnit;                  //Frequency unit of PCLK
    double          mStartPCLKTime;                 // Start time of PCLK

    sc_dt::uint64   mTimeResolutionValue;           //Time resolution value of simulation
    sc_time_unit    mTimeResolutionUnit;            //Time resolution unit of simulation

    //Channel internal coefficient buffer
    bool            mPICTR_FSUMID_buf;
    bool            mPICTR_FSUMIQ_buf;
    signed int      mIDIN_buf;
    signed int      mIQIN_buf;
    signed int      mID_buf;
    signed int      mIQ_buf;
    signed int      mGPD0_buf;
    signed int      mGPQ0_buf;
    signed int      mGPD_buf;
    signed int      mGPQ_buf;
    signed int      mGID_buf;
    signed int      mGIQ_buf;
    unsigned int    mGIDMAX_buf;
    unsigned int    mGIQMAX_buf;
    signed int      mSUMID_buf;
    signed int      mSUMIQ_buf;
    unsigned int    mVDMAX_buf;
    unsigned int    mVQMAX_buf;

    unsigned int    mPI0_cyc;               //pi0 cycle value
    unsigned int    mPI1_cyc;               //pi1 cycle value
    unsigned int    mPI_border_cyc;         //pi border cycle value
    unsigned int    mPI_cyc_sum;            //PI IP total process cycle

    signed int      msum_id;                 //int32_t msum_id
    signed int      msum_iq;                 //int32_t msum_iq
    signed int      mg_n_sumidm;             //int32_t mg_n_sumidm
    signed int      mg_n_sumiqm;             //int32_t mg_n_sumiqm

    bool            mpi_func_wait;          //value for pi_func_wait port
    bool            mpi_func_busy;          //value for pi_func_busy port
    bool            mpi_ovfl_func;          //value for pi_ovfl_func port
    bool            mpi_ovfl_flag;

    signed int      mIDFIXOBUF;             //D-axis current value
    signed int      mIQFIXOBUF;             //Q-axis current value
    signed int      mIIROUTDAT0;            //IIR filter conversion result channel 0
    signed int      mIIROUTDAT1;            //IIR filter conversion result channel 1
    signed int      mIDDIRREG;              //D-axis current command value (EMU3nIDIN)
    signed int      mIQDIRREG;              //Q-axis current command value (EMU3nIQIN)
    signed int      mIDREG;                 //D-axis current value soft input (EMU3nID)
    signed int      mIQREG;                 //Q-axis current value soft input (EMU3nIQ)
    signed int      mIDSELOUT;              //D-axis current value (after input IP and IIR filter selection)
    signed int      mIQSELOUT;              //Q-axis current value (after input IP and IIR filter selection)
    unsigned char   mFUNCFLGRPA1;           //Transition control after function block completion
    bool            mIPTRG_PIIPTRG;         //PI control IP input data selection

    bool            mResetCurVal[emResetSignalNum];         //Store current value of PRESETn or rst_n
    bool            mResetPreVal[emResetSignalNum];         //Store previous value of PRESETn or rst_n
    bool            mIsResetHardActive[emResetSignalNum];   //Reset operation status of PRESETn or rst_n signal
    bool            mIsResetCmdReceive[emResetSignalNum];   //Receive status of AssertReset command of PRESETn or rst_n
    bool            mIsResetCmdActive[emResetSignalNum];    //Active status of AssertReset command of PRESETn or rst_n
    double          mResetCmdPeriod[emResetSignalNum];      //Period of AssertReset command of PRESETn or rst_n
    bool            mEnableRegisterMessage;                 //Enable/disable info/warning/error message of register IF
    bool            mDumpInterrupt;                         //Enable/Disable dumping the interrupt info
    bool            mEnableStatInfo;                        //Enable/Disable dumping the status info
    bool            mPISignal_NextVal;                      //Store next value to be written to PI control IP signal port
    unsigned char   mPIFuncEnd_NextVal;                     //Store next value to be written to pi_func_end port
    unsigned char   mPIFuncSt_NextVal;                      //Store next value to be written to pi_func_st port

    // Reset Events
    sc_event        mResetHardEvent[emResetSignalNum];         //Call HandleResetHardMethod when PRESETn or rst_n is active
    sc_event        mResetCmdEvent[emResetSignalNum];          //Call HandleResetCmdMethod when AssertReset is called
    sc_event        mResetCmdCancelEvent[emResetSignalNum];    //Call CancelResetCmdMethod when AssertReset is over
    // Operation Events
    sc_event        mPI0BlockProcessEvent;                     // Call PI0BlockProcessMethod()
    sc_event        mPI1BlockProcessEvent;                     // Call PI1BlockProcessMethod()
    sc_event        mSumCyclePIIP0Event;                       // Call SumCyclePIIP0Method() to sum Func(pi0) process cycles to total cycle
    sc_event        mSumCyclePIIP1Event;                       // Call SumCyclePIIP1Method() to Sum Func(pi1) process cycles to total cycle
    //Write Port Events
    sc_event        mWritePISignalPortsEvent[emSignalPortNum];              // Call WritePISignalOutPortMethod()
    sc_event        mAssertPISignalPortsEvent[emSignalPortNum];             // Call AssertPISignalOutMethod()
    sc_event        mDeassertPISignalPortsEvent[emSignalPortNum];           // Call DeassertPISignalOutMethod()
    sc_event        mWritePIDataPortsEvent[emDataPortsNum];                 // Call WritePIDataOutPortsMethod()

    // Threads/Methods
    //****************
    // Input handler
    void HandlePCLKSignalMethod(void);
    void HandlePRESETnSignalMethod(void);
    void Handlerst_nSignalMethod(void);

    void PIIP_IDLE_Start_Method(void);
    void PIIP_WAIT_Start_Method(void);
    void HandleFUNCFLGRPA1Method(void);
    void Handle_rdata_em2iidfixobuf_Method(void);
    void Handle_rdata_em2iiqfixobuf_Method(void);
    void Handle_rdata_iiroutdat0_Method(void);
    void Handle_rdata_iiroutdat1_Method(void);
    void Handle_IPTRG_Method(void);

    // Reset handler
    void HandleResetHardMethod(const unsigned int reset_id);
    void HandleResetCmdMethod(const unsigned int reset_id);
    void CancelResetCmdMethod(const unsigned int reset_id);
    void EnableReset(const bool is_active, const unsigned int reset_id);
    void InitializeBuffer(void);
    void InitializeOperation(void);
    void ResetPorts(void);
    void CancelEvents(void);

    // Output handler
    void DumpInterruptMsg(const std::string intr_name, const bool value);
    void WritePIDataOutPortsMethod(const unsigned int port_id);
    void WritePISignalOutPortMethod(const unsigned int port_id);
    void AssertPISignalOutMethod(const unsigned int port_id);
    void DeassertPISignalOutMethod(const unsigned int port_id);

    // PI Control operation
    void PISetBufferRegisters(void);
    void PI0BlockProcessMethod(void);
    void PI1BlockProcessMethod(void);
    void PI1ProcessSetting(void);
    void FuncPI0(void);
    void FuncPI1(signed int in_id, signed int in_iq, signed int *out_vd, signed int *out_vq);
    void SumCyclePIIP0Method(void);
    void SumCyclePIIP1Method(void);
    unsigned int JudgeCycPIIP(unsigned int cyc, bool end_flag);
    bool JudgePIIPEnd(unsigned char shift);
    void EndPIIP(void);
    void SetStateIDLEPIIP(void);
    void SetStateBUSYPIIP(void);
    void SetStateWAITPIIP(void);
    void Chk_overflow_s(signed long long input, bool* ovfflag);

    // Check clock and reset
    bool CheckClockPeriod(void);
    bool GetResetStatus(const std::string reset_name);
    void Callback_IDIN(void);
    void Callback_IQIN(void);
    void Callback_ID(void);
    void Callback_IQ(void);
    void Callback_VD(void);
    void Callback_VQ(void);

};
#endif // __HWCORE_PI_H__
