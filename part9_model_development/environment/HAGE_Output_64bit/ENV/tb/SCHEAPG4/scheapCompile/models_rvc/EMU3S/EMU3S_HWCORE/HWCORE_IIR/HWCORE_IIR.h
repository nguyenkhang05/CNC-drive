// ---------------------------------------------------------------------
// $Id: HWCORE_IIR.h,v 1.3 2020/10/20 03:29:38 minhquocha Exp $
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
// Description: Copyright of HWCORE_IIR.h
// Ref: {HWCORE_IIR_UD_Copyright_001, HWCORE_IIR_UD_ATT1_010}
#ifndef __HWCORE_IIR_H__
#define __HWCORE_IIR_H__
#include "HWCORE_IIR_AgentController.h"
#include "CRWInterface.h"
#include "rvc_common_model.h"
#include <vector>

class HWCORE_IIR_Func;

// Description: Declaration of HWCORE_IIR class
// Ref: {HWCORE_IIR_UD_ATT1_004, HWCORE_IIR_UD_ATT1_005, HWCORE_IIR_UD_BlockDiagram_001, HWCORE_IIR_UD_Ports_001, HWCORE_IIR_UD_Ports_002, HWCORE_IIR_UD_Ports_003, HWCORE_IIR_UD_Ports_004, HWCORE_IIR_UD_Ports_005, HWCORE_IIR_UD_Ports_006, HWCORE_IIR_UD_Ports_007, HWCORE_IIR_UD_Ports_008, HWCORE_IIR_UD_Ports_009}
class HWCORE_IIR : public sc_module
    , public HWCORE_IIR_AgentController
    , public CRWInterface
    , public rvc_common_model
{
public:
    sc_in<sc_dt::uint64>    PCLK;                 //SSCG clock port
    sc_in<bool>             PRESETn;              //APB reset port
    sc_in<bool>             rst_n;                //SW reset port
    sc_in<bool>             em3ictrinmd_iirdq;    //Input IP IIR filter select
    sc_in<signed int>       rdata_em2iidfixobuf;  //D-axis current value
    sc_in<signed int>       rdata_em2iiqfixobuf;  //Q-axis current value
    sc_in<unsigned short>   em2iadi0;             //AD current value (CH0)
    sc_in<unsigned short>   em2iadi1;             //AD current value (CH1)
    sc_in<unsigned short>   em2iadi2;             //AD current value (CH2)
    sc_in<bool>             adtrg_end_trig;       //A/D conversion completion trigger
    sc_in<bool>             input_end_sp_reg;     //Input IP completion trigger

    sc_out<bool>            iir_end0;       //IIR filter channel 0 completion signal
    sc_out<bool>            iir_end1;       //IIR filter channel 1 completion signal
    sc_out<bool>            iir_end2;       //IIR filter channel 2 completion signal
    sc_out<signed int>      iir_output0;    //IIR filter conversion result channel 0 (EMU3nIIROUTDAT0)
    sc_out<signed int>      iir_output1;    //IIR filter conversion result channel 1 (EMU3nIIROUTDAT1)
    sc_out<signed int>      iir_output2;    //IIR filter conversion result channel 2 (EMU3nIIROUTDAT2)
    sc_out<bool>            iir_ovfl_out;   //IIR Filter overflow occurrence notification signal
    sc_out<unsigned char>   iir_ovfl_func;  //IIR Filter overflow channel

    SC_HAS_PROCESS(HWCORE_IIR);
    //Constructor/Destructor
    HWCORE_IIR(sc_module_name name);
    ~HWCORE_IIR();

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
    void ChgProcessingTime (const unsigned int cycle);

private:
    // Class HWCORE_IIR_Func pointer
    HWCORE_IIR_Func            *mpHWCORE_IIRFunc;       //pointer of HWCORE_IIR_Func

    enum eResetInfo{
        emPRESETnId             = 0
        , emrst_nId             = 1
        , emResetSignalNum      = 2
    };
    
    enum eIIR_STATE{
        emIIR_INACTIVE
        , emIIR_ACTIVE
    };

    enum eIIR_RESERVED{
        emIIR_NO_RESERVE
        , emIIR_RESERVE
    };

    // IIR channel
    enum eIIR_CH{
        emIIR_CH0
        , emIIR_CH1
        , emIIR_CH2
    };

    //IIRDQ bit modes
    enum eIIRDQ{
        emAD    = 0     //AD current mode
        , emDQ  = 1     //dq-axis current mdoe
    };

    // Variables
    sc_dt::uint64   mPCLKPeriod;                    //Period of PCLK
    sc_dt::uint64   mPCLKOrgFreq;                   //Previous frequency of PCLK
    sc_dt::uint64   mPCLKFreq;                      //Frequency of PCLK
    std::string     mPCLKFreqUnit;                  //Frequency unit of PCLK
    double          mStartPCLKTime;                 // Start time of PCLK

    sc_dt::uint64   mTimeResolutionValue;           //Time resolution value of simulation
    sc_time_unit    mTimeResolutionUnit;            //Time resolution unit of simulation

    //Channel 0 internal coefficient buffers
    signed int      mCh0_IIRICOEF;
    signed int      mCh0_IIRCOEF0;
    signed int      mCh0_IIRCOEF1;
    signed int      mCh0_IIRCOEF2;
    signed int      mCh0_IIRCOEF3;
    signed int      mCh0_IIRCOEF4;
    signed int      mCh0_IIROCOEF;

    //Channel 1 internal coefficient buffers
    signed int      mCh1_IIRICOEF;
    signed int      mCh1_IIRCOEF0;
    signed int      mCh1_IIRCOEF1;
    signed int      mCh1_IIRCOEF2;
    signed int      mCh1_IIRCOEF3;
    signed int      mCh1_IIRCOEF4;
    signed int      mCh1_IIROCOEF;

    //Channel 2 internal coefficient buffers
    signed int      mCh2_IIRICOEF;
    signed int      mCh2_IIRCOEF0;
    signed int      mCh2_IIRCOEF1;
    signed int      mCh2_IIRCOEF2;
    signed int      mCh2_IIRCOEF3;
    signed int      mCh2_IIRCOEF4;
    signed int      mCh2_IIROCOEF;

    unsigned char   mIIR_state[3];            /* whether IIR is active */
    unsigned char   mIIR_reserve[3];          /* whether IIR trigger appeared */

    unsigned int    mIIR_cyc;               //Cycle value

    bool            mIIRDQ;            //Input IP IIR filter select
    signed int      mIDFIXOBUF;        //D-axis current value
    signed int      mIQFIXOBUF;        //Q-axis current value
    unsigned short  mAD0;              //AD current value (CH0)
    unsigned short  mAD1;              //AD current value (CH1)
    unsigned short  mAD2;              //AD current value (CH2)

    bool            mResetCurVal[emResetSignalNum];         //Store current value of PRESETn or rst_n
    bool            mResetPreVal[emResetSignalNum];         //Store previous value of PRESETn or rst_n
    bool            mIsResetHardActive[emResetSignalNum];   //Reset operation status of PRESETn or rst_n signal
    bool            mIsResetCmdReceive[emResetSignalNum];   //Receive status of AssertReset command of PRESETn or rst_n
    bool            mIsResetCmdActive[emResetSignalNum];    //Active status of AssertReset command of PRESETn or rst_n
    double          mResetCmdPeriod[emResetSignalNum];      //Period of AssertReset command of PRESETn or rst_n
    bool            mEnableRegisterMessage;                 //Enable/disable info/warning/error message of register IF
    bool            mDumpInterrupt;                         //Enable/Disable dumping the interrupt info
    bool            mEnableStatInfo;                        // Enable/Disable dumping the status info
    bool            mIIREnd0_NextVal;                       //Store next value to be written to iir_end0 port
    bool            mIIREnd1_NextVal;                       //Store next value to be written to iir_end1 port
    bool            mIIREnd2_NextVal;                       //Store next value to be written to iir_end2 port
    bool            mIIROvflOut_NextVal;                    //Store next value to be written to iir_ovfl_out port
    unsigned char   mIIROvflFunc_NextVal;                   //Store next value to be written to iir_ovfl_func port
    bool            mADTRGCurVal;            //Store current value of adtrg_end_trig port
    bool            mADTRGPreVal;            //Store previous value of adtrg_end_trig port
    bool            mInputTRGCurVal;            //Store current value of input_end_sp_reg port
    bool            mInputTRGPreVal;            //Store previous value of input_end_sp_reg port


    // Reset Events
    sc_event        mResetHardEvent[emResetSignalNum];         //Call HandleResetHardMethod when PRESETn or rst_n is active
    sc_event        mResetCmdEvent[emResetSignalNum];          //Call HandleResetCmdMethod when AssertReset is called
    sc_event        mResetCmdCancelEvent[emResetSignalNum];    //Call CancelResetCmdMethod when AssertReset is over
    // Operation Events
    sc_event        mADCCompletionEvent;                    // Call ADCCompletionTriggerMethod() when adtrg_end_trig port is asserted
    sc_event        mInputIPCompletionEvent;                // Call InputIPCompletionTriggerMethod() when input_end_sp_reg port is asserted
    sc_event        mIIRJudgeStartupEvent;                  // Call IIRJudgeStartupMethod() to start the selected channel operation
    sc_event        mIIRCh0StartEvent;                      // Call IIRStartChannel0Method() to start IIR channel 0
    sc_event        mIIRCh1StartEvent;                      // Call IIRStartChannel0Method() to start IIR channel 1
    sc_event        mIIRCh2StartEvent;                      // Call IIRStartChannel0Method() to start IIR channel 2
    //Write Port Events
    sc_event        mWriteIIROverflowOutPortEvent;          // Call WriteIIROverflowOutPortMethod()
    sc_event        mAssertIIROverflowOutPortEvent;         // Call AssertIIROverflowOutPortMethod()
    sc_event        mDeassertIIROverflowOutPortEvent;       // Call DeassertIIROverflowOutPortMethod()
    sc_event        mWriteIIREnd0PortEvent;                 // Call WriteIIREnd0PortMethod()
    sc_event        mAssertIIREnd0PortEvent;                // Call AssertIIREnd0PortMethod()
    sc_event        mDeassertIIREnd0PortEvent;              // Call DeassertIIREnd0PortMethod()
    sc_event        mWriteIIREnd1PortEvent;                 // Call WriteIIREnd1PortMethod()
    sc_event        mAssertIIREnd1PortEvent;                // Call AssertIIREnd1PortMethod()
    sc_event        mDeassertIIREnd1PortEvent;              // Call DeassertIIREnd1PortMethod()
    sc_event        mWriteIIREnd2PortEvent;                 // Call WriteIIREnd2PortMethod()    
    sc_event        mAssertIIREnd2PortEvent;                // Call AssertIIREnd2PortMethod()
    sc_event        mDeassertIIREnd2PortEvent;              // Call DeassertIIREnd2PortMethod()
    sc_event        mResetPortsEvent;                       // Call ResetPortMethod()
    sc_event        mWriteIIROutputPortsEvent;              // Call WriteOutputPortMethod()
    sc_event        mWriteIIROvflFuncPortsEvent;            // Call WriteIIROvflFuncPortMethod()

    // Threads/Methods
    void HandlePCLKSignalMethod(void);
    void HandlePRESETnSignalMethod(void);
    void Handlerst_nSignalMethod(void);

    void Handle_adtrg_end_trig_Method(void);       //Method to handle adtrg_end_trig port input
    void Handle_input_end_sp_reg_Method(void);     //Method to handle input_end_sp_reg port input
    void Handle_em3ictrinmd_iirdq_Method(void);    //Method to handle em3ictrinmd_iirdq port input
    void Handle_rdata_em2iidfixobuf_Method(void);  //Method to handle rdata_em2iidfixobuf port input
    void Handle_rdata_em2iiqfixobuf_Method(void);  //Method to handle rdata_em2iiqfixobuf port input
    void Handle_em2iadi0_Method(void);             //Method to handle em2iadi0 port input
    void Handle_em2iadi1_Method(void);             //Method to handle em2iadi1 port input
    void Handle_em2iadi2_Method(void);             //Method to handle em2iadi2 port input

    void HandleResetHardMethod(const unsigned int reset_id);
    void HandleResetCmdMethod(const unsigned int reset_id);
    void CancelResetCmdMethod(const unsigned int reset_id);

    void EnableReset(const bool is_active, const unsigned int reset_id);
    void InitializeBuffer(void);
    void InitializeOperation(void);
    void ResetPortsMethod(void);
    void CancelEvents(void);
    void DumpInterruptMsg(const std::string intr_name, const bool value);
    void WriteIIROutputPortsMethod(void);
    void WriteIIROvflFuncPortMethod(void);
    void WriteIIREnd0PortMethod(void);
    void AssertIIREnd0Method(void);
    void DeassertIIREnd0Method(void);
    void WriteIIREnd1PortMethod(void);
    void AssertIIREnd1Method(void);
    void DeassertIIREnd1Method(void);
    void WriteIIREnd2PortMethod(void);
    void AssertIIREnd2Method(void);
    void DeassertIIREnd2Method(void);
    void WriteIIROverflowOutPortMethod(void);
    void AssertIIROverflowOutPortMethod(void);
    void DeassertIIROverflowOutPortMethod(void);
    bool Is_HWCORE_IIR_Active(void);
    void ADCCompletionTriggerMethod(void);
    void InputIPCompletionTriggerMethod(void);
    void IIRJudgeStartupMethod(void);
    void IIRStartChannel0Method(void);
    void IIRStartChannel1Method(void);
    void IIRStartChannel2Method(void);
    signed int IIRInputSelection(unsigned char iir_ch);
    void IIROperation(unsigned char channel);
    void Chk_overflow_s(signed long long input, unsigned char channel);

    // Overwrite functions of Agent Controller class
    bool CheckClockPeriod(void);
    bool GetResetStatus(const std::string reset_name);
    void callback_IIRSFT_TRG(void);
    void callback_IIRRLD(void);
    void callback_IIRRSL(void);
};
#endif // __HWCORE_IIR_H__
