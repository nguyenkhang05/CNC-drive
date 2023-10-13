// ---------------------------------------------------------------------
// $Id$
//
// Copyright(c) 2012 Renesas Electronics Corporation
// Copyright(c) 2012 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------

#ifndef __SCI3_H__
#define __SCI3_H__
#include "systemc.h"
#include "sci3_data.h"
#include "general_sci.h"
#include "sci3_regif.h"
#include "tlm.h"
#include "tlm_tgt_if.h"
#include "iterator"
#include <sstream>
#include <stdint.h>

//===============Csci3 class=======================
class Csci3: public sc_module 
           ,public Csci3_regif
           ,public vpcl::Cgeneral_sci
           ,public vpcl::tlm_tgt_if<32>
{
#include "sci3_cmdif.h"
private:
    //Declare enum type
    enum eGeneralEnum {
            emChannelIndex = 0          // SCI3 just has 1 channel
           ,emDefaultNumOfRecBuf = 1
    };
    enum eTimeEnum {
            emNanoSecond = 1000000000  
           ,emDeviation = 6             // Deviation of bit time
    };
    enum eDataFormatEnum {
            emSevenBitData = 7          // Seven bit data length
           ,emEightBitData = 8          // Eight bit data length
           ,em16BitsData   = 16         // 16 bits data length
           ,emNoParity     = 0          // Parity bit is disable
           ,emOnePSMBit    = 1          // One Parity/Stop/Multiprocessor bit 
           ,emTwoStopBit   = 2          // Two Stop bit
    };
    enum eInterruptIdEnum {
            emReceiveErrorIntr = 1
           ,emReceiveFullIntr  = 2
           ,emTransmitEmptyIntr= 4
           ,emTransmitEndIntr  = 8
    };
    enum eOperationMode {
            emAsynMode       = 0
           ,emClockedSynMode = 1
    };
    enum eTxControlData {
            emTxControlDefault = 0x00000008
           ,emTxControlIdle    = 0x0000C108
           ,emTxControlBreak   = 0x000000C8
           ,emTxDataDefault    = 0xFFFFFFFF
           ,emTxDataBreak      = 0x00000000
    };
    enum eControlMask {
            emBITTMask      = 0xFFFF0000
           ,emTEMask        = 0x00008000
           ,emTCMask        = 0x00004000
           ,emDIRMask       = 0x00000100
           ,emSTBEMask      = 0x000000C0
           ,emSIZEMask      = 0x0000001F
    };
    enum eDataMask {
            emSevenBitMask       = 0xFFFFFF00
           ,emEightBitMask       = 0xFFFFFE00
           ,em16BitsMask         = 0xFFFE0000
           ,emParitySevenBitMask = 0x80
           ,emParityEightBitMask = 0x100
           ,emParity16BitsMask   = 0x10000
           ,emDataSevenBitMask   = 0x7F
           ,emDataEightBitMask   = 0xFF
           ,emData16BitsMask     = 0xFFFF
    };

    //Declare event
    sc_event mAssertResetEvent;
    sc_event mCmdResetEvent;
    sc_event mCancelCmdResetEvent;
    sc_event mTransmitEvent;
    sc_event mReceiveEvent;
    sc_event mCancelTransmitEvent;
    sc_event mCancelReceiveEvent;
    sc_event mTXIInterruptEvent;
    sc_event mTEIInterruptEvent;
    sc_event mRXIInterruptEvent;
    sc_event mERIInterruptEvent;
    sc_event mWriteSCKOEvent;
    sc_event mWriteTxDEvent;
    sc_event mReadRxDEvent;
    sc_event mCheckBreakEvent;

    //Declare internal variable
    std::string mTransStatus;         // Transmission status
    std::string mRecStatus;           // Reception status
    unsigned int mInterruptKind;
    unsigned int mTransmitDataAmount; // Total transmited data packet
    unsigned int mReceiveDataAmount;  // Total received data packet
    unsigned int mOperationMode;      // Operation mode of SCI3
    unsigned int mBRRVal;             // Value of BRR register
    unsigned int mMDDRVal;            // Value of MDDR register
    unsigned int mTransSeq;           // Sequential number of transmit data packet
    unsigned int mTSRreg;            // Store temp data of TDR
    unsigned int mTxTransNum;         // Store current number of data in SCI
    unsigned int mRDRtemp;            // Store temp data of RDR
    double mSCKO;                     // Value of SCKO port
    double mSCKI;                     // Value of SCKI port
    double mPCLKFreq;                 // Value of pclk frequency
    double mPCLKPeriod;               // Value of pclk period
    double mResetPeriod;              // handleCommand reset period time
    double mFramePeriod;              // Period of transferring a frame (subtract 0.5 etu)
    bool mSCI3Reset;                  // Indicate SCI3 is resetting by preset_n port
    bool mCmdReset;                   // Indicate SCI3 is resetting by handleCommand
    bool mIsCmdResetStatus;           // Indicate handleCommand reset status
    bool mIsSendingBreak;             // Indicate SCI3 is sending break signal
    bool mClearTEI;                   // Indicate TEI interrupt is cleared
    bool mClearERI;                   // Indicate ERI interrupt is cleared
    bool mAssertRXI;                  // Indicate RXI interrupt is asserted
    bool mAssertTXI;                  // Indicate TXI interrupt is asserted
    bool mSSRIsRead;                  // Indicate that SSR register has been read
    bool mIsContTransmit;             // Indicate that SCI3 transmit continuously
    bool mReceiveLatencyFlag;         // Flag used to synchronize reception with transmission
    bool mIsFirstDataTransmit;        // Flag to indicate the first data packet in Clock-synchronous mode (Tx)
    bool mIsFirstDataReceive;         // Flag to indicate the first data packet in Clock-synchronous mode (Rx)
    bool mNewRxData;                    // Flag to indicate the new data is transfered.
    bool mIsTSR;
    bool mIsSlaveTrans;
    unsigned int mCurrentSTBE;        // Store value of STBE from SPI_CONTROL port.
    CSciIF mTransmitDataPacket;       // Transmit data packet
    CSciIF mReceiveDataPacket;        // Receive data packet

    //SCI3 inherited virtual functions
    bool ScifTxData(const unsigned char data,const unsigned int ch_number);
    void Scif_interrupt(const unsigned int interrupt_id,const unsigned int ch_number);

    //SCI3 Thread
    void TransmitThread(void);
    void ReceiveThread(void);

    //SCI3 Method
    void ResetMethod (void);
    void CmdResetMethod(void);
    void CancelCmdResetMethod(void);
    void PCLKPeriodMethod(void);
    void SCKIPeriodMethod(void);
    void CancelBreakMethod(void);
    void WriteSCKOMethod(void);
    void WriteTxDMethod(void);
    void RxControlMethod(void);
    void SPIControlMethod(void);
    void ReadRxDMethod(void);
    void TXIInterruptMethod(void);
    void TEIInterruptMethod(void);
    void RXIInterruptMethod(void);
    void ERIInterruptMethod(void);
    void CheckBreakMethod(void);

    //SCI3 internal functions
    void Initialize(void);
    void SetUpGeneralSCI(void);
    void StartTransmission(void);
    void WriteSCKO(const double value, const bool receiving_flag);
    void IssueRecIntr(const bool error_flag);
    void AssertReset(const double delay,const double period);
    void EnableReset(const bool is_active);
    void NotifyInterruptEvent(const unsigned int interrupt_id);
    void DumpInfo(const char *type, const char *message, ...);
    void DumpStatInfo(void);
    void DumpEnableTransInfo(const bool status);
    void DumpInterruptMsg(const unsigned int interrupt_id);
    void DumpOperationInfo(const char *operation);
    bool FrameChecking(void);
    bool CompareRecDataMatch(void);
    bool CheckWriteRegister(const char *str_name);
    bool GetParity(const unsigned int data);
    double BitRateCalc(void);
    double TransmitProcess(const double period);
    double TransTimeCalc(const double period, const bool receiving_flag);
    unsigned int BitRateCoeffCal(void);
    double GetTimeResolution(void);
    void SetLatency_TLM(const double pclk_period, const bool is_constructor);
    void SetCLKfreq(std::string clk_name, double clk_freq);
    unsigned int TXControlConvert (CSciIF TransmitDataPacket);
    unsigned int TXDataConvert (CSciIF TransmitDataPacket);
    void ProcessReceivedData (CSciIF &ReceiveDataPacket);

    //tlm_tgt_if api functions
    void tgt_acc (tlm::tlm_generic_payload &trans, sc_time &t);
    unsigned int tgt_acc_dbg(tlm::tlm_generic_payload &trans);

    //Register IF callback functions
    void cb_RDR_RDR(RegCBstr str);
    void cb_TDR_TDR(RegCBstr str);
    void cb_SMR_CM(RegCBstr str);
    void cb_SCR_RIE(RegCBstr str);
    void cb_SCR_TE(RegCBstr str);
    void cb_SCR_RE(RegCBstr str);
    void cb_SCR_TEIE(RegCBstr str);
    void cb_SCR_CKE1(RegCBstr str);
    void cb_SCR_CKE0(RegCBstr str);
    void cb_SSR_TDRE(RegCBstr str);
    void cb_SSR_RDRF(RegCBstr str);
    void cb_SSR_ORER(RegCBstr str);
    void cb_BRR_BRR(RegCBstr str);
    void cb_SCMR_SMIF(RegCBstr str);
    void cb_SEMR_BRME(RegCBstr str);

public:
    //Declare input ports
    sc_in<bool>  preset_n;
    sc_in<sc_dt::uint64 > pclk;
    sc_in<sc_dt::uint64 > SCKI;
    sc_in<sc_uint<32> > RX_CONTROL;
    sc_in<sc_uint<32> > RX_DATA;
    sc_in<bool>  PDR;

    //Declare output ports
    sc_out<sc_dt::uint64 > SCKO;
    sc_out<sc_uint<32> > TX_CONTROL;
    sc_out<sc_uint<32> > TX_DATA;
    sc_out<bool>  TEI;
    sc_out<bool>  TXI;
    sc_out<bool>  RXI;
    sc_out<bool>  ERI;
    sc_inout<sc_uint<32> > SPI_CONTROL;

    //Construct and Destruct
    SC_HAS_PROCESS(Csci3);
    Csci3(sc_module_name name);
    ~Csci3();
};
#endif //__SCI3_H__
