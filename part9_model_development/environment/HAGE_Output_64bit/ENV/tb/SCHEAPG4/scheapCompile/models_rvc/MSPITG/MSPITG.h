//---------------------------------------------------------------------
// $Id: MSPITG.h,v 1.5 2020/02/08 08:01:54 ducla Exp $
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
// Description: Copyright of MSPITG.h
// Ref: {MSPITG_DD_Copyright_001}
// Description: About the include header files of rvc_common_model.cpp
// Ref: {MSPITG_DD_ATT1_009}

#ifndef __MSPITG_H__
#define __MSPITG_H__

#include "re_define.h"
#include "BusSlaveBase.h"
#include "MSPITG_AgentController.h"
#include "global.h"
#include "Common.h"
#include "rvc_common_model.h"

#define MAX_DMAMSPI_NUM 30
#define MAX_DTSMSPI_NUM 64
#define MAX_MSPI_UNIT   10
class MSPITG_Func;
class MSPITG: public sc_module
            ,public MSPITG_AgentController
            ,public BusSlaveBase<32, 1>  // number of bus slave
            ,public rvc_common_model
{
public:
    // Clock/Reset
    sc_in<sc_dt::uint64> PCLK;                                        // clock for APB
    sc_in<bool> PRESETn;                                              // reset of domain PCLK

    // input 
    sc_in<bool> INTMSPI0TX[8];
    sc_in<bool> INTMSPI1TX[8];
    sc_in<bool> INTMSPI2TX[8];
    sc_in<bool> INTMSPI3TX[8];
    sc_in<bool> INTMSPI4TX[8];
    sc_in<bool> INTMSPI5TX[4];
    sc_in<bool> INTMSPI6TX[4];
    sc_in<bool> INTMSPI7TX[4];
    sc_in<bool> INTMSPI8TX[4];
    sc_in<bool> INTMSPI9TX[4];
    sc_in<bool> INTMSPI0RX[8];
    sc_in<bool> INTMSPI1RX[8];
    sc_in<bool> INTMSPI2RX[8];
    sc_in<bool> INTMSPI3RX[8];
    sc_in<bool> INTMSPI4RX[8];
    sc_in<bool> INTMSPI5RX[4];
    sc_in<bool> INTMSPI6RX[4];
    sc_in<bool> INTMSPI7RX[4];
    sc_in<bool> INTMSPI8RX[4];
    sc_in<bool> INTMSPI9RX[4];
    sc_in<bool> INTMSPI0FE[8];
    sc_in<bool> INTMSPI1FE[8];
    sc_in<bool> INTMSPI2FE[8];
    sc_in<bool> INTMSPI3FE[8];
    sc_in<bool> INTMSPI4FE[8];
    sc_in<bool> INTMSPI5FE[4];
    sc_in<bool> INTMSPI6FE[4];
    sc_in<bool> INTMSPI7FE[4];
    sc_in<bool> INTMSPI8FE[4];
    sc_in<bool> INTMSPI9FE[4];
    
    //Interrupt output
    sc_out<bool> DMAMSPI[MAX_DMAMSPI_NUM];
    sc_out<bool> DTSMSPI[MAX_DTSMSPI_NUM];

    // TargetSocket
    TlmTargetSocket<32>     *ts; // target socket for MSPITG's registers and memory.

    SC_HAS_PROCESS (MSPITG);
    MSPITG (sc_module_name name, const unsigned int rLatency = 0, const unsigned int wLatency = 0);
    ~MSPITG ();

    // PythonIF
    void SetMessageLevel (const std::string msg_lv);
    void DumpRegisterRW (const std::string is_enable);
    void DumpInterrupt (const std::string is_enable);
    void AssertReset (const std::string reset_name, const double start_time, const double period);
    void SetCLKFreq (const std::string clock_name, const sc_dt::uint64  freq, const std::string unit);
    void GetCLKFreq (const std::string clock_name);
    void ForceRegister (const std::string reg_name, const unsigned int reg_value);
    void ReleaseRegister (const std::string reg_name);
    void WriteRegister (const std::string reg_name, const unsigned int reg_value);
    void ReadRegister (const std::string reg_name);
    void ListRegister (void);
    void Help (const std::string type);
    void EnableRegisterMessage(const std::string is_enable);
    void UpdateMSPITGCTLValue(unsigned int ch_id, unsigned int value);
    void UpdateMSPITGDMAALTValue(unsigned int value);
    void UpdateMSPITGDTSALTValue(unsigned int value);
    
private:
    // Func
    MSPITG_Func     *mpMSPITG_Func;  // Pointer of MSPITG Func
    enum eClockInfo{//Define number of clock signal
        emPCLKId                = 0
        ,emNumClk               = 1
    };
    enum eResetInfo{//Define number of reset signal
        emPRESETnId             = 0
        ,emNumReset             = 1
    };
    enum eUnitInfo{//Define number of unit
         emUnit0             = 0
        ,emUnit1             = 1
        ,emUnit2             = 2
        ,emUnit3             = 3
        ,emUnit4             = 4
        ,emUnit5             = 5
        ,emUnit6             = 6
        ,emUnit7             = 7
        ,emUnit8             = 8
        ,emUnit9             = 9
    };
    enum eChannelInfo{//Define number of Chanel
         emChanel0             = 0
        ,emChanel1             = 1
        ,emChanel2             = 2
        ,emChanel3             = 3
        ,emChanel4             = 4
        ,emChanel5             = 5
        ,emChanel6             = 6
        ,emChanel7             = 7
    };
    enum eDMAInfo{//Define number of DMA
         emDMA0              = 0
        ,emDMA1              = 1
        ,emDMA2              = 2
        ,emDMA3              = 3
        ,emDMA4              = 4
        ,emDMA5              = 5
        ,emDMA6              = 6
        ,emDMA7              = 7
        ,emDMA8              = 8
        ,emDMA9              = 9
        ,emDMA10             = 10
        ,emDMA11             = 11
        ,emDMA12             = 12
        ,emDMA13             = 13
        ,emDMA14             = 14
        ,emDMA15             = 15
        ,emDMA16             = 16
        ,emDMA17             = 17
        ,emDMA18             = 18
        ,emDMA19             = 19
        ,emDMA20             = 20
        ,emDMA21             = 21
        ,emDMA22             = 22
        ,emDMA23             = 23
        ,emDMA24             = 24
        ,emDMA25             = 25
        ,emDMA26             = 26
        ,emDMA27             = 27
        ,emDMA28             = 28
        ,emDMA29             = 29
    };
        enum eDTSInfo{//Define number of Chanel
         emDTS0              = 0
        ,emDTS1              = 1
        ,emDTS2              = 2
        ,emDTS3              = 3
        ,emDTS4              = 4
        ,emDTS5              = 5
        ,emDTS6              = 6
        ,emDTS7              = 7
        ,emDTS8              = 8
        ,emDTS9              = 9
        ,emDTS10             = 10
        ,emDTS11             = 11
        ,emDTS12             = 12
        ,emDTS13             = 13
        ,emDTS14             = 14
        ,emDTS15             = 15
        ,emDTS16             = 16
        ,emDTS17             = 17
        ,emDTS18             = 18
        ,emDTS19             = 19
        ,emDTS20             = 20
        ,emDTS21             = 21
        ,emDTS22             = 22
        ,emDTS23             = 23
        ,emDTS24             = 24
        ,emDTS25             = 25
        ,emDTS26             = 26
        ,emDTS27             = 27
        ,emDTS28             = 28
        ,emDTS29             = 29
        ,emDTS30             = 30
        ,emDTS31             = 31
        ,emDTS32             = 32
        ,emDTS33             = 33
        ,emDTS34             = 34
        ,emDTS35             = 35
        ,emDTS36             = 36
        ,emDTS37             = 37
        ,emDTS38             = 38
        ,emDTS39             = 39
        ,emDTS40             = 40
        ,emDTS41             = 41
        ,emDTS42             = 42
        ,emDTS43             = 43
        ,emDTS44             = 44
        ,emDTS45             = 45
        ,emDTS46             = 46
        ,emDTS47             = 47
        ,emDTS48             = 48
        ,emDTS49             = 49
        ,emDTS50             = 50
        ,emDTS51             = 51
        ,emDTS52             = 52
        ,emDTS53             = 53
        ,emDTS54             = 54
        ,emDTS55             = 55
        ,emDTS56             = 56
        ,emDTS57             = 57
        ,emDTS58             = 58
        ,emDTS59             = 59
        ,emDTS60             = 60
        ,emDTS61             = 61
        ,emDTS62             = 62
        ,emDTS63             = 63
    };
    // Variables
    sc_dt::uint64                       mPCLKPeriod;                    // Period of PCLK
    sc_dt::uint64                       mPCLKOrgFreq;                   // Previous frequency of PCLK
    sc_dt::uint64                       mPCLKFreq;                      // Frequency of PCLK
    std::string                         mPCLKFreqUnit;                  // Frequency unit of PCLK
    double                              mStartPCLKTime;                 // Start time of PCLK

    bool            mResetCurVal;       // Store current value of reset signals
    bool            mResetPreVal;       // Store previous value of reset signals
    bool            mIsResetHardActive; // Is reset by port
    bool            mIsResetCmdReceive; // Is receive cmd reset
    bool            mIsResetCmdActive;  // Is reset by cmd
    double          mResetCmdPeriod;    // Period of cmd reset
    bool            mEnableRegisterMessage;         // Enable/disable info/warning/error message of register IF
    unsigned int    mMSPITGCTL[MAX_MSPI_UNIT];
    unsigned int    mMSPITGDMAALT;
    unsigned int    mMSPITGDTSALT;
    bool            mDMAMSPI[MAX_DMAMSPI_NUM];
    bool            mDTSMSPI[MAX_DTSMSPI_NUM];
    sc_dt::uint64   mTimeResolutionValue;           // Time resolution value of simulation
    sc_time_unit    mTimeResolutionUnit;            // Time resolution unit of simulation
    bool            mDumpInterrupt;                 // Indicate dump info of interrupt or not

    // Events
    sc_event        mResetHardEvent;                // Call HandleResetHardMethod when reset is active
    sc_event        mResetCmdEvent;                 // Call HandleResetCmdMethod when AssertReset is called
    sc_event        mResetCmdCancelEvent;           // Event to cancel reset by command
    
    sc_event mAssertDMAMSPIEvent[MAX_DMAMSPI_NUM];     // Call AssertDMAMSPIMethod to write DMAMSPI port
    sc_event mDeAssertDMAMSPIEvent[MAX_DMAMSPI_NUM];   // Call DeAssertDMAMSPIMethod to write DMAMSPI port
    sc_event mWriteDMAMSPIEvent[MAX_DMAMSPI_NUM];      // Call WriteDMAMSPIMethod to write DMAMSPI port
    
    sc_event mAssertDTSMSPIEvent[MAX_DTSMSPI_NUM];     // Call AssertDTSMSPIMethod to write DTSMSPI port
    sc_event mDeAssertDTSMSPIEvent[MAX_DTSMSPI_NUM];   // Call DeAssertDTSMSPIMethod to write DTSMSPI port
    sc_event mWriteDTSMSPIEvent[MAX_DTSMSPI_NUM];      // Call WriteDTSMSPIMethod to write DTSMSPI port

    //Methods
    //Interrupts methods
    void HandleMSPI0InterruptMethod(unsigned int ch_id);      ///Method to handle interrupt
    void HandleMSPI1InterruptMethod(unsigned int ch_id);      ///Method to handle interrupt
    void HandleMSPI2InterruptMethod(unsigned int ch_id);      ///Method to handle interrupt
    void HandleMSPI3InterruptMethod(unsigned int ch_id);      ///Method to handle interrupt
    void HandleMSPI4InterruptMethod(unsigned int ch_id);      ///Method to handle interrupt
    void HandleMSPI5InterruptMethod(unsigned int ch_id);      ///Method to handle interrupt
    void HandleMSPI6InterruptMethod(unsigned int ch_id);      ///Method to handle interrupt
    void HandleMSPI7InterruptMethod(unsigned int ch_id);      ///Method to handle interrupt
    void HandleMSPI8InterruptMethod(unsigned int ch_id);      ///Method to handle interrupt
    void HandleMSPI9InterruptMethod(unsigned int ch_id);      ///Method to handle interrupt
    
    void WriteDMAMSPIMethod(unsigned int id);
    void AssertDMAMSPIMethod(unsigned int id);
    void DeAssertDMAMSPIMethod(unsigned int id);
    
    void WriteDTSMSPIMethod(unsigned int id);
    void AssertDTSMSPIMethod(unsigned int id);
    void DeAssertDTSMSPIMethod(unsigned int id);
    
    void TriggerTXtoDMA(unsigned int Unit,unsigned int TX_index, unsigned int DMA_index );
    void TriggerTXtoDTS(unsigned int Unit,unsigned int TX_index, unsigned int DTS_index );
    void TriggerRXtoDMA(unsigned int Unit,unsigned int RX_index, unsigned int DMA_index );
    void TriggerRXtoDTS(unsigned int Unit,unsigned int RX_index, unsigned int DTS_index );
    void TriggerFEtoDMA(unsigned int Unit,unsigned int FE_index, unsigned int DMA_index );
    void TriggerFEtoDTS(unsigned int Unit,unsigned int FE_index, unsigned int DTS_index );
    // Communication method

    // Clock handle
    void HandlePCLKSignalMethod (void);
    //Input handle method
    // Reset handle
    void HandlePRESETnSignalMethod (void);
    void HandleResetHardMethod (void);
    void HandleResetCmdMethod (void);
    void CancelResetCmdMethod (void);

    // Internal Functions
    void Initialize (void);
    void EnableReset (const bool is_active);
    void CancelEvents();
    void DumpInterruptMsg (const std::string intr_name, const bool value);
    // Overwrite virtual functions of MSPITG_AgentController
    bool CheckClockPeriod(void);
    bool GetResetStatus(void);
};
#endif //__MSPITG_H__
